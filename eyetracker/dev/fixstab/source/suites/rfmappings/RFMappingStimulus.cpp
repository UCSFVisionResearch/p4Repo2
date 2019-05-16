#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "RFMappingStimulus.h"
#include "utils/Utils.h"
#include "vsgext/vsgext.h"

/*****************************************************************************
COMMUNICATIONS:
	Analog Out 0:	Horizontal target location (matches calibration stimulus)
	Analog Out 1:	Vertical target location (matches calibration stimulus)
	Analog Out 3:	Target pulse state. +5=on -5=off

*****************************************************************************/
#define A_OUT_HORIZONTAL (0)
#define A_OUT_VERTICAL   (2)
#define A_OUT_PULSE      (3)
#define PULSE_ON_VOLTS   (5)
#define PULSE_OFF_VOLTS (-5)

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/**
 @brief Initializes the newly instantiated object. 
 */
CRFMappingStimulus::CRFMappingStimulus() 
:	m_screenWidthPix(vsgGetScreenWidthPixels()), 
	m_screenHeightPix(vsgGetScreenHeightPixels()),
	m_crossXPix(0),
	m_crossYPix(0),
	m_crossDiameterPix(0),
	m_crossThicknessPix(g_pRFMapping->getCrossThickness()),
	m_pageNum(0),
	m_usePulse(false),
	m_pulsed(false),
	m_pulseStateHigh(false),
	m_stimulusChanged(false)
{
	double xxDeg, yyDeg;
	double frameTime;

	bEndThread = false;

	if (!g_bIsInitialized) {
		if (vsgInit("") < 0) return;
		g_bIsInitialized = true;
		/* Set page width to 2048; display error if vsg.cfg isn't sync'ed on
		 * green. */
		vsgSetVideoMode(vsgPANSCROLLMODE);
	}
		
	blackColor.a = 0;
	blackColor.b = 0;
	blackColor.c = 0;
	
	dotColor.a = ((double)g_pSettings->getRed()) / 255.0f;
	dotColor.b = ((double)g_pSettings->getGreen()) / 255.0f;
	dotColor.c = ((double)g_pSettings->getBlue()) / 255.0f;

	m_bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	m_bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	m_bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	/* The object system is an alternative to declaring a LUT buffer for the 
	 * few colors used. */
	g_stim = vsgObjCreate();
	vsgObjSetContrast(100);
	vsgObjSetPixelLevels(2,1);
	vsgObjSetColourVector(&blackColor, &dotColor, vsgBIPOLAR);
		
	frameTime = vsgGetSystemAttribute(vsgFRAMETIME);
	m_frameRate = 1000000.0 / frameTime;
	g_dFramerate = m_frameRate;
	g_dHalfFrameInterval = DWORD(floor(frameTime / 2000.0f));
	g_dSleepThreshold = (double)g_dHalfFrameInterval / 2.0f;

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);
	vsgSetFixationColour(&dotColor);
	vsgSetBackgroundColour(&m_bgColor);
	vsgSetPageWidth(2048); /* In case we didn't call vsgSetVideoMode. */
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(m_screenWidthPix / 2, m_screenHeightPix / 2);

	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	/* Set up cross coordinates for duration of stimulus. */
	xxDeg = g_pRFMapping->getCrossX() + g_pSettings->getXOffset();
	yyDeg = g_pRFMapping->getCrossY() + g_pSettings->getYOffset();

	/* Analog outs 0 and 1 match with the calibration stimulus. */
	etu::AnalogOut(0, (float)(xxDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(1, (float)(yyDeg / g_pSettings->getDegPerVolt()));

	xxDeg += g_pSettings->getScreenXOffset();
	yyDeg += g_pSettings->getScreenYOffset();

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, xxDeg, vsgPIXELUNIT, &m_crossXPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -1 * yyDeg, vsgPIXELUNIT, &m_crossYPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getCrossDiameter(), 
		vsgPIXELUNIT, &m_crossDiameterPix);

	vsgSetDrawPage(vsgVIDEOPAGE, m_pageNum, vsgBACKGROUND);
	VsgExt::drawCross((int)m_crossXPix, (int)m_crossYPix, 
		(int)m_crossDiameterPix, (int)m_crossThicknessPix, 2);

	/* Call DrawStimulus directly to bypass pulse checking. */
	DrawStimulus();

	if (m_usePulse = g_pRFMapping->usePulse()) {
		m_pulseTimer.StartTimer();
	}
}

/** Trial running signal turned off and tear down. */
CRFMappingStimulus::~CRFMappingStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	vsgObjDestroy(g_stim);
	vsgObjDestroy(g_fixation);
	vsgObjDestroy(g_grating);
	vsgObjDestroy(g_stim2);
}

/** Coordinate stimulus display w/pulse. */
bool CRFMappingStimulus::DisplayStimulus() 
{
	/* If timer pulse is in use, we redraw stimulus
	 *     1. When timer pulse goes high (turns on); and
	 *     2. When user changes the stimulus while the timer pulse is high.
	 * We draw a blank page when timer pulse goes low, and draw nothing again
	 * until timer pulse goes high.
	 * 
	 * If timer pulse is not in use, we redraw the stimulus when and only when
	 * the user changes it. */
	if (m_usePulse) {
		if (m_pulsed && m_pulseStateHigh) {
			DrawStimulus();
			etu::AnalogOut(A_OUT_PULSE, PULSE_ON_VOLTS);
		}
		else if (m_pulsed && !m_pulseStateHigh) {
			NewDrawingPage();
			vsgPresent();
			etu::AnalogOut(A_OUT_PULSE, PULSE_OFF_VOLTS);
		}
		else if (m_pulseStateHigh && m_stimulusChanged) {
			/* m_pulsed is false (implicitly) */
			DrawStimulus();
		}
	}
	else if (m_stimulusChanged) {
		DrawStimulus();
	}
	
	/* If timer pulse is in use, check timer.  If a rising or falling edge
	 * occurred, set m_pulsed flag true and indicate direction of edge by
	 * negating m_pulseStateHigh. Do not force a redraw by setting 
	 * m_stimulusChanged to true. */
	if (m_usePulse && m_pulseTimer.CheckTimer() > 150) {
		m_pulsed = true;
		m_pulseStateHigh = !m_pulseStateHigh;
		m_pulseTimer.CheckTimerWithReset();
	} 

	Sleep(1);
	return true;
}

/** Draw applicable stimulus. */
void CRFMappingStimulus::DrawStimulus() 
{
	double xPix, yPix;

	NewDrawingPage();

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getXPosition(),
		vsgPIXELUNIT, &xPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getYPosition(),
		vsgPIXELUNIT, &yPix);

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

	m_stimulusChanged = false;
	vsgPresent();
}

/** Set the current drawing page to a new, blank page. */
void CRFMappingStimulus::NewDrawingPage()
{
	vsgSetDrawPage(vsgVIDEOPAGE, m_pageNum, vsgBACKGROUND);
	VsgExt::drawCross((int)m_crossXPix, (int)m_crossYPix, 
		(int)m_crossDiameterPix, (int)m_crossThicknessPix, vsgFIXATION);
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
 *     traversed by the mouse pointer.
 * @param yPct Percent of the Y dimension of the stim dialog's screen 
 *     traversed by the mouse pointer.
 */
void CRFMappingStimulus::updatePosition(float xPct, float yPct) 
{
	double xDeg, yDeg;
	double xPix, yPix;

	/* Use scale to get pixel coordinates w/0 at center of screen */
	xPix = xPct * m_screenWidthPix - (m_screenWidthPix / 2);
	yPix = yPct * m_screenHeightPix - (m_screenHeightPix / 2);

	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, xPix, vsgDEGREEUNIT, &xDeg);
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, yPix, vsgDEGREEUNIT, &yDeg);
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

#endif // APP_EYE_TRACKING
