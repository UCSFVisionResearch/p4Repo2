/**
 * @file TrialSaccadesStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <memory>
#include <fstream>
#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "SaccadeTrial.h"
#include "TrialSaccades.h"
#include "TrialSaccadesStimulus.h"
#include "utils/Utils.h"
#include "utils/Asserts.h"
#include "utils/Math.h"
#include "vsgext/vsgext.h"
#include "sys/sys_concurrency.h"

/*****************************************************************************
 * Defines
 *****************************************************************************/

/*** Communications **************************************/
#define A_OUT_CTR_TARG_HORIZ  (0) /* 1401 in channel 3 */
#define A_OUT_CTR_TARG_VERT   (1) /* 1401 in channel 4 */
#define A_OUT_PER_TARG_HORIZ  (2) /* 1401 in channel 7 */
#define A_OUT_PER_TARG_VERT   (3) /* 1401 in channel 8 */
#define A_OUT_CTR_TARG_COLOR  (4) /* VOLTS_COLOR_xxx */
#define A_OUT_PER_TARG_COLOR  (5) /* VOLTS_COLOR_xxx */

#define D_OUT_LOW_FOR_CTR      (vsgDIG0) // Complex multi-state w/vsgDIG2
#define D_OUT_BEHAV_CONT_RESET (vsgDIG1) // VSG pin 2; 1401 pin 18; 1401 dig in 6
#define D_OUT_LOW_FOR_ALT      (vsgDIG2) // Complex multi-state w/vsgDIG0
#define D_OUT_FEED             (vsgDIG7) // 0 = Don't feed.  1 = Feed

/*
 * Complex multi-state handling of vsgDIG0 and vsgDIG2
 *
 *   vsgDIG0 | vsgDIG2 | Meaning
 *  ---------+---------+-------------------------------------------
 *     0     |    0    | Center target either eye
 *     0     |    1    | Center target correct eye
 *     1     |    0    | Calibration (set by calibration stimulus)
 *     1     |    1    | Saccade target
 *
 * There weren't enough lines connected to do this cleanly.
 */

/** 
 * @brief Fixation: 1 = broken fixation, abort trial; 0 = all clear.
 *
 * DAQER pulls line low when subject acquires target (center or peripheral).
 * DAQER drives line high when subject is off target fixation.  
 * DisplayStimulus resets the center fixation timer if subject breaks fixation 
 * during cross display.  For saccade target display, subject must acquire the
 * target by the end of the period and must hold it once acquired to be
 * rewarded; otherwise, he receives a timeout.
 */
#define D_IN_NEXT_STIM  (vsgDIG0)
							           
/**
 * @brief Status of saccade; 0 = subject has not fixated on the saccade target 
 * yet; 1 = subject has acquired the saccade target.
 *
 * DAQER pulls this line low at the beginning of a trial.  Once
 * DisplayStimulus drives the D_OUT_LOW_FOR_CTR line high, DAQER monitors
 * fixation with respect to the saccade target.  Once the subject has fixated
 * on the saccade target, DAQER drives this line high for the rest of the 
 * trial.  DisplayStimulus requires this line have been driven high by the end 
 * of the saccade target period and that the subject have maintained fixation 
 * once this line is driven high until the end of the period.
 */
#define D_IN_TARGET_ACQUIRED  (vsgDIG1)

#define VOLTS_COLOR_RED     (5)
#define VOLTS_COLOR_BLUE   (-5)
#define VOLTS_COLOR_PURPLE  (0)

/*** LUT Levels ******************************************/
#define LEVEL_CTR_TARG  (0)
#define LEVEL_PER_TARG  (1)
#define LEVEL_BG_CONTR  (2) // Reserve 2-101 and 102-201
#define LEVEL_TIME_OUT                    (202)
#define LEVEL_PRIMARY                     (203)
#define LEVEL_SECONDARY                   (204)
#define LEVEL_TERTIARY                    (205)
#define LEVEL_PRIMARY_PLUS_NOISE          (206)
#define LEVEL_SECONDARY_PLUS_NOISE        (207)
#define LEVEL_PRIMARY_PLUS_SECONDARY      (208)
#define LEVEL_DIM_PRIMARY                 (209)
#define LEVEL_DIM_SECONDARY               (210)
#define LEVEL_NOISE_PLUS_DIM_PRIMARY      (212)
#define LEVEL_NOISE_PLUS_DIM_SECONDARY    (213)


/** Page Numbers ******************************************
 **  Our only offers four video pages. ********************
 **********************************************************/
#define DRAW_PAGE_CROSS_ONLY (0)
#define DRAW_PAGE_CROSS_DOT  (1)
#define DRAW_PAGE_DOT_ONLY   (2)
#define DRAW_PAGE_REF_BG     (3)

/**
 * @brief Choose left, center, or right based on predicates.
 *
 * @param predA Predicate of outer ternary.
 * @param predB Predicate of inner ternary, evaluated if predA is false.
 * @param left Value of outer ternary if predA is true.
 * @param center Value of inner ternary if predB is true, evaluated if
 *        predA is false.
 * @param right Value of inner ternary if predB is false, evaluated if
 *        predA is false.
 */
#define NESTED_TERNARY(predA, predB, left, center, right)  \
	((predA) ? (left) : ((predB) ? (center) : (right)))

/**
 * @brief Optionally display the string message on screen.
 * 
 * @param display Whether to display the message.
 * @param msg String literal message.
 */
#define DEBUG_DISPLAY(display, msg)  \
	if (display) {                   \
		vsgSetPen1(LEVEL_CTR_TARG);  \
		vsgSetPen2(LEVEL_BG_CONTR);  \
		vsgDrawString(0, -300, msg); \
	}

/**********************************************************
 * Conventions in effect
 *
 * Text label coding:
 * code		center		periph
 * 0x0		pri			pri
 * 0x1		pri			sec
 * 0x2		sec			pri
 * 0x3		sec			sec
 *
 * 0x4		pri			ter
 * 0x5		sec			ter
 * 
 * 0x6		pri			blank
 * 0x7		sec			blank
 *
 * primary    =  red    =  right
 * secondary  =  blue   =  left
 * tertiary   =  purple
 *********************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/**
 * @brief Constructor.
 */
CTrialSaccadesStimulus::CTrialSaccadesStimulus()
: m_bgColor(m_lutbuf[LEVEL_BG_CONTR]), m_noiseColor(m_lutbuf[LEVEL_BG_CONTR+100]),
  m_noiseBgController(NULL), m_noiseColorMap(NULL), m_primarySpec(NULL), 
  m_secondarySpec(NULL), m_tertiarySpec(NULL), m_verticalGrating(NULL), 
  m_horizontalGrating(NULL)
{
	size_t numBytes; 

	Initialize();

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	m_screenWidthPix = vsgGetScreenWidthPixels();
	m_screenHeightPix = vsgGetScreenHeightPixels();
	
	m_currentPage = 0;
	m_lastTrialNoise = true;
	m_useNoise = true;
	m_flashTimer.StartTimer();
	m_stimCenterXDeg = 0.0; 
	m_stimCenterYDeg = 0.0; 
	m_stimPeriphXDeg = 0.0;
	m_stimPeriphYDeg = 0.0;

	srand((unsigned int)time(NULL));
	
	vsgIOWriteDigitalOut(0x0000, D_OUT_LOW_FOR_CTR);
	vsgIOWriteDigitalOut(0x0000, D_OUT_BEHAV_CONT_RESET);
	vsgIOWriteDigitalOut(0xFFFF, D_OUT_LOW_FOR_ALT);
	vsgIOWriteDigitalOut(0x0000, D_OUT_FEED);

	/* Set the drawing mode, and set the origin to the center of the screen */
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(m_screenWidthPix / 2, m_screenHeightPix / 2);

	memset(m_lutbuf, 0, 256 * sizeof(VSGTRIVAL));
	vsgSetBackgroundColour(&m_lutbuf[vsgBACKGROUND]);
	vsgSetFixationColour(&m_lutbuf[vsgFIXATION]);
	vsgLUTBUFFERWrite(0, &m_lutbuf);
	vsgLUTBUFFERtoPalette(0);

	g_pTrialSaccades->sendSettings(g_pSettings->theSerialComm);
	
	{
		SmartLock trialSaccadesLock(*g_pTrialSaccades);

		/* New sequence every time we run */
		g_pTrialSaccades->GenerateTrials();
	}

	/* Populate colors generally; per-trial colors are populated later. */
	InitLutBuffer();
	InitNoiseHandling();
	InitGratings();

	ETU_DEBUG_BLOCK {
		std::ofstream fout("c:\\temp\\etdebuglog.txt", std::ios::app);
		TrialPtrContainer_t::const_iterator iter = g_pTrialSaccades->getTrials().begin();
		TrialPtrContainer_t::const_iterator end = g_pTrialSaccades->getTrials().end();
		const CSaccadeTrial* current;
		for (int ix = 1; iter != end; ++iter, ++ix) {
			current = *iter;
			fout << ix << ":\t" << (*current);
		}
		fout.close();
	}
}

/**
 * @brief Destructor
 */
CTrialSaccadesStimulus::~CTrialSaccadesStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, D_OUT_LOW_FOR_CTR);
	vsgIOWriteDigitalOut(0x0000, D_OUT_BEHAV_CONT_RESET);
	vsgIOWriteDigitalOut(0x0000, D_OUT_LOW_FOR_ALT);
	vsgIOWriteDigitalOut(0x0000, D_OUT_FEED);

	CleanUpNoiseHandling();
	CleanUpGratings();

	etu::AnalogOut(A_OUT_CTR_TARG_COLOR, (float)0.0, true);
	
	/* Send stop mark */
	char buffer[80];
	sprintf(buffer, "Saccade Monkey;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}

/**
 * @brief Set colors in member vsgLUTBUFFER for targets, background, and 
 * gratings.
 */
void CTrialSaccadesStimulus::InitLutBuffer() 
{
	double contrast = (double)g_pTrialSaccades->getDotGratingContrast();

	/* For use here and later with BG Controller (if needed) */
	m_bgColor = etu::RgbToTrival(g_pSettings->getRedBG(), 
		g_pSettings->getGreenBG(), g_pSettings->getBlueBG());
	m_noiseColor = etu::RgbToTrival(g_pTrialSaccades->getNoiseRed(), 
		g_pTrialSaccades->getNoiseGreen(), g_pTrialSaccades->getNoiseBlue());

	/* Time out punishments are a blank screen. */
	m_lutbuf[LEVEL_TIME_OUT] = etu::RgbToTrival(0, 0, 0);
	
	/* Save copies of all periph colors */
	m_lutbuf[LEVEL_PRIMARY] = etu::RgbToTrival(
		g_pTrialSaccades->getPrimaryRed(), 
		g_pTrialSaccades->getPrimaryGreen(),
		g_pTrialSaccades->getPrimaryBlue());
	m_lutbuf[LEVEL_SECONDARY] = etu::RgbToTrival(
		g_pTrialSaccades->getSecondaryRed(), 
		g_pTrialSaccades->getSecondaryGreen(),
		g_pTrialSaccades->getSecondaryBlue());
	m_lutbuf[LEVEL_TERTIARY] = etu::RgbToTrival(
		g_pTrialSaccades->getTertiaryRed(),
		g_pTrialSaccades->getTertiaryGreen(),
		g_pTrialSaccades->getTertiaryBlue());

	m_lutbuf[LEVEL_PRIMARY_PLUS_NOISE].a = m_lutbuf[LEVEL_PRIMARY].a;
	m_lutbuf[LEVEL_PRIMARY_PLUS_NOISE].b = 0.0f;
	m_lutbuf[LEVEL_PRIMARY_PLUS_NOISE].c = m_noiseColor.c;

	m_lutbuf[LEVEL_SECONDARY_PLUS_NOISE].a = m_noiseColor.a;
	m_lutbuf[LEVEL_SECONDARY_PLUS_NOISE].b = 0.0f;
	m_lutbuf[LEVEL_SECONDARY_PLUS_NOISE].c = m_lutbuf[LEVEL_SECONDARY].c;

	m_lutbuf[LEVEL_PRIMARY_PLUS_SECONDARY].a = m_lutbuf[LEVEL_PRIMARY].a;
	m_lutbuf[LEVEL_PRIMARY_PLUS_SECONDARY].b = 0.0f;
	m_lutbuf[LEVEL_PRIMARY_PLUS_SECONDARY].c = m_lutbuf[LEVEL_SECONDARY].c;

	m_lutbuf[LEVEL_DIM_PRIMARY].a = VsgExt::getLowlight(
		m_lutbuf[LEVEL_PRIMARY].a, contrast);
	m_lutbuf[LEVEL_DIM_PRIMARY].b = 0.0f;
	m_lutbuf[LEVEL_DIM_PRIMARY].c = 0.0f;

	m_lutbuf[LEVEL_DIM_SECONDARY].a = 0.0f;
	m_lutbuf[LEVEL_DIM_SECONDARY].b = 0.0f;
	m_lutbuf[LEVEL_DIM_SECONDARY].c = VsgExt::getLowlight(
		m_lutbuf[LEVEL_SECONDARY].c, contrast);

	m_lutbuf[LEVEL_NOISE_PLUS_DIM_PRIMARY].a = m_lutbuf[LEVEL_DIM_PRIMARY].a;
	m_lutbuf[LEVEL_NOISE_PLUS_DIM_PRIMARY].b = 0.0f;
	m_lutbuf[LEVEL_NOISE_PLUS_DIM_PRIMARY].c = m_noiseColor.c;

	m_lutbuf[LEVEL_NOISE_PLUS_DIM_SECONDARY].a = m_noiseColor.a;
	m_lutbuf[LEVEL_NOISE_PLUS_DIM_SECONDARY].b = 0.0f;
	m_lutbuf[LEVEL_NOISE_PLUS_DIM_SECONDARY].c = m_lutbuf[LEVEL_DIM_SECONDARY].c;
}

/**
 * @brief Allocate the NoiseBgController and NoiseColorMap, and initialize
 * them.
 */
void CTrialSaccadesStimulus::InitNoiseHandling()
{
	int size = g_pTrialSaccades->getNoiseSize();
	int percent = g_pTrialSaccades->getNoisePct();

	CleanUpNoiseHandling();

	m_noiseBgController = new VsgExt::NoiseBgController(size, m_bgColor, 
		m_noiseColor, LEVEL_BG_CONTR, percent, m_lutbuf);

	m_noiseColorMap = new VsgExt::NoiseColorMap;
	m_noiseColorMap->map(LEVEL_PRIMARY, LEVEL_PRIMARY_PLUS_NOISE);
	m_noiseColorMap->map(LEVEL_SECONDARY, LEVEL_SECONDARY_PLUS_NOISE);
	m_noiseColorMap->map(LEVEL_DIM_PRIMARY, LEVEL_NOISE_PLUS_DIM_PRIMARY);
	m_noiseColorMap->map(LEVEL_DIM_SECONDARY, LEVEL_NOISE_PLUS_DIM_SECONDARY);
	m_noiseColorMap->map(LEVEL_TERTIARY, LEVEL_TERTIARY);
}

/**
 * @brief Deletes noise handling objects.
 */
void CTrialSaccadesStimulus::CleanUpNoiseHandling()
{
	if (m_noiseBgController) {
		delete m_noiseBgController;
		m_noiseBgController = NULL;
	}
	
	if (m_noiseColorMap) {
		delete m_noiseColorMap;
		m_noiseColorMap = NULL;
	}
}

/**
 * @brief Initializes grating handlers.
 */
void CTrialSaccadesStimulus::InitGratings()
{
	const int cycles = (int)g_pTrialSaccades->getDotGratingCycles();

	CleanUpGratings();

	m_primarySpec = VsgExt::IGrating::identity(LEVEL_PRIMARY);
	m_secondarySpec = VsgExt::IGrating::identity(LEVEL_SECONDARY);
	m_tertiarySpec = VsgExt::IGrating::identity(LEVEL_TERTIARY);

	m_verticalGrating = VsgExt::IGrating::gratingVertical(LEVEL_PRIMARY,
		LEVEL_DIM_PRIMARY, cycles);
	m_horizontalGrating = VsgExt::IGrating::gratingHorizontal(LEVEL_SECONDARY,
		LEVEL_DIM_SECONDARY, cycles);
}

/**
 * @brief Deletes grating objecgts.
 */
void CTrialSaccadesStimulus::CleanUpGratings()
{
	if (m_primarySpec) {
		delete m_primarySpec;
		m_primarySpec = NULL;
	}

	if (m_secondarySpec) {
		delete m_secondarySpec;
		m_secondarySpec = NULL;
	}
	
	if (m_tertiarySpec) {
		delete m_tertiarySpec;
		m_tertiarySpec = NULL;
	}

	if (m_verticalGrating) {
		delete m_verticalGrating;
		m_verticalGrating = NULL;
	}

	if (m_horizontalGrating) {
		delete m_horizontalGrating;
		m_horizontalGrating = NULL;
	}
}

/**
 * @brief Configure LEVEL_CTR_TARG and LEVEL_PER_TARG for this trial.
 * @param currentTrial Reference to current CSaccadeTrial for access to
 * isCenterRed, isPeriphRed, isPeriphBlue, etc.
 */
void CTrialSaccadesStimulus::SetupBufferForTrial(CSaccadeTrial& currentTrial)
{
	VSGTRIVAL& centerColor = m_lutbuf[LEVEL_CTR_TARG];
	VSGTRIVAL& periphColor = m_lutbuf[LEVEL_PER_TARG];
	
	if (currentTrial.isCenterRed()) {
		centerColor = m_lutbuf[LEVEL_PRIMARY];
	} 
	else if (currentTrial.isCenterBlue()) {
		centerColor = m_lutbuf[LEVEL_SECONDARY];
	}
	else {
		centerColor = m_lutbuf[LEVEL_TERTIARY];
	}

	if (currentTrial.isPeriphRed()) {
		periphColor = m_lutbuf[LEVEL_PRIMARY];
	} 
	else if (currentTrial.isPeriphBlue()) {
		periphColor = m_lutbuf[LEVEL_SECONDARY];
	}
	else {
		periphColor = m_lutbuf[LEVEL_TERTIARY];
	}
}

/** 
 * @brief Paints the stimuli in advance of the trial.
 *
 * @param currentTrial Reference to current trial object.
 * @param offset True to offset red or blue cross; false otherwise. Validity
 *               is not checked.
 */
void CTrialSaccadesStimulus::SetupStimsForTrial(CSaccadeTrial& currentTrial,
												bool offset)
{
	/* Set up the LUTBUFFER with per-trial colors. */
	SetupBufferForTrial(currentTrial);
	vsgLUTBUFFERWrite(0, &m_lutbuf);
	vsgLUTBUFFERtoPalette(0);
	
	vsgSetBackgroundColour(&m_bgColor);
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);
	vsgSetDrawMode(vsgCENTREXY);

	/* Used for drawing the same BG throughout a trial. */
	m_useNoise = g_pTrialSaccades->getUseNoise();
	if (g_pTrialSaccades->getInterleaveNoise()) {
		m_lastTrialNoise = !m_lastTrialNoise;
		m_useNoise = m_lastTrialNoise;
	}
	m_noiseBgController->initReferenceBg(vsgVIDEOPAGE, DRAW_PAGE_REF_BG);

	/* Get center location. */
	m_stimCenterXDeg = currentTrial.getCtrXDeg();
	m_stimCenterYDeg = currentTrial.getCtrYDeg();

	/**
	 * @todo Hack
	 * This is the relocated hack to generate eccentric fixation cross 
	 * locations. Perform this eccentric offset without checking any
	 * conditions (which is the functionality we advertise).
	 *
	 * This is _not_ the same as a grid for the center fixation cross, which
	 * is handled quite nicely without a hack.
	 */
	if (offset) {
		double deviation = g_pEyeTrackingDlg->avgrx - g_pEyeTrackingDlg->avglx;
		if (currentTrial.isCenterRed()) {
			m_stimCenterXDeg = deviation;
		}
		else {
			m_stimCenterXDeg = -1 * deviation;
		}
	}

	/* Add stim offset to the stored value, as this location is reported via 
	 * AnalogOut.*/
	m_stimCenterXDeg += g_pSettings->getXOffset();
	m_stimCenterYDeg += g_pSettings->getYOffset();

	/* Add stim offset to the stored peripheral target location. Note we might
	 * defer drawing it, but the overhead here happens before the trial and
	 * doesn't cost us any performance. */
	m_stimPeriphXDeg = currentTrial.getPerXDeg() + g_pSettings->getXOffset();
	m_stimPeriphYDeg = currentTrial.getPerYDeg() + g_pSettings->getYOffset();

	int radius = radiusToCode(currentTrial.getPerRDeg(), 
		g_pTrialSaccades->getRadiusOffset());
	int angle = angleToCode(currentTrial.getPerADeg(), 
		g_pTrialSaccades->getAngleOffset());
	int code = ((currentTrial.getStimEye() << 24) | (radius << 16) | 
		(angle << 8) | currentTrial.getStimCode());
	
	m_tagString.Format("cRGB: %.2f, %.2f, %.2f pRGB: %.2f, %.2f, %.2f|0x%x/\0", 
		m_lutbuf[LEVEL_CTR_TARG].a, m_lutbuf[LEVEL_CTR_TARG].b, m_lutbuf[LEVEL_CTR_TARG].c, 
		m_lutbuf[LEVEL_PER_TARG].a, m_lutbuf[LEVEL_PER_TARG].b, m_lutbuf[LEVEL_PER_TARG].c, 
		code);
	
	unsigned int byte1 = code & 0x000000FF;
	unsigned int byte2 = (code & 0x0000FF00) >> 8;
	unsigned int byte3 = (code & 0x00FF0000) >> 16;
	unsigned int byte4 = (code & 0xFF000000) >> 24;

	m_msgString.Format( \
		  "  Stim:\t\t0x%.2x %s\n" \
		  "  Angle:\t\t0x%.2x \n" \
		  "  Radius:\t0x%.2x \n" \
		  "  Eye:\t\t0x%.2x\n", 
		byte1, stimCodeToString(currentTrial.getStimCode()), 
		byte2, byte3, byte4);

	g_pTrialSaccadesDlg->SetInfoText((LPCTSTR)m_msgString);

	/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 * Cross only page
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
	vsgSetDrawPage(vsgVIDEOPAGE, DRAW_PAGE_CROSS_ONLY, vsgBACKGROUND);
	if (m_useNoise && g_pTrialSaccades->getSameBG()) {
		m_noiseBgController->drawReferenceBg();
	}
	else if (m_useNoise) {
		m_noiseBgController->drawBg();
	}
	DrawCrossWithBG(m_stimCenterXDeg, m_stimCenterYDeg, currentTrial);

	/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 * Page w/cross + dot. Dot drawing might be deferred.
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
	vsgSetDrawPage(vsgVIDEOPAGE, DRAW_PAGE_CROSS_DOT, vsgBACKGROUND);
	if (m_useNoise && g_pTrialSaccades->getSameBG()) {
		m_noiseBgController->drawReferenceBg();
	}
	else if (m_useNoise) {
		m_noiseBgController->drawBg();
	}
	DrawCrossWithBG(m_stimCenterXDeg, m_stimCenterYDeg, currentTrial);
	/** 
	 * @TODO HACK ALERT: See other hack in the PresentStimCrossDot and
	 * PresentStimDotOnly functions.
	 */
	if ((g_pTrialSaccades->getEyeCoverage() == CTrialSaccades::COVERAGE_SPATIOTOPIC) ||
		g_pTrialSaccades->getUseApprox())
	{
		DrawPeripheralStim(m_stimPeriphXDeg, m_stimPeriphYDeg, currentTrial);
	}

	/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 * Page w/dot only. Dot drawing might be deferred.
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
	vsgSetDrawPage(vsgVIDEOPAGE, DRAW_PAGE_CROSS_DOT, vsgBACKGROUND);
	if (m_useNoise && g_pTrialSaccades->getSameBG()) {
		m_noiseBgController->drawReferenceBg();
	}
	else if (m_useNoise) {
		m_noiseBgController->drawBg();
	}
	/** 
	 * @TODO HACK ALERT: See other hack in the PresentStimCrossDot and
	 * PresentStimDotOnly functions.
	 */
	if ((g_pTrialSaccades->getEyeCoverage() == CTrialSaccades::COVERAGE_SPATIOTOPIC) ||
		g_pTrialSaccades->getUseApprox())
	{
		DrawPeripheralStim(m_stimPeriphXDeg, m_stimPeriphYDeg, currentTrial);
	}
}

/**
 * @brief Display page with only the cross.
 * 
 * Display the stimulus page showing only the cross at the previously 
 * configured location. Broadcast the coordinates of the cross only, as it is
 * the only stimulus displayed. 
 *
 * It is the _CALLER's_ responsibility to broadcast which target -- central 
 * cross or peripheral dot -- must be fixated.
 *
 * @param currentTrial Reference to the current trial object.
 */
void CTrialSaccadesStimulus::PresentStimCrossOnly(CSaccadeTrial& currentTrial)
{
	vsgSetDrawPage(vsgVIDEOPAGE, DRAW_PAGE_CROSS_ONLY, vsgNOCLEAR);
	vsgPresent();
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)(m_stimCenterXDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)(m_stimCenterYDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)-9.95);
	etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)-9.95);
}

/**
 * @brief Display page with cross and dot.
 * 
 * Display the stimulus page showing the cross and the peripheral dot at the
 * configured locations. Broadcast the coordinates of the cross and the dot to 
 * DAQER/DAQ2 via analog out lines, as both are displayed.
 *
 * It is the _CALLER's_ responsibility to broadcast which target -- central 
 * cross or peripheral dot -- must be fixated.
 *
 * @param currentTrial Reference to the current trial object.
 */
void CTrialSaccadesStimulus::PresentStimCrossDot(CSaccadeTrial& currentTrial)
{
	vsgSetDrawPage(vsgVIDEOPAGE, DRAW_PAGE_CROSS_DOT, vsgNOCLEAR);
	if ((g_pTrialSaccades->getEyeCoverage() == CTrialSaccades::COVERAGE_RETINOTOPIC) &&
		!g_pTrialSaccades->getUseApprox())
	{
		/** 
		 * @todo HACK ALERT
		 * We deferred drawing the dot so it could be updated w/eye position;
		 * draw it from saved values. 
		 */
		DrawPeripheralStim(m_stimPeriphXDeg, m_stimPeriphYDeg, currentTrial);
	}
	vsgPresent();
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)(m_stimCenterXDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)(m_stimCenterYDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)(m_stimPeriphXDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)(m_stimPeriphYDeg / g_pSettings->getDegPerVolt()));
	if (!m_tagString.IsEmpty()) {
		g_pSettings->theSerialComm.transmit((LPCTSTR)m_tagString, m_tagString.GetLength());
		m_tagString.Empty();
	}
}

/**
 * @brief Display page with dot only.
 * 
 * Display the stimulus page showing the dot only at the configured locations. 
 * Broadcast the coordinates of only the dot to DAQER/DAQ2 via analog out 
 * lines, as it is the only stimulus displayed.
 *
 * It is the _CALLER's_ responsibility to broadcast which target -- central 
 * cross or peripheral dot -- must be fixated.
 *
 * @param currentTrial Reference to the current trial object.
 */
void CTrialSaccadesStimulus::PresentStimDotOnly(CSaccadeTrial& currentTrial)
{
	vsgSetDrawPage(vsgVIDEOPAGE, DRAW_PAGE_DOT_ONLY, vsgNOCLEAR);
	if ((g_pTrialSaccades->getEyeCoverage() == CTrialSaccades::COVERAGE_RETINOTOPIC) &&
		!g_pTrialSaccades->getUseApprox())
	{
		/** 
		 * @todo HACK ALERT
		 * We deferred drawing the dot so it could be updated w/eye position;
		 * draw it from saved values. 
		 */
		DrawPeripheralStim(m_stimPeriphXDeg, m_stimPeriphYDeg, currentTrial);
	}
	vsgPresent();
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)-9.95);
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)-9.95);
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)(m_stimPeriphXDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)(m_stimPeriphYDeg / g_pSettings->getDegPerVolt()));
	if (!m_tagString.IsEmpty()) {
		g_pSettings->theSerialComm.transmit((LPCTSTR)m_tagString, m_tagString.GetLength());
		m_tagString.Empty();
	}
}

/**
 * @brief Display page w/no stim, possibly baited.
 * 
 * Display the stimulus page showing nothing but the background. Conditionally
 * broadcast the coordinates of the dot to DAQER/DAQ2 via analog out lines.
 *
 * It is the _CALLER's_ responsibility to broadcast which target -- central 
 * cross or peripheral dot -- must be fixated.
 * 
 * @param currentTrial Reference to the current trial object.
 * @param baited True to broadcast the coordinates of the peripheral dot; 
 *        false otherwise.
 */
void CTrialSaccadesStimulus::PresentStimBgOnly(CSaccadeTrial& currentTrial, bool baited)
{
	vsgSetDrawPage(vsgVIDEOPAGE, DRAW_PAGE_REF_BG, vsgNOCLEAR);
	vsgPresent();
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)-9.95);
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)-9.95);
	if (baited) {
		etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)(m_stimPeriphXDeg / g_pSettings->getDegPerVolt()));
		etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)(m_stimPeriphYDeg / g_pSettings->getDegPerVolt()));
	}
	else {
		etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)-9.95);
		etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)-9.95);
	}
}

/**
 * @brief Present a blank page.
 * 
 * Present a timeout page as punishment for misbehavior. Monkeys require an
 * identifiable non-stimulus page, usually blank (black, no noise background).
 * Humans do not require this feature, and it is a distraction during human
 * testing. Detect the requested behavior.
 *
 * WARNING: This clobbers the cross page for a blank screen. This function
 * should only be called when you're done a trial anyway.
 * 
 * @param currentTrial Reference to the current trial object.
 */
void CTrialSaccadesStimulus::PresentStimTimeout(CSaccadeTrial& currentTrial)
{
	if (g_pTrialSaccades->getBlankTimeOutScreen()) {
		vsgSetDrawPage(vsgVIDEOPAGE, DRAW_PAGE_CROSS_ONLY, vsgBACKGROUND);
	}
	else {
		vsgSetDrawPage(vsgVIDEOPAGE, DRAW_PAGE_REF_BG, vsgNOCLEAR);
	}

	vsgPresent();
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)-9.95);
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)-9.95);
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)-9.95);
	etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)-9.95);
}

/**
 * @brief Display the stimulus, trial by trial.
 *
 * Display the next trial in the Trial Saccades stimulus. The g_pTrialSaccades 
 * object holds the container of trials along with the current position within 
 * the container. Repeated calls to DisplayStimulus() cause each trial to be 
 * displayed in turn.
 *
 * This function was significantly re-written in October/November, 2014, to
 * use pre-drawn stimulus pages wherever possible. The hope was to improve 
 * timing accuracy by reducing overhead.
 *
 * @return true if stimulus was displayed "successfully", false otherwise. In
 *         practice, this value is used to signal the caller not to attempt
 *         further calls.
 */
bool CTrialSaccadesStimulus::DisplayStimulus()
{
	// Start trial w/"reset" boiler plate
	// Get current trial object
	// Call SetupStimsForTrial
	// Enter "fixate the cross" phase
	// Must update peripheral eye position before displaying periph dot
	// If satisfied, move to peripheral dot phase.
	//     Does text mark go in here? (rather than awkward "Delete when sent")
	//     The behavioral logic goes IN HERE

	CSaccadeTrial* currentTrial;
	const int behaveContingencyPulse = 100;
	bool fixationCrossAcquired = false;

	/* Once the D_IN_TARGET_ACQUIRED line goes high, this is set to true */
	bool saccadeTargetAcquired = false;

	/* Trigger debugging behavior */
	const bool bDebugLabelsOn = (BST_CHECKED == 
		g_pTrialSaccadesDlg->m_debugLabelsOn.GetCheck());

	bool isOffsetTrial = false;
	bool advanceEitherEye = false;

	const int initTimePeriod = etu::RandChoice(
		g_pTrialSaccades->getInitTimeMin(),
		g_pTrialSaccades->getInitTimeMax()); 
	const int flashTm = etu::RandChoice(
		g_pTrialSaccades->getFlashTimeMin(),
		g_pTrialSaccades->getFlashTimeMax());
	const int delayTm = etu::RandChoice(
		g_pTrialSaccades->getDelayTimeMin(), 
		g_pTrialSaccades->getDelayTimeMax());
	const int baitedPeriod = g_pTrialSaccades->getBaitedTime();
	const int holdPeriod = g_pTrialSaccades->getHoldTime();
	int currTimeLimit;

	/* Get next trial within mutex handle; we're in a different thread from
	 * the CTrialSaccades object. */
	{
		SmartLock trialSaccadesLock(*g_pTrialSaccades);
		
		currentTrial = g_pTrialSaccades->GetNextTrial();

		if (currentTrial == NULL) {
			AfxMessageBox("No trials.");
			return false;
		}
	}

	if ((currentTrial->getCtrXDeg() != 0.0) &&
		(g_pTrialSaccades->getCtrStepSize() > 30) &&
		(g_pTrialSaccades->getEyeCoverage() == CTrialSaccades::COVERAGE_SPATIOTOPIC))
	{
		double cXDeg = currentTrial->getCtrXDeg();

		isOffsetTrial = true;

		/* Trials w/offset fixation cross are skipped for purple crosses or 
		 * purple saccade targets. Since the grid adds steps to left *and* 
		 * right, we must manually remove one side (hence the 'red and >0' or 
		 * 'blue and <0') to keep the balance right. */
		if (currentTrial->isCenterPurple() 
			|| (currentTrial->isCenterRed() && (cXDeg < 0)) 
			|| (currentTrial->isCenterBlue() && (cXDeg > 0))
			|| currentTrial->isPeriphPurple())
		{
			return true; // Skip this trial altogether
		}		
	}

	SetupStimsForTrial(*currentTrial, isOffsetTrial);

	/* Analog output for trial type */
	etu::AnalogOut(A_OUT_CTR_TARG_COLOR, 
		NESTED_TERNARY(currentTrial->isCenterRed(), currentTrial->isCenterBlue(), 
			(float)VOLTS_COLOR_RED, (float)VOLTS_COLOR_BLUE, (float)VOLTS_COLOR_PURPLE));
	etu::AnalogOut(A_OUT_PER_TARG_COLOR, 
		NESTED_TERNARY(currentTrial->isPeriphRed(), currentTrial->isPeriphBlue(), 
			(float)VOLTS_COLOR_RED, (float)VOLTS_COLOR_BLUE, (float)VOLTS_COLOR_PURPLE));

	/* Behavioral contingency counter should (always) be reset for next trial 
	 * even if it's a repeat of a past CSaccadeTrial object. */
	vsgIOWriteDigitalOut(0xFFFF, D_OUT_BEHAV_CONT_RESET);
	WaitUntil(behaveContingencyPulse);
	vsgIOWriteDigitalOut(0x0000, D_OUT_BEHAV_CONT_RESET);

	advanceEitherEye = (g_pTrialSaccades->getAdvanceEitherEye() ||
		currentTrial->isCenterPurple());
	vsgIOWriteDigitalOut(0x0000, D_OUT_LOW_FOR_CTR);
	vsgIOWriteDigitalOut((advanceEitherEye ? 0x0000 : 0xFFFF), D_OUT_LOW_FOR_ALT);

	/* -------------------------------------------------------------------- *
	 * --------------------------- FIXATION ------------------------------- *
	 * -------------------------------------------------------------------- */
	PresentStimCrossOnly(*currentTrial);

	DEBUG_DISPLAY(bDebugLabelsOn, "FIXATION");
	g_pTrialSaccadesDlg->AddInfoText("Fixation");

	/* Pause with dot off */
	while (!bEndThread && 
		(g_pTrialSaccadesDlg->m_PauseDotOff.GetCheck() == BST_CHECKED)) 
	{
		WaitUntil(250);
	}

	if (bEndThread) { return true; }
	m_flashTimer.StartTimer();
	while ((m_flashTimer.CheckTimer() < initTimePeriod) && !bEndThread) {
		/* Sample eye position and update GUI. */
		g_pEyeTrackingDlg->EyePosSample();
		g_pTrialSaccadesDlg->UpdateEyePositions(
			g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
			g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);
		WaitUntil(1);

		if (!this->isDebugResponserOn() && (vsgIOReadDigitalIn() & D_IN_NEXT_STIM)) {
			/* Loop here until fixation criteria are met */
			m_flashTimer.CheckTimerWithReset();
			g_pTrialSaccadesDlg->AddInfoText("reset...");
		}
	}
	g_pTrialSaccadesDlg->AddInfoText("\n");

	if (bEndThread) { return true; }

	/* Write different loops to optimize the number of if-statements. */
	if (flashTm == delayTm) {
		/* Flash dot and cross for same time, extinguish together. */
		m_flashTimer.CheckTimerWithReset();
		PresentStimCrossDot(*currentTrial);
		while ((m_flashTimer.CheckTimer() < crossDotPeriod) && !bEndThread) {
			/* MUST FIXATE CROSS */
			WaitUntil(10);
		}

		m_flashTimer.CheckTimerWithReset();
		PresentStimBgOnly(*currentTrial, true);
		while ((m_flashTimer.CheckTimer() < baitedPeriod) && !bEndThread) {

			/* Only check for an accurate saccade here, since the cross was
			 * on until now. */

			WaitUntil(10);
		}
	}
	else if (delayTm == 0) {
		/* Extinguish cross now, flash dot. */
	}
	else if (delayTm > flashTm) {
		/* Flash dot and cross for same time, extinguish together. */
		const int crossDotPeriod = delayTm - flashTm;

		m_flashTimer.CheckTimerWithReset();
		PresentStimCrossDot(*currentTrial);
		while ((m_flashTimer.CheckTimer() < crossDotPeriod) && !bEndThread) {
			/* MUST FIXATE CROSS */
			WaitUntil(10);
		}

		PresentStimCrossOnly(*currentTrial);
		while ((m_flashTime.CheckTimer() < delayTm) && !bEndThread) {
			/* MUST FIXATE CROSS */
			WaitUntil(10);
		}
		
		m_flashTimer.CheckTimerWithReset();
		PresentStimBgOnly(*currentTrial, true);
		while ((m_flashTimer.CheckTimer() < baitedPeriod) && !bEndThread) {

			/* Only check for an accurate saccade here, since the cross was
			 * on until now. */

			WaitUntil(10);
		}
	}
	else { /* flashTm > delayTm */
		const int crossDotPeriod = flashTm - delayTm;
		bool targetAcquired = false;

		m_flashTimer.CheckTimerWithReset();
		PresentStimCrossDot(*currentTrial);
		while ((m_flashTimer.CheckTimer() < crossDotPeriod) && !bEndThread) {
			/* MUST FIXATE CROSS */
			WaitUntil(10);
		}

		PresentStimDotOnly(*currentTrial);
		while ((m_flashTimer.CheckTimer() < flashTm) && !bEndThread) {
			/* CHECK FOR SACCADE */
		}

		PresentStimBgOnly(*currentTrial, true);

	}

	return true;


	
#if 0
	if (crossDotPeriod > 0) {
		PresentStimCrossDot(*currentTrial);
		while ((m_flashTimer.CheckTimer() < crossDotPeriod) && !bEndThread) {
			if (!this->isDebugResponserOn() && (vsgIOReadDigitalIn() & D_IN_NEXT_STIM)) 
			{
				/* Fail for now. In future, have an unconditional call to
				 * SequenceTrial(*currentTrial). */
				this->FailTrial(*currentTrial, g_pTrialSaccades->getMisbehaviorDelay());
				return true;
			}
		}
	}

	if (crossOnlyPeriod > 0) {
		PresentStimCrossOnly(*currentTrial);
		while ((m_flashTimer.CheckTimer() < crossOnlyPeriod) && !bEndThread) 
		{
			if (!this->isDebugResponserOn() && (vsgIOReadDigitalIn() & D_IN_NEXT_STIM)) {
				/* Fail for now. In future, have an unconditional call to
				 * SequenceTrial(*currentTrial). */
				this->FailTrial(*currentTrial, g_pTrialSaccades->getMisbehaviorDelay());
				return true;
			}
		}
	}

	if (dotOnlyPeriod > 0) {
	}
#endif
	return true;
}

#if 0

/**
 * @brief Display the cross, then the cross plus periph target, based on 
 * behaviorial contingencies.
 * @return True for success; false otherwise. 
 */
bool CTrialSaccadesStimulus::DisplayStimulusOld()
{
	const int behaveContingencyPulse = 100;
	double    centerXDeg, centerYDeg;
	double    periphXDeg, periphYDeg;
	char      buffer[1024];
	size_t    stringSize;
	int       timePeriod;
	const int flashPeriod = GetFlashTimePeriod();
	bool      inFlashPeriod = false;
	double    pixPeriphX, pixPeriphY;
	double    screenXOffset, screenYOffset;
	double    stimXOffset, stimYOffset;
	double    periphSize;
	
	CSaccadeTrial* currentTrial;

	bool fixationCrossAcquired = false;

	/* Once the D_IN_TARGET_ACQUIRED line goes high, this is set to true */
	bool saccadeTargetAcquired = false;

	/* Trigger debugging behavior */
	const bool bDebugLabelsOn = (BST_CHECKED == 
		g_pTrialSaccadesDlg->m_debugLabelsOn.GetCheck());
	const bool this->isDebugResponserOn() = (BST_CHECKED == 
		g_pTrialSaccadesDlg->m_debugResponderOn.GetCheck());

	// ***********************************************************************
	// Relocate below code to SetupStimsForTrial
	// 
	m_useNoise = g_pTrialSaccades->getUseNoise();
	if (g_pTrialSaccades->getInterleaveNoise()) {
		m_lastTrialNoise = !m_lastTrialNoise;
		m_useNoise = m_lastTrialNoise;
	}
	// 
	// Relocate above code to SetupStimsForTrial
	// ***********************************************************************

	/* Get next trial within mutex handle; we're in a different thread from
	 * the CTrialSaccades model. */
	{
		SmartLock trialSaccadesLock(*g_pTrialSaccades);
		
		currentTrial = g_pTrialSaccades->GetNextTrial();

		if (currentTrial == NULL) {
			AfxMessageBox("No trials.");
			return false;
		}
	}

	// ***********************************************************************
	// Relocate below code to SetupStimsForTrial
	// 

	/* Set up the LUTBUFFER with per-trial colors. */
	SetupBufferForTrial(*currentTrial);
	vsgLUTBUFFERWrite(0, &m_lutbuf);
	vsgLUTBUFFERtoPalette(0);
	
	vsgSetBackgroundColour(&m_bgColor);
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);
	vsgSetDrawMode(vsgCENTREXY);

	/* Used for drawing the same BG throughout a trial. */
	m_noiseBgController->initReferenceBg(vsgVIDEOPAGE, DRAW_PAGE_REF_BG);

	/* Get center location. */
	centerXDeg = currentTrial->getCtrXDeg();
	centerYDeg = currentTrial->getCtrYDeg();

	/**
	 * @todo NUCLEAR HACK
	 * Hack in a special case that was called for on impossibly short notice.
	 * We manipulate spatiotopic trials where the fixation cross is not at 
	 * (0,0) in the following way:
	 *     1. If the cross is red, position it at (deviation, 0).
	 *     2. If the cross is blue, position it at (-deviation, 0).
	 * The "deviation" is the measured strabismic angle, positive for XT and
	 * negative for ET. Purple trials not at (0,0) are dropped altogether.
	 *
	 * This is an utterly grotesque assault on good coding practice. I am
	 * ashamed. Worse, this hack required another hack to make it possible,
	 * in CSpatiotopicTrialGenerator::GenerateLocationsByCode. I am going
	 * straight to programmer pergutory. (JVB 10/25/2013)
	 */
	if ((g_pTrialSaccades->getEyeCoverage() == CTrialSaccades::COVERAGE_SPATIOTOPIC) &&
		(centerXDeg != 0.0))
	{
		if (currentTrial->isCenterPurple() || 
			(currentTrial->isCenterRed() && (centerXDeg < 0)) ||
			(currentTrial->isCenterBlue() && (centerXDeg > 0))) 
		{
			/* Skip off-center purple trials and "duplicated" Rs and Bs */
			return true;
		}
		else if (currentTrial->isPeriphPurple()) {
			/* Also skip deviated-cross purple target trials */
			return true;
		}
		else {
			double deviation = ((g_pTrialSaccades->getCtrStepSize() > 30.0) ?
				(g_pEyeTrackingDlg->avgrx - g_pEyeTrackingDlg->avglx)       :
				g_pTrialSaccades->getCtrStepSize());
			if (currentTrial->isCenterRed() && (centerXDeg > 0)) {
				centerXDeg = deviation;
			}
			else {
				centerXDeg = -1 * deviation;
			}
		}
	}

	/* Add stim offset to the stored value, as this location is reported via 
	 * AnalogOut.*/
	centerXDeg += g_pSettings->getXOffset();
	centerYDeg += g_pSettings->getYOffset();

	// 
	// Relocate above code to SetupStimsForTrial
	// ***********************************************************************
	
	/* Analog output for trial type */
	etu::AnalogOut(A_OUT_CTR_TARG_COLOR, 
		NESTED_TERNARY(currentTrial->isCenterRed(), currentTrial->isCenterBlue(), 
			(float)VOLTS_COLOR_RED, (float)VOLTS_COLOR_BLUE, (float)VOLTS_COLOR_PURPLE));
	etu::AnalogOut(A_OUT_PER_TARG_COLOR, 
		NESTED_TERNARY(currentTrial->isPeriphRed(), currentTrial->isPeriphBlue(), 
			(float)VOLTS_COLOR_RED, (float)VOLTS_COLOR_BLUE, (float)VOLTS_COLOR_PURPLE));

	/* Behavioral contingency counter should (always) be reset for next trial 
	 * even if it's a repeat of a past CSaccadeTrial object. */
	vsgIOWriteDigitalOut(0xFFFF, D_OUT_BEHAV_CONT_RESET);
	WaitUntil(behaveContingencyPulse);
	vsgIOWriteDigitalOut(0x0000, D_OUT_BEHAV_CONT_RESET);

	// ***********************************************************************
	// Relocate below code to SetupStimsForTrial
	// 

	/* Prep text mark before stimulus starts to prevent time lags later. */
	{
		int radius = radiusToCode(currentTrial->getPerRDeg(), 
			g_pTrialSaccades->getRadiusOffset());
		int angle = angleToCode(currentTrial->getPerADeg(), 
			g_pTrialSaccades->getAngleOffset());
		int code = ((currentTrial->getStimEye() << 24) | (radius << 16) | 
			(angle << 8) | currentTrial->getStimCode());
		
		sprintf(buffer, "cRGB: %.2f, %.2f, %.2f pRGB: %.2f, %.2f, %.2f|0x%x/\0", 
			m_lutbuf[LEVEL_CTR_TARG].a, m_lutbuf[LEVEL_CTR_TARG].b, m_lutbuf[LEVEL_CTR_TARG].c, 
			m_lutbuf[LEVEL_PER_TARG].a, m_lutbuf[LEVEL_PER_TARG].b, m_lutbuf[LEVEL_PER_TARG].c, 
			code);
		stringSize = strlen(buffer);

		CString msg;
		unsigned int byte1 = code & 0x000000FF;
		unsigned int byte2 = (code & 0x0000FF00) >> 8;
		unsigned int byte3 = (code & 0x00FF0000) >> 16;
		unsigned int byte4 = (code & 0xFF000000) >> 24;

		msg.Format( \
			  "  Stim:\t\t0x%.2x %s\n" \
			  "  Angle:\t\t0x%.2x \n" \
			  "  Radius:\t0x%.2x \n" \
			  "  Eye:\t\t0x%.2x\n", 
			byte1, stimCodeToString(currentTrial->getStimCode()), 
			byte2, byte3, byte4);

		g_pTrialSaccadesDlg->SetInfoText((LPCTSTR)msg);
	}

	// 
	// Relocate above code to SetupStimsForTrial
	// ***********************************************************************

	/* ----------------------------------------------------------------------
	 * ------------------------------ FIXATION ------------------------------
	 * ----------------------------------------------------------------------
	 *
	 * Subject must fixate center cross for time period given by
	 * GetCenterTimePeriod().
	 */
	bool advanceEitherEye = (g_pTrialSaccades->getAdvanceEitherEye() ||
		currentTrial->isCenterPurple());
	vsgIOWriteDigitalOut(0x0000, D_OUT_LOW_FOR_CTR);
	vsgIOWriteDigitalOut((advanceEitherEye ? 0x0000 : 0xFFFF), D_OUT_LOW_FOR_ALT);

	NewDrawingPage(m_useNoise);
	DrawCrossWithBG(centerXDeg, centerYDeg, *currentTrial);

	DEBUG_DISPLAY(bDebugLabelsOn, "FIXATION");
	g_pTrialSaccadesDlg->AddInfoText("Fixation");

	// ***********************************************************************
	// Relocate below code to SetupStimsForTrial
	// 

	/* Present stimulus and set analog out as simultaneously as possible. */
	vsgPresent();
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)(centerXDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)(centerYDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)-9.95);
	etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)-9.95);

	// 
	// Relocate above code to SetupStimsForTrial
	// ***********************************************************************

	/* Pause with dot off */
	while (!bEndThread && 
		(g_pTrialSaccadesDlg->m_PauseDotOff.GetCheck() == BST_CHECKED)) 
	{
		WaitUntil(250);
	}

	if (bEndThread) { return true; }

	m_flashTimer.StartTimer();
	timePeriod = GetCenterTimePeriod(); 
	while ((m_flashTimer.CheckTimer() < timePeriod) && !bEndThread) {
		/* Sample eye position and update GUI. */
		g_pEyeTrackingDlg->EyePosSample();
		g_pTrialSaccadesDlg->UpdateEyePositions(
			g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
			g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);
		WaitUntil(1);
		if (!this->isDebugResponserOn() && (vsgIOReadDigitalIn() & D_IN_NEXT_STIM)) {
			/* Loop here until fixation criteria are met */
			m_flashTimer.CheckTimerWithReset();
			g_pTrialSaccadesDlg->AddInfoText("reset...");
		}
	}
	g_pTrialSaccadesDlg->AddInfoText("\n");

	if (bEndThread) { return true; }

	/* ----------------------------------------------------------------------
	 * -------------------- SACCADE TARGET FLASH & HOLD ---------------------
	 * ----------------------------------------------------------------------
	 */
	
	long failedSaccadeMsTime = 0;
	bool satisfiedTrial = false;

	switch (g_pTrialSaccades->getSaccadeTrigger()) {
		case CTrialSaccades::TRIGGER_CUE:
			satisfiedTrial = CueTriggeredTargetSequence(centerXDeg, centerYDeg, 
				buffer, stringSize, currentTrial, failedSaccadeMsTime);
			break;
		case CTrialSaccades::TRIGGER_SUBJECT:
			satisfiedTrial = SubjectTriggeredTargetSequence(centerXDeg, centerYDeg, 
				buffer, stringSize, currentTrial, failedSaccadeMsTime);
			break;
		case CTrialSaccades::TRIGGER_FLASH:
		default:
			satisfiedTrial = FlashTriggeredTargetSequence(centerXDeg, centerYDeg, 
				buffer, stringSize, currentTrial, failedSaccadeMsTime);
			break;
	}

	if (!satisfiedTrial) { 
		FailTrial(*currentTrial, failedSaccadeMsTime); 
		return true;
	}

	if (bEndThread) { return true; }

	/* ----------------------------------------------------------------------
	 * ------------------------------- REWARD -------------------------------
	 * ----------------------------------------------------------------------
	 */
	vsgIOWriteDigitalOut(0x0000, D_OUT_LOW_FOR_CTR);
	vsgIOWriteDigitalOut(0xFFFF, D_OUT_LOW_FOR_ALT);

	NewDrawingPage(m_useNoise);

	DEBUG_DISPLAY(bDebugLabelsOn, "REWARD");
	g_pTrialSaccadesDlg->AddInfoText("Reward\n");

	// ***********************************************************************
	// Relocate below code to SetupStimsForTrial
	// 

	/* Turn off both target locations */
	vsgPresent();
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)-9.95, true);
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)-9.95, true);
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)-9.95, true);
	etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)-9.95, true);

	// 
	// Relocate above code to SetupStimsForTrial
	// ***********************************************************************

	/* Send remote reward */
	vsgIOWriteDigitalOut(0xFFFF, D_OUT_FEED);
	WaitUntil(200);
	vsgIOWriteDigitalOut(0x0000, D_OUT_FEED);

	g_pTrialSaccadesDlg->IncrementCount(currentTrial->getStimCode(), true);

	/* Delay */
	WaitUntil(g_pTrialSaccades->getRewardDelay());

	/* Clear screen */
	NewDrawingPage(true);
	vsgPresent();

	return true;
}

/**
 * @brief Display the saccade target according to the Flash-Triggered 
 * sequence.
 *
 * Flash-Triggered is the traditional trial sequence: the saccade target is
 * flashed for a pre-determined duration, and the subject must saccade to it
 * within a pre-determined duration that runs concurrently with the flash
 * period.
 *
 * @param centerXDeg X coordinate of center cross, in degrees.
 * @param centerYDeg Y coordinate of center cross, in degrees.
 * @param buffer String buffer containing text mark.
 * @param stringSize Number of characters in buffer.
 * @param currentTrial Pointer to the object representing the current trial.
 * @param[out] mstime Receives the duratioin of the timeout the subject should
 *             receive for failing the trial, if s/he failed it.
 * @return True if the subject successfully completed the trial, false if the 
 *         subject failed it.
 */
bool CTrialSaccadesStimulus::FlashTriggeredTargetSequence(double centerXDeg, 
														  double centerYDeg,
														  char* buffer, 
														  size_t stringSize,
														  const CSaccadeTrial* currentTrial, 
														  long& mstime)
{
	const int flashPeriod = GetFlashTimePeriod();
	double periphXDeg = 0.0;
	double periphYDeg = 0.0;
	long   timePeriod = 0;
	bool   trialFulfilled = false;
	bool   inFlashPeriod = true;
	bool   saccadeTargetAcquired = false;

	/* Trigger debugging behavior */
	const bool bDebugLabelsOn = (BST_CHECKED == 
		g_pTrialSaccadesDlg->m_debugLabelsOn.GetCheck());
	const bool this->isDebugResponserOn() = (BST_CHECKED == 
		g_pTrialSaccadesDlg->m_debugResponderOn.GetCheck());
	
	timePeriod = g_pTrialSaccades->getFixationFlashDelay();
	if (timePeriod > 0) {
		/* Draw noise BG (if noise is on) and, optionally, fixation cross
		 * during this intermezzo. */
		NewDrawingPage(m_useNoise);

		/* Turn cross display and target requirement off. */
		vsgPresent();
		etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)-9.95);
		etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)-9.95);

		/* Short delay between fixation cross and flashed saccade target. */
		m_flashTimer.StartTimer();

		g_pTrialSaccadesDlg->AddInfoText("Fixation-flash delay\n");
		 
		while ((m_flashTimer.CheckTimer() < timePeriod) && !bEndThread) {
			/* High-resolution busy wait w/no behavioral requirement */
			WaitUntil(1);
		}
	}
	
	/* Draw saccade target and, optionally, fixation cross.  The target for
	 * eye tracking is only the saccade target, on Analog Out. */
	NewDrawingPage(m_useNoise);

	/* Get peripheral location, and add stim offset to the stored value, as 
	 * this location is reported via AnalogOut. Do so after sampling eyes. */
	periphXDeg = currentTrial->getPerXDeg() + g_pSettings->getXOffset();
	periphYDeg = currentTrial->getPerYDeg() + g_pSettings->getYOffset();

	/** 
	 * @TODO HACK ALERT: this is really a hack
	 * Hack to update retinotopic positions to actual eye location.
	 * Before all this, retinotopic positioning only affects the CSaccadeTrial
	 * objects' stim eye and fixation point.
	 */
	if ((g_pTrialSaccades->getEyeCoverage() == CTrialSaccades::COVERAGE_RETINOTOPIC) &&
		!g_pTrialSaccades->getUseApprox())
	{
		switch (currentTrial->getStimEye()) {
			case CSaccadeTrial::EYE_RIGHT:
				periphXDeg += g_pEyeTrackingDlg->avgrx;
				periphYDeg += g_pEyeTrackingDlg->avgry;
				break;
			case CSaccadeTrial::EYE_LEFT:
				periphXDeg += g_pEyeTrackingDlg->avglx;
				periphYDeg += g_pEyeTrackingDlg->avgly;
				break;
		}
	}

	DrawPeripheralStim(periphXDeg, periphYDeg, *currentTrial);

	/* Turn cross display and target requirement off. Turn periph display and
	 * target requirement on. Send prepared text mark. */
	vsgPresent();
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)-9.95);
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)-9.95);
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)(periphXDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)(periphYDeg / g_pSettings->getDegPerVolt()));
	g_pSettings->theSerialComm.transmit(buffer, (int)stringSize);
	vsgIOWriteDigitalOut(0xFFFF, D_OUT_LOW_FOR_CTR);
	vsgIOWriteDigitalOut(0xFFFF, D_OUT_LOW_FOR_ALT);

	/* Timer starts before pausing. */
	m_flashTimer.StartTimer();

	DEBUG_DISPLAY(bDebugLabelsOn, "SACCADE TARGET");
	g_pTrialSaccadesDlg->AddInfoText("Saccade...");

	/* Pause w/dot on and timer running. */
	while (!bEndThread && 
		(g_pTrialSaccadesDlg->m_PauseDotOn.GetCheck() == BST_CHECKED)) 
	{
		WaitUntil(1);
	}

	if (bEndThread) { return true; }

	/* Prep next drawing page before checking timer. */
	NewDrawingPage(m_useNoise);

	timePeriod = g_pTrialSaccades->getTargetActivePeriod();

	/* Now check the timer */
	while (!trialFulfilled && !bEndThread && 
		(m_flashTimer.CheckTimer() < timePeriod)) 
	{
		/* Subject is fixating periph. target. */
		if (saccadeTargetAcquired) {
			if (vsgIOReadDigitalIn() & D_IN_NEXT_STIM) {
				g_pTrialSaccadesDlg->SetInfoText("Broken hold");
				mstime = g_pTrialSaccades->getMisbehaviorDelay();
				return false;
			}
		}
		/* Subject not yet fixating */
		else if (vsgIOReadDigitalIn() & D_IN_TARGET_ACQUIRED) {
			/* Subject just acquired the target. */
			saccadeTargetAcquired = true;

			if (g_pTrialSaccades->getSaccadeHoldPeriod() > 50) {
				/* Don't try to time hold periods less than 50 ms. For
				 * longer hold periods, time from now until hold period ends, 
				 * but do not reset timer so that we can simultaneously 
				 * time the target extinction. */
				timePeriod = (g_pTrialSaccades->getSaccadeHoldPeriod() + 
					m_flashTimer.CheckTimer());
				g_pTrialSaccadesDlg->AddInfoText("hold...\n");
			}
			else {
				trialFulfilled = true;
				g_pTrialSaccadesDlg->AddInfoText("held\n");
			}
		}
		
		/* Subject has not acquired target yet */
		if (inFlashPeriod && (m_flashTimer.CheckTimer() >= flashPeriod))
		{
			/* Flash period just ended; turn off periph display but maintain
			 * target requirement. */
			inFlashPeriod = false;
			vsgPresent();
		}

		WaitUntil(1);
	}

	if (!saccadeTargetAcquired) {
		g_pTrialSaccadesDlg->SetInfoText("Not acquired");
		mstime = g_pTrialSaccades->getWrongDelay();
		return false;
	}

	return true;
}

/**
 * @brief Display the saccade target according to the Cue-Triggered 
 * sequence.
 *
 * Cue-Triggered sequencing is subtly different from Flash-Triggered. Here,
 * the prompt for saccading to the peripheral target is not the appearance of
 * the target, but rather the disappearance of the cross a prescribed time 
 * later. The target remains for a prescribed time following cross extinction.
 *
 * @param centerXDeg X coordinate of center cross, in degrees.
 * @param centerYDeg Y coordinate of center cross, in degrees.
 * @param buffer String buffer containing text mark.
 * @param stringSize Number of characters in buffer.
 * @param currentTrial Pointer to the object representing the current trial.
 * @param[out] mstime Receives the duratioin of the timeout the subject should
 *             receive for failing the trial, if s/he failed it.
 * @return True if the subject successfully completed the trial, false if the 
 *         subject failed it.
 */
bool CTrialSaccadesStimulus::CueTriggeredTargetSequence(double centerXDeg, 
														double centerYDeg,
														char* buffer, 
														size_t stringSize,
														const CSaccadeTrial* currentTrial, 
														long& mstime)
{
	const int cueTime = GetRandomizedTimePeriod(
		g_pTrialSaccades->getCueTimeMin(), g_pTrialSaccades->getCueTimeMax());
	const int delayTime = GetRandomizedTimePeriod(
		g_pTrialSaccades->getDelayTimeMin(), g_pTrialSaccades->getDelayTimeMax());
	const int lingerTime = GetRandomizedTimePeriod(
		g_pTrialSaccades->getLingerTimeMin(), g_pTrialSaccades->getLingerTimeMax());

	double periphXDeg = 0.0;
	double periphYDeg = 0.0;
	long   timePeriod = 0;
	bool   trialFulfilled = false;
	bool   crossPhase = true;
	bool   saccadeTargetAcquired = false;
	double currentTime = 0.0;
	bool   tagTransmitted = false;

	/* Trigger debugging behavior */
	const bool bDebugLabelsOn = (BST_CHECKED == 
		g_pTrialSaccadesDlg->m_debugLabelsOn.GetCheck());
	const bool this->isDebugResponserOn() = (BST_CHECKED == 
		g_pTrialSaccadesDlg->m_debugResponderOn.GetCheck());
	
	/* Get peripheral location, and add stim offset to the stored value, as 
	 * this location is reported via AnalogOut. Do so after sampling eyes. */
	periphXDeg = currentTrial->getPerXDeg() + g_pSettings->getXOffset();
	periphYDeg = currentTrial->getPerYDeg() + g_pSettings->getYOffset();

	/** 
	 * @TODO HACK ALERT: this is really a hack
	 * Hack to update retinotopic positions to actual eye location.
	 * Before all this, retinotopic positioning only affects the CSaccadeTrial
	 * objects' stim eye and fixation point.
	 */
	if ((g_pTrialSaccades->getEyeCoverage() == CTrialSaccades::COVERAGE_RETINOTOPIC) &&
		!g_pTrialSaccades->getUseApprox())
	{
		switch (currentTrial->getStimEye()) {
			case CSaccadeTrial::EYE_RIGHT:
				periphXDeg += g_pEyeTrackingDlg->avgrx;
				periphYDeg += g_pEyeTrackingDlg->avgry;
				break;
			case CSaccadeTrial::EYE_LEFT:
				periphXDeg += g_pEyeTrackingDlg->avglx;
				periphYDeg += g_pEyeTrackingDlg->avgly;
				break;
		}
	}

	/* ----------------------------------------------------------------------
	 * CUE TIME
	 * Display periph target while leaving center target on.
	 * ---------------------------------------------------------------------*/
	NewDrawingPage(m_useNoise);
	DrawCrossWithBG(centerXDeg, centerYDeg, *currentTrial);
	DrawPeripheralStim(periphXDeg, periphYDeg, *currentTrial);
	vsgPresent();
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)(periphXDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)(periphYDeg / g_pSettings->getDegPerVolt()));
	DEBUG_DISPLAY(bDebugLabelsOn, "CUE PERIOD");
	g_pTrialSaccadesDlg->AddInfoText("Cue...");

	/* Time the cue period, where subject must fixate the cross. Don't bother
	 * sampling eye positions anymore since the periph target is already up. */
	m_flashTimer.StartTimer();
	while (!bEndThread && (m_flashTimer.CheckTimer() < cueTime)) {
		WaitUntil(1);
		if (!this->isDebugResponserOn() && (vsgIOReadDigitalIn() & D_IN_NEXT_STIM)) {
			g_pTrialSaccadesDlg->SetInfoText("Broken cue");
			mstime = g_pTrialSaccades->getMisbehaviorDelay();
			return false;
		}
	}

	if (bEndThread) { return true; }

	/* Timer starts before pausing. */
	m_flashTimer.StartTimer();

	/* Pause w/dot on and timer running. */
	while (!bEndThread && 
		(g_pTrialSaccadesDlg->m_PauseDotOn.GetCheck() == BST_CHECKED)) 
	{
		WaitUntil(1);
	}

	if (bEndThread) { return true; }

	DEBUG_DISPLAY(bDebugLabelsOn, "SACCADE AND HOLD");

	timePeriod = (g_pTrialSaccades->getTargetActivePeriod() + delayTime);

	/* Now check the timer */
	while (!trialFulfilled && !bEndThread && 
		((currentTime = m_flashTimer.CheckTimer()) < timePeriod)) 
	{
		/* Three possibilities
		 * delayTime > lingerTime : Memory-guided saccade.
		 *     - Extinguish target at end of lingerTime.
		 *     - Leave D_OUT_LOW_FOR_CTR line low while cross is on.
		 *     - Since cue is over, broken fixation = misbehavior timeout.
		 *     
		 * lingerTime > delayTime  : Overlap saccade.
		 *     - Extinguish cross at end of delayTime.
		 *
		 * delayTime == lingerTime : Cued saccade.
		 *     - Do both together.
		 *
		 * Always, at end of delayTime:
		 *     - Pull D_OUT_LOW_FOR_CTR line high.
		 *     - Send saccade textmark.
		 *     - Extinguish cross.
		 *     - Set crossPhase = false
		 */
		if ((currentTime > lingerTime) && (currentTime < delayTime)) {
			/* Turn target off, leave cross on. */
			NewDrawingPage(m_useNoise);
			DrawCrossWithBG(centerXDeg, centerYDeg, *currentTrial);
			vsgPresent();
		}
		else if ((currentTime > delayTime) && (currentTime < lingerTime)) {
			/* Turn cross off, leave target on. Send prepared text mark, near
			 * the actual saccade. */
			NewDrawingPage(m_useNoise);
			DrawPeripheralStim(periphXDeg, periphYDeg, *currentTrial);
			vsgPresent();
			etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)-9.95);
			etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)-9.95);
			if (!tagTransmitted) {
				g_pSettings->theSerialComm.transmit(buffer, (int)stringSize);
				tagTransmitted = true;
			}
			vsgIOWriteDigitalOut(0xFFFF, D_OUT_LOW_FOR_CTR);
			vsgIOWriteDigitalOut(0xFFFF, D_OUT_LOW_FOR_ALT);
			crossPhase = false;
		}
		else if ((currentTime > lingerTime) || (currentTime > delayTime)) {
			/* One was turned off, turn the other off. */
			NewDrawingPage(m_useNoise);
			vsgPresent();
			if (crossPhase) {
				etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)-9.95);
				etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)-9.95);
				if (!tagTransmitted) {
					g_pSettings->theSerialComm.transmit(buffer, (int)stringSize);
					tagTransmitted = true;
				}
				vsgIOWriteDigitalOut(0xFFFF, D_OUT_LOW_FOR_CTR);
				vsgIOWriteDigitalOut(0xFFFF, D_OUT_LOW_FOR_ALT);
				crossPhase = false;
			}
		}

		/* Subject is (must be) fixating required target. */
		if (crossPhase || saccadeTargetAcquired) {
			if (vsgIOReadDigitalIn() & D_IN_NEXT_STIM) {
				g_pTrialSaccadesDlg->SetInfoText("Broken fixation");
				mstime = g_pTrialSaccades->getMisbehaviorDelay();
				return false;
			}
		}
		/* Saccade target phase and subject not yet fixating */
		else if (vsgIOReadDigitalIn() & D_IN_TARGET_ACQUIRED) {
			/* Subject just acquired the target. */
			saccadeTargetAcquired = true;

			if (g_pTrialSaccades->getSaccadeHoldPeriod() > 50) {
				/* Don't try to time hold periods less than 50 ms. For
				 * longer hold periods, time from now until hold period ends, 
				 * but do not reset timer so that we can simultaneously 
				 * time the target extinction. */
				timePeriod = (currentTime + 
					g_pTrialSaccades->getSaccadeHoldPeriod());
				g_pTrialSaccadesDlg->AddInfoText("hold...");
			}
			else {
				trialFulfilled = true;
				g_pTrialSaccadesDlg->AddInfoText("held\n");
			}
		}

		g_pTrialSaccadesDlg->AddInfoText("Saccade...");
		
		WaitUntil(1);
	}

	if (!saccadeTargetAcquired) {
		g_pTrialSaccadesDlg->SetInfoText("Not acquired");
		mstime = g_pTrialSaccades->getWrongDelay();
		return false;
	}

	return true;
}

/**
 * @brief Display the saccade target according to the Subject-Triggered 
 * sequence.
 *
 * Subject-Triggered is the most lax sequence. After the subject satisfies the
 * fixation time and causes the peripheral target to be displayed, s/he may
 * continue fixating the cross indefinitely. The cross only disappears when
 * the subject breaks center fixation with a saccade. The peripheral target
 * disappears a prescribed time following the extinction of the cross.
 *
 * @param centerXDeg X coordinate of center cross, in degrees.
 * @param centerYDeg Y coordinate of center cross, in degrees.
 * @param buffer String buffer containing text mark.
 * @param stringSize Number of characters in buffer.
 * @param currentTrial Pointer to the object representing the current trial.
 * @param[out] mstime Receives the duratioin of the timeout the subject should
 *             receive for failing the trial, if s/he failed it.
 * @return True if the subject successfully completed the trial, false if the 
 *         subject failed it.
 */
bool CTrialSaccadesStimulus::SubjectTriggeredTargetSequence(double centerXDeg, 
														    double centerYDeg,
															char* buffer, 
															size_t stringSize,
															const CSaccadeTrial* currentTrial, 
															long& mstime)
{
	double periphXDeg = 0.0;
	double periphYDeg = 0.0;
	long   timePeriod = 0;
	bool   trialFulfilled = false;
	bool   saccadeTargetAcquired = false;

	/* Trigger debugging behavior */
	const bool bDebugLabelsOn = (BST_CHECKED == 
		g_pTrialSaccadesDlg->m_debugLabelsOn.GetCheck());
	const bool this->isDebugResponserOn() = (BST_CHECKED == 
		g_pTrialSaccadesDlg->m_debugResponderOn.GetCheck());
	
	/* Get peripheral location, and add stim offset to the stored value, as 
	 * this location is reported via AnalogOut. Do so after sampling eyes. */
	periphXDeg = currentTrial->getPerXDeg() + g_pSettings->getXOffset();
	periphYDeg = currentTrial->getPerYDeg() + g_pSettings->getYOffset();

	/** 
	 * @TODO HACK ALERT: this is really a hack
	 * Hack to update retinotopic positions to actual eye location.
	 * Before all this, retinotopic positioning only affects the CSaccadeTrial
	 * objects' stim eye and fixation point.
	 */
	if ((g_pTrialSaccades->getEyeCoverage() == CTrialSaccades::COVERAGE_RETINOTOPIC) &&
		!g_pTrialSaccades->getUseApprox())
	{
		switch (currentTrial->getStimEye()) {
			case CSaccadeTrial::EYE_RIGHT:
				periphXDeg += g_pEyeTrackingDlg->avgrx;
				periphYDeg += g_pEyeTrackingDlg->avgry;
				break;
			case CSaccadeTrial::EYE_LEFT:
				periphXDeg += g_pEyeTrackingDlg->avglx;
				periphYDeg += g_pEyeTrackingDlg->avgly;
				break;
		}
	}

	/* Display periph target while leaving center target on as requirement. */
	NewDrawingPage(m_useNoise);
	DrawCrossWithBG(centerXDeg, centerYDeg, *currentTrial);
	DrawPeripheralStim(periphXDeg, periphYDeg, *currentTrial);
	vsgPresent();
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)(periphXDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)(periphYDeg / g_pSettings->getDegPerVolt()));

	DEBUG_DISPLAY(bDebugLabelsOn, "VOLUNTARY PERIOD");
	g_pTrialSaccadesDlg->AddInfoText("Target; waiting...");

	/* Prep page with no cross or target. */
	NewDrawingPage(m_useNoise);

	/* Wait for subject to make a saccade. Don't bother sampling eye positions 
	 * anymore since the periph target is already up. */
	m_flashTimer.StartTimer();
	while (!bEndThread) {
		WaitUntil(1);
		if (this->isDebugResponserOn() || (vsgIOReadDigitalIn() & D_IN_NEXT_STIM)) {
			/* Subject made saccade. */
			g_pTrialSaccadesDlg->AddInfoText("saccade\n");
			break;
		}
	}

	if (bEndThread) { return true; }

	/* Extinguish cross and require periph target. Send prepared text mark, 
	 * near the actual saccade. */
	vsgPresent();
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)-9.95);
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)-9.95);
	g_pSettings->theSerialComm.transmit(buffer, (int)stringSize);
	vsgIOWriteDigitalOut(0xFFFF, D_OUT_LOW_FOR_CTR);
	vsgIOWriteDigitalOut(0xFFFF, D_OUT_LOW_FOR_ALT);

	/* Timer starts before pausing. */
	m_flashTimer.StartTimer();

	/* Pause w/dot on and timer running. */
	while (!bEndThread && 
		(g_pTrialSaccadesDlg->m_PauseDotOn.GetCheck() == BST_CHECKED)) 
	{
		WaitUntil(1);
	}

	if (bEndThread) { return true; }

	DEBUG_DISPLAY(bDebugLabelsOn, "SACCADE AND HOLD");

	timePeriod = g_pTrialSaccades->getTargetActivePeriod();

	/* Now check the timer */
	while (!trialFulfilled && !bEndThread && 
		(m_flashTimer.CheckTimer() < timePeriod)) 
	{
		/* Subject is fixating periph. target. */
		if (saccadeTargetAcquired) {
			if (vsgIOReadDigitalIn() & D_IN_NEXT_STIM) {
				g_pTrialSaccadesDlg->SetInfoText("Broken hold\n");
				mstime = g_pTrialSaccades->getMisbehaviorDelay();
				return false;
			}
		}
		/* Subject not yet fixating */
		else if (vsgIOReadDigitalIn() & D_IN_TARGET_ACQUIRED) {
			/* Subject just acquired the target. */
			saccadeTargetAcquired = true;

			if (g_pTrialSaccades->getSaccadeHoldPeriod() > 50) {
				/* Don't try to time hold periods less than 50 ms. For
				 * longer hold periods, time from now until hold period ends, 
				 * but do not reset timer so that we can simultaneously 
				 * time the target extinction. */
				timePeriod = (g_pTrialSaccades->getSaccadeHoldPeriod() + 
					m_flashTimer.CheckTimer());
				g_pTrialSaccadesDlg->AddInfoText("Hold...\n");
			}
			else {
				trialFulfilled = true;
				g_pTrialSaccadesDlg->AddInfoText("Held\n");
			}
		}
		
		WaitUntil(1);
	}

	if (!saccadeTargetAcquired) {
		g_pTrialSaccadesDlg->SetInfoText("Not acquired");
		mstime = g_pTrialSaccades->getWrongDelay();
		return false;
	}

	return true;
}

#endif

/**
 * @brief Draw fixation cross w/background pattern and/or occlusion.
 *
 * Draws the center fixation cross by respecting the current background 
 * setting.  Does not modify Analog Out channels.  The caller is responsible 
 * for specifying the correct center location, and thus the caller is also
 * responsible for modifying Analog Out.
 *
 * @param centerXDeg X coordinate, in degrees.
 * @param centerYDeg Y coordinate, in degrees.
 * @param trial Const reference to the current trial.
 */
void CTrialSaccadesStimulus::DrawCrossWithBG(double centerXDeg, 
											 double centerYDeg, 
											 const CSaccadeTrial& trial) 
{
	double centerXPix, centerYPix, centerSizePix;
	VsgExt::IGrating* gratingDesc = NESTED_TERNARY(trial.isCenterRed(), 
		trial.isCenterBlue(), m_primarySpec, m_secondarySpec, m_tertiarySpec);
	VsgExt::NoiseBgController* controller = NULL;
	VsgExt::NoiseColorMap* map = NULL;
	
	/* Screen offsets only; stim offsets already applied to function params */
	centerXDeg += g_pSettings->getScreenXOffset();
	centerYDeg += g_pSettings->getScreenYOffset();

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, centerXDeg, vsgPIXELUNIT, &centerXPix);
	/* Y pixels increase as Y degrees decrease (as Y goes down screen) */
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -1 * centerYDeg, vsgPIXELUNIT, &centerYPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pTrialSaccades->getCenterDotSize(), 
		vsgPIXELUNIT, &centerSizePix);
	
	/* Draw center point (only) */
	if (m_useNoise && !g_pTrialSaccades->getOccludeBG()) {
		controller = m_noiseBgController;
		map = m_noiseColorMap;
	}

	VsgExt::drawCross(centerXPix, centerYPix, centerSizePix, 
		g_pTrialSaccades->getCrossPix(), *gratingDesc, map, controller);
}

/**
 * @brief Draw the peripheral dot, respecting noise BG and grating settings.
 *
 * @param xDeg X coordinate in degrees.
 * @param yDeg Y coordinate in degrees.
 * @param trial Reference to current trial object, for access to center and 
 *        peripheral color queries.
 */
void CTrialSaccadesStimulus::DrawPeripheralStim(double xDeg, double yDeg,
												const CSaccadeTrial& trial)
{
	double xPix, yPix, sizePix;
	/* Optionally set to non-null if BG is on */
	VsgExt::NoiseColorMap* colorMap = NULL;
	VsgExt::NoiseBgController* bgController = NULL;
	/* Must be set to non-null, either grating or solid color. */
	VsgExt::IGrating* pixelSpec = NULL;

	if (m_useNoise && !g_pTrialSaccades->getOccludeBG()) {
		/* Need to respect BG when drawing. */
		bgController = m_noiseBgController;
		colorMap = m_noiseColorMap;
	}

	if (g_pTrialSaccades->getDotGratings()) {
		pixelSpec = NESTED_TERNARY(trial.isPeriphRed(), trial.isPeriphBlue(),
			m_verticalGrating, m_horizontalGrating, m_tertiarySpec);
	}
	else {
		pixelSpec = NESTED_TERNARY(trial.isPeriphRed(), trial.isPeriphBlue(),
			m_primarySpec, m_secondarySpec, m_tertiarySpec);
	}

	/* Screen offsets only; stim offsets already applied to function params */
	xDeg += g_pSettings->getScreenXOffset();
	yDeg += g_pSettings->getScreenYOffset();

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, xDeg, vsgPIXELUNIT, &xPix);
	/* Y pixels increase as Y degrees decrease (as Y goes down screen) */
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -1 * yDeg, vsgPIXELUNIT, &yPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pTrialSaccades->getPeriphDotSize(), 
		vsgPIXELUNIT, &sizePix);

	/* The value entered via the GUI is radius.  The drawing 
	 * function call expects the diameter, thus double it.
	 */
	sizePix *= 2.0;

	if (g_pTrialSaccades->getUseShapes() && trial.isPeriphRed()) {
		VsgExt::drawTriangle((int)xPix, (int)yPix, (int)sizePix, *pixelSpec, 
			colorMap, bgController);
	}
	else if (g_pTrialSaccades->getUseShapes()) {
		VsgExt::drawRectangle((int)xPix, (int)yPix, (int)sizePix, 
			(int)sizePix, *pixelSpec, colorMap, bgController);
	}
	else {
		VsgExt::drawCircle((int)xPix, (int)yPix, (int)sizePix, *pixelSpec, 
			colorMap, bgController);
	}
}

/**
 * @brief Give the research subject a time out.
 * 
 * A time out is used as a no-reward indicator.  During the time out period,
 * the screen is blank.
 *
 * @param currentTrial The trial, for querries.
 * @param mstime Time for time out, in milliseconds.
 */
void CTrialSaccadesStimulus::FailTrial(CSaccadeTrial& currentTrial, long mstime) 
{
	const int response = 4;
	char buffer[256];

	/* Zero timer to start timing. */
	m_flashTimer.CheckTimerWithReset();

	PresentStimTimeout(currentTrial);

	/* Aborted trial, send response = 4, just for John */
	sprintf(buffer, "response=%d|0x%2x/\0", response, 48 + response);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));

	g_pTrialSaccadesDlg->IncrementCount(currentTrial.getStimCode(), false);

	/* Timer has been running since function start; now check. */
	while ((m_flashTimer.CheckTimer() < mstime) && !bEndThread) { WaitUntil(1); }
}

/**
 * @brief Get type of stimulus as a string.
 *
 * Inherited method from CGenericStimulus.
 *
 * @return Name of stimulus as a CString object.
 */
CString CTrialSaccadesStimulus::StimulusType() 
{
	return "Trial-based Saccades Stimulus";
}


bool CTrialSaccadesStimulus::isDebugResponserOn() const
{
	return (BST_CHECKED == g_pTrialSaccadesDlg->m_debugResponderOn.GetCheck());
}

#endif // APP_EYE_TRACKING
