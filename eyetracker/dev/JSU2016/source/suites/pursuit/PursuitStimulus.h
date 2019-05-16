/**
 * @file PursuitStimulus.h
 * @brief 
 */

#ifndef PURSUIT_STIMULUS_H
#define PURSUIT_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CPursuitStimulus : public CGenericStimulus 
{
public:
	CPursuitStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CPursuitStimulus();

	// override the initialize method in CGeneric Stimulus
	virtual bool Initialize();
	
	// displays the stimulus
	virtual bool DisplayStimulus();

	// return the name of the current stimulus (override StimulusType from CGenericStimulus)
	virtual CString StimulusType();

private:
	char buffer[1024];
	char letter[2];
	
	int iScrWidthPix, iScrHeightPix;		// screen size
	double dFrameRate;						// framerate of the display
	double dCalculatedFreq;					// frequency of the primary motion
	double dCalculatedHighFreq;				// frequency of the high freq motion
	double dAmplitudePix, dAmplitudeHighPix;// amplitude of the motion in pixels
	double dFrameNum;						// current frame number
	int iPage;								
	double dOffsetPixX, dOffsetPixY;		// pixel offsets from default display coordinates
	
	int cycle;
	double prevX, prevY;
};

#endif // PURSUIT_STIMULUS_H
