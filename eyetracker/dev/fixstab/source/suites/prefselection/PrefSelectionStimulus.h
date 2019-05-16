/**
 * @file PrefSelectionStimulus.h
 * @brief 
 */

#ifndef PREF_SELECTION_STIMULUS_H
#define PREF_SELECTION_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CPrefSelectionStimulus : public CGenericStimulus 
{
public:
	CPrefSelectionStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CPrefSelectionStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	
	// returns the stimulus type string
	virtual CString StimulusType();

	// increasese the step size for the current
	// staircase
	void increaseStep();

	// decreases the step size fo teh current 
	// staircase
	void decreaseStep();

private:
	// perform some misc initializatiosn for the trial
	bool SetupTrial(float& contrast, float& spatialFreq, int iSide);

	// draw the pretrial fixation dot
	bool DisplayPretrial(float contrast);

	// wait for trial start and draw grating patches
	bool DisplayTrial(float fGratingLoc, int iSide, float contrast);

	// wait for end of target delay, and draw gratings + targets
	bool DisplayTrialTargets(float fGratingLoc, int iSide, float contrast);

	// wait for trial end and update staircase according to response
	bool GetTrialResult();

	// waits for the trial start signal from teh CED.
	bool WaitForTrialStart();

	// draws the grating and sham patches
	void DrawPatches(float fGratingLoc, int iSide, float contrast);

	// wait for the trial end signal from the CED
	bool WaitForTrialEnd();

	// checks the correct/incorrect response from teh CED
	bool GetTrialResponse(int &response);
	
	// calculates the actual spatial frequency for the 
	// nominal spatial frequency given
	float CreateFilter(double dDegPix, double spatialFreq);

	// displays the results of the trial to the user
	void DisplayResults();

	int iPage;								// video drawing page
	int iScrWidthPix, iScrHeightPix;		// screen dimensions
	float** m_pFilter;						// the filter used for edge blurring
	int filterWidth;						// width of the filter
	int filterHeight;						// height of the filter
	int barWidth;							// width of one stripe of the grating
	int randomStaircase;					// staircase number chosen randomly
	double xOffset, yOffset;				// display offset
	double screenXOffset, screenYOffset;	// screen offset
	double dSizePix, dDegPix;				// pixel values for params given in degrees
	double xDeg, yDeg;						// degree values for target locations

	VSGTRIVAL bgColor, whiteColor, grayColor;	// colors
	float actualSpatialFreq;				// actual spatial frequency of the grating

	etu::CTimer myTimer;							// high resolution timer
	level* m_pLevels;						// staircase objects
};

#endif // PREF_SELECTION_STIMULUS_H
