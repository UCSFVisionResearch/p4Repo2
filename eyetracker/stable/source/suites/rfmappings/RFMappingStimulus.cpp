#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "RFMappingStimulus.h"
#include "utils/Utils.h"
#include "utils/Grid.h"
#include "vsgext/vsgext.h"
#include <algorithm>

/*****************************************************************************
 * COMMUNICATIONS:
 *****************************************************************************/

#define A_OUT_CTR_TARG_HORIZ  (0) /* 1401 in channel 3 */
#define A_OUT_CTR_TARG_VERT   (1) /* 1401 in channel 4 */
#define A_OUT_PER_TARG_HORIZ  (2) /* 1401 in channel 7 */
#define A_OUT_PER_TARG_VERT   (3) /* 1401 in channel 8; used to be 
                                   * A_OUT_PULSE */
#define A_OUT_PER_TARG_PULSE  (4) /* PULSE_ON_VOLTS */

#define PULSE_ON_VOLTS   (5)
#define PULSE_OFF_VOLTS (-5)

#define LEVEL_CROSS (1)
#define LEVEL_STIM (2)

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
	m_crossDiameterPix(0),
	m_crossThicknessPix(g_pRFMapping->getCrossThickness()),
	m_pageNum(0),
	m_useGrid(g_pRFMapping->getUseGrid()),
	m_usePulse(g_pRFMapping->usePulse()),
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
	m_pStimLocationGenerator(NULL)
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

	m_crossColor.a = ((double)g_pSettings->getRed()) / 255.0f;
	m_crossColor.b = ((double)g_pSettings->getGreen()) / 255.0f;
	m_crossColor.c = ((double)g_pSettings->getBlue()) / 255.0f;

	m_bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	m_bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	m_bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	m_stimColor.a = ((double)g_pRFMapping->getStimRed()) / 255.0;
	m_stimColor.b = ((double)g_pRFMapping->getStimGreen()) / 255.0;
	m_stimColor.c = ((double)g_pRFMapping->getStimBlue()) / 255.0;

	memset(lutbuf, 0, 256 * sizeof(VSGTRIVAL));
	lutbuf[LEVEL_CROSS] = m_crossColor;
	lutbuf[LEVEL_STIM] = m_stimColor;
	lutbuf[vsgBACKGROUND] = m_bgColor;
		
	frameTime = vsgGetSystemAttribute(vsgFRAMETIME);
	m_frameRate = 1000000.0 / frameTime;
	g_dFramerate = m_frameRate;
	g_dHalfFrameInterval = DWORD(floor(frameTime / 2000.0f));
	g_dSleepThreshold = (double)g_dHalfFrameInterval / 2.0f;

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);
	vsgSetFixationColour(&m_crossColor);
	vsgSetBackgroundColour(&m_bgColor);
	vsgSetPageWidth(2048); /* In case we didn't call vsgSetVideoMode. */
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(m_screenWidthPix / 2, m_screenHeightPix / 2);
	vsgLUTBUFFERWrite(0, &lutbuf);
	vsgLUTBUFFERtoPalette(0);

	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	/* Set up cross coordinates for duration of stimulus. */
	xxDeg = m_crossXDeg;
	yyDeg = m_crossYDeg;

	/* Analog outs 0 and 1 match with the calibration stimulus. */
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)(xxDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)(yyDeg / g_pSettings->getDegPerVolt()));

	xxDeg += m_screenOffsetXDeg;
	yyDeg += m_screenOffsetYDeg;

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, xxDeg, vsgPIXELUNIT, &m_crossXPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -1 * yyDeg, vsgPIXELUNIT, &m_crossYPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getCrossDiameter(), 
		vsgPIXELUNIT, &m_crossDiameterPix);

	vsgSetDrawPage(vsgVIDEOPAGE, m_pageNum, vsgBACKGROUND);
	VsgExt::drawCross((int)m_crossXPix, (int)m_crossYPix, 
		(int)m_crossDiameterPix, (int)m_crossThicknessPix, 2);

	if (g_pSettings->getXOffset() || g_pSettings->getXOffset()) {
		AfxMessageBox("Warning: Nonzero stim offsets are ignored by RF Mapping.");
	}

	if (m_useGrid) {
		m_sequenceTimer.StartTimer();
		m_pStimLocationGenerator = new StimLocationGenerator;
		etu::SPoint newPt = m_pStimLocationGenerator->next();
		g_pRFMapping->setXPosition(newPt.xDeg);
		g_pRFMapping->setYPosition(newPt.yDeg);
		m_stimulusChanged = true;
	}
	
	if (m_usePulse) {
		m_pulseTimer.StartTimer();
	}

	/* Call DrawStimulus directly to bypass pulse checking. */
	//DrawStimulus();

	
}

/** Trial running signal turned off and tear down. */
CRFMappingStimulus::~CRFMappingStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	vsgObjDestroy(g_stim);
	vsgObjDestroy(g_fixation);
	vsgObjDestroy(g_grating);
	vsgObjDestroy(g_stim2);
	delete m_pStimLocationGenerator;
}

/** Coordinate stimulus display w/pulse. */
bool CRFMappingStimulus::DisplayStimulus() 
{
	if (!m_messageSent) {
		g_pRFMapping->sendSettings(g_pSettings->theSerialComm);
		m_messageSent = true;
	}

	/* Move to the new location */
	if (m_useGrid && 
		(m_sequenceTimer.CheckTimer() > m_displayTimeMs)) 
	{
		if (m_useBlanks) {
			NewDrawingPage();
			vsgPresent();
			etu::AnalogOut(A_OUT_PER_TARG_HORIZ, -99.0);
			etu::AnalogOut(A_OUT_PER_TARG_VERT, -99.0);
			m_sequenceTimer.CheckTimerWithReset();
			while (m_sequenceTimer.CheckTimer() < m_blankTimeMs) {
				Sleep(5);
			}
		}
		etu::SPoint newPt = m_pStimLocationGenerator->next();
		g_pRFMapping->setXPosition(newPt.xDeg);
		g_pRFMapping->setYPosition(newPt.yDeg);
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
				etu::AnalogOut(A_OUT_PER_TARG_PULSE, PULSE_ON_VOLTS);
			}
			else {
				NewDrawingPage();
				vsgPresent();
				etu::AnalogOut(A_OUT_PER_TARG_PULSE, PULSE_OFF_VOLTS);
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
		DrawStimulus();
	}

	Sleep(1);
	return true;
}

/** Draw applicable stimulus. Send no output. */
void CRFMappingStimulus::DrawStimulus() 
{
	double xPix, yPix;
	const double xDeg = g_pRFMapping->getXPosition();
	const double yDeg = g_pRFMapping->getYPosition();

	NewDrawingPage();

	/* Add offsets, take starting point as cross (not necessarily 0,0) and flip
	 * the Y coordinate. */
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, xDeg +  m_screenOffsetXDeg + m_crossXDeg, 
		vsgPIXELUNIT, &xPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -(yDeg +  m_screenOffsetYDeg + m_crossYDeg), 
		vsgPIXELUNIT, &yPix);

	vsgSetPen1(LEVEL_STIM);

	if (g_pRFMapping->useCircle()) {
		double radiusPix;

		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getCircleRadius(), 
			vsgPIXELUNIT, &radiusPix);

		vsgDrawOval(xPix, yPix, radiusPix * 2, radiusPix * 2);
	} 
	else {
		double widthPix, heightPix;
		float barAngle = g_pRFMapping->getOrientation();

		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getBarWidth(), 
			vsgPIXELUNIT, &widthPix);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getBarHeight(), 
			vsgPIXELUNIT, &heightPix);

		vsgDrawBar(xPix, yPix, widthPix, heightPix, barAngle);
	}

	/* Reported location only reflects the offset. It ignores the screen offset 
	 * as per usual. It is interpreted offline as being relative to the cross 
	 * location so that loc isn't added either. */
	etu::AnalogOut(A_OUT_PER_TARG_HORIZ, ((float)xDeg) / g_pSettings->getDegPerVolt());
	etu::AnalogOut(A_OUT_PER_TARG_VERT, ((float)yDeg) / g_pSettings->getDegPerVolt());
	vsgPresent();
	m_stimulusChanged = false;
}

/** Set the current drawing page to a new, blank page. */
void CRFMappingStimulus::NewDrawingPage()
{
	vsgSetDrawPage(vsgVIDEOPAGE, m_pageNum, vsgBACKGROUND);
	VsgExt::drawCross((int)m_crossXPix, (int)m_crossYPix, 
		(int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
	m_pageNum = 1 - m_pageNum;
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
	double xDeg, yDeg;
	double xPix, yPix;

	/* Use scale to get pixel coordinates w/0 at center of screen */
	xPix = xPct * m_screenWidthPix - (m_screenWidthPix / 2);
	yPix = yPct * m_screenHeightPix - (m_screenHeightPix / 2);

	/* Convert to degrees, flip the Y coordinate, then subtract cross 
	 * coordinates so that we reflect the displacement from the cross that the 
	 * subject actually sees. */
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, xPix, vsgDEGREEUNIT, &xDeg);
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, yPix, vsgDEGREEUNIT, &yDeg);

	xDeg -= (m_crossXDeg +  m_screenOffsetXDeg);
	yDeg = (-1 * yDeg) - (m_crossYDeg +  m_screenOffsetYDeg);

	g_pRFMapping->setXPosition((float)xDeg);
	g_pRFMapping->setYPosition((float)yDeg);

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

/*************************************************************************/

CRFMappingStimulus::StimLocationGenerator::StimLocationGenerator()
: m_random(false)
{
	m_pGrid = etu::createCartesianGrid(g_pRFMapping->getStepsX(), 
		g_pRFMapping->getStepsY(), g_pRFMapping->getStepSizeDeg(), false);
	m_iter = m_pGrid->begin();
	m_end = m_pGrid->end();
	if (m_iter >= m_end) {
		AfxMessageBox("Failed assertion: No grid locations generated.");
	}
	else {
		std::random_shuffle(m_iter, m_end);
	}
	m_iter = m_pGrid->begin();
	m_end = m_pGrid->end();
}

etu::SPoint CRFMappingStimulus::StimLocationGenerator::next()
{
	if (m_iter >= m_end) {
		m_iter = m_pGrid->begin();
		std::random_shuffle(m_iter, m_end);

		m_iter = m_pGrid->begin();
		m_end = m_pGrid->end();
	}
	return *m_iter++;
}

#endif // APP_EYE_TRACKING
