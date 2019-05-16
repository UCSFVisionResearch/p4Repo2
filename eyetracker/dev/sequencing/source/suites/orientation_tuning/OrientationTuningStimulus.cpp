/**
 * @file OrientationTuningStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "OrientationTuningStimulus.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	Analog Out 0:	Horizontal target location
	Analog Out 1:	Vertical target location
	Analog Out 2:	moving bar x
	Analog Out 3:	moving bar y
	Analog Out 4:	Trial phase 5=blank -5=intertrial 0=trial
	Analog Out 5:	angle

NOTES
	MKF 11/06/2007 reused analog output 3? needs a full test
	MKF 11/09/2007 moved "trial phase" to analog out 4 to avoid
		conflict with "moving bar y"
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

COrientationTuningStimulus::COrientationTuningStimulus() 
{
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	Initialize();
	pTuningRuns = NULL;
	numTuningRuns = 0;

	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);
}

COrientationTuningStimulus::~COrientationTuningStimulus() 
{
	if (pTuningRuns) delete [] pTuningRuns;
	pTuningRuns = NULL;
	
	if (myTimer) delete myTimer;

	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
}

// generic vsg initialization. 
// All overriding methods must set bEndThread to false, and create a new myTimer.
bool COrientationTuningStimulus::Initialize() 
{
	bEndThread=false;
	myTimer = new etu::CTimer();
	
	// set trial phase to intertrial
	etu::AnalogOut(4, -5, true);
	
	blackColor.a = 0;
	blackColor.b = 0;
	blackColor.c = 0;
	
	dotColor.a = ((double)g_pSettings->getRed()) / 255.0f;//color of moving bar
	dotColor.b = ((double)g_pSettings->getGreen()) / 255.0f;
	dotColor.c = ((double)g_pSettings->getBlue()) / 255.0f;

	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	fixSpotColor.a = ((double)g_pOrientationTuning->getFixColorR()) / 255.0f;//init color of fixation dot
	fixSpotColor.b = ((double)g_pOrientationTuning->getFixColorG()) / 255.0f;
	fixSpotColor.c = ((double)g_pOrientationTuning->getFixColorB()) / 255.0f;

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

	bTrialsInitialized = false;
	bStartTrials = false;

	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, iScrWidthPix, vsgDEGREEUNIT, &dScreenWidthDeg);
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, iScrHeightPix, vsgDEGREEUNIT, &dScreenHeightDeg);
    	
	iPage = 0;
	return true;	
}

bool COrientationTuningStimulus::DisplayStimulus() 
{
	// if searching for RF
	if (bStartTrials) {
		DisplayTrials();
	}
	// allow the UI thread to run for a brief moment
	Sleep(1);
	return true;
}

void COrientationTuningStimulus::DisplayTrials() 
{
	if (!bTrialsInitialized) InitializeTrials();
	char bufferTag[80];
	sprintf(bufferTag, "%s|0x0/", g_pOrientationTuning->getUnitTag());
	//theComm.transmit(bufferTag, (int)strlen(bufferTag));
	g_pSettings->theSerialComm.transmit(bufferTag, (int)strlen(bufferTag));
	Sleep(10);
	char buffer[80];
	if (g_pOrientationTuning->getField() == COrientationTuning::FIELD_LEFT_EYE) {
		sprintf(buffer, "W %.2f, H %.2f, A: %.2f, S %.2f, LX %.2f, LY %.2f|0x0/", g_pOrientationTuning->getBarWidth(), g_pOrientationTuning->getBarHeight(), g_pOrientationTuning->getAngularStep(), g_pOrientationTuning->getMotionSpeed() , g_receptiveFieldLeft.xPos, g_receptiveFieldLeft.yPos);
	} 
	else if (g_pOrientationTuning->getField() == COrientationTuning::FIELD_RIGHT_EYE) {
		sprintf(buffer, "W %.2f, H %.2f, A: %.2f, S %.2f, RX %.2f, RY %.2f|0x0/", g_pOrientationTuning->getBarWidth(), g_pOrientationTuning->getBarHeight(), g_pOrientationTuning->getAngularStep(), g_pOrientationTuning->getMotionSpeed() , g_receptiveFieldRight.xPos, g_receptiveFieldRight.yPos);
	} 
	else if (g_pOrientationTuning->getField() == COrientationTuning::FIELD_INTERLEAVED) {
		sprintf(buffer, "W %.2f, H %.2f, A: %.2f, S %.2f, LX %.2f, LY %.2f, RX %.2f, RY %.2f|0x0/", g_pOrientationTuning->getBarWidth(), g_pOrientationTuning->getBarHeight(), g_pOrientationTuning->getAngularStep(), g_pOrientationTuning->getMotionSpeed() , g_receptiveFieldLeft.xPos, g_receptiveFieldLeft.yPos, g_receptiveFieldRight.xPos, g_receptiveFieldRight.yPos);
	} 
	else {
		sprintf(buffer, "W %.2f, H %.2f, A: %.2f, S %.2f, X %.2f, Y %.2f|0x0/", g_pOrientationTuning->getBarWidth(), g_pOrientationTuning->getBarHeight(), g_pOrientationTuning->getAngularStep(), g_pOrientationTuning->getMotionSpeed() , g_pOrientationTuning->getXPosition(), g_pOrientationTuning->getYPosition());
	}
	
	//theComm.transmit(buffer, (int)strlen(buffer));
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
	myTimer->StartTimer();
	// iterate through all the trials
	int lastIndex = -1;
	for (int k = 0; k < g_pOrientationTuning->getRepeats(); k++) {
		int targetRunsLeft = g_pOrientationTuning->getRepeats() - k;
		for (int i = 0; i < numTuningRuns; i++) {

			// pick an orientation at random
			int j;
			if (numTuningRuns ==1) {
				j = 0;
			} 
			else {
				do {
					j = rand() % numTuningRuns;
				} while (pTuningRuns[j].nRunsLeft != targetRunsLeft || j == lastIndex);
			}
			
			lastIndex = j;

			// decrement the nRunsLeft parameter
			pTuningRuns[j].nRunsLeft--;

			// update display strings
			g_pOrientationTuningDlg->updateDisplayStrings(pTuningRuns[j].angle, targetRunsLeft, numTuningRuns * g_pOrientationTuning->getRepeats() - (k*numTuningRuns + i), -1, -1);

			if (g_pOrientationTuning->getUseFixation()) {
				//draw with fixation dot
				//requires frame-redraw animation instead of a moving screen
				// draw the bar at the current orientation
				DrawMoveBarWithDot(pTuningRuns[j]);
			} 
			else {
				// draw the bar at the current orientation
				DrawBar(pTuningRuns[j]);
				
				// move it across the screen 
				MoveBar(pTuningRuns[j]);
			}
			
			//Set the current drawing page.
			myTimer->CheckTimerWithReset();
			etu::AnalogOut(0, (float)-9.95, false);
			etu::AnalogOut(1, (float)-9.95, false);

			// blank period
			if (g_pOrientationTuning->getBlankPeriod() > 0) {
				vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
				vsgPresent();
				iPage = 1 - iPage;
				etu::AnalogOut(4, 5, true);
				WaitUntil(g_pOrientationTuning->getBlankPeriod()  - myTimer->CheckTimerWithReset(), 5);
			} 
	
			// intertrial delay
			if (g_pOrientationTuning->getIntertrialTime() > 0) {
				vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
				vsgPresent();
				iPage = 1 - iPage;
				etu::AnalogOut(4, -5, true);
				WaitUntil(g_pOrientationTuning->getIntertrialTime() - elapsedTime, 5);
				myTimer->CheckTimerWithReset();
			}

			if (bEndThread) {
				//theComm.stop();
				bTrialsInitialized = false;
				return;
			}
		}
	}
	
	//theComm.stop();
	g_pOrientationTuningDlg->updateDisplayStrings(-1, -1, 0, -1, -1);
	bStartTrials = false;
	bTrialsInitialized = false;
	bEndThread = true;
}

void COrientationTuningStimulus::DrawBar(OTTuningRun currentRun) 
{
	vsgSetBackgroundColour(&bgColor);
	//Set the current drawing page.
	vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
	vsgMoveScreen(screenPixOffsetX, screenPixOffsetY);
	
	vsgSetDrawMode(vsgCENTREXY);
	vsgObjDestroy(g_stim);
	g_stim = vsgObjCreate();
	vsgObjSetContrast(100);
	vsgObjSetPixelLevels(2,1);

	vsgSetDrawOrigin(screenPixOffsetX + iScrWidthPix / 2, screenPixOffsetY + iScrHeightPix / 2);

	vsgObjSetColourVector(&blackColor,&dotColor,vsgBIPOLAR);

	// convert the degrees to pixels
	double dSizeH, dSizeV;
	double startX, startY;

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.barWidth, vsgPIXELUNIT, &dSizeH);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.barHeight, vsgPIXELUNIT, &dSizeV);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.startX, vsgPIXELUNIT, &startX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.startY, vsgPIXELUNIT, &startY);

	vsgSetDrawMode(vsgCENTREXY);
	float barAngle = currentRun.angle + 90;
	while (barAngle >= 180) {
		barAngle -= 180;
	}
	vsgDrawBar(startX, startY, dSizeH, dSizeV, barAngle);

	char buffer[80];
	sprintf(buffer, "Orientation %.2f, Field: %s|0x0%x/", currentRun.angle, currentRun.field, currentRun.index);
	int length = (int)strlen(buffer);
	//theComm.transmit(buffer, length);
	g_pSettings->theSerialComm.transmit(buffer, length);
	
	elapsedTime = (int)myTimer->CheckTimerWithReset();

	iPage = 1-iPage;
}

void COrientationTuningStimulus::MoveBar(OTTuningRun currentRun) 
{
	int nFrames = (int)(g_pOrientationTuning->getTrialDuration() * dFrameRate / 1000);
	double dx, dy;
	double xDeg, yDeg;
	double x, y;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.dx, vsgPIXELUNIT, &dx);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.dy, vsgPIXELUNIT, &dy);
	x = screenPixOffsetX;
	y = screenPixOffsetY;
	xDeg = currentRun.startX;
	yDeg = -currentRun.startY;
	
	vsgPresent();
	etu::AnalogOut(0, currentRun.startX / g_pSettings->getDegPerVolt(), false);
	etu::AnalogOut(1, currentRun.startY / g_pSettings->getDegPerVolt(), false);
	etu::AnalogOut(4, 0, true); // trial phase
	for (int i = 1; i < currentRun.nFrames; i++) {
		x-= dx;
		y-= dy;
		xDeg += currentRun.dx;
		yDeg += -currentRun.dy;
		vsgMoveScreen(x, y);
		etu::AnalogOut(0, (float)xDeg / g_pSettings->getDegPerVolt(), false);
		etu::AnalogOut(1, (float)yDeg / g_pSettings->getDegPerVolt(), true);
	}
}

// palette redraw 30Hz version
void COrientationTuningStimulus::DrawMoveBarWithDot(OTTuningRun currentRun) 
{
	bool showFrameRate = false;
	//init palette
	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;
	dotColor.a = ((double)g_pSettings->getRed()) / 255.0f;
	dotColor.b = ((double)g_pSettings->getGreen()) / 255.0f;
	dotColor.c = ((double)g_pSettings->getBlue()) / 255.0f;
	//init color of fixation dot
	fixSpotColor.a = ((double)g_pOrientationTuning->getFixColorR()) / 255.0f;
	fixSpotColor.b = ((double)g_pOrientationTuning->getFixColorG()) / 255.0f;
	fixSpotColor.c = ((double)g_pOrientationTuning->getFixColorB()) / 255.0f;
	VSGLUTBUFFER lutbuffer;
	lutbuffer[0].a=bgColor.a;
	lutbuffer[0].b=bgColor.b;
	lutbuffer[0].c=bgColor.c;
	lutbuffer[1].a=dotColor.a;
	lutbuffer[1].b=dotColor.b;
	lutbuffer[1].c=dotColor.c;
	lutbuffer[2].a=fixSpotColor.a;
	lutbuffer[2].b=fixSpotColor.b;
	lutbuffer[2].c=fixSpotColor.c;
	for (int i=3; i<256; i++) {
		lutbuffer[i].a=bgColor.a;//blackColor
		lutbuffer[i].b=bgColor.b;
		lutbuffer[i].c=bgColor.c;
	}
	vsgLUTBUFFERWrite(1,&lutbuffer);
	vsgLUTBUFFERtoPalette(1);
	vsgSetBackgroundColour(&bgColor);
	vsgSetFixationColour(&dotColor);

	//Set the current drawing page.
	vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
	vsgMoveScreen(screenPixOffsetX, screenPixOffsetY);
	
	// convert the degrees to pixels
	double dSizeH, dSizeV;
	double startX, startY;
	double dotsize, fixx, fixy;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.barWidth, vsgPIXELUNIT, &dSizeH);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.barHeight, vsgPIXELUNIT, &dSizeV);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.startX, vsgPIXELUNIT, &startX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.startY, vsgPIXELUNIT, &startY);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pOrientationTuning->getFixD(), vsgPIXELUNIT, &dotsize);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pOrientationTuning->getFixX() + g_pSettings->getXOffset() + g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &fixx);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -(g_pOrientationTuning->getFixY() + g_pSettings->getYOffset() + g_pSettings->getScreenYOffset()), vsgPIXELUNIT, &fixy);
	double dx, dy;
	double xDeg, yDeg;
	double x, y;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.dx, vsgPIXELUNIT, &dx);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.dy, vsgPIXELUNIT, &dy);
	x = startX;//screenPixOffsetX;
	y = startY;//screenPixOffsetY;
	xDeg = currentRun.startX;
	yDeg = -currentRun.startY;

	//prepare to draw
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(screenPixOffsetX + iScrWidthPix / 2, screenPixOffsetY + iScrHeightPix / 2);
	int nFrames = (int)(g_pOrientationTuning->getTrialDuration() * dFrameRate / 1000);

	//begin loop
	etu::AnalogOut(2, (float)(currentRun.startX + g_pSettings->getXOffset()) / g_pSettings->getDegPerVolt(), false);
	etu::AnalogOut(3, (float)(currentRun.startY + g_pSettings->getYOffset()) / g_pSettings->getDegPerVolt(), false);
	etu::AnalogOut(4, 0, true); // trial phase

	//debug frame rate
	long double frameAvgTime = 0;
	long before = clock();

	//write analog output of fix dot location
	etu::AnalogOut(0, (float)(g_pOrientationTuning->getFixX() + g_pSettings->getXOffset()) / g_pSettings->getDegPerVolt(), false);
	etu::AnalogOut(1, (float)(g_pOrientationTuning->getFixY() + g_pSettings->getYOffset()) / g_pSettings->getDegPerVolt(), true);

	//send output textmark
	char buffer[80];
	sprintf(buffer, "Orientation %.2f, Field: %s|0x0%x/\0", currentRun.angle, currentRun.field, currentRun.index);
	int length = (int)strlen(buffer);
	//theComm.transmit(buffer, length);
	g_pSettings->theSerialComm.transmit(buffer, length);

	elapsedTime = (int)myTimer->CheckTimerWithReset();

	for (int i = 1; i < currentRun.nFrames; i++) {
		vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
		//x-= dx;
		//y-= dy;
		x+= dx;
		y+= dy;
		xDeg += currentRun.dx;
		yDeg += -currentRun.dy;

		//draw the bar
		vsgSetPen1(1);
		float barAngle = currentRun.angle + 90;
		while (barAngle >= 180) {
			barAngle -= 180;
		}
		//vsgDrawBar(startX, startY, dSizeH, dSizeV, barAngle);
		vsgDrawBar(fixx+x, fixy+y, dSizeH, dSizeV, barAngle);

		//draw the fixation dot
		vsgSetPen1(2);
		vsgDrawOval(fixx, fixy, dotsize, dotsize);
		
		//ok show a frame now
		vsgPresent();
		iPage = 1-iPage;
		
		//write analog output of moving bar location
		etu::AnalogOut(2, (float)(xDeg + g_pSettings->getXOffset()) / g_pSettings->getDegPerVolt(), false);
		etu::AnalogOut(3, (float)(yDeg + g_pSettings->getYOffset())/ g_pSettings->getDegPerVolt(), true);
		//and orientation
		//port 16 is phase
		//port 17 is orientation
		etu::AnalogOut(5, currentRun.angle / 36.0, true);

		//debug frame rate
		if (showFrameRate) {
			long now = clock();
			frameAvgTime += now-before;
			before = now;
		}
	}

	//debug frame rate
	if (showFrameRate) {
		char buffer[1024];
		sprintf(buffer, "frame avg time = %.6f ms", 1000.0*(frameAvgTime/((double)currentRun.nFrames-1.0))/((double)CLOCKS_PER_SEC));
		AfxMessageBox(buffer, 0, 0);
	}
}

void COrientationTuningStimulus::InitializeTrials() 
{
	// clear the drawing pages
	vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
    vsgPresent();
	vsgSetDrawPage(vsgVIDEOPAGE, 1-iPage, vsgBACKGROUND);
	vsgPresent();
	vsgMoveScreen(0, 0);

	screenPixOffsetX = 1024 - (iScrWidthPix / 2);
	screenPixOffsetY = 512 - (iScrHeightPix / 2);
	if (pTuningRuns) delete [] pTuningRuns;
	int numInitializingLoops;

	numTuningRuns = (int)((g_pOrientationTuning->getEndAngle() - g_pOrientationTuning->getStartAngle()) / g_pOrientationTuning->getAngularStep()) +1;
	numInitializingLoops = numTuningRuns;
	if (g_pOrientationTuning->getField()  == COrientationTuning::FIELD_INTERLEAVED) {
		numTuningRuns *=2;
	}
	pTuningRuns = new OTTuningRun[numTuningRuns];
	if (numTuningRuns == 0) numTuningRuns = 1;

	float xLocationLeft, yLocationLeft;
	float xLocationRight, yLocationRight;
	float barWidthLeft, barWidthRight, barHeightLeft, barHeightRight;
	bool bUseLeft = false;
	bool bUseRight = false;

	if (g_pOrientationTuning->getField() == COrientationTuning::FIELD_LEFT_EYE || g_pOrientationTuning->getField() == COrientationTuning::FIELD_INTERLEAVED) {
		bUseLeft = true;
		xLocationLeft = g_receptiveFieldLeft.xPos;
		yLocationLeft = g_receptiveFieldLeft.yPos;
		if (g_pOrientationTuning->getField() == COrientationTuning::FIELD_INTERLEAVED) {
			barWidthLeft = min(g_receptiveFieldLeft.barWidth, g_receptiveFieldLeft.barHeight);
			barHeightLeft = max(g_receptiveFieldLeft.barWidth, g_receptiveFieldLeft.barHeight);
		} 
		else {
			barWidthLeft = min(g_pOrientationTuning->getBarWidth(), g_pOrientationTuning->getBarHeight());
			barHeightLeft = max(g_pOrientationTuning->getBarWidth(), g_pOrientationTuning->getBarHeight());
		}
	} 
	
	if (g_pOrientationTuning->getField() == COrientationTuning::FIELD_RIGHT_EYE || g_pOrientationTuning->getField() == COrientationTuning::FIELD_INTERLEAVED) {
		bUseRight = true;
		xLocationRight = g_receptiveFieldRight.xPos;
		yLocationRight = g_receptiveFieldRight.yPos;
		if (g_pOrientationTuning->getField() == COrientationTuning::FIELD_INTERLEAVED) {
			barWidthRight = min(g_receptiveFieldRight.barWidth, g_receptiveFieldRight.barHeight);
			barHeightRight = max(g_receptiveFieldRight.barWidth, g_receptiveFieldRight.barHeight);
		} 
		else {
			barWidthRight = min(g_pOrientationTuning->getBarWidth(), g_pOrientationTuning->getBarHeight());
			barHeightRight = max(g_pOrientationTuning->getBarWidth(), g_pOrientationTuning->getBarHeight());
		}
	} 

	if (g_pOrientationTuning->getField() == COrientationTuning::FIELD_MANUAL) {
		bUseLeft = true;
		xLocationLeft = g_pOrientationTuning->getXPosition();
		yLocationLeft = g_pOrientationTuning->getYPosition();
		barWidthLeft = min(g_pOrientationTuning->getBarWidth(), g_pOrientationTuning->getBarHeight());
		barHeightLeft = max(g_pOrientationTuning->getBarWidth(), g_pOrientationTuning->getBarHeight());
	}

	int index = 0;
	float angle = g_pOrientationTuning->getStartAngle();
	for (int i = 0; i < numInitializingLoops; i++) {
		if (bUseLeft) {
			pTuningRuns[index].index = index+1;
			pTuningRuns[index].angle = angle;
			pTuningRuns[index].nRunsLeft = g_pOrientationTuning->getRepeats();
			pTuningRuns[index].dx = g_pOrientationTuning->getMotionSpeed() / (float)dFrameRate * (float)sin(etu::DegToRad(angle));
			pTuningRuns[index].dy = g_pOrientationTuning->getMotionSpeed() / (float)dFrameRate * (float)cos(etu::DegToRad(angle));
			pTuningRuns[index].startX = xLocationLeft - (pTuningRuns[index].dx * g_pOrientationTuning->getTrialDuration() * (float)dFrameRate / 2000.0f);
			pTuningRuns[index].startY = -yLocationLeft - (pTuningRuns[index].dy * g_pOrientationTuning->getTrialDuration() * (float)dFrameRate / 2000.0f);
			pTuningRuns[index].nFrames = (int)(g_pOrientationTuning->getTrialDuration() * dFrameRate / 1000.0f);
			pTuningRuns[index].barHeight = barHeightLeft;
			pTuningRuns[index].barWidth = barWidthLeft;
			if (g_pOrientationTuning->getField() == COrientationTuning::FIELD_MANUAL) {
				pTuningRuns[index].field = "Manual";
			} 
			else {
				pTuningRuns[index].field = "Left";
			}

			while (isOffscreen(pTuningRuns[index])) {
				pTuningRuns[index].startX += pTuningRuns[index].dx;
				pTuningRuns[index].startY += pTuningRuns[index].dy;
				pTuningRuns[index].nFrames--;
			}

			while (endsOffscreen(pTuningRuns[index])) {
				pTuningRuns[index].nFrames--;
			}
			index++;
		}
		if (bUseRight) {
			pTuningRuns[index].index = index+1;
			pTuningRuns[index].angle = angle;
			pTuningRuns[index].nRunsLeft = g_pOrientationTuning->getRepeats();
			pTuningRuns[index].dx = g_pOrientationTuning->getMotionSpeed() / (float)dFrameRate * (float)sin(etu::DegToRad(angle));
			pTuningRuns[index].dy = g_pOrientationTuning->getMotionSpeed() / (float)dFrameRate * (float)cos(etu::DegToRad(angle));
			pTuningRuns[index].startX = xLocationRight - (pTuningRuns[index].dx * g_pOrientationTuning->getTrialDuration() * (float)dFrameRate / 2000.0f);
			pTuningRuns[index].startY = -yLocationRight - (pTuningRuns[index].dy * g_pOrientationTuning->getTrialDuration() * (float)dFrameRate / 2000.0f);
			pTuningRuns[index].nFrames = (int)(g_pOrientationTuning->getTrialDuration() * dFrameRate / 1000.0f);
			pTuningRuns[index].barHeight = barHeightRight;
			pTuningRuns[index].barWidth = barWidthRight;
			pTuningRuns[index].field = "Right";

			while (isOffscreen(pTuningRuns[index])) {
				pTuningRuns[index].startX += pTuningRuns[index].dx;
				pTuningRuns[index].startY += pTuningRuns[index].dy;
				pTuningRuns[index].nFrames--;
			}


			while (endsOffscreen(pTuningRuns[index])) {
				pTuningRuns[index].nFrames--;
			}
			index++;
		}

		angle += g_pOrientationTuning->getAngularStep();		
	}

	bTrialsInitialized = true;
}

bool COrientationTuningStimulus::isOffscreen(OTTuningRun run) 
{
	if (fabs(run.startX) - (dScreenWidthDeg / 2) - run.barWidth > 0) {
		return true;
	}
	if (fabs(run.startY) - (dScreenHeightDeg / 2) - run.barWidth > 0) {
		return true;
	}
	return false;
}

bool COrientationTuningStimulus::endsOffscreen(OTTuningRun run) 
{
	if (fabs(run.startX + (run.nFrames * run.dx)) - (dScreenWidthDeg / 2) - run.barWidth > 0) {
		return true;
	}
	if (fabs(run.startY + (run.nFrames * run.dy)) - (dScreenHeightDeg / 2) - run.barWidth > 0) {
		return true;
	}
	return false;
}

void COrientationTuningStimulus::StartTrials(bool bStart) 
{
	bStartTrials = true;
}

CString COrientationTuningStimulus::StimulusType() 
{
	return "Orientation Tuning Stimulus";
}

void COrientationTuningStimulus::StopExecution() 
{
	bEndThread = true;
}

#endif // APP_EYE_TRACKING
