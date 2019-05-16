#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "RFMappingStimulus.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	Analog Out 3:	Target pulse state. +5=on -5=off

************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor
	 -  Initializes the newly instantiated object. 
************************************************************/
CRFMappingStimulus::CRFMappingStimulus() 
{
	Initialize();

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);

	// get screen width and height
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	
	iPage = 0;
	
	// setup digital signals. Trial running (Dig0) is set to high
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	bUpdatePosition = false;
	bDrawStimulus = true;
}

/***********************************************************
	 Destructor
	 -  Destroys the object. Trial running signal turned off
************************************************************/
CRFMappingStimulus::~CRFMappingStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	vsgObjDestroy(g_stim);
	vsgObjDestroy(g_fixation);
	vsgObjDestroy(g_grating);
	vsgObjDestroy(g_stim2);
}

bool CRFMappingStimulus::Initialize() 
{
	bEndThread=false;
		
	blackColor.a = 0;
	blackColor.b = 0;
	blackColor.c = 0;
	
	dotColor.a = ((double)g_pSettings->getRed()) / 255.0f;
	dotColor.b = ((double)g_pSettings->getGreen()) / 255.0f;
	dotColor.c = ((double)g_pSettings->getBlue()) / 255.0f;

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
	
	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);

	double dFrameTime = vsgGetSystemAttribute(vsgFRAMETIME);
	
	dFrameRate = 1000000.0 / dFrameTime;
	g_dHalfFrameInterval = DWORD(floor(dFrameTime / 2000.0f));
	g_dSleepThreshold = (double)g_dHalfFrameInterval / 2.0f;
	g_dFramerate = 1000000.0f / vsgGetSystemAttribute(vsgFRAMETIME);

	// set the view distance
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);
	vsgSetPageWidth(2048);
	
	startX = 0;
	startY = 0;
    	
	iPage = 0;
	
	pulseTimer.StartTimer();
	bPulse = true;
	bPulseState = true;

	return true;	
}

bool CRFMappingStimulus::DisplayStimulus() 
{
	if (bDrawStimulus) {
		if (g_pRFMapping->usePulse() && bPulse)  {
			if (bPulseState) {
				DrawStimulus();
				etu::AnalogOut(3, 5, true);
			} 
			else {
				vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
				vsgPresent();
				etu::AnalogOut(3, -5, true);
				iPage = 1 - iPage;
			}
			bPulse = false;
		} 
		else {
			DrawStimulus();
		}	
	}

	if (bUpdatePosition) {
		double fieldX, fieldY;
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getXPosition(), vsgPIXELUNIT, &fieldX);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getYPosition(), vsgPIXELUNIT, &fieldY);
		vsgMoveScreen(-fieldX, -fieldY);
		
		bUpdatePosition = false;
	}

	if (g_pRFMapping->usePulse() && pulseTimer.CheckTimer() > 150) {
		bPulse = true;
		bPulseState = !bPulseState;
		pulseTimer.CheckTimerWithReset();
		bDrawStimulus = true;
	} 

	Sleep(1);
	return true;
}

void CRFMappingStimulus::DrawStimulus() 
{
	if (g_pRFMapping->useCircle()) {
		DrawCircle();
	} 
	else {
		DrawBar();
	}
	bDrawStimulus = false;
}

void CRFMappingStimulus::DrawBar() 
{
	vsgSetBackgroundColour(&bgColor);
	//Set the current drawing page.
	vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
		
	vsgSetDrawMode(vsgCENTREXY);
	vsgObjDestroy(g_stim);
	g_stim = vsgObjCreate();
	vsgObjSetContrast(100);
	vsgObjSetPixelLevels(2,1);

	vsgSetDrawOrigin(screenPixOffsetX + iScrWidthPix / 2, screenPixOffsetY + iScrHeightPix / 2);

	vsgObjSetColourVector(&blackColor,&dotColor,vsgBIPOLAR);

	// convert the degrees to pixels
	double dSizeH, dSizeV;
	double posX, posY;

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getBarWidth(), vsgPIXELUNIT, &dSizeH);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getBarHeight(), vsgPIXELUNIT, &dSizeV);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getXPosition(), vsgPIXELUNIT, &posX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getYPosition(), vsgPIXELUNIT, &posY);
	
	vsgSetDrawMode(vsgCENTREXY);
	
	float barAngle = g_pRFMapping->getOrientation();
	vsgDrawBar(0, 0, dSizeH, dSizeV, barAngle);
	vsgPresent();
	iPage = 1 - iPage;

	bUpdatePosition = true;
}

void CRFMappingStimulus::DrawCircle() 
{
	vsgSetBackgroundColour(&bgColor);
	//Set the current drawing page.
	vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
	
	vsgSetDrawMode(vsgCENTREXY);
	vsgObjDestroy(g_stim);
	g_stim = vsgObjCreate();
	vsgObjSetContrast(100);
	vsgObjSetPixelLevels(2,1);

	vsgSetDrawOrigin(screenPixOffsetX + iScrWidthPix / 2, screenPixOffsetY + iScrHeightPix / 2);

	vsgObjSetColourVector(&blackColor,&dotColor,vsgBIPOLAR);

	// convert the degrees to pixels
	double dRadius;
	double posX, posY;

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getCircleRadius(), vsgPIXELUNIT, &dRadius);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getXPosition(), vsgPIXELUNIT, &posX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pRFMapping->getYPosition(), vsgPIXELUNIT, &posY);
	
	vsgSetDrawMode(vsgCENTREXY);
		
	vsgDrawOval(0, 0, dRadius*2, dRadius*2);
	vsgPresent();
	iPage = 1 - iPage;
}

void CRFMappingStimulus::updatePosition(float x, float y) 
{
	double degX, degY;

	x  = x * iScrWidthPix - (iScrWidthPix / 2);
	y = y * iScrHeightPix - (iScrHeightPix / 2);

	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, x, vsgDEGREEUNIT, &degX);
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, y, vsgDEGREEUNIT, &degY);
	g_pRFMapping->setXPosition((float)degX);
	g_pRFMapping->setYPosition((float)degY);
	bUpdatePosition = true;
}

void CRFMappingStimulus::updateStimulus(int pixelsX, int pixelsY, bool bLeftButton) 
{
	if (bLeftButton) {
		//change width/height
		double degsDraggedX, degsDraggedY;
		g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, pixelsX, vsgDEGREEUNIT, &degsDraggedX);
		g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, pixelsY, vsgDEGREEUNIT, &degsDraggedY);
		
		if (g_pRFMapping->useBar()) {
			g_pRFMapping->setBarHeight(max(0, g_pRFMapping->getBarHeight() + (float)degsDraggedY));
			g_pRFMapping->setBarWidth(max(0, g_pRFMapping->getBarWidth() + (float)degsDraggedX));
		} 
		else {
			g_pRFMapping->setCircleRadius(max(0, g_pRFMapping->getCircleRadius() + (float)degsDraggedY));
		}
		bDrawStimulus = true;
	} 
	else {
		//change angle
		if (g_pRFMapping->useBar()) {
			g_pRFMapping->setOrientation(g_pRFMapping->getOrientation() + (float)pixelsY / 2);
			bDrawStimulus = true;
		}
	}
}

void CRFMappingStimulus::StopExecution() 
{
	bEndThread = true;
}

CString CRFMappingStimulus::StimulusType() 
{
	return "RF Mapping Stimulus";
}

#endif // APP_EYE_TRACKING
