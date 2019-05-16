/**
 * @file StepRampStimulus.h
 * @brief 
 */

#ifndef STEPRAMPSTIMULUS_H
#define STEPRAMPSTIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CStepRampStimulus : public CGenericStimulus 
{
public:
	CStepRampStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CStepRampStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();
	void DoInitialDot();
	void DoExcursion();
	void DoPlateau();
	void DoIntertrial();

private:
	float degPerSec;
	float degPerFrame;
	int iScrWidthPix, iScrHeightPix;
	double pointX;
	double pointY;
	int iPhase;
	int iPage;
	etu::CTimer* waitTimer;
	bool bDrawn;
};

#endif // STEPRAMPSTIMULUS_H
