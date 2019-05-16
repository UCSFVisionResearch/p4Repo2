/**
 * @file FullfieldStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "FullfieldStimulus.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	Analog Out 0:	x voltage
	Analog Out 1:	y voltage
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CFullfieldStimulus::CFullfieldStimulus() 
{
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	iFrameNum = 0;
	Initialize();
	
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);
	
	g_pFullfield->sendSettings(g_pSettings->theSerialComm);
}

CFullfieldStimulus::~CFullfieldStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);

	//send stop mark
	char buffer[80];
	sprintf(buffer, "FullField;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}

/**
 * @brief Apply contrast from midpoint between highlight and lowlight..
 * @param left Floating, between 0 and 1, inclusive.
 * @param right Floating, between 0 and 1, inclusive.
 * @param contrast Between 0 and 100 inclusive.
 */
void CFullfieldStimulus::ApplyContrast(double& highlight, double& lowlight, double contrast)
{
	double halfSpan = (highlight - lowlight) / 2.0;
	double midpoint = lowlight + halfSpan;
	double newSpan = halfSpan * (contrast / 100.0);

	highlight = midpoint + newSpan;
	lowlight = midpoint - newSpan;
}

/**
 * @brief Apply contrast from midpoint between colors.
 */
void CFullfieldStimulus::ApplyContrast(VSGTRIVAL& left, VSGTRIVAL& right, double contrast)
{
	if (left.a > right.a) {
		ApplyContrast(left.a, right.a, contrast);
	}
	else {
		ApplyContrast(right.a, left.a, contrast);
	}

	if (left.b > right.b) {
		ApplyContrast(left.b, right.b, contrast);
	}
	else {
		ApplyContrast(right.b, left.b, contrast);
	}

	if (left.c > right.c) {
		ApplyContrast(left.c, right.c, contrast);
	}
	else {
		ApplyContrast(right.c, left.c, contrast);
	}
}

bool CFullfieldStimulus::Initialize() 
{
	VSGTRIVAL bgColor;
	VSGTRIVAL highlight, lowlight;

	bgColor.a = g_pSettings->getRedBG() / 255.0f;
	bgColor.b = g_pSettings->getGreenBG() / 255.0f;
	bgColor.c = g_pSettings->getBlueBG() / 255.0f;

	dotColor.a = g_pSettings->getRed() / 255.0f;
	dotColor.b = g_pSettings->getGreen() / 255.0f;
	dotColor.c = g_pSettings->getBlue() / 255.0f;

	if (g_pFullfield->getUseGlobalColors()) {
		highlight = dotColor;
		lowlight = bgColor;
	}
	else {
		highlight.a = 1.0f;
		highlight.b = 1.0f;
		highlight.c = 1.0f;

		lowlight.a = 0.0f;
		lowlight.b = 0.0f;
		lowlight.c = 0.0f;
	}

	if (g_pFullfield->getNoise()) {
		/*---------------------------------------------------------------------------------*/
		/*                  drifting noise pattern  moving draw page                       */
		/*---------------------------------------------------------------------------------*/
		//page 0 repeated noise
		//page 1 noise source
		//page 2 fixation point
		//page 3 white/black
		int CheckCard;
	 	//Initialise the vsg card then check that it initialised O.K.
		if (!g_bIsInitialized) {
			CheckCard = vsgInit("");
			if (CheckCard < 0) return false;
			g_bIsInitialized = true;
			CheckCard=vsgSetVideoMode(vsgPANSCROLLMODE);
		}
		// set the view distance
		vsgSetViewDistMM(g_pSettings->getViewDistance());
		vsgSetSpatialUnits(vsgPIXELUNIT);	

		vsgSetBackgroundColour(&bgColor);

		vsgSetDisplayPage(2);
		vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgBACKGROUND);
		vsgSetDrawMode(vsgCENTREXY);
		vsgSetDrawOrigin(0, 0);
		vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);

		/* Calculate contrast, centered around the midpoint between highlight
		 * and lowlight. */
		ApplyContrast(highlight, lowlight, g_pFullfield->getContrast()); 

		vsgMoveScreen(0,0);
		//DrawNonTrialFixation();
		VSGLUTBUFFER lutbuf;
		for (int i=0; i<256; i++) {
			lutbuf[i].a = bgColor.a;
			lutbuf[i].b = bgColor.b;
			lutbuf[i].c = bgColor.c;
		}
		
		lutbuf[1] = dotColor;
		lutbuf[2] = lowlight;
		lutbuf[3] = highlight;
		lutbuf[10].a = ((double)g_pFullfield->getFixationRed()) / 255.0f;
		lutbuf[10].b = ((double)g_pFullfield->getFixationGreen()) / 255.0f;
		lutbuf[10].c = ((double)g_pFullfield->getFixationBlue()) / 255.0f;
		lutbuf[20].a = 0.5;
		lutbuf[20].b = 0.5;
		lutbuf[20].c = 0.5;
		vsgSetBackgroundColour(&lutbuf[vsgBACKGROUND]);
		vsgSetFixationColour(&lutbuf[vsgFIXATION]);
		vsgLUTBUFFERWrite(0, &lutbuf);
		vsgLUTBUFFERtoPalette(0);

		double dFrameTime = vsgGetSystemAttribute(vsgFRAMETIME);
		
		g_dFramerate = 1000000.0 / dFrameTime;
		g_dHalfFrameInterval = DWORD(floor(dFrameTime / 2000.0f));
		g_dSleepThreshold = (double)g_dHalfFrameInterval / 2.0f;
		g_dFramerate = 1000000.0f / vsgGetSystemAttribute(vsgFRAMETIME);

		double angle = 0;
		switch (g_pFullfield->getDriftDirection()) {
			case CFullfield::DIR_UP: angle = 0; xVoltageStep = 0; yVoltageStep = 1; break;
			case CFullfield::DIR_DN: angle = 180; xVoltageStep = 0; yVoltageStep = -1; break;
			case CFullfield::DIR_LT: angle = 90; xVoltageStep = -1; yVoltageStep = 0; break;
			case CFullfield::DIR_RT: angle = 270; xVoltageStep = 1; yVoltageStep = 0; break;
		}

		double cyclesPerSec = g_pFullfield->getDriftVelocity()  / (2 * g_pFullfield->getBarWidth());
		
		//draw screen of noise
		vsgSetDrawPage(vsgVIDEOPAGE, 1, vsgBACKGROUND);
		vsgSetSpatialUnits(vsgPIXELUNIT);	
		//original noise pattern
		vsgDrawNoise(iScrWidthPix/2, iScrHeightPix/2, iScrWidthPix, iScrHeightPix,
			g_pFullfield->getBarWidth(), g_pFullfield->getBarWidth(), 0, 2, 3);

		//now repeat screen of noise so it can be looped
		vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgNOCLEAR);
		vsgSetSpatialUnits(vsgPIXELUNIT);	
		//copy left
		vsgDrawMoveRect(vsgVIDEOPAGE, 1, iScrWidthPix/2, iScrHeightPix/2, iScrWidthPix, iScrHeightPix,
			(iScrWidthPix/2),				(iScrHeightPix/2),		iScrWidthPix, iScrHeightPix);
		//copy right
		vsgDrawMoveRect(vsgVIDEOPAGE, 1, iScrWidthPix/2, iScrHeightPix/2, iScrWidthPix, iScrHeightPix,
			iScrWidthPix+(iScrWidthPix/2),	(iScrHeightPix/2),		iScrWidthPix, iScrHeightPix);
		//lower right
		vsgDrawMoveRect(vsgVIDEOPAGE, 1, iScrWidthPix/2, iScrHeightPix/2, iScrWidthPix, iScrHeightPix,
			iScrWidthPix+(iScrWidthPix/2),	 iScrHeightPix+(iScrHeightPix/2), iScrWidthPix, iScrHeightPix);
		//lower left
		vsgDrawMoveRect(vsgVIDEOPAGE, 1, iScrWidthPix/2, iScrHeightPix/2, iScrWidthPix, iScrHeightPix,
			(iScrWidthPix/2),				 iScrHeightPix+(iScrHeightPix/2), iScrWidthPix, iScrHeightPix);

		//now draw fixation dot
		double offsetX, offsetY, screenOffsetX, screenOffsetY, fixSizePix;
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pFullfield->getFixationSize(), vsgPIXELUNIT, &fixSizePix);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &offsetX);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getYOffset(), vsgPIXELUNIT, &offsetY);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenOffsetX);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenOffsetY);
		vsgSetDrawPage(vsgVIDEOPAGE, 2, vsgBACKGROUND);
		vsgSetBackgroundColour(&bgColor);
		vsgSetPen1(10);
		vsgDrawOval(iScrWidthPix / 2 + offsetX + screenOffsetX, iScrHeightPix / 2 - offsetY - screenOffsetY, fixSizePix, fixSizePix);

		vsgPresent();
		etu::AnalogOut(0, (float)g_pSettings->getXOffset());
		etu::AnalogOut(1, (float)g_pSettings->getYOffset());

		g_dFramerate = 60.0;
		iCycleFrames = (int)(100 / (g_pFullfield->getDriftVelocity() / g_dFramerate));
		dVoltageStep = g_pSettings->getDegPerVolt() / (double)iCycleFrames;
		bShowMotion = false;
		g_pFullfieldDlg->ActivateMotionButton();

		return true;
	} 
	else {
		/*---------------------------------------------------------------------------------*/
		/*                  drifting grating. color table object animation                 */
		/*---------------------------------------------------------------------------------*/
		bEndThread=false;
		myTimer = new etu::CTimer();
		iFrameNum = 0;
				
		int CheckCard;
	 	
		//Initialise the vsg card then check that it initialised O.K.
		if (!g_bIsInitialized) {
			CheckCard = vsgInit("");
			if (CheckCard < 0) return false;
			g_bIsInitialized = true;
			CheckCard=vsgSetVideoMode(vsgPANSCROLLMODE);
		}

		vsgMoveScreen(0,0);

		DrawNonTrialFixation();
			
		vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);

		double dFrameTime = vsgGetSystemAttribute(vsgFRAMETIME);
		
		g_dFramerate = 1000000.0 / dFrameTime;
		g_dHalfFrameInterval = DWORD(floor(dFrameTime / 2000.0f));
		g_dSleepThreshold = (double)g_dHalfFrameInterval / 2.0f;
		g_dFramerate = 1000000.0f / vsgGetSystemAttribute(vsgFRAMETIME);

		// set the view distance
		vsgSetViewDistMM(g_pSettings->getViewDistance());
		vsgSetSpatialUnits(vsgPIXELUNIT);	

		vsgSetBackgroundColour(&bgColor);

		vsgSetDisplayPage(0);
		vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgBACKGROUND);
		vsgSetDrawMode(vsgCENTREXY);
		vsgSetDrawOrigin(0, 0);

		vsgObjDestroy(g_stim);
		vsgObjDestroy(g_fixation);
		g_stim = vsgObjCreate();
		vsgObjSetColourVector(&highlight, &lowlight, vsgBIPOLAR);
		vsgObjSetContrast(g_pFullfield->getContrast());
		vsgObjSetPixelLevels(0, 200);
		
		long tableSize = vsgObjGetTableSize(vsgSWTABLE);
		vsgObjTableSquareWave(vsgSWTABLE, 0, (DWORD)(((double)tableSize) *0.5f));

		double angle = 0;
		switch (g_pFullfield->getDriftDirection()) {
			case CFullfield::DIR_UP: angle = 0; xVoltageStep = 0; yVoltageStep = 1; break;
			case CFullfield::DIR_DN: angle = 180; xVoltageStep = 0; yVoltageStep = -1; break;
			case CFullfield::DIR_LT: angle = 90; xVoltageStep = -1; yVoltageStep = 0; break;
			case CFullfield::DIR_RT: angle = 270; xVoltageStep = 1; yVoltageStep = 0; break;
		}

		double cyclesPerSec = g_pFullfield->getDriftVelocity()  / (2 * g_pFullfield->getBarWidth());
		vsgObjSetDriftVelocity(cyclesPerSec);
		vsgDrawGrating(iScrWidthPix / 2, iScrHeightPix / 2, iScrWidthPix, iScrHeightPix, angle, 1 / (2 * g_pFullfield->getBarWidth()));
		
		vsgPresent();
		if (g_pFullfield->useFixation()) {
			DrawTrialFixation();
		}

		vsgPresent();
		vsgSetDisplayPage(1);

		iCycleFrames = (int)(100 / (g_pFullfield->getDriftVelocity() / g_dFramerate));
		dVoltageStep = g_pSettings->getDegPerVolt() / (double)iCycleFrames;
		bShowMotion = false;
		g_pFullfieldDlg->ActivateMotionButton();
		
		return true;
	}
}

bool CFullfieldStimulus::DisplayStimulus() 
{
	etu::CTimer* FullFieldTimer = new etu::CTimer();
	double xVoltage, yVoltage;
	iFrameNum = 0;
	if (g_pFullfield->getNoise()) {
		/*---------------------------------------------------------------------------------*/
		/*                  drifting noise pattern  moving draw page                       */
		/*---------------------------------------------------------------------------------*/
		if (!bShowMotion) {
			WaitUntil(15);
			return true;
		}
		vsgSetDisplayPage(0);
		//vsgPresent();

		//move screen in direction required
		double x=0, y=0;
		double speed = g_pFullfield->getDriftVelocity();//pixels per frame
		double widthdeg;
		g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, iScrWidthPix, vsgDEGREEUNIT, &widthdeg);
		speed *= iScrWidthPix/widthdeg;//now in pix/s
		speed /= g_dFramerate;

		FullFieldTimer->StartTimer();
		while (FullFieldTimer->CheckTimer() < g_pFullfield->getDurationMSec()) {
			x += xVoltageStep*speed;
			y += yVoltageStep*speed;
			double xbase,ybase;
			xbase = iScrWidthPix/2;
			ybase = (yVoltageStep<0)? iScrHeightPix : 0;
			x = ((int)x)%iScrWidthPix;
			y = ((int)y)%iScrHeightPix;
			//vsgMoveScreen(-x,y);
			vsgMoveScreen(xbase-x,ybase+y);

			xVoltage = ((iFrameNum * dVoltageStep) - (g_pSettings->getDegPerVolt() / 2)) * xVoltageStep * 0.9995;
			yVoltage = ((iFrameNum * dVoltageStep) - (g_pSettings->getDegPerVolt() / 2)) * yVoltageStep * 0.9995;
			
			iFrameNum++;
			if (iFrameNum > iCycleFrames) iFrameNum = 0;

			etu::AnalogOut(0, (float)xVoltage);
			etu::AnalogOut(1, (float)yVoltage);

			if (bEndThread) {
				//CleanUpStimulus();
				return false;
			}

			WaitUntil(0);
		}
		etu::AnalogOut(0, (float)-9.9);
		etu::AnalogOut(1, (float)-9.9);
		//blank period
		FullFieldTimer->CheckTimerWithReset();
		vsgSetDrawPage(vsgVIDEOPAGE, 3, vsgBACKGROUND);
		//vsgSetDisplayPage(3);
		vsgMoveScreen(0,0);
		vsgSetPen1(0);
		vsgDrawRect(iScrWidthPix/2,iScrHeightPix/2,iScrWidthPix,iScrHeightPix);
		vsgPresent();
		WaitUntil(g_pFullfield->getDurationMSec() - FullFieldTimer->CheckTimer());

		//white flash
		vsgSetPen1(20);
		vsgDrawRect(iScrWidthPix/2,iScrHeightPix/2,iScrWidthPix,iScrHeightPix);
		WaitUntil(2000);
		
		vsgSetDisplayPage(2);

		bShowMotion = false;
		bEndThread = true;
	
		return true;
	} 
	else {
		/*---------------------------------------------------------------------------------*/
		/*                  drifting grating. color table object animation                 */
		/*---------------------------------------------------------------------------------*/
		if (!bShowMotion) {
			WaitUntil(15);
			return true;
		}

		VSGTRIVAL bgColor;
		bgColor.a = 0.5;
		bgColor.b = 0.5;
		bgColor.c = 0.5;

		FullFieldTimer->StartTimer();
		while (FullFieldTimer->CheckTimer() < g_pFullfield->getDurationMSec()) {
			vsgSetDisplayPage(0);
			xVoltage = ((iFrameNum * dVoltageStep) - (g_pSettings->getDegPerVolt() / 2)) * xVoltageStep * 0.9995;
			yVoltage = ((iFrameNum * dVoltageStep) - (g_pSettings->getDegPerVolt() / 2)) * yVoltageStep * 0.9995;
			
			etu::AnalogOut(0, (float)xVoltage);
			etu::AnalogOut(1, (float)yVoltage);
			
			iFrameNum++;
			if (iFrameNum > iCycleFrames) iFrameNum = 0;

			if (bEndThread) {
				CleanUpStimulus();
				return false;
			}

			WaitUntil(0);
		}
		vsgSetDisplayPage(1);

		FullFieldTimer->CheckTimerWithReset();

		vsgSetDrawPage(vsgVIDEOPAGE, 1, vsgBACKGROUND);
		vsgSetBackgroundColour(&blackColor);
		vsgPresent();

		etu::AnalogOut(0, (float)-9.9);
		etu::AnalogOut(1, (float)-9.9);
		WaitUntil(g_pFullfield->getDurationMSec() - FullFieldTimer->CheckTimer());
		
		if (bEndThread) {
			CleanUpStimulus();
			return false;
		}

		vsgSetDrawPage(vsgVIDEOPAGE, 2, vsgBACKGROUND);
		vsgSetBackgroundColour(&bgColor);
		vsgPresent();

		WaitUntil(2000);

		CleanUpStimulus();

		return true;
	}
}

void CFullfieldStimulus::CleanUpStimulus() 
{
	vsgObjDestroy(g_fixation);
	vsgObjDestroy(g_stim);

	vsgSetDisplayPage(1);

	DrawNonTrialFixation();
	
	bShowMotion = false;
	bEndThread = true;
}

CString CFullfieldStimulus::StimulusType() 
{
	return "Full Field Stimulus";
}

void CFullfieldStimulus::DrawNonTrialFixation() 
{
	vsgSetBackgroundColour(&blackColor);
	vsgSetDrawPage(vsgVIDEOPAGE, 1, vsgBACKGROUND);
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(0, 0);
	double fixSizePix;

	double offsetX, offsetY, screenOffsetX, screenOffsetY;
	vsgObjDestroy(fixation2);
	fixation2 = vsgObjCreate();
	vsgObjSetContrast(100);
	vsgObjSetPixelLevels(210, 3);

	VSGTRIVAL fixationColor;
	fixationColor.a = ((double)g_pFullfield->getFixationRed()) / 255.0f;
	fixationColor.b = ((double)g_pFullfield->getFixationGreen()) / 255.0f;
	fixationColor.c = ((double)g_pFullfield->getFixationBlue()) / 255.0f;

	vsgObjSetColourVector(&blackColor, &fixationColor, vsgBIPOLAR);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pFullfield->getFixationSize(), vsgPIXELUNIT, &fixSizePix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &offsetX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getYOffset(), vsgPIXELUNIT, &offsetY);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenOffsetX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenOffsetY);
	
	vsgDrawOval(iScrWidthPix / 2 + offsetX + screenOffsetX, iScrHeightPix / 2 - offsetY - screenOffsetY, fixSizePix, fixSizePix);

	vsgPresent();
}

void CFullfieldStimulus::DrawTrialFixation() 
{
	vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgNOCLEAR);
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(0, 0);
	double fixPosXPix, fixPosYPix, fixSizePix;
	double offsetX, offsetY, screenOffsetX, screenOffsetY;
	vsgObjDestroy(g_fixation);
	g_fixation = vsgObjCreate();
	vsgObjSetContrast(100);
	vsgObjSetPixelLevels(201, 3);

	VSGTRIVAL fixationColor;
	fixationColor.a = ((double)g_pFullfield->getFixationRed()) / 255.0f;
	fixationColor.b = ((double)g_pFullfield->getFixationGreen()) / 255.0f;
	fixationColor.c = ((double)g_pFullfield->getFixationBlue()) / 255.0f;

	vsgObjSetColourVector(&blackColor, &fixationColor, vsgBIPOLAR);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pFullfield->getFixationXPos() + g_pSettings->getXOffset(), vsgPIXELUNIT, &fixPosXPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pFullfield->getFixationYPos() + g_pSettings->getYOffset(), vsgPIXELUNIT, &fixPosYPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pFullfield->getFixationSize(), vsgPIXELUNIT, &fixSizePix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &offsetX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getYOffset(), vsgPIXELUNIT, &offsetY);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenOffsetX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenOffsetY);

	vsgDrawOval(iScrWidthPix / 2 + fixPosXPix + screenOffsetX, iScrHeightPix / 2 - fixPosYPix - screenOffsetY, fixSizePix, fixSizePix);
}

void CFullfieldStimulus::ActivateMotion() 
{
	bShowMotion = true;
}

#endif