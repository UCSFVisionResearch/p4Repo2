/**
 * @file StepRampStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "StepRampStimulus.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	VSG IN 0:	Next Stim Pulse. 1 => Behavioral contingency
				has been met, move on to the excursion phase.
				0 => behvioral continengency criteria has not 
				yet been met.

	Analog Out 0:	Horizontal target location
	Analog Out 1:	Vertical target location
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor     
	 -  Instantiates the newly created object
************************************************************/
CStepRampStimulus::CStepRampStimulus() 
{
	Initialize();

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	
	pointX, pointY = 0;
	iPage = 0;
	waitTimer = new etu::CTimer();
	iPhase = 0;
	bDrawn = false;
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	g_pStepRamp->sendSettings(g_pSettings->theSerialComm);
}


/*****************************************************************
	 Destructor
	 -  Cleans up when the object is deleted. The trial running
	 signal is turned off.
*****************************************************************/
CStepRampStimulus::~CStepRampStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	if (waitTimer)
		delete waitTimer;

	//send stop mark
	char buffer[80];
	sprintf(buffer, "StepRamp;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}


/*************************************************************************
	 DisplayStimulus()
	 -  Handles the display of the stimulus. This function simply
	 dispatches the display responsibilities to different functions
	 based on which phase the stimulus is in. (After redesigning, 
	 iPhase is always 0 at this point -- and the DoInitialDot() 
	 routine calls DoExcursion, which calls DoPlaateau(). Thus the
	 entire stimulus presentation is initiated by calling DoInitialDot.
*************************************************************************/
bool CStepRampStimulus::DisplayStimulus() 
{
	switch (iPhase) {
		case 0: DoInitialDot(); break;
		case 1: DoExcursion(); break;
		case 2: DoPlateau(); break;
	}
	return true;
}

/************************************************************************
	 DoInitialDot()
	 -  Presents the initial fixation dot that starts teh trial. 
	 The dot is drawn at the location specified by the offsets
	 specified in the settings tab. If the stimulus is running with a 
	 behavioral contingency, then this function waits for the 
	 fixation criteria to be met before moving on to the 
	 DoExcursion method.
************************************************************************/
void CStepRampStimulus::DoInitialDot() 
{	
	if (!bDrawn) {
		// start the timer
		waitTimer->StartTimer();

		vsgSetDisplayPage(0);

		// Randomly choose a direction based on the 
		// directions allowed in the dialog box.
		g_pStepRamp->chooseDirection();
		
		// calculate the degrees per frame for the motion
		degPerSec = g_pStepRamp->getVelocity();
		degPerFrame = degPerSec / ((float)g_dFramerate);
		
		// initialize the colors
		dotColor.a = ((double)g_pSettings->getRed()) / 255.0f;
		dotColor.b = ((double)g_pSettings->getGreen()) / 255.0f;
		dotColor.c = ((double)g_pSettings->getBlue()) / 255.0f;

		VSGTRIVAL bgColor;
		bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
		bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
		bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;
				
		// set the view distance
		vsgSetViewDistMM(g_pSettings->getViewDistance());
		vsgSetSpatialUnits(vsgPIXELUNIT);	

		// set the background color for the next page to be drawn
		vsgSetBackgroundColour(&bgColor);

		// convert the degrees to pixels
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pStepRamp->getDotSize(), vsgPIXELUNIT, &dSize);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &xOffset);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &yOffset);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);

		// delete old OAS objects
		vsgObjDestroy(g_stim);
		vsgObjDestroy(g_fixation);

		// set the drawing page and mode
		vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
		vsgSetDrawMode(vsgCENTREXY);

		// reset the screen origins
		pointX = 0;
		pointY = 0;
		vsgMoveScreen(pointX, pointY);

		// set the drawing origin at the center of the screen
		vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix /2 );

		// create a new OAS object
		g_stim = vsgObjCreate();
		vsgObjSetContrast(100);
		vsgObjSetPixelLevels(2,1); // this line is probably not necessary
		
		// set the dot color
		vsgObjSetColourVector(&blackColor,&dotColor,vsgBIPOLAR);

		// draw hte fixation point
		vsgSetDrawMode(vsgCENTREXY);
		//vsgDrawOval(xOffset + screenXOffset, yOffset + screenYOffset, dSize, dSize);
		double startX=0, startY=0;
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pStepRamp->getFixationX(), vsgPIXELUNIT, &startX);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pStepRamp->getFixationY(), vsgPIXELUNIT, &startY);
		vsgDrawOval(startX + xOffset + screenXOffset, startY + yOffset + screenYOffset, dSize, dSize);
		
		// present the fixation point
		vsgPresent();

		// set the drawn flag to true to prevent the drawing to occur again
		// until after the excursion and plateau are done.
		bDrawn = true;
		
		// update the analog output for the target position
		etu::AnalogOut(0, (g_pSettings->getXOffset()+g_pStepRamp->getFixationX()) / g_pSettings->getDegPerVolt());
		etu::AnalogOut(1, (g_pSettings->getYOffset()+g_pStepRamp->getFixationY()) / g_pSettings->getDegPerVolt());
	}

	// if the stimulus is set to behaviorally contingent, then wait for
	// the nextStim signal from the 1401 (spike2)
	if (g_pStepRamp->useBehavior()) {
		int response = vsgIOReadDigitalIn() & vsgDIG0;//0x001
		
		// if the signal has not come in yet, return from teh function
		// to allow the UI thread to process for a brief moment. This will
		// allow the user to abort whenever needed. 
		if (response == 0) {
			return;
		}
	} 
	else { // if not running on behavioral contingency, wait for the OnDelay()

		WaitUntil(g_pStepRamp->getOnDelay() - waitTimer->CheckTimer());
	}
	
	// reset the drawn flag to force the next trial to redraw the dot when
	// needed.
	bDrawn = false;
	iPhase = 1;
	iPage = 1 - iPage;
	
	// do the excursion
	DoExcursion();
}

/*********************************************************************
	 DoExcursion
	 -  Executes the excursion portion of the stimulus presentation.
*********************************************************************/
void CStepRampStimulus::DoExcursion() 
{
	double dPixPerFrame, startX, startY;
	double startDegX, startDegY;
		
	// calculate the number of frames needed to display teh excursion
	int iNumTableElements = (int)(g_pStepRamp->getExcursion() / degPerFrame);
	
	// set the viewing distance and spatial units
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);	

	// convert degrees to pixels
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, degPerFrame, vsgPIXELUNIT, &dPixPerFrame);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pStepRamp->getStartPointX(), vsgPIXELUNIT, &startX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pStepRamp->getStartPointY(), vsgPIXELUNIT, &startY);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);

	// move the dots to the starting point of the excursion
	// we do not incorporate the offset into these 3 lines because
	// teh original dot was already drawn with offsets taken into consideration.
	// This way we only need to offset it to the starting position.
	pointX -= (startX);
	pointY += (startY);
	vsgMoveScreen(pointX, pointY);

	// calculate the target position
	startDegX = (g_pSettings->getXOffset() + g_pStepRamp->getStartPointX())+g_pStepRamp->getFixationX();
	startDegY = (g_pSettings->getYOffset() + g_pStepRamp->getStartPointY())+g_pStepRamp->getFixationY();

	// update the analog output for the target position 
	etu::AnalogOut(0, (float)startDegX / g_pSettings->getDegPerVolt());
	etu::AnalogOut(1, (float)startDegY / g_pSettings->getDegPerVolt());

	// Move the dot frame by frame
	for (int i = 0; i < iNumTableElements; i++) {
		// calculate the movement in the x and y directions
		pointX += -dPixPerFrame * g_pStepRamp->getStepX();
		pointY += -dPixPerFrame * g_pStepRamp->getStepY();

		// move the screen the appropriate number of pixels to create the appearance of motion
		vsgMoveScreen(pointX, pointY);

		// update the analog output for the target position to reflect the motion of the dot
		etu::AnalogOut(0, (float)(startDegX + degPerFrame * g_pStepRamp->getStepX() * i) / g_pSettings->getDegPerVolt());
		etu::AnalogOut(1, (float)(startDegY - degPerFrame * g_pStepRamp->getStepY() * i) / g_pSettings->getDegPerVolt());
	}
	
	iPhase = 2;

	// move to the next phase of hte stimulus
	DoPlateau();
}

/********************************************************************
	 DoPlateau
	 -  Executes the plateau portion of the stimulus presentation.
*********************************************************************/
void CStepRampStimulus::DoPlateau() 
{
	// If the plateau time is non-zero, then
	// wait for that amount of time (leaving the dot on the screen)
	if (g_pStepRamp->getPlateau() > 1) {
		WaitUntil(g_pStepRamp->getPlateau());
	}

	// if there is an off delay, turn off the stimulus
	// and wait for the specified amount of time
	if (g_pStepRamp->getOffDelay() > 1) {
		// start the timer
		waitTimer->StartTimer();

		// clear the screen
		vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
		vsgPresent();
		iPage = 1 - iPage;
		vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
		vsgPresent();
		iPage = 1 - iPage;

		// dot is off screen so make output as far off screen as possible.
		etu::AnalogOut(0, (float)-9.95);
		etu::AnalogOut(1, (float)-9.95);		
		
		// wait for the off delay
		WaitUntil(g_pStepRamp->getOffDelay() - waitTimer->CheckTimer());
	}

	iPhase = 0;
}

/***************************************************************
	 StimulusType()
	 -  Inherited method that returns the stimulus type string.
****************************************************************/
CString CStepRampStimulus::StimulusType() 
{
	return "Step Ramp Stimulus";
}

#endif // APP_EYE_TRACKING
