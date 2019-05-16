/**
 * @file PursuitStimulus.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <fstream>
#include "app/AppCore.h"
#include "PursuitStimulus.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	VSG Out 0:	what??
	VSG Out 1:	??

	Analog Out 0:	Horizontal target location
	Analog Out 1:	Vertical target location

NOTES:
	MKF 11/06/2007 maybe doesn't need to set any digital outputs at all? test.
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor     
	 -  Initializes the newly instantiated object. 
************************************************************/
CPursuitStimulus::CPursuitStimulus() 
{
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	Initialize();
	
	g_pPursuit->sendSettings(g_pSettings->theSerialComm);
}

/*******************************************************************
	Destructor
		- Destroys any objecs that were instantiated on the heap.
		In this case, no objects were created within this stimulus
		thus nothing needs to be done. 
********************************************************************/
CPursuitStimulus::~CPursuitStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	
	//send stop mark
	char buff[80];
	sprintf(buff, "Pursuit;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buff, (int)strlen(buff));
}

/*******************************************************************
	Initialize
		- Initializes the pursuit stimulus. This function overrides
		the Initialize function in CGenericStimulus. This version of
		the function sets the vsgOVERLAYDRIFT and vsgVIDEODRIFT to
		allow for animation through vsgMoveScreen. This function also
		draws the target spot.

********************************************************************/
bool CPursuitStimulus::Initialize() 
{
	double dSize;
	bEndThread=false;
	g_pPursuit->redrawBG=true;
	myTimer = new etu::CTimer();
	dFrameNum = 0;
	VSGTRIVAL bgColor, eccColor;
	double screenXOffset, screenYOffset;
	
	//set the text string
	CString s = g_pPursuit->getTextString();
	strcpy(buffer, s.GetBuffer());
	//set the current display character
	letter[0] = buffer[0];
	letter[1] = 0;

	blackColor.a = 0;
	blackColor.b = 0;
	blackColor.c = 0;
	
	dotColor.a = ((double)g_pSettings->getRed()) / 255.0f;
	dotColor.b = ((double)g_pSettings->getGreen()) / 255.0f;
	dotColor.c = ((double)g_pSettings->getBlue()) / 255.0f;

	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	eccColor.a = ((double)g_pPursuit->getChangeR()) / 255.0f;
	eccColor.b = ((double)g_pPursuit->getChangeG()) / 255.0f;
	eccColor.c = ((double)g_pPursuit->getChangeB()) / 255.0f;

	int CheckCard;
 	
	//Initialise the vsg card then check that it initialised O.K.
	if (!g_bIsInitialized) {
		CheckCard = vsgInit("");
		if (CheckCard < 0) return false;
		g_bIsInitialized = true;
		CheckCard=vsgSetVideoMode(vsgPANSCROLLMODE);
	}

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);

	double dFrameTime = vsgGetSystemAttribute(vsgFRAMETIME);
	
	dFrameRate = 1000000.0 / dFrameTime;
	g_dHalfFrameInterval = DWORD(floor(dFrameTime / 2000.0f));
	g_dSleepThreshold = (double)g_dHalfFrameInterval / 2.0f;
	g_dFramerate = 1000000.0f / vsgGetSystemAttribute(vsgFRAMETIME);

	// set the view distance
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);

	vsgSetBackgroundColour(&bgColor);

	vsgSetDrawOrigin(0, 0);

	//Set the current drawing page.
	vsgSetDrawPage(vsgVIDEOPAGE,0,vsgBACKGROUND);
	vsgSetDrawMode(vsgCENTREXY);

	//-- text
	VSGLUTBUFFER buf;
	int i;
	for (i=0; i<256; i++) {
		buf[i].a = bgColor.a;//((double)0)/255.0f;
		buf[i].b = bgColor.b;//((double)0)/255.0f;
		buf[i].c = bgColor.c;//((double)0)/255.0f;
	}
	buf[2].a = dotColor.a;
	buf[2].b = dotColor.b;
	buf[2].c = dotColor.c;
	buf[4].a = eccColor.a;
	buf[4].b = eccColor.b;
	buf[4].c = eccColor.c;
	vsgSetBackgroundColour(&buf[vsgBACKGROUND]);
	vsgSetFixationColour(&buf[vsgFIXATION]);
	vsgLUTBUFFERWrite(1,&buf);
	vsgLUTBUFFERtoPalette(1);
	//--text

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);
	
	// convert the degrees to pixels
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pPursuit->getSize(), vsgPIXELUNIT, &dSize);
	
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetPen1(2);
	if (!g_pPursuit->getUseText()) {
		vsgDrawOval(iScrWidthPix / 2 + screenXOffset, iScrHeightPix / 2 + screenYOffset, dSize, dSize);
	} 
	else {
		vsgDrawString(iScrWidthPix / 2 + screenXOffset, iScrHeightPix / 2 + screenYOffset, letter);
	}
	
	vsgPresent();

	// make sure that the awakeLGN sequence will allow for eye tracking
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	//vsgUnit2Unit(vsgDEGREEUNIT, g_pPursuit->getAmplitude(), vsgPIXELUNIT, &dAmplitudePix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pPursuit->getAmplitude(), vsgPIXELUNIT, &dAmplitudePix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pPursuit->getHighAmplitude(), vsgPIXELUNIT, &dAmplitudeHighPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &dOffsetPixX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getYOffset(), vsgPIXELUNIT, &dOffsetPixY);
	dCalculatedFreq = g_pPursuit->getFrequency() * 2 * 3.14159265358979323846 / dFrameRate;// * 1.28; //M_PI
	dCalculatedHighFreq = g_pPursuit->getHighFrequency() * 2 * 3.14159265358979323846 / dFrameRate;// * 1.28;//M_PI
	
	cycle = 0;
	prevX = 0;
	prevY = 0;

	return true;	
}

/*******************************************************************
	DisplayStimulus
		- Updates the position of the target spot based on each frame
		based on the current frame number and the type of motion
		specified in the menu system.

********************************************************************/
bool CPursuitStimulus::DisplayStimulus() 
{
	bool debug = false;
	char *logfile = "C:\\Documents and Settings\\HortonAdmin\\Desktop\\debug_eyet.txt";
	
	double x, y;		// variables for the x and y position of the spot in the primary direction
	double x2, y2;      // x and y offsets from the primary path based on the high frequency oscillations
	
	double screenXOffset, screenYOffset;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);

	// if a sinewave oscillation is chosen
	if (g_pPursuit->getWaveform() == CPursuit::WAVEFORM_SINE) {
		// update the x and y coordinates based on the direction of primary motion and the high frequency motion
		switch (g_pPursuit->getOrientation()) {
			case CPursuit::ORIENT_HORIZ:
					x = SineWave(dFrameNum, dCalculatedFreq, dAmplitudePix);
					if (g_pPursuit->isHighFreqEnabled()) {
						y = SineWave(dFrameNum, dCalculatedHighFreq, dAmplitudeHighPix);;
					} 
					else {
						y = 0;
					}
					break;
			case CPursuit::ORIENT_VERT: 
					if (g_pPursuit->isHighFreqEnabled()) {
                        x = SineWave(dFrameNum, dCalculatedHighFreq, dAmplitudeHighPix);;;
					} 
					else {
						x = 0;
					}
					y = SineWave(dFrameNum, dCalculatedFreq, dAmplitudePix);
					break;
			case CPursuit::ORIENT_45DEG: 
					if (g_pPursuit->isHighFreqEnabled()) {
						x2 = SineWave(dFrameNum, dCalculatedHighFreq, dAmplitudeHighPix);
						y2 = -x2;
					} 
					else {
						x2 = 0;
						y2 = 0;
					}

					x = SineWave(dFrameNum, dCalculatedFreq, dAmplitudePix);
					y = x + y2;
					x += x2;	
					break;
			case CPursuit::ORIENT_135DEG: 
					if (g_pPursuit->isHighFreqEnabled()) {
						x2 = SineWave(dFrameNum, dCalculatedHighFreq, dAmplitudeHighPix);
						y2 = x2;
					} 
					else {
						x2 = 0;
						y2 = 0;
					}
					x = SineWave(dFrameNum, dCalculatedFreq, dAmplitudePix);
					y = -x + y2;
					x+=x2;
					break;
		}
	} 
	else {
		// else if tringle wave motion is selected, do the same thing using triangle waves instead of sine.
		switch (g_pPursuit->getOrientation()) {
			case CPursuit::ORIENT_HORIZ:
					x = TriangleWave(dFrameNum, dCalculatedFreq, dAmplitudePix);//g_pPursuit->getAmplitude()//use to match const velocity on the eye
					if (g_pPursuit->isHighFreqEnabled()) {
						y = TriangleWave(dFrameNum, dCalculatedHighFreq, dAmplitudeHighPix);;
					} 
					else {
						y = 0;
					}
					break;
			case CPursuit::ORIENT_VERT: 
					if (g_pPursuit->isHighFreqEnabled()) {
                        x = TriangleWave(dFrameNum, dCalculatedHighFreq, dAmplitudeHighPix);;;
					} 
					else {
						x = 0;
					}
					y = TriangleWave(dFrameNum, dCalculatedFreq, dAmplitudePix);
					break;
			case CPursuit::ORIENT_45DEG: 
					if (g_pPursuit->isHighFreqEnabled()) {
						x2 = TriangleWave(dFrameNum, dCalculatedHighFreq, dAmplitudeHighPix);
						y2 = -x2;
					} 
					else {
						x2 = 0;
						y2 = 0;
					}

					x = TriangleWave(dFrameNum, dCalculatedFreq, dAmplitudePix);
					y = x + y2;
					x += x2;
					break;
			case CPursuit::ORIENT_135DEG: 
					if (g_pPursuit->isHighFreqEnabled()) {
						x2 = TriangleWave(dFrameNum, dCalculatedHighFreq, dAmplitudeHighPix);
						y2 = x2;
					} 
					else {
						x2 = 0;
						y2 = 0;
					}
					x = TriangleWave(dFrameNum, dCalculatedFreq, dAmplitudePix);
					y = -x + y2;
					x+=x2;
					break;
		}
	}

	VSGTRIVAL bgColor, eccColor;

	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	eccColor.a = ((double)g_pPursuit->getChangeR()) / 255.0f;
	eccColor.b = ((double)g_pPursuit->getChangeG()) / 255.0f;
	eccColor.c = ((double)g_pPursuit->getChangeB()) / 255.0f;

	// calculate the position of the dot in degrees
	double xDeg, yDeg;
	
	/*
	//use to match constant velocity on the eye
	xDeg = x;//"convert" x to xDeg
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, xDeg, vsgPIXELUNIT, &x);//put back x*/
	
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, x, vsgDEGREEUNIT, &xDeg);
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, y, vsgDEGREEUNIT, &yDeg);

	if (debug) {
		std::ofstream fout( logfile, std::ios::app );
		fout << dFrameNum << "\t" << x << "\t" << xDeg << std::endl;
		fout.close();
	}

	if (g_pPursuit->getAlternating()) {
		//change color if alternating
		char buffer[80];
		if ( (prevX < 0.0) && (xDeg >= 0.0) ) {
			//transition between cycles now
			cycle = 1-cycle;
			
			//send change txt
			sprintf(buffer, "Change color now;%ld;%ld|0x%02x/\0", 0, cycle, 1+cycle);
			g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
		}
		if (cycle==0) {	//eccColor
			vsgPaletteWrite((VSGLUTBUFFER*)&eccColor, 2,1);
		} 
		else {		//dotColor
			vsgPaletteWrite((VSGLUTBUFFER*)&dotColor, 2,1);
		}

	} 
	else {
		//NOT alternating colors
		if (g_pPursuit->getEccentricity() > 0) {
			//change color of the dot at some eccentricity
			//time to change the color

			double farx = (xDeg<0)? -xDeg : xDeg;
			double fary = (yDeg<0)? -yDeg : yDeg;

			if ((farx > g_pPursuit->getEccentricity()) || (fary > g_pPursuit->getEccentricity())) {
				//eccColor
				vsgPaletteWrite((VSGLUTBUFFER*)&eccColor, 2,1);
			} 
			else {
				//dotColor
				vsgPaletteWrite((VSGLUTBUFFER*)&dotColor, 2,1);
			}
		}
	}

	// move the screen to create the appearance of the dot moving
	vsgMoveScreen(-x-dOffsetPixX, dOffsetPixY + y);
	if (g_pPursuit->getUseText()) {
		// convert the degrees to pixels
		double dSize;
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pPursuit->getSize(), vsgPIXELUNIT, &dSize);
		vsgSetStringMode(0,(int)dSize, vsgALIGNCENTRETEXT, vsgALIGNCENTRETEXT, 0, vsgFONTNORMAL);

		vsgSetPen1(vsgBACKGROUND);//black
		if (g_pPursuit->redrawBG) {
			vsgDrawRect(iScrWidthPix / 2, iScrHeightPix / 2, iScrWidthPix, iScrHeightPix);
			g_pPursuit->redrawBG = false;
		} 
		else {
			vsgDrawRect(iScrWidthPix / 2 + screenXOffset, iScrHeightPix / 2 + screenYOffset, dSize*2, dSize*2);
		}
		vsgSetPen1(2);//white
		vsgSetPen2(vsgBACKGROUND);//black
		letter[0] = buffer[((int)dFrameNum/30)%strlen(buffer)];
		vsgDrawString(iScrWidthPix / 2 + screenXOffset, iScrHeightPix / 2 + screenYOffset, letter);
	}
	
	// update the analog IO.
	etu::AnalogOut(0, (float)((xDeg+g_pSettings->getXOffset()) / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(1, (float)((yDeg +g_pSettings->getYOffset()) / g_pSettings->getDegPerVolt()));
	
	// increment the framenumber
	dFrameNum += 1;

	//save old values
	prevX = xDeg;
	prevY = yDeg;

	// allow the UI thread to run for a brief moment
	Sleep(0);
	return true;
}

/*******************************************************************
	StimulusType
		- Returns a string containing "Pursuit Stimulus" to identify
		the current stimulus.

********************************************************************/
CString CPursuitStimulus::StimulusType() 
{
	return "Pursuit Stimulus";
}
