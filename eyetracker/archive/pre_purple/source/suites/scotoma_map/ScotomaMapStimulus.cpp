/**
 * @file ScotomaMapStimulus.cpp
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
#include "ScotomaMapStimulus.h"
#include "utils/Utils.h"
#include "utils/Asserts.h"
#include "utils/Math.h"
#include "vsgext/vsgext.h"
#include "sys/sys_concurrency.h"

/*****************************************************************************
 * Defines
 *****************************************************************************/

/*** Communications **************************************/
#define A_OUT_CTR_TARG_HORIZ  (0)
#define A_OUT_CTR_TARG_VERT   (1)
#define A_OUT_PER_TARG_HORIZ  (2)
#define A_OUT_PER_TARG_VERT   (3)
#define A_OUT_CTR_TARG_COLOR  (4) // red = 5V; blue = -5V; other = 0
#define A_OUT_PER_TARG_COLOR  (5) // red = 5V; blue = -5V; ppl = 0; blank = -10V

#define A_IN_L_EYE_HORIZ  (0)
#define A_IN_L_EYE_VERT   (1)
#define A_IN_R_EYE_HORIZ  (3)
#define A_IN_R_EYE_VERT   (4)

#define D_OUT_TRIAL_STATE      (vsgDIG0) /* 0 = eye tracking part of trial;
                                          * 1 = lever/joystick part of trial */
#define D_OUT_BEHAV_CONT_RESET (vsgDIG1) // VSG pin 2; 1401 pin 18; 1401 dig in 6
#define D_OUT_PARADIGM_TYPE    (vsgDIG2) // High to indicate alternate reward paradigm
#define D_OUT_FEED             (vsgDIG7) // 0 = Don't feed.  1 = Feed

#define D_IN_NEXT_STIM  (vsgDIG0) /* 1 = Behavioral cont. met, move on;
							       * 0 = Behavioral cont. not yet met. */
#define D_IN_JOY_RIGHT  (vsgDIG1) /* 0 = Joystick moved left;  
							       * 1 = Joystick moved right */
#define D_IN_JOY_MOVED  (vsgDIG2) /* 0 = Joystick not moved; 
							       * 1 = Joystick moved L or R */

#define VOLTS_COLOR_RED    (5)
#define VOLTS_COLOR_BLUE   (-5)
#define VOLTS_COLOR_PURPLE (0)
#define VOLTS_COLOR_BLANK  (-10)

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
#define LEVEL_DIM_TERTIARY                (211)
#define LEVEL_NOISE_PLUS_DIM_PRIMARY      (212)
#define LEVEL_NOISE_PLUS_DIM_SECONDARY    (213)
#define LEVEL_SECONDARY_PLUS_DIM_PRIMARY  (214)
#define LEVEL_PRIMARY_PLUS_DIM_SECONDARY  (215)

/*** Page Numbers*****************************************/
#define DRAW_PAGE_REF_BG (2)

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

typedef std::auto_ptr<CScotomaTrial> TRIAL_AUTO_PTR;

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/**
 * @brief Constructor.
 */
CScotomaMapStimulus::CScotomaMapStimulus()
: m_bgColor(m_lutbuf[LEVEL_BG_CONTR]), m_noiseColor(m_lutbuf[LEVEL_BG_CONTR+100]),
  m_noiseBgController(NULL), m_noiseColorMap(NULL), m_primarySpec(NULL), 
  m_secondarySpec(NULL), m_tertiarySpec(NULL), m_verticalGrating(NULL), 
  m_horizontalGrating(NULL), m_dualGrating(NULL)
{
	size_t numBytes; 

	Initialize();

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	m_screenWidthPix = vsgGetScreenWidthPixels();
	m_screenHeightPix = vsgGetScreenHeightPixels();
	
	m_periphXDeg = 0;
	m_periphYDeg = 0;
	m_currentPage = 0;
	m_lastTrialNoise = true;
	m_useNoise = true;
	m_flashTimer.StartTimer();

	srand((unsigned int)time(NULL));
	
	vsgIOWriteDigitalOut(0x0000, D_OUT_TRIAL_STATE);
	vsgIOWriteDigitalOut(0x0000, D_OUT_BEHAV_CONT_RESET);
	vsgIOWriteDigitalOut(0xFFFF, D_OUT_PARADIGM_TYPE);
	vsgIOWriteDigitalOut(0x0000, D_OUT_FEED);

	/* Set the drawing mode, and set the origin to the center of the screen */
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(m_screenWidthPix / 2, m_screenHeightPix / 2);
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);

	memset(m_lutbuf, 0, 256 * sizeof(VSGTRIVAL));
	vsgSetDrawPage(vsgVIDEOPAGE, m_currentPage, vsgBACKGROUND);

	g_pScotomaMap->sendSettings(g_pSettings->theSerialComm);

	/* Init left- and right-fixating eye position arrays */
	pLCenterX = new double[g_pSettings->getNSamples()];
	pLCenterY = new double[g_pSettings->getNSamples()];
	pLPeriphX = new double[g_pSettings->getNSamples()];
	pLPeriphY = new double[g_pSettings->getNSamples()];
	pRCenterX = new double[g_pSettings->getNSamples()];
	pRCenterY = new double[g_pSettings->getNSamples()];
	pRPeriphX = new double[g_pSettings->getNSamples()];
	pRPeriphY = new double[g_pSettings->getNSamples()];
	numBytes = g_pSettings->getNSamples() * sizeof(double);
	memset(pLCenterX, 0, numBytes);
	memset(pLCenterY, 0, numBytes);
	memset(pLPeriphX, 0, numBytes);
	memset(pLPeriphY, 0, numBytes);
	memset(pRCenterX, 0, numBytes);
	memset(pRCenterY, 0, numBytes);
	memset(pRPeriphX, 0, numBytes);
	memset(pRPeriphY, 0, numBytes);

	lcount = 0;
	rcount = 0;
	alcx = 0;
	alcy = 0;
	alpx = 0;
	alpy = 0;
	arcx = 0;
	arcy = 0;
	arpx = 0;
	arpy = 0;
	
	{
		SmartLock scotomaMapDlgLock(*g_pScotomaMapDlg);
		SmartLock scotomaMapLock(*g_pScotomaMap);

		/* New sequence every time we run */
		g_pScotomaMapDlg->resetGraphs();
		g_pScotomaMap->GenerateTrials();
		m_currentTrial = g_pScotomaMap->getTrials().begin();
	}

	/* Populate colors generally; per-trial colors are populated in 
	 * DisplayStimulus. */
	InitLutBuffer();
	InitNoiseHandling();
	InitGratings();

	ETU_DEBUG_BLOCK {
		std::ofstream fout("c:\\temp\\etdebuglog.txt", std::ios::app);
		CScotomaMap::TrialPtrContainer_t::const_iterator iter = 
			g_pScotomaMap->getTrials().begin(); 
		CScotomaMap::TrialPtrContainer_t::const_iterator end = 
			g_pScotomaMap->getTrials().end();
		CScotomaTrial* current;
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
CScotomaMapStimulus::~CScotomaMapStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, D_OUT_TRIAL_STATE);
	vsgIOWriteDigitalOut(0x0000, D_OUT_BEHAV_CONT_RESET);
	vsgIOWriteDigitalOut(0x0000, D_OUT_PARADIGM_TYPE);
	vsgIOWriteDigitalOut(0x0000, D_OUT_FEED);

	delete[] pLCenterX;
	delete[] pLCenterY;
	delete[] pLPeriphX;
	delete[] pLPeriphY;
	delete[] pRCenterX;
	delete[] pRCenterY;
	delete[] pRPeriphX;
	delete[] pRPeriphY;

	CleanUpNoiseHandling();
	CleanUpGratings();

	etu::AnalogOut(A_OUT_CTR_TARG_COLOR, (float)0.0, true);
	
	/* Send stop mark */
	char buffer[80];
	sprintf(buffer, "Scotoma Monkey;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}

/**
 * @brief Set colors in member vsgLUTBUFFER for targets, background, and 
 * gratings.
 */
void CScotomaMapStimulus::InitLutBuffer() 
{
	double contrast = (double)g_pScotomaMap->getDotGratingContrast();

	/* For use here and later with BG Controller (if needed) */
	m_bgColor.a    = ((double)g_pSettings->getRedBG()) / 255.0f;
	m_bgColor.b    = ((double)g_pSettings->getGreenBG()) / 255.0f;
	m_bgColor.c    = ((double)g_pSettings->getBlueBG()) / 255.0f;
	m_noiseColor.a = ((double)g_pScotomaMap->getNoiseRed()) / 255.0f;
	m_noiseColor.b = ((double)g_pScotomaMap->getNoiseGreen()) / 255.0f;
	m_noiseColor.c = ((double)g_pScotomaMap->getNoiseBlue()) / 255.0f;

	/* Time out punishments are a blank screen. */
	m_lutbuf[LEVEL_TIME_OUT].a = 0.0;	
	m_lutbuf[LEVEL_TIME_OUT].b = 0.0;	
	m_lutbuf[LEVEL_TIME_OUT].c = 0.0;
	
	/* Save copies of all periph colors */
	m_lutbuf[LEVEL_PRIMARY].a = ((double)g_pScotomaMap->getPrimaryRed())/255.0f;
	m_lutbuf[LEVEL_PRIMARY].b = ((double)g_pScotomaMap->getPrimaryGreen())/255.0f;
	m_lutbuf[LEVEL_PRIMARY].c = ((double)g_pScotomaMap->getPrimaryBlue())/255.0f;

	m_lutbuf[LEVEL_SECONDARY].a = ((double)g_pScotomaMap->getSecondaryRed())/255.0f;
	m_lutbuf[LEVEL_SECONDARY].b = ((double)g_pScotomaMap->getSecondaryGreen())/255.0f;
	m_lutbuf[LEVEL_SECONDARY].c = ((double)g_pScotomaMap->getSecondaryBlue())/255.0f;

	m_lutbuf[LEVEL_TERTIARY].a = ((double)g_pScotomaMap->getTertiaryRed())/255.0f;
	m_lutbuf[LEVEL_TERTIARY].b = ((double)g_pScotomaMap->getTertiaryGreen())/255.0f;
	m_lutbuf[LEVEL_TERTIARY].c = ((double)g_pScotomaMap->getTertiaryBlue())/255.0f;

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

	m_lutbuf[LEVEL_DIM_TERTIARY].a = m_lutbuf[LEVEL_DIM_PRIMARY].a;
	m_lutbuf[LEVEL_DIM_TERTIARY].b = 0.0f;
	m_lutbuf[LEVEL_DIM_TERTIARY].c = m_lutbuf[LEVEL_DIM_SECONDARY].c;

	m_lutbuf[LEVEL_NOISE_PLUS_DIM_PRIMARY].a = m_lutbuf[LEVEL_DIM_PRIMARY].a;
	m_lutbuf[LEVEL_NOISE_PLUS_DIM_PRIMARY].b = 0.0f;
	m_lutbuf[LEVEL_NOISE_PLUS_DIM_PRIMARY].c = m_noiseColor.c;

	m_lutbuf[LEVEL_NOISE_PLUS_DIM_SECONDARY].a = m_noiseColor.a;
	m_lutbuf[LEVEL_NOISE_PLUS_DIM_SECONDARY].b = 0.0f;
	m_lutbuf[LEVEL_NOISE_PLUS_DIM_SECONDARY].c = m_lutbuf[LEVEL_DIM_SECONDARY].c;

	m_lutbuf[LEVEL_SECONDARY_PLUS_DIM_PRIMARY].a = m_lutbuf[LEVEL_DIM_PRIMARY].a;
	m_lutbuf[LEVEL_SECONDARY_PLUS_DIM_PRIMARY].b = 0.0f;
	m_lutbuf[LEVEL_SECONDARY_PLUS_DIM_PRIMARY].c = m_lutbuf[LEVEL_SECONDARY].c;

	m_lutbuf[LEVEL_PRIMARY_PLUS_DIM_SECONDARY].a = m_lutbuf[LEVEL_PRIMARY].a;
	m_lutbuf[LEVEL_PRIMARY_PLUS_DIM_SECONDARY].b = 0.0f;
	m_lutbuf[LEVEL_PRIMARY_PLUS_DIM_SECONDARY].c = m_lutbuf[LEVEL_DIM_SECONDARY].c;

	vsgSetBackgroundColour(&m_lutbuf[vsgBACKGROUND]);
	vsgSetFixationColour(&m_lutbuf[vsgFIXATION]);
}

/**
 * @brief Allocate the NoiseBgController and NoiseColorMap, and initialize
 * them.
 */
void CScotomaMapStimulus::InitNoiseHandling()
{
	int size = g_pScotomaMap->getNoiseSize();
	int percent = g_pScotomaMap->getNoisePct();

	CleanUpNoiseHandling();

	m_noiseBgController = new VsgExt::NoiseBgController(size, m_bgColor, 
		m_noiseColor, LEVEL_BG_CONTR, percent, m_lutbuf);

	m_noiseColorMap = new VsgExt::NoiseColorMap;
	m_noiseColorMap->map(LEVEL_PRIMARY, LEVEL_PRIMARY_PLUS_NOISE);
	m_noiseColorMap->map(LEVEL_SECONDARY, LEVEL_SECONDARY_PLUS_NOISE);
	m_noiseColorMap->map(LEVEL_TERTIARY, LEVEL_TERTIARY);
	m_noiseColorMap->map(LEVEL_DIM_PRIMARY, LEVEL_NOISE_PLUS_DIM_PRIMARY);
	m_noiseColorMap->map(LEVEL_DIM_SECONDARY, LEVEL_NOISE_PLUS_DIM_SECONDARY);
	m_noiseColorMap->map(LEVEL_PRIMARY_PLUS_DIM_SECONDARY, LEVEL_PRIMARY_PLUS_DIM_SECONDARY);
	m_noiseColorMap->map(LEVEL_SECONDARY_PLUS_DIM_PRIMARY, LEVEL_SECONDARY_PLUS_DIM_PRIMARY);
	m_noiseColorMap->map(LEVEL_DIM_TERTIARY, LEVEL_DIM_TERTIARY);
}

/**
 * @brief Deletes noise handling objects.
 */
void CScotomaMapStimulus::CleanUpNoiseHandling()
{
	if (m_noiseBgController) delete m_noiseBgController;
	if (m_noiseColorMap)     delete m_noiseColorMap;
}

/**
 * @brief Initializes grating handlers.
 */
void CScotomaMapStimulus::InitGratings()
{
	const int cycles = (int)g_pScotomaMap->getDotGratingCycles();

	CleanUpGratings();

	m_primarySpec = VsgExt::IGrating::identity(LEVEL_PRIMARY);
	m_secondarySpec = VsgExt::IGrating::identity(LEVEL_SECONDARY);
	m_tertiarySpec = VsgExt::IGrating::identity(LEVEL_TERTIARY);
	m_verticalGrating = VsgExt::IGrating::gratingVertical(LEVEL_PRIMARY,
		LEVEL_DIM_PRIMARY, cycles);
	m_horizontalGrating = VsgExt::IGrating::gratingHorizontal(LEVEL_SECONDARY,
		LEVEL_DIM_SECONDARY, cycles);
	m_dualGrating = VsgExt::IGrating::gratingDual(LEVEL_PRIMARY, 
		LEVEL_DIM_PRIMARY, cycles, LEVEL_SECONDARY, LEVEL_DIM_SECONDARY, 
		cycles, LEVEL_TERTIARY, LEVEL_PRIMARY_PLUS_DIM_SECONDARY, 
		LEVEL_SECONDARY_PLUS_DIM_PRIMARY, LEVEL_DIM_TERTIARY);
}

/**
 * @brief Deletes grating objecgts.
 */
void CScotomaMapStimulus::CleanUpGratings()
{
	if (m_primarySpec)      delete m_primarySpec;
	if (m_secondarySpec)    delete m_secondarySpec;
	if (m_tertiarySpec)     delete m_tertiarySpec;
	if (m_verticalGrating)   delete m_verticalGrating;
	if (m_horizontalGrating) delete m_horizontalGrating;
	if (m_dualGrating)       delete m_dualGrating;
}

/**
 * @brief Configure LEVEL_CTR_TARG and LEVEL_PER_TARG for this trial.
 *
 * Writes the buffer to the card and sets it as the current palette.
 *
 * @param currentTrial Reference to current CScotomaTrial for access to
 * isCenterRed, isPeriphRed, isPeriphBlue, etc.
 */
void CScotomaMapStimulus::SetupBufferForTrial(CScotomaTrial& currentTrial)
{
	VSGTRIVAL& centerColor = m_lutbuf[LEVEL_CTR_TARG];
	VSGTRIVAL& periphColor = m_lutbuf[LEVEL_PER_TARG];
	
	if (g_pScotomaMap->getCenterColorRadio() == CScotomaMap::COLOR_GLOBAL) {
		centerColor.a = ((double)g_pSettings->getRed())/255.0f;
		centerColor.b = ((double)g_pSettings->getGreen())/255.0f;
		centerColor.c = ((double)g_pSettings->getBlue())/255.0f;
	} 
	else if (currentTrial.isCenterRed()) {
		centerColor = m_lutbuf[LEVEL_PRIMARY];
	} 
	else {
		centerColor = m_lutbuf[LEVEL_SECONDARY];
	}

	if (currentTrial.isPeriphRed()) {
		periphColor = m_lutbuf[LEVEL_PRIMARY];
	} 
	else if (currentTrial.isPeriphBlue()) {
		periphColor = m_lutbuf[LEVEL_SECONDARY];
	} 
	else {
		periphColor = m_lutbuf[LEVEL_TERTIARY];;
	}

	vsgLUTBUFFERWrite(0, &m_lutbuf);
	vsgLUTBUFFERtoPalette(0);
}

/**
 * @brief Display the cross, then the cross plus periph target, based on 
 * behaviorial contingencies.
 * @return True for success; false otherwise. 
 */
bool CScotomaMapStimulus::DisplayStimulus()
{
	double centerXDeg, centerYDeg;
	int    behaveContingencyPulse = 20;
	bool   resetFlagged = false;
	char   buffer[1024];
	DWORD  result;
	int    timeperiod;
	TRIAL_AUTO_PTR currentTrial;
	/* Trigger debugging behavior */
	bool bDebugLabelsOn = (BST_CHECKED == 
		g_pScotomaMapDlg->m_debugLabelsOn.GetCheck());
	bool bDebugResponderOn = (BST_CHECKED == 
		g_pScotomaMapDlg->m_debugResponserOn.GetCheck());

	m_useNoise = g_pScotomaMap->getUseNoise();
	if (g_pScotomaMap->getInterleaveNoise()) {
		m_lastTrialNoise = !m_lastTrialNoise;
		m_useNoise = m_lastTrialNoise;
	}

	/* Store the center location for use across display calls:
	 *   1. Display fixation cross only, and
	 *   2. Display cross w/peripheral target. 
	 * Add stim offset to the stored value, as this location is reported 
	 * via AnalogOut.*/
	g_pScotomaMap->getRandomCtrLocation(&centerXDeg, &centerYDeg);
	centerXDeg += g_pSettings->getXOffset();
	centerYDeg += g_pSettings->getYOffset();

	/* Get next trial within mutex handle; we're in a different thread from
	 * the CScotomaMap model. */
	{
		SmartLock scotomaMapLock(*g_pScotomaMap);
		currentTrial = TRIAL_AUTO_PTR(GetNextTrial()->clone());
	}

	/* Set up the LUTBUFFER with per-trial colors and store it to VSG. */
	SetupBufferForTrial(*currentTrial);

	/* Used for drawing the same BG throughout a trial. */
	m_noiseBgController->initReferenceBg(vsgVIDEOPAGE, DRAW_PAGE_REF_BG);
	
	/* Analog output for trial type */
	if (g_pScotomaMap->getCenterColorRadio() == CScotomaMap::COLOR_GLOBAL) {
		etu::AnalogOut(A_OUT_CTR_TARG_COLOR, (float)0.0, true);
	} 
	else if (currentTrial->isCenterRed()) {
		etu::AnalogOut(A_OUT_CTR_TARG_COLOR, (float)VOLTS_COLOR_RED, true);
	} 
	else {
		etu::AnalogOut(A_OUT_CTR_TARG_COLOR, (float)VOLTS_COLOR_BLUE, true);
	}

	if (currentTrial->isPeriphRed()) {
		etu::AnalogOut(A_OUT_PER_TARG_COLOR, (float)VOLTS_COLOR_RED, true);
	} 
	else if (currentTrial->isPeriphBlue()) {
		etu::AnalogOut(A_OUT_PER_TARG_COLOR, (float)VOLTS_COLOR_BLUE, true);
	} 
	else if (currentTrial->isPeriphPurple()) {
		etu::AnalogOut(A_OUT_PER_TARG_COLOR, (float)VOLTS_COLOR_PURPLE, true);
	} 
	else {
		etu::AnalogOut(A_OUT_PER_TARG_COLOR, (float)VOLTS_COLOR_BLANK, true);
	}

	/* Behavioral contingency counter should (always) be reset */
	vsgIOWriteDigitalOut(0xFFFF, D_OUT_BEHAV_CONT_RESET);
	WaitUntil(behaveContingencyPulse);
	vsgIOWriteDigitalOut(0x0000, D_OUT_BEHAV_CONT_RESET);

	// --------------------------- TRIAL DELAY ---------------------------
	// --------------------------- fixation cross ---------------------------
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)centerXDeg / g_pSettings->getDegPerVolt(), true);
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)centerYDeg / g_pSettings->getDegPerVolt(), true);
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)-9.95, true);
	etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)-9.95, true);
	
	vsgIOWriteDigitalOut(0x0000, D_OUT_TRIAL_STATE);

	m_currentPage = 1 - m_currentPage;
	vsgSetDrawPage(vsgVIDEOPAGE, m_currentPage, vsgBACKGROUND);

	DrawCrossWithBG(currentTrial->isCenterRed(), centerXDeg, centerYDeg);

	if (bDebugLabelsOn) {
		vsgSetPen1(0); vsgSetPen2(2); vsgDrawString(0, -300, "TRIAL DELAY");
	}

	/* Avoid flicker by updating after dot is off the screen. Cross flicker
	 * is okay. */
	vsgPresent();

	/* Pause turns dot off */
	while (g_pScotomaMapDlg->m_Pause2.GetCheck() == BST_CHECKED) {
		WaitUntil(1);
	}

	m_flashTimer.StartTimer();
	timeperiod = GetCenterTimePeriod(); 
	while ((m_flashTimer.CheckTimer() < timeperiod) && !bEndThread) {
		if (bDebugResponderOn) {
			resetFlagged = false;
		} 
		else {
			resetFlagged = vsgIOReadDigitalIn() & D_IN_NEXT_STIM;
		}
		EyePosResample(!currentTrial->isCenterRed());
		WaitUntil(1);
		if (resetFlagged) {
			/* Loop here until fixation criteria are met */
			m_flashTimer.CheckTimerWithReset();

			/* If reset happens, do not save value from vsgDIG0 line */
			resetFlagged = false;
		}
	}

	if (bEndThread) {
		return true;
	}

	// --------------------------- TRIAL FLASH ---------------------------
	// --------------------------- fixation cross + dot ---------------------------
	double pixPeriphX, pixPeriphY;
	double screenXOffset, screenYOffset;
	double stimXOffset, stimYOffset;
	double periphSize;
	
	m_periphXDeg = currentTrial->getXDeg();
	m_periphYDeg = currentTrial->getYDeg();
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, m_periphXDeg, vsgPIXELUNIT, &pixPeriphX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, m_periphYDeg, vsgPIXELUNIT, &pixPeriphY);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &stimXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &stimYOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pScotomaMap->getPeriphDotSize(), vsgPIXELUNIT, &periphSize);

	m_currentPage = 1 - m_currentPage;
	vsgSetDrawPage(vsgVIDEOPAGE, m_currentPage, vsgBACKGROUND);

	DrawCrossWithBG(currentTrial->isCenterRed(), centerXDeg, centerYDeg);
	
	DrawPeripheralStim(pixPeriphX + screenXOffset + stimXOffset,
		-1 * pixPeriphY + screenYOffset + stimYOffset, 2.0 * periphSize,
		*currentTrial);

	if (bDebugLabelsOn) {vsgSetPen1(0); vsgSetPen2(2); vsgDrawString(0, -300, "TRIAL FLASH"); }

	vsgPresent();
	m_flashTimer.StartTimer();

	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, 
		(float)(m_periphXDeg + g_pSettings->getXOffset()) / g_pSettings->getDegPerVolt(), 
		true);
	etu::AnalogOut(A_OUT_PER_TARG_VERT, 
		(float)(m_periphYDeg + g_pSettings->getYOffset()) / g_pSettings->getDegPerVolt(), 
		true);

	/* Send textmark */
	sprintf(buffer, "cRGB: %.2f, %.2f, %.2f pRGB: %.2f, %.2f, %.2f|0x0%x/", 
		m_lutbuf[LEVEL_CTR_TARG].a, m_lutbuf[LEVEL_CTR_TARG].b, m_lutbuf[LEVEL_CTR_TARG].c, 
		m_lutbuf[LEVEL_PER_TARG].a, m_lutbuf[LEVEL_PER_TARG].b, m_lutbuf[LEVEL_PER_TARG].c, 
		currentTrial->getStimCode());
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
	
	/* Pause w/dot on */
	while (g_pScotomaMapDlg->m_Pause.GetCheck() == BST_CHECKED) {
		WaitUntil(1);
	}

	/* Now check the timer */
	timeperiod = GetFlashTimePeriod();
	while (!resetFlagged && (m_flashTimer.CheckTimer() < timeperiod) && !bEndThread) {
		if (bDebugResponderOn) {
			resetFlagged = false;
		} 
		else {
			resetFlagged = vsgIOReadDigitalIn() & D_IN_NEXT_STIM;
		}
		EyePosResample(!currentTrial->isCenterRed());
		WaitUntil(1);
	}
	if (resetFlagged) {
		DoTimeout(g_pScotomaMap->getMisbehaviorDelay());
		
		/* Aborted trial, send response = 4, just for John */
		int response = 4;
		sprintf(buffer, "response=%d|0x%2x/\0", response, 48+response);
		g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));

		/* Do not advance to the next trial; return now before advancing
		 * m_currentTrial iterator. */
		return true;
	}

	if (bEndThread) {
		return true;
	}

	// --------------------------- TRIAL CHOICE ---------------------------
	// --------------------------- no fixation cross ---------------------------
	vsgIOWriteDigitalOut(0xFFFF, D_OUT_TRIAL_STATE);

	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)-9.95, true);
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)-9.95, true);

	/* No peripheral target */
	if (!g_pScotomaMap->getDisplayDuringChoice()) {
		etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)-9.95, true);
		etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)-9.95, true);
	}

	m_currentPage = 1 - m_currentPage;
	vsgSetDrawPage(vsgVIDEOPAGE, m_currentPage, vsgBACKGROUND);
	
	if (m_useNoise && g_pScotomaMap->getSameBG()) {
		m_noiseBgController->drawReferenceBg();
	}
	else if (m_useNoise) {
		m_noiseBgController->drawBg();
	}

	if (g_pScotomaMap->getDisplayDuringChoice()) {
		DrawPeripheralStim(pixPeriphX + screenXOffset + stimXOffset,
			-1 * pixPeriphY + screenYOffset + stimYOffset, 2.0 * periphSize,
			*currentTrial);
	}

	if (bDebugLabelsOn) {
		vsgSetPen1(LEVEL_CTR_TARG); 
		vsgSetPen2(LEVEL_BG_CONTR); 
		vsgDrawString(0, -300, "TRIAL CHOICE"); 
	}

	vsgPresent();

	m_flashTimer.StartTimer();
	bool bMoved = false;
	bool right = false;//moved joystick to the right?
	bool bCorrect = false;
	g_pScotomaMapDlg->response = -1;
	if (g_pScotomaMap->getRewardScheme() == CScotomaMap::REWARD_TRAINING) {
		/* Delay until the monkey gives the correct answer */
		while (!bCorrect && (m_flashTimer.CheckTimer() < g_pScotomaMap->getResponseDelay()) && !bEndThread) {
			if (g_pScotomaMap->getUseKeyboard()) {
				bMoved = g_pScotomaMapDlg->response != -1;
				right = g_pScotomaMapDlg->response == 0;
			} 
			else {
				bMoved = vsgIOReadDigitalIn() & D_IN_JOY_MOVED;
				right = vsgIOReadDigitalIn() & D_IN_JOY_RIGHT;
			}
			bCorrect = bMoved && IsCorrect(*currentTrial, right);
			WaitUntil(1);
		}
	} 
	else {
		while (!bMoved && (m_flashTimer.CheckTimer() < g_pScotomaMap->getResponseDelay()) && !bEndThread) {
			if (bDebugResponderOn) {
				bMoved = true;
			} 
			else if (g_pScotomaMap->getUseKeyboard()) {
				bMoved = g_pScotomaMapDlg->response != -1;
			} 
			else {
				bMoved = vsgIOReadDigitalIn() & D_IN_JOY_MOVED;
			}
			WaitUntil(1);
		}
	}

	vsgIOWriteDigitalOut(0x0000, D_OUT_TRIAL_STATE);
	
	/* Now figure out how to branch */
	if (bMoved) {
		/* Check for correct or incorrect answer */
		right = vsgIOReadDigitalIn() & D_IN_JOY_RIGHT;
		int response = (right)? 0 : 2; //0=red 2=blue 3=blank
		if (bDebugResponderOn) response = ((rand() % 2) == 0) ? 0 : 2; // Random
		if (g_pScotomaMap->getUseKeyboard()) response = g_pScotomaMapDlg->response;
		
		/* Update the display on the dialog window */
		UpdateGraphs(response, *currentTrial, m_periphXDeg, m_periphYDeg);

		/* Send textmark to verify the determined choice */
		sprintf(buffer, "response=%d|0x%2x/\0", response, 48+response);
		g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
		
		/* Reward (or not) based on current reward scheme; also reward if 
		 * bCorrect from training mode.  Do not reward an answer to a blank 
		 * trial (code 6,7) */
		if ((!currentTrial->isPeriphBlank()) && (bCorrect || IsCorrect(*currentTrial, right))) {
			if (g_pScotomaMap->getRewardDelay() > 0) {
				/* Ccorrect: show blank reward screen */
				m_currentPage = 1 - m_currentPage;
				vsgSetDrawPage(vsgVIDEOPAGE, m_currentPage, vsgBACKGROUND);
				if (m_useNoise && g_pScotomaMap->getSameBG()) {
					m_noiseBgController->drawReferenceBg();
				}
				else if (m_useNoise) {
					m_noiseBgController->drawBg();
				}

				if (bDebugLabelsOn) {
					vsgSetPen1(LEVEL_CTR_TARG); 
					vsgSetPen2(LEVEL_BG_CONTR); 
					vsgDrawString(0, -300, "REWARD");
				}
				vsgPresent();
			}

			/* Turn off fixation target location */
			etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)-9.95, true);
			etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)-9.95, true);

			/* Send remote reward */
			vsgIOWriteDigitalOut(0xFFFF, D_OUT_FEED);
			WaitUntil(200);
			vsgIOWriteDigitalOut(0x0000, D_OUT_FEED);

			/* Delay */
			WaitUntil(g_pScotomaMap->getRewardDelay());
		} 
		else {
			/* Incorrect */
			DoTimeout(g_pScotomaMap->getWrongDelay());
		}
	} 
	else {
		/* Timeout on lever, even though he was fixating */
		int response = 3;

		/* Update the display on the dialog window */
		UpdateGraphs(response, *currentTrial, m_periphXDeg, m_periphYDeg);

		sprintf(buffer, "response=%d|0x%2x/\0", response, 48+response);
		g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));

		/* @TODO - timeout in training mode? */
		if (currentTrial->isPeriphBlank()) {
			/* Blank trial => correct! */
			if (g_pScotomaMap->getRewardDelay() > 0) {
				m_currentPage = 1 - m_currentPage;
				vsgSetDrawPage(vsgVIDEOPAGE, m_currentPage, vsgBACKGROUND);
				if (m_useNoise && g_pScotomaMap->getSameBG()) {
					m_noiseBgController->drawReferenceBg();
				}
				else if (m_useNoise) {
					m_noiseBgController->drawBg();
				}

				if (bDebugLabelsOn) { 
					vsgSetPen1(LEVEL_PRIMARY); 
					vsgSetPen2(LEVEL_BG_CONTR); 
					vsgDrawString(0, -300, "REWARD"); 
				}
				vsgPresent();
			}

			/* Turn off fixation target location */
			etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)-9.95, true);
			etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)-9.95, true);

			/* Send remote reward */
			vsgIOWriteDigitalOut(0xFFFF, D_OUT_FEED);
			//was 100ms. somehow daqer is missing the signal even though seq 
			// runs in est 11ms?? changed 10.23.08. see above too
			WaitUntil(200);
			vsgIOWriteDigitalOut(0x0000, D_OUT_FEED);

			/* Delay */
			WaitUntil(g_pScotomaMap->getRewardDelay());
		} 
		else {
			DoTimeout(g_pScotomaMap->getWrongDelay());
		}
	}
	
	/* Done with a trial; go to the next one */
	{
		SmartLock scotomaMapLock(*g_pScotomaMap);
		++m_currentTrial;
	}

	return true;
}

/**
 * @brief Inform stimulus to start over with first trial.
 */
void CScotomaMapStimulus::ResetTrialSequence() 
{ 
	m_currentTrial = g_pScotomaMap->getTrials().begin(); 
}

/**
 * @brief Determine if subject response for current trial was correct and 
 * should be rewarded.
 *
 * The concept of correct is entirely the concept of reward.  A "correct" 
 * reply is either color for a purple trial, but is only red for a red trial,
 * only blue for a blue trial, and is absence of reply for a blank trial.
 * This function returns true to indicate that a reward should be given,
 * false to indicate that it should not be given.  The reward scheme chosen
 * by the user is considered, including randomization and so forth.
 *
 * Function does not handle case of no reply; those are detected before 
 * calling this function.
 *
 * @param trial Current trial.
 * @param movedRight True if the response was a rightward lever pull, false
 * if a leftward lever pull.
 */
bool CScotomaMapStimulus::IsCorrect(CScotomaTrial& trial, bool movedRight) 
{
	bool randomRewDecision = rand() % 100 < g_pScotomaMap->getTertiaryRewardPct();
	const bool definiteTrial = (trial.isPeriphBlue() || trial.isPeriphRed());
	const bool ambiguousTrial = !definiteTrial;
	const bool correctPull = ((trial.isPeriphRed() && movedRight) || 
		(trial.isPeriphBlue() && !movedRight));
	
	switch (g_pScotomaMap->getRewardScheme()) {
		case CScotomaMap::REWARD_ANY: 
			return true; 
		
		case CScotomaMap::REWARD_RANDOM: /* Randomly for correct answers */
			return (ambiguousTrial || (definiteTrial && correctPull) ?
				randomRewDecision : false);

		case CScotomaMap::REWARD_TRAINING: /* For correct catch trials */
		case CScotomaMap::REWARD_TERTIARYRANDOM: /* Randomly for Purple */
		default:
			return (ambiguousTrial ? randomRewDecision : correctPull);
	}
}

/**
 * @brief Get the period to display the center fixation cross, in ms.
 */
int CScotomaMapStimulus::GetCenterTimePeriod()
{
	return (g_pScotomaMap->getRandomizeDisplayTime() ? 
		GetRandomizedTimePeriod(g_pScotomaMap->getDisplayRandMin(), 
								g_pScotomaMap->getDisplayRandMax())
		: g_pScotomaMap->getDisplayTime());
}

/**
 * @brief Get the period to flash the peripheral stimulus on screen, in ms.
 */
int CScotomaMapStimulus::GetFlashTimePeriod()
{
	return (g_pScotomaMap->getRandomizeFlashInterval() ? 
		GetRandomizedTimePeriod(g_pScotomaMap->getFlashIntervalRandMin(), 
								g_pScotomaMap->getFlashIntervalRandMax())
		: g_pScotomaMap->getFlashInterval());
}

/**
 * @brief Get a random time period between min and max.
 * @param min Time period, in ms.
 * @param max Time period, in ms.
 */
int CScotomaMapStimulus::GetRandomizedTimePeriod(int min, int max)
{
	return (max > min ? min + (rand() % (max - min)) : min);
}

/**
 * @brief Draw fixation cross w/background pattern and/or occlusion.
 *
 * Draws the center fixation cross by respecting the current background 
 * setting.  Does not modify Analog Out channels.  The caller is responsible 
 * for specifying the correct center location, and thus the caller is also
 * responsible for modifying Analog Out.
 *
 * @param drawPrimary True to draw the primary color (red).  False to draw the 
 * secondary color (blue).  Actual color is specified by the current trial 
 * object.
 * @param centerXDeg X coordinate of the center fixation cross, in degrees, 
 * with the stim offset already added, but the screen offset not.
 * @param centerYDeg Y coordinate of the center fixation cross, in degrees,
 * with the stim offset already added, but the screen offset not.
 */
void CScotomaMapStimulus::DrawCrossWithBG(bool drawPrimary, double centerXDeg, double centerYDeg) 
{
	double centerXPix, centerYPix, centerSizePix;
	VsgExt::IGrating* gratingDesc = drawPrimary ? m_primarySpec : m_secondarySpec;
	VsgExt::NoiseBgController* controller = NULL;
	VsgExt::NoiseColorMap* map = NULL;

	/* (Re-)use reference background if the "do not switch background between 
	 * trials" feature is on. */
	if (m_useNoise && g_pScotomaMap->getSameBG()) {
		m_noiseBgController->drawReferenceBg();
	}
	else if (m_useNoise) {
		m_noiseBgController->drawBg();
	}
	
	/* Screen offsets only; stim offsets already applied to function params */
	centerXDeg += g_pSettings->getScreenXOffset();
	centerYDeg += g_pSettings->getScreenYOffset();

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, centerXDeg, 
		vsgPIXELUNIT, &centerXPix);
	/* Y pixels increase as Y degrees decrease (as Y goes down screen) */
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -1 * centerYDeg, 
		vsgPIXELUNIT, &centerYPix);
	g_pSettings->tanScreenUnit2Unit(
		vsgDEGREEUNIT, g_pScotomaMap->getCenterDotSize(), 
		vsgPIXELUNIT, &centerSizePix);
	
	/* Draw center point (only) */
	if (g_pScotomaMap->getCenterColorRadio() != CScotomaMap::COLOR_GLOBAL && 
		g_pScotomaMap->getUseNoise() && !g_pScotomaMap->getOccludeBG()) 
	{
		controller = m_noiseBgController;
		map = m_noiseColorMap;
	}

	VsgExt::drawCross(centerXPix, centerYPix, centerSizePix, 
		g_pScotomaMap->getCrossPix(), *gratingDesc, map, controller);
}

/**
 * @brief Give the research subject a time out.
 * 
 * A time out is used as a no-reward indicator.  During the time out period,
 * the screen is blank.
 *
 * @param mstime Time for time out, in milliseconds.
 */
void CScotomaMapStimulus::DoTimeout(long mstime) 
{
	if (mstime > 0) {
		m_currentPage = 1 - m_currentPage;
		vsgSetDrawPage(vsgVIDEOPAGE, m_currentPage, vsgBACKGROUND);
		vsgSetPen1(LEVEL_TIME_OUT);
		vsgDrawRect(0, 0, m_screenWidthPix, m_screenHeightPix);
		vsgPresent();
	}
	vsgLUTBUFFERWrite(0, &m_lutbuf);//update after dot is off screen to avoid flicker on dot
	vsgLUTBUFFERtoPalette(0);//if the cross flickers, it's ok because it's supposed to change anyway

	//reset digital, analog outputs
	vsgIOWriteDigitalOut(0x0000, D_OUT_TRIAL_STATE);
	vsgIOWriteDigitalOut(0x0000, D_OUT_BEHAV_CONT_RESET);
	vsgIOWriteDigitalOut(0x0000, D_OUT_FEED);
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)-9.95, true);
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)-9.95, true);
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, (float)-9.95, true);
	etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)-9.95, true);
	m_flashTimer.CheckTimerWithReset();
	m_flashTimer.StartTimer();
	while ((m_flashTimer.CheckTimer() < mstime) && !bEndThread) { WaitUntil(1); }
}

/**
 * @brief Update the graphs on g_pScotomaMapDlg with the most recent response 
 * data.
 * @param responseCode As per the following:
 *     -- 0 Response "red"
 *     -- 1 Response "both"
 *     -- 2 Response "blue"
 *     -- 3 Response "blank"
 *     -- 4 Response "ignored"
 * @param trial The object for the current trial.
 * @param xDeg The X coordinate of this response, in degrees.
 * @param yDeg The Y coordinate of this response, in degrees.
 */
void CScotomaMapStimulus::UpdateGraphs(int responseCode, CScotomaTrial& trial, double xDeg, double yDeg) 
{
	SmartLock scotomaMapLock(*g_pScotomaMap);
	SmartLock scotomaMapDlgLock(*g_pScotomaMapDlg);

	char buffer[1024];buffer[0]=0;
	char buffer2[1024];buffer2[0]=0;
	char buffer3[1024];buffer3[0]=0;
	char buffer4[1024];buffer4[0]=0;
	char buffer5[1024];buffer5[0]=0;
	if (false) {//test round
		sprintf(buffer, "0.5 = %d\n-0.5 = %d\n10.1 = %d\n-10.1 = %d\n10.9 = %d\n-10.9 = %d\n10.5 = %d\n-10.5 = %d\0",
			etu::Round(0.5),
			etu::Round(-0.5),
			etu::Round(10.1),
			etu::Round(-10.1),
			etu::Round(10.9),
			etu::Round(-10.9),
			etu::Round(10.5),
			etu::Round(-10.5)
			);
		AfxMessageBox(buffer);
	}

	int i = g_pScotomaMap->getNumStepsH()+ etu::Round(xDeg/g_pScotomaMap->getStepSize());
	if (i<0) {
		i=0;
	}
	if (i > 2*g_pScotomaMap->getNumStepsH()) {
		i=2*g_pScotomaMap->getNumStepsH();
	}

	int j = g_pScotomaMap->getNumStepsV()+ etu::Round(yDeg/g_pScotomaMap->getStepSize());
	if (j<0) {
		j=0;
	}
	if (j > 2*g_pScotomaMap->getNumStepsV()) {
		j=2*g_pScotomaMap->getNumStepsV();
	}
	int locationIndex = i*(1+2*g_pScotomaMap->getNumStepsV())+j;

	if (trial.isPeriphPurple()) {
		if (trial.isCenterRed()) {//if right eye on center
			g_pScotomaMapDlg->reptotal++;
			switch(responseCode) {
				case 0: g_pScotomaMapDlg->re_r[locationIndex]++; break;
				case 2: g_pScotomaMapDlg->re_b[locationIndex]++; break;
				case 1: g_pScotomaMapDlg->re_o[locationIndex]++; break;
				default: break;
			}
		} 
		else {//left eye
			g_pScotomaMapDlg->leptotal++;
			switch(responseCode) {
				case 0: g_pScotomaMapDlg->le_r[locationIndex]++; break;
				case 2: g_pScotomaMapDlg->le_b[locationIndex]++; break;
				case 1: g_pScotomaMapDlg->le_o[locationIndex]++; break;
				default: break;
			}
		}
	} 
	else {//catch trial!
		//Beep(100,100);
		if (trial.isPeriphBlank()) {
			//blank
			g_pScotomaMapDlg->catch_n_total++;
			if (responseCode == 3) g_pScotomaMapDlg->catch_n += 1;
		} 
		else if (trial.isPeriphRed()) {
			//red dot
			g_pScotomaMapDlg->catch_r_total++;
			if (responseCode == 0) g_pScotomaMapDlg->catch_r += 1;
			if (trial.isCenterRed()) {//red cross
				if (responseCode == 0) {
					g_pScotomaMapDlg->re_red_r[locationIndex]++;//called it red
				} 
				else if (responseCode == 2) {
					g_pScotomaMapDlg->re_red_b[locationIndex]++;//called it blue
				} 
				else {
					g_pScotomaMapDlg->re_red_o[locationIndex]++;//who knows
				}
			} 
			else {//blue cross
				if (responseCode == 0) {
					g_pScotomaMapDlg->le_red_r[locationIndex]++;//called it red
				} 
				else if (responseCode == 2) {
					g_pScotomaMapDlg->le_red_b[locationIndex]++;//called it blue
				} 
				else {
					g_pScotomaMapDlg->le_red_o[locationIndex]++;//who knows
				}
			}
		} 
		else if (trial.isPeriphBlue()) {
			//blue dot
			g_pScotomaMapDlg->catch_b_total++;
			if (responseCode == 2) g_pScotomaMapDlg->catch_b += 1;
			if (trial.isCenterRed()) {//red cross
				if (responseCode == 0) {
					g_pScotomaMapDlg->re_blue_r[locationIndex]++;//called it red
				} 
				else if (responseCode == 2) {
					g_pScotomaMapDlg->re_blue_b[locationIndex]++;//called it blue
				} 
				else {
					g_pScotomaMapDlg->re_blue_o[locationIndex]++;//who knows
				}
			} else {//blue cross
				if (responseCode == 0) {
					g_pScotomaMapDlg->le_blue_r[locationIndex]++;//called it red
				} else if (responseCode == 2) {
					g_pScotomaMapDlg->le_blue_b[locationIndex]++;//called it blue
				} else {
					g_pScotomaMapDlg->le_blue_o[locationIndex]++;//who knows
				}
			}
		}
	}

	//update catch trial things
	int rr=0, rrtotal=0, lr=0, lrtotal=0, rb=0, rbtotal=0, lb=0, lbtotal=0;
	for (i=0; i<(1+2*g_pScotomaMap->getNumStepsH())*(1+2*g_pScotomaMap->getNumStepsV()); i++) {
		rb += g_pScotomaMapDlg->re_blue_b[i];
		rbtotal += g_pScotomaMapDlg->re_blue_r[i] + g_pScotomaMapDlg->re_blue_b[i] + g_pScotomaMapDlg->re_blue_o[i];
		lb += g_pScotomaMapDlg->le_blue_b[i];
		lbtotal += g_pScotomaMapDlg->le_blue_r[i] + g_pScotomaMapDlg->le_blue_b[i] + g_pScotomaMapDlg->le_blue_o[i];
		rr += g_pScotomaMapDlg->re_red_r[i];
		rrtotal += g_pScotomaMapDlg->re_red_r[i] + g_pScotomaMapDlg->re_red_b[i] + g_pScotomaMapDlg->re_red_o[i];
		lr += g_pScotomaMapDlg->le_red_r[i];
		lrtotal += g_pScotomaMapDlg->le_red_r[i] + g_pScotomaMapDlg->le_red_b[i] + g_pScotomaMapDlg->le_red_o[i];
	}
	sprintf(buffer, "Left: %d/%d %d%%   Right: %d/%d %d%%\0", lr, lrtotal, ((lrtotal > 0)? lr*100/lrtotal:0), rr, rrtotal, ((rrtotal > 0)? rr*100/rrtotal:0) );
	sprintf(buffer2, "Left: %d/%d %d%%   Right: %d/%d %d%%\0", lb, lbtotal, ((lbtotal > 0)? lb*100/lbtotal:0), rb, rbtotal, ((rbtotal > 0)? rb*100/rbtotal:0) );
	sprintf(buffer3, "Blank: %d/%d %d%%\0", g_pScotomaMapDlg->catch_n, g_pScotomaMapDlg->catch_n_total, ((g_pScotomaMapDlg->catch_n_total > 0)? g_pScotomaMapDlg->catch_n*100/g_pScotomaMapDlg->catch_n_total:0) );
	sprintf(buffer4, "Left: %d   Right: %d\0", g_pScotomaMapDlg->leptotal, g_pScotomaMapDlg->reptotal);
	sprintf(buffer5, "Total Trials: %d\0", lrtotal+rrtotal+lbtotal+rbtotal+ g_pScotomaMapDlg->catch_n_total + g_pScotomaMapDlg->leptotal + g_pScotomaMapDlg->reptotal);

	g_pScotomaMapDlg->m_CatchPctR.SetWindowText(buffer);
	g_pScotomaMapDlg->m_CatchPctB.SetWindowText(buffer2);
	g_pScotomaMapDlg->m_CatchPctN.SetWindowText(buffer3);
	g_pScotomaMapDlg->m_PurpleCount.SetWindowText(buffer4);
	g_pScotomaMapDlg->m_TotalTrials.SetWindowText(buffer5);

	//show updated image
	
	g_pScotomaMapDlg->Invalidate(FALSE);
}

/**
 * @brief Draw the peripheral dot, respecting noise BG and grating settings.
 *
 * Detects if there should be vertical separation; then calls its helper
 * function the appropriate number of times.
 *
 * @param xPix X coordinate in pixels.
 * @param yPix Y coordinate in pixels.
 * @param sizePix Size of peripheral stim in pixels.
 * @param trial Reference to current trial object, for access to center and 
 * peripheral color queries.
 */
void CScotomaMapStimulus::DrawPeripheralStim(double xPix, double yPix, 
											 double sizePix, CScotomaTrial& trial)
{
	if (trial.isPeriphPurple() && (g_pScotomaMap->getSeparationV() != 0)) {
		double sepy = 0;

		TRIAL_AUTO_PTR currentRed(trial.clone());
		TRIAL_AUTO_PTR currentBlue(trial.clone());

		currentRed->setStimCode(trial.isCenterRed() ? 
			CScotomaTrial::CODE_C_RED_P_RED : 
			CScotomaTrial::CODE_C_BLUE_P_RED);

		currentBlue->setStimCode(trial.isCenterRed() ? 
			CScotomaTrial::CODE_C_RED_P_BLUE : 
			CScotomaTrial::CODE_C_BLUE_P_BLUE);

		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pScotomaMap->getSeparationV(), vsgPIXELUNIT, &sepy);

		HelperDrawPeripheralStim(xPix, yPix - (sepy / 2.0), sizePix, *currentRed);
		HelperDrawPeripheralStim(xPix, yPix + (sepy / 2.0), sizePix, *currentBlue);
	} 
	else {
		HelperDrawPeripheralStim(xPix, yPix, sizePix, trial);
	}
}

/**
 * @brief Actually draw the peripheral dot, respecting noise BG and grating settings.
 *
 * @param xPix X coordinate in pixels.
 * @param yPix Y coordinate in pixels.
 * @param sizePix Size of peripheral stim in pixels.
 * @param trial Reference to current trial object, for access to center and 
 * peripheral color queries.
 */
void CScotomaMapStimulus::HelperDrawPeripheralStim(double xPix, double yPix,
												   double sizePix, 
												   CScotomaTrial& trial)
{
	/* Optionally set to non-null if BG is on */
	VsgExt::NoiseColorMap* colorMap = NULL;
	VsgExt::NoiseBgController* bgController = NULL;
	/* Must be set to non-null, either grating or solid color. */
	VsgExt::IGrating* pixelSpec = NULL;

	if (trial.isPeriphBlank()) return;

	if (g_pScotomaMap->getUseNoise() && !g_pScotomaMap->getOccludeBG() &&
		(g_pScotomaMap->getDotGratings() || !trial.isPeriphPurple()))
	{
		/* Need to respect BG when drawing. */
		bgController = m_noiseBgController;
		colorMap = m_noiseColorMap;
	}

	if (g_pScotomaMap->getDotGratings()) {
		pixelSpec = (trial.isPeriphRed() ? m_verticalGrating : 
			(trial.isPeriphBlue() ? m_horizontalGrating : m_dualGrating));
	}
	else {
		pixelSpec = (trial.isPeriphRed() ? m_primarySpec : 
			(trial.isPeriphBlue() ? m_secondarySpec : m_tertiarySpec));
	}

	if (g_pScotomaMap->getUseShapes() && trial.isPeriphRed()) {
		VsgExt::drawTriangle((int)xPix, (int)yPix, (int)sizePix, *pixelSpec, 
			colorMap, bgController);
	}
	else if (g_pScotomaMap->getUseShapes() && trial.isPeriphBlue()) {
		VsgExt::drawRectangle((int)xPix, (int)yPix, (int)sizePix, 
			(int)sizePix, *pixelSpec, colorMap, bgController);
	}
	else {
		VsgExt::drawCircle((int)xPix, (int)yPix, (int)sizePix, *pixelSpec, 
			colorMap, bgController);
	}
}

/**
 * @brief Get pointer to object for next trial, with coordinates correctly
 * modified for foveal enrichment (if active).
 *
 * @TODO Design could use improvement.  At present, it accesses a public
 * member array of CScotomaMap.  The CScotomaMapStimulus has a member int
 * to hold the index of the next trial; this int is modified throughout the
 * object's member functions as appropriate.
 *
 * This function will check the trial object to see if it's a foveal 
 * enrichment target, and if so, will adjust the correct target coordinates
 * for the type of fixation.  It uses the running average of eye positions
 * to apply the correct adjustment.
 *
 * @return Pointer to a CScotomaTrial.  Caller does <b>not</b> own the memory;
 * call the clone() method if you want your own.
 */
CScotomaTrial* CScotomaMapStimulus::GetNextTrial() 
{
	SmartLock scotomaMapLock(*g_pScotomaMap);
	double xMod = 0, yMod = 0;

	if (g_pScotomaMap->getTrials().size()) {
		/* Get trial */
		if (m_currentTrial >= g_pScotomaMap->getTrials().end()) {
			m_currentTrial = g_pScotomaMap->getTrials().begin();
		}
		
		CScotomaTrial* trial = (*m_currentTrial);

		/* Modify location if it's a foveal target
		 * @TODO Do not modify if it has already been modified! (even though 
		 * that case is unlikely given the long length of the sequence before 
		 * it loops) 
		 */
		if (trial->getStimClass() == CScotomaTrial::CLASS_FOVEA_FIXATING) {
			if (trial->isCenterRed()) {	//right eye at center
				xMod = arcx;
				yMod = -arcy;
			} 
			else {				//left eye at center
				xMod = alcx;
				yMod = -alcy;
			}
		} 
		else if (trial->getStimClass() == CScotomaTrial::CLASS_FOVEA_DEVIATED) {
			if (trial->isCenterRed()) {	//left eye deviated
				xMod = alpx;
				yMod = -alpy;
			} 
			else {				//right eye deviated
				xMod = arpx;
				yMod = -arpy;
			}
		}

		trial->setXDeg(trial->getXDeg() + xMod);
		trial->setYDeg(trial->getYDeg() + yMod);

		return trial;
	} 
	else return NULL;
}

/**
 * @brief Update the running sample of eye positions.
 *
 * The samples are kept in a series of arrays internally.  The arrays contain,
 * as best I can determine, a moving average of the moving average of eye
 * position.  I believe, on quick thought, that this achieves a weighted
 * moving average over a range of 2 * numSamples where the middle sample
 * is weighted most heavily, with a bell shape to either side.
 *
 * @param leftFixating True if left eye is fixating, right eye is deviated; 
 * false if right eye is fixating, left eye is deviated.
 */
void CScotomaMapStimulus::EyePosResample(bool leftFixating) 
{
	//resample from Dlg
	g_pEyeTrackingDlg->EyePosSample();
	if (leftFixating) {
		//remove old value from average and add new
		alcx += (g_pEyeTrackingDlg->avglx - pLCenterX[lcount]) / ((double)g_pSettings->getNSamples());
		pLCenterX[lcount] = g_pEyeTrackingDlg->avglx;
		alcy += (g_pEyeTrackingDlg->avgly - pLCenterY[lcount]) / ((double)g_pSettings->getNSamples());
		pLCenterY[lcount] = g_pEyeTrackingDlg->avgly;
		arpx += (g_pEyeTrackingDlg->avgrx - pRPeriphX[lcount]) / ((double)g_pSettings->getNSamples());
		pRPeriphX[lcount] = g_pEyeTrackingDlg->avgrx;
		arpy += (g_pEyeTrackingDlg->avgry - pRPeriphY[lcount]) / ((double)g_pSettings->getNSamples());
		pRPeriphY[lcount] = g_pEyeTrackingDlg->avgry;
		lcount = (lcount + 1) % (g_pSettings->getNSamples());
	} 
	else {
		//remove old value from average and add new
		arcx += (g_pEyeTrackingDlg->avgrx - pRCenterX[rcount]) / ((double)g_pSettings->getNSamples());
		pRCenterX[rcount] = g_pEyeTrackingDlg->avgrx;
		arcy += (g_pEyeTrackingDlg->avgry - pRCenterY[rcount]) / ((double)g_pSettings->getNSamples());
		pRCenterY[rcount] = g_pEyeTrackingDlg->avgry;
		alpx += (g_pEyeTrackingDlg->avglx - pLPeriphX[rcount]) / ((double)g_pSettings->getNSamples());
		pLPeriphX[rcount] = g_pEyeTrackingDlg->avglx;
		alpy += (g_pEyeTrackingDlg->avgly - pLPeriphY[rcount]) / ((double)g_pSettings->getNSamples());
		pLPeriphY[rcount] = g_pEyeTrackingDlg->avgly;
		rcount = (rcount + 1) % (g_pSettings->getNSamples());
	}
	
	//hijack text field to debug eye position sampling quickly.
	char buffer[1024];
	sprintf(buffer, "left: c=%.2f,%.2f p=%.2f,%.2f\nright: c=%.2f,%.2f p=%.2f,%.2f\0", alcx, alcy, alpx, alpy, arcx, arcy, arpx, arpy);
	g_pScotomaMapDlg->m_Countdown.SetWindowText(buffer);
}

/**
 * @brief Get type of stimulus as a string.
 *
 * Inherited method from CGenericStimulus.
 *
 * @return Name of stimulus as a CString object.
 */
CString CScotomaMapStimulus::StimulusType() 
{
	return "Scotoma Map Stimulus";
}

#endif // APP_EYE_TRACKING
