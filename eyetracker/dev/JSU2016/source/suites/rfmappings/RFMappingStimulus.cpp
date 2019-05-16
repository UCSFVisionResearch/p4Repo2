#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/
#include <memory>
#include <fstream>
#include "app/AppCore.h"
#include "RFMappingStimulus.h"
#include "app/EyeTrackingDlg.h"
#include "utils/Utils.h"
#include "utils/Grid.h"
#include "vsgext/vsgext.h"
#include <algorithm>
#include "sys/sys_concurrency.h"

/*****************************************************************************
 * COMMUNICATIONS:
 *****************************************************************************/

#define A_OUT_CTR_TARG_HORIZ  (0) /* 1401 in channel 3 */
#define A_OUT_CTR_TARG_VERT   (1) /* 1401 in channel 4 */
#define A_OUT_PER_TARG_HORIZ  (2) /* 1401 in channel 7 */
#define A_OUT_PER_TARG_VERT   (3) /* 1401 in channel 8; used to be 
                                   * A_OUT_PULSE */
#define A_OUT_CTR_TARG_COLOR  (4) /* VOLTS_COLOR_xxx */
#define A_OUT_PER_TARG_COLOR  (5) /* VOLTS_COLOR_xxx */
#define D_OUT_LOW_FOR_CTR      (vsgDIG0) // Complex multi-state w/vsgDIG2
#define D_OUT_LOW_FOR_ALT      (vsgDIG2) // Complex multi-state w/vsgDIG0
#define D_ON_TARGET  (vsgDIG6)
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
 * Only use 1 0 since we are only looking for on target signal and not following TriSacc timing conventions
 */

#define VOLTS_COLOR_RED		(5)
#define VOLTS_COLOR_BLUE	(-5)
#define VOLTS_COLOR_PURPLE	(0)
#define VOLTS_OFF (-10)

/******LUT Levels********************************/

#define LEVEL_CROSS (0)
#define LEVEL_STIM (1)
#define LEVEL_BG_CONTR	(2)
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

/*** Page Numbers*****************************************/
#define DRAW_PAGE_REF_BG (2)


/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/**
 @brief Initializes the newly instantiated object. 
 */
CRFMappingStimulus::CRFMappingStimulus() 
:	m_screenWidthPix(vsgGetScreenWidthPixels()), 
	m_screenHeightPix(vsgGetScreenHeightPixels()),
	m_crossXDeg(g_pRFMapping->getCrossX()),
	m_crossYDeg(g_pRFMapping->getCrossY()),
	m_crossXPix(0),
	m_crossYPix(0),
	m_crossDiameterPix(g_pRFMapping->getCrossDiameter()),
	m_crossThicknessPix(g_pRFMapping->getCrossThickness()),
	m_useBinocular(g_pRFMapping->getUseBinocular()),
	m_crossX2Deg(g_pRFMapping->getCrossX2()),
	m_crossY2Deg(g_pRFMapping->getCrossY2()),
	m_pageNum(0),
	m_useGrid(g_pRFMapping->getUseGrid()),
	m_usePulse(g_pRFMapping->usePulse()),
	m_useNoise(g_pRFMapping->getUseNoise()),
	m_displayTimeMs(g_pRFMapping->getDisplayTimeMs()),
	m_useBlanks(g_pRFMapping->getUseBlanks()),
	m_blankTimeMs(g_pRFMapping->getBlankTimeMs()),
	m_pulsed(false),
	m_pulseStateHigh(false),
	m_pulseTime(g_pRFMapping->getUseGrid() ? ((int)(g_pRFMapping->getDisplayTimeMs()/3.0)) : 150),
	m_stimulusChanged(false),
	m_screenOffsetXDeg(g_pSettings->getScreenXOffset()),
	m_screenOffsetYDeg(g_pSettings->getScreenYOffset()),
	m_messageSent(false),
	m_pStimLocationGenerator(NULL),
	m_bgColor(m_lutbuf[LEVEL_BG_CONTR]),  m_noiseColor(m_lutbuf[LEVEL_BG_CONTR+100]),
  m_noiseBgController(NULL), m_noiseColorMap(NULL),m_primarySpec(NULL),m_secondarySpec(NULL),m_tertiarySpec(NULL),
  m_swapCounter(0),m_swapInt(g_pRFMapping->getSwapInt()),m_fixEye(CRFTrial::EYE_RIGHT),
  targetAcquired(false), m_windowAllowancePix(g_pRFMapping->getWindowAllowancePix()), m_useWait(g_pRFMapping->getUseWait())
{
	double xxDeg, yyDeg;
	double frameTime;
	VSGLUTBUFFER lutbuf;
	
	bEndThread = false;

	if (!g_bIsInitialized) {
		if (vsgInit("") < 0) return;
		g_bIsInitialized = true;
		/* Set page width to 2048; display error if vsg.cfg isn't sync'ed on
		 * green. */
		vsgSetVideoMode(vsgPANSCROLLMODE);
	}

	memset(lutbuf, 0, 256 * sizeof(VSGTRIVAL));
	
	m_currentPage =0;
	frameTime = vsgGetSystemAttribute(vsgFRAMETIME);
	m_frameRate = 1000000.0 / frameTime;
	g_dFramerate = m_frameRate;
	g_dHalfFrameInterval = DWORD(floor(frameTime / 2000.0f));
	g_dSleepThreshold = (double)g_dHalfFrameInterval / 2.0f;

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);
	vsgSetFixationColour(&m_lutbuf[vsgFIXATION]);
	vsgSetBackgroundColour(&m_lutbuf[vsgBACKGROUND]);
	vsgSetPageWidth(2048); /* In case we didn't call vsgSetVideoMode. */
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(m_screenWidthPix / 2, m_screenHeightPix / 2);
	vsgLUTBUFFERWrite(0, &lutbuf);
	vsgLUTBUFFERtoPalette(0);


	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG2);
	//Reset in case coming from behavior contingent suite
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG1);
	Sleep(100);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getCrossDiameter(), 
		vsgPIXELUNIT, &m_crossDiameterPix);

	vsgSetDrawPage(vsgVIDEOPAGE, m_pageNum, vsgBACKGROUND);
	/* Populate colors generally; per-trial colors are populated in 
	 * DisplayStimulus. */
	InitLutBuffer();
	InitNoiseHandling();
	InitGratings();

	if (m_useGrid) {
			
			m_sequenceTimer.StartTimer();
			g_pRFMapping->GenerateTrials();
			m_stimulusChanged = true;
			//Set up first cross
			g_pRFMapping->setFixX(m_crossXDeg);
			g_pRFMapping->setFixY(m_crossYDeg);
		
	}else{
	//Set up color scheme for Positioning with Mouse
		VSGTRIVAL& centerColor = m_lutbuf[LEVEL_CROSS];
		VSGTRIVAL& periphColor = m_lutbuf[LEVEL_STIM];
		centerColor = m_lutbuf[LEVEL_PRIMARY];
		g_pRFMapping->setCrossColor(VOLTS_COLOR_RED);
		periphColor = m_lutbuf[LEVEL_PRIMARY];
		g_pRFMapping->setStimColor(VOLTS_COLOR_RED);
		g_pRFMapping->setFixX(m_crossXDeg);
		g_pRFMapping->setFixY(m_crossYDeg);
		m_fixEye =CRFTrial::EYE_RIGHT;
		m_stimulusChanged =true;

		vsgLUTBUFFERWrite(0, &m_lutbuf);
		vsgLUTBUFFERtoPalette(0);
		
		vsgSetBackgroundColour(&m_bgColor);
		vsgSetViewDistMM(g_pSettings->getViewDistance());
		vsgSetSpatialUnits(vsgPIXELUNIT);
		vsgSetDrawMode(vsgCENTREXY);

		NewDrawingPage(false);
		vsgPresent();
	}
	
	if (m_usePulse) {
		m_pulseTimer.StartTimer();
	}
	Sleep(100);
	NewDrawingPage(g_pRFMapping->getUseNoise());

	/* Call DrawStimulus directly to bypass pulse checking. */

	//DrawStimulus();

	
}

/** Trial running signal turned off and tear down. */
CRFMappingStimulus::~CRFMappingStimulus() 
{
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)((-99.0)/ g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)((-99.0)/ g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_CTR_TARG_COLOR, (float)0.0, true);
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ,(float)((-99.0)/ g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_PER_TARG_VERT,(float)((-99.0)/ g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_PER_TARG_COLOR,(float)((-99.0)/ g_pSettings->getDegPerVolt()));
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	vsgObjDestroy(g_stim);
	vsgObjDestroy(g_fixation);
	vsgObjDestroy(g_grating);
	vsgObjDestroy(g_stim2);

	CleanUpNoiseHandling();
	CleanUpGratings();
	/* Send stop mark */
	char buffer[80];
	sprintf(buffer, "RFMapping;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));

}

/** Coordinate stimulus display w/pulse. */
bool CRFMappingStimulus::DisplayStimulus() 
{

	CRFTrial* currentTrial;
	if (!m_messageSent) {
		g_pRFMapping->sendSettings(g_pSettings->theSerialComm);
		m_messageSent = true;
	}

	/* Move to the new location */
	if (m_useGrid && 
		(m_sequenceTimer.CheckTimer() > m_displayTimeMs)) 
	{
		m_useNoise = g_pRFMapping->getUseNoise();
		
		if (g_pRFMapping->getInterleaveNoise()) {
			m_lastTrialNoise = !m_lastTrialNoise;
			m_useNoise = m_lastTrialNoise;
		}
		if (m_useBlanks) {
			NewDrawingPage(m_useNoise);
			vsgPresent();
			m_sequenceTimer.CheckTimerWithReset();
			while (m_sequenceTimer.CheckTimer() < m_blankTimeMs) {
				Sleep(5);
			}
		}
		currentTrial = g_pRFMapping->GetNextTrial();
		if(!g_pRFMapping->getUseBinocular()){
			g_pRFMapping->setFixX(m_crossXDeg);
			g_pRFMapping->setFixY(m_crossYDeg);
			m_fixEye =CRFTrial::EYE_RIGHT; //uses primary for cross
		}else if(m_swapCounter<m_swapInt){
			NewDrawingPage(m_useNoise);
			vsgPresent();
			g_pRFMapping->setFixX(m_crossXDeg);
			g_pRFMapping->setFixY(m_crossYDeg);
			m_fixEye =CRFTrial::EYE_RIGHT;
			m_swapCounter++;
		}else if(m_swapCounter==m_swapInt){
			Clear(m_useNoise);
			vsgPresent();
			g_pRFMapping->setFixX(m_crossX2Deg);
			g_pRFMapping->setFixY(m_crossY2Deg);
			m_fixEye =CRFTrial::EYE_LEFT;
			m_swapCounter++;
			NewDrawingPage(m_useNoise);
			vsgPresent();
			etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)((g_pRFMapping->getFixX())/ g_pSettings->getDegPerVolt()));
			etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)((g_pRFMapping->getFixY()) / g_pSettings->getDegPerVolt()));
		}else if(m_swapCounter<2*m_swapInt){
			NewDrawingPage(m_useNoise);
			vsgPresent();
			g_pRFMapping->setFixX(m_crossX2Deg);
			g_pRFMapping->setFixY(m_crossY2Deg);
			m_fixEye =CRFTrial::EYE_LEFT;
			m_swapCounter++;
		}else{
			Clear(m_useNoise);
			vsgPresent();
			g_pRFMapping->setFixX(m_crossXDeg);
			g_pRFMapping->setFixY(m_crossYDeg);
			m_fixEye =CRFTrial::EYE_RIGHT;
			m_swapCounter =0;
			NewDrawingPage(m_useNoise);
			vsgPresent();
			etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)((g_pRFMapping->getFixX())/ g_pSettings->getDegPerVolt()));
			etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)((g_pRFMapping->getFixY()) / g_pSettings->getDegPerVolt()));
		}
		/* Set up the LUTBUFFER with per-trial colors. */
		SetupBufferForTrial(*currentTrial, m_fixEye);
		vsgLUTBUFFERWrite(0, &m_lutbuf);
		vsgLUTBUFFERtoPalette(0);
		
		vsgSetBackgroundColour(&m_bgColor);
		vsgSetViewDistMM(g_pSettings->getViewDistance());
		vsgSetSpatialUnits(vsgPIXELUNIT);
		vsgSetDrawMode(vsgCENTREXY);
		
		g_pRFMapping->setStimEye(currentTrial->getStimEye());
		g_pRFMapping->setXPosition(currentTrial->getPerXDeg());
		g_pRFMapping->setYPosition(currentTrial->getPerYDeg());

		g_pRFMappingDlg->updateFieldResults();
		m_sequenceTimer.CheckTimerWithReset();
		m_pulseTimer.CheckTimerWithReset();
		m_pulseStateHigh = true;
		m_stimulusChanged = true;
		
	}

	/* If timer pulse is in use, we redraw stimulus
	 *     1. When timer pulse goes high (turns on); and
	 *     2. When user changes the stimulus while the timer pulse is high.
	 * We draw a blank page when timer pulse goes low, and draw nothing again
	 * until timer pulse goes high.
	 */
	if (m_usePulse) {
		if (m_pulsed) {
			m_pulsed = false;
			if (m_pulseStateHigh) {
				DrawStimulus();
			}
			else {
				NewDrawingPage(m_useNoise);
				vsgPresent();
				etu::AnalogOut(A_OUT_PER_TARG_COLOR, VOLTS_OFF);

			}
		}
		else if (m_pulseStateHigh && m_stimulusChanged) {
			/* m_pulsed is false (implicitly) */
			DrawStimulus();
		}

		/* Check for a rising or falling edge Do not force a redraw by setting
		 * m_stimulusChanged to true. */
		if (m_pulseTimer.CheckTimer() > m_pulseTime) {
			m_pulsed = true;
			m_pulseStateHigh = !m_pulseStateHigh;
			m_pulseTimer.CheckTimerWithReset();
		}
	}
	/* If timer pulse is not in use, we redraw the stimulus only when we need
	 * to because of changes in the stimulus location, size, or shape. */
	else if (m_stimulusChanged) {
		//NewDrawingPage(m_useNoise);
		//vsgPresent();
		DrawStimulus();
	
		//g_pRFMappingDlg->printDebug("Display Stimulus sees m_stimulus Changed");

	}

	Sleep(1);
	return true;
}

/** Draw applicable stimulus. Send no output. */
void CRFMappingStimulus::DrawStimulus() 
{
	double xPix, yPix;
	double stimCtrXDeg =0;
	double stimCtrYDeg = 0;
	double perXDeg = g_pRFMapping->getXPosition();
	double perYDeg = g_pRFMapping->getYPosition();
	VsgExt::IGrating* pixelSpec = NULL;
	char bf[1024];
	double fixX = g_pRFMapping->getFixX(); 
	double fixY = g_pRFMapping->getFixY(); // cross location of current Trial
	double currFixEyePosX, currFixEyePosY; //Current Location of fixation Eye
	//Set up Color scheme based on Colors of trial
	if(g_pRFMapping->getStimColor() == VOLTS_COLOR_RED){
		pixelSpec = m_primarySpec;
	}else if(g_pRFMapping->getStimColor() == VOLTS_COLOR_BLUE){
		pixelSpec = m_secondarySpec;
	}else{
		pixelSpec = m_tertiarySpec;
	}

	VsgExt::NoiseColorMap* colorMap = NULL;
	VsgExt::NoiseBgController* bgController = NULL;

	if (m_useNoise && !g_pRFMapping->getOccludeBG()) {
		/* Need to respect BG when drawing. */
		bgController = m_noiseBgController;
		colorMap = m_noiseColorMap;
	}
	sprintf(bf,"useNoise:%s\n X:%f\n Y:%f\n", m_useNoise?"true":"false", fixX, fixY);
	g_pRFMappingDlg->printDebug(bf);
	NewDrawingPage(m_useNoise);
	vsgPresent();
	//If using Wait, Check if on Target if using Grid and Wait
	
	if (m_useGrid && m_useWait ) {
		g_pEyeTrackingDlg->EyePosSample();
		g_pRFMappingDlg->UpdateEyePositions(
			g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
			g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);
		if(m_fixEye ==CRFTrial::EYE_RIGHT){
			currFixEyePosX = g_pEyeTrackingDlg->avgrx;
			currFixEyePosY = g_pEyeTrackingDlg->avgry;
		}else{
			currFixEyePosX = g_pEyeTrackingDlg->avglx;
			currFixEyePosY = g_pEyeTrackingDlg->avgly;
		}

		if((abs(currFixEyePosX-fixX)<=m_windowAllowancePix)&(abs(currFixEyePosY-fixY)<=m_windowAllowancePix)){
			targetAcquired =true;
		}else{
			targetAcquired =false;
		}
	}
	//sprintf(bf,"Target:%s\n bEndthread:%s\n m_useWait:%s\nDeltaX:%d\nDeltaY:%d\n,",targetAcquired?"True":"False",bEndThread?"True":"False",m_useWait?"True":"False",abs(currFixEyePosX-fixX),abs(currFixEyePosY-fixY));
	//g_pRFMappingDlg->printDebug(bf);
	while(!bEndThread && (m_useGrid && m_useWait && !targetAcquired)){
		etu::AnalogOut(A_OUT_PER_TARG_HORIZ,(float)((-99.0)/ g_pSettings->getDegPerVolt()));
		etu::AnalogOut(A_OUT_PER_TARG_VERT, (float)((-99.0)/ g_pSettings->getDegPerVolt()));
		g_pEyeTrackingDlg->EyePosSample();
		g_pRFMappingDlg->UpdateEyePositions(
			g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
			g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);
		if(m_fixEye ==CRFTrial::EYE_RIGHT){
			currFixEyePosX = g_pEyeTrackingDlg->avgrx;
			currFixEyePosY = g_pEyeTrackingDlg->avgry;
		}else{
			currFixEyePosX = g_pEyeTrackingDlg->avglx;
			currFixEyePosY = g_pEyeTrackingDlg->avgly;
		}		
		if((abs(currFixEyePosX-fixX)<=m_windowAllowancePix)&&(abs(currFixEyePosY-fixY)<=m_windowAllowancePix)){
			targetAcquired =true;
		}else{
			targetAcquired =false;
		}
		//sprintf(bf,"Target:%s\n bEndthread:%s\n m_useWait:%s\nDeltaX:%d\nDeltaY:%d\n,",targetAcquired?"True":"False",bEndThread?"True":"False",m_useWait?"True":"False",abs(currFixEyePosX-fixX),abs(currFixEyePosY-fixY));
		//g_pRFMappingDlg->printDebug(bf);
	}
	
	//Peripheral dot in respet to stimCtrX/YDeg 
	//StimCtrX/YDeg is taken as the actual eye position of whichever that is stimulated
	//Monocular Mode is chosen based on Primary Eye selection
	//For Mouse this is 0.
	if(!m_useGrid){
		stimCtrXDeg = 0;
		stimCtrYDeg = 0;
	}else if(!g_pRFMapping->getUseBinocular()){
		g_pEyeTrackingDlg->EyePosSample();
		if(g_pRFMapping->usePrimaryL()){
			stimCtrXDeg = g_pEyeTrackingDlg->avglx;
			stimCtrYDeg = g_pEyeTrackingDlg->avgly;
		}else{
			stimCtrXDeg =g_pEyeTrackingDlg->avgrx;
			stimCtrYDeg =g_pEyeTrackingDlg->avgry;
		}
	}else{
		g_pEyeTrackingDlg->EyePosSample();
		if(g_pRFMapping->getStimEye() == CRFTrial::EYE_RIGHT){
			stimCtrXDeg =g_pEyeTrackingDlg->avgrx;
			stimCtrYDeg =g_pEyeTrackingDlg->avgry;
		}else{
			stimCtrXDeg = g_pEyeTrackingDlg->avglx;
			stimCtrYDeg = g_pEyeTrackingDlg->avgly;
			//stimCtrXDeg =m_crossX2Deg;
			//stimCtrYDeg =m_crossY2Deg; <-- original based off of cross Position
		}
	}
	g_pRFMappingDlg->UpdateEyePositions(
			g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
			g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);

	

	/* Add offsets, take starting point as stimCtrXDeg (not necessarily 0,0) and flip
	 * the Y coordinate. */
	perXDeg= perXDeg + stimCtrXDeg+ g_pSettings->getXOffset();
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, perXDeg+m_screenOffsetXDeg, 
		vsgPIXELUNIT, &xPix);
	perYDeg= perYDeg + stimCtrYDeg + g_pSettings->getYOffset();
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -(perYDeg+m_screenOffsetYDeg), 
		vsgPIXELUNIT, &yPix);

	vsgSetPen1(LEVEL_STIM);


	if (g_pRFMapping->useCircle()) {
		double radiusPix;
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getCircleRadius(), 
			vsgPIXELUNIT, &radiusPix);
		if(m_useNoise){
			VsgExt::drawCircle((int)xPix, (int)yPix, (int)radiusPix * 2, *pixelSpec, colorMap, bgController);
		}else{
			vsgDrawOval(xPix,yPix, radiusPix*2, radiusPix*2);
		}
	} 
	else {
		double widthPix, heightPix;
		float barAngle = g_pRFMapping->getOrientation();

		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getBarWidth(), 
			vsgPIXELUNIT, &widthPix);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getBarHeight(), 
			vsgPIXELUNIT, &heightPix);

/** TODO: Change Draw Bar to accept Noise controller**/
		vsgDrawBar(xPix, yPix, widthPix, heightPix, barAngle);
	}

	/* Reported location only reflects the offset. It ignores the screen offset 
	 * as per usual. It is interpreted offline as being relative to the cross 
	 * location so that loc isn't added either. */
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, ((float)perXDeg) / g_pSettings->getDegPerVolt());
	etu::AnalogOut(A_OUT_PER_TARG_VERT, ((float)perYDeg) / g_pSettings->getDegPerVolt());
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)((g_pRFMapping->getFixX())/ g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)((g_pRFMapping->getFixY()) / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_CTR_TARG_COLOR, g_pRFMapping->getCrossColor());
	etu::AnalogOut(A_OUT_PER_TARG_COLOR, g_pRFMapping->getStimColor());
	
	vsgPresent();
	m_stimulusChanged = false;
}



/** Set the current drawing page to a new, blank page checking noise */
void CRFMappingStimulus::NewDrawingPage(bool noise) 
{

	m_currentPage = 1 - m_currentPage;	
	vsgSetDrawPage(vsgVIDEOPAGE, m_currentPage, vsgBACKGROUND);
	float ctrXDeg = g_pRFMapping->getFixX();
	float ctrYDeg = g_pRFMapping->getFixY();//Screenoffset applied at DrawCross
	if (noise && g_pRFMapping->getSameBG()) {
		m_noiseBgController->drawReferenceBg();
		DrawCrossWithBG(ctrXDeg, ctrYDeg,m_fixEye);
	}
	else if (noise) {
		m_noiseBgController->drawBg();
		DrawCrossWithBG(ctrXDeg, ctrYDeg, m_fixEye);
	}else{


		/* Screen offsets only;  */
		//calling vsgExt:Draw requires offsets and translation to pixels
		ctrXDeg += g_pSettings->getScreenXOffset();
		ctrYDeg += g_pSettings->getScreenYOffset();
		double centerXPix, centerYPix;

		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, ctrXDeg, vsgPIXELUNIT, &centerXPix);
		/* Y pixels increase as Y degrees decrease (as Y goes down screen) */
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -1 * ctrYDeg, vsgPIXELUNIT, &centerYPix);
		VsgExt::drawCross(centerXPix, centerYPix, 
		(int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
	}

}
void CRFMappingStimulus::Clear(bool noise)
{
	m_currentPage = 1 - m_currentPage;
	vsgSetDrawPage(vsgVIDEOPAGE, m_currentPage, vsgBACKGROUND);
	
	if (noise && g_pTrialSaccades->getSameBG()) {
		m_noiseBgController->drawReferenceBg();
	}
	else if (noise) {
		m_noiseBgController->drawBg();
	}
}

void CRFMappingStimulus::DrawCrossWithBG(double centerXDeg, 
											 double centerYDeg, 
											 CRFTrial::EYE_CODE eye) 
{
	double centerXPix, centerYPix;
	VsgExt::NoiseBgController* controller = NULL;
	VsgExt::NoiseColorMap* map = NULL;
	VsgExt::IGrating* gratingDesc=NULL;

	if (eye == CRFTrial::EYE_RIGHT){
		gratingDesc = m_primarySpec;
	}else{
		gratingDesc = m_secondarySpec;

	}
	
	/* Screen offsets only; stim offsets already applied to function params */
	centerXDeg += g_pSettings->getScreenXOffset();
	centerYDeg += g_pSettings->getScreenYOffset();

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, centerXDeg, vsgPIXELUNIT, &centerXPix);
	/* Y pixels increase as Y degrees decrease (as Y goes down screen) */
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -1 * centerYDeg, vsgPIXELUNIT, &centerYPix);
	
	/*Respect BG if not occlude*/
	if (m_useNoise && !g_pRFMapping->getOccludeBG()) {
		controller = m_noiseBgController;
		map = m_noiseColorMap;
	}

	VsgExt::drawCross(centerXPix, centerYPix,(int) m_crossDiameterPix, 
		(int)m_crossThicknessPix, *gratingDesc, map, controller);
}

/**
 * @brief Set colors in member vsgLUTBUFFER for targets, background, and 
 * gratings.
 */
void CRFMappingStimulus::InitLutBuffer() 
{
	/* For use here and later with BG Controller (if needed) */
	m_bgColor.a    = ((double)g_pSettings->getRedBG()) / 255.0f;
	m_bgColor.b    = ((double)g_pSettings->getGreenBG()) / 255.0f;
	m_bgColor.c    = ((double)g_pSettings->getBlueBG()) / 255.0f;
	m_noiseColor.a = ((double)g_pRFMapping->getNoiseRed()) / 255.0f;
	m_noiseColor.b = ((double)g_pRFMapping->getNoiseGreen()) / 255.0f;
	m_noiseColor.c = ((double)g_pRFMapping->getNoiseBlue()) / 255.0f;

	
	/* Save copies of all periph colors */
	m_lutbuf[LEVEL_PRIMARY].a = ((double)g_pRFMapping->getPrimaryRed())/255.0f;
	m_lutbuf[LEVEL_PRIMARY].b = ((double)g_pRFMapping->getPrimaryGreen())/255.0f;
	m_lutbuf[LEVEL_PRIMARY].c = ((double)g_pRFMapping->getPrimaryBlue())/255.0f;

	m_lutbuf[LEVEL_SECONDARY].a = ((double)g_pRFMapping->getSecondaryRed())/255.0f;
	m_lutbuf[LEVEL_SECONDARY].b = ((double)g_pRFMapping->getSecondaryGreen())/255.0f;
	m_lutbuf[LEVEL_SECONDARY].c = ((double)g_pRFMapping->getSecondaryBlue())/255.0f;

	m_lutbuf[LEVEL_TERTIARY].a = ((double)g_pRFMapping->getTertiaryRed())/255.0f;
	m_lutbuf[LEVEL_TERTIARY].b = ((double)g_pRFMapping->getTertiaryGreen())/255.0f;
	m_lutbuf[LEVEL_TERTIARY].c = ((double)g_pRFMapping->getTertiaryBlue())/255.0f;

	m_lutbuf[LEVEL_PRIMARY_PLUS_NOISE].a = m_lutbuf[LEVEL_PRIMARY].a;
	m_lutbuf[LEVEL_PRIMARY_PLUS_NOISE].b = 0.0f;
	m_lutbuf[LEVEL_PRIMARY_PLUS_NOISE].c = m_noiseColor.c;

	m_lutbuf[LEVEL_SECONDARY_PLUS_NOISE].a = m_noiseColor.a;
	m_lutbuf[LEVEL_SECONDARY_PLUS_NOISE].b = 0.0f;
	m_lutbuf[LEVEL_SECONDARY_PLUS_NOISE].c = m_lutbuf[LEVEL_SECONDARY].c;

	m_lutbuf[LEVEL_PRIMARY_PLUS_SECONDARY].a = m_lutbuf[LEVEL_PRIMARY].a;
	m_lutbuf[LEVEL_PRIMARY_PLUS_SECONDARY].b = 0.0f;
	m_lutbuf[LEVEL_PRIMARY_PLUS_SECONDARY].c = m_lutbuf[LEVEL_SECONDARY].c;

}
/**
 * @brief Allocate the NoiseBgController and NoiseColorMap, and initialize
 * them.
 */
void CRFMappingStimulus::InitNoiseHandling()
{
	int size = g_pRFMapping->getNoiseSize();
	int percent = g_pRFMapping->getNoisePct();

	CleanUpNoiseHandling();

	m_noiseBgController = new VsgExt::NoiseBgController(size, m_bgColor, 
		m_noiseColor, LEVEL_BG_CONTR, percent, m_lutbuf);

	m_noiseColorMap = new VsgExt::NoiseColorMap;
	m_noiseColorMap->map(LEVEL_PRIMARY, LEVEL_PRIMARY_PLUS_NOISE);
	m_noiseColorMap->map(LEVEL_SECONDARY, LEVEL_SECONDARY_PLUS_NOISE);
	m_noiseColorMap->map(LEVEL_TERTIARY, LEVEL_TERTIARY);

	/* Used for drawing the same BG throughout a trial. */
	m_noiseBgController->initReferenceBg(vsgVIDEOPAGE, DRAW_PAGE_REF_BG);
}

/**
 * @brief Deletes noise handling objects.
 */
void CRFMappingStimulus::CleanUpNoiseHandling()
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
 * @brief Configure LEVEL_CTR_TARG and LEVEL_PER_TARG for this trial.
 * @param currentTrial Reference to current CRFTrial for access to
 * @param eye Reference to which eye is fixating on cross
 */
void CRFMappingStimulus::SetupBufferForTrial(CRFTrial& currentTrial, CRFTrial::EYE_CODE eye)
{
	VSGTRIVAL& centerColor = m_lutbuf[LEVEL_CROSS];
	VSGTRIVAL& periphColor = m_lutbuf[LEVEL_STIM];
	
	if (eye == CRFTrial::EYE_RIGHT) {
		centerColor = m_lutbuf[LEVEL_PRIMARY];
		g_pRFMapping->setCrossColor(VOLTS_COLOR_RED);
	}else{
		centerColor = m_lutbuf[LEVEL_SECONDARY];
		g_pRFMapping->setCrossColor(VOLTS_COLOR_BLUE);
	}

	if (currentTrial.getStimColor()==CRFTrial::CODE_C_RED) {
		periphColor = m_lutbuf[LEVEL_PRIMARY];
		g_pRFMapping->setStimColor(VOLTS_COLOR_RED);
	} 
	else if (currentTrial.getStimColor()==CRFTrial::CODE_C_BLUE) {
		periphColor = m_lutbuf[LEVEL_SECONDARY];
		g_pRFMapping->setStimColor(VOLTS_COLOR_BLUE);
	}
	else {
		periphColor = m_lutbuf[LEVEL_TERTIARY];
		g_pRFMapping->setStimColor(VOLTS_COLOR_PURPLE);
	}
}

void CRFMappingStimulus::InitGratings()
{
	const int cycles = (int)g_pTrialSaccades->getDotGratingCycles();

	CleanUpGratings();

	m_primarySpec = VsgExt::IGrating::identity(LEVEL_PRIMARY);
	m_secondarySpec = VsgExt::IGrating::identity(LEVEL_SECONDARY);
	m_tertiarySpec = VsgExt::IGrating::identity(LEVEL_TERTIARY);
}

/**
 * @brief Deletes grating objecgts.
 */
void CRFMappingStimulus::CleanUpGratings()
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

}

/** 
 * @brief Update the position of the stimulus on screen. 
 *
 * Because the mouse is moved on the stim dialog's screen but must represent 
 * movement of the shape on the stimulus display, we take scaling factors 
 * rather than absolute values.
 * 
 * @param xPct Percent of the X dimension of the stim dialog's screen 
 *     subtended by the new mouse pointer position.
 * @param yPct Percent of the Y dimension of the stim dialog's screen 
 *     subtended by the new mouse pointer position.
 */
void CRFMappingStimulus::updatePosition(float xPct, float yPct) 
{
	double perXDeg, perYDeg;
	double xPix, yPix;

	/* Use scale to get pixel coordinates w/0 at center of screen */
	xPix = xPct * m_screenWidthPix - (m_screenWidthPix / 2);
	yPix = yPct * m_screenHeightPix - (m_screenHeightPix / 2);

	/* Convert to degrees, flip the Y coordinate, then subtract cross 
	 * coordinates so that we reflect the displacement from the cross that the 
	 * subject actually sees. */
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, xPix, vsgDEGREEUNIT, &perXDeg);
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, yPix, vsgDEGREEUNIT, &perYDeg);

	perXDeg -= (g_pRFMapping->getFixX() +  m_screenOffsetXDeg);
	perYDeg = (-1 * perYDeg) - (g_pRFMapping->getFixY() +  m_screenOffsetYDeg);

	g_pRFMapping->setXPosition((float)perXDeg);
	g_pRFMapping->setYPosition((float)perYDeg);

	m_stimulusChanged = true;
}

/** Indicate resizing or angle change in stimulus. */
void CRFMappingStimulus::updateStimulus(int pixelsX, int pixelsY, bool bLeftButton) 
{
	m_stimulusChanged = true;

	if (bLeftButton) {
		//change width/height
		double degsDraggedX, degsDraggedY;
		g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, pixelsX, vsgDEGREEUNIT, &degsDraggedX);
		g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, pixelsY, vsgDEGREEUNIT, &degsDraggedY);
		
		if (g_pRFMapping->useBar()) {
			g_pRFMapping->setBarHeight(max(0, g_pRFMapping->getBarHeight() + (float)degsDraggedY));
			g_pRFMapping->setBarWidth(max(0, g_pRFMapping->getBarWidth() + (float)degsDraggedX));
		} 
		else {
			g_pRFMapping->setCircleRadius(max(0, g_pRFMapping->getCircleRadius() + (float)degsDraggedY));
		}
	} 
	else if (g_pRFMapping->useBar()) {
		g_pRFMapping->setOrientation(g_pRFMapping->getOrientation() + (float)pixelsY / 2);
		m_stimulusChanged = true;
	}
	else {
		/* Right button = rotated the circle, a no-op */
		m_stimulusChanged = false;
	}
}

/** Stop. */
void CRFMappingStimulus::StopExecution() 
{
	bEndThread = true;
}

/** Name. */
CString CRFMappingStimulus::StimulusType() 
{
	return "RF Mapping Stimulus";
}


#endif // APP_EYE_TRACKING