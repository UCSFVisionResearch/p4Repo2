/**
 * @file FullfieldStimulus.h
 * @brief 
 */

#ifndef FULLFIELD_STIMULUS_H
#define FULLFIELD_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CFullfieldStimulus : public CGenericStimulus 
{
public:
	CFullfieldStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CFullfieldStimulus();

	virtual bool Initialize();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();

	void DrawNonTrialFixation();
	void DrawTrialFixation();

	void ActivateMotion();

	void CleanUpStimulus();
	
private:
	int iScrWidthPix, iScrHeightPix;
	int totalPix;
	int iFrameNum;
	int iCycleFrames;
	double dVoltageStep;
	int xVoltageStep, yVoltageStep;
	VSGOBJHANDLE fixation2;
	bool bShowMotion;
};

#endif // FULLFIELD_STIMULUS_H
