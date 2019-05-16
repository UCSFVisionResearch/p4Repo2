/*****************************************************************************
 * Typedefs and Structs
 *****************************************************************************/
#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "CircleAnnulusStimulus.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
COMMUNICATIONS:
	Analog Out 0:	Horizontal target location
	Analog Out 1:	Vertical target location
	Analog Out 3:	Trial phase. -5V => intertrial; 0V => Trial;
					5V => Blank Time
************************************************************/

/***********************************************************
	 Constructor     
	 -  Initializes the newly instantiated object. 
		This stimulus does not actively use the Digital output ports
		VSG Output 0, carries the trial running signal which is
		high when stimulus is running, 0 otherwise.
************************************************************/
CCircleAnnulusStimulus::CCircleAnnulusStimulus() 
{
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	Initialize();
		
	pAnnulusTesting = NULL;
	iNumAnnulusRuns = 0;
		
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);	
}

/***********************************************************
	 Destructor     
	 -  Destroys arrays used in the class.
************************************************************/
CCircleAnnulusStimulus::~CCircleAnnulusStimulus() 
{
	if (pAnnulusTesting) delete [] pAnnulusTesting;
	pAnnulusTesting = NULL;

	if (myTimer) delete myTimer;

	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
}

/***********************************************************
	 Initialize()     
	 - Initialize the stimulus for display. This method sets
	 up color objects, intializes the VSG card, and calculates
	 timing constants for frame intervals.
************************************************************/
bool CCircleAnnulusStimulus::Initialize() 
{
	bEndThread=false;
	//myTimer = new etu::CTimer();
	
	// set trial phase to intertrial
	etu::AnalogOut(3, -5);
	
	// initialize colors
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

	// calculate timing constants
	dFrameTime = vsgGetSystemAttribute(vsgFRAMETIME);
	dFrameRate = 1000000.0 / dFrameTime;
	g_dHalfFrameInterval = DWORD(floor(dFrameTime / 2000.0f));
	g_dSleepThreshold = (double)g_dHalfFrameInterval / 2.0f;
	g_dFramerate = 1000000.0f / vsgGetSystemAttribute(vsgFRAMETIME);

	// set the view distance
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);
	vsgSetPageWidth(2048);

	// intialize display flags to false
	bTrialsInitialized = false;
	bStartTrials = false;

	// convert frame time into ms
	dFrameTime /= 1000;

	iPage = 0;
	return true;	
}

/***********************************************************
	 DisplayStimulus()
	 -  This function decides which type of stimulus has been 
	 requested, and dispatches the appropriate function to display
	 that stimulus once the start button has been pressed.
************************************************************/
bool CCircleAnnulusStimulus::DisplayStimulus() 
{
	// check to see if the start button has been pressed
	if (bStartTrials) {
		if (g_pCircleAnnulus->getOnOff()){
			DisplayTrialsOnOff();
		} else {
			DisplayTrials();
		}	
	}

	// allow the UI thread to run for a brief moment
	Sleep(1);
	return true;
}

/***********************************************************
	 DisplayTrials     
	 -  This function displays the standard circle annulus stimulus
	 where the display transitions to the background color between
	 presentations.
************************************************************/
void CCircleAnnulusStimulus::DisplayTrials() 
{
	// if the trial has not been initialized, then
	// try to initialize it. If initialization fails, 
	// then abort.
	if (!bTrialsInitialized) 
		if (!InitializeTrials()) {
			return;
		}	
	
	// Transmit the name of the unit as to the text mark channel
	// in spike 2.
	char bufferTag[80];
	sprintf(bufferTag, "%s|0x0/", g_pCircleAnnulus->getUnitTag());
	//theComm.transmit(bufferTag, (int)strlen(bufferTag));
	g_pSettings->theSerialComm.transmit(bufferTag, (int)strlen(bufferTag));

	// wait for serial port transmission. 10 milliseconds is usually adequate.
	Sleep(10);
	char buffer[80];

	// create a buffer with stimulus information pertinent for all trials in this sequence (RF location for either eye)
	if (g_pCircleAnnulus->getField() == CCircleAnnulus::FIELD_LEFT_EYE) {
		sprintf(buffer, "LX %.2f, LY %.2f|0x0/", g_receptiveFieldLeft.xPos, g_receptiveFieldLeft.yPos);
	} 
	else if (g_pCircleAnnulus->getField() == CCircleAnnulus::FIELD_RIGHT_EYE) {
		sprintf(buffer, "RX %.2f, RY %.2f|0x0/", g_receptiveFieldRight.xPos, g_receptiveFieldRight.yPos);
	} 
	else if (g_pCircleAnnulus->getField() == CCircleAnnulus::FIELD_INTERLEAVED) {
		sprintf(buffer, "LX %.2f, LY %.2f, RX %.2f, RY %.2f|0x0/", g_receptiveFieldLeft.xPos, g_receptiveFieldLeft.yPos, g_receptiveFieldRight.xPos, g_receptiveFieldRight.yPos);
	} 
	else {
		sprintf(buffer, "fX %.2f, fY %.2f|0x0/", g_pCircleAnnulus->getXPosition(), g_pCircleAnnulus->getYPosition());
	}
	//theComm.transmit(buffer, (int)strlen(buffer));
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
	myTimer->StartTimer();

	// iterate through all of the trials, and display the stimulus
	// based on the set of parameters saved in pAnnulusTesting[]
	int lastIndex = -1;
	for (int k = 0; k < g_pCircleAnnulus->getRepeats(); k++) {

		// targetRunsLeft keeps track of which block of stimuli are currently
		// being presented. This stimulus is presented in a block randomized fashion, 
		// so we make sure that all of the stimuli presented have "targetRunsLeft" runs
		// remaining. This ensures that each trial is presented exactly once before the block is
		// repeated.
		int targetRunsLeft = g_pCircleAnnulus->getRepeats() - k;

		// randomly iterate through each stimulus in the current block, and 
		// display it.
		for (int i = 0; i < iNumAnnulusRuns; i++) {
			// pick an annulus / circle at random
			int j;
			// if there is only one variant of the stimulus parameters, 
			// then we have to bypass the randomization, becaue there is
			// only one stimulus to present, and the randomization loop
			// would loop forever searching for a stimulus to run.
			if (iNumAnnulusRuns ==1) {
				j = 0;
			} 
			else {
				// find a stimulus that has not been displayed in this block, and we also 
				// want to make sure that the newly selected stimulsu was not the same one 
				// that was just presented. (This situation only occurs when this is the first
				// stimulus in a block -- in this case we want to make sure that the block does
				// start with the same stimulus that the previous block ended with.
				do {
					j = rand() % iNumAnnulusRuns;
				} while (pAnnulusTesting[j].nRunsLeft != targetRunsLeft || j == lastIndex);
			}
			lastIndex = j;

			// decrement the nRunsLeft parameter
			pAnnulusTesting[j].nRunsLeft--;

			// update display strings on the dialog box
			// and draw the circle or annulus
			if (pAnnulusTesting[j].bCircle) {
				g_pCircleAnnulusDlg->updateDisplayStrings(pAnnulusTesting[j].circleRadius, -1, targetRunsLeft, iNumAnnulusRuns * g_pCircleAnnulus->getRepeats() - (k*iNumAnnulusRuns + i));
				DrawCircle(pAnnulusTesting[j]);
			} 
			else {
				g_pCircleAnnulusDlg->updateDisplayStrings(pAnnulusTesting[j].circleRadius, pAnnulusTesting[j].annulusWidth, targetRunsLeft, iNumAnnulusRuns * g_pCircleAnnulus->getRepeats() - (k*iNumAnnulusRuns + i));
				DrawAnnulus(pAnnulusTesting[j]);
			}
			
			//Set the current drawing page to an unused draw page.
			vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
			
			// wait the trial duration
			WaitUntil(g_pCircleAnnulus->getTrialDuration() - myTimer->CheckTimer() - (2* dFrameTime), 5);

			// present the blank page (turn off the stimulus)
			vsgPresent();

			// reset the timer
			myTimer->CheckTimerWithReset();

			// set the analog represent for the target to offscreen (99.5 degrees from center fixation)
			etu::AnalogOut(0, (float)-9.95, false);
			etu::AnalogOut(1, (float)-9.95, false);

			// if the blank time is greater than zero, then change the trial phase signal
			// and wait for the blank period
			if (g_pCircleAnnulus->getBlankPeriod() > 0) {
				etu::AnalogOut(3, 5, true);
				WaitUntil(g_pCircleAnnulus->getBlankPeriod()  - myTimer->CheckTimer(), 5);
			} 

			// Add the intertrial delay -- set the trial phase signal to the intertrial voltage
			// and wait for the intertrial period. (This timing is not perfect, but it is the only
			// phase where some jitter is allowable).
			etu::AnalogOut(3, -5, true);
			WaitUntil(g_pCircleAnnulus->getIntertrialDuration() - elapsedTime, 5);
			myTimer->CheckTimerWithReset();
			iPage = 1 - iPage;

			// if the stop button has been pressed, stop execution of the stimulus presentations
			if (bEndThread) {
				//theComm.stop();
				bTrialsInitialized = false;
				return;
			}
		}
	}
	
	// stop execution of the stimulus. Close the serial port, and change the display strings
	// to blank values. 
	//theComm.stop();
	g_pCircleAnnulusDlg->updateDisplayStrings(-1, -1, -1, 0);

	// reset the trial flags
	bStartTrials = false;
	bTrialsInitialized = false;

	// signal the end of the presentation
	bEndThread = true;
}

/***********************************************************
	 IntializeTrials()     
	 -  Initializes circle / annulus trials based on the parameters
		set in the dialog box.
************************************************************/
bool CCircleAnnulusStimulus::InitializeTrials() 
{
	float fNumCircles = 1;
	float fNumAnnuli = 1;
	float totalNumCircles = 0;
	float totalNumAnnuli = 0;
	double xLocationLeft, yLocationLeft;
	double xLocationRight, yLocationRight;
	bool bUseLeft = false;
	bool bUseRight = false;
	
	// clear the drawing pages and reset the display coordinates
	vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
    vsgPresent();
	vsgSetDrawPage(vsgVIDEOPAGE, 1-iPage, vsgBACKGROUND);
	vsgPresent();
	vsgMoveScreen(0, 0);

	// if the annulusTesting array exists, delete it
	// since we will be recreating it in this function
	if (pAnnulusTesting) delete [] pAnnulusTesting;

	// if we are stimulating the left eye field, or interleaving fields, set the left eye position to the appropriate coordinates
	if (g_pCircleAnnulus->getField() == CCircleAnnulus::FIELD_LEFT_EYE || g_pCircleAnnulus->getField() == CCircleAnnulus::FIELD_INTERLEAVED) {
		bUseLeft = true;
		xLocationLeft = g_receptiveFieldLeft.xPos;
		yLocationLeft = g_receptiveFieldLeft.yPos;
	} 
	
	// if we are stimulating the right eye field, or interleaving fields, set the right eye position to the appropriate coordinates
	if (g_pCircleAnnulus->getField() == CCircleAnnulus::FIELD_RIGHT_EYE || g_pCircleAnnulus->getField() == CCircleAnnulus::FIELD_INTERLEAVED) {
		bUseRight = true;
		xLocationRight = g_receptiveFieldRight.xPos;
		yLocationRight = g_receptiveFieldRight.yPos;
	} 

	// if the fields are set to manual -- set the local represenation of the left eye fields to the manual field location, and 
	// set the flags to use the left eye locations only. (This is kind of a hack, so we may want to make the manual setting have its own
	// local representation at some point -- but it works for now).
	if (g_pCircleAnnulus->getField() == CCircleAnnulus::FIELD_MANUAL) {
		bUseLeft = true;
		xLocationLeft = g_pCircleAnnulus->getXPosition();
		yLocationLeft = g_pCircleAnnulus->getYPosition();
	}
	
	// calculate the total number of circles to present
	// numCircles = ((radiusMax - radiusMin) / radiusStep + 1) * numColors
	if (g_pCircleAnnulus->getShape() != CCircleAnnulus::SHAPES_ANNULI) {
		fNumCircles = (float)((int)(((g_pCircleAnnulus->getCircleRadiusMax() - g_pCircleAnnulus->getCircleRadiusMin()) / g_pCircleAnnulus->getCircleRadiusStep())+1));
		totalNumCircles = fNumCircles * g_pCircleAnnulus->getNumColors();
	}

	// calculate the total number of annuli to present
	// numAnnuli = numCircles * ((annulusWidthMax - annulusWidthMin) / annulusWidthStep + 1)
	if (g_pCircleAnnulus->getShape() != CCircleAnnulus::SHAPES_CIRCLES) {
		fNumAnnuli = (float)((int)(((g_pCircleAnnulus->getAnnulusWidthMax() - g_pCircleAnnulus->getAnnulusWidthMin()) / g_pCircleAnnulus->getAnnulusWidthStep())+1));
		totalNumAnnuli = fNumAnnuli * totalNumCircles;
	} 

	// add number of annuli and circles to get total number of presentations
	iNumAnnulusRuns = (int)totalNumAnnuli + (int)totalNumCircles;

	// if interleaved presentations, double the number of presentations
	if (bUseLeft && bUseRight) {
		iNumAnnulusRuns *= 2;
	}

	// make sure there is at least one presentation
	if (iNumAnnulusRuns == 0) {
		AfxMessageBox("There are no stimuli! Please change \nsome settings and try again.");
		return false;
		 
	}

	// allocate space for the the annulusTesting array
	pAnnulusTesting = new AnnulusTestingRun[iNumAnnulusRuns];

	// iterate through the range of circle sizes
	int index = 0;
	for (int i = 0; i < fNumCircles; i++) {
		// calcualte the current circle radius
		float circleRadius = g_pCircleAnnulus->getCircleRadiusMin() + i * g_pCircleAnnulus->getCircleRadiusStep();

		// iterate through the range of colors
		for (int k = 0; k < g_pCircleAnnulus->getNumColors(); k++) {
			// iterate through the number of annuli
			for (int j = 0; j < fNumAnnuli; j++) {
				// set the standard annulus width to zero
				float annulusWidth = 0;
				// if annuli are to be presented, calculate the current annulus width
				if (g_pCircleAnnulus->getShape() != CCircleAnnulus::SHAPES_CIRCLES) {
					annulusWidth = g_pCircleAnnulus->getAnnulusWidthMin() + j * g_pCircleAnnulus->getAnnulusWidthStep();
				}

				// if the left eye field is to be used, then create an entry in the array, then 
				// make the appropriate entries in the array to display a circle, an annulus or both
				// to the left eye field
				if (bUseLeft) {
					if (g_pCircleAnnulus->getShape() != CCircleAnnulus::SHAPES_ANNULI && j==0) {
						pAnnulusTesting[index].annulusWidth = annulusWidth;
						pAnnulusTesting[index].circleRadius = circleRadius;
						pAnnulusTesting[index].color.a = g_pCircleAnnulus->getColor(k).red;
						pAnnulusTesting[index].color.b = g_pCircleAnnulus->getColor(k).green;
						pAnnulusTesting[index].color.c = g_pCircleAnnulus->getColor(k).blue;
						pAnnulusTesting[index].bCircle = true;
						pAnnulusTesting[index].nRunsLeft = g_pCircleAnnulus->getRepeats();
						pAnnulusTesting[index].index = index;
						pAnnulusTesting[index].xLocation = xLocationLeft;
						pAnnulusTesting[index].yLocation = yLocationLeft;
						index++;
					}
					if (g_pCircleAnnulus->getShape() != CCircleAnnulus::SHAPES_CIRCLES) {
						pAnnulusTesting[index].annulusWidth = annulusWidth;
						pAnnulusTesting[index].circleRadius = circleRadius;
						pAnnulusTesting[index].color.a = g_pCircleAnnulus->getColor(k).red;
						pAnnulusTesting[index].color.b = g_pCircleAnnulus->getColor(k).green;
						pAnnulusTesting[index].color.c = g_pCircleAnnulus->getColor(k).blue;
						pAnnulusTesting[index].bCircle = false;
						pAnnulusTesting[index].nRunsLeft = g_pCircleAnnulus->getRepeats();
						pAnnulusTesting[index].index = index;
						pAnnulusTesting[index].xLocation = xLocationLeft;
						pAnnulusTesting[index].yLocation = yLocationLeft;
						index++;
					}
				} 
				// if the right eye field is to be used, then create an entry in the array, then 
				// make the appropriate entries in the array to display a circle, an annulus or both
				// to the right eye field
				if (bUseRight) {
					if (g_pCircleAnnulus->getShape() != CCircleAnnulus::SHAPES_ANNULI && j==0) {
						pAnnulusTesting[index].annulusWidth = annulusWidth;
						pAnnulusTesting[index].circleRadius = circleRadius;
						pAnnulusTesting[index].color.a = g_pCircleAnnulus->getColor(k).red;
						pAnnulusTesting[index].color.b = g_pCircleAnnulus->getColor(k).green;
						pAnnulusTesting[index].color.c = g_pCircleAnnulus->getColor(k).blue;
						pAnnulusTesting[index].bCircle = true;
						pAnnulusTesting[index].nRunsLeft = g_pCircleAnnulus->getRepeats();
						pAnnulusTesting[index].index = index;
						pAnnulusTesting[index].xLocation = xLocationRight;
						pAnnulusTesting[index].yLocation = yLocationRight;
						index++;
					}
					if (g_pCircleAnnulus->getShape() != CCircleAnnulus::SHAPES_CIRCLES) {
						pAnnulusTesting[index].annulusWidth = annulusWidth;
						pAnnulusTesting[index].circleRadius = circleRadius;
						pAnnulusTesting[index].color.a = g_pCircleAnnulus->getColor(k).red;
						pAnnulusTesting[index].color.b = g_pCircleAnnulus->getColor(k).green;
						pAnnulusTesting[index].color.c = g_pCircleAnnulus->getColor(k).blue;
						pAnnulusTesting[index].bCircle = false;
						pAnnulusTesting[index].nRunsLeft = g_pCircleAnnulus->getRepeats();
						pAnnulusTesting[index].index = index;
						pAnnulusTesting[index].xLocation = xLocationRight;
						pAnnulusTesting[index].yLocation = yLocationRight;
						index++;
					}
				}
			}
		}
	}

	// intiialize the serial port used for the text mark transmissions
	//theComm.init(false, CBR_115200, 1);
	bTrialsInitialized = true;

	return true;
}

/***********************************************************
	 DrawCircle()     
	 -  Draws a circle based on the parameters in the
		AnnulusTestingRun structure passed in
************************************************************/
void CCircleAnnulusStimulus::DrawCircle(AnnulusTestingRun currentRun) 
{	
	// set the background color
	vsgSetBackgroundColour(&bgColor);
	
	//Set the current drawing page to a blank page
	vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
	
	// setup the OAS to draw the circle
	vsgSetDrawMode(vsgCENTREXY);
	vsgObjDestroy(g_stim);
	g_stim = vsgObjCreate();
	vsgObjSetContrast(100);
	vsgObjSetPixelLevels(2,1);

	// set the origin to the center of the screen
	vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix / 2);

	// set the color for the circle
	vsgObjSetColourVector(&blackColor,&currentRun.color,vsgBIPOLAR);

	// convert the degrees to pixels
	double dRadius;
	double startX, startY;

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.circleRadius, vsgPIXELUNIT, &dRadius);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.xLocation, vsgPIXELUNIT, &startX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -currentRun.yLocation, vsgPIXELUNIT, &startY);
	
	// set the drawing mode to where the coordinates specified for the cirlce
	// are the coordinates for the center of the circle.
	vsgSetDrawMode(vsgCENTREXY);
	vsgDrawOval(startX, startY, dRadius*2, dRadius*2);

	// Create a text mark string that contains the color, radius, location and marker code
	// for this presentation
	char buffer[80];
	sprintf(buffer, "Circle R %.2f RGB: %.2f, %.2f, %.2f, x: %.2f, y: %.2f|0x0%x/", currentRun.circleRadius, currentRun.color.a, currentRun.color.b, currentRun.color.c, currentRun.xLocation, currentRun.yLocation, currentRun.index+1);
	int length = (int)strlen(buffer);
	elapsedTime = (int)myTimer->CheckTimerWithReset();

	// present the stimulus
	vsgPresent();

	// update the anlog out for the target position, and the trial phase signal
	etu::AnalogOut(0, (float)currentRun.xLocation / g_pSettings->getDegPerVolt(), false);
	etu::AnalogOut(1, (float)currentRun.yLocation / g_pSettings->getDegPerVolt(), false);
	etu::AnalogOut(3, (float)0, true); // trial phase

	// reset the timer, indicating the stimulus was just turned on
	myTimer->CheckTimerWithReset();

	// transmit the text mark string
	//theComm.transmit(buffer, length);
	g_pSettings->theSerialComm.transmit(buffer, length);
	iPage = 1-iPage;
}

/***********************************************************
	 DrawAnnulus()     
	 -  Draws a annulus based on the parameters in the
		AnnulusTestingRun structure passed in
************************************************************/
void CCircleAnnulusStimulus::DrawAnnulus(AnnulusTestingRun currentRun) 
{
	// set the background color
	vsgSetBackgroundColour(&bgColor);

	//Set the current drawing page to a blank page
	vsgSetDrawPage(vsgVIDEOPAGE,iPage,vsgBACKGROUND);
	
	// setup the OAS to draw the annulus
	vsgSetDrawMode(vsgCENTREXY);
	vsgObjDestroy(g_stim);
	g_stim = vsgObjCreate();
	vsgObjSetContrast(100);
	vsgObjSetPixelLevels(2,1);

	// set the drawing origin to the center of the screen
	vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix / 2);

	// set the annulus color
	vsgObjSetColourVector(&blackColor,&currentRun.color,vsgBIPOLAR);
	
	// convert the degrees to pixels
	double dRadius;
	double dRadius2;
	double startX, startY;

	// circle radius is the center circle (the hole in the middle)
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.circleRadius, vsgPIXELUNIT, &dRadius);

	// radius 2 is the outer radius of the annulus
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.circleRadius+currentRun.annulusWidth, vsgPIXELUNIT, &dRadius2);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, currentRun.xLocation, vsgPIXELUNIT, &startX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -currentRun.yLocation, vsgPIXELUNIT, &startY);
	
	// draw the outer circle
	vsgSetDrawMode(vsgCENTREXY);
	vsgDrawOval(startX, startY, dRadius2, dRadius2);

	// create a new OAS object for the center circle (the hole)
	vsgObjDestroy(g_fixation);
	g_fixation = vsgObjCreate();
	vsgObjSetContrast(100);
	vsgObjSetPixelLevels(4, 1);

	// set the origin to the center of the screen
	vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix / 2);

	// set the circle color to the background
	vsgObjSetColourVector(&bgColor,&bgColor,vsgBIPOLAR);

	// draw the inner circle to make the hole in teh annulus
	vsgDrawOval(startX, startY, dRadius, dRadius);

	// Create a text mark string that contains the color, radius, location and marker code
	// for this presentation
	char buffer[80];
	sprintf(buffer, "Annulus W %.2f,Circle R %.2f RGB: %.2f, %.2f, %.2f x: %.2f, y: %.2f|0x0%x/", currentRun.annulusWidth, currentRun.circleRadius, currentRun.color.a, currentRun.color.b, currentRun.color.c, currentRun.xLocation, currentRun.yLocation, currentRun.index+1);
	int length = (int)strlen(buffer);
	elapsedTime = (int)myTimer->CheckTimerWithReset();

	// present the annulus (Draws both circles) at the "same" time
    vsgPresent();

	// update the analog out for the target position and the trial phase
	etu::AnalogOut(0, (float)currentRun.xLocation / g_pSettings->getDegPerVolt(), false);
	etu::AnalogOut(1, (float)currentRun.yLocation / g_pSettings->getDegPerVolt(), false);
	etu::AnalogOut(3, 0, true); // trial phase
	
	// reset the timer
	myTimer->CheckTimerWithReset();

	// send the text mark
	//theComm.transmit(buffer, length);
	g_pSettings->theSerialComm.transmit(buffer, length);
	iPage = 1-iPage;
}

/***********************************************************
	 InitializeTrialsOnOff     
	 -  Initializes On / Off trials. These trials immediately
	 transition from one stimulus to the next, without transitioning 
	 to the background between presentations.
************************************************************/
bool CCircleAnnulusStimulus::InitializeTrialsOnOff() 
{
	VSGLUTBUFFER buff;
	VSGTRIVAL paletteColors[3];

	// make sure there is at least 1 primary color and 1 secondary color
	if (g_pCircleAnnulus->getNumColors() < 1 || g_pCircleAnnulus->getNumSecondaryColors() < 1) {
		AfxMessageBox("There must be at least 1 primary and secondary color.");
		return false;
	}

	// initialize the palette to the background color
	for (int i = 0; i < 255; i++) {
			buff[i].a = ((double)g_pSettings->getRedBG())/255.0f;
			buff[i].b = ((double)g_pSettings->getGreenBG())/255.0f;
			buff[i].c = ((double)g_pSettings->getBlueBG())/255.0f;
	}
	vsgSetBackgroundColour(&buff[vsgBACKGROUND]);
	vsgSetFixationColour(&buff[vsgFIXATION]);
	vsgLUTBUFFERWrite(0, &buff);
	vsgLUTBUFFERtoPalette(0);


	// set the LUT locations 0 and 1 to the background color
	paletteColors[0].a = ((double)g_pSettings->getRedBG())/255.0f;
	paletteColors[0].b = ((double)g_pSettings->getGreenBG())/255.0f;
	paletteColors[0].c = ((double)g_pSettings->getBlueBG())/255.0f;

	paletteColors[1].a = ((double)g_pSettings->getRedBG())/255.0f;
	paletteColors[1].b = ((double)g_pSettings->getGreenBG())/255.0f;
	paletteColors[1].c = ((double)g_pSettings->getBlueBG())/255.0f;

	// convert degrees to pixels
    double centerX, centerY, annulusRadius, circleRadius;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pCircleAnnulus->getXPosition(), vsgPIXELUNIT, &centerX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pCircleAnnulus->getYPosition(), vsgPIXELUNIT, &centerY);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pCircleAnnulus->getAnnulusWidthMin() + g_pCircleAnnulus->getCircleRadiusMin(), vsgPIXELUNIT, &annulusRadius);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pCircleAnnulus->getCircleRadiusMin(), vsgPIXELUNIT, &circleRadius);

	// set the background color on the drawing page
	vsgSetBackgroundColour(&paletteColors[0]);
	vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgBACKGROUND);
	
	// set the drawing mode, and set the origin to the center of the screen
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix  /2, iScrHeightPix / 2);

	// draw the outer circle (annulus) with the colors set in LUT location 0
	vsgSetPen1(0);
	vsgDrawOval(centerX,centerY, annulusRadius*2, annulusRadius*2);
	vsgPresent();

	// draw the inner circle (circle, and when the annulus is displayed the hole) with the colors set in LUT location 1
	vsgSetPen1(1);
	vsgDrawOval(centerX,centerY, circleRadius*2, circleRadius*2);
	vsgPresent();
	
	// make sure the LUT is set to only display the background at this time
	vsgPaletteSet(0, 0, &(paletteColors[0]));
	vsgPaletteSet(1, 1, &(paletteColors[1]));

	bTrialsInitialized = true;
	bPrimaryColor = true;
	myTimer->StartTimer();
	
	// set the analog out to target position of the circle / annulus
	etu::AnalogOut(0, g_pCircleAnnulus->getXPosition() / g_pSettings->getDegPerVolt(), false);
	etu::AnalogOut(1, g_pCircleAnnulus->getYPosition()/ g_pSettings->getDegPerVolt(), true);

	// create the text mark describing the set of trials about to run
	char buffer[80];
	if (g_pCircleAnnulus->getShape() == CCircleAnnulus::SHAPES_CIRCLES) {
		sprintf(buffer, "C: Circle R %.2f RGB: %.2f, %.2f, %.2f RGB2: %.2f, %.2f, %.2f|0x00/", g_pCircleAnnulus->getCircleRadiusMin(), g_pCircleAnnulus->getColor(0).red, g_pCircleAnnulus->getColor(0).green, g_pCircleAnnulus->getColor(0).blue, g_pCircleAnnulus->getSecondaryColor(0).red, g_pCircleAnnulus->getSecondaryColor(0).green, g_pCircleAnnulus->getSecondaryColor(0).blue);
	} 
	else if (g_pCircleAnnulus->getShape() == CCircleAnnulus::SHAPES_ANNULI) {
		sprintf(buffer, "A: Annulus W: %.2f Circle R %.2f RGB: %.2f, %.2f, %.2f RGB2: %.2f, %.2f, %.2f|0x01/", g_pCircleAnnulus->getAnnulusWidthMin(), g_pCircleAnnulus->getCircleRadiusMin(), g_pCircleAnnulus->getColor(0).red, g_pCircleAnnulus->getColor(0).green, g_pCircleAnnulus->getColor(0).blue, g_pCircleAnnulus->getSecondaryColor(0).red, g_pCircleAnnulus->getSecondaryColor(0).green, g_pCircleAnnulus->getSecondaryColor(0).blue);
	} 
	else {
		sprintf(buffer, "CA: Annulus W: %.2f Circle R %.2f RGB: %.2f, %.2f, %.2f RGB2: %.2f, %.2f, %.2f|0x02/", g_pCircleAnnulus->getAnnulusWidthMin(), g_pCircleAnnulus->getCircleRadiusMin(), g_pCircleAnnulus->getColor(0).red, g_pCircleAnnulus->getColor(0).green, g_pCircleAnnulus->getColor(0).blue, g_pCircleAnnulus->getSecondaryColor(0).red, g_pCircleAnnulus->getSecondaryColor(0).green, g_pCircleAnnulus->getSecondaryColor(0).blue);
	}
	int length = (int)strlen(buffer);
	
	//initialize the serial port
	//theComm.init(false, CBR_115200, 1);

	// transmit the text mark to spike 2
	g_pSettings->theSerialComm.transmit(buffer, length);

	// update the display strings on the dialog box
	g_pCircleAnnulusDlg->updateDisplayStrings(g_pCircleAnnulus->getCircleRadiusMin(), g_pCircleAnnulus->getAnnulusWidthMin(), -1, -1);

	return true;
}

/***********************************************************
	 DisplayTrialsOnOff     
	 -  Display On / Off trials. These trials immediately
	 transition from one stimulus to the next, without transitioning 
	 to the background between presentations.
************************************************************/
void CCircleAnnulusStimulus::DisplayTrialsOnOff() 
{
	// if trials have not been initialized, then try to initialize them
	// if initializatin fails, then abort.
	if (!bTrialsInitialized) 
		if (!InitializeTrialsOnOff()) {
			bEndThread = true;
			return;
		}
	int voltOut;
	VSGLUTBUFFER lutValues;
	
	// if we are only displaying cirlces
	if (g_pCircleAnnulus->getShape() == CCircleAnnulus::SHAPES_CIRCLES) {
		// change the LUT values for location 1 based on the color of the circle
		// and set the voltage output for the trialPhase signal to +5 for primary color
		// and -5 for secondary
		if (bPrimaryColor) {
			lutValues[1].a = g_pCircleAnnulus->getColor(0).red;
			lutValues[1].b = g_pCircleAnnulus->getColor(0).green;
			lutValues[1].c = g_pCircleAnnulus->getColor(0).blue;
			voltOut = 5;
		} 
		else {
			lutValues[1].a = g_pCircleAnnulus->getSecondaryColor(0).red;
			lutValues[1].b = g_pCircleAnnulus->getSecondaryColor(0).green;
			lutValues[1].c = g_pCircleAnnulus->getSecondaryColor(0).blue;
			voltOut = -5;
		}

		// make sure the lut location 0 is set to the background
		lutValues[0].a = ((float)g_pSettings->getRedBG())/255.0f;
		lutValues[0].b = ((float)g_pSettings->getGreenBG())/255.0f;
		lutValues[0].c = ((float)g_pSettings->getBlueBG())/255.0f;

		// wait until the current trial time has expired
		WaitUntil(g_pCircleAnnulus->getTrialDuration() - myTimer->CheckTimer(), 5);

		// write the new values to the LUT, thereby updating the display to the 
		// new colors
		vsgPaletteWrite(&lutValues, 0, 2);

		// reset the timer
		myTimer->StartTimer();

	} 
	else if (g_pCircleAnnulus->getShape() == CCircleAnnulus::SHAPES_ANNULI) {
		// if we are only displaying annuli, change the LUT values for location 0 based 
		// on the color of the circle and set the voltage output for the trialPhase signal 
		// to +5 for primary color and -5 for secondary.
		if (bPrimaryColor) {
			lutValues[0].a = g_pCircleAnnulus->getColor(0).red;
			lutValues[0].b = g_pCircleAnnulus->getColor(0).green;
			lutValues[0].c = g_pCircleAnnulus->getColor(0).blue;
			voltOut = 5;
		} 
		else {
			lutValues[0].a = g_pCircleAnnulus->getSecondaryColor(0).red;
			lutValues[0].b = g_pCircleAnnulus->getSecondaryColor(0).green;
			lutValues[0].c = g_pCircleAnnulus->getSecondaryColor(0).blue;
			voltOut = -5;
		}
		// make sure the lut location 1 is set to the background
		lutValues[1].a = ((float)g_pSettings->getRedBG())/255.0f;
		lutValues[1].b = ((float)g_pSettings->getGreenBG())/255.0f;
		lutValues[1].c = ((float)g_pSettings->getBlueBG())/255.0f;

		// wait until the current trial time has expired
		WaitUntil(g_pCircleAnnulus->getTrialDuration() - myTimer->CheckTimer(), 5);

		// write the new value to the LUT, thereby updating the display to the
		// new colors
		vsgPaletteWrite(&lutValues, 0, 2);

		// reset the timer
		myTimer->StartTimer();
	} 
	else { // we are alternating between circles and annuli
		
		// In this case, we use the primary color for the circle, and the secondary
		// color for the annulus on one trial. The next presentation, the colors are swapped
		// to where the primary color is on the annulus and the secondary color is on the circle.
		// In the first case, the voltage out is set to 5V and in the second case the trial
		// phase voltage is set to -5V
		if (bPrimaryColor) {
			lutValues[0].a = g_pCircleAnnulus->getColor(0).red;
			lutValues[0].b = g_pCircleAnnulus->getColor(0).green;
			lutValues[0].c = g_pCircleAnnulus->getColor(0).blue;
			lutValues[1].a = g_pCircleAnnulus->getSecondaryColor(0).red;
			lutValues[1].b = g_pCircleAnnulus->getSecondaryColor(0).green;
			lutValues[1].c = g_pCircleAnnulus->getSecondaryColor(0).blue;
			voltOut = 5;

		} 
		else {
			lutValues[1].a = g_pCircleAnnulus->getColor(0).red;
			lutValues[1].b = g_pCircleAnnulus->getColor(0).green;
			lutValues[1].c = g_pCircleAnnulus->getColor(0).blue;
			lutValues[0].a = g_pCircleAnnulus->getSecondaryColor(0).red;
			lutValues[0].b = g_pCircleAnnulus->getSecondaryColor(0).green;
			lutValues[0].c = g_pCircleAnnulus->getSecondaryColor(0).blue;
			voltOut = -5;
		}

		// wait until the current trial time has expired
		WaitUntil(g_pCircleAnnulus->getTrialDuration() - myTimer->CheckTimer(), 5);

		// write the new values to the LUT, thereby updating the display to the
		// new colors
		vsgPaletteWrite(&lutValues, 0, 2);

		// reset the timer
		myTimer->StartTimer();
	}

	// update the trial phase voltage
	etu::AnalogOut(3, (float)voltOut, true);

	// change the primary color flag.
	bPrimaryColor = !bPrimaryColor;
}

/***********************************************************
	 StartTrials()     
	 -  sets the trialSTart flag to true. This is called
	 by the dialog box when the start button is pressed.
************************************************************/
void CCircleAnnulusStimulus::StartTrials(bool bStart) 
{
	bStartTrials = true;
}

/***********************************************************
	 StimulusType()     
	 -  Returns the stimulus name.
************************************************************/
CString CCircleAnnulusStimulus::StimulusType() 
{
	return "Circle Annulus Stimulus";
}

/***********************************************************
	 StopExecution    
	 -  overrides the generic stop execution method to close 
	 the serial port when execution is stopped.
************************************************************/
void CCircleAnnulusStimulus::StopExecution() 
{
	//theComm.stop();
	bEndThread = true;
}

#endif // APP_EYE_TRACKING
