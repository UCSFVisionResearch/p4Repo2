/**
 * @file GenericStimulus.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"

/***************************************************
CLASS: CGenericStimulus
- Parent class for all stimuli. This class provides generic 
methods used by all stimuli. This class should not be instantiated
because it does nothing on its own.
******************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Contstructor
	 - Initialization routine for most stimuli
************************************************************/
CGenericStimulus::CGenericStimulus() 
{
	Initialize();
}

/***********************************************************
	 Destructor
************************************************************/
CGenericStimulus::~CGenericStimulus() {}

/***********************************************************
	 Intitialize()
	 - Generic vsg initialization. All overriding methods 
	 must set bEndThread to false.
************************************************************/
bool CGenericStimulus::Initialize() 
{
	bEndThread=false;
	myTimer = new etu::CTimer();

	blackColor.a = 0;
	blackColor.b = 0;
	blackColor.c = 0;

	dotColor.a = 1;
	dotColor.b = 0;
	dotColor.c = 0;

	int CheckCard;
 	
	//Initialise the vsg card then check that it initialised O.K.
	if (!g_bIsInitialized) {
		CheckCard = vsgInit("");
		if (CheckCard < 0) return false;
		g_bIsInitialized = true;
		CheckCard=vsgSetVideoMode(vsgPANSCROLLMODE);
	}

	// calculate framerate, and frame timing constants
	g_dHalfFrameInterval = DWORD(floor(vsgGetSystemAttribute(vsgFRAMETIME) / 2000.0f));
	g_dSleepThreshold = (double)g_dHalfFrameInterval / 2.0f;
	g_dFramerate = 1000000.0f / vsgGetSystemAttribute(vsgFRAMETIME);
	//Clear the palette to black so no drawing will be seen.
	vsgSetCommand(vsgPALETTECLEAR);

	//Set the current drawing page.
	vsgSetDrawPage(vsgVIDEOPAGE,3, 0);
	vsgSetDrawPage(vsgVIDEOPAGE,2, 0);
	vsgSetDrawPage(vsgVIDEOPAGE,1, 0);
	vsgSetDrawPage(vsgVIDEOPAGE,0, 0);
	vsgMoveScreen(0, 0);
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(0,0);

	//this one is easy to forget, just do it here.
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	return true;	
}

/***********************************************************
	 Run()
	 - Run the stimulus by calling the DisplayStimulus method
	 in the child stimulus class.
************************************************************/
bool CGenericStimulus::Run() 
{
	bool bSuccess = true;

	while (!bEndThread && bSuccess) {
		bSuccess = DisplayStimulus();
	}

	if (!bSuccess) {
		return false;
	}

	bEndThread = false;
	return true;
}

/***********************************************************
	 DisplayStimulus()
	 - This method is a place holder. It is a virtual function
	 that is overridden in each of the child classes. This 
	 version of the function should never be called because a
	 CGenericClass object should never be instantiated.
************************************************************/
bool CGenericStimulus::DisplayStimulus() 
{
	return false;
}

/***********************************************************
	 WaitUntil(double, DWORD)
	 - Waits for the specified number of milliseconds by
	 by sleepign in the increments specified. 
************************************************************/
void CGenericStimulus::WaitUntil(double dMilliSeconds, DWORD increment) 
{
	double elapsedTime = 0;
	myTimer->StartTimer();
	
	if (increment < 1) increment = g_dHalfFrameInterval;

	// sleep in half frame intervals until the desired number of msecs have passed
	while (elapsedTime < dMilliSeconds) {
		Sleep(increment);
		elapsedTime = myTimer->CheckTimer();
	}
}

/***********************************************************
	 StimulusType()
	 - Generic method returning the type of stimulus currently
	 executing. This function should never be called because 
	 this class should never be instantiated. All children
	 must override this method.
************************************************************/
CString CGenericStimulus::StimulusType() 
{
	return "Generic Stimulus";
}

/***********************************************************
	 StopExecution()
	 - Stops the execution of the run command. This function
	 is called by the dialog processing thread whenever the
	 stop button is pressed. This function signals the run 
	 method to break out of the execution loop.
************************************************************/
void CGenericStimulus::StopExecution() 
{
	bEndThread = true;
}

/***********************************************************
	 SineWave()
	 - Returns a point along a sinewave with parameters given.
	 This function is used by any stimulus using sinusoidal 
	 oscillations to determine the frame by frame position of
	 the dot.
************************************************************/
double CGenericStimulus::SineWave(double x, double dCalculatedFreq, double dAmplitude) 
{
	return dAmplitude * sin(dCalculatedFreq*x);
}

/***********************************************************
	 TriangleWave()
	 - Returns a point along a trianglewave with parameters given.
	 This function is used by any stimulus using constant velocity 
	 oscillations to determine the frame by frame position of
	 the dot.
************************************************************/
double CGenericStimulus::TriangleWave(double x, double dCalculatedFreq, double dAmplitude) 
{
	//return 2 * dAmplitude * ((acos(sin(dCalculatedFreq*x)) / M_PI) - 0.5);
	//let's use a REAL triangle wave instead of an approximation
	/*
		|  /\      |
		| /  \     |
		|     \  / |
		|      \/  |
		|-11223344-|
	*/
	double scalex = fmod(x*dCalculatedFreq, 2.0*3.14159265358979323846) / (2.0*3.14159265358979323846);//0-1//M_PI
	if ( scalex < 0.25 ) {					//1st quarter
		return dAmplitude*(scalex*4);
	} 
	else if ( scalex < 0.5 ) {				//2nd quarter
		return dAmplitude*(1.0-(scalex-0.25)*4);
	} 
	else if ( scalex < 0.75 ) {			//3rd quarter
		return -1.0*dAmplitude*((scalex-0.5)*4);
	} 
	else {								//4th quarter
		return -1.0*dAmplitude*(1.0-(scalex-0.75)*4);
	}
}
