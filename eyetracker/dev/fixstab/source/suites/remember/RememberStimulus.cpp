/**
 * @file RememberStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "RememberStimulus.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	VSG Out 0:	Trial Running pulse. 1 => trial is running. 
				0 => trial is not running
	VSG Out 1:	behavior count timer

	VSG IN 0:	Next Stim Pulse. 1 => Behavioral contingency
				has been met, move on to the excursion phase.
				0 => behvioral continengency criteria has not 
				yet been met.

	Analog Out 0:	Horizontal center target location
	Analog Out 1:	Vertical center target location
	Analog Out 2:	Horizontal peripheral target location
	Analog Out 3:	Vertical peripheral target location
	Analog Out 4:	target color red=+5, blue=-5
	Analog Out 5:	flash color red=+5, blue=-5, purple=0, blank=-10
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor     
	 -  Instantiates the newly created object
************************************************************/
CRememberStimulus::CRememberStimulus() 
{
	Initialize();

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	
	pointX = 0;
	pointY = 0;
	periphX = 0;
	periphY = 0;
	iPage = 0;
	iOn = 1;
	bOn = true;
	bNoiseOn = true;
	iOldLocIndex=-1;
	iSeqLocIndex=0;
	FlashTimer.StartTimer();
	g_pRememberDlg->response=-1;
	prevTextCode=-1;

	iTotalCounter=-1;

	srand( (unsigned int)time(NULL) );
	
	dFrameTime = vsgGetSystemAttribute(vsgFRAMETIME)/ 1000.0;
	
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	// set the drawing mode, and set the origin to the center of the screen
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix  /2, iScrHeightPix / 2);

	for (int i=0; i<256; i++) {
		lutbuf[i].a = 0;
		lutbuf[i].b = 0;
		lutbuf[i].c = 0;
	}

	g_pRemember->sendSettings(g_pSettings->theSerialComm);

	if (g_pRemember->getResponse()) {
		int stephsize = 1+2*g_pRemember->getNumStepsH();
		int stepvsize = 1+2*g_pRemember->getNumStepsV();
		if (g_pRememberDlg->le_r != NULL) free(g_pRememberDlg->le_r);
		g_pRememberDlg->le_r = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->le_b != NULL) free(g_pRememberDlg->le_b);
		g_pRememberDlg->le_b = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->le_o != NULL) free(g_pRememberDlg->le_o);
		g_pRememberDlg->le_o = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->re_r != NULL) free(g_pRememberDlg->re_r);
		g_pRememberDlg->re_r = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->re_b != NULL) free(g_pRememberDlg->re_b);
		g_pRememberDlg->re_b = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->re_o != NULL) free(g_pRememberDlg->re_o);
		g_pRememberDlg->re_o = (int*)malloc(sizeof(int)*stephsize*stepvsize);

		if (g_pRememberDlg->re_red_r != NULL) free(g_pRememberDlg->re_red_r);
		g_pRememberDlg->re_red_r = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->re_red_b != NULL) free(g_pRememberDlg->re_red_b);
		g_pRememberDlg->re_red_b = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->re_red_o != NULL) free(g_pRememberDlg->re_red_o);
		g_pRememberDlg->re_red_o = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->re_blue_r != NULL) free(g_pRememberDlg->re_blue_r);
		g_pRememberDlg->re_blue_r = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->re_blue_b != NULL) free(g_pRememberDlg->re_blue_b);
		g_pRememberDlg->re_blue_b = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->re_blue_o != NULL) free(g_pRememberDlg->re_blue_o);
		g_pRememberDlg->re_blue_o = (int*)malloc(sizeof(int)*stephsize*stepvsize);

		if (g_pRememberDlg->le_red_r != NULL) free(g_pRememberDlg->le_red_r);
		g_pRememberDlg->le_red_r = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->le_red_b != NULL) free(g_pRememberDlg->le_red_b);
		g_pRememberDlg->le_red_b = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->le_red_o != NULL) free(g_pRememberDlg->le_red_o);
		g_pRememberDlg->le_red_o = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->le_blue_r != NULL) free(g_pRememberDlg->le_blue_r);
		g_pRememberDlg->le_blue_r = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->le_blue_b != NULL) free(g_pRememberDlg->le_blue_b);
		g_pRememberDlg->le_blue_b = (int*)malloc(sizeof(int)*stephsize*stepvsize);
		if (g_pRememberDlg->le_blue_o != NULL) free(g_pRememberDlg->le_blue_o);
		g_pRememberDlg->le_blue_o = (int*)malloc(sizeof(int)*stephsize*stepvsize);

		for (int i=0; i<stephsize; i++) {
			for (int j=0; j<stepvsize; j++) {
				g_pRememberDlg->le_r[i*stepvsize+j] = 0;
				g_pRememberDlg->le_b[i*stepvsize+j] = 0;
				g_pRememberDlg->le_o[i*stepvsize+j] = 0;
				g_pRememberDlg->re_r[i*stepvsize+j] = 0;
				g_pRememberDlg->re_b[i*stepvsize+j] = 0;
				g_pRememberDlg->re_o[i*stepvsize+j] = 0;
				//
				g_pRememberDlg->re_red_r[i*stepvsize+j] = 0;
				g_pRememberDlg->re_red_b[i*stepvsize+j] = 0;
				g_pRememberDlg->re_red_o[i*stepvsize+j] = 0;
				g_pRememberDlg->re_blue_r[i*stepvsize+j] = 0;
				g_pRememberDlg->re_blue_b[i*stepvsize+j] = 0;
				g_pRememberDlg->re_blue_o[i*stepvsize+j] = 0;
				//
				g_pRememberDlg->le_red_r[i*stepvsize+j] = 0;
				g_pRememberDlg->le_red_b[i*stepvsize+j] = 0;
				g_pRememberDlg->le_red_o[i*stepvsize+j] = 0;
				g_pRememberDlg->le_blue_r[i*stepvsize+j] = 0;
				g_pRememberDlg->le_blue_b[i*stepvsize+j] = 0;
				g_pRememberDlg->le_blue_o[i*stepvsize+j] = 0;
			}
		}
		g_pRememberDlg->catch_r_total = 0;
		g_pRememberDlg->catch_b_total = 0;
		g_pRememberDlg->catch_n_total = 0;
		g_pRememberDlg->catch_r= 0;
		g_pRememberDlg->catch_b= 0;
		g_pRememberDlg->catch_n= 0;
	}
}

/*****************************************************************
	 Destructor
	 -  Cleans up when the object is deleted. The trial running
	 signal is turned off.
*****************************************************************/
CRememberStimulus::~CRememberStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	
	//send stop mark
	char buffer[80];
	sprintf(buffer, "Scotoma Mapping;Stop;%ld;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}

/*************************************************************************
	 DisplayStimulus()
	 -  Handles the display of the stimulus. 
*************************************************************************/
bool CRememberStimulus::DisplayStimulus() 
{
	char buffer[80];

	if (g_pRemember->getInterleaveNoise()) {
		bNoiseOn = !bNoiseOn;
	} 
	else {
		bNoiseOn = true;
	}

	while (g_pRememberDlg->m_Pause.GetCheck()==BST_CHECKED) {
		g_pEyeTrackingDlg->EyePosSample();
		WaitUntil(50);
	}
	/*
	text label coding:
	code	center	periph
	0x0		pri		pri
	0x1		pri		sec
	0x2		sec		pri
	0x3		sec		sec

	0x4		pri		ter
	0x5		sec		ter

	0x6		pri		blank
	0x7		sec		blank
	*/
	int textCode=0;
	/* -- primary=red -- secondary=blue -- */
	/* --                tertiary=purple -- */

	bool bReset = false;
	
	VSGTRIVAL centerColor, periphColor, centerDimColor;
	if (g_pRemember->useSeqRandom()) {
		textCode = g_pRemember->getLocationType(iSeqLocIndex);
		if (textCode<2 || textCode==4 || textCode==6) {
			centerColor.a = ((double)g_pRemember->getCenterPrimaryRed())/255.0f;
			centerColor.b = ((double)g_pRemember->getCenterPrimaryGreen())/255.0f;
			centerColor.c = ((double)g_pRemember->getCenterPrimaryBlue())/255.0f;
		} 
		else {
			centerColor.a = ((double)g_pRemember->getCenterSecondaryRed())/255.0f;
			centerColor.b = ((double)g_pRemember->getCenterSecondaryGreen())/255.0f;
			centerColor.c = ((double)g_pRemember->getCenterSecondaryBlue())/255.0f;
		}
		if (textCode==0 || textCode==2) {
			periphColor.a = ((double)g_pRemember->getPeriphPrimaryRed())/255.0f;
			periphColor.b = ((double)g_pRemember->getPeriphPrimaryGreen())/255.0f;
			periphColor.c = ((double)g_pRemember->getPeriphPrimaryBlue())/255.0f;
		} 
		else if (textCode==1 || textCode==3) {
			periphColor.a = ((double)g_pRemember->getPeriphSecondaryRed())/255.0f;
			periphColor.b = ((double)g_pRemember->getPeriphSecondaryGreen())/255.0f;
			periphColor.c = ((double)g_pRemember->getPeriphSecondaryBlue())/255.0f;
		} 
		else {
			periphColor.a = ((double)g_pRemember->getPeriphTertiaryRed())/255.0f;
			periphColor.b = ((double)g_pRemember->getPeriphTertiaryGreen())/255.0f;
			periphColor.c = ((double)g_pRemember->getPeriphTertiaryBlue())/255.0f;
		}
	} 
	else {
		switch(g_pRemember->getCenterColorRadio()) {
			case CRemember::COLOR_PRIMARY:
				centerColor.a = ((double)g_pRemember->getCenterPrimaryRed())/255.0f;
				centerColor.b = ((double)g_pRemember->getCenterPrimaryGreen())/255.0f;
				centerColor.c = ((double)g_pRemember->getCenterPrimaryBlue())/255.0f;
				break;
			case CRemember::COLOR_SECONDARY:
				centerColor.a = ((double)g_pRemember->getCenterSecondaryRed())/255.0f;
				centerColor.b = ((double)g_pRemember->getCenterSecondaryGreen())/255.0f;
				centerColor.c = ((double)g_pRemember->getCenterSecondaryBlue())/255.0f;
				textCode+=2;
				break;
			case CRemember::COLOR_ALTERNATE:
				if (iOn>0) {
					centerColor.a = ((double)g_pRemember->getCenterPrimaryRed())/255.0f;
					centerColor.b = ((double)g_pRemember->getCenterPrimaryGreen())/255.0f;
					centerColor.c = ((double)g_pRemember->getCenterPrimaryBlue())/255.0f;
				} 
				else {
					centerColor.a = ((double)g_pRemember->getCenterSecondaryRed())/255.0f;
					centerColor.b = ((double)g_pRemember->getCenterSecondaryGreen())/255.0f;
					centerColor.c = ((double)g_pRemember->getCenterSecondaryBlue())/255.0f;
					textCode+=2;
				}
				break;
			default://CRemember::COLOR_RANDOM
				if (rand()%2==0) {
					centerColor.a = ((double)g_pRemember->getCenterPrimaryRed())/255.0f;
					centerColor.b = ((double)g_pRemember->getCenterPrimaryGreen())/255.0f;
					centerColor.c = ((double)g_pRemember->getCenterPrimaryBlue())/255.0f;
				} 
				else {
					centerColor.a = ((double)g_pRemember->getCenterSecondaryRed())/255.0f;
					centerColor.b = ((double)g_pRemember->getCenterSecondaryGreen())/255.0f;
					centerColor.c = ((double)g_pRemember->getCenterSecondaryBlue())/255.0f;
					textCode+=2;
				}
				break;
		}
		switch(g_pRemember->getPeriphColorRadio()) {
			case CRemember::COLOR_PRIMARY:
				periphColor.a = ((double)g_pRemember->getPeriphPrimaryRed())/255.0f;
				periphColor.b = ((double)g_pRemember->getPeriphPrimaryGreen())/255.0f;
				periphColor.c = ((double)g_pRemember->getPeriphPrimaryBlue())/255.0f;
				break;
			case CRemember::COLOR_SECONDARY:
				periphColor.a = ((double)g_pRemember->getPeriphSecondaryRed())/255.0f;
				periphColor.b = ((double)g_pRemember->getPeriphSecondaryGreen())/255.0f;
				periphColor.c = ((double)g_pRemember->getPeriphSecondaryBlue())/255.0f;
				textCode+=1;
				break;
			case CRemember::COLOR_TERTIARY:
				periphColor.a = ((double)g_pRemember->getPeriphTertiaryRed())/255.0f;
				periphColor.b = ((double)g_pRemember->getPeriphTertiaryGreen())/255.0f;
				periphColor.c = ((double)g_pRemember->getPeriphTertiaryBlue())/255.0f;
				//textCode+=1;//????????????????????????
				break;
			case CRemember::COLOR_ALTERNATE:
				if (bOn) {
					periphColor.a = ((double)g_pRemember->getPeriphPrimaryRed())/255.0f;
					periphColor.b = ((double)g_pRemember->getPeriphPrimaryGreen())/255.0f;
					periphColor.c = ((double)g_pRemember->getPeriphPrimaryBlue())/255.0f;
				} 
				else {
					periphColor.a = ((double)g_pRemember->getPeriphSecondaryRed())/255.0f;
					periphColor.b = ((double)g_pRemember->getPeriphSecondaryGreen())/255.0f;
					periphColor.c = ((double)g_pRemember->getPeriphSecondaryBlue())/255.0f;
					textCode+=1;
				}
				break;
			default://CRemember::COLOR_RANDOM
				if (rand()%2==0) {
					periphColor.a = ((double)g_pRemember->getPeriphPrimaryRed())/255.0f;
					periphColor.b = ((double)g_pRemember->getPeriphPrimaryGreen())/255.0f;
					periphColor.c = ((double)g_pRemember->getPeriphPrimaryBlue())/255.0f;
				} 
				else {
					periphColor.a = ((double)g_pRemember->getPeriphSecondaryRed())/255.0f;
					periphColor.b = ((double)g_pRemember->getPeriphSecondaryGreen())/255.0f;
					periphColor.c = ((double)g_pRemember->getPeriphSecondaryBlue())/255.0f;
					textCode+=1;
				}
				break;
		}
	}
	
	// set the background color
	VSGTRIVAL bgColor;
	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	vsgSetBackgroundColour(&bgColor);

	//set the noise color
	VSGTRIVAL noiseColor;
	noiseColor.a = ((double)g_pRemember->getNoiseRed()) / 255.0f;
	noiseColor.b = ((double)g_pRemember->getNoiseGreen()) / 255.0f;
	noiseColor.c = ((double)g_pRemember->getNoiseBlue()) / 255.0f;

	//set the dim center color
	centerDimColor.a = (centerColor.a)*((double)g_pRemember->getDimBrightness()/100.0);
	centerDimColor.b = (centerColor.b)*((double)g_pRemember->getDimBrightness()/100.0);
	centerDimColor.c = (centerColor.c)*((double)g_pRemember->getDimBrightness()/100.0);

	// set the view distance
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);

	// convert offsets from degrees to pixels
	double centerX, centerY, centerSize, periphSize;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRemember->getCenterXPos(), vsgPIXELUNIT, &centerX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRemember->getCenterYPos(), vsgPIXELUNIT, &centerY);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRemember->getCenterDotSize(), vsgPIXELUNIT, &centerSize);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRemember->getPeriphDotSize(), vsgPIXELUNIT, &periphSize);
	
	double screenXOffset, screenYOffset;
	double stimXOffset, stimYOffset;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &stimXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &stimYOffset);
	
	//update the trial counter
	if (g_pRemember->useSeqRandom()) {
		if (iSeqLocIndex==0) {//eliminate repeatable randomness
			g_pRemember->GenerateLocations2();
			//increase total by one layer
			iTotalCounter++;
		}
		if (iSeqLocIndex >= g_pRemember->getNumLocations()) {
			//reset counter if random list has been regenerated
			iSeqLocIndex=0;
		}
		sprintf(buffer, "Counter: %d/%d\0", (iSeqLocIndex+1), g_pRemember->getNumLocations() );
	} 
	else {
		sprintf(buffer, "Counter\0" );
	}
	g_pRememberDlg->m_Countdown.SetWindowText(buffer);
	sprintf(buffer, "Total Layers: %.2f\0", ((double)(iSeqLocIndex+iTotalCounter*g_pRemember->getNumLocations()))/((double)g_pRemember->getNumLocations())   );
	g_pRememberDlg->m_Countdown2.SetWindowText(buffer);

	// wait for the fixation delay / behvioral contingency to be met
	// and then display the on period
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	vsgSetDrawMode(vsgCENTREXY);

	if (g_pRemember->getUseNoise() && bNoiseOn) {
		vsgDrawNoise(0,0,iScrWidthPix, iScrHeightPix, g_pRemember->getNoiseSize(),g_pRemember->getNoiseSize(),0,2+g_pRemember->getNoisePct(),100);
	}
	
	//draw center point (only)
	vsgSetPen1(0);
	vsgDrawRect(centerX + screenXOffset + stimXOffset,
		-1 * centerY + screenYOffset + stimYOffset,
		centerSize,
		g_pRemember->getCrossPix());
	vsgDrawRect(centerX + screenXOffset + stimXOffset,
		-1 * centerY + screenYOffset + stimYOffset,
		g_pRemember->getCrossPix(),
		centerSize);
	
	// choose a delay time based on the stimulus that is being displayed
	int iDelay;
	if (g_pRemember->getStimType() == CRemember::STIM_MEMORY_GUIDED) {
		iDelay = g_pRemember->getFlashInterval();
		WaitUntil(iDelay - FlashTimer.CheckTimer() -32, 5);//2*16ms frame drawing delay
	} 
	else {
		if (g_pRemember->getUseDim()) {
			//wait until they answer for the last trial
			WaitUntil(g_pRemember->getAnswerPeriod() - FlashTimer.CheckTimer() -32, 5);//2*16ms frame drawing delay
		} 
		else {
			//wait until the dot is finished being drawn
			iDelay = g_pRemember->getFlashDuration();
			WaitUntil(iDelay - FlashTimer.CheckTimer() -32, 5);//2*16ms frame drawing delay
		}
	}

	//set up VSGLUTBUFFER before the first screen draw of this "trial"
	lutbuf[0].a = centerColor.a;	lutbuf[0].b = centerColor.b;	lutbuf[0].c = centerColor.c;
	lutbuf[1].a = periphColor.a;	lutbuf[1].b = periphColor.b;	lutbuf[1].c = periphColor.c;
	for (int i=2; i<102; i++) {
		lutbuf[i].a = bgColor.a;	lutbuf[i].b = bgColor.b;	lutbuf[i].c = bgColor.c;
	}
	for (int i=102; i<202; i++) {
		lutbuf[i].a = noiseColor.a;	lutbuf[i].b = noiseColor.b;	lutbuf[i].c = noiseColor.c;
	}
	lutbuf[202].a = centerDimColor.a;	lutbuf[202].b = centerDimColor.b;	lutbuf[202].c = centerDimColor.c;
	//save all periph colors
	lutbuf[203].a = ((double)g_pRemember->getPeriphPrimaryRed())/255.0f;//primary
	lutbuf[203].b = ((double)g_pRemember->getPeriphPrimaryGreen())/255.0f;
	lutbuf[203].c = ((double)g_pRemember->getPeriphPrimaryBlue())/255.0f;
	lutbuf[204].a = ((double)g_pRemember->getPeriphSecondaryRed())/255.0f;//secondary
	lutbuf[204].b = ((double)g_pRemember->getPeriphSecondaryGreen())/255.0f;
	lutbuf[204].c = ((double)g_pRemember->getPeriphSecondaryBlue())/255.0f;
	lutbuf[205].a = ((double)g_pRemember->getPeriphTertiaryRed())/255.0f;//tertiary
	lutbuf[205].b = ((double)g_pRemember->getPeriphTertiaryGreen())/255.0f;
	lutbuf[205].c = ((double)g_pRemember->getPeriphTertiaryBlue())/255.0f;
	
	vsgPresent();//begin drawing only center dot

	vsgLUTBUFFERWrite(0, &lutbuf);//update after dot is off screen to avoid flicker on dot
	vsgLUTBUFFERtoPalette(0);//if the cross flickers, it's ok because it's supposed to change anyway
	etu::CTimer IntervalTimer, theMaxWaitTimer, behaviorTimer;
	IntervalTimer.StartTimer();
	theMaxWaitTimer.StartTimer();

	//reset behavior count timer
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	//reset periph target trial line
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	
	double xDeg, yDeg;
	// convert the dot location from pixels to degrees for the analog output
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, centerX + stimXOffset, vsgDEGREEUNIT, &xDeg);
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, centerY + stimYOffset, vsgDEGREEUNIT, &yDeg);
	etu::AnalogOut(0, (float)xDeg/g_pSettings->getDegPerVolt(), true);//center x
	etu::AnalogOut(1, (float)-yDeg/g_pSettings->getDegPerVolt(), true);//center y
	etu::AnalogOut(2, (float)-9.95, true);//periph x
	etu::AnalogOut(3, (float)-9.95, true);//periph y
	if (textCode<2 || textCode==4 || textCode==6) {	//(center) target primary=red
		etu::AnalogOut(4, (float)5.0, true);//target color. red=+5, blue=-5
	} 
	else {		//(center) target primary=blue
		etu::AnalogOut(4, (float)-5.0, true);//target color. red=+5, blue=-5
	}
	if (textCode==0 || textCode==2) {	//flash color
		etu::AnalogOut(5, (float)5.0, true);//target color. red=+5, blue=-5, purple=0, blank=-10
	} 
	else if (textCode==1 || textCode==3) {
		etu::AnalogOut(5, (float)-5.0, true);//target color. red=+5, blue=-5, purple=0, blank=-10
	} 
	else if (textCode==4 || textCode==5) {
		etu::AnalogOut(5, (float)0.0, true);//target color. red=+5, blue=-5, purple=0, blank=-10
	} 
	else {
		etu::AnalogOut(5, (float)-10.0, true);//target color. red=+5, blue=-5, purple=0, blank=-10
	}

	if (g_pRemember->getResponse()) {
		//wait until response is typed in
		etu::CTimer timer_timeout;
		timer_timeout.StartTimer();
		while (g_pRememberDlg->response==-1 && timer_timeout.CheckTimer()<g_pRemember->getResponseTimeout()){
			g_pEyeTrackingDlg->EyePosSample();
			WaitUntil(1);
		}
		//if it timed out, say nothing
		if (timer_timeout.CheckTimer()>=g_pRemember->getResponseTimeout()) g_pRememberDlg->response=3;
		
		//send textmark with response and textcode
		sprintf(buffer, "response=%d|0x%2x/\0", g_pRememberDlg->response, 48+g_pRememberDlg->response);
		//g_pRememberDlg->theComm.transmit(buffer, (int)strlen(buffer));
		g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
		//periphX, periphY in degrees relative to center, with an offset
		int i = (int)((float)(g_pRemember->getNumStepsH())+(((float)periphX)-g_pSettings->getXOffset())/g_pRemember->getStepSize());
		int j = (int)((float)(g_pRemember->getNumStepsV())+(((float)periphY)-g_pSettings->getYOffset())/g_pRemember->getStepSize());

		//if (textCode<6) {//if not blank
		if (prevTextCode==4 || prevTextCode==5) {//if not blank
			if (prevTextCode<2 || prevTextCode==4) {//if right eye on center
				switch(g_pRememberDlg->response) {
					case 0: g_pRememberDlg->re_r[i*(1+2*g_pRemember->getNumStepsV())+j]++;
						break;
					case 2: g_pRememberDlg->re_b[i*(1+2*g_pRemember->getNumStepsV())+j]++;
						break;
					case 1: g_pRememberDlg->re_o[i*(1+2*g_pRemember->getNumStepsV())+j]++;
						break;
					default: break;
				}
			} 
			else {//left eye
				switch(g_pRememberDlg->response) {
					case 0: g_pRememberDlg->le_r[i*(1+2*g_pRemember->getNumStepsV())+j]++;
						break;
					case 2: g_pRememberDlg->le_b[i*(1+2*g_pRemember->getNumStepsV())+j]++;
						break;
					case 1: g_pRememberDlg->le_o[i*(1+2*g_pRemember->getNumStepsV())+j]++;
						break;
					default: break;
				}
			}
		} 
		else {//catch trial!
			//Beep(100,100);
			if (prevTextCode==6 || prevTextCode==7) {
				//blank
				g_pRememberDlg->catch_n_total++;
				if (g_pRememberDlg->response == 3 || g_pRememberDlg->response == 4) {
					g_pRememberDlg->catch_n += 1;
				}
			} 
			else if (prevTextCode==0 || prevTextCode==2) {
				//red dot
				g_pRememberDlg->catch_r_total++;
				if (g_pRememberDlg->response == 0) {
					g_pRememberDlg->catch_r += 1;
				}
				if (prevTextCode==0) {//red cross
					if (g_pRememberDlg->response == 0) {
						g_pRememberDlg->re_red_r[i*(1+2*g_pRemember->getNumStepsV())+j]++;//called it red
					} 
					else if (g_pRememberDlg->response == 2) {
						g_pRememberDlg->re_red_b[i*(1+2*g_pRemember->getNumStepsV())+j]++;//called it blue
					} 
					else {
						g_pRememberDlg->re_red_o[i*(1+2*g_pRemember->getNumStepsV())+j]++;//who knows
					}
				} 
				else {//blue cross
					if (g_pRememberDlg->response == 0) {
						g_pRememberDlg->le_red_r[i*(1+2*g_pRemember->getNumStepsV())+j]++;//called it red
					} 
					else if (g_pRememberDlg->response == 2) {
						g_pRememberDlg->le_red_b[i*(1+2*g_pRemember->getNumStepsV())+j]++;//called it blue
					} 
					else {
						g_pRememberDlg->le_red_o[i*(1+2*g_pRemember->getNumStepsV())+j]++;//who knows
					}
				}
			} 
			else if (prevTextCode==1 || prevTextCode==3) {
				//blue dot
				g_pRememberDlg->catch_b_total++;
				if (g_pRememberDlg->response == 2) {
					g_pRememberDlg->catch_b += 1;
				}
				if (prevTextCode==1) {//red cross
					if (g_pRememberDlg->response == 0) {
						g_pRememberDlg->re_blue_r[i*(1+2*g_pRemember->getNumStepsV())+j]++;//called it red
					} 
					else if (g_pRememberDlg->response == 2) {
						g_pRememberDlg->re_blue_b[i*(1+2*g_pRemember->getNumStepsV())+j]++;//called it blue
					} 
					else {
						g_pRememberDlg->re_blue_o[i*(1+2*g_pRemember->getNumStepsV())+j]++;//who knows
					}
				} else {//blue cross
					if (g_pRememberDlg->response == 0) {
						g_pRememberDlg->le_blue_r[i*(1+2*g_pRemember->getNumStepsV())+j]++;//called it red
					} 
					else if (g_pRememberDlg->response == 2) {
						g_pRememberDlg->le_blue_b[i*(1+2*g_pRemember->getNumStepsV())+j]++;//called it blue
					} 
					else {
						g_pRememberDlg->le_blue_o[i*(1+2*g_pRemember->getNumStepsV())+j]++;//who knows
					}
				}
			}

			//update catch trial things
			sprintf(buffer, "Red: %d/%d %d%%\0", g_pRememberDlg->catch_r, g_pRememberDlg->catch_r_total, ((g_pRememberDlg->catch_r_total > 0)? g_pRememberDlg->catch_r*100/g_pRememberDlg->catch_r_total:0) );
			g_pRememberDlg->m_CatchPctR.SetWindowText(buffer);
			sprintf(buffer, "Blue: %d/%d %d%%\0", g_pRememberDlg->catch_b, g_pRememberDlg->catch_b_total, ((g_pRememberDlg->catch_b_total > 0)? g_pRememberDlg->catch_b*100/g_pRememberDlg->catch_b_total:0) );
			g_pRememberDlg->m_CatchPctB.SetWindowText(buffer);
			sprintf(buffer, "Blank: %d/%d %d%%\0", g_pRememberDlg->catch_n, g_pRememberDlg->catch_n_total, ((g_pRememberDlg->catch_n_total > 0)? g_pRememberDlg->catch_n*100/g_pRememberDlg->catch_n_total:0) );
			g_pRememberDlg->m_CatchPctN.SetWindowText(buffer);
		}

		//show updated image
		g_pRememberDlg->Invalidate();

		//reset
		g_pRememberDlg->response=-1;
	} 
	else if (false) {//if self_report
		//draw boxes on the screen to choose from (choose which one you saw)
		iPage=1-iPage;
		vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
		vsgSetDrawMode(vsgCENTREXY);

		if (g_pRemember->getUseNoise() && bNoiseOn) {
			vsgDrawNoise(0,0,iScrWidthPix, iScrHeightPix, g_pRemember->getNoiseSize(),g_pRemember->getNoiseSize(),0,2+g_pRemember->getNoisePct(),100);
		}

		double fiveDeg;
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, 5.0, vsgPIXELUNIT, &fiveDeg);
		
		//draw 4 boxes: red, blue, both, nothing
		//at (+/-10, +/-10) from center
		// both   blank
		// blue   red
		// --- red box ---
		vsgSetPen1(203);
		vsgDrawRect(centerX + screenXOffset + stimXOffset + 2*fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset + fiveDeg,
			2*fiveDeg, g_pRemember->getCrossPix());
		vsgDrawRect(centerX + screenXOffset + stimXOffset + 2*fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset + 3*fiveDeg,
			2*fiveDeg, g_pRemember->getCrossPix());
		vsgDrawRect(centerX + screenXOffset + stimXOffset + fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset + 2*fiveDeg,
			g_pRemember->getCrossPix(), 2*fiveDeg);
		vsgDrawRect(centerX + screenXOffset + stimXOffset + 3*fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset + 2*fiveDeg,
			g_pRemember->getCrossPix(), 2*fiveDeg);
		vsgSetPen1(203);
		vsgDrawOval(centerX + screenXOffset + stimXOffset + 2*fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset + 2*fiveDeg,
			2*periphSize, 2*periphSize);
		// --- blue box ---
		vsgSetPen1(204);
		vsgDrawRect(centerX + screenXOffset + stimXOffset - 2*fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset + fiveDeg,
			2*fiveDeg, g_pRemember->getCrossPix());
		vsgDrawRect(centerX + screenXOffset + stimXOffset - 2*fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset + 3*fiveDeg,
			2*fiveDeg, g_pRemember->getCrossPix());
		vsgDrawRect(centerX + screenXOffset + stimXOffset - fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset + 2*fiveDeg,
			g_pRemember->getCrossPix(), 2*fiveDeg);
		vsgDrawRect(centerX + screenXOffset + stimXOffset - 3*fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset + 2*fiveDeg,
			g_pRemember->getCrossPix(), 2*fiveDeg);
		vsgSetPen1(204);
		vsgDrawOval(centerX + screenXOffset + stimXOffset - 2*fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset + 2*fiveDeg,
			2*periphSize, 2*periphSize);
		// --- blank box ---
		vsgSetPen1(2);//background color outline
		vsgDrawRect(centerX + screenXOffset + stimXOffset + 2*fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset - 2*fiveDeg,
			2*fiveDeg, 2*fiveDeg);
		// --- both box ---
		vsgSetPen1(205);
		vsgDrawRect(centerX + screenXOffset + stimXOffset - 2*fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset - fiveDeg,
			2*fiveDeg, g_pRemember->getCrossPix());
		vsgDrawRect(centerX + screenXOffset + stimXOffset - 2*fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset - 3*fiveDeg,
			2*fiveDeg, g_pRemember->getCrossPix());
		vsgDrawRect(centerX + screenXOffset + stimXOffset - fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset - 2*fiveDeg,
			g_pRemember->getCrossPix(), 2*fiveDeg);
		vsgDrawRect(centerX + screenXOffset + stimXOffset - 3*fiveDeg,
			-1 * centerY + screenYOffset + stimYOffset - 2*fiveDeg,
			g_pRemember->getCrossPix(), 2*fiveDeg);
		vsgSetPen1(203);//red
		vsgDrawOval(centerX + screenXOffset + stimXOffset - 2*fiveDeg+periphSize,
			-1 * centerY + screenYOffset + stimYOffset - 2*fiveDeg,
			2*periphSize, 2*periphSize);
		vsgSetPen1(204);//blue
		vsgDrawOval(centerX + screenXOffset + stimXOffset - 2*fiveDeg-periphSize,
			-1 * centerY + screenYOffset + stimYOffset - 2*fiveDeg,
			2*periphSize, 2*periphSize);

		vsgPresent();//begin drawing response choice boxes

		//now wait until they look in one of the boxes
		WaitUntil(600);
		
		//now go back to the previous page to show the fixation cross again
		iPage=1-iPage;
		vsgSetDisplayPage(iPage);//begin drawing cross by itself

		//wait until on target
		WaitUntil(500);
	}

	WaitUntil(100);
	
	sprintf(buffer, "cRGB: %.2f, %.2f, %.2f pRGB: %.2f, %.2f, %.2f|0x0%x/\0", centerColor.a, centerColor.b, centerColor.c, periphColor.a, periphColor.b, periphColor.c, textCode);
	//g_pRememberDlg->theComm.transmit(buffer, (int)strlen(buffer));
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));

	iPage=1-iPage;

	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	vsgSetDrawMode(vsgCENTREXY);

	if (g_pRemember->getUseNoise() && bNoiseOn && !g_pRemember->getUseDim()) {
		vsgDrawNoise(0,0,iScrWidthPix, iScrHeightPix, g_pRemember->getNoiseSize(),g_pRemember->getNoiseSize(),0,2+g_pRemember->getNoisePct(),100);
	}

	if (!g_pRemember->getUseDim()) {
		vsgSetPen1(0);
		//draw center point
		if (!g_pRemember->getHideCenter() && !g_pRemember->getUseDim()) {
			vsgDrawRect(centerX + screenXOffset + stimXOffset,
				-1 * centerY + screenYOffset + stimYOffset,
				centerSize,
				g_pRemember->getCrossPix());
			vsgDrawRect(centerX + screenXOffset + stimXOffset,
				-1 * centerY + screenYOffset + stimYOffset,
				g_pRemember->getCrossPix(),
				centerSize);
		}
	}

	//draw peripheral point
	vsgSetPen1(1);
	//figure out where to draw the peripheral dot
	double pixPeriphX, pixPeriphY;

	//make the point randomly along the grid as before
	GetNextPoint(periphX, periphY);

	//preempt next point in sequence and place it on the other eye?

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRemember->getCenterXPos() + periphX, vsgPIXELUNIT, &pixPeriphX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRemember->getCenterYPos() + periphY, vsgPIXELUNIT, &pixPeriphY);

	//do not draw the dot if we're going to dim the center point
	if (!g_pRemember->getUseDim()) {
		//do not draw the dot on a blank trial
		if (textCode < 6) {
			vsgDrawOval(pixPeriphX + screenXOffset + stimXOffset, -1 * pixPeriphY + screenYOffset + stimYOffset, 2*periphSize, 2*periphSize);
		}
	}
	
	if (g_pRemember->useBehavior()) {
		//wait for the appropriate delay
		//(add in dim stuff here...)
		WaitUntil(g_pRemember->getIntertrialDelay() - IntervalTimer.CheckTimer() -32, 5);

		//wait the correct amount of time for behavioral contingency
		//ie, wait until the max limit, or until they have been in agreement with the timer
		//modified waituntil
		DWORD increment=5;
		double elapsedTime = theMaxWaitTimer.CheckTimer();
		
		// sleep in half frame intervals until the desired number of msecs have passed
		while ((elapsedTime < g_pRemember->getMaxDelay()) && ((vsgIOReadDigitalIn() & vsgDIG0) <= 0) && (!bEndThread)) {//0x001
			g_pEyeTrackingDlg->EyePosSample();
			WaitUntil(10, 5);
			elapsedTime = theMaxWaitTimer.CheckTimer();
		} //end waituntil

		//check to see if it timed out
		if (elapsedTime >= g_pRemember->getMaxDelay()) {
			bReset = true;
		}
				
		// behvioral contingency counter should be reset
		vsgIOWriteDigitalOut(0xFFFF, vsgDIG1);
		WaitUntil(10);
		vsgIOWriteDigitalOut(0x0000, vsgDIG1);
	} 
	else {
        //WaitUntil(g_pRemember->getFlashInterval() - IntervalTimer.CheckTimer() -32, 5);
		if (g_pRemember->getRandomizeFlashInterval()) {
			double randomtime = g_pRemember->getFlashIntervalRandMin() + (rand()%(g_pRemember->getFlashIntervalRandMax()-g_pRemember->getFlashIntervalRandMin()));
			WaitUntil(randomtime - IntervalTimer.CheckTimer() -32, 5);
		} 
		else {
			if (g_pRemember->getUseDim()) {
				double randomtime = g_pRemember->getDimOnsetMin();
				if (g_pRemember->getDimOnsetMax() > g_pRemember->getDimOnsetMin()) {
					randomtime += rand()%(g_pRemember->getDimOnsetMax()-g_pRemember->getDimOnsetMin());
				}
				WaitUntil(g_pRemember->getIntertrialDelay() + randomtime - IntervalTimer.CheckTimer() -32, 5);
			} 
			else {
				WaitUntil(g_pRemember->getIntertrialDelay() - IntervalTimer.CheckTimer() -32, 5);
			}
		}
	}
	if (!bReset) {
		//if behavior timeout happens, skip all of this stuff
		if (g_pRemember->getUseDim()) {
			//make the cross dim until the button is pressed BEFORE ever showing the center crosshair
			lutbuf[0].a = centerDimColor.a;
			lutbuf[0].b = centerDimColor.b;
			lutbuf[0].c = centerDimColor.c;
			vsgLUTBUFFERWrite(0, &lutbuf);
			vsgLUTBUFFERtoPalette(0);

			FlashTimer.StartTimer();
			iPage=1-iPage;

			vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
			vsgSetDrawMode(vsgCENTREXY);

			if (g_pRemember->getUseNoise() && bNoiseOn) {
				vsgDrawNoise(0,0,iScrWidthPix, iScrHeightPix, g_pRemember->getNoiseSize(),g_pRemember->getNoiseSize(),0,2+g_pRemember->getNoisePct(),100);
			}
			
			//draw center point
			if (!g_pRemember->getHideCenter()) {
				vsgSetPen1(202);
				vsgDrawRect(centerX + screenXOffset + stimXOffset,
					-1 * centerY + screenYOffset + stimYOffset,
					centerSize,
					g_pRemember->getCrossPix());
				vsgDrawRect(centerX + screenXOffset + stimXOffset,
					-1 * centerY + screenYOffset + stimYOffset,
					g_pRemember->getCrossPix(),
					centerSize);
			}

			//draw peripheral dot
			vsgSetPen1(1);

			//do not draw the dot on a blank trial
			if (textCode < 6) {
				vsgDrawOval(pixPeriphX + screenXOffset + stimXOffset, -1 * pixPeriphY + screenYOffset + stimYOffset, 2*periphSize, 2*periphSize);
			}

			//wait until button is pushed or timeout happens
			// sleep in half frame intervals until the desired number of msecs have passed
			double elapsedTime = FlashTimer.CheckTimer();
			while (elapsedTime < g_pRemember->getDimTimeout() && (!bEndThread)
				&& ((vsgIOReadDigitalIn() & 0x003) <= 0)  ) //???? what; 0x003 should be vsgDIG0 right?
			{
				//digital in 3 = pin 17
				elapsedTime = FlashTimer.CheckTimer();
				g_pEyeTrackingDlg->EyePosSample();
				WaitUntil(10, 5);
			}
		}//end if dim
	
		//BEEP!!!
		if (g_pRemember->getResponse()) {
			//beep to prompt response typer
			Beep(1000,100);
			g_pEyeTrackingDlg->EyePosSample();

			//reset
			g_pRememberDlg->response=-1;
		}

		//Ramp Up Peripheral Dot
		etu::CTimer RampTimer;
		RampTimer.StartTimer();
		if (g_pRemember->getRampUp()) {
			while (RampTimer.CheckTimer() < g_pRemember->getRampDuration()) {
				double ramp_pct = RampTimer.CheckTimer() / (double)(g_pRemember->getRampDuration());
				//ramp up from either background color or noise color if noise is on and greater-equal than 50%
				double ramp_r, ramp_g, ramp_b;
				if (g_pRemember->getUseNoise() && bNoiseOn && (g_pRemember->getNoisePct() >= 50)) {
					ramp_r = noiseColor.a;
					ramp_g = noiseColor.b;
					ramp_b = noiseColor.c;
				} 
				else {
					ramp_r = bgColor.a;
					ramp_g = bgColor.b;
					ramp_b = bgColor.c;
				}

				//change palette to have new ramped value
				lutbuf[1].a = ramp_r + ((periphColor.a - ramp_r) * ramp_pct);
				lutbuf[1].b = ramp_g + ((periphColor.b - ramp_g) * ramp_pct);
				lutbuf[1].c = ramp_b + ((periphColor.c - ramp_b) * ramp_pct);
				vsgLUTBUFFERWrite(0, &lutbuf);
				vsgLUTBUFFERtoPalette(0);

				vsgPresent();//draw center dot with ramping intensity flashing peripheral dot
			}
			lutbuf[1].a = periphColor.a;
			lutbuf[1].b = periphColor.b;
			lutbuf[1].c = periphColor.c;
			vsgLUTBUFFERWrite(0, &lutbuf);
			vsgLUTBUFFERtoPalette(0);
		}

		vsgPresent();//draw center dot with flashing peripheral dot

		FlashTimer.StartTimer();

		etu::AnalogOut(2, (float)(xDeg+periphX) / g_pSettings->getDegPerVolt(), true);//periph x (in degrees)
		etu::AnalogOut(3, (float)(-yDeg+periphY) / g_pSettings->getDegPerVolt(), true);//periph y
		if (g_pRemember->getStimType() == CRemember::STIM_MEMORY_GUIDED) {
			etu::AnalogOut(0, (float)xDeg/g_pSettings->getDegPerVolt(), true);//center x
			etu::AnalogOut(1, (float)-yDeg/g_pSettings->getDegPerVolt(), true);//center y
			if (textCode<2) {	//(center) target primary=red
				etu::AnalogOut(4, (float)5.0, true);//target color. red=+5, blue=-5
			} 
			else {		//(center) target primary=blue
				etu::AnalogOut(4, (float)-5.0, true);//target color. red=+5, blue=-5
			}
		} 
		else {
			etu::AnalogOut(0, (float)(xDeg+periphX)/g_pSettings->getDegPerVolt(), true);//periph x
			etu::AnalogOut(1, (float)(-yDeg+periphY)/g_pSettings->getDegPerVolt(), true);//periph y
			/*if ((textCode%2)==0) {	//(periph) target primary=red
				etu::AnalogOut(4, (float)5.0, true);//target color. red=+5, blue=-5
			} else {		//(periph) target primary=blue
				etu::AnalogOut(4, (float)-5.0, true);//target color. red=+5, blue=-5
			}*///commented out so this channel always represents the center cross color
		}
		iPage = 1-iPage;

		// Memory Guided Stimulus
		if (g_pRemember->getStimType() == CRemember::STIM_MEMORY_GUIDED) {
			vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
			vsgSetDrawMode(vsgCENTREXY);

			if (g_pRemember->getUseNoise() && bNoiseOn) {
				vsgDrawNoise(0,0,iScrWidthPix, iScrHeightPix, g_pRemember->getNoiseSize(),g_pRemember->getNoiseSize(),0,2+g_pRemember->getNoisePct(),100);
			}
			
			//draw center point
			if (g_pRemember->getUseDim()) {
				//make the cross dim instead!
				vsgSetPen1(202);
			} 
			else {
				vsgSetPen1(0);
			}
			vsgDrawRect(centerX + screenXOffset + stimXOffset,
				-1 * centerY + screenYOffset + stimYOffset,
				centerSize,
				g_pRemember->getCrossPix());
			vsgDrawRect(centerX + screenXOffset + stimXOffset,
				-1 * centerY + screenYOffset + stimYOffset,
				g_pRemember->getCrossPix(),
				centerSize);

			WaitUntil(g_pRemember->getFlashDuration() - FlashTimer.CheckTimer());

			vsgPresent();//draw center dot only

			FlashTimer.StartTimer();
			etu::AnalogOut(0, (float)xDeg/g_pSettings->getDegPerVolt(), true);//center x
			etu::AnalogOut(1, (float)-yDeg/g_pSettings->getDegPerVolt(), true);//center y
			etu::AnalogOut(2, (float)-9.95, true);//periph x
			etu::AnalogOut(3, (float)-9.95, true);//periph y
			if (textCode<2) {	//(center) target primary=red
				etu::AnalogOut(4, (float)5.0, true);//target color. red=+5, blue=-5
			} 
			else {		//(center) target primary=blue
				etu::AnalogOut(4, (float)-5.0, true);//target color. red=+5, blue=-5
			}
			
			iPage=1-iPage;

			vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
			vsgSetDrawMode(vsgCENTREXY);
			
			if (g_pRemember->getRandomizePostFlash()) {
				double randomtime = g_pRemember->getPostFlashRandMin() + (rand()%(g_pRemember->getPostFlashRandMax()-g_pRemember->getPostFlashRandMin()));
				WaitUntil(randomtime - FlashTimer.CheckTimer());
			} 
			else {
				WaitUntil(g_pRemember->getPostFlashDelay() - FlashTimer.CheckTimer());
			}

			vsgPresent();//draw blank screen

			FlashTimer.StartTimer();
			
			//periph target trial line => make saccade to remembered location
			vsgIOWriteDigitalOut(0x0000, vsgDIG0);

			etu::AnalogOut(0, (float)(xDeg+periphX) / g_pSettings->getDegPerVolt(), true);//periph x (in degrees)
			etu::AnalogOut(1, (float)(-yDeg+periphY) / g_pSettings->getDegPerVolt(), true);//periph y
			etu::AnalogOut(2, (float)-9.95, true);//periph x
			etu::AnalogOut(3, (float)-9.95, true);//periph y
			
			etu::AnalogOut(4, (float)0.0, true);//reward for either color
			
			iPage = 1 - iPage;
		} 
		else {
			//not memory guided
			if (g_pRemember->getUseDim()) {
				
				vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
				vsgSetDrawMode(vsgCENTREXY);

				if (g_pRemember->getUseNoise() && bNoiseOn) {
					vsgDrawNoise(0,0,iScrWidthPix, iScrHeightPix, g_pRemember->getNoiseSize(),g_pRemember->getNoiseSize(),0,2+g_pRemember->getNoisePct(),100);
				}
				
				//draw center point
				vsgSetPen1(202);
				vsgDrawRect(centerX + screenXOffset + stimXOffset,
					-1 * centerY + screenYOffset + stimYOffset,
					centerSize,
					g_pRemember->getCrossPix());
				vsgDrawRect(centerX + screenXOffset + stimXOffset,
					-1 * centerY + screenYOffset + stimYOffset,
					g_pRemember->getCrossPix(),
					centerSize);
				WaitUntil(g_pRemember->getFlashDuration() - FlashTimer.CheckTimer(), 5);

				vsgPresent();//draw dim cross without dot

				FlashTimer.StartTimer();

				etu::AnalogOut(0, (float)xDeg/g_pSettings->getDegPerVolt(), true);//center x
				etu::AnalogOut(1, (float)-yDeg/g_pSettings->getDegPerVolt(), true);//center y
				etu::AnalogOut(2, (float)-9.95, true);//periph x
				etu::AnalogOut(3, (float)-9.95, true);//periph y

				iPage=1-iPage;
			}
			//periph target trial line high => look at periph dot
			vsgIOWriteDigitalOut(0x0000, vsgDIG0);
		}//end yes-no memory guided
	}

	if (iOn >= g_pRemember->getAlternateBlockSize()) {
		iOn = -1*g_pRemember->getAlternateBlockSize();
	}
	iOn++;
	
	bOn = !bOn;
	prevTextCode=textCode;

	return true;
}

/**************************************************************
	 GetNextPoint
	 -   This function calculates the dot position for the next
	 update. 
***************************************************************/
void CRememberStimulus::GetNextPoint(double& dPosXDeg, double& dPosYDeg) 
{
	int iLocIndex;

	// set the position to zero
	dPosXDeg = 0;
	dPosYDeg = 0;

	if (g_pRemember->useSeqRandom()) {
		//continue through sequence
		iLocIndex = g_pRemember->getLocationIndex(iSeqLocIndex);
		iSeqLocIndex = (iSeqLocIndex+1)%( g_pRemember->getNumLocations());//4 *
		//look up index of real location to use
		//iLocIndex = g_pRemember->getLocationIndex(iSeqLocIndex);
	} 
	else {
		// if we are not avoiding the zero point, choose a random point
		// and make sure it is not the same point as was previously displayed
		iLocIndex = rand() % g_pRemember->getNumLocations();
		if (iLocIndex == iOldLocIndex) iLocIndex = (iLocIndex + 1) % g_pRemember->getNumLocations();
	}

	// calculate the dot position based on the location just chosen
	dPosXDeg = g_pRemember->getLocationX(iLocIndex) + g_pSettings->getXOffset();
	dPosYDeg = g_pRemember->getLocationY(iLocIndex) + g_pSettings->getYOffset();

	//if we're using the eye tracker input and it's a foveal trial, trash the value
	// classes:   1 grid   2 periph fovea   3 center fovea
	if (g_pRemember->getAutoPosition() && (g_pRemember->getLocationClass(iLocIndex) > 1)) {
		double randx = 0, randy=0;
		double rand_pct = 0.01*((double)(rand()%100));
		if (g_pRemember->getLocationClass(iLocIndex) == 2) {
			//periph
			randx = (rand_pct*g_pRemember->getPeriphXRadius()*2.0)-g_pRemember->getPeriphXRadius();
			randy = (rand_pct*g_pRemember->getPeriphYRadius()*2.0)-g_pRemember->getPeriphYRadius();
		} 
		else {
			//center
			randx = (rand_pct*g_pRemember->getCenterXRadius()*2.0)-g_pRemember->getCenterXRadius();
			randy = (rand_pct*g_pRemember->getCenterYRadius()*2.0)-g_pRemember->getCenterYRadius();
		}
		//use the position from where the eye is now (not for the upcoming text code)
		int textCode = prevTextCode;//g_pRemember->getLocationType(iLocIndex);//iSeqLocIndex
		if (textCode<2 || textCode==4 || textCode==6) {
			//right eye
			dPosXDeg = g_pEyeTrackingDlg->avgrx + randx;
			dPosYDeg = g_pEyeTrackingDlg->avgry + randy;
		} 
		else {
			//left eye
			dPosXDeg = g_pEyeTrackingDlg->avglx + randx;
			dPosYDeg = g_pEyeTrackingDlg->avgly + randy;
		}
	}

	// save the index of the current position so we do not choose it again 
	// for the next position
	iOldLocIndex = iLocIndex;
}

/***************************************************************
	 StimulusType()
	 -  Inherited method that returns the stimulus type string.
****************************************************************/
CString CRememberStimulus::StimulusType() 
{
	//return "Saccade To Remember Stimulus";
	return "Human Suppression Scotomas";
}

#endif // APP_EYE_TRACKING
