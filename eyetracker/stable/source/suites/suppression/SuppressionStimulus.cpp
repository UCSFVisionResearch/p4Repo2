/**
 * @file SuppressionStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "SuppressionStimulus.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	VSG IN 0:	Next Stim Pulse. 1 => Behavioral contingency
				has been met, move on to the excursion phase.
				0 => behvioral continengency criteria has not 
				yet been met.
	Analog Out 0:	Horizontal target location?
	Analog Out 1:	Vertical target location?

NOTE:
	MKF 11/06/2007 None of these IO lines are really used
	without the behaviorally contingent dot.  Too many change
	requests were made over time and the remnants are left
	here in the code.
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor     
	 -  Instantiates the newly created object
************************************************************/
CSuppressionStimulus::CSuppressionStimulus() 
{
	Initialize();

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	
	pointX, pointY = 0;
	iPage = 0;
	iOn = 1;
	bOn = true;
	iOldLocIndex=-1;
	DisplayTimer.StartTimer();

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
	vsgLUTBUFFERWrite(1,&lutbuffer);
	vsgLUTBUFFERtoPalette(1);

	vsgSetBackgroundColour(&bgColor);
	vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgBACKGROUND);

	//delay to update pages in vram
	g_pSuppression->setDelay(1);

	theComm.init(false, CBR_115200, 1);
}

/*****************************************************************
	 Destructor
	 -  Cleans up when the object is deleted. The trial running
	 signal is turned off.
*****************************************************************/
CSuppressionStimulus::~CSuppressionStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	
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
	vsgSetBackgroundColour(&lutbuffer[vsgBACKGROUND]);
	vsgSetFixationColour(&lutbuffer[vsgFIXATION]);
	vsgLUTBUFFERWrite(1,&lutbuffer);
	vsgLUTBUFFERtoPalette(1);
	vsgSetBackgroundColour(&bgColor);
	vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgBACKGROUND);

	theComm.stop();
}

/*************************************************************************
	 DisplayStimulus()
	 -  Handles the display of the stimulus. 
*************************************************************************/
bool CSuppressionStimulus::DisplayStimulus() 
{
	char buffer[80];
	bool showtext = g_pSuppression->getTextBlock();
	bool showredblock = g_pSuppression->getRedBlock();
	bool showblueblock = g_pSuppression->getBlueBlock();
	bool adjust = g_pSuppression->getAdjustBG();
	int i;
	double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;

	double screenXOffset, screenYOffset;
	double stimXOffset, stimYOffset;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &stimXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &stimYOffset);

	if (g_pSuppression->getTest() == 0) {
		//minimum motion test
		if (g_pSuppression->getDelay()==2) {
			//only rebuild colors
			VSGLUTBUFFER lutbuffer;

			VSGTRIVAL redColor, blueColor, darkGrayColor, lightGrayColor;
			g_pSuppression->setDelay(0);//go back into full animation after we're done here
			vsgLUTBUFFERRead(1,&lutbuffer);

			if (adjust) {
				int offset=30;
				int darkred = (g_pSuppression->getRedValue()-offset <0)? 0 : g_pSuppression->getRedValue()-offset;
				int lightred = (g_pSuppression->getRedValue()+offset >255)? 255 : g_pSuppression->getRedValue()+offset;
				int darkblue = (g_pSuppression->getBlueValue()-offset <0)? 0 : g_pSuppression->getBlueValue()-offset;
				int lightblue = (g_pSuppression->getBlueValue()+offset >255)? 255 : g_pSuppression->getBlueValue()+offset;

				darkGrayColor.a = ((double)(darkred))/255.0f;
				darkGrayColor.b = ((double)(0))/255.0f;
				darkGrayColor.c = ((double)(darkblue))/255.0f;

				lightGrayColor.a = ((double)(lightred))/255.0f;
				lightGrayColor.b = ((double)(0))/255.0f;
				lightGrayColor.c = ((double)(lightblue))/255.0f;
			} 
			else {
				darkGrayColor.a = ((double)(g_pSuppression->getBgDarkRedValue()))/255.0f;
				darkGrayColor.b = ((double)(g_pSuppression->getBgDarkGreenValue()))/255.0f;
				darkGrayColor.c = ((double)(g_pSuppression->getBgDarkBlueValue()))/255.0f;

				lightGrayColor.a = ((double)(g_pSuppression->getBgLightRedValue()))/255.0f;
				lightGrayColor.b = ((double)(g_pSuppression->getBgLightGreenValue()))/255.0f;
				lightGrayColor.c = ((double)(g_pSuppression->getBgLightBlueValue()))/255.0f;
			}

			redColor.a = (g_pSuppression->getRedValue())/255.0f;
			redColor.b = 0.0/255.0f;
			redColor.c = 0.0/255.0f;

			blueColor.a = 0.0/255.0f;
			blueColor.b = 0.0/255.0f;
			blueColor.c = (g_pSuppression->getBlueValue())/255.0f;

			//set red and blue to their new values
			lutbuffer[1].a = redColor.a; lutbuffer[1].b = redColor.b; lutbuffer[1].c = redColor.c;
			lutbuffer[2].a = blueColor.a; lutbuffer[2].b = blueColor.b; lutbuffer[2].c = blueColor.c;
			lutbuffer[3].a = darkGrayColor.a; lutbuffer[3].b = darkGrayColor.b; lutbuffer[3].c = darkGrayColor.c;
			lutbuffer[4].a = lightGrayColor.a; lutbuffer[4].b = lightGrayColor.b; lutbuffer[4].c = lightGrayColor.c;
			
		
			vsgLUTBUFFERWrite(1,&lutbuffer);
			vsgLUTBUFFERtoPalette(1);
		} 
		else if (g_pSuppression->getDelay()==1) {
			//build 4 pages in VRAM
			g_pSuppression->setDelay(0);

			VSGTRIVAL whiteColor, redColor, blueColor, darkGrayColor, lightGrayColor;
			
			whiteColor.a = 255.0/255.0f;
			whiteColor.b = 255.0/255.0f;
			whiteColor.c = 255.0/255.0f;

			if (adjust) {
				int offset=30;
				int darkred = (g_pSuppression->getRedValue()-offset <0)? 0 : g_pSuppression->getRedValue()-offset;
				int lightred = (g_pSuppression->getRedValue()+offset >255)? 255 : g_pSuppression->getRedValue()+offset;
				int darkblue = (g_pSuppression->getBlueValue()-offset <0)? 0 : g_pSuppression->getBlueValue()-offset;
				int lightblue = (g_pSuppression->getBlueValue()+offset >255)? 255 : g_pSuppression->getBlueValue()+offset;

				darkGrayColor.a = ((double)(darkred))/255.0f;
				darkGrayColor.b = ((double)(0))/255.0f;
				darkGrayColor.c = ((double)(darkblue))/255.0f;

				lightGrayColor.a = ((double)(lightred))/255.0f;
				lightGrayColor.b = ((double)(0))/255.0f;
				lightGrayColor.c = ((double)(lightblue))/255.0f;
			} 
			else {
				darkGrayColor.a = ((double)(g_pSuppression->getBgDarkRedValue()))/255.0f;
				darkGrayColor.b = ((double)(g_pSuppression->getBgDarkGreenValue()))/255.0f;
				darkGrayColor.c = ((double)(g_pSuppression->getBgDarkBlueValue()))/255.0f;

				lightGrayColor.a = ((double)(g_pSuppression->getBgLightRedValue()))/255.0f;
				lightGrayColor.b = ((double)(g_pSuppression->getBgLightGreenValue()))/255.0f;
				lightGrayColor.c = ((double)(g_pSuppression->getBgLightBlueValue()))/255.0f;
			}

			redColor.a = (g_pSuppression->getRedValue())/255.0f;
			redColor.b = 0.0/255.0f;
			redColor.c = 0.0/255.0f;

			blueColor.a = 0.0/255.0f;
			blueColor.b = 0.0/255.0f;
			blueColor.c = (g_pSuppression->getBlueValue())/255.0f;
			/*---------------------------------------------*/
			GridSpace = (int)(4*(g_pSuppression->getGridSpace()));
			//vsgUnit2Unit(vsgDEGREEUNIT, g_pSuppression->getGridSpace(), vsgPIXELUNIT, &GridSpace);
			
			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSuppression->getStripeHeight(), vsgPIXELUNIT, &GridHeight);
			
			int iGridSize = g_pSuppression->getGridSize();
			sprintf(buffer, "r=%d b=%d\0", g_pSuppression->getRedValue(), g_pSuppression->getBlueValue());
			/*---------------------------------------------*/
			vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgBACKGROUND);
			vsgSetDrawMode(vsgCENTREXY);

			//light grey
			//--
			VSGLUTBUFFER buf;
			for (i=0; i<256; i++) {
				buf[i].a = ((double)g_pSettings->getRedBG())/255.0f;//0
				buf[i].b = ((double)g_pSettings->getBlueBG())/255.0f;
				buf[i].c = ((double)g_pSettings->getGreenBG())/255.0f;
			}
			buf[1].a = redColor.a;
			buf[1].b = redColor.b;
			buf[1].c = redColor.c;
			buf[2].a = blueColor.a;
			buf[2].b = blueColor.b;
			buf[2].c = blueColor.c;
			buf[3].a = darkGrayColor.a;
			buf[3].b = darkGrayColor.b;
			buf[3].c = darkGrayColor.c;
			buf[4].a = lightGrayColor.a;
			buf[4].b = lightGrayColor.b;
			buf[4].c = lightGrayColor.c;
			vsgLUTBUFFERWrite(1,&buf);
			vsgLUTBUFFERtoPalette(1);
			vsgSetPen1(4);
			//--
			if (showtext) vsgDrawString(0, -iScrHeightPix/4, buffer);

			//dark grey
			
			//red
			
			//--
			vsgSetPen1(1);
			//--
			vsgDrawRect(-iGridSize*GridSpace-GridSpace/4,0,GridSpace/2,GridHeight);//left edge
			vsgDrawRect(iGridSize*GridSpace-3*GridSpace/4,0,GridSpace/2,GridHeight);//right edge 
			for (i=1; i<iGridSize; i++) {
				vsgDrawRect((2*i)*GridSpace-iGridSize*GridSpace-GridSpace/2,0,GridSpace,GridHeight);
			}
			if (showredblock) vsgDrawRect(iScrWidthPix/2-GridSpace,GridHeight/4,GridHeight,GridHeight/2);//block

			//blue
			
			//--
			vsgSetPen1(2);
			//--
			for (i=0; i<iGridSize; i++) {
				vsgDrawRect((1+2*i)*GridSpace-iGridSize*GridSpace-GridSpace/2,0,GridSpace,GridHeight);
			}
			if (showblueblock) vsgDrawRect(iScrWidthPix/2-GridSpace,-GridHeight/4,GridHeight,GridHeight/2);//block
			/*---------------------------------------------*/
			vsgSetDrawPage(vsgVIDEOPAGE, 1, vsgBACKGROUND);
			vsgSetDrawMode(vsgCENTREXY);

			//light grey
			
			//--
			vsgSetPen1(4);
			//--
			if (showtext) vsgDrawString(0, -iScrHeightPix/4, buffer);
			for (i=0; i<iGridSize; i++) {
				vsgDrawRect(2*i*GridSpace-iGridSize*GridSpace,0,GridSpace,GridHeight);
			}

			//dark grey
			
			//--
			vsgSetPen1(3);
			//--
			for (i=0; i<iGridSize; i++) {
				vsgDrawRect((1+2*i)*GridSpace-iGridSize*GridSpace,0,GridSpace,GridHeight);
			}
			
			//red
			
			//--
			vsgSetPen1(1);
			//--
			if (showredblock) vsgDrawRect(iScrWidthPix/2-GridSpace,GridHeight/4,GridHeight,GridHeight/2);//block

			//blue
			
			//--
			vsgSetPen1(2);
			//--
			if (showblueblock) vsgDrawRect(iScrWidthPix/2-GridSpace,-GridHeight/4,GridHeight,GridHeight/2);//block
			/*---------------------------------------------*/
			vsgSetDrawPage(vsgVIDEOPAGE, 2, vsgBACKGROUND);
			vsgSetDrawMode(vsgCENTREXY);

			//light grey
			
			//--
			vsgSetPen1(4);
			//--
			if (showtext) vsgDrawString(0, -iScrHeightPix/4, buffer);

			//dark grey
			
			//red
			
			//--
			vsgSetPen1(1);
			//--
			for (i=0; i<iGridSize; i++) {
				vsgDrawRect((1+2*i)*GridSpace-iGridSize*GridSpace-GridSpace/2,0,GridSpace,GridHeight);
			}
			if (showredblock) vsgDrawRect(iScrWidthPix/2-GridSpace,GridHeight/4,GridHeight,GridHeight/2);//block

			//blue
			
			//--
			vsgSetPen1(2);
			//--
			vsgDrawRect(-iGridSize*GridSpace-GridSpace/4,0,GridSpace/2,GridHeight);//left edge
			vsgDrawRect(iGridSize*GridSpace-3*GridSpace/4,0,GridSpace/2,GridHeight);//right edge 
			for (i=1; i<iGridSize; i++) {
				vsgDrawRect((2*i)*GridSpace-iGridSize*GridSpace-GridSpace/2,0,GridSpace,GridHeight);
			}
			if (showblueblock) vsgDrawRect(iScrWidthPix/2-GridSpace,-GridHeight/4,GridHeight,GridHeight/2);//block
			/*---------------------------------------------*/
			vsgSetDrawPage(vsgVIDEOPAGE, 3, vsgBACKGROUND);
			vsgSetDrawMode(vsgCENTREXY);

			//light grey
			
			//--
			vsgSetPen1(4);
			//--
			if (showtext) vsgDrawString(0, -iScrHeightPix/4, buffer);
			for (i=0; i<iGridSize; i++) {
				vsgDrawRect((1+2*i)*GridSpace-iGridSize*GridSpace,0,GridSpace,GridHeight);
			}

			//dark grey
			
			//--
			vsgSetPen1(3);
			//--
			for (i=0; i<iGridSize; i++) {
				vsgDrawRect(2*i*GridSpace-iGridSize*GridSpace,0,GridSpace,GridHeight);
			}
			
			//red
			
			//--
			vsgSetPen1(1);
			//--
			if (showredblock) vsgDrawRect(iScrWidthPix/2-GridSpace,GridHeight/4,GridHeight,GridHeight/2);//block

			//blue
			
			//--
			vsgSetPen1(2);
			//--
			if (showblueblock) vsgDrawRect(iScrWidthPix/2-GridSpace,-GridHeight/4,GridHeight,GridHeight/2);//block
		}

		vsgSetZoneDisplayPage(vsgVIDEOPAGE, iPage);
		//b rr bb rr bb rr bb rr b
		//ll dd ll dd ll dd ll dd
		//r bb rr bb rr bb rr bb r
		//dd ll dd ll dd ll dd ll
		WaitUntil(g_pSuppression->getDelayms());

		vsgPresent();//draw

		//iPage=1-iPage;
		iPage=(iPage+1)%4;
	} 
	else if (g_pSuppression->getTest()==1){
		//flicker fusion test
		if (g_pSuppression->getDelay()==2) {
			//only rebuild colors
			VSGTRIVAL redColor, blueColor;
			g_pSuppression->setDelay(0);

			redColor.a = (g_pSuppression->getRedValue())/255.0f;
			redColor.b = 0.0/255.0f;
			redColor.c = 0.0/255.0f;

			blueColor.a = 0.0/255.0f;
			blueColor.b = 0.0/255.0f;
			blueColor.c = (g_pSuppression->getBlueValue())/255.0f;

			vsgObjSetPixelLevels(0,2);
			vsgObjSetColourVector(&blueColor,&redColor,vsgBIPOLAR);
		} 
		else if (g_pSuppression->getDelay()==1) {
			g_pSuppression->setDelay(0);

			VSGTRIVAL whiteColor, redColor, blueColor;
			
			whiteColor.a = 255.0/255.0f;
			whiteColor.b = 255.0/255.0f;
			whiteColor.c = 255.0/255.0f;

			redColor.a = (g_pSuppression->getRedValue())/255.0f;
			redColor.b = 0.0/255.0f;
			redColor.c = 0.0/255.0f;

			blueColor.a = 0.0/255.0f;
			blueColor.b = 0.0/255.0f;
			blueColor.c = (g_pSuppression->getBlueValue())/255.0f;

			GridSpace = (int)(g_pSuppression->getGridSpace());//?
			
			int iGridSize = g_pSuppression->getGridSize();
			sprintf(buffer, "r=%d b=%d\0", g_pSuppression->getRedValue(), g_pSuppression->getBlueValue());

			vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgBACKGROUND);
			vsgSetDrawMode(vsgCENTREXY);
			vsgObjDestroy(g_grating);
			g_grating = vsgObjCreate();
			vsgObjSetContrast(100);
			vsgObjSetPixelLevels(0,2);
			vsgObjSetColourVector(&blueColor,&redColor,vsgBIPOLAR);

			vsgDrawChessboard(0,0,iGridSize*GridSpace,iGridSize*GridSpace,GridSpace,GridSpace,0);
			vsgDrawChessboard(iScrWidthPix/2,0,iGridSize*GridSpace,iGridSize*GridSpace,iGridSize*GridSpace/2,iGridSize*GridSpace,0);
			vsgDrawString(-iScrWidthPix/4, 0, buffer);

			vsgSetDrawPage(vsgVIDEOPAGE, 1, vsgBACKGROUND);
			vsgSetDrawMode(vsgCENTREXY);
			vsgObjDestroy(g_grating);
			g_grating = vsgObjCreate();
			vsgObjSetContrast(100);
			vsgObjSetPixelLevels(0,2);
			vsgObjSetColourVector(&blueColor,&redColor,vsgBIPOLAR);
	
			vsgDrawChessboard(0,0,iGridSize*GridSpace,iGridSize*GridSpace,GridSpace,GridSpace,90);
			vsgDrawChessboard(iScrWidthPix/2,0,iGridSize*GridSpace,iGridSize*GridSpace,iGridSize*GridSpace/2,iGridSize*GridSpace,0);
			vsgDrawString(-iScrWidthPix/4, 0, buffer);
		}
	
		vsgSetZoneDisplayPage(vsgVIDEOPAGE, iPage);
		//blue-red	red-blue
		WaitUntil(0 + (iPage==0? 10 : 0));

		vsgPresent();//draw
		iPage=1-iPage;
	} 
	else if (g_pSuppression->getTest()==2) {
		//minimum motion with sine wave gratings
		if (g_pSuppression->getDelay()==2) {
			//only rebuild colors
			g_pSuppression->setDelay(0);
		} 
		else if (g_pSuppression->getDelay()==1) {
			//build 4 pages in VRAM
			g_pSuppression->setDelay(0);
			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSuppression->getStripeHeight(), vsgPIXELUNIT, &GridHeight);
			double spatialfreq = g_pSuppression->getSpatialFreq();

			//draw page 0
			vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgBACKGROUND);
			vsgSetDrawMode(vsgCENTREXY+vsgSOLIDPEN);//
			for (int i=0; i<GridHeight; i++) {
				int pixPerCyc = (GridHeight / g_pSuppression->getStripeHeight()) / spatialfreq;
				int pencolor = (((double)(i%pixPerCyc))/((double)pixPerCyc)) * 126.0;//position in cycle/pixPerCyc, times colors per cycle
				
				vsgSetPen1(pencolor%126);

				vsgSetPenSize(1,1);
				vsgDrawLine(screenXOffset + stimXOffset-GridHeight/2,screenYOffset + stimYOffset+i-GridHeight/2,screenXOffset + stimXOffset+GridHeight/2,screenYOffset + stimYOffset+i-GridHeight/2);
			}

			//draw page 2 (blank)
			vsgSetDrawPage(vsgVIDEOPAGE, 2, vsgBACKGROUND);
			vsgSetDrawMode(vsgCENTREXY);
			vsgSetPen1(253);
			vsgDrawRect(0 + screenXOffset + stimXOffset,
				0 + screenYOffset + stimYOffset,1024,768);//ten pixles(hack)

			//draw page 1 (fixation spot)
			vsgSetDrawPage(vsgVIDEOPAGE, 1, vsgBACKGROUND);
			vsgSetDrawMode(vsgCENTREXY);
			vsgSetPen1(252);
			vsgDrawOval(0,0,4,4);//ten pixles(hack)
		}

		bool draw = false;
		if (DisplayTimer.CheckTimer() > g_pSuppression->getDisplayTime()) {
			//draw off
			vsgSetZoneDisplayPage(vsgVIDEOPAGE, 2);
			vsgSetDrawMode(vsgCENTREXY);
			vsgSetPen1(253);
			vsgDrawRect(0,0,1024,768);//black
			vsgPresent();
			WaitUntil(g_pSuppression->getOffTime(), 10);

			//draw dot
			etu::AnalogOut(0, 0, true);
			etu::AnalogOut(1, 0, true);
			vsgSetZoneDisplayPage(vsgVIDEOPAGE, 1);
			vsgSetDrawMode(vsgCENTREXY);
			vsgSetPen1(252);
			vsgDrawOval(0 + screenXOffset + stimXOffset, 0 + screenYOffset + stimYOffset,4,4);//ten pixles(hack)
			vsgPresent();

			DotTimer.StartTimer();
			double elapsedTime = DotTimer.CheckTimer();
		
			//WaitUntil(g_pSuppression->getDotTime(), 10);
			// sleep in half frame intervals until the desired number of msecs have passed
			while ((elapsedTime < g_pSuppression->getDotTime()) && ((vsgIOReadDigitalIn() & vsgDIG0) <= 0) && (!bEndThread)) {//0x001
				WaitUntil(10, 5);
				elapsedTime = DotTimer.CheckTimer();
			}

			etu::AnalogOut(0, (float)-50.95/g_pSettings->getDegPerVolt(), true);
			etu::AnalogOut(1, (float)-50.95/g_pSettings->getDegPerVolt(), true);

			//now go into minimum motion loop
			switch(g_pSuppression->getSineColor()) {
				case 0://up/down
					if ((rand()%2)==0) {
						g_pSuppression->setRedValue( 255 );//up
						g_pSuppression->setBlueValue( 20 );
					} 
					else {
						g_pSuppression->setRedValue( 20 );//down
						g_pSuppression->setBlueValue( 255 );
					}
					break;
				case 1://random
					g_pSuppression->setRedValue( rand()%256 );
					g_pSuppression->setBlueValue( 255 );//rand()%256
					break;
				case 2://existing
					break;
				default: break;
			}
			float f = 0.45;//percent contrast
			float bdark = (0.5-f)*((float)(g_pSuppression->getRedValue() + g_pSuppression->getBlueValue()));
			float blight = (0.5+f)*((float)(g_pSuppression->getRedValue() + g_pSuppression->getBlueValue()));
			g_pSuppression->setBgDarkRedValue(  (bdark<0)? 0 : bdark  );
			g_pSuppression->setBgDarkGreenValue(0);
			g_pSuppression->setBgDarkBlueValue(  (bdark<0)? 0 : bdark  );
			g_pSuppression->setBgLightRedValue(  (blight>255)? 255 : blight  );
			g_pSuppression->setBgLightGreenValue(0);
			g_pSuppression->setBgLightBlueValue(  (blight>255)? 255 : blight  );

			VSGLUTBUFFER lutbuffer;
			for (int i=126; i<256; i++) {
				//253
				lutbuffer[i].a=((double)g_pSettings->getRedBG()) / 255.0f;	lutbuffer[i].b=((double)g_pSettings->getGreenBG()) / 255.0f;	lutbuffer[i].c=((double)g_pSettings->getBlueBG()) / 255.0f;
			}
			lutbuffer[252].a=(double)g_pSettings->getRed() / 255.0f;	lutbuffer[252].b=(double)g_pSettings->getGreen() / 255.0f;	lutbuffer[252].c=(double)g_pSettings->getBlue() / 255.0f;
			for (int i=0; i<126; i++) {
				double d = 0.5 + (
					(iPage%2==0)?
					0.5*sin( 2.0*PI * ((double)(i)) / 125.0) :
					0.5*sin(PI/2.0 + 2.0*PI * ((double)((i+63)%126)) / 125.0)//
					);
				d = (iPage<2)? d : 1.0-d;
				
				if (iPage%2==0){
					//126 colors of red-blue
					lutbuffer[i].a=d * (g_pSuppression->getRedValue())/255.0f;
					lutbuffer[i].b=0.0/255.0f;
					lutbuffer[i].c=(1.0-d)*(g_pSuppression->getBlueValue())/255.0f;
				} 
				else{//126+
					//126 colors of background
					lutbuffer[i].a=d*((double)(g_pSuppression->getBgDarkRedValue()))/255.0f + (1.0-d)*((double)(g_pSuppression->getBgLightRedValue()))/255.0f;
					lutbuffer[i].b=d*((double)(g_pSuppression->getBgDarkGreenValue()))/255.0f + (1.0-d)*((double)(g_pSuppression->getBgLightGreenValue()))/255.0f;
					lutbuffer[i].c=d*((double)(g_pSuppression->getBgDarkBlueValue()))/255.0f + (1.0-d)*((double)(g_pSuppression->getBgLightBlueValue()))/255.0f;
				}
			}
			vsgLUTBUFFERWrite(iPage,&lutbuffer);
			vsgLUTBUFFERtoPalette(iPage);
			
			vsgSetZoneDisplayPage(vsgVIDEOPAGE, 0);//iPage
			DisplayTimer.StartTimer();
			//vsgPresent();//causes extra flash
			draw = true;
		} 
		else {
			vsgSetZoneDisplayPage(vsgVIDEOPAGE, 0);//iPage
		}
		//b rr bb rr bb rr bb rr b
		//ll dd ll dd ll dd ll dd
		//r bb rr bb rr bb rr bb r
		//dd ll dd ll dd ll dd ll
		WaitUntil(g_pSuppression->getDelayms());

		//change palette
		VSGLUTBUFFER lutbuffer;
		for (int i=126; i<256; i++) {
			//253
			lutbuffer[i].a=((double)g_pSettings->getRedBG()) / 255.0f;	lutbuffer[i].b=((double)g_pSettings->getGreenBG()) / 255.0f;	lutbuffer[i].c=((double)g_pSettings->getBlueBG()) / 255.0f;
		}
		lutbuffer[252].a=(double)g_pSettings->getRed() / 255.0f;	lutbuffer[252].b=(double)g_pSettings->getGreen() / 255.0f;	lutbuffer[252].c=(double)g_pSettings->getBlue() / 255.0f;
		for (int i=0; i<126; i++) {
			double d = 0.5 + (
				(iPage%2==0)?
				0.5*sin( 2.0*PI * ((double)(i)) / 125.0) :
				0.5*sin(PI/2.0 + 2.0*PI * ((double)((i+63)%126)) / 125.0)//
				);
			d = (iPage<2)? d : 1.0-d;
			
			if (iPage%2==0){
				//126 colors of red-blue
				lutbuffer[i].a=d * (g_pSuppression->getRedValue())/255.0f;
				lutbuffer[i].b=0.0/255.0f;
				lutbuffer[i].c=(1.0-d)*(g_pSuppression->getBlueValue())/255.0f;
			} 
			else{//126+
				//126 colors of background
				lutbuffer[i].a=d*((double)(g_pSuppression->getBgDarkRedValue()))/255.0f + (1.0-d)*((double)(g_pSuppression->getBgLightRedValue()))/255.0f;
				lutbuffer[i].b=d*((double)(g_pSuppression->getBgDarkGreenValue()))/255.0f + (1.0-d)*((double)(g_pSuppression->getBgLightGreenValue()))/255.0f;
				lutbuffer[i].c=d*((double)(g_pSuppression->getBgDarkBlueValue()))/255.0f + (1.0-d)*((double)(g_pSuppression->getBgLightBlueValue()))/255.0f;
			}
		}
		vsgLUTBUFFERWrite(iPage,&lutbuffer);
		vsgLUTBUFFERtoPalette(iPage);

		sprintf(buffer, "r=%d b=%d BG: lr=%d lg=%d lb=%d dr=%d dg=%d db=%d size=%f sf=%f|0x00/\0", g_pSuppression->getRedValue(), g_pSuppression->getBlueValue(), g_pSuppression->getBgLightRedValue(), g_pSuppression->getBgLightGreenValue(), g_pSuppression->getBgLightBlueValue(), g_pSuppression->getBgDarkRedValue(), g_pSuppression->getBgDarkGreenValue(), g_pSuppression->getBgDarkBlueValue(), g_pSuppression->getStripeHeight(), g_pSuppression->getSpatialFreq());
		theComm.transmit(buffer, (int)strlen(buffer));
		iPage=(iPage+1)%4;
	} 
	else {
		//minimum motion multiple gratings
		int num_g = g_pSuppression->getNumGratings();
		int num_c = 252/num_g;
		if (g_pSuppression->getDelay()==2) {
			//only rebuild colors
			g_pSuppression->setDelay(0);
		} 
		else if (g_pSuppression->getDelay()==1) {
			g_pSuppression->setDelay(0);
			//build video pages
			//build 4 pages in VRAM

			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSuppression->getStripeHeight(), vsgPIXELUNIT, &GridHeight);
			double spatialfreq = g_pSuppression->getSpatialFreq();
			
			//draw page 0
			vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgBACKGROUND);
			vsgSetDrawMode(vsgCENTREXY+vsgSOLIDPEN);//
			for (int i=0; i<GridHeight; i++) {
				int pixPerCyc = (GridHeight / g_pSuppression->getStripeHeight()) / spatialfreq;
				vsgSetPenSize(1,1);
				int pencolor = (((double)(i%pixPerCyc))/((double)pixPerCyc)) * ((double)num_c);//position in cycle/pixPerCyc, times colors per cycle
				for (int j=0; j<num_g; j++) {
					vsgSetPen1(pencolor%num_c +num_c*j);
					vsgDrawLine(screenXOffset + stimXOffset-GridHeight/6	+ GridHeight*-num_g/4+GridHeight*j/2,
						screenYOffset + stimYOffset+i-GridHeight/2,
						screenXOffset + stimXOffset+GridHeight/6			+ GridHeight*-num_g/4+GridHeight*j/2,
						screenYOffset + stimYOffset+i-GridHeight/2);
				}
			}
			for (int j=0; j<num_g; j++) {
				sprintf(buffer, "%d\0", j+1);
				vsgSetPen1(252);
				vsgSetPen2(253);
				vsgDrawString(screenXOffset + stimXOffset	+ GridHeight*-num_g/4+GridHeight*j/2,
					screenYOffset + stimYOffset+GridHeight,
					buffer);
			}
		} // else { }//0
		
		//now go into minimum motion loop
		float f = 0.35;//percent contrast
		g_pSuppression->setRedValue(255);
		g_pSuppression->setBlueValue(255);
		float bdark = (0.5-f)*((float)(g_pSuppression->getRedValue() + g_pSuppression->getBlueValue()));
		float blight = (0.5+f)*((float)(g_pSuppression->getRedValue() + g_pSuppression->getBlueValue()));
		g_pSuppression->setBgDarkRedValue(  (bdark<0)? 0 : bdark  );
		g_pSuppression->setBgDarkGreenValue(0);
		g_pSuppression->setBgDarkBlueValue(  (bdark<0)? 0 : bdark  );
		g_pSuppression->setBgLightRedValue(  (blight>255)? 255 : blight  );
		g_pSuppression->setBgLightGreenValue(0);
		g_pSuppression->setBgLightBlueValue(  (blight>255)? 255 : blight  );
		
		VSGLUTBUFFER lutbuffer;
		
		vsgSetZoneDisplayPage(vsgVIDEOPAGE, 0);//iPage
		
		//b rr bb rr bb rr bb rr b
		//ll dd ll dd ll dd ll dd
		//r bb rr bb rr bb rr bb r
		//dd ll dd ll dd ll dd ll
		WaitUntil(g_pSuppression->getDelayms());

		//change palette
		for (int i=0; i<256; i++) {
			//253
			lutbuffer[i].a=((double)g_pSettings->getRedBG()) / 255.0f;	lutbuffer[i].b=((double)g_pSettings->getGreenBG()) / 255.0f;	lutbuffer[i].c=((double)g_pSettings->getBlueBG()) / 255.0f;
		}
		lutbuffer[252].a=(double)g_pSettings->getRed() / 255.0f;	lutbuffer[252].b=(double)g_pSettings->getGreen() / 255.0f;	lutbuffer[252].c=(double)g_pSettings->getBlue() / 255.0f;
		for (int i=0; i<num_c; i++) {
			for (int j=0; j<num_g; j++) {
				double d = 0.5 + (
					(iPage%2==0)?
					0.5*sin( 2.0*PI * ((double)(i)) / ((double)num_c)) :
					0.5*sin(PI/2.0 + 2.0*PI * ((double)((i+num_c/2)%num_c)) / ((double)num_c))//
					);
				d = (iPage<2)? d : 1.0-d;
				
				if (iPage%2==0){
					//num_c colors of red-blue//126
					lutbuffer[num_c*j+i].a=d * ((num_g-1-j)*g_pSuppression->getRedValue()/(num_g-1))/255.0f;//
					lutbuffer[num_c*j+i].b=0.0/255.0f;
					if (g_pSuppression->getMultipleFullBlue()) {
						//lutbuffer[num_c*j+i].c=255.0/255.0f;
						lutbuffer[num_c*j+i].c=(1.0-d)*255.0/255.0f;
					} else {
						lutbuffer[num_c*j+i].c=(1.0-d)*(j*g_pSuppression->getBlueValue()/(num_g-1))/255.0f;//(5-j)*/5
					}
				} 
				else{
					//num_c colors of background//126
					lutbuffer[num_c*j+i].a=d*((double)(g_pSuppression->getBgDarkRedValue()))/255.0f + (1.0-d)*((double)(g_pSuppression->getBgLightRedValue()))/255.0f;
					lutbuffer[num_c*j+i].b=d*((double)(g_pSuppression->getBgDarkGreenValue()))/255.0f + (1.0-d)*((double)(g_pSuppression->getBgLightGreenValue()))/255.0f;
					lutbuffer[num_c*j+i].c=d*((double)(g_pSuppression->getBgDarkBlueValue()))/255.0f + (1.0-d)*((double)(g_pSuppression->getBgLightBlueValue()))/255.0f;
				}
			}
		}
		vsgLUTBUFFERWrite(iPage,&lutbuffer);
		vsgLUTBUFFERtoPalette(iPage);

		sprintf(buffer, "r=%d b=%d BG: lr=%d lg=%d lb=%d dr=%d dg=%d db=%d size=%f sf=%f|0x00/\0", g_pSuppression->getRedValue(), g_pSuppression->getBlueValue(), g_pSuppression->getBgLightRedValue(), g_pSuppression->getBgLightGreenValue(), g_pSuppression->getBgLightBlueValue(), g_pSuppression->getBgDarkRedValue(), g_pSuppression->getBgDarkGreenValue(), g_pSuppression->getBgDarkBlueValue(), g_pSuppression->getStripeHeight(), g_pSuppression->getSpatialFreq());
		theComm.transmit(buffer, (int)strlen(buffer));

		iPage=(iPage+1)%4;
	}
	return true;
}

/***************************************************************
	 StimulusType()
	 -  Inherited method that returns the stimulus type string.
****************************************************************/
CString CSuppressionStimulus::StimulusType() 
{
	return "Minimum Motion Stimulus";
}

#endif // APP_EYE_TRACKING
