/**
 * @file RulerStimulus.cpp
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

NOTES:
	MKF 11/06/2007 Implemented a listening UDP socket to read
	network transmissions from Daqer with eye positions.  The
	Spike2 script was very slow to run the UDP send and this
	test never worked very well.  Later, analog input lines
	were connected to the VSG ports to send eye position,
	making this code rather useless.
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor     
	 -  Instantiates the newly created object
************************************************************/
CRulerStimulus::CRulerStimulus() 
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
	lutbuffer[0].a=((double)g_pSettings->getRed())/255.0f;
	lutbuffer[0].b=((double)g_pSettings->getGreen())/255.0f;
	lutbuffer[0].c=((double)g_pSettings->getBlue())/255.0f;
	lutbuffer[2].a=1.0;
	lutbuffer[2].b=1.0;
	lutbuffer[2].c=1.0;
	vsgSetBackgroundColour(&lutbuffer[vsgBACKGROUND]);
	vsgSetFixationColour(&lutbuffer[vsgFIXATION]);
	vsgLUTBUFFERWrite(1,&lutbuffer);
	vsgLUTBUFFERtoPalette(1);

	l_eye_x=0;
	l_eye_y=0;
	r_eye_x=0;
	r_eye_y=0;
	timestamp=0;
}

/*****************************************************************
	 Destructor
	 -  Cleans up when the object is deleted. The trial running
	 signal is turned off.
*****************************************************************/
CRulerStimulus::~CRulerStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
}

/*************************************************************************
	 DisplayStimulus()
	 -  Handles the display of the stimulus. 
*************************************************************************/
bool CRulerStimulus::DisplayStimulus() 
{
	char buff[80];
	double stimXOffset,stimYOffset,screenXOffset,screenYOffset;
	
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix / 2);
	
	iPage = 1-iPage;
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);

	// convert offsets from degrees to pixels
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &stimXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &stimYOffset);

	vsgSetCommand(vsgCLIPENABLE);
	vsgSetClipWindow(0,0,1024,768);

	if (g_pRuler->getBox()==BST_CHECKED) {
		double yyPix,xxPix;
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, 30.0, vsgPIXELUNIT, &yyPix);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, 60.0, vsgPIXELUNIT, &xxPix);
		vsgSetPen1(0);//dot color
		vsgDrawRect(stimXOffset+screenXOffset,screenYOffset+stimYOffset,xxPix,yyPix);

		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, 29.0, vsgPIXELUNIT, &yyPix);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, 59.0, vsgPIXELUNIT, &xxPix);
		vsgSetPen1(1);//bg color
		vsgDrawRect(stimXOffset+screenXOffset,screenYOffset+stimYOffset,xxPix,yyPix);
	} 
	else {
		for (int i=g_pRuler->getNumRings(); i>=1; i--) {
			double r_inner, r_outer;
			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, (i*10.0)-1.0, vsgPIXELUNIT, &r_inner);
			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, i*10.0, vsgPIXELUNIT, &r_outer);

			vsgSetPen1(0);//dot color
			vsgDrawOval(stimXOffset+screenXOffset,screenYOffset+stimYOffset,2*r_outer,2*r_outer);

			vsgSetPen1(1);//bg color
			vsgDrawOval(stimXOffset+screenXOffset,screenYOffset+stimYOffset,2*r_inner,2*r_inner);

			double r_inner_half, r_outer_half;
			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, (i*10.0)-5.25, vsgPIXELUNIT, &r_inner_half);
			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, i*10.0-5.0, vsgPIXELUNIT, &r_outer_half);

			vsgSetPen1(0);//dot color
			vsgDrawOval(stimXOffset+screenXOffset,screenYOffset+stimYOffset,2*r_outer_half,2*r_outer_half);

			vsgSetPen1(1);//bg color
			vsgDrawOval(stimXOffset+screenXOffset,screenYOffset+stimYOffset,2*r_inner_half,2*r_inner_half);

			vsgSetPen1(2);//white
			vsgSetPen2(1);//bg color
			sprintf(buff, "%d\0", i*10);
			vsgDrawString(stimXOffset+screenXOffset+cos(3.1415926/4.0)*r_inner,screenYOffset+stimYOffset-cos(3.1415926/4.0)*r_inner,buff);
		}
	}
	//fixation spot in center
	double a;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, 1.0, vsgPIXELUNIT, &a);
	vsgSetPen1(0);//dot color
	vsgDrawOval(stimXOffset+screenXOffset,screenYOffset+stimYOffset,a,a);

	vsgPresent();

	return true;
}

/***************************************************************
	 StimulusType()
	 -  Inherited method that returns the stimulus type string.
****************************************************************/
CString CRulerStimulus::StimulusType() 
{
	return "Ruler Stimulus";
}

#endif // APP_EYE_TRACKING
