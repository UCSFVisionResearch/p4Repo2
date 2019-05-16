/**
 * @file OcularFollowingStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "OcularFollowingStimulus.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	VSG IN 3:	behavior????? Wire is disconnected????

	Analog Out 0:	Horizontal fixation target location
	Analog Out 1:	Vertical fixation target location
	Analog Out 2:	(bg) screen position x
	Analog Out 3:	(bg) screen position y
	Analog Out 4:	remote reward???? MKF4/23/09 use dig7 instead??

	Analog Out 4:	center color red=+5, blue=-5, none=0
	Analog Out 5:	bg color red=+5, blue=-5, none=0

    VSG Digital Out 1:	behavioral contingency reset 
						(vsg pin 2; 1401 pin 18; 1401 dig in 6)
	VSG Digital Out 7:	remote feed.  0=don't feed.  1=feed

	VSG Digital In 0:	Next Stim Pulse. 
						1 => Behavioral contingency has been met, move on to the 
							 excursion phase.
						0 => behvioral continengency criteria has not yet been met.

NOTES:
	AM
	The stimulus is controlled by DisplayStimulus().  There are three
	phases, represented by DoFixation(), DoStep(), and DoTimeOut().

	MKF 11/06/2007 Not sure if this actually works at all. Needs a complete test
	MKF 4/23/2009 Added checkbox to show randomized red/blue colors for dichoptic 
				  filters. Also uses analog out 4-5 now to show color
	MKF 4/28/2009 Changed digital lines to work with updated script/sequence
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

COcularFollowingStimulus::COcularFollowingStimulus() 
{
	Initialize();

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	
	pointX, pointY = 0;
	iPage = 0;
	iPhase = 0;
	selectedDirection=0;
	bDrawn = false;

	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);
	vsgIOWriteDigitalOut(0x0000, vsgDIG2);
	vsgIOWriteDigitalOut(0x0000, vsgDIG3);
	vsgIOWriteDigitalOut(0x0000, vsgDIG4);
	vsgIOWriteDigitalOut(0x0000, vsgDIG5);
	vsgIOWriteDigitalOut(0x0000, vsgDIG6);
	vsgIOWriteDigitalOut(0x0000, vsgDIG7);
	
	g_pOcularFollowing->sendSettings(g_pSettings->theSerialComm);
	
	srand( (unsigned int)time(NULL) );
}

COcularFollowingStimulus::~COcularFollowingStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	
	//send stop mark
	char buffer[80];
	sprintf(buffer, "OcularFollowing;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));

	//reset palette
	VSGLUTBUFFER lutbuf;
	for (int i=0; i<256; i++) {
		lutbuf[i].a =((double)0) / 255.f;
		lutbuf[i].b =((double)0) / 255.0f;
		lutbuf[i].c =((double)0) / 255.0f;
	}	
	vsgSetBackgroundColour(&lutbuf[vsgBACKGROUND]);
	vsgSetFixationColour(&lutbuf[vsgFIXATION]);
	vsgLUTBUFFERWrite(0, &lutbuf);
	vsgLUTBUFFERtoPalette(0);
}

bool COcularFollowingStimulus::Initialize() 
{
	bEndThread=false;
	VSGTRIVAL bgColor;

	blackColor.a = 0;
	blackColor.b = 0;
	blackColor.c = 0;
	
	dotColor.a = ((double)g_pSettings->getRed() * ((double)g_pFullfield->getContrast()/100.0f)) / 255.0f;
	dotColor.b = ((double)g_pSettings->getGreen()  * ((double)g_pFullfield->getContrast()/100.0f)) / 255.0f;
	dotColor.c = ((double)g_pSettings->getBlue() * ((double)g_pFullfield->getContrast() / 100.0f)) / 255.0f;
	
	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	int CheckCard;
 	
	//Initialise the vsg card then check that it initialised O.K.
	if (!g_bIsInitialized) {
		CheckCard = vsgInit("");
		if (CheckCard < 0) return false;
		g_bIsInitialized = true;
		CheckCard=vsgSetVideoMode(vsgPANSCROLLMODE);
	}

	vsgMoveScreen(0,0);
	
	vsgSetCommand(vsgPALETTECLEAR + vsgVIDEODRIFT);// + vsgOVERLAYDRIFT

	double dFrameTime = vsgGetSystemAttribute(vsgFRAMETIME);
	
	g_dFramerate = 1000000.0 / dFrameTime;
	g_dHalfFrameInterval = DWORD(floor(dFrameTime / 2000.0f));
	g_dSleepThreshold = (double)g_dHalfFrameInterval / 2.0f;
	g_dFramerate = 1000000.0f / vsgGetSystemAttribute(vsgFRAMETIME);

	// set the view distance
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);	

	vsgSetBackgroundColour(&bgColor);

	//set up the color palette
	VSGLUTBUFFER lutbuf;
	for (int i=0; i<201; i++) {
		//lutbuf[i].a = (float)i/200.0;	lutbuf[i].b = (float)i/200.0;	lutbuf[i].c = (float)i/200.0;
		
		lutbuf[i].a =((double)i) / 200.0f;
		lutbuf[i].b =((double)i) / 200.0f;
		lutbuf[i].c =((double)i) / 200.0f;
	}
	for (int i=201; i<256; i++) {
		lutbuf[i].a = bgColor.a;	lutbuf[i].b = bgColor.b;	lutbuf[i].c = bgColor.c;
	}
	lutbuf[250].a = dotColor.a;		lutbuf[250].b = dotColor.b;	lutbuf[250].c = dotColor.c;
	vsgSetBackgroundColour(&lutbuf[vsgBACKGROUND]);
	vsgSetFixationColour(&lutbuf[vsgFIXATION]);
	vsgLUTBUFFERWrite(0, &lutbuf);
	vsgLUTBUFFERtoPalette(0);

	double degPerFrame = g_pOcularFollowing->getBackgroundSpeed() / g_dFramerate;
	dVoltageStep = 1/(g_pSettings->getDegPerVolt() / degPerFrame);
    g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, degPerFrame, vsgPIXELUNIT, &dPixPerFrame);

	bShowMotion = false;

	//clear all 3 pages
	vsgSetDrawPage(vsgVIDEOPAGE, 2, vsgBACKGROUND);	
	vsgSetDrawPage(vsgVIDEOPAGE, 1, vsgBACKGROUND);	
	vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgBACKGROUND);	
	selectedDirection=0;
	return true;
}

bool COcularFollowingStimulus::DisplayStimulus() 
{
	switch (iPhase) {
		case 0: DoFixation(); break;
		case 1: DoStep(selectedDirection); break;
		case 2: DoTimeOut(); break;
	}
	return true;
}

void COcularFollowingStimulus::DoFixation() 
{
	int PAGE_NOISE = 0;
	int PAGE_PERIPHDOT = 1;
	int PAGE_CENTERDOT = 2;
	VSGTRIVAL whiteColor;
	whiteColor.a = 1;
	whiteColor.b = 1;
	whiteColor.c = 1;

	double xPosPix, yPosPix;
	bool bRandomize = g_pOcularFollowing->getRandom();//checkbox

	if (!bDrawn) {
		bool bRedFixation = (rand()%2) == 0;
		if (bRandomize) {
			dotColor.a = (double)((bRedFixation)? g_pScotomaMap->getPrimaryRed() : g_pScotomaMap->getSecondaryRed()) / 255.0f;
			dotColor.b = (double)((bRedFixation)? g_pScotomaMap->getPrimaryGreen() : g_pScotomaMap->getSecondaryGreen()) / 255.0f;
			dotColor.c = (double)((bRedFixation)? g_pScotomaMap->getPrimaryBlue() : g_pScotomaMap->getSecondaryBlue()) / 255.0f;

			etu::AnalogOut(4, (float)((bRedFixation)? 5 : -5), true);//center color red 5 blue -5
		} 
		else {
			dotColor.a = ((double)g_pSettings->getRed()) / 255.0f;
			dotColor.b = ((double)g_pSettings->getGreen()) / 255.0f;
			dotColor.c = ((double)g_pSettings->getBlue()) / 255.0f;
			etu::AnalogOut(4, 0.0f, true);//no center color
		}

		VSGTRIVAL bgColor;
		bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
		bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
		bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

		// reset the screen origins
		pointX = 0;
		pointY = 0;
		
		// set the view distance
		vsgSetViewDistMM(g_pSettings->getViewDistance());
		vsgSetSpatialUnits(vsgPIXELUNIT);	

		vsgSetBackgroundColour(&bgColor);

		VSGLUTBUFFER lutbuf;
		bool bRedBG = (rand()%2) == 0;
		if (bRandomize) {
			etu::AnalogOut(5, (float)((bRedBG)? 5 : -5), true);//bg color red 5 blue -5
		} 
		else {
			etu::AnalogOut(5, 0.0f, true);//now bg color
		}
		
		//uniform white noise
		for (int i=0; i<101; i++) {
			lutbuf[i].a = bgColor.a;	lutbuf[i].b = bgColor.b;	lutbuf[i].c = bgColor.c;
		}
		for (int i=101; i<201; i++) {
			if (bRandomize) {
				//gradient
				//lutbuf[i].a = ((float)i/100.0) * (float)((bRedBG)? g_pScotomaMap->getPrimaryRed() : g_pScotomaMap->getSecondaryRed()) / 255.0f;
				//lutbuf[i].b = ((float)i/100.0) * (float)((bRedBG)? g_pScotomaMap->getPrimaryGreen() : g_pScotomaMap->getSecondaryGreen()) / 255.0f;
				//lutbuf[i].c = ((float)i/100.0) * (float)((bRedBG)? g_pScotomaMap->getPrimaryBlue() : g_pScotomaMap->getSecondaryBlue()) / 255.0f;
				//full on
				//lutbuf[i].a = (float)((bRedBG)? g_pScotomaMap->getPrimaryRed() : g_pScotomaMap->getSecondaryRed()) / 255.0f;
				//lutbuf[i].b = (float)((bRedBG)? g_pScotomaMap->getPrimaryGreen() : g_pScotomaMap->getSecondaryGreen()) / 255.0f;
				//lutbuf[i].c = (float)((bRedBG)? g_pScotomaMap->getPrimaryBlue() : g_pScotomaMap->getSecondaryBlue()) / 255.0f;
				lutbuf[i].a = ((float)g_pOcularFollowing->getNoiseContrast()/100.0) * (float)((bRedBG)? g_pScotomaMap->getPrimaryRed() : g_pScotomaMap->getSecondaryRed()) / 255.0f;
				lutbuf[i].b = ((float)g_pOcularFollowing->getNoiseContrast()/100.0) * (float)((bRedBG)? g_pScotomaMap->getPrimaryGreen() : g_pScotomaMap->getSecondaryGreen()) / 255.0f;
				lutbuf[i].c = ((float)g_pOcularFollowing->getNoiseContrast()/100.0) * (float)((bRedBG)? g_pScotomaMap->getPrimaryBlue() : g_pScotomaMap->getSecondaryBlue()) / 255.0f;
			} 
			else {
				lutbuf[i].a = (float)i/100.0;	lutbuf[i].b = (float)i/100.0;	lutbuf[i].c = (float)i/100.0;
			}
		}
		for (int i=201; i<256; i++) {
			lutbuf[i].a = bgColor.a;	lutbuf[i].b = bgColor.b;	lutbuf[i].c = bgColor.c;
		}
		lutbuf[250].a = dotColor.a;		lutbuf[250].b = dotColor.b;	lutbuf[250].c = dotColor.c;		
		vsgSetBackgroundColour(&lutbuf[vsgBACKGROUND]);
		vsgSetFixationColour(&lutbuf[vsgFIXATION]);
		vsgLUTBUFFERWrite(0, &lutbuf);
		vsgLUTBUFFERtoPalette(0);

		//trial description
		selectedDirection = g_pOcularFollowing->getDirection();//randomly pick direction now
		char buffer[80];
		int trialcode = 0;
		if (bRandomize) {
			if (bRedFixation) trialcode += 1;
			if (bRedBG) trialcode += 2;
		} 
		else {
			trialcode += 16;
		}
		if (selectedDirection == COcularFollowing::DIR_UP) trialcode += 0;
		if (selectedDirection == COcularFollowing::DIR_DOWN) trialcode += 4;
		if (selectedDirection == COcularFollowing::DIR_LEFT) trialcode += 8;
		if (selectedDirection == COcularFollowing::DIR_RIGHT) trialcode += 12;
		sprintf(buffer, "Start Trial Now|0x%02x/\0", trialcode);
		g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
		
		// convert the degrees to pixels
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pOcularFollowing->getSize(), vsgPIXELUNIT, &dSize);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &xOffset);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &yOffset);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);
		float angle = 2.0 * 3.1415926 * (float)(rand()%10000) * 0.0001;
		float radius = g_pOcularFollowing->getMinRadius() + (g_pOcularFollowing->getMaxRadius()-g_pOcularFollowing->getMinRadius()) * (float)(rand()%10000) * 0.0001;
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, radius*cos(angle), vsgPIXELUNIT, &xPosPix);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, radius*sin(angle), vsgPIXELUNIT, &yPosPix);
		//should draw point in a random position here

		// invert yPosition
		yPosPix *= -1;

		vsgSetDrawPage(vsgVIDEOPAGE, PAGE_NOISE, vsgBACKGROUND);
		vsgSetDrawMode(vsgCENTREXY);

		vsgMoveScreen(0, 768);//0,0

		//Sets the size in pixels of the noise.
		noisesize=g_pOcularFollowing->getNoiseSize();//16;

		//Allows screen heights that aren't multiples of the noise size.
		rndHeight= ((iScrHeightPix+noisesize-1) / noisesize) * noisesize;

		vsgSetDrawMode(0);
		vsgSetDrawOrigin(0, 0);

		//Fills the video memory with noise.
		
		//draw a screenful of gaussian noise
		//uniform noise
		vsgDrawNoise(0,0,iScrWidthPix,rndHeight,noisesize,noisesize,0,g_pOcularFollowing->getNoisePct(), 100);//100
		//duplicate noise on iPage, first horizontally
		vsgDrawMoveRect(vsgVIDEOPAGE,PAGE_NOISE,//from videopage 0
			0,0,iScrWidthPix,rndHeight,//src x,y,w,h
			iScrWidthPix,0,iScrWidthPix,rndHeight);//dest x,y,w,h
		//now duplicate that again vertically (filling 4 quadrants with the 1st original)
		vsgDrawMoveRect(vsgVIDEOPAGE,PAGE_NOISE,
			0,0, 2048, 1024-rndHeight,
			0, rndHeight, 2048, 1024-rndHeight);   //  0, rndHeight+1, 2048, 1024-rndHeight);
		//change to periphdot page (and clear it)
		vsgSetDrawPage(vsgVIDEOPAGE, PAGE_PERIPHDOT, vsgBACKGROUND);
		//now copy huge noise pattern to the other page so we can draw on this one
		vsgDrawMoveRect(vsgVIDEOPAGE,PAGE_NOISE,
		//	0,0,2048, 1024, 0, 0, 2048, 1024);
			0,0,2048, 768*2, 0, 0, 2048, 768*2);
		//change to center dot page (and clear it)
		vsgSetDrawPage(vsgVIDEOPAGE, PAGE_CENTERDOT, vsgBACKGROUND);
		//now copy huge noise pattern to the other page so we can draw on this one
		int result= vsgDrawMoveRect(vsgVIDEOPAGE,PAGE_NOISE,
		//	0,0,2048, 1024, 0, 0, 2048, 1024);
			0,0,2048, 768*2, 0, 0, 2048, 768*2);
		//finally draw peripheral dot
		vsgSetDrawPage(vsgVIDEOPAGE, PAGE_PERIPHDOT, vsgNOCLEAR);
		vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix /2 +768);//+768
		vsgSetDrawMode(vsgCENTREXY);
		vsgSetPen1(250);
		vsgDrawOval(xPosPix + xOffset + screenXOffset, yPosPix + yOffset + screenYOffset, dSize, dSize);
		
		vsgPresent();
		bDrawn = true;
		
		// wait 1 milli seconds to all for UI threads to process
		etu::AnalogOut(0, (radius*(double)cos(angle) + g_pSettings->getXOffset()) / g_pSettings->getDegPerVolt());
		etu::AnalogOut(1, (radius*(double)sin(angle) + g_pSettings->getYOffset()) / g_pSettings->getDegPerVolt());
		etu::AnalogOut(2, 0);
		etu::AnalogOut(3, 0);
	}

	//behaviorally contingent		
	// sleep in half frame intervals until the desired number of msecs have passed
	//wait until the eye is on target
	while ((g_pOcularFollowing->getBehavior()) && ((vsgIOReadDigitalIn() & vsgDIG0) <= 0) && (!bEndThread)) {//vsgDIG3
		WaitUntil(10, 5);
	}//end waituntil

	//here, if they have met the behavioral contingency on the peripheral dot, draw a new dot at center
	//if they meet the behavioral contingency there as well, move on to the next phase

	// behvioral contingency counter should be reset

	iPage = 1-iPage;
	vsgSetDrawPage(vsgVIDEOPAGE, PAGE_CENTERDOT, vsgNOCLEAR);//PAGE_NOISE//vsgBACKGROUND
	
	vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix /2 +768);//+768
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetPen1(250);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pOcularFollowing->getXPosition(), vsgPIXELUNIT, &xPosPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pOcularFollowing->getYPosition(), vsgPIXELUNIT, &yPosPix);
	
	vsgPresent();

	etu::AnalogOut(0, g_pOcularFollowing->getXPosition() / g_pSettings->getDegPerVolt());
	etu::AnalogOut(1, g_pOcularFollowing->getYPosition() / g_pSettings->getDegPerVolt());
	
	//saccade is done, wait before moving

	//behaviorally contingent		
	// sleep in half frame intervals until the desired number of msecs have passed
	//wait until the eye is off target
	while ((g_pOcularFollowing->getBehavior()) && ((vsgIOReadDigitalIn() & vsgDIG0) >= 1) && (!bEndThread)) {//pin 17//vsgDIG3
		WaitUntil(10, 5);
	}
	
	WaitUntil(g_pOcularFollowing->getLatency());//+2000
	
	//set target location to nowhere
	etu::AnalogOut(0, (float)-9.95, true);//x
	etu::AnalogOut(1, (float)-9.95, true);//y

	bDrawn = false;
	iPhase = 1;

	DoStep(selectedDirection);
}

void COcularFollowingStimulus::DoStep(int selectedDirection) 
{
	switch (selectedDirection) {
		case COcularFollowing::DIR_UP: xVoltageStep = 0; yVoltageStep = 1; break;
		case COcularFollowing::DIR_DOWN: xVoltageStep = 0; yVoltageStep = -1; break;
		case COcularFollowing::DIR_LEFT: xVoltageStep = 1; yVoltageStep = 0; break;
		case COcularFollowing::DIR_RIGHT: xVoltageStep = -1; yVoltageStep = 0; break;
	}
	double startX, startY, startXVolts, startYVolts;
	
	vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgNOCLEAR);//iPage
	vsgPresent();
	
	startX = 0;//1024
	startY = 768;//768
	startXVolts = 0;
	startYVolts = 0;
	
	int numFrames = (int)(g_pOcularFollowing->getBackgroundDuration() * g_dFramerate / 1000);
	
	for (int i = 0; i < numFrames; i++) {
		vsgMoveScreen(startX, startY);

		etu::AnalogOut(2, (float)-startXVolts);//was a bug, moving wrong way through 5/13/09
		etu::AnalogOut(3, (float)startYVolts);
		
		startX = startX + (dPixPerFrame * xVoltageStep);
		startY = startY + (dPixPerFrame * yVoltageStep);
		startXVolts += (dVoltageStep * xVoltageStep);
		startYVolts += (dVoltageStep * yVoltageStep);

		//???? never gets here anyway...
		/*if (startX == 2048 - iScrWidthPix || startY == 2*768-iScrHeightPix) {//== 1024-iScrHeightPix//<0
			startX = 0;
			startY = 768;//0
			startXVolts = 0;
			startYVolts = 0;
		}*/
		Sleep(1);
	}
	
	iPhase = 2;
	DoTimeOut();
}

void COcularFollowingStimulus::DoTimeOut() 
{
	etu::CTimer blankTimer;
	
	vsgSetDrawPage(vsgVIDEOPAGE, 3, vsgBACKGROUND);//1

	vsgPresent();
	blankTimer.StartTimer();

	etu::AnalogOut(2, 0);
	etu::AnalogOut(3, 0);
	
	if (g_pOcularFollowing->getBehavior()) {
		//send remote reward
		vsgIOWriteDigitalOut(0xFFFF, vsgDIG7);
		WaitUntil(200);
		vsgIOWriteDigitalOut(0x0000, vsgDIG7);
	}

	while ( (blankTimer.CheckTimer() < g_pOcularFollowing->getTimeOut()) && (!bEndThread)) {
		WaitUntil(10, 5);
	}
	
	iPhase = 0;
}

CString COcularFollowingStimulus::StimulusType() 
{
	return "Ocular Following Stimulus";
}

#endif // APP_EYE_TRACKING
