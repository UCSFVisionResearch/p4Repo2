#ifdef APP_ACUITY
#include "app/AppCore.h"

/***********************************************************
COMMUNICATIONS:
	VSG Out 0:	Trial Running pulse. 1 => trial is running. 
				0 => trial is not running
				only "run trial" during reward period

	VSG IN 0:	0x1 vsgDIG0 end trial
	VSG IN 1:	0x2 vsgDIG1 correct/incorrect
	VSG IN 2:	0x4 vsgDIG2 next stim cue to start trial

	Analog Out 0:	Horizontal target location
	Analog Out 1:	Vertical target location
	Analog Out 2:	spatial frequency
	Analog Out 3:	contrast
************************************************************/



/***********************************************************
	 Constructor     
	 -  Initializes the newly instantiated object. 
		Sets up staircase objects needed for the stimulus. 
************************************************************/

CPrefSelectionStimulus::CPrefSelectionStimulus() {
	Initialize();
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	
	iPage = 0;
	g_pPrefSelection->generateLevelsList();
	m_pLevels = g_pPrefSelection->getLevels();

	for (int i = 0; i < g_pPrefSelection->getNumLevels(); i++) {
		m_pLevels[i].pStaircase = new CStaircase(g_pPrefSelection->getStaircaseSettings());
		m_pLevels[i].pStaircase->setInitialValue(m_pLevels[i].initialContrast);
		m_pLevels[i].pStaircase->init();
	}

	vsgMoveScreen(0, 0);

	g_pPrefSelection->sendSettings(g_pSettings->theSerialComm);
	srand( (unsigned)time( NULL ) );
}


/*******************************************************************
	Destructor
		- Destroys any objecs that were instantiated on the heap.
		In this case, no objects were created within this stimulus
		thus nothing needs to be done. (m_pLevels is created and 
		destroyed in the CPrefSelection class.

********************************************************************/
CPrefSelectionStimulus::~CPrefSelectionStimulus() {
	vsgObjDestroy(g_grating);
	vsgObjDestroy(g_stim);
	vsgObjDestroy(g_fixation);

	VSGLUTBUFFER lb;
	for (int i=0; i<256; i++) {
		lb[i].a = ((double)g_pSettings->getRedBG()) / 255.0f;;
		lb[i].b = ((double)g_pSettings->getGreenBG()) / 255.0f;;
		lb[i].c = ((double)g_pSettings->getBlueBG()) / 255.0f;;
	}
	vsgSetBackgroundColour(&lb[vsgBACKGROUND]);
	vsgSetFixationColour(&lb[vsgFIXATION]);
	vsgLUTBUFFERWrite(iPage,&lb);
	vsgLUTBUFFERtoPalette(iPage);
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	
	//send stop mark
	char buff[80];
	sprintf(buff, "PrefSelection;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buff, (int)strlen(buff));
}



/******************************************************************
	DisplayStimulus
		This function coordinates a single iteration of the contrast
		sensitivity trial. The result of this iteration is used to
		update the staircases stored in (m_pLevels).
	
	Arguments:
		none.
	Return Value:
		- returns true on success. Returning false will abort 
		the stimulus.
*******************************************************************/
bool CPrefSelectionStimulus::DisplayStimulus() {

		
	// randomly choose a side for the presentation based on the percentage given 
	// in the dialog box
	int iSide = (rand() % 100) + 1;
	if (iSide < g_pPrefSelection->getPctTrialsRight()) {
		iSide = 1;
	} else {
		iSide = -1;
	}
	
	// calculate the grating location based on the side
	float fGratingLoc = iSide * g_pPrefSelection->getGratingPosition();
	
	float contrast, spatialFreq;

	// Perform a variety of setup tasks such as:
	// - Randomly pick an active staircase to present a trial from
	// - Intialize colors (dotColor etc)
	// - Convert degree values to pixels.
	if (!SetupTrial(contrast, spatialFreq, iSide)) {
		bEndThread = true;
		return false;
	}
	

	// Display the pretrial fixation point 
	if (!DisplayPretrial(contrast)) {
		bEndThread = true;
		return false;
	}
		
	// Display the gratings on the screen as soon as the
	// the CED sends the trial start pulse indicating the end 
	// of the pretrial period.
	if (!DisplayTrial(fGratingLoc, iSide, contrast)) {
		bEndThread = true;
		return false;
	}

	// Wait the Target Delay period -- then display
	// the exact same gratings as above and add the two
	// target fixation dots to each side
	if (!DisplayTrialTargets(fGratingLoc, iSide, contrast)) {
		bEndThread = true;
		return false;
	}

	// Wait for the CED send the trial end pulse indicating that
	// the subject has fixated a target for a sufficient amount of time.
	// Check the result of the trial, and update the appropriate
	// staircase accordingly.
	if (!GetTrialResult()) {
		bEndThread = true;
		return false;
	}


	// iterate through all staircases and determine if we need to
	// present any more trials
	bool bContinue = false;
	for (int i = 0; i < g_pPrefSelection->getNumLevels(); i++) {
		if (!m_pLevels[i].pStaircase->isComplete()) {
			bContinue = true;
		} 
	}

	// if all staircases have completed, then
	// destroy all of the vsg display objects
	// and display the results from all of the staircases.
	if (!bContinue) {
		bEndThread = true;
		vsgObjDestroy(g_grating);
		vsgObjDestroy(g_stim);
		vsgObjDestroy(g_fixation);
		DisplayResults();
	}

	// return successfully
	return true;
}



/******************************************************************
	SetupTrial
		This function performs various setup tasks for the trial run.
		It chooses which staircase to present next, intializes the 
		colors, convert degree values into pixels, and update the 
		trial info on teh dialog box.
	
	Arguments:
		&contrast - variable for the contrast of the current trial.
		This values is passed by reference to allow this function to 
		set this variable for the rest of the trial.

		&spatialFreq - variable for the requested spatialFreq of the 
		current trial. 	This values is passed by reference to allow 
		this function to set this variable for the rest of the trial.

		iSide -	An integer describing which side the correct grating
		is displayed on. +1 indicates teh grating is on the right, -1 indicates
		the grating is on the left. Any other values will result in
		unpredicatble behvior!


	Return Value:
		- returns true on success. Returning false will abort 
		the stimulus.
*******************************************************************/
bool CPrefSelectionStimulus::SetupTrial(float& contrast, float& spatialFreq, int iSide) {

	// pick which staircase will be presented
	randomStaircase = rand() % g_pPrefSelection->getNumLevels();
	while (m_pLevels[randomStaircase].pStaircase->isComplete()) {
		randomStaircase = rand() % g_pPrefSelection->getNumLevels();
	}

	// extract the contrast and spacial freq from the staircase selected
	contrast = m_pLevels[randomStaircase].pStaircase->getCurrentLevel();
	spatialFreq = m_pLevels[randomStaircase].spatialFreq;

	// initialize all the colors
	dotColor.a = ((double)g_pSettings->getRed()) / 255.0f;
	dotColor.b = ((double)g_pSettings->getGreen()) / 255.0f;
	dotColor.c = ((double)g_pSettings->getBlue()) / 255.0f;

	whiteColor.a = 1.0f;
	whiteColor.b = 1.0f;
	whiteColor.c = 1.0f;

	grayColor.a = 0.5f;
	grayColor.b = 0.5f;
	grayColor.c = 0.5f;

	bgColor.a = 0;
	bgColor.b = 0;
	bgColor.c = 0;

	
	// set the parameters for the display
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);	
	vsgSetBackgroundColour(&bgColor);
	vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix /2 );

	// convert the degrees to pixels
	//etu::tanScreenUnit2Unit(vsgDEGREEUNIT, g_pPrefSelection->getDotSize(), vsgPIXELUNIT, &dSizePix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pPrefSelection->getDotSize(), vsgPIXELUNIT, &dSizePix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, 1, vsgPIXELUNIT, &dDegPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &xOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &yOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);

	// calculate the actual spatial frequency
	actualSpatialFreq = CreateFilter(dDegPix, spatialFreq);

	// adjust the spatial frequency in the staircase object to reflect
	// the actual spatial frequency.
	m_pLevels[randomStaircase].spatialFreq = actualSpatialFreq;

	// update the display on the dialog
	g_pPrefSelectionDlg->setCurrParameters(actualSpatialFreq, contrast, iSide, m_pLevels[randomStaircase].pStaircase->getReversals(), m_pLevels[randomStaircase].pStaircase->getStepSize());

	//send settings as a textmark
	char buffer[80];
	sprintf(buffer, "c:%.2f f:%.2f|0x%02x/\0", contrast, actualSpatialFreq, 100+randomStaircase);
	//AfxMessageBox(buffer, 0, 0);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));

	return true;
}



/******************************************************************
	DisplayPretrial
		This function sets up the pretrial portion of the trial run.
		A fixation dot is drawn as specified by the parameters in the 
		dialog box. This function does not wait for the end of the 
		pretrial period -- it only draws the dot and updates
		the analog out accordingly.
	
	Arguments:
		contrast - the contrast of the current trial run.
	Return Value:
		- returns true on success. Returning false will abort 
		the stimulus.
*******************************************************************/
bool CPrefSelectionStimulus::DisplayPretrial(float contrast) {
	// intialize the video page to draw the fixation target 
	// to initiate the trial
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	vsgSetDrawMode(vsgCENTREXY);

	vsgObjDestroy(g_fixation);
	g_fixation = vsgObjCreate();
	vsgObjSetContrast(100);
	vsgObjSetPixelLevels(0,1);
	vsgObjSetColourVector(&blackColor,&dotColor,vsgBIPOLAR);
	
	// draw the g_fixation target
	vsgDrawOval(xOffset + screenXOffset, yOffset + screenYOffset, dSizePix, dSizePix);
	vsgPresent();

	// send a "trial not running" signal to the CED
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	
	
	// retrieve the degree offsets
	xDeg = g_pSettings->getXOffset();
	yDeg = -g_pSettings->getYOffset();

    // update the analog output to reflect the presentation of the fixation dot.
	// the spatial frequency and contrast for the current trial is also updated
	// at this time.
	etu::AnalogOut(0, xDeg / g_pSettings->getDegPerVolt()); 
	etu::AnalogOut(1, yDeg / g_pSettings->getDegPerVolt());
	etu::AnalogOut(2, actualSpatialFreq / 10.0f -5);
	etu::AnalogOut(3, contrast / 10.0f - 5);

	return true;
}



/******************************************************************
	DisplayTrial
		This function displays the two grating patches as soon as the
		CED sigals "Trial start". This function does not wait until
		the TargetDelay has expired -- it only draws the grating 
		patches.
	
	Arguments:
		fGraingLoc - The location of the correct grating in degrees.

		iSide -	An integer describing which side the correct grating
		is displayed on. +1 indicates teh grating is on the right, -1 indicates
		the grating is on the left. Any other values will result in
		unpredicatble behvior!

		contrast - the contrast level for the current display. This value
		should be between 0 and 100. Values above 100 will result in 100%
		contrast. Values below 0 will result in 0 contrast.

	Return Value:
		- returns true on success. Returning false will abort 
		the stimulus.
*******************************************************************/
bool CPrefSelectionStimulus::DisplayTrial(float fGratingLoc, int iSide, float contrast) {
	// flip to the other video page, and use that to draw actual
	// stimulus screen.
	iPage = 1-iPage;
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	vsgSetDrawMode(vsgCENTREXY);
	
	// Call the DrawPatches function to draw the two patches on the 
	// offscreen video page (patches not displayed yet -- but the work
	// of drawing them is done here).
	DrawPatches(fGratingLoc, iSide, contrast);

	// wait for the CED to send the trial start pulse indicating
	// that the subject has been on target for a sufficient amount
	// of time.
#ifndef _OVERRIDE
	if (WaitForTrialStart()) return true;
#endif

	// Present the newly drawn gratings.
	vsgPresent();

	// Move the target positions offscreen. During this
	// time, there are no fixatoin points -- so the subject
	// should not be able to choose the grating yet.
	etu::AnalogOut(0, 9.5);
	etu::AnalogOut(1, 9.5);

	// start a timer to keep track of the amount of time 
	// before the targets are shown
	myTimer.StartTimer();

	// send a "trial running" signal to the CED
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);

	return true;
}



/******************************************************************
	DisplayTrialTargets
		This function displays the grating patches as well as the fixation
		targets -- as soon as the target delay has expired. This function will
		not wait until the subject makes a decision -- it only draws the gratings
		and targets.
	
	Arguments:
		fGraingLoc - The location of the correct grating in degrees.

		iSide -	An integer describing which side the correct grating
		is displayed on. +1 indicates teh grating is on the right, -1 indicates
		the grating is on the left. Any other values will result in
		unpredicatble behvior!

		contrast - the contrast level for the current display. This value
		should be between 0 and 100. Values above 100 will result in 100%
		contrast. Values below 0 will result in 0 contrast.

	Return Value:
		- returns true on success. Returning false will abort 
		the stimulus.
*******************************************************************/
bool CPrefSelectionStimulus::DisplayTrialTargets(float fGratingLoc, int iSide, float contrast) {
	// Select the offscreen video page.
	iPage = 1-iPage;
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	vsgSetDrawMode(vsgCENTREXY);

	// initialize the fixation points
	if (!bEndThread) {
		vsgObjSelect(g_fixation);
		vsgObjSetContrast(100);
		vsgObjSetPixelLevels(0,1);
		vsgObjSetColourVector(&blackColor,&dotColor,vsgBIPOLAR);
	} else {
		return false;
	}
	
	// draw the two fixation targets at the appropriate location
	// (These are drawn to the offscreen video page -- so they are
	// not displayed on the screen yet).
	if (g_pPrefSelection->isTrainingMode()) {
		if (fGratingLoc > 0) {
			vsgDrawOval(dDegPix * g_pPrefSelection->getDotPosition() + xOffset + screenXOffset, yOffset + screenYOffset, dSizePix, dSizePix);
		} else {
			vsgDrawOval(-dDegPix*g_pPrefSelection->getDotPosition() + xOffset + screenXOffset, yOffset + screenYOffset, dSizePix, dSizePix);
		}
		
	} else {
		vsgDrawOval(-dDegPix*g_pPrefSelection->getDotPosition() + xOffset + screenXOffset, yOffset + screenYOffset, dSizePix, dSizePix);
		vsgDrawOval(dDegPix * g_pPrefSelection->getDotPosition() + xOffset + screenXOffset, yOffset + screenYOffset, dSizePix, dSizePix);
	}



	// draw the gratings (exact same gratings that were drawn before
	// on the offscreen video page.)
	DrawPatches(fGratingLoc, iSide, contrast);

	// Wait until the target delay expires
	WaitUntil(g_pPrefSelection->getTargetDelay() - myTimer.CheckTimerWithReset());
	//while ( g_pPrefSelection->getTargetDelay() > myTimer.CheckTimer()) {WaitUntil(10);}myTimer.CheckTimerWithReset();
	
	// switch video pages -- display the gratings with the fixation dots
	vsgPresent();
		
	// change target position output to match the fixation target location
	etu::AnalogOut(0, xDeg / g_pSettings->getDegPerVolt());
	etu::AnalogOut(1, yDeg / g_pSettings->getDegPerVolt());

	return true;
	
}




/******************************************************************
	GetTrialResult
		This function waits for the trial end signal from the CED indicating
		that the subject has chosen one of the targets. The screen is then
		cleared, and the appropriate staircase is updated based on the
		subject's response.
	
	Arguments:
		none.
	Return Value:
		- returns true on success. Returning false will abort 
		the stimulus.
*******************************************************************/
bool CPrefSelectionStimulus::GetTrialResult() {
		// initialize the trial result to incorrect
	int response = 0;

	// switch to the offscreen video page
	iPage = 1-iPage;
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	vsgSetBackgroundColour(&bgColor);

	// check for end trial pulse from the CED. This pulse will indicate
	// that the subject has fixated one of the two targets for a sufficient
	// amount of time.
#ifndef _OVERRIDE
	if (WaitForTrialEnd()) 	return true;
#endif

	
#ifdef _OVERRIDE
	// hack -- gives operator choice of correct or incorrect
	if (AfxMessageBox("Correct?", MB_YESNO) == IDYES) response = 1; else response = 0;
#endif

	// clear the screen after end trial pulse
	// (present a blank video screen)
	vsgPresent();

	
	// get the subject's response. 
#ifndef _OVERRIDE
	if (GetTrialResponse(response)) return true;
#endif

	
	// set voltages to "offscreen" position to prevent reward from occuring
	// without having any stimulus on the screen
	etu::AnalogOut(0, 9.5);
	etu::AnalogOut(1, 9.5);
		
	
	// if the response is correct, delay the correct time out
	// if the response is incorrect, delay the incorrect time out
	if (response > 0) {
		WaitUntil(g_pPrefSelection->getCorrectTimeOut());
	} else {
		WaitUntil(g_pPrefSelection->getTimeOut());
	}
	
		
	// update the appropriate staircase based on the response
	m_pLevels[randomStaircase].pStaircase->update(response);
	iPage = 1 - iPage;

	return true;
}




/******************************************************************
	WaitForTrialStart
		This function checks for the trial start pulse. The digital input
		line is queried every 10 ms until the pulse is received or until
		the trial has been aborted by the user.
	
	Arguments:
		none.
	Return Value:
		- returns false on success. Returning true will abort 
		the stimulus.
*******************************************************************/
bool CPrefSelectionStimulus::WaitForTrialStart() {
	long startTrial = 0;
	while (startTrial == 0) {
		startTrial =  (vsgIOReadDigitalIn() & 0x0004);
		WaitUntil(10);
		if (bEndThread) {
			vsgObjDestroy(g_grating);
			vsgObjDestroy(g_stim);
			vsgObjDestroy(g_fixation);
			return true;
		}
	}

	return false;
}



/******************************************************************
	DrawPatches
		This function simply draws the grating patch and the sham patch
		on the screen. This function is used by DisplayTrial and
		DisplayTrialTargets.
	
	Arguments:
		fGraingLoc - The location of the correct grating in degrees.

		iSide -	An integer describing which side the correct grating
		is displayed on. +1 indicates teh grating is on the right, -1 indicates
		the grating is on the left. Any other values will result in
		unpredicatble behvior!

		contrast - the contrast level for the current display. This value
		should be between 0 and 100. Values above 100 will result in 100%
		contrast. Values below 0 will result in 0 contrast.

	Return Value:
		- returns true on success. Returning false will abort 
		the stimulus.
*******************************************************************/
void CPrefSelectionStimulus::DrawPatches(float fGratingLoc, int iSide, float contrast) {

	vsgObjDestroy(g_grating);
	g_grating = vsgObjCreate();
	vsgObjSetContrast(contrast);
	vsgObjSetPixelLevels(1,2);

	VSGTRIVAL RGBGreen, RGBRed, CIEGreen, CIERed, Background;


	vsgObjSetColourVector(&blackColor,&whiteColor,vsgBIPOLAR);

	float fTableSize = (float)vsgObjGetTableSize(vsgSWTABLE);
	vsgObjTableSquareWave(vsgSWTABLE, 0.25f * fTableSize, 0.75 * fTableSize);
	vsgDrawGrating(fGratingLoc * dDegPix + xOffset + screenXOffset, yOffset + screenYOffset, filterWidth, filterHeight, 0, actualSpatialFreq);
		
	// draw sham patch
	vsgObjDestroy(g_stim);
	g_stim = vsgObjCreate();
	vsgObjSetPixelLevels(3, 1);
	vsgObjSetContrast(100);
	vsgObjSetColourVector(&blackColor, &grayColor, vsgBIPOLAR);

	vsgDrawRect(-fGratingLoc * dDegPix + xOffset + screenXOffset, yOffset + screenYOffset, filterWidth, filterHeight);
		
	// calculate target positions
	xDeg = (iSide * g_pPrefSelection->getDotPosition()) + g_pSettings->getXOffset();
	yDeg = -g_pSettings->getYOffset();


}



/******************************************************************
	WaitForTrialEnd
		This function checks for the trial end pulse. The digital input
		line is queried every 10 ms until the pulse is received or until
		the trial has been aborted by the user.
	
	Arguments:
		none.
	Return Value:
		- returns false on success. Returning true will end the trial.
*******************************************************************/
bool CPrefSelectionStimulus::WaitForTrialEnd() {
	long endTrial = 0;
	while (endTrial == 0) {
		endTrial =  (vsgIOReadDigitalIn() & 0x0001);
		WaitUntil(10);
		if (bEndThread) {
			vsgObjDestroy(g_grating);
			vsgObjDestroy(g_stim);
			vsgObjDestroy(g_fixation);
			return true;
		}
	}

	return false;
}



/******************************************************************
	GetTrialResponse
		This function checks for the trial response pulse. The digital input
		line is queried every 10 ms for 100 ms or until the trial has 
		been aborted by the user.
	
	Arguments:
		&response - an integer variable that will hold the response of the 
		trial. This variable will be set to 1 for a correct response, 0 for
		an incorrect response.
		
	Return Value:
		- returns true on success. Returning false will abort 
		the stimulus.
*******************************************************************/
bool CPrefSelectionStimulus::GetTrialResponse(int &response) {
	
	// check for correct / incorrect
	int time = 0;
	
	while ((time < 100) && (response < 2)) {
		response = vsgIOReadDigitalIn() & 0x0002;
		WaitUntil(10);
		time += 10;
		if (bEndThread) {
			vsgObjDestroy(g_grating);
			vsgObjDestroy(g_fixation);
			vsgObjDestroy(g_stim);
			return true;
		}
	}

	return false;
}



/******************************************************************
	CreateFilter
		This function calculates the actual spatial frequency that
		is closest to the spatial frequency requested by the user. This 
		function is a holdover from when the edges of the grating were
		blurred -- hence the name. If we were to add edge blurring to the
		gratings again (that could not be done within the VSG) -- we would
		initlalize the filter here.
	
	Arguments:
		dDegPix - floating point value for the number of pixels in 1
		degree of visual angle.

		spatialFreq - requested spatial frequency.
		
	Return Value:
		- floating point value for the actual spatial frequency for the 
		grating displayed.
*******************************************************************/
float CPrefSelectionStimulus::CreateFilter(double dDegPix, double spatialFreq) {
	filterWidth = (int)(g_pPrefSelection->getGratingWidth() * dDegPix + 0.5);
	filterHeight = (int)(g_pPrefSelection->getGratingHeight() * dDegPix + 0.5);

	barWidth = (int)(dDegPix / (2 * spatialFreq) + 0.5);
	if (barWidth < 1) {
		barWidth = 1;
	}
	
	return (dDegPix / (2.0f * ((float)(barWidth))));
}



/******************************************************************
	DisplayResults
		This function displays the result of each staircase to the 
		user. The results are displayed in a message box upon trial
		completion, and they are also saved into a text file.
	Arguments:
		none.
	Return Value:
		none.
*******************************************************************/
void CPrefSelectionStimulus::DisplayResults() {
	CString fileName = etu::BrowseForFile(false, "*.txt.", "Text Files (*.txt)|*.txt", "StaircaseResults.txt");
	FILE* fp = fopen(fileName, "wt");
	fprintf(fp, "SpatialFreq\tContrastThreshold\tContrastSensitivity\n");
	CString results = "Staircasing complete -- here are your results .....\n\n";
	for (int i = 0; i < g_pPrefSelection->getNumLevels(); i++) {
		char buffer[1000];
		sprintf(buffer, "\tSpatial Frequency : %f (c/d) --- Contrast : %f%%\n\n", m_pLevels[i].spatialFreq, m_pLevels[i].pStaircase->getCurrentLevel());
		fprintf(fp, "%f\t%f\t%f\n", m_pLevels[i].spatialFreq, m_pLevels[i].pStaircase->getCurrentLevel() / 100, 100 / m_pLevels[i].pStaircase->getCurrentLevel());
		results += CString(buffer);
	}

	// add staircase data points to the data file.
	for (int i = 0; i < g_pPrefSelection->getNumLevels(); i++) {
		fprintf(fp, "\nStaircase for %f (c/d)\n", m_pLevels[i].spatialFreq);
		m_pLevels[i].pStaircase->outputStaircaseData(fp);
	}

	fclose(fp);
	AfxMessageBox(results);

	
}

/******************************************************************
	StimulusType
		This function is a virtual function that allows the
		dialog box system figure out what type of stimulus is currently
		active.
	
	Arguments:
		none.
	Return Value:
		- String with the stimulus type.
*******************************************************************/
CString CPrefSelectionStimulus::StimulusType() {
	return "Preferrential Selection Stimulus";
}


/******************************************************************
	increaseStep
		This function is used by the dialog system to override
		the staircase stepping functions. This allows the user
		to increase the step size being used for the current staircase.
	
	Arguments:
		none.
	Return Value:
		none.
*******************************************************************/
void CPrefSelectionStimulus::increaseStep() {
	m_pLevels[randomStaircase].pStaircase->increaseStep();
}


/******************************************************************
	decreaseStep
		This function is used by the dialog system to override
		the staircase stepping functions. This allows the user
		to decrease the step size being used for the current staircase.
	
	Arguments:
		none.
	Return Value:
		none.
*******************************************************************/
void CPrefSelectionStimulus::decreaseStep() {
	m_pLevels[randomStaircase].pStaircase->decreaseStep();
}

#endif