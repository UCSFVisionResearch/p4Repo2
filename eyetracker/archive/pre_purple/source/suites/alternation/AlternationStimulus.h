/**
 * @file AlternationStimulus.h
 * @brief 
 */

#ifndef ALTERNATION_STIMULUS_H
#define ALTERNATION_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CAlternationStimulus : public CGenericStimulus 
{
public:
	CAlternationStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CAlternationStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();

private:		
	int iScrWidthPix, iScrHeightPix;
	double pointX;
	double pointY;

	int iPage;

	double dFrameTime;
};

#endif // ALTERNATION_STIMULUS_H
