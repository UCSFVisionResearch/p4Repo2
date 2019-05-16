/**
 * @file OcularFollowingStimulus.h
 * @brief 
 */

#ifndef OCULAR_FOLLOWING_STIMULUS_H
#define OCULAR_FOLLOWING_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class COcularFollowingStimulus : public CGenericStimulus 
{
public:
	COcularFollowingStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~COcularFollowingStimulus();
	virtual bool Initialize();
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();
	void DoFixation();
	void DoStep(int selectedDirection);
	void DoTimeOut();

private:
	float degPerSec;
	float degPerFrame;
	int iScrWidthPix, iScrHeightPix;
	double pointX;
	double pointY;
	int iPhase;
	int iPage;
	bool bDrawn;
	int noisesize;
	int rndHeight;
	double xVoltageStep, yVoltageStep, dVoltageStep, dPixPerFrame;;
	bool bShowMotion;
	
	int selectedDirection;
};

#endif // OCULAR_FOLLOWING_STIMULUS_H
