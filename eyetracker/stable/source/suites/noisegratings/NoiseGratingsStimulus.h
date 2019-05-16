/**
 * @file NoiseGratingsStimulus.h
 * @brief 
 */

#ifndef NOISE_GRATINGS_STIMULUS_H
#define NOISE_GRATINGS_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CNoiseGratingsStimulus : public CGenericStimulus 
{
public:
	CNoiseGratingsStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CNoiseGratingsStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();

private:
	bool GenerateGrating();
	bool GenerateMask();
	bool Draw();

	int iPage;
	int iScrWidthPix, iScrHeightPix;
};

#endif // NOISE_GRATINGS_STIMULUS_H
