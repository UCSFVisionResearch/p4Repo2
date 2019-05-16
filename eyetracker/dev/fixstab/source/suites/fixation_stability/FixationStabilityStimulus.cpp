/**
 * @file FixationStabilityStimulus.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "comm/SerialComm.h"
#include "comm/SMISerialComm.h"
#include "FixationStabilityStimulus.h"
#include "utils/Utils.h"

#include <fstream>

/*****************************************************************************
 * Defines
 *****************************************************************************/

/*** Communications **************************************/
#define A_OUT_TARG_HORIZ  (0) /* 1401 in channel 3 */
#define A_OUT_TARG_VERT   (1) /* 1401 in channel 4 */
#define A_OUT_TARG_COLOR  (4) /* VOLTS_COLOR_xxx */

#define D_OUT_FEED        (vsgDIG7) // 0 = Don't feed.  1 = Feed

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

#define VOLTS_COLOR_RIGHT     (5)
#define VOLTS_COLOR_LEFT     (-5)

/*** LUT Levels ******************************************/
#define LEVEL_LEFT_EYE  (0)
#define LEVEL_RIGHT_EYE (1)
#define LEVEL_BG_CONTR  (2) // Reserve 2-101 and 102-201
#define LEVEL_TIME_OUT  (202)
#define LEVEL_LEFT_PLUS_NOISE   (206)
#define LEVEL_RIGHT_PLUS_NOISE  (207)

/*** Page Numbers*****************************************/
#define DRAW_PAGE_REF_BG (2)

/*****************************************************************************
 * Functions
 *****************************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/** Sets up arrays for the various calibration types. */
CFixationStabilityStimulus::CFixationStabilityStimulus() 
: m_screenWidthPix(0),
  m_screenHeightPix(0),
  m_currentPage(0),
  m_useNoise(true),
  m_lastTrialNoise(false),
  m_leftClosed(false),
  m_rightClosed(false),
  m_bgColor(m_lutbuf[LEVEL_BG_CONTR]), 
  m_noiseColor(m_lutbuf[LEVEL_BG_CONTR+100]),
  m_noiseBgController(NULL), 
  m_noiseColorMap(NULL), 
  m_leftEyeSpec(NULL), 
  m_rightEyeSpec(NULL)
{
	Initialize();

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	m_screenWidthPix = vsgGetScreenWidthPixels();
	m_screenHeightPix = vsgGetScreenHeightPixels();
	
	m_currentPage = 0;
	m_useNoise = true;
	m_flashTimer.StartTimer();

	srand((unsigned int)time(NULL));

	vsgIOWriteDigitalOut(0xFFFF, (vsgDIG0)); // Set to 1 to match calibration
	vsgIOWriteDigitalOut(0x0000, (vsgDIG1)); // Behav. contingency reset = off
	vsgIOWriteDigitalOut(0x0000, (vsgDIG2)); // Set to 0 to match calibration

	/* Set the drawing mode, and set the origin to the center of the screen */
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(m_screenWidthPix / 2, m_screenHeightPix / 2);

	memset(m_lutbuf, 0, 256 * sizeof(VSGTRIVAL));
	vsgSetDrawPage(vsgVIDEOPAGE, m_currentPage, vsgBACKGROUND);
	//vsgSetBackgroundColour(&m_lutbuf[vsgBACKGROUND]);
	//vsgSetFixationColour(&m_lutbuf[vsgFIXATION]);
	
	g_pFixationStabilityModel->sendSettings(g_pSettings->theSerialComm);
	
	/* Populate colors generally; per-trial colors are populated in 
	 * DisplayStimulus. */
	InitLutBuffer();
	InitNoiseHandling();
	InitGratings();
	InitShutters();
	vsgLUTBUFFERWrite(0, &m_lutbuf);
	vsgLUTBUFFERtoPalette(0);

	vsgSetBackgroundColour(&m_bgColor);
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);

	/* Used for drawing the same BG throughout all trial. */
	m_noiseBgController->initReferenceBg(vsgVIDEOPAGE, DRAW_PAGE_REF_BG);
}

/** The singleLock object must be deleted here to prevent memory leaks. */
CFixationStabilityStimulus::~CFixationStabilityStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, D_OUT_FEED);

	CleanUpNoiseHandling();
	CleanUpGratings();

	etu::AnalogOut(A_OUT_TARG_COLOR, (float)0.0, true);
	
	/* Send stop mark */
	char buffer[80];
	sprintf(buffer, "Fixation Stability;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}

void CFixationStabilityStimulus::InitLutBuffer()
{
	m_lutbuf[LEVEL_LEFT_EYE] = g_pFixationStabilityModel->leftEyeParameters().color().toVsg();
	m_lutbuf[LEVEL_RIGHT_EYE] = g_pFixationStabilityModel->rightEyeParameters().color().toVsg();

	m_bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	m_bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	m_bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	m_noiseColor = g_pFixationStabilityModel->noiseParameters().color().toVsg();

	m_lutbuf[LEVEL_TIME_OUT].a = 0.0;
	m_lutbuf[LEVEL_TIME_OUT].b = 0.0;
	m_lutbuf[LEVEL_TIME_OUT].c = 0.0;

	m_lutbuf[LEVEL_LEFT_PLUS_NOISE]    = m_noiseColor;
	m_lutbuf[LEVEL_RIGHT_PLUS_NOISE]   = m_noiseColor;
	m_lutbuf[LEVEL_LEFT_PLUS_NOISE].c  = m_lutbuf[LEVEL_LEFT_EYE].c;
	m_lutbuf[LEVEL_RIGHT_PLUS_NOISE].a = m_lutbuf[LEVEL_RIGHT_EYE].a;
}

void CFixationStabilityStimulus::InitNoiseHandling()
{
	int size = g_pFixationStabilityModel->noiseParameters().noiseSize();
	int percent = g_pFixationStabilityModel->noiseParameters().noisePct();

	CleanUpNoiseHandling();

	m_noiseBgController = new VsgExt::NoiseBgController(size, m_bgColor, 
		m_noiseColor, LEVEL_BG_CONTR, percent, m_lutbuf);

	m_noiseColorMap = new VsgExt::NoiseColorMap;
	m_noiseColorMap->map(LEVEL_LEFT_EYE, LEVEL_LEFT_PLUS_NOISE);
	m_noiseColorMap->map(LEVEL_RIGHT_EYE, LEVEL_RIGHT_PLUS_NOISE);
}

void CFixationStabilityStimulus::InitGratings()
{
	CleanUpGratings();

	m_leftEyeSpec = VsgExt::IGrating::identity(LEVEL_LEFT_EYE);
	m_rightEyeSpec = VsgExt::IGrating::identity(LEVEL_RIGHT_EYE);
}

void CFixationStabilityStimulus::InitShutters()
{
	int leftEyeOccluded = g_pFixationStabilityModel->leftEyeParameters().occluded().size();
	int rightEyeOccluded = g_pFixationStabilityModel->rightEyeParameters().occluded().size();

	/* Prepare shutters */
	if ((leftEyeOccluded == 0) && (rightEyeOccluded == 0)) {
		/* Enable controls, leave shutters as user set. */
		g_pEyeTrackingDlg->m_ShutterL.EnableWindow();
		g_pEyeTrackingDlg->m_ShutterR.EnableWindow();
	} 
	else {
		/* Disable controls */
		g_pEyeTrackingDlg->m_ShutterL.EnableWindow(false);
		g_pEyeTrackingDlg->m_ShutterR.EnableWindow(false);

		/* Open both to start. */
		g_pEyeTrackingDlg->OpenRightShutter(true);
		g_pEyeTrackingDlg->OpenLeftShutter(true);
		ShutterDelay();
	}
}

void CFixationStabilityStimulus::CleanUpNoiseHandling()
{
	if (m_noiseBgController) delete m_noiseBgController;
	if (m_noiseColorMap) delete m_noiseColorMap;
}

void CFixationStabilityStimulus::CleanUpGratings()
{
	if (m_leftEyeSpec) delete m_leftEyeSpec;
	if (m_rightEyeSpec) delete m_rightEyeSpec;
}

/**  */
bool CFixationStabilityStimulus::DisplayStimulus() 
{
	//const int behaveContingencyPulse = 100;
	double    centerXDeg, centerYDeg;
	char      buffer[1024];
	size_t    stringSize;
	double    screenXOffset, screenYOffset;
	double    stimXOffset, stimYOffset;

	const fstab::CFixationStabilityTrial* currentTrial;

	m_useNoise = g_pFixationStabilityModel->noiseParameters().useNoise();
	if (g_pFixationStabilityModel->noiseParameters().interleaveNoise()) {
		m_lastTrialNoise = !m_lastTrialNoise;
		m_useNoise = m_lastTrialNoise;
	}

	/* Get next trial within mutex handle; we're in a different thread from
	 * the CTrialSaccades model. */
	{
		SmartLock fixationStabilityModelLock(*g_pFixationStabilityModel);
		
		currentTrial = g_pFixationStabilityModel->getNextTrial();

		if (currentTrial == NULL) {
			AfxMessageBox("No trials.");
			return false;
		}
	}

	/* Get center location, and add stim offset to the stored value, as this 
	 * location is reported via AnalogOut.*/
	centerXDeg = currentTrial->xDeg() + g_pSettings->getXOffset();
	centerYDeg = currentTrial->yDeg() + g_pSettings->getYOffset();
	
	/* Analog output for trial type */
	etu::AnalogOut(A_OUT_TARG_COLOR, ((currentTrial->eye() == fstab::LEFT_EYE) ? 
		(float)VOLTS_COLOR_LEFT : (float)VOLTS_COLOR_LEFT));

	/* Behavioral contingency counter should (always) be reset for next trial 
	 * even if it's a repeat of a past object. */
	//vsgIOWriteDigitalOut(0xFFFF, D_OUT_BEHAV_CONT_RESET);
	//WaitUntil(behaveContingencyPulse);
	//vsgIOWriteDigitalOut(0x0000, D_OUT_BEHAV_CONT_RESET);

	/* Prep text mark before stimulus starts to prevent time lags later. */
	{
		VSGTRIVAL& current = ((currentTrial->eye() == fstab::LEFT_EYE) ? 
			m_lutbuf[LEVEL_LEFT_EYE] : m_lutbuf[LEVEL_RIGHT_EYE]);
		sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0/\0", current.a, current.b, 
			current.c);
		stringSize = strlen(buffer);
	}

	NewDrawingPage(false);
	vsgPresent();
	g_pEyeTrackingDlg->OpenRightShutter(!currentTrial->occludeRight());
	g_pEyeTrackingDlg->OpenLeftShutter(!currentTrial->occludeLeft());

	/* Time for shutters to respond */
	ShutterDelay();

	NewDrawingPage(m_useNoise);
	DrawCross(currentTrial->eye(), centerXDeg, centerYDeg);

	/* Present stimulus and set analog out as simultaneously as possible. */
	vsgPresent();
	etu::AnalogOut(A_OUT_TARG_HORIZ, (float)(centerXDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_TARG_VERT, (float)(centerYDeg / g_pSettings->getDegPerVolt()));
	g_pSettings->theSerialComm.transmit(buffer, stringSize);

	m_flashTimer.StartTimer();
	while ((m_flashTimer.CheckTimer() < g_pFixationStabilityModel->fixationTime()) 
		&& !bEndThread) 
	{
		WaitUntil(5);
		if (vsgIOReadDigitalIn() & D_IN_NEXT_STIM) {
			/* Loop here until fixation criteria are met */
			m_flashTimer.CheckTimerWithReset();
		}
	}

	return !bEndThread;
}

/**
 * @brief Switches to the other drawing page and clears it for use.
 * @param noise True to draw a noise background on the page; false to leave 
 * the page blank.
 */
void CFixationStabilityStimulus::NewDrawingPage(bool noise) 
{
	m_currentPage = 1 - m_currentPage;
	vsgSetDrawPage(vsgVIDEOPAGE, m_currentPage, vsgBACKGROUND);
	
	if (noise && g_pFixationStabilityModel->noiseParameters().sameBg()) {
		m_noiseBgController->drawReferenceBg();
	}
	else if (noise) {
		m_noiseBgController->drawBg();
	}
}

void CFixationStabilityStimulus::DrawCross(fstab::EYE eye, double centerXDeg, 
										   double centerYDeg)
{
	double centerXPix, centerYPix, centerSizePix;
	VsgExt::IGrating* gratingDesc = (eye == fstab::LEFT_EYE ? m_leftEyeSpec : m_rightEyeSpec);
	VsgExt::NoiseBgController* controller = NULL;
	VsgExt::NoiseColorMap* map = NULL;
	
	/* Screen offsets only; stim offsets already applied to function params */
	centerXDeg += g_pSettings->getScreenXOffset();
	centerYDeg += g_pSettings->getScreenYOffset();

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, centerXDeg, vsgPIXELUNIT, &centerXPix);
	/* Y pixels increase as Y degrees decrease (as Y goes down screen) */
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -1 * centerYDeg, vsgPIXELUNIT, &centerYPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, 
		g_pFixationStabilityModel->crosshairDiameter(), vsgPIXELUNIT, &centerSizePix);
	
	/* Draw center point (only) */
	if (m_useNoise && !g_pFixationStabilityModel->noiseParameters().occludeBg()) {
		controller = m_noiseBgController;
		map = m_noiseColorMap;
	}

	VsgExt::drawCross(centerXPix, centerYPix, centerSizePix, 
		g_pFixationStabilityModel->crosshairThickness(), *gratingDesc, map, 
		controller);
}

void CFixationStabilityStimulus::SetShutters(bool occludeLeft, bool occludeRight)
{
	if (occludeLeft || occludeRight || m_leftClosed || m_rightClosed) {
		NewDrawingPage(false);
		vsgPresent();
		g_pEyeTrackingDlg->OpenRightShutter(!occludeRight);
		g_pEyeTrackingDlg->OpenLeftShutter(!occludeLeft);
		ShutterDelay();
	}
	m_leftClosed = occludeLeft;
	m_rightClosed = occludeRight;
}

void CFixationStabilityStimulus::ShutterDelay()
{
	m_flashTimer.StartTimer();
	while ((m_flashTimer.CheckTimer() < 250) && !bEndThread) {
		WaitUntil(5);
	}
}

/**  */
CString CFixationStabilityStimulus::StimulusType() 
{
	return "Fixation Stability Stimulus";
}
