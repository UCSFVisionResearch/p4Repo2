/**
 * @file APDStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	VSG IN 0:	Next Stim Pulse. 1 => Behavioral contingency
				has been met, move on to the excursion phase.
				0 => behvioral continengency criteria has not 
				yet been met.
	Analog Out 0:	Horizontal target location
	Analog Out 1:	Vertical target location
	Analog Out 2:	Dots on=5, off=-5, intertrial=0
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor     
	 -  Instantiates the newly created object
************************************************************/
CAPDStimulus::CAPDStimulus() 
{
	Initialize();

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	
	pointX, pointY = 0;
	iPage = 0;
	waitTimer = new etu::CTimer();
	bOn = true;
	bLeft = true;

	dFrameTime = vsgGetSystemAttribute(vsgFRAMETIME)/ 1000.0;

	VSGLUTBUFFER lutbuffer;
	for (int i=0; i<256; i++) {
		lutbuffer[i].a=0.0;
		lutbuffer[i].b=0.0;
		lutbuffer[i].c=0.0;
	}
	vsgSetBackgroundColour(&lutbuffer[vsgBACKGROUND]);
	vsgSetFixationColour(&lutbuffer[vsgFIXATION]);
	vsgLUTBUFFERWrite(1,&lutbuffer);
	vsgLUTBUFFERtoPalette(1);
	
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	// set the drawing mode, and set the origin to the center of the screen
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix  /2, iScrHeightPix / 2);

	waitTimer->StartTimer();

	//theComm.init(false, CBR_115200, 1);
	g_pAPD->sendSettings(g_pSettings->theSerialComm);
}

/*****************************************************************
	 Destructor
	 -  Cleans up when the object is deleted. The trial running
	 signal is turned off.
*****************************************************************/
CAPDStimulus::~CAPDStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	if (waitTimer)
		delete waitTimer;

	//theComm.stop();
	
	//send stop mark
	char buffer[80];
	sprintf(buffer, "APD;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}

/*************************************************************************
	 DisplayStimulus()
	 -  Handles the display of the stimulus. 
*************************************************************************/
bool CAPDStimulus::DisplayStimulus() 
{

	VSGTRIVAL fixationColor;
	VSGTRIVAL backgroundColorOn, backgroundColorOff;

	fixationColor.a = ((double)g_pAPD->getFixationRed())/255.0f;
	fixationColor.b = ((double)g_pAPD->getFixationGreen())/255.0f;
	fixationColor.c = ((double)g_pAPD->getFixationBlue())/255.0f;

	backgroundColorOn.a = ((double)g_pAPD->getOnRed())/255.0f;
	backgroundColorOn.b = ((double)g_pAPD->getOnGreen())/255.0f;
	backgroundColorOn.c = ((double)g_pAPD->getOnBlue())/255.0f;

	backgroundColorOff.a = ((double)g_pAPD->getOffRed())/255.0f;
	backgroundColorOff.b = ((double)g_pAPD->getOffGreen())/255.0f;
	backgroundColorOff.c = ((double)g_pAPD->getOffBlue())/255.0f;

	int onMarker = 1;
	int offMarker = 2;

	VSGLUTBUFFER lutbuffer;
	vsgPaletteRead(&lutbuffer);
	bool change=false; //used to reduce flicker when updating palette
	if (lutbuffer[0].a != fixationColor.a ||
		lutbuffer[0].b != fixationColor.b ||
		lutbuffer[0].c != fixationColor.c ||
		lutbuffer[1].a != backgroundColorOn.a ||
		lutbuffer[1].b != backgroundColorOn.b ||
		lutbuffer[1].c != backgroundColorOn.c ||
		lutbuffer[2].a != backgroundColorOff.a ||
		lutbuffer[2].b != backgroundColorOff.b ||
		lutbuffer[2].c != backgroundColorOff.c)
	{
		change = change || true;
	}
	lutbuffer[0].a=fixationColor.a;
	lutbuffer[0].b=fixationColor.b;
	lutbuffer[0].c=fixationColor.c;
	lutbuffer[1].a=backgroundColorOn.a;
	lutbuffer[1].b=backgroundColorOn.b;
	lutbuffer[1].c=backgroundColorOn.c;
	lutbuffer[2].a=backgroundColorOff.a;
	lutbuffer[2].b=backgroundColorOff.b;
	lutbuffer[2].c=backgroundColorOff.c;
	for (int i=3; i<256; i++) {
		lutbuffer[i].a=backgroundColorOff.a;//0.0;
		lutbuffer[i].b=backgroundColorOff.b;//0.0;
		lutbuffer[i].c=backgroundColorOff.c;//0.0;
	}

	if (change) {
		vsgSetBackgroundColour(&lutbuffer[vsgBACKGROUND]);
		vsgSetFixationColour(&lutbuffer[vsgFIXATION]);
		vsgLUTBUFFERWrite(1,&lutbuffer);
		vsgLUTBUFFERtoPalette(1);
	}
	
	double screenXOffset, screenYOffset;
	double stimXOffset, stimYOffset;

	// convert offsets from degrees to pixels
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &stimXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &stimYOffset);

	double centerX, centerY, fixationRadius;
	double centerXLeft, centerXRight, centerYLeft, centerYRight;
	double fieldWidth, fieldHeight;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAPD->getFixationPosX(), vsgPIXELUNIT, &centerX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAPD->getFixationPosY(), vsgPIXELUNIT, &centerY);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAPD->getFixationSize(), vsgPIXELUNIT, &fixationRadius);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAPD->getFieldWidth(), vsgPIXELUNIT, &fieldWidth);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAPD->getFieldHeight(), vsgPIXELUNIT, &fieldHeight);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAPD->getCenterXLeft(), vsgPIXELUNIT, &centerXLeft);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAPD->getCenterYLeft(), vsgPIXELUNIT, &centerYLeft);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAPD->getCenterXRight(), vsgPIXELUNIT, &centerXRight);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAPD->getCenterYRight(), vsgPIXELUNIT, &centerYRight);
	
	char buffer[80];
	int length;
	double fieldCenterX, fieldCenterY;
	double fieldCenterXDeg, fieldCenterYDeg;
	if (g_pAPD->getField() == CAPD::FIELD_FULLSCREEN) {
		fieldCenterX = 0;
		fieldCenterY = 0;
		fieldCenterXDeg = 0;
		fieldCenterYDeg = 0;
		fieldWidth = 1024;
		fieldHeight = 768;
	} 
	else {
		if (bLeft) {
			fieldCenterX = centerXLeft;
			fieldCenterY = centerYLeft;
			fieldCenterXDeg = g_pAPD->getCenterXLeft();
			fieldCenterYDeg = g_pAPD->getCenterYLeft();
			onMarker = 3;
			offMarker = 4;
		} 
		else {
			fieldCenterX = centerXRight;
			fieldCenterY = centerYRight;
			fieldCenterXDeg = g_pAPD->getCenterXRight();
			fieldCenterYDeg = g_pAPD->getCenterYRight();
			onMarker = 5;
			offMarker = 6;
		}
	}

	// wait for the intertrial delay
	if (g_pAPD->getIntertrialDelay() > 0) {
		// no fixation target
		etu::AnalogOut(0, (float)-9.95);
		etu::AnalogOut(1, (float)-9.95);
		etu::AnalogOut(2, 0);

		vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
		iPage = 1-iPage;
		vsgSetDrawMode(vsgCENTREXY);
		
		if (g_pAPD->getShowFixation()) {
			if (g_pAPD->getField() == CAPD::FIELD_FULLSCREEN) {
				vsgSetPen1(0);
				// TODO
				vsgDrawOval(centerX + stimXOffset + screenXOffset, (-1 * centerY) + stimYOffset + screenYOffset, fixationRadius*2, fixationRadius*2);
			} 
			else {
				vsgSetPen1(0);
				// TODO
				vsgDrawOval(centerX + stimXOffset + screenXOffset, (-1 * centerY) + stimYOffset + screenYOffset, fixationRadius*2, fixationRadius*2);

				// TODO
				vsgDrawOval((-1 * centerX) + stimXOffset + screenXOffset, (-1 * centerY) + stimYOffset + screenYOffset, fixationRadius*2, fixationRadius*2);
			}
		}
		if (g_pAPD->getIntertrialDelay() > 0) {
			WaitUntil(g_pAPD->getIntertrialDelay() - waitTimer->CheckTimer() - (dFrameTime * 2), 5);
		}
		vsgPresent(); // dots, nothing
	}
	waitTimer->StartTimer();
	etu::AnalogOut(0, (float)fieldCenterXDeg / g_pSettings->getDegPerVolt());
	etu::AnalogOut(1, (float)fieldCenterYDeg / g_pSettings->getDegPerVolt());

	// wait for the fixation delay / behvioral contingency to be met
	// and then display the on period
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	iPage = 1-iPage;

	vsgSetDrawMode(vsgCENTREXY);
	vsgSetPen1(1);
	vsgDrawRect(fieldCenterX, -1 * fieldCenterY, fieldWidth, fieldHeight);
	
	if (g_pAPD->getShowFixation()) {
		if (g_pAPD->getField() == CAPD::FIELD_FULLSCREEN) {
			vsgSetPen1(0);
			vsgDrawOval(centerX + stimXOffset + screenXOffset, (-1 * centerY) + stimYOffset + screenYOffset, fixationRadius*2, fixationRadius*2);
		} 
		else {
			vsgSetPen1(0);
			vsgDrawOval(centerX + stimXOffset + screenXOffset, (-1 * centerY) + stimYOffset + screenYOffset, fixationRadius*2, fixationRadius*2);

			// TODO
			vsgDrawOval((-1 * centerX) + stimXOffset + screenXOffset, (-1 * centerY) + stimYOffset + screenYOffset, fixationRadius*2, fixationRadius*2);
		}
	}

	sprintf(buffer, "RGB: %.2f, %.2f, %.2f, x: %.2f, y: %.2f, W: %.2f, H: %.2f|0x0%x/", backgroundColorOn.a, backgroundColorOn.b, backgroundColorOn.c, fieldCenterXDeg, fieldCenterYDeg, g_pAPD->getFieldWidth(), g_pAPD->getFieldHeight(), onMarker);
	length = (int)strlen(buffer);
	if (g_pAPD->useBehavior() || g_pAPD->getFixationDelay() > 0) {
		int response = 0;
		while (response == 0) {
			response = vsgIOReadDigitalIn() & vsgDIG0;//0x001
			WaitUntil(10);
		}
	} 
	else {
		WaitUntil(g_pAPD->getFixationDelay() - waitTimer->CheckTimer() - (dFrameTime * 2), 5);//
	}

	vsgPresent();// dots, on

	waitTimer->StartTimer();
	etu::AnalogOut(0, (float)fieldCenterXDeg / g_pSettings->getDegPerVolt());
	etu::AnalogOut(1, (float)fieldCenterYDeg / g_pSettings->getDegPerVolt());
	etu::AnalogOut(2, (float)5);
	
	g_pSettings->theSerialComm.transmit(buffer, length);

	// display teh off period with fixation dot
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	iPage = 1-iPage;

	vsgSetPen1(2);
	vsgDrawRect(fieldCenterX, -1 * fieldCenterY, fieldWidth, fieldHeight);

	if (g_pAPD->getShowFixation()) {
		if (g_pAPD->getField() == CAPD::FIELD_FULLSCREEN) {
			vsgSetPen1(0);
			vsgDrawOval(centerX + stimXOffset + screenXOffset, (-1 * centerY) + stimYOffset + screenYOffset, fixationRadius*2, fixationRadius*2);
		} 
		else {
			vsgSetPen1(0);
			vsgDrawOval(centerX + stimXOffset + screenXOffset, (-1 * centerY) + stimYOffset + screenYOffset, fixationRadius*2, fixationRadius*2);

			vsgDrawOval((-1 * centerX) + stimXOffset + screenXOffset, (-1 * centerY) + stimYOffset + screenYOffset, fixationRadius*2, fixationRadius*2);
		}
	}
	sprintf(buffer, "RGB: %.2f, %.2f, %.2f, x: %.2f, y: %.2f, W: %.2f, H: %.2f|0x0%x/", backgroundColorOff.a, backgroundColorOff.b, backgroundColorOff.c, fieldCenterXDeg, fieldCenterYDeg, g_pAPD->getFieldWidth(), g_pAPD->getFieldHeight(), offMarker);
	length = (int)strlen(buffer);

	WaitUntil(g_pAPD->getOnTime() - waitTimer->CheckTimer() - (dFrameTime * 2), 5);
	vsgPresent();//----------------------------------------------------------------- dots, off
	waitTimer->StartTimer();
	etu::AnalogOut(0, (float)fieldCenterXDeg / g_pSettings->getDegPerVolt());
	etu::AnalogOut(1, (float)fieldCenterYDeg / g_pSettings->getDegPerVolt());
	etu::AnalogOut(2, (float)-5);
	g_pSettings->theSerialComm.transmit(buffer, length);

	// extinguish dot
	if (false) {
		vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
		iPage = 1-iPage;
		/*vsgObjDestroy(g_stim);
		vsgObjDestroy(fixation);*/
		
		vsgSetPen1(2);
		vsgDrawRect(fieldCenterX, -1 * fieldCenterY, fieldWidth, fieldHeight);
		
		WaitUntil(g_pAPD->getOffTime() - waitTimer->CheckTimer()-(dFrameTime * 2), 5);
		vsgPresent();
		waitTimer->StartTimer();
		etu::AnalogOut(0, (float)-9.95);
		etu::AnalogOut(1, (float)-9.95);
		etu::AnalogOut(2, (float)0);
	}
	WaitUntil(g_pAPD->getOffTime() - waitTimer->CheckTimer()-(dFrameTime * 2), 5);
	waitTimer->StartTimer();

	bLeft = !bLeft;

	return true;
}

void CAPDStimulus::DrawLeftField() {}

void CAPDStimulus::DrawRightField() {}

void CAPDStimulus::DrawFullScreen() {}

/***************************************************************
	 StimulusType()
	 -  Inherited method that returns the stimulus type string.
****************************************************************/

CString CAPDStimulus::StimulusType() 
{
	return "Afferent Pupil Defect Stimulus";
}

#endif // APP_EYE_TRACKING
