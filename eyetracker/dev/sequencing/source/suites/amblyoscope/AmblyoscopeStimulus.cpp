/**
 * @file AmblyoscopeStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "AmblyoscopeStimulus.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:

ANALOG OUTPUT
CHANNEL		DESC					VALUES
0			x position of center	degrees
1			y position of center	degrees
2			x position of mouse		degrees
3			y position of mouse		degrees
4			fixation target info	neg="blue" 0=off pos="red"
5			mouse target info		neg="blue" 0=off pos="red"

ANALOG INPUT
VSG 0		left eye x				0-4095 see conversion to degrees in EyeTrackingDlg.cpp EyePosSample()
VSG 1		left eye y
VSG 3		right eye x
VSG 4		right eye y

************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor     
	 -  Instantiates the newly created object
************************************************************/
CAmblyoscopeStimulus::CAmblyoscopeStimulus() 
{
	Initialize();

	vsgSetCommand( vsgOVERLAYDRIFT + vsgVIDEODRIFT);
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
	g_pAmblyoscope->sendSettings(g_pSettings->theSerialComm);

	l_ih = NULL;
	r_ih = NULL;
	l_fh = NULL;
	r_fh = NULL;
	l_data = NULL;
	r_data = NULL;
	sprintf(l_name, "\0");
	sprintf(r_name, "\0");

	//draw blank
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	iPage = 1-iPage;
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	vsgPresent();
	refreshPalette();

	iFlashBlocks = 0;

	//draw noise to be copied later
	noisepage = 2;
	vsgSetDrawPage(vsgVIDEOPAGE, noisepage, vsgBACKGROUND);
	vsgDrawNoise(0,0,1024,768,2,2,0,12,3);

	vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix / 2);
	vsgSetCommand(vsgCLIPENABLE);
	vsgSetClipWindow(0,0,1024,768);
}

/*****************************************************************
	 Destructor
	 -  Cleans up when the object is deleted. The trial running
	 signal is turned off.
*****************************************************************/
CAmblyoscopeStimulus::~CAmblyoscopeStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);

	if (l_ih) free(l_ih); l_ih = NULL;
	if (r_ih) free(r_ih); r_ih = NULL;
	if (l_fh) free(l_fh); l_fh = NULL;
	if (r_fh) free(r_fh); r_fh = NULL;
	if (l_data) free(l_data); l_data = NULL;
	if (r_data) free(r_data); r_data = NULL;
	sprintf(l_name, "\0");
	sprintf(r_name, "\0");

	//send stop mark
	char buffer[80];
	sprintf(buffer, "Amblyoscope;Stop;%ld;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}

/*************************************************************************
	 refreshPalette()
	 writes most recent colors to palette
*************************************************************************/
void CAmblyoscopeStimulus::refreshPalette() 
{
	VSGLUTBUFFER lutbuffer;
	VSGTRIVAL bgColor;
	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	for (int i=0; i<256; i++) {
		lutbuffer[i].a=bgColor.a;
		lutbuffer[i].b=bgColor.b;
		lutbuffer[i].c=bgColor.c;
	}
	lutbuffer[0].a=((double)g_pAmblyoscope->getRRed())/255.0f;
	lutbuffer[0].b=((double)g_pAmblyoscope->getRGreen())/255.0f;
	lutbuffer[0].c=((double)g_pAmblyoscope->getRBlue())/255.0f;
	lutbuffer[1].a=((double)g_pAmblyoscope->getLRed())/255.0f;
	lutbuffer[1].b=((double)g_pAmblyoscope->getLGreen())/255.0f;
	lutbuffer[1].c=((double)g_pAmblyoscope->getLBlue())/255.0f;
	lutbuffer[2].a=((double) ((g_pAmblyoscope->getLRed()+g_pAmblyoscope->getRRed())%256) )/255.0f;
	lutbuffer[2].b=((double) ((g_pAmblyoscope->getLGreen()+g_pAmblyoscope->getRGreen())%256) )/255.0f;
	lutbuffer[2].c=((double) ((g_pAmblyoscope->getLBlue()+g_pAmblyoscope->getRBlue())%256) )/255.0f;
	lutbuffer[3].a=bgColor.a;
	lutbuffer[3].b=bgColor.b;
	lutbuffer[3].c=bgColor.c;
	//...
	double pct = ((double)g_pAmblyoscope->getBGBrightness())/100.0;
	lutbuffer[10].a=((double) ((g_pAmblyoscope->getLRed()+g_pAmblyoscope->getRRed())%256) )/255.0f;
	lutbuffer[10].b=(pct*(double) ((g_pAmblyoscope->getLGreen()+g_pAmblyoscope->getRGreen())%256) )/255.0f;
	lutbuffer[10].c=(pct*(double) ((g_pAmblyoscope->getLBlue()+g_pAmblyoscope->getRBlue())%256) )/255.0f;
	lutbuffer[11].a=(pct*(double) ((g_pAmblyoscope->getLRed()+g_pAmblyoscope->getRRed())%256) )/255.0f;
	lutbuffer[11].b=(pct*(double) ((g_pAmblyoscope->getLGreen()+g_pAmblyoscope->getRGreen())%256) )/255.0f;
	lutbuffer[11].c=((double) ((g_pAmblyoscope->getLBlue()+g_pAmblyoscope->getRBlue())%256) )/255.0f;
	lutbuffer[12].a=(pct*(double) ((g_pAmblyoscope->getLRed()+g_pAmblyoscope->getRRed())%256) )/255.0f;
	lutbuffer[12].b=(pct*(double) ((g_pAmblyoscope->getLGreen()+g_pAmblyoscope->getRGreen())%256) )/255.0f;
	lutbuffer[12].c=(pct*(double) ((g_pAmblyoscope->getLBlue()+g_pAmblyoscope->getRBlue())%256) )/255.0f;
	lutbuffer[13].a=bgColor.a;
	lutbuffer[13].b=bgColor.b;
	lutbuffer[13].c=bgColor.c;
	
	vsgSetBackgroundColour(&lutbuffer[vsgBACKGROUND]);
	vsgSetFixationColour(&lutbuffer[vsgFIXATION]);
	vsgLUTBUFFERWrite(1,&lutbuffer);
	vsgLUTBUFFERtoPalette(1);
}

/*************************************************************************
	 DisplayStimulus()
	 -  Handles the display of the stimulus. 
*************************************************************************/
bool CAmblyoscopeStimulus::DisplayStimulus() 
{
	iPage = 1-iPage;
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);

	if (g_pAmblyoscope->getBackground()) {
		vsgDrawMoveRect(vsgVIDEOPAGE, noisepage, 0, 0, 1024, 768, 0, 0, 1024, 768);
	}

	double stimXOffset, stimYOffset, screenXOffset, screenYOffset, picXOffset, picYOffset;
	
	// convert offsets from degrees to pixels
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &stimXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &stimYOffset);

	picXOffset = stimXOffset + g_pAmblyoscope->getXOffset();
	picYOffset = stimYOffset + g_pAmblyoscope->getYOffset();
	double xd, yd;
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, picXOffset, vsgDEGREEUNIT, &xd);
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, picYOffset, vsgDEGREEUNIT, &yd);

	//gaze positions
	double lxdeg = g_pEyeTrackingDlg->avglx;//-(vsgIOReadADC(0) -2047.0)/10.0;
	double lydeg = g_pEyeTrackingDlg->avgly;//-(vsgIOReadADC(1) -2047.0)/10.0;
	double rxdeg = g_pEyeTrackingDlg->avgrx;//-(vsgIOReadADC(3) -2047.0)/10.0;
	double rydeg = g_pEyeTrackingDlg->avgry;//-(vsgIOReadADC(4) -2047.0)/10.0;
	double lx=0, ly=0, rx=0, ry=0;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, lxdeg, vsgPIXELUNIT, &lx);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, lydeg, vsgPIXELUNIT, &ly);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, rxdeg, vsgPIXELUNIT, &rx);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, rydeg, vsgPIXELUNIT, &ry);

	double fixx=0, fixy=0, fixs=0, centerXOffset, centerYOffset;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAmblyoscope->getFixationX(), vsgPIXELUNIT, &fixx);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pAmblyoscope->getFixationY(), vsgPIXELUNIT, &fixy);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAmblyoscope->getFixationSize(), vsgPIXELUNIT, &fixs);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAmblyoscope->getCenterX(), vsgPIXELUNIT, &centerXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pAmblyoscope->getCenterY(), vsgPIXELUNIT, &centerYOffset);
	int dotcolor = 0;
	if (g_pAmblyoscope->getFixationDot()) {
		if (g_pAmblyoscope->getFlashCenter()==0) {//red
			dotcolor = 0;
		} 
		else if (g_pAmblyoscope->getFlashCenter()==1) {//blue
			dotcolor = 1;
		} 
		else if (g_pAmblyoscope->getFlashCenter()==2) {//random
			dotcolor=(rand()%2)==0;
		} 
		else {
			if (iFlashBlocks >= 2*g_pAmblyoscope->getFlashBlocks()) {//blocks
				iFlashBlocks = 0;//reset
			}
			dotcolor = iFlashBlocks < g_pAmblyoscope->getFlashBlocks();//first half vs second half
			
			iFlashBlocks++;
		}

		vsgSetPen1(dotcolor);
		vsgDrawOval(stimXOffset+screenXOffset+fixx, stimYOffset+screenYOffset+fixy, fixs, fixs);
	}

	if (g_pAmblyoscope->getFlashTest()) {
		//run test without mouse movement
		//				center orientation
		//center color	(peripheral is opposite)	periph color	periph location
		//----------	-----------------------		------------	---------------
		//monocular trials
		// R			 horiz						 R				 center
		// R			 horiz						 R				 periphery
		// R			 vert						 R				 center
		// R			 vert						 R				 periphery
		// B			 horiz						 B				 center
		// B			 horiz						 B				 periphery
		// B			 vert						 B				 center
		// B			 vert						 B				 periphery
		//binocular trials
		// R			 horiz						 B				 center
		// R			 horiz						 B				 periphery
		// R			 vert						 B				 center
		// R			 vert						 B				 periphery
		// B			 horiz						 R				 center
		// B			 horiz						 R				 periphery
		// B			 vert						 R				 center
		// B			 vert						 R				 periphery

		int redx = 0;
		int redy = 0;
		char redimg[1024];
		int bluex = 0;
		int bluey = 0;
		char blueimg[1024];
		double redPeriphX=0, redPeriphY=0, bluePeriphX=0, bluePeriphY=0;
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAmblyoscope->getDeviatedRightX(), vsgPIXELUNIT, &redPeriphX);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pAmblyoscope->getDeviatedRightY(), vsgPIXELUNIT, &redPeriphY);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pAmblyoscope->getDeviatedLeftX(), vsgPIXELUNIT, &bluePeriphX);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pAmblyoscope->getDeviatedLeftY(), vsgPIXELUNIT, &bluePeriphY);

		bool centerRed;//primary arm of the cross at the center should be red?
		if (g_pAmblyoscope->getFlashCenter()==0) {//red
			centerRed=true;
		} 
		else if (g_pAmblyoscope->getFlashCenter()==1) {//blue
			centerRed=false;
		} 
		else if (g_pAmblyoscope->getFlashCenter()==2) {//random
			centerRed=(rand()%2)==0;
		} 
		else {
			if (iFlashBlocks >= 2*g_pAmblyoscope->getFlashBlocks()) {//blocks
				iFlashBlocks = 0;//reset
			}
			centerRed = iFlashBlocks < g_pAmblyoscope->getFlashBlocks();//first half vs second half
			
			iFlashBlocks++;
		}
		bool redHoriz = (rand()%2)==0;//red arm should be horizontal?
		bool peripheral = (rand()%2)==0;//secondary arm should be in th periphery?
		bool monocular = rand()%100 < g_pAmblyoscope->getPctMonocular()-1;//seconday arm should be same color as primary?
		int code = 10 + ((centerRed)? 1:0) + ((redHoriz)? 2:0) + ((peripheral)? 4:0) + ((monocular)? 8:0);
		
		bool monocularNonCross = false;

		if (redHoriz) {
			sprintf(redimg, "C:\\Stimulus Settings\\Amblyoscope\\segment_h.bmp\0");
			sprintf(blueimg, "C:\\Stimulus Settings\\Amblyoscope\\segment_v.bmp\0");
		} 
		else {//blue is horiz
			sprintf(redimg, "C:\\Stimulus Settings\\Amblyoscope\\segment_v.bmp\0");
			sprintf(blueimg, "C:\\Stimulus Settings\\Amblyoscope\\segment_h.bmp\0");
		}

		if (monocular) {
			if (centerRed) {
				redx = (int)screenXOffset;
				redy = (int)screenYOffset;
				if (peripheral) {
					bluex = screenXOffset + bluePeriphX;
					bluey = screenYOffset + bluePeriphY;
					//if (redHoriz) { ... must use two calls to original draw image
					monocularNonCross = true;
				} 
				else {//form a cross on screen
					bluex = screenXOffset;
					bluey = screenYOffset;
					//if (redHoriz) {} else {}
					
					sprintf(redimg, "C:\\Stimulus Settings\\Amblyoscope\\segment_mono.bmp\0");
					sprintf(blueimg, "C:\\Stimulus Settings\\Amblyoscope\\segment_blank.bmp\0");
				}
			} 
			else {//center blue
				bluex = screenXOffset;
				bluey = screenYOffset;
				if (peripheral) {
					redx = screenXOffset + redPeriphX;
					redy = screenYOffset + redPeriphY;
					//if (redHoriz) { ... must use two calls to original draw image
					monocularNonCross = true;
				} 
				else {//form a cross on screen
					redx = screenXOffset;
					redy = screenYOffset;
					//if (redHoriz) {} else {}
					
					sprintf(redimg, "C:\\Stimulus Settings\\Amblyoscope\\segment_blank.bmp\0");
					sprintf(blueimg, "C:\\Stimulus Settings\\Amblyoscope\\segment_mono.bmp\0");
				}
			}
		} 
		else {//binocular
			if (centerRed) {
				redx = screenXOffset;
				redy = screenYOffset;
				if (peripheral) {
					bluex = screenXOffset + bluePeriphX;
					bluey = screenYOffset + bluePeriphY;
				} 
				else {//form a cross on screen
					bluex = screenXOffset;
					bluey = screenYOffset;
				}
			} 
			else {//center blue
				bluex = screenXOffset;
				bluey = screenYOffset;
				if (peripheral) {
					redx = screenXOffset + redPeriphX;
					redy = screenYOffset + redPeriphY;
				} 
				else {//form a cross on screen
					redx = screenXOffset;
					redy = screenYOffset;
				}
			}
		}
		if (monocularNonCross) {
			//use center color for both red and blue. red==0, blue==1
			drawBitmapImage( ((centerRed)? 0:1), redx, redy, redimg );
			drawBitmapImage( ((centerRed)? 0:1), bluex, bluey, blueimg );
		} 
		else {
			drawBitmapImage2( bluex, bluey, blueimg, redx, redy, redimg );
		}

		//send text mark
		char buffer[80];
		sprintf(buffer, "centrRed:%d redHrz:%d periph:%d monc:%d|0x%02x/\0", centerRed, redHoriz, peripheral, monocular, 0xff & code);
		g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
	} 
	else {
		int length = g_pAmblyoscope->getArmLength();
		int width = g_pAmblyoscope->getArmWidth();
		int space = g_pAmblyoscope->getSpaceSize();
		if (g_pAmblyoscopeDlg->m_Fovea.GetCheck()==BST_CHECKED) {
			//gaze contingent display
			if (g_pAmblyoscopeDlg->righton) {
				if (!g_pAmblyoscope->getAlternate() ||
					(((iPage%2) == 0) && g_pAmblyoscope->getAlternate())) 
				{
					drawAxis(screenXOffset+lx, screenYOffset+ly,
						length, width, space,
						g_pAmblyoscopeDlg->flip, !(g_pAmblyoscopeDlg->flip));
				}
			}
			if (g_pAmblyoscopeDlg->lefton) {
				if (!g_pAmblyoscope->getAlternate() ||
					(((iPage%2) == 1) && g_pAmblyoscope->getAlternate())) 
				{
					drawAxis(screenXOffset+rx, screenYOffset+ry,
						length, width, space,
						!(g_pAmblyoscopeDlg->flip), g_pAmblyoscopeDlg->flip);
				}
			}
		} 
		else {
			//normal
			if (g_pAmblyoscopeDlg->righton) {
				if (!g_pAmblyoscope->getAlternate() ||
					(((iPage%2) == 0) && g_pAmblyoscope->getAlternate())) 
				{
					drawAxis(screenXOffset+centerXOffset, screenYOffset+centerYOffset,
						length, width, space,
						g_pAmblyoscopeDlg->flip, !(g_pAmblyoscopeDlg->flip));
				}
			}
			if (g_pAmblyoscopeDlg->lefton) {
				if (!g_pAmblyoscope->getAlternate() ||
					(((iPage%2) == 1) && g_pAmblyoscope->getAlternate())) 
				{
					drawAxis(screenXOffset+picXOffset, screenYOffset+picYOffset,
						length, width, space,
						!(g_pAmblyoscopeDlg->flip), g_pAmblyoscopeDlg->flip);
				}
			}
		}

		//display real dimensions
		double dArmLength=0, dArmWidth=0;
		g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, width, vsgDEGREEUNIT, &dArmWidth);
		g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, length*2+space, vsgDEGREEUNIT, &dArmLength);
		char buffer[80];
		sprintf(buffer, "length: %.2f deg\nwidth: %.2f deg\0", dArmLength, dArmWidth);
		g_pAmblyoscopeDlg->m_ArmStatic.SetWindowText(CString(buffer));

		if (false) {
			//debug color palette
			for (int i=0; i<256; i++) {
				vsgSetPen1(i);
				vsgDrawRect(i*iScrWidthPix/256+iScrWidthPix/512-iScrWidthPix/2,iScrHeightPix/2-9,iScrWidthPix/256,20);
			}
		}

		//output position of mouse thing to 1401
		etu::AnalogOut(2, (float)(xd/10.0), true);//mouse x
		etu::AnalogOut(3, (float)(-yd/10.0), true);//mouse y
		etu::AnalogOut(0, (float)(g_pAmblyoscope->getCenterX()/10.0), true);//center of stationary cross arm x
		etu::AnalogOut(1, (float)(g_pAmblyoscope->getCenterY()/10.0), true);//center of stationary cross arm y
		if (g_pAmblyoscopeDlg->flip) {
			etu::AnalogOut(5, (float)(  (g_pAmblyoscopeDlg->lefton)? 5.0 : 0 ), true);//fixation target
			etu::AnalogOut(4, (float)(  (g_pAmblyoscopeDlg->righton)? -5.0 : 0  ), true);//mouse target
		} 
		else {
			etu::AnalogOut(4, (float)(  (g_pAmblyoscopeDlg->righton)? 5.0 : 0 ), true);//mouse target
			etu::AnalogOut(5, (float)(  (g_pAmblyoscopeDlg->lefton)? -5.0 : 0  ), true);//fixation target
		}

	}
	//draw and delay
	refreshPalette();//before vsgPresent()
	vsgPresent();
	//refreshPalette();//after vsgPresent()
	etu::CTimer rolex;
	rolex.StartTimer();
	while (	(rolex.CheckTimer() < g_pAmblyoscope->getFlashDuration()) && !bEndThread) {
		g_pEyeTrackingDlg->EyePosSample();
		WaitUntil(10);
	}//Sleep(10)

	if ((g_pAmblyoscope->getBlankDuration() > 0) && !g_pAmblyoscope->getAlternate()) {
		//draw a blank page
		iPage = 1-iPage;
		vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);

		//background?
		if (g_pAmblyoscope->getBackground()) {
			vsgDrawMoveRect(vsgVIDEOPAGE, noisepage, 0, 0, 1024, 768, 0, 0, 1024, 768);
		}

		//draw fixation dot?
		if (g_pAmblyoscope->getFixationDot()) {
			vsgSetPen1(dotcolor);
			vsgDrawOval(stimXOffset+screenXOffset+fixx, stimYOffset+screenYOffset+fixy, fixs, fixs);
		}

		vsgPresent();

		etu::AnalogOut(2, -10.0, true);
		etu::AnalogOut(3, -10.0, true);

		//wait
		rolex.CheckTimerWithReset();
		while (	(rolex.CheckTimer() < g_pAmblyoscope->getBlankDuration()) && !bEndThread) {
			g_pEyeTrackingDlg->EyePosSample();
			WaitUntil(10);
		}
	}

	char buffer[1024];
	sprintf(buffer, "Left Eye X (deg): %.3f\nLeft Eye Y (deg): %.3f\nRight Eye X (deg): %.3f\nRight Eye Y (deg): %.3f\0", lxdeg, lydeg, rxdeg, rydeg);
	g_pAmblyoscopeDlg->m_EyePositions.SetWindowText(CString(buffer));

	return true;
}

int CAmblyoscopeStimulus::drawBitmapImage(int color, int x, int y, char *imagefile) 
{
	bool debug = false;
	char buffer[1024];
	int total = 0;
	int i,j,w,h;

	if (debug) {
		sprintf(buffer, "start. color=%d\0", color);
		AfxMessageBox(buffer, 0, 0);
	}

	//read image into array
	BITMAPINFOHEADER *ih;
	BITMAPFILEHEADER *fh;
	char *data;
	if (color==0) {
		//red
		if (strcmp(imagefile, r_name) != 0) {
			if (r_ih==NULL) r_ih = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
			if (r_fh==NULL) r_fh = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
			if (r_data) free(r_data);
			FILE* fp = fopen(imagefile, "rb");
			total += (int)fread(r_fh, sizeof(byte), sizeof(BITMAPFILEHEADER), fp);
			total += (int)fread(r_ih, sizeof(byte), sizeof(BITMAPINFOHEADER), fp);
			r_data = (char*)malloc(r_ih->biSizeImage);
			fread(r_data, sizeof(byte), r_ih->biSizeImage, fp);
			fclose(fp);
			sprintf(r_name, "%s\0", imagefile);
		}
		ih = r_ih;
		fh = r_fh;
		data = r_data;
		w = r_ih->biWidth;
		h = r_ih->biHeight;
	} 
	else {
		//blue
		if (strcmp(imagefile, l_name) != 0) {
			if (l_ih==NULL) l_ih = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
			if (l_fh==NULL) l_fh = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
			if (l_data) free(l_data);
			FILE* fp = fopen(imagefile, "rb");
			total += (int)fread(l_fh, sizeof(byte), sizeof(BITMAPFILEHEADER), fp);
			total += (int)fread(l_ih, sizeof(byte), sizeof(BITMAPINFOHEADER), fp);
			l_data = (char*)malloc(l_ih->biSizeImage);
			fread(l_data, sizeof(byte), l_ih->biSizeImage, fp);
			fclose(fp);
			sprintf(l_name, "%s\0", imagefile);
		}
		ih = l_ih;
		fh = l_fh;
		data = l_data;
		w = l_ih->biWidth;
		h = l_ih->biHeight;
	}
	//w = ih->biWidth;
	//h = ih->biHeight;

	if (debug) {
		sprintf(buffer, "got data. w=%d h=%d total=%d size=%d\0", w, h, total, fh->bfSize);
		AfxMessageBox(buffer, 0, 0);
	}

	//draw nonzero pixels in specified color channel
	vsgSetPen1(color);
	//vsgDrawRect(x, y, ih.biWidth+2, ih.biHeight+2);
	//for (i=0; i<ih.biWidth; i++) {
	//	for (j=0; j<ih.biHeight; j++) {
	for (i=-w/2; i<w/2; i++) {
		for (j=-h/2; j<h/2; j++) {
			//char pixelr = data[((ih.biHeight-1-(j+ih.biHeight/2))*ih.biWidth + i+ih.biWidth/2)*3];
			//char pixelg = data[((ih.biHeight-1-(j+ih.biHeight/2))*ih.biWidth + i+ih.biWidth/2)*3+1];
			//char pixelb = data[((ih.biHeight-1-(j+ih.biHeight/2))*ih.biWidth + i+ih.biWidth/2)*3+2];
			char pixelr = (unsigned)(byte)data[((h-1-(j+h/2))*w + i+w/2)*3+2];
			char pixelg = (unsigned)(byte)data[((h-1-(j+h/2))*w + i+w/2)*3+1];
			char pixelb = (unsigned)(byte)data[((h-1-(j+h/2))*w + i+w/2)*3];
			int p = vsgReadPixel(x+i,y+j);//might be able to eliminate this call
			if (p > 1) {// == 253
				if (pixelr+pixelg+pixelb == 0) {
					vsgSetPen1(3);//0 on bg = nothing
				} 
				else {
					vsgSetPen1(color);//color on bg = color
				}
			} 
			else {
				if (pixelr+pixelg+pixelb == 0) {
					vsgSetPen1(p);//0 on p = p
				} 
				else {
					vsgSetPen1(2);//color on p = purple
				}
			}
			//vsgDrawPixel(1024/2+x+i,768/2+y+j);
			vsgDrawPixel(x+i,y+j);
		}
	}

	if (debug) {
		sprintf(buffer, "end");
		AfxMessageBox(buffer, 0, 0);
	}

	return 0;
}

int CAmblyoscopeStimulus::drawBitmapImage2(int x, int y, char *l_imagefile, int x2, int y2, char *r_imagefile) 
{
	int total = 0;
	int i,j,w,h,i2,j2;

	//read image into array
	//red
	if (strcmp(r_imagefile, r_name) != 0) {
		if (r_ih==NULL) r_ih = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
		if (r_fh==NULL) r_fh = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
		if (r_data) free(r_data);
		FILE* fp = fopen(r_imagefile, "rb");
		if (!fp) return -1;
		total += (int)fread(r_fh, sizeof(byte), sizeof(BITMAPFILEHEADER), fp);
		total += (int)fread(r_ih, sizeof(byte), sizeof(BITMAPINFOHEADER), fp);
		r_data = (char*)malloc(r_ih->biSizeImage);
		fread(r_data, sizeof(byte), r_ih->biSizeImage, fp);
		fclose(fp);
		sprintf(r_name, "%s\0", r_imagefile);
	}
	if (!(g_pAmblyoscopeDlg->righton)) {
		memset(r_data, 0, r_ih->biSizeImage);
		sprintf(r_name, " \0");
	}
	//blue
	if (strcmp(l_imagefile, l_name) != 0) {
		if (l_ih==NULL) l_ih = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
		if (l_fh==NULL) l_fh = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
		if (l_data) free(l_data);
		FILE* fp = fopen(l_imagefile, "rb");
		if (!fp) return -1;
		total += (int)fread(l_fh, sizeof(byte), sizeof(BITMAPFILEHEADER), fp);
		total += (int)fread(l_ih, sizeof(byte), sizeof(BITMAPINFOHEADER), fp);
		l_data = (char*)malloc(l_ih->biSizeImage);
		fread(l_data, sizeof(byte), l_ih->biSizeImage, fp);
		fclose(fp);
		sprintf(l_name, "%s\0", l_imagefile);
	}
	if (!(g_pAmblyoscopeDlg->lefton)) {
		memset(l_data, 0, l_ih->biSizeImage);
		sprintf(l_name, " \0");
	}

	//draw nonzero pixels in specified color channel
	
	int startx1 = x - (l_ih->biWidth)/2;	int endx1 = x + (l_ih->biWidth)/2;
	int startx2 = x2- (r_ih->biWidth)/2;	int endx2 = x2+ (r_ih->biWidth)/2;
	int starty1 = y - (l_ih->biHeight)/2;	int endy1 = y + (l_ih->biHeight)/2;
	int starty2 = y2- (r_ih->biHeight)/2;	int endy2 = y2+ (r_ih->biHeight)/2;
	int startx = ( (startx1<startx2)? startx1 : startx2 );
	int endx = ( (endx1>endx2)? endx1 : endx2 );
	int starty = ( (starty1<starty2)? starty1 : starty2 );
	int endy = ( (endy1>endy2)? endy1 : endy2 );

	for (i=startx; i<endx; i++) {
		for (j=starty; j<endy; j++) {
			bool red = (i >= startx2) && (i < endx2) && (j >= starty2) && (j < endy2);//in red picture's area?
			bool blue = (i >= startx1) && (i < endx1) && (j >= starty1) && (j < endy1);//in blue picture's area?
			if (red || blue) {
				w = r_ih->biWidth;
				h = r_ih->biHeight;
				i2 = i-startx2;//image based coords
				j2 = j-starty2;
				char pixelr_r = (red)? (unsigned)(byte)r_data[((h-1-j2)*w + i2)*3+2] : 0;
				char pixelr_g = (red)? (unsigned)(byte)r_data[((h-1-j2)*w + i2)*3+1] : 0;
				char pixelr_b = (red)? (unsigned)(byte)r_data[((h-1-j2)*w + i2)*3] : 0;
				w = l_ih->biWidth;
				h = l_ih->biHeight;
				i2 = i-startx1;//image based coords
				j2 = j-starty1;
				char pixell_r = (blue)? (unsigned)(byte)l_data[((h-1-j2)*w + i2)*3+2] : 0;
				char pixell_g = (blue)? (unsigned)(byte)l_data[((h-1-j2)*w + i2)*3+1] : 0;
				char pixell_b = (blue)? (unsigned)(byte)l_data[((h-1-j2)*w + i2)*3] : 0;

				//new
				int bgVal = (vsgReadPixel(i,j)==12)? 10 : 0;
				//new

				if (pixelr_r+pixelr_g+pixelr_b == 0) {
					//no red
					if (pixell_r+pixell_g+pixell_b == 0) {
						vsgSetPen1(3);//no blue, show nothing
					} else {
						vsgSetPen1(bgVal+((g_pAmblyoscopeDlg->flip)? 0 : 1));//blue, show blue unless flipped
						vsgDrawPixel(i,j);
					}
				} else {
					//red
					if (pixell_r+pixell_g+pixell_b == 0) {
						vsgSetPen1(bgVal+((g_pAmblyoscopeDlg->flip)? 1 : 0));//no blue, show red unless flipped
						vsgDrawPixel(i,j);
					} else {
						vsgSetPen1(2);//blue, show purple
						vsgDrawPixel(i,j);
					}
				}
				//vsgDrawPixel(i,j);
			}
		}
	}

	return 0;
}

/*
draws one axis with a space in the middle
like a propellor blade
*/
void CAmblyoscopeStimulus::drawAxis(int x, int y, int seglength, int segwidth, int segspace, bool bRed, bool horizontal) 
{
	bool debug = false;
	for (int i=0; i<2*seglength+segspace; i++) {//along length of axis
		for (int j=0; j<segwidth; j++) {//along width of blade
			int drawx = (horizontal)?
				x + seglength + (int)floor(segspace/2.0) - i	:
				x + (int)floor(segwidth/2.0) - j;
			int drawy = (horizontal)?
				y + (int)floor(segwidth/2.0) - j	:
				y + seglength + (int)floor(segspace/2.0) - i;
			bool spaceSegment = (i >= seglength) && (i < seglength+segspace);
			//if ( (drawx >= 0) && (drawx < iScrWidthPix) && (drawy >= 0) && (drawy > iScrHeightPix) ) {
			if ( (drawx > -iScrWidthPix/2) && (drawx < iScrWidthPix/2) && (drawy > -iScrHeightPix/2) && (drawy < iScrHeightPix/2) ) {
				int p = vsgReadPixel(drawx,drawy);
				int now = (bRed)? 0 : 1;
				//0 red
				//1 blue
				//2 purple
				//3 blank

				//todo revise color selection code here
				//conditions:
				//nothing on anything
				//color on same color
				//color on opposite color
				//color on purple
				//color on nothing

				if (spaceSegment) {
					//nothing on whatever
					vsgSetPen1(p);
				} 
				else {
					if (p == now) {
						//color on same
						vsgSetPen1(p);
					} 
					else if (p < 2) {
						//color on opposite
						vsgSetPen1(2);
					} 
					else if (p == 2){
						//color on purple
						vsgSetPen1(2);
					} 
					else {
						//color on bg
						vsgSetPen1(now);
					}
				}
				
				vsgDrawPixel(drawx,drawy);
			} 
			else {
				if (debug) {
					char buffer[1024];
					sprintf(buffer, "off screen %d,%d", drawx, drawy);
					AfxMessageBox(buffer, 0, 0);
					//AfxMessageBox("off screen");
				}
			}
		}
	}
	if (debug) {
		vsgSetPen1((bRed)? 0 : 1);
		vsgDrawOval(x,y,seglength,seglength);
	}
}

/***************************************************************
	 StimulusType()
	 -  Inherited method that returns the stimulus type string.
****************************************************************/

CString CAmblyoscopeStimulus::StimulusType() 
{
	return "Amblyoscope Stimulus";
}

#endif // APP_EYE_TRACKING