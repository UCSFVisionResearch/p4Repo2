/**
 * @file CalibrationStimulus.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "app/AppCore.h"
#include "comm/SerialComm.h"
#include "comm/SMISerialComm.h"
#include "CalibrationStimulus.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	VSG IN 0:	Next Stim Pulse. 1 => Behavioral contingency
				has been met, move on to the excursion phase.
				0 => behvioral continengency criteria has not 
				yet been met.

	VSG Out 0:	Trial Running pulse. 1 => trial is running. 
				0 => trial is not running
	VSG Out 1:	behavior count timer
	VSG Out 2:  Set low to indicate "calibration"

	Analog Out 0:	Horizontal target location
	Analog Out 1:	Vertical target location


NOTES:
	MKF 11/06/07 At some point in 2005 (or earlier?) AM wrote much
		of this code intending to support automated calibration.
		Now it just shows a dot to move around manually
		while calibration is done by hand
************************************************************/

/*****************************************************************************
 * Functions
 *****************************************************************************/

/***********************************************************
	 CommControllingFunctionLeft()
	 - Initialization routine for the left eye serial port 
	 thread.
************************************************************/
UINT __cdecl CommControllingFunctionLeft( LPVOID pParam ) 
{
	g_serialCommLeft.init(true, CBR_9600, 3);
	return 0;
}

/***********************************************************
	 CommControllingFunctionRight()
	 - Initialization routine for the right eye serial port 
	 thread.
************************************************************/
UINT __cdecl CommControllingFunctionRight( LPVOID pParam ) 
{
	g_serialCommRight.init(true, CBR_9600, 4);
	return 0;
}

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor     
	 -  Initializes the newly instantiated object. 
		Sets up arrays for the various calibration types
************************************************************/
CCalibrationStimulus::CCalibrationStimulus() 
{
	Initialize();
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	iDotNum = 0;
	iCalibrationType = 9;
	iPage = 0;
	pointX, pointY = 0;

	numAccepts = 0;

	// arrays store the dot locations for each type of calibration
	TWO_PT_CAL[0].x = -1;
	TWO_PT_CAL[0].y = -1;
	TWO_PT_CAL[1].x = 1;
	TWO_PT_CAL[1].y = 1;

	FIVE_PT_CAL[0].x = 0;
	FIVE_PT_CAL[0].y = 0;
	FIVE_PT_CAL[1].x =-1;
	FIVE_PT_CAL[1].y =-1;
	FIVE_PT_CAL[2].x =1;
	FIVE_PT_CAL[2].y =-1;
	FIVE_PT_CAL[3].x =-1;
	FIVE_PT_CAL[3].y =1;
	FIVE_PT_CAL[4].x =1;
	FIVE_PT_CAL[4].y =1;

	NINE_PT_CAL[0].x = 0;
	NINE_PT_CAL[0].y = 0;
	NINE_PT_CAL[1].x = -1;
	NINE_PT_CAL[1].y = -1;
	NINE_PT_CAL[2].x = 1;
	NINE_PT_CAL[2].y = -1;
	NINE_PT_CAL[3].x = -1;
	NINE_PT_CAL[3].y = 1;
	NINE_PT_CAL[4].x = 1;
	NINE_PT_CAL[4].y = 1;
	NINE_PT_CAL[5].x = -1;
	NINE_PT_CAL[5].y = 0;
	NINE_PT_CAL[6].x = 0;
	NINE_PT_CAL[6].y = -1;
	NINE_PT_CAL[7].x = 1;
	NINE_PT_CAL[7].y = 0;
	NINE_PT_CAL[8].x = 0;
	NINE_PT_CAL[8].y = 1;

	g_pCalibration->sendSettings(g_pSettings->theSerialComm);

	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);
	vsgIOWriteDigitalOut(0x0000, vsgDIG2);

	bDrawn = false;

	// set the view distance
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);	
}

/***********************************************************
	 Destructor
	 -  Destroys the current instantiation of the obejct. 
	 The singleLock object must be deleted here to prevent 
	 memory leaks.
************************************************************/
CCalibrationStimulus::~CCalibrationStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);

	//send stop mark
	char buffer[80];
	sprintf(buffer, "Calibration;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}

/***********************************************************
	 DisplayStimulus    
	 -  Displays the stimulus based on the parameters set in
	 the dialog box as well as parameters read from the serial
	 ports (connected to SMI machines)
************************************************************/
bool CCalibrationStimulus::DisplayStimulus() 
{
	// if we are cycling through all the points, then call
	// DoCycle() method and then return.
	if (g_pCalibration->getCycle()) {
		DoCycle();
		iPage = 1 - iPage;
		return true;	
	}
	
	double dSize, dSeparationPix;
	double screenXOffset, screenYOffset;
	double stimXOffset, stimYOffset;
	double xDeg, yDeg;
	int xPix, yPix;
	
	// Set the dot color and background color
	VSGTRIVAL bgColor;
	dotColor.a = ((double)g_pSettings->getRed()) / 255.0f;
	dotColor.b = ((double)g_pSettings->getGreen()) / 255.0f;
	dotColor.c = ((double)g_pSettings->getBlue()) / 255.0f;

	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	VSGLUTBUFFER lb;
	for (int i=0; i<256; i++) {
		lb[i].a = bgColor.a;
		lb[i].b = bgColor.b;
		lb[i].c = bgColor.c;
	}
	lb[2].a = dotColor.a;	lb[2].b = dotColor.b;	lb[2].c = dotColor.c;
	vsgSetBackgroundColour(&lb[vsgBACKGROUND]);
	vsgSetFixationColour(&lb[vsgFIXATION]);
	vsgLUTBUFFERWrite(iPage,&lb);
	vsgLUTBUFFERtoPalette(iPage);
	
	// convert the degrees to pixels
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pCalibration->getSize(), vsgPIXELUNIT, &dSize);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pCalibration->getSeparation(), vsgPIXELUNIT, &dSeparationPix);
	
	//test display pix/degree
	if (false) {
		char buff[1024];
		double oneDegInPix = 0;
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, 1.0, vsgPIXELUNIT, &oneDegInPix);
		sprintf(buff, "1 degree = %f pixels.", oneDegInPix);
		MessageBox(NULL, buff, NULL, 0);
	}

	// initialize the drawing routines
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix / 2);
	
	vsgSetPen1(2);

	// when we specify the dot location, we want to make sure
	// that that position is the center of the dot
	vsgSetDrawMode(vsgCENTREXY);

	// convert offsets from degrees to pixels
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &stimXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &stimYOffset);

	// if all of the dots are to be displayed, iterate through each point and 
	// draw the dots
	if (g_pCalibration->showAll()) {
		//serialComm.stop();
		switch (iCalibrationType) {
			case 2:
				for (int i = 0; i < iCalibrationType; i++) {
					vsgDrawOval(stimXOffset + screenXOffset+(TWO_PT_CAL[i].x * dSeparationPix), stimYOffset + screenYOffset + (TWO_PT_CAL[i].y * dSeparationPix), dSize, dSize);
				}		
				break;
			case 5:
				for (int i = 0; i < iCalibrationType; i++) {
					vsgDrawOval(stimXOffset + screenXOffset+(FIVE_PT_CAL[i].x * dSeparationPix), stimYOffset + screenYOffset + (FIVE_PT_CAL[i].y * dSeparationPix), dSize, dSize);
				}
				break;
			case 9:
				for (int i = 0; i < iCalibrationType; i++) {
					vsgDrawOval(stimXOffset + screenXOffset+(NINE_PT_CAL[i].x * dSeparationPix), stimYOffset + screenYOffset + (NINE_PT_CAL[i].y * dSeparationPix), dSize, dSize);
				}
				break;
		}
		xDeg = 0;
		yDeg = 0;
	} 
	else  { // otherwise, draw the current dot in the calibration process as specified by the SMI machines
		switch (iCalibrationType) {
			case 2:
				xPix = TWO_PT_CAL[iDotNum].x;
				yPix = TWO_PT_CAL[iDotNum].y;
				break;
			case 5:
				xPix = FIVE_PT_CAL[iDotNum].x;
				yPix = FIVE_PT_CAL[iDotNum].y;
				break;
			case 9:
				if (g_pCalibrationDlg->m_Dot9.GetCheck()==BST_CHECKED) iDotNum=4;
				else if (g_pCalibrationDlg->m_Dot8.GetCheck()==BST_CHECKED) iDotNum=8;
				else if (g_pCalibrationDlg->m_Dot7.GetCheck()==BST_CHECKED) iDotNum=3;
				else if (g_pCalibrationDlg->m_Dot6.GetCheck()==BST_CHECKED) iDotNum=7;
				else if (g_pCalibrationDlg->m_Dot4.GetCheck()==BST_CHECKED) iDotNum=5;
				else if (g_pCalibrationDlg->m_Dot3.GetCheck()==BST_CHECKED) iDotNum=2;
				else if (g_pCalibrationDlg->m_Dot2.GetCheck()==BST_CHECKED) iDotNum=6;
				else if (g_pCalibrationDlg->m_Dot1.GetCheck()==BST_CHECKED) iDotNum=1;
				else iDotNum=0;
				xPix = NINE_PT_CAL[iDotNum].x;
				yPix = NINE_PT_CAL[iDotNum].y;
				break;
			default:
				AfxMessageBox("Invalid Calibration Type");
				break;
		}
		xPix *= (int)dSeparationPix;
		yPix *= (int)dSeparationPix;

		// draw the current dot
		vsgDrawOval(xPix + screenXOffset + stimXOffset, yPix + screenYOffset + stimYOffset, dSize, dSize);

		// convert the dot location from pixels to degrees for the analog output
		g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, xPix + stimXOffset, vsgDEGREEUNIT, &xDeg);
		g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, yPix + stimYOffset, vsgDEGREEUNIT, &yDeg);
	}

	// present the changes to the display
	vsgPresent();
	// Set the analog output to the target position
	etu::AnalogOut(0, (float)xDeg / g_pSettings->getDegPerVolt());
	etu::AnalogOut(1, (float)-yDeg / g_pSettings->getDegPerVolt());
	
	WaitUntil(50);

	iPage = 1 - iPage;
	return true;
}

/***********************************************************
	 AcceptPoint()
	 -  This function counted the total number accpets  in an 
	 attempt to coordinate simultaneous binocular calibrations, 
	 but this was abandoned at some point. This code has not been
	 deleted incase there is need for it at some point.
************************************************************/
void CCalibrationStimulus::AcceptPoint() 
{
	// make sure  no other thread is accessing numAccepts
	singleLock->Lock();
	numAccepts++;
	singleLock->Unlock();
}

/***********************************************************
	 setActiveCalibrationPoint
	 -  Changes the active point during a calibration run. This
	 function is called from the SMISerialComm class whenever an
	 "ET_CHG" command was received. The support for simultaneous
	 binocular calibrations is still in this function, but unused
	 since we are no longer pursuing that method for calibrations.
************************************************************/
void CCalibrationStimulus::setActiveCalibrationPoint(int iPoint) 
{
	char buffer[100];
	// if both eyes - check for two ET_ACC instructions from SMI
	// this part, is not currently used and if used does not function
	// perfectly.
	if (g_pCalibration->getEye() == CCalibration::EYE_BOTH) {
		singleLock->Lock();
		// if 2 Accepts have been received, move on to next point
		if (numAccepts >= 2) {
			sprintf(buffer, "ET_CHG %d\n", iPoint);
			numAccepts = 0;
			g_serialCommLeft.transmit(buffer, 9);
			g_serialCommRight.transmit(buffer, 9);
			iDotNum = iPoint - 1;
		} 
		else {
			// if only 1 has been received, then wait for some time, and check again
			singleLock->Unlock();
			Sleep(g_pCalibration->getAcceptDelay());
			singleLock->Lock();
			// if after waiting, 2 or more acc have come in, then move to next point
			if (numAccepts >= 2) {
				sprintf(buffer, "ET_CHG %d\n", iPoint);
				numAccepts = 0;
				g_serialCommLeft.transmit(buffer, 9);
				g_serialCommRight.transmit(buffer, 9);
				iDotNum = iPoint - 1;
			} 
			else {
				// if less than 2 acc have come in, then retry the same point
				sprintf(buffer, "ET_CHG %d\n", iDotNum+1);
				numAccepts = 0;
				g_serialCommLeft.transmit(buffer, 9);
				g_serialCommRight.transmit(buffer, 9);
			}
		}
		singleLock->Unlock();
	} 
	else {
		// if only one eye is being used, simply advance
		// the dot
		iDotNum = iPoint - 1;
	}
}

/***********************************************************
	 DoCycle()
	 -  This function cycles through the 9 point calibration 
	 locations in order to train the animals to look at these
	 specific locations. The analog output signal follows the 
	 dots through out the cycling. (NOTE: This function does NOT
	 make use of the stimulus offset!)
************************************************************/
void CCalibrationStimulus::DoCycle() 
{
	double dSize, dSeparationPix;
	double screenXOffset, screenYOffset;
	double xDeg, yDeg;
	//int xPix, yPix;

	// set the dot color and background color
	VSGTRIVAL bgColor;
	dotColor.a = ((double)g_pSettings->getRed()) / 255.0f;
	dotColor.b = ((double)g_pSettings->getGreen()) / 255.0f;
	dotColor.c = ((double)g_pSettings->getBlue()) / 255.0f;

	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	// set the view distance
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);	

	// end memory leaks!
	VSGLUTBUFFER lb;
	vsgPaletteRead(&lb);
	lb[253].a = bgColor.a;	lb[253].b = bgColor.b;	lb[253].c = bgColor.c;
	lb[2].a = dotColor.a;	lb[2].b = dotColor.b;	lb[2].c = dotColor.c;
	vsgSetBackgroundColour(&lb[vsgBACKGROUND]);
	vsgSetFixationColour(&lb[vsgFIXATION]);
	vsgLUTBUFFERWrite(0,&lb);
	vsgLUTBUFFERtoPalette(0);
	vsgSetPen1(2);
	
	// convert the degrees to pixels
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pCalibration->getSize(), vsgPIXELUNIT, &dSize);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pCalibration->getSeparation(), vsgPIXELUNIT, &dSeparationPix);
	
	// set drawing surface parameters
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix / 2);
	
	// convert offsets from degrees to pixels
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);

	// iterate through the 9 locations and draw the points
	for (int i = 0; i < 9; i++) {
		vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
		vsgSetDrawMode(vsgCENTREXY);
		vsgDrawOval(screenXOffset+(NINE_PT_CAL[i].x * dSeparationPix), screenYOffset + (NINE_PT_CAL[i].y * dSeparationPix), dSize, dSize);
		double xPix = NINE_PT_CAL[i].x * dSeparationPix;
		double yPix = NINE_PT_CAL[i].y * dSeparationPix;

		// convert the location from pixels to degrees
		g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, xPix, vsgDEGREEUNIT, &xDeg);
		g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, yPix, vsgDEGREEUNIT, &yDeg);
	
		// display the current dot
		vsgPresent();

		// Set the analog output to the target position
		etu::AnalogOut(0, (float)xDeg / g_pSettings->getDegPerVolt());
		etu::AnalogOut(1, (float)(-yDeg / g_pSettings->getDegPerVolt()));

		int response = 0;
		int totalTime = 0;
		while (response == 0 && totalTime <= g_pCalibration->getAcceptDelay() ) {
			response =  (vsgIOReadDigitalIn() & vsgDIG0);//0x001
			WaitUntil(10);
			totalTime += 10;
			if (bEndThread) {
				vsgObjDestroy(g_stim);
				return;
			}
		}
	}
}

/***********************************************************
	 setCalibratinType()
	 -  This function is called by the serial port 
	 communication threads in order to set the calibration 
	 type (2, 5 or 9 point).
************************************************************/
void CCalibrationStimulus::setCalibrationType(int iType) 
{
	iCalibrationType = iType;
}

/***********************************************************
	 setCalibrationPoint()
	 - This function attempted to customize point location
	 based on the geometry specified in the SMI software, but
	 we decided to simply use a grid of dots separted by a
	 specified number of degrees. (if uncommented, this function
	 will work as intended)
************************************************************/
void CCalibrationStimulus::setCalibrationPoint(int pt, int x, int y) 
{
	/*switch  (iCalibrationType) {
		case 2:
			TWO_PT_CAL[pt-1].x = x;
			TWO_PT_CAL[pt-1].y = y;
			break;
		case 5:
			FIVE_PT_CAL[pt-1].x = x;
			FIVE_PT_CAL[pt-1].y = y;
			break;
		case 9:
			NINE_PT_CAL[pt-1].x = x;
			NINE_PT_CAL[pt-1].y = y;
			break;

	}*/
}

/***********************************************************
	 StimulusType()
	 -  Inherited method that returns the stimulus type.
************************************************************/
CString CCalibrationStimulus::StimulusType() 
{
	return "Calibration Stimulus";
}

/***********************************************************
	 StopExecution()
	 -  Stops this stimulus execution. Serial ports are
	 closed down, and the stimulus thread is signaled to end
	 execution.
************************************************************/
void CCalibrationStimulus::StopExecution() 
{
	// stop comm thread
	g_serialCommLeft.stop();
	g_serialCommRight.stop();
	// stop calibration thread
	bEndThread = true;
}


void CCalibrationStimulus::DoStuff() 
{
	if (!bDrawn) {
		// Set the dot color and background color
		VSGTRIVAL bgColor, red, blue, white;
		white.a = 1.0f;
		white.b = 1.0f;
		white.c = 1.0f;

		red.a = 1.0f;
		red.b = 0.0f;
		red.c = 0.0f;

		blue.a = 0.0f;
		blue.b = 0.0f;
		blue.c = 1.0f;

		bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
		bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
		bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

		// delete the old VSG g_stim object, and create a new one.
		vsgObjDestroy(g_stim);
		g_stim = vsgObjCreate();
		vsgObjSetContrast(100);
		vsgObjSetPixelLevels(2,1);
		
		// set the dot color for the vsg object.
		vsgObjSetColourVector(&blackColor,&white,vsgBIPOLAR);
		// when we specify the dot location, we want to make sure
		// that that position is the center of the dot
		vsgSetDrawMode(vsgCENTREXY);
		vsgDrawOval(0, 0, 5, 5);

		// delete the old VSG stim object, and create a new one.
		vsgObjDestroy(g_fixation);
		g_fixation = vsgObjCreate();
		vsgObjSetContrast(100);
		vsgObjSetPixelLevels(3,1);
		
		// set the dot color for the vsg object.
		vsgObjSetColourVector(&blackColor,&red,vsgBIPOLAR);
		// when we specify the dot location, we want to make sure
		// that that position is the center of the dot
		vsgSetDrawMode(vsgCENTREXY);
		vsgDrawOval(-3, -50, 5, 5);

		// delete the old VSG stim object, and create a new one.
		vsgObjDestroy(g_grating);
		g_grating = vsgObjCreate();
		vsgObjSetContrast(100);
		vsgObjSetPixelLevels(4,1);
		
		// set the dot color for the vsg object.
		vsgObjSetColourVector(&blackColor,&blue,vsgBIPOLAR);
		// when we specify the dot location, we want to make sure
		// that that position is the center of the dot
		vsgSetDrawMode(vsgCENTREXY);
		vsgDrawOval(3, -50, 5, 5);
		
		vsgPresent();

		bDrawn = true;
	}
}