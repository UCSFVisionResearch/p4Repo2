/**
 * @file AlternationStimulus.cpp
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
	VSG Out 0:	periph target trial line
	VSG Out 1:	behavior count timer

	Analog Out 0:	center x
	Analog Out 1:	center y
	Analog Out 2:	peripheral x
	Analog Out 3:	peripheral y
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor     
	 -  Instantiates the newly created object
************************************************************/
CAlternationStimulus::CAlternationStimulus() 
{
	Initialize();

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	
	pointX, pointY = 0;
	iPage = 0;

	srand( (unsigned int)time(NULL) );
	
	dFrameTime = vsgGetSystemAttribute(vsgFRAMETIME)/ 1000.0;
	
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	// set the drawing mode, and set the origin to the center of the screen
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix  /2, iScrHeightPix / 2);
	vsgSetViewDistMM(g_pSettings->getViewDistance());

	// set the background color
	VSGTRIVAL bgColor;
	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;
	//init palette
	VSGLUTBUFFER lutbuffer;
	for (int i=0; i<256; i++) {
		lutbuffer[i].a=bgColor.a;
		lutbuffer[i].b=bgColor.b;
		lutbuffer[i].c=bgColor.c;
	}
	lutbuffer[0].a=((double)g_pAlternation->getRed1())/255.0f;
	lutbuffer[0].b=((double)g_pAlternation->getGreen1())/255.0f;
	lutbuffer[0].c=((double)g_pAlternation->getBlue1())/255.0f;
	lutbuffer[1].a=((double)g_pAlternation->getRed2())/255.0f;
	lutbuffer[1].b=((double)g_pAlternation->getGreen2())/255.0f;
	lutbuffer[1].c=((double)g_pAlternation->getBlue2())/255.0f;
	lutbuffer[2].a=0.0;
	lutbuffer[2].b=0.0;
	lutbuffer[2].c=0.0;
	vsgSetBackgroundColour(&lutbuffer[vsgBACKGROUND]);
	vsgSetFixationColour(&lutbuffer[vsgFIXATION]);
	vsgLUTBUFFERWrite(1,&lutbuffer);
	vsgLUTBUFFERtoPalette(1);

	g_pAlternation->sendSettings(g_pSettings->theSerialComm);
}

/*****************************************************************
	 Destructor
	 -  Cleans up when the object is deleted. The trial running
	 signal is turned off.
*****************************************************************/
CAlternationStimulus::~CAlternationStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);

	//send stop mark
	char buffer[80];
	sprintf(buffer, "Alternation;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}

/*************************************************************************
	 DisplayStimulus()
	 -  Handles the display of the stimulus. 
*************************************************************************/
bool CAlternationStimulus::DisplayStimulus() 
{
	char buffer[80];
	
	VSGTRIVAL centerColor, periphColor;

	centerColor.a = ((double)g_pAlternation->getRed1())/255.0f;
	centerColor.b = ((double)g_pAlternation->getGreen1())/255.0f;
	centerColor.c = ((double)g_pAlternation->getBlue1())/255.0f;

	periphColor.a = ((double)g_pAlternation->getRed2())/255.0f;
	periphColor.b = ((double)g_pAlternation->getGreen2())/255.0f;
	periphColor.c = ((double)g_pAlternation->getBlue2())/255.0f;

	VSGLUTBUFFER lutbuffer;
	vsgPaletteRead(&lutbuffer);
	bool change=false;//used to reduce flicker when updating palette
	if (lutbuffer[0].a != centerColor.a ||
		lutbuffer[0].b != centerColor.b ||
		lutbuffer[0].c != centerColor.c ||
		((lutbuffer[1].a != periphColor.a ||
		  lutbuffer[1].b != periphColor.b ||
		  lutbuffer[1].c != periphColor.c) && 
		 periphColor.a != 0.0 && periphColor.b != 0.0 && periphColor.c != 0.0 ))
	{
		change = change || true;
	}
	
	if (change) {
		lutbuffer[0].a=centerColor.a;
		lutbuffer[0].b=centerColor.b;
		lutbuffer[0].c=centerColor.c;
		lutbuffer[1].a=periphColor.a;
		lutbuffer[1].b=periphColor.b;
		lutbuffer[1].c=periphColor.c;
		lutbuffer[2].a=0.0;
		lutbuffer[2].b=0.0;
		lutbuffer[2].c=0.0;
		for (int i=3; i<256; i++) {
			lutbuffer[i].a=((double)g_pSettings->getRedBG()) / 255.0f;
			lutbuffer[i].b=((double)g_pSettings->getGreenBG()) / 255.0f;
			lutbuffer[i].c=((double)g_pSettings->getBlueBG()) / 255.0f;
		}

		vsgSetBackgroundColour(&lutbuffer[vsgBACKGROUND]);
		vsgSetFixationColour(&lutbuffer[vsgFIXATION]);
		vsgLUTBUFFERWrite(1,&lutbuffer);
		vsgLUTBUFFERtoPalette(1);
	}

	double centerX, centerY, periphX, periphY, centerSize, periphSize;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAlternation->getXPos1(), vsgPIXELUNIT, &centerX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAlternation->getYPos1(), vsgPIXELUNIT, &centerY);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAlternation->getXPos2(), vsgPIXELUNIT, &periphX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAlternation->getYPos2(), vsgPIXELUNIT, &periphY);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAlternation->getDotSize1(), vsgPIXELUNIT, &centerSize);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAlternation->getDotSize2(), vsgPIXELUNIT, &periphSize);
	
	// convert offsets from degrees to pixels
	double screenXOffset, screenYOffset;
	double stimXOffset, stimYOffset;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &stimXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &stimYOffset);

	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	vsgSetDrawMode(vsgCENTREXY);
	
	vsgSetPen1(0);
	
	vsgDrawOval(centerX + screenXOffset + stimXOffset, -1 * centerY + screenYOffset + stimYOffset, 2*centerSize, 2*centerSize);
	if (periphColor.a == 0.0 && periphColor.b == 0.0 && periphColor.c == 0.0) {
		vsgSetPen1(2);
	} 
	else {
		vsgSetPen1(1);
	}
	
	//vsgDrawOval(periphX + screenXOffset + stimXOffset, -1 * periphY + screenYOffset + stimYOffset, 2*periphSize, 2*periphSize);
	double separation=0;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAlternation->getDiamond(), vsgPIXELUNIT, &separation);
	vsgDrawOval(periphX + screenXOffset + stimXOffset-separation, -1 * periphY + screenYOffset + stimYOffset, 2*periphSize, 2*periphSize);
	vsgDrawOval(periphX + screenXOffset + stimXOffset, -1 * periphY + screenYOffset + stimYOffset-separation, 2*periphSize, 2*periphSize);
	vsgDrawOval(periphX + screenXOffset + stimXOffset+separation, -1 * periphY + screenYOffset + stimYOffset, 2*periphSize, 2*periphSize);
	vsgDrawOval(periphX + screenXOffset + stimXOffset, -1 * periphY + screenYOffset + stimYOffset+separation, 2*periphSize, 2*periphSize);

	// Draw
	vsgPresent();

	//reset behavior count timer
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	//reset periph target trial line
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	
	double xDeg, yDeg, x2Deg, y2Deg;
	// convert the dot location from pixels to degrees for the analog output
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, centerX + stimXOffset, vsgDEGREEUNIT, &xDeg);
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, centerY + stimYOffset, vsgDEGREEUNIT, &yDeg);
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, periphX + stimXOffset, vsgDEGREEUNIT, &x2Deg);
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, periphY + stimYOffset, vsgDEGREEUNIT, &y2Deg);
	etu::AnalogOut(0, (float)xDeg/g_pSettings->getDegPerVolt(), true);//center x
	etu::AnalogOut(1, (float)yDeg/g_pSettings->getDegPerVolt(), true);//center y
	etu::AnalogOut(2, (float)x2Deg/g_pSettings->getDegPerVolt(), true);//periph x
	etu::AnalogOut(3, (float)y2Deg/g_pSettings->getDegPerVolt(), true);//periph y

	iPage = 1-iPage;

	if (g_pAlternation->getDiamond() == 0.0) {
		sprintf(buffer, "cRGB: %.2f, %.2f, %.2f pRGB: %.2f, %.2f, %.2f| /", centerColor.a, centerColor.b, centerColor.c, periphColor.a, periphColor.b, periphColor.c);
		g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));

		WaitUntil(50);//just to avoid monopolizing the cpu
	}

	return true;
}

/***************************************************************
	 StimulusType()
	 -  Inherited method that returns the stimulus type string.
****************************************************************/
CString CAlternationStimulus::StimulusType() 
{
	return "Alternation Stimulus";
}

#endif // APP_EYE_TRACKING
