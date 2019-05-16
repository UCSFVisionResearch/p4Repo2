/**
 * @file GratingsStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "GratingsStimulus.h"
#include "utils/Math.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	VSG Out 0:	Trial Running pulse. 1 => trial is running. 
				0 => trial is not running
				only "run trial" during reward period
	VSG Out 1:	behavioral contingency (vsg pin 2; 1401 pin 18; 1401 dig in 6)
				Trial Reset pulse. This pulse signals the 1401
				to reset the behavioral contingency count because
				the time out period has expired and thus the stimulus
				has been moved.
	VSG Out 7:  remote reward / manual "feed now" signal
				0: don't feed
				1: feed

	VSG IN 0:	Next Stim Pulse. 1 => Behavioral contingency
				has been met, move on to the next phase.
				0 => behvioral continengency criteria has not 
				yet been met.
				The pulse only lasts 500ms, so we have to catch it before it's over
	VSG IN 1:	Select vertical during auto RF mapping. 1=top, 0=bottom
	VSG IN 2:	Select horizontal during auto RF mapping. 1=right, 0=left

	Analog Out 0:	Horizontal target location
	Analog Out 1:	Vertical target location
	Analog Out 2:	Horizontal peripheral target location (for RF mapping)
	Analog Out 3:	Vertical peripheral target location (for RF mapping)
	Analog Out 4:	Phase (deg)
	Analog Out 5:	Orientation (deg)

	Notes:
	1/27/2009	MKF scaled all analog outputs using g_pSettings->getDegPerVolt()
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor     
	 -  Instantiates the newly created object
************************************************************/
CGratingsStimulus::CGratingsStimulus() 
{
	Initialize();
	m_screenWidthPix = vsgGetScreenWidthPixels();
	m_screenHeightPix = vsgGetScreenHeightPixels();
	vsgMoveScreen(0, 0);

	vsgSetCommand(vsgPALETTECLEAR);

	srand((unsigned int)time(NULL));
	
	/* Behavioral contingency */
	vsgIOWriteDigitalOut(0x0000, vsgDIG0); // Trial is (not) running
	vsgIOWriteDigitalOut(0x0000, vsgDIG1); // No reset mode
	vsgIOWriteDigitalOut(0x0000, vsgDIG2); // F noise
	vsgIOWriteDigitalOut(0x0000, vsgDIG7); // Prevent remote reward

	/* Not using background target location */
	etu::AnalogOut(2, -100.0 / g_pSettings->getDegPerVolt()); // -10.0
	etu::AnalogOut(3, -100.0 / g_pSettings->getDegPerVolt());

	/* Set the drawing mode, and set the origin to the center of the screen */
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(m_screenWidthPix / 2, m_screenHeightPix / 2);
	vsgSetViewDistMM(g_pSettings->getViewDistance());

	/* Set the background color and init palette */
	m_bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	m_bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	m_bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	for (int ix = 0; ix < 256; ix++) {
		m_lutBuffer[ix] = m_bgColor;
	}

	vsgLUTBUFFERWrite(1, &m_lutBuffer);
	vsgLUTBUFFERtoPalette(1);

	vsgSetBackgroundColour(&m_lutBuffer[vsgBACKGROUND]);
	vsgSetFixationColour(&m_lutBuffer[vsgFIXATION]);

	vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgBACKGROUND);
	m_pageIx = 1;

	m_leftFixPoint = dbl_pair(g_pGratings->getXLeft(), g_pGratings->getYLeft());
	m_rightFixPoint = dbl_pair(g_pGratings->getXRight(), g_pGratings->getYRight());

	m_bReset = false;
	m_totalPresentations = 0;
	m_firstInit = true;
	m_interleavedCounter = 0;
	m_interleavedState = 0;
	m_savedShutterState = 0;

	m_gCounter = 0;
	m_gridXDeg=0; 
	m_gridYDeg=0; 
	m_gridXPix=0; 
	m_gridYPix=0;

	if (g_pGratings->getType() == CGratings::STYLE_RFMAP) {
		g_pGratings->GenerateRFMappingTrials();
	} 
	else {
		/* New trial class paradigm instead of old arrays */
		g_pGratings->GenerateTrials();
	}

	m_trialIndex = 0;

	m_rfCurrentWidth = 0;
	m_rfCurrentX = 0;
	m_rfCurrentY = 0;

	g_pGratings->sendSettings(g_pSettings->theSerialComm);
}

/*****************************************************************
	 Destructor
	 -  Cleans up when the object is deleted. The trial running
	 signal is turned off.
*****************************************************************/
CGratingsStimulus::~CGratingsStimulus() 
{
	vsgSetCommand(vsgOVERLAYDISABLE);
	vsgIOWriteDigitalOut(0x0000, vsgDIG0); // Trial not running
	vsgIOWriteDigitalOut(0x0000, vsgDIG7); // Prevent remote reward

	/* Restore the background color & palette */
	m_bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	m_bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	m_bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	for (int ix = 0; ix < 256; ix++) {
		m_lutBuffer[ix] = m_bgColor;
	}

	vsgSetBackgroundColour(&m_lutBuffer[vsgBACKGROUND]);
	vsgSetFixationColour(&m_lutBuffer[vsgFIXATION]);
	vsgLUTBUFFERWrite(1, &m_lutBuffer);
	vsgLUTBUFFERtoPalette(1);
	vsgSetBackgroundColour(&m_bgColor);
	vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgBACKGROUND);
	vsgPresent();

	/* Make sure shutter buttons are enabled */
	g_pEyeTrackingDlg->m_ShutterL.EnableWindow();
	g_pEyeTrackingDlg->m_ShutterR.EnableWindow();

	for (int ix = 0; ix < g_pGratings->numTrials; ix++) {
		delete g_pGratings->trials[ix];
		g_pGratings->trials[ix] = NULL;
	}
	delete [] g_pGratings->trials;
	g_pGratings->trials = NULL;

	/* Send stop mark */
	char buffer[80];
	sprintf(buffer, "Gratings;Stop;0;%ld|0x%02x/\0", time(NULL), 200);//c8
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}

/*************************************************************************
	 DisplayStimulus()
	 -	Handles the display of the stimulus.
	 -	Because we're redrawing the frames, it will only run at 30Hz
	 -	With behavior contingency, loss of fixation at any time (except reward period) should reset the trial to initiation
		There are up to 4 possible periods:
		1) Initiation period
			Only shown when behavior contingency is on and the delay is > 0
			Shows the fixation dot on a blank screen
		2) Presentation period
			Always shown
			Because it can take some time to draw a new full screen grating, delays < 200ms or so might not work
			Small gabor patches draw faster and allow smaller delays
		3) Blank period
			Only shown when the delay is > 0
			Shows the fixation dot on a blank screen
		4) Reward period
			Only shown when behavior contingency is on and the delay is > 0
			Shows a blank screen
*************************************************************************/
bool CGratingsStimulus::DisplayStimulus() 
{	
	double tmpAvgLX, tmpAvgLY, tmpAvgRX, tmpAvgRY;

	/* Set up */
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);
	vsgSetDrawMode(vsgCENTREXY);

	/* Current draw page */
	vsgSetDrawPage(vsgVIDEOPAGE, m_pageIx, vsgBACKGROUND);
	vsgMoveScreen(0, 0);

	/* Reset rotating phases and angles when behavioral contingency fails */
	int resetPhase = 0;
	int resetAngle = 0;
	
	/* Update static displays */
	char buffer[1024];

	/* Get next trial, making list circular if we've indexed out. */
	if ((m_trialIndex < 0) || (m_trialIndex >= g_pGratings->numTrials)) m_trialIndex = 0;
	CGratingsTrial *currentTrial = g_pGratings->trials[m_trialIndex]->clone();
	m_phase = currentTrial->phase;
	m_angle = currentTrial->orientation;

	/* Analog output */
	/* Line 16 is phase */
	/* Line 17 is orientation */
	etu::AnalogOut(4, -100.0 / g_pSettings->getDegPerVolt());
	etu::AnalogOut(5, -100.0 / g_pSettings->getDegPerVolt());

	/* Always allow reward when behavioral contingency is off */
	if (g_pGratings->getBehavior()) {
		/* New, remote-reward-driven behavior-contingent trial */
		vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	} 
	else {
		/* Trial / non-behavior-contingent */
		vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	}

	/* Move shutters on first init, with a blank screen; otherwise they'll be 
	 * moved if needed during the reward period */
	if (m_firstInit) {
		drawBlankScreen(false, false);
		m_savedShutterState = changeShutters(currentTrial);

		/* Set up automatic rf mapping */
		m_rfCurrentWidth = g_pGratings->getInitialSize();
		if (g_pGratings->getRFEye()==0) {
			m_rfCurrentX = g_pGratings->getXLeft();
			m_rfCurrentY = g_pGratings->getYLeft();
		} 
		else {
			m_rfCurrentX = g_pGratings->getXRight();
			m_rfCurrentY = g_pGratings->getYRight();
		}

		m_firstInit = false;
	}
	
	/* Get log_2(initial/final) */
	int numsteps = 1 + (int)ceil(log(g_pGratings->getInitialSize()/g_pGratings->getFinalSize()) / log(2.0));
	m_bReset = false;
	
	/* Show initiation period? */
	etu::AnalogOut(0, (float)(fixPoint(currentTrial->leftEyeFixating).xStimDeg()) / g_pSettings->getDegPerVolt());
	etu::AnalogOut(1, (float)(fixPoint(currentTrial->leftEyeFixating).yStimDeg()) / g_pSettings->getDegPerVolt());
	if (g_pGratings->getBehavior() && (g_pGratings->getInitiationDelay() > 0)) 
	{
		behaviorResetPulse();
		drawBlankScreen(true, currentTrial->leftEyeFixating);

		m_frameTimer.StartTimer();
		while (!m_bReset && !bEndThread && 
			(m_frameTimer.CheckTimer() < g_pGratings->getInitiationDelay())) 
		{
			m_bReset = vsgIOReadDigitalIn() & vsgDIG0;
			/* Sample eye position and update GUI. */
			g_pEyeTrackingDlg->EyePosSample();
			g_pGratingsDlg->UpdateEyePositions(
				g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
				g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);
			WaitUntil(5);
		}
	}

	/* Draw grating */
	if (!m_bReset) {
		/* Prep serial output*/
		char buffer2[256];
		int shutterCode = (((vsgIOReadDigitalOut() & (vsgDIG6)) != 0)? 1 : 0) + (((vsgIOReadDigitalOut() & (vsgDIG5)) != 0)? 2 : 0);
		int angleCode = (1+(  ((int)m_angle)/((360/g_pGratings->getNumAngles()))  ));
		sprintf(buffer2, "Phs %.2f, Orn %.2f|0x%02x/\0", m_phase, m_angle, 0xFF & angleCode );

		/* Store eye positions and draw grating */
		tmpAvgLX = g_pEyeTrackingDlg->avglx;
		tmpAvgLY = g_pEyeTrackingDlg->avgly;
		tmpAvgRX = g_pEyeTrackingDlg->avgrx;
		tmpAvgRY = g_pEyeTrackingDlg->avgry;
		drawGrating(currentTrial, tmpAvgLX, tmpAvgLY, tmpAvgRX, tmpAvgRY);

		/* Counter */
		if (g_pGratings->getNumRepeats() > 0) {
			sprintf(buffer, "Counter: %d / %d\0", m_trialIndex+1, g_pGratings->numTrials);
		} 
		else {
			sprintf(buffer, "Counter: %d / infinity\0", m_trialIndex+1);
		}
		g_pGratingsDlg->m_PresentationCounter.SetWindowText(buffer);

		/* Send serial mark */
		g_pSettings->theSerialComm.transmit(buffer2, (int)strlen(buffer2));

		/* 0xA1 - neither (not used) */
		int eyeCode = 0xA1;
		char buffer3[60];
		if (currentTrial->type == CGratingsTrial::TYPE_GABOR_LEFT) {
			sprintf(buffer3, "Left\0");
			eyeCode = 0xA2;
		} 
		else if (currentTrial->type == CGratingsTrial::TYPE_GABOR_RIGHT) {
			sprintf(buffer3, "Right\0");
			eyeCode = 0xA3;
		} 
		else if (currentTrial->type == CGratingsTrial::TYPE_GABOR_BOTH) {
			sprintf(buffer3, "Both\0");
			eyeCode = 0xA4;
		} 
		else if (currentTrial->type == CGratingsTrial::TYPE_SINEWAVE) {
			sprintf(buffer3, "FFSine\0");
			eyeCode = 0xA5;
		} 
		else if (currentTrial->type == CGratingsTrial::TYPE_SQUAREWAVE){
			sprintf(buffer3, "FFSquare\0");
			eyeCode = 0xA6;
		} 
		else {
			sprintf(buffer3, "RFMap%d %.4f\0", currentTrial->locationCode, m_rfCurrentWidth);
			eyeCode = 0xA7;

			/* RF mapping analog output location */
			etu::AnalogOut(2, ((float)m_rfCurrentX) / g_pSettings->getDegPerVolt());
			etu::AnalogOut(3, ((float)m_rfCurrentY) / g_pSettings->getDegPerVolt());
		}
		sprintf(buffer2, "Fixating: %s, Grating: %s, L[%.1f, %.1f], R[%.1f, %.1f]|0x%02x/\0",
			(currentTrial->leftEyeFixating)? "Left\0" : "Right\0",
			buffer3, tmpAvgLX, tmpAvgLY, tmpAvgRX, tmpAvgRY, 0xFF & eyeCode);
		g_pSettings->theSerialComm.transmit(buffer2, (int)strlen(buffer2));

		/* Presentation delay */
		m_frameTimer.StartTimer();

		VSGLUTBUFFER oldLutBuffer;
		vsgLUTBUFFERRead(1, &oldLutBuffer);
		for (int ix = 0; ix < 256; ix++) { m_lutBuffer[ix] = oldLutBuffer[ix]; }
		while (!(g_pGratings->getBehavior() && m_bReset) &&
			(m_frameTimer.CheckTimer() < g_pGratings->getDelay()) &&
			!bEndThread) 
		{
			/* Add motion */
			if (g_pGratings->getSpeed() != 0) {
				/* Modify phase for animation */
				double cyclesPerSec = currentTrial->speed * g_pGratings->getGratingWidth();
				/* Animate palette instead or image for a better frame rate */
				int pshift = (int)((double)(currentTrial->nPaletteItems)*cyclesPerSec/60.0);//60Hz
				/* Use [2,201] for grating palette levels */
				for (int ix = 0; ix < currentTrial->nPaletteItems; ix++) {
					/* Shift palette and redraw it */
					int index = (ix + pshift) % (currentTrial->nPaletteItems);
					m_lutBuffer[ix+2] = oldLutBuffer[index+2];
				}
				for (int ix = 0; ix < 256; ix++) { oldLutBuffer[ix] = m_lutBuffer[ix]; }
				vsgSetBackgroundColour(&m_lutBuffer[vsgBACKGROUND]);
				vsgSetFixationColour(&m_lutBuffer[vsgFIXATION]);
				vsgLUTBUFFERWrite(1, &m_lutBuffer);
				vsgLUTBUFFERtoPalette(1);
			}

			/* Sample eye position and update GUI. */
			g_pEyeTrackingDlg->EyePosSample();
			g_pGratingsDlg->UpdateEyePositions(
				g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
				g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);

			if (g_pGratings->getBehavior()) {
				m_bReset = vsgIOReadDigitalIn() & vsgDIG0;
			}
			WaitUntil(1);
		}

		m_frameTimer.CheckTimerWithReset();
	}

	/* Show blank period */
	if (!m_bReset && (g_pGratings->getBlank() > 0)) {
		drawBlankScreen(true, currentTrial->leftEyeFixating);
		
		/* Blank period delay */
		m_frameTimer.StartTimer();
		if (g_pGratings->getBehavior()) {
			while (!m_bReset && (m_frameTimer.CheckTimer() < g_pGratings->getBlank()) && !bEndThread) {
				m_bReset = vsgIOReadDigitalIn() & vsgDIG0;

				/* Sample eye position and update GUI. */
				g_pEyeTrackingDlg->EyePosSample();
				g_pGratingsDlg->UpdateEyePositions(
					g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
					g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);

				WaitUntil(5);
			}
		} 
		else {
			while ( (m_frameTimer.CheckTimer() < g_pGratings->getBlank()) && !bEndThread) { WaitUntil(10); }
		}
		m_frameTimer.CheckTimerWithReset();
	}

	/* Show reward period? */
	m_frameTimer.StartTimer();
	if (!m_bReset) {
		/* Move to next trial */
		m_trialIndex++;
		if (m_trialIndex >= g_pGratings->numTrials) {
			if (g_pGratings->getNumRepeats() > 0) {
				bEndThread = true;
			}
			/* Loop */
			m_trialIndex = 0;
		}

		/* Update rf mapping? */
		if (currentTrial->type == CGratingsTrial::TYPE_AUTORFMAP) {
			if ((m_trialIndex % (g_pGratings->numTrials / numsteps)) == 0) {
				/* If it's time to move to another step, find which position 
				 * to zoom into */
				bool choosetop = vsgIOReadDigitalIn() & vsgDIG1;
				bool chooseright = vsgIOReadDigitalIn() & vsgDIG2;
				
				/* Do the zoom */
				m_rfCurrentWidth /= 2.0;
				m_rfCurrentX += (chooseright)? 0.5 * m_rfCurrentWidth : -0.5 * m_rfCurrentWidth;
				m_rfCurrentY += (choosetop)? 0.5 * m_rfCurrentWidth : -0.5 * m_rfCurrentWidth;

				/* Update rf location estimate */
				sprintf(buffer, "%.3f, %.3f\0", m_rfCurrentX, m_rfCurrentY);
				if (g_pGratings->getRFEye()==0) {
					g_pGratingsDlg->m_RFL.SetWindowText(buffer);
				} 
				else {
					g_pGratingsDlg->m_RFR.SetWindowText(buffer);
				}
			}
		}
	}

	CGratingsTrial *nextTrial = g_pGratings->trials[m_trialIndex]->clone();
	if (!(g_pGratings->getBehavior())) m_savedShutterState = changeShutters(nextTrial);
	if (!m_bReset && (g_pGratings->getRewardDelay() > 0) && (g_pGratings->getBehavior())) {
		m_savedShutterState = changeShutters(nextTrial);
		drawBlankScreen(false, currentTrial->leftEyeFixating);

		bool rewnow = true;
		if (currentTrial->type == CGratingsTrial::TYPE_AUTORFMAP) {
			rewnow = (m_trialIndex % (g_pGratings->numTrials / numsteps)) == 0;
		}
		if (rewnow) {
			/* Pulse remote reward */
			vsgIOWriteDigitalOut(0x0000, vsgDIG0); // Redundant but safe
			vsgIOWriteDigitalOut(0xFFFF, vsgDIG7); // Allow reward
			WaitUntil(140);
			vsgIOWriteDigitalOut(0x0000, vsgDIG7); // Prevent, so there is only one feed
			while ((m_frameTimer.CheckTimer() < g_pGratings->getRewardDelay()) && !bEndThread) { WaitUntil(10); }
			m_frameTimer.CheckTimerWithReset();
		}
	}

	/* Time to stop? */
	if (g_pGratings->getNumRepeats() > 0) {
		if (m_trialIndex >= g_pGratings->numTrials) {
			/* It's time to stop */
			bEndThread = true;
		}
	}

	delete currentTrial;	currentTrial=NULL;
	delete nextTrial;	nextTrial=NULL;

	return true;
}

void CGratingsStimulus::drawBlankScreen(bool displayDot, bool displayFixLeft) 
{
	VSGTRIVAL bgColor;
	VSGTRIVAL fgColor;

	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;
	
	fgColor.a = ((double)g_pSettings->getRed()) / 255.0f;
	fgColor.b = ((double)g_pSettings->getGreen()) / 255.0f;
	fgColor.c = ((double)g_pSettings->getBlue()) / 255.0f;
		
	m_lutBuffer[0] = bgColor;
	m_lutBuffer[1] = fgColor;
	
	for (int ix = 2; ix < 256; ix++) {
		m_lutBuffer[ix] = bgColor;
	}

	vsgSetDrawPage(vsgVIDEOPAGE, m_pageIx, vsgBACKGROUND);

	if (displayDot) {
		CCoordinate& fixCoord = fixPoint(displayFixLeft);
		double dotPix;

		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,  g_pGratings->getDotSize(), 
			vsgPIXELUNIT, &dotPix);

		vsgSetPen1(1);
		vsgDrawOval(fixCoord.xScreenPix(), fixCoord.yScreenPix(), dotPix, dotPix);

		etu::AnalogOut(0, (float)(fixCoord.xStimDeg()) / g_pSettings->getDegPerVolt());
		etu::AnalogOut(1, (float)(fixCoord.yStimDeg()) / g_pSettings->getDegPerVolt());
	} 
	else {
		etu::AnalogOut(0, (float)-99.5 / g_pSettings->getDegPerVolt() );
		etu::AnalogOut(1, (float)-99.5 / g_pSettings->getDegPerVolt() );
	}

	/* Analog output */
	etu::AnalogOut(4, -100.0 / g_pSettings->getDegPerVolt()); // Phase
	etu::AnalogOut(5, -100.0 / g_pSettings->getDegPerVolt()); // Angle

	vsgLUTBUFFERWrite(1, &m_lutBuffer);
	vsgLUTBUFFERtoPalette(1);
	vsgSetBackgroundColour(&m_lutBuffer[vsgBACKGROUND]);
	vsgSetFixationColour(&m_lutBuffer[vsgFIXATION]);

	vsgPresent();
	m_pageIx = 1 - m_pageIx;
}

void CGratingsStimulus::behaviorResetPulse() 
{
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG1);
	WaitUntil(20);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);
}

int CGratingsStimulus::changeShutters(CGratingsTrial *trial) 
{
	/* Prepare shutters */
	if (g_pGratings->getShutters() == CGratings::SHUTTER_MANUAL) {
		/* Enable controls */
		g_pEyeTrackingDlg->m_ShutterL.EnableWindow();
		g_pEyeTrackingDlg->m_ShutterR.EnableWindow();
	} 
	else {
		/* Disable controls */
		g_pEyeTrackingDlg->m_ShutterL.EnableWindow(false);
		g_pEyeTrackingDlg->m_ShutterR.EnableWindow(false);

		/* Automatically pick current shutter state.
		 * Do not change shutters after a reset trial. */
		if (!m_bReset) {
			g_pEyeTrackingDlg->OpenRightShutter(trial->rightShutterOpen);
			g_pEyeTrackingDlg->OpenLeftShutter(trial->leftShutterOpen);
		}
	}

	/* Delay for shutters to move.
	 * Do not use this state after a reset. */
	if (!m_bReset && (g_pGratings->getShutterDelay() > 0)) {
		/* Show blank screen */
		drawBlankScreen(false, trial->leftEyeFixating);

		etu::AnalogOut(0, (float)-99.5 / g_pSettings->getDegPerVolt() );
		etu::AnalogOut(1, (float)-99.5 / g_pSettings->getDegPerVolt() );

		etu::CTimer ShutterTimer;
		/* Now wait while, presumably, the shutters move. */
		ShutterTimer.StartTimer();
		while ((ShutterTimer.CheckTimer() < g_pGratings->getShutterDelay()) &&
				!bEndThread) 
		{
			WaitUntil(10);
		}
	}

	return 0; /* sstate */
}

void CGratingsStimulus::drawGrating(CGratingsTrial *trial, double tmpAvgLX, 
									double tmpAvgLY, double tmpAvgRX, 
									double tmpAvgRY)
{
	VSGTRIVAL bgColor;
	VSGTRIVAL fgColor;
	double leftX = 0;
	double leftY = 0; 
	double rightX = 0;
	double rightY = 0;
	double offsetX = 0;
	double offsetY = 0;
	double screenOffsetX = 0;
	double screenOffsetY = 0;
	double dotSize = 0;
	int numUsed = 200;
	double cyclesPerDeg;
	double totalOffsetX;
	double totalOffsetY;
	double patchSize = 0;
	double patch1X = 0;
	double patch1Y = 0;
	double patch2X = 0;
	double patch2Y = 0;
	bool drawGabor;

	/* Init palette */
	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;
	
	fgColor.a = ((double)g_pSettings->getRed()) / 255.0f;
	fgColor.b = ((double)g_pSettings->getGreen()) / 255.0f;
	fgColor.c = ((double)g_pSettings->getBlue()) / 255.0f;

	m_lutBuffer[0] = bgColor;
	m_lutBuffer[1] = fgColor;

	for (int ix = 2; ix < 256; ix++) {
		m_lutBuffer[ix] = bgColor;
	}

	m_lutBuffer[vsgBACKGROUND] = bgColor;
	m_lutBuffer[vsgFIXATION] = fgColor;

	/* Get fixation dot and grating sizes */
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,  g_pSettings->getXOffset(), vsgPIXELUNIT, &offsetX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &offsetY);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,  g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenOffsetX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenOffsetY);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,  g_pGratings->getDotSize(), vsgPIXELUNIT, &dotSize);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,  g_pGratings->getGaborSD(), vsgPIXELUNIT, &patchSize);

	cyclesPerDeg = g_pGratings->getGratingWidth();

	totalOffsetX = offsetX + screenOffsetX + m_gridXPix;
	totalOffsetY = offsetY + screenOffsetY + m_gridYPix;

	vsgSetDrawPage(vsgVIDEOPAGE, m_pageIx, vsgBACKGROUND);
	vsgSetPen1(2);
	vsgSetPen2(201);

	drawGabor = (
		(trial->type == CGratingsTrial::TYPE_GABOR_LEFT) || 
		(trial->type == CGratingsTrial::TYPE_GABOR_RIGHT) || 
		(trial->type == CGratingsTrial::TYPE_GABOR_BOTH));

	if (drawGabor) {
		double fieldLocX = 0;
		double fieldLocY = 0;
		double trackedLeftX = 0;
		double trackedLeftY = 0;
		double trackedRightX = 0;
		double trackedRightY = 0;

		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,  g_pGratings->getRFX(),  vsgPIXELUNIT, &fieldLocX);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pGratings->getRFY(),  vsgPIXELUNIT, &fieldLocY);

		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,  tmpAvgLX,  vsgPIXELUNIT, &trackedLeftX);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -tmpAvgLY,  vsgPIXELUNIT, &trackedLeftY);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,  tmpAvgRX,  vsgPIXELUNIT, &trackedRightX);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -tmpAvgRY,  vsgPIXELUNIT, &trackedRightY);
		
		/* Draw for fixating eye */
		if (trial->type == CGratingsTrial::TYPE_GABOR_LEFT) {
			patch1X = trackedLeftX + fieldLocX + totalOffsetX;
			patch1Y = trackedLeftY + fieldLocY + totalOffsetY;
		} 
		else if (trial->type == CGratingsTrial::TYPE_GABOR_RIGHT) {
			patch1X = trackedRightX + fieldLocX + totalOffsetX;
			patch1Y = trackedRightY + fieldLocY + totalOffsetY;
		} 
		else if (trial->type == CGratingsTrial::TYPE_GABOR_BOTH){
			if (trial->leftEyeFixating) {
				patch1X = trackedLeftX + fieldLocX + totalOffsetX;
				patch1Y = trackedLeftY + fieldLocY + totalOffsetY;
				patch2X = trackedRightX + fieldLocX + totalOffsetX;
				patch2Y = trackedRightY + fieldLocY + totalOffsetY;
			} 
			else {
				patch1X = trackedRightX + fieldLocX + totalOffsetX;
				patch1Y = trackedRightY + fieldLocY + totalOffsetY;
				patch2X = trackedLeftX + fieldLocX + totalOffsetX;
				patch2Y = trackedLeftY + fieldLocY + totalOffsetY;
			}
		} 
		else {
			/* Should never happen */
			char buffer[1024];
			sprintf(buffer, "unknown trial type! %d\0", trial->type);
			AfxMessageBox(buffer);
		}

		numUsed = vsgDrawGrating(patch1X, patch1Y, patchSize, patchSize, trial->orientation, cyclesPerDeg);
		if (trial->type == CGratingsTrial::TYPE_GABOR_BOTH){
			numUsed = vsgDrawGrating(patch2X, patch2Y, patchSize, patchSize, trial->orientation, cyclesPerDeg);
		}
	} 
	else {
		/* Square and sine wave (palette determines which one) */
		if (trial->type == CGratingsTrial::TYPE_AUTORFMAP) {
			/* Draw correct quadrant:
			 *    0 | 1
			 *   ---+---
			 *    2 | 3
			 */
			double x = m_rfCurrentX + ((((trial->locationCode-1) % 2)==0)? -1.0 : 1.0)*0.25*m_rfCurrentWidth;
			double y = m_rfCurrentY + ((((trial->locationCode-1) / 2)==0)? 1.0 : -1.0)*0.25*m_rfCurrentWidth;
			/**
			 * @TODO Send secondary x/y analog channels with x,y location
			 */
			double xpix=0, ypix=0, wpix=0;
			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, x, vsgPIXELUNIT, &xpix);
			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -y, vsgPIXELUNIT, &ypix);
			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, m_rfCurrentWidth*0.5, vsgPIXELUNIT, &wpix);
				
			numUsed = vsgDrawGrating(screenOffsetX+xpix, screenOffsetY+ypix, wpix, wpix, trial->orientation, cyclesPerDeg);
		} 
		else {
			/* Full screen */
			numUsed = vsgDrawGrating(0, 0, m_screenWidthPix, m_screenHeightPix, trial->orientation, cyclesPerDeg);//full field ignores offsets
		}
	}

	if (false) {
		/* Debug color palette */
		const double width = m_screenWidthPix / 256;
		const double pad   = m_screenWidthPix / 512;
		const double shift = -1.0 * m_screenWidthPix / 2;
		const double yPos  = (m_screenHeightPix / 2) - 9;
		for (int ix = 0; ix < 256; ix++) {
			vsgSetPen1(ix);
			vsgDrawRect((ix * width) + pad + shift, yPos, width, 20);
		}
	}

	/* Draw a circular mat over the square grating.  Do all the work for two
	 * mats even if we won't use both, as we use both often enough. */
	if (drawGabor) {
		vsgSetPen1(vsgBACKGROUND);
		const int pointsOnCircle = 160;
		const double patchRadius = 0.5 * patchSize;
		/* Border so mat extends past edges of square grating */
		const int border = 5;
		/* Add six extra points to 'tie off' mat shape */
		const int polyPoints = pointsOnCircle + 1 + 4 + 1;
		/* Buffers [x1, y1, x2, y2, ...]; thus num points * 2 for size. */
		double* cutout1 = new double[polyPoints * 2];
		double* cutout2 = new double[polyPoints * 2];

		/* Store points for approx. circles around patch1 and patch2 */
		for (int ix = 0; ix < pointsOnCircle; ix++) {
			const double circleAngle = ((double)ix/(double)pointsOnCircle) * 2.0 * PI_DOUBLE;
			const double cosCircleAngle = cos(circleAngle);
			const double sinCircleAngle = sin(circleAngle);
			cutout1[2*ix]     = patch1X + patchRadius * cosCircleAngle;
			cutout1[2*ix + 1] = patch1Y - patchRadius * sinCircleAngle;
			cutout2[2*ix]     = patch2X + patchRadius * cosCircleAngle;
			cutout2[2*ix + 1] = patch2Y - patchRadius * sinCircleAngle;
		}

		/* Middle right. */
		cutout1[pointsOnCircle*2 + 0] =  border + patch1X + patchRadius;
		cutout1[pointsOnCircle*2 + 1] =  patch1Y;
		cutout2[pointsOnCircle*2 + 0] =  border + patch2X + patchRadius;
		cutout2[pointsOnCircle*2 + 1] =  patch2Y;
		/* Bottom right. */
		cutout1[pointsOnCircle*2 + 2] =  border + patch1X + patchRadius;
		cutout1[pointsOnCircle*2 + 3] =  border + patch1Y + patchRadius;
		cutout2[pointsOnCircle*2 + 2] =  border + patch2X + patchRadius;
		cutout2[pointsOnCircle*2 + 3] =  border + patch2Y + patchRadius;
		/* Bottom left. */
		cutout1[pointsOnCircle*2 + 4] = -border + patch1X - patchRadius;
		cutout1[pointsOnCircle*2 + 5] =  border + patch1Y + patchRadius;
		cutout2[pointsOnCircle*2 + 4] = -border + patch2X - patchRadius;
		cutout2[pointsOnCircle*2 + 5] =  border + patch2Y + patchRadius;
		/* Top left. */
		cutout1[pointsOnCircle*2 + 6] = -border + patch1X - patchRadius;
		cutout1[pointsOnCircle*2 + 7] = -border + patch1Y - patchRadius;
		cutout2[pointsOnCircle*2 + 6] = -border + patch2X - patchRadius;
		cutout2[pointsOnCircle*2 + 7] = -border + patch2Y - patchRadius;
		/* Top right. */
		cutout1[pointsOnCircle*2 + 8] =  border + patch1X + patchRadius;
		cutout1[pointsOnCircle*2 + 9] = -border + patch1Y - patchRadius;
		cutout2[pointsOnCircle*2 + 8] =  border + patch2X + patchRadius;
		cutout2[pointsOnCircle*2 + 9] = -border + patch2Y - patchRadius;
		/* Middle right (again). */
		cutout1[pointsOnCircle*2 + 10] = cutout1[pointsOnCircle*2 + 0];
		cutout1[pointsOnCircle*2 + 11] = cutout1[pointsOnCircle*2 + 1];
		cutout2[pointsOnCircle*2 + 10] = cutout2[pointsOnCircle*2 + 0];
		cutout2[pointsOnCircle*2 + 11] = cutout2[pointsOnCircle*2 + 1];
		
		vsgDrawPolygon(polyPoints, cutout1, 0, 0, 0);

		if (trial->type == CGratingsTrial::TYPE_GABOR_BOTH){
			vsgDrawPolygon(polyPoints, cutout2, 0, 0, 0);
		}

		delete [] cutout1;
		delete [] cutout2;
	}

	/* Fixation dot */
	vsgSetPen1(1);
	vsgDrawOval(fixPoint(trial->leftEyeFixating).xScreenPix(),
		fixPoint(trial->leftEyeFixating).yScreenPix(), dotSize, dotSize);
	/* Save the number of palette indices that are actually used */
	trial->nPaletteItems = numUsed;

	/* Refine palette 
	 * Use [2,201] for grating palette levels. */
	for (int ix = 2; ix < 2 + numUsed; ix++) {
		double pct = ((double)(ix - 2))/((double)numUsed);
		pct += trial->phase / 360.0;
		while (pct < 0.0) pct += 1.0;
		while (pct >= 1.0) pct -= 1.0;
		if (g_pGratings->getType() == CGratings::STYLE_SQUARE) {
			pct = (pct <= 0.5) ? 1.0 : 0.0;
		} 
		else if (g_pGratings->getType() == CGratings::STYLE_SINE) {
			pct = 0.5 + 0.5 * sin(2.0 * PI_DOUBLE * pct);
		} 
		else {
			/* Cropped grating */
			pct = 0.5 + 0.5 * sin(2.0 * PI_DOUBLE * pct);
		}
		m_lutBuffer[ix].a = (pct * ((double)g_pGratings->getGratingRed()) + (1.0 - pct) * ((double)g_pGratings->getGratingRed2())) / 255.0f;
		m_lutBuffer[ix].b = (pct * ((double)g_pGratings->getGratingGreen()) + (1.0 - pct) * ((double)g_pGratings->getGratingGreen2())) / 255.0f;
		m_lutBuffer[ix].c = (pct * ((double)g_pGratings->getGratingBlue()) + (1.0 - pct) * ((double)g_pGratings->getGratingBlue2())) / 255.0f;
	}

	/* Now display grating by updating palette and calling vsgPresent */
	vsgLUTBUFFERWrite(1, &m_lutBuffer);
	vsgLUTBUFFERtoPalette(1);
	vsgSetBackgroundColour(&m_lutBuffer[vsgBACKGROUND]);
	vsgSetFixationColour(&m_lutBuffer[vsgFIXATION]);
	vsgPresent();
	m_pageIx = 1 - m_pageIx;
	/* Analog output */
	etu::AnalogOut(4, (float)(trial->phase/360.0) * 100.0 / g_pSettings->getDegPerVolt());//output 0-360deg over range
	etu::AnalogOut(5, (float)(trial->orientation/360.0) * 100.0 / g_pSettings->getDegPerVolt());
}

/***************************************************************
	 StimulusType()
	 -  Inherited method that returns the stimulus type string.
****************************************************************/
CString CGratingsStimulus::StimulusType() 
{
	return "Gratings Stimulus";
}

#endif // APP_EYE_TRACKING
