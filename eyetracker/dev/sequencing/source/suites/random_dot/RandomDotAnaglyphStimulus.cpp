/**
 * @file RandomDotAnaglyphStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"

/***********************************************************
COMMUNICATIONS:
	N/A
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor     
	 -  Instantiates the newly created object
************************************************************/
CRandomDotAnaglyphStimulus::CRandomDotAnaglyphStimulus() 
{
	Initialize();

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	
	pointX, pointY = 0;
	iPage = 0;

	srand( (unsigned int)time(NULL) );
	
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	// set the drawing mode, and set the origin to the center of the screen
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix  /2, iScrHeightPix / 2);
	vsgSetViewDistMM(g_pSettings->getViewDistance());

	//theComm.init(false, CBR_115200, 1);
	g_pRandomDotAnaglyph->sendSettings(g_pSettings->theSerialComm);
}

/*****************************************************************
	 Destructor
	 -  Cleans up when the object is deleted. The trial running
	 signal is turned off.
*****************************************************************/
CRandomDotAnaglyphStimulus::~CRandomDotAnaglyphStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);

	//send stop mark
	char buffer[80];
	sprintf(buffer, "RandomDotAnaglyph;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}

/*************************************************************************
	 DisplayStimulus()
	 -  Handles the display of the stimulus.
	 - show depth image embedded in red-blue-purple noise
	 only visible through stereopsis and red-blue glasses
*************************************************************************/
bool CRandomDotAnaglyphStimulus::DisplayStimulus() 
{
	char buff[80];
	double stimXOffset,stimYOffset,screenXOffset,screenYOffset;

	//show blank to start
	if (!g_pRandomDotAnaglyph->bStarted) {
		iPage=0;
		vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
		vsgPresent();
		g_pRandomDotAnaglyph->bStarted = true;
	} 
	else {
		//don't do anything when the image is already displayed on screen
		return true;
	}
	
	// convert offsets from degrees to pixels
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &stimXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &stimYOffset);


	// set the background color
	VSGTRIVAL bgColor;
	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;
	//init palette
	VSGLUTBUFFER lutbuffer;
	for (int i=0; i<256; i++) {
		lutbuffer[i].a=bgColor.a;									//0	black
		lutbuffer[i].b=bgColor.b;
		lutbuffer[i].c=bgColor.c;
	}
	lutbuffer[1].a=((double)g_pRandomDotAnaglyph->getRed())/255.0f;	//1	red
	lutbuffer[1].b=0.0f;
	lutbuffer[1].c=0.0f;
	lutbuffer[2].a=0.0f;											//2	blue
	lutbuffer[2].b=0.0f;
	lutbuffer[2].c=((double)g_pRandomDotAnaglyph->getBlue())/255.0f;
	lutbuffer[3].a=((double)g_pRandomDotAnaglyph->getRed())/255.0f;	//3	purple
	lutbuffer[3].b=0.0f;
	lutbuffer[3].c=((double)g_pRandomDotAnaglyph->getBlue())/255.0f;
	vsgSetBackgroundColour(&lutbuffer[vsgBACKGROUND]);
	vsgSetFixationColour(&lutbuffer[vsgFIXATION]);
	vsgLUTBUFFERWrite(1,&lutbuffer);
	vsgLUTBUFFERtoPalette(1);

	vsgSetDrawPage(vsgVIDEOPAGE, 2, vsgBACKGROUND);//first draw the depth map

	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix / 2);
	
	//could draw noise on page 3 if needed
	vsgSetCommand(vsgCLIPENABLE);
	vsgSetClipWindow(0,0,1024,768);
	int bgDisparity = g_pRandomDotAnaglyph->getBGDisparity();
	vsgSetPen2(bgDisparity);//black (bg)

	vsgSetPen1(bgDisparity);//background disparity
	vsgDrawRect(0,0,iScrWidthPix, iScrHeightPix);

	if (g_pRandomDotAnaglyph->getRadio()==0) {						// ************* text *************
		double a;
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, 6.0, vsgPIXELUNIT, &a);
		sprintf(buff, "%s\0", g_pRandomDotAnaglyph->getText());
		vsgSetTrueTypeFont("Arial Black");//Times New Roman
		vsgSetStringMode(100,300,vsgALIGNCENTRETEXT,vsgALIGNCENTRETEXT,0,vsgFONTNORMAL);

		vsgSetPen1(bgDisparity+g_pRandomDotAnaglyph->getTextDisparity());
		vsgDrawString(stimXOffset+screenXOffset, screenYOffset+stimYOffset, buff);
		
	} 
	else if (g_pRandomDotAnaglyph->getRadio()==1) {				// ************* depth image *************
		sprintf(buff, "%s\0", g_pRandomDotAnaglyph->getImagePath());
		vsgDrawImage(0, 0, 0, buff);//vsgPALETTELOAD

		//scale to g_pRandomDotAnaglyph->getMaxDisparity()
		char pixels[1024];
		for (int j=0; j<iScrHeightPix; j++) {
			vsgReadPixelLine(0,j,pixels,1024);
			for (int i=0; i<iScrWidthPix; i++) {
				pixels[i] = (pixels[i] * g_pRandomDotAnaglyph->getMaxDisparity())/256;
			}
			vsgDrawPixelLineFast(0,j,pixels,1024);
		}
	} 
	else {														// ************* shapes *************
		//pyramid
		for (int i=1; i<100; i++) {
			vsgSetPen1(bgDisparity + i*(g_pRandomDotAnaglyph->getMaxDisparity()-bgDisparity)/100);

			//4 sides
			vsgDrawRect(300,-300,200-i*2,200-i*2);
			
			//3 sides
			double triangle[] = { -2*(100-i)/tan(60*3.141593/180),-100+i,  0,100-i,  2*(100-i)/tan(60*3.141593/180),-100+i};
			vsgDrawPolygon(3,triangle,0,0,0);

			//cone
			vsgDrawOval(300,300,200-i*2,200-i*2);

			//egg
			double pct = 0.5+0.5*cos(3.141593*1.0*i/100.0);//transform [1,-1] to [0,1]
			vsgDrawOval(-300,300, 200*pct , 200*pct);

			//long edge of prism
			vsgDrawLine(-400+i, -400, -400+i, -200);
			vsgDrawLine(-400+1+i, -400, -400+1+i, -200);
			vsgDrawLine(-200-i, -400, -200-i, -200);
			vsgDrawLine(-200-1-i, -400, -200-1-i, -200);
		}
	}

	iPage = 1-iPage;
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	vsgSetDrawOrigin(0,0);
	//now mix in noise over the disparity map
	char depths[1024];
	char pixels[1024];
	for (int j=0; j<iScrHeightPix; j++) {
		vsgSetDrawPage(vsgVIDEOPAGE, 2, vsgNOCLEAR);//first get the depth map
		vsgReadPixelLine(0,j,depths,1024);
		for (int i=0; i<iScrWidthPix; i++) {
			//get depth
			int depth_add = (int)floor(0.5 * (double)((int)depths[i]));
			int depth_sub = (int)ceil(0.5 * (double)((int)depths[i]));
			bool c2 = (rand()%100) < g_pRandomDotAnaglyph->getNoise();									//noise value
			
			if (i-depth_sub>=0) {
				int left_c = pixels[i-depth_sub];
				int left_new = (c2)?
					(((left_c==2)||(left_c==3))? 3 : 1)	://purple noise => red or purple
					(((left_c==2)||(left_c==3))? 2 : 0)	;//black noise => blue or black
				pixels[i-depth_sub] = left_new;
			}
			if (i+depth_add<iScrWidthPix) {
				int right_c = pixels[i+depth_add];//vsgReadPixel(i-depth, j);
				int right_new = (c2)?
					(((right_c==1)||(right_c==3))? 3 : 2)	://purple noise => blue or purple
					(((right_c==1)||(right_c==3))? 1 : 0)	;//black noise => red or black
				pixels[i+depth_add] = right_new;
			}
			pixels[i] = (c2)? 3 : 0;
			
		}
		vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgNOCLEAR);
		vsgDrawPixelLineFast(0,j,pixels,1024);
	}

	//show disparity map
	if (g_pRandomDotAnaglyphDlg->m_ShowDisparity.GetCheck() == BST_CHECKED) {
		for (int i=0; i<256; i++) {
			lutbuffer[i].a=(double)i/255.0;
			lutbuffer[i].b=(double)i/255.0;
			lutbuffer[i].c=(double)i/255.0;
		}
		vsgSetBackgroundColour(&lutbuffer[vsgBACKGROUND]);
		vsgSetFixationColour(&lutbuffer[vsgFIXATION]);
		vsgLUTBUFFERWrite(1,&lutbuffer);
		vsgLUTBUFFERtoPalette(1);
		vsgSetDrawPage(vsgVIDEOPAGE, 2, vsgNOCLEAR);
	}
	
	vsgPresent();

	return true;
}

/***************************************************************
	 StimulusType()
	 -  Inherited method that returns the stimulus type string.
****************************************************************/
CString CRandomDotAnaglyphStimulus::StimulusType() 
{
	return "Random Dot Anaglyph Stimulus";
}

#endif // APP_EYE_TRACKING
