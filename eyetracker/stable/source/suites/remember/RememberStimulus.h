/**
 * @file RememberStimulus.h
 * @brief 
 */

#ifndef REMEMBER_STIMULUS_H
#define REMEMBER_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"
#include "utils/Timer.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CRememberStimulus : public CGenericStimulus 
{
public:
	CRememberStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CRememberStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();
	
private:
	etu::CTimer FlashTimer;
	int iScrWidthPix, iScrHeightPix;
	double pointX;
	double pointY;
	
	int iPage;
	bool bOn;
	int iOn;
	bool bNoiseOn;

	int prevTextCode;

	VSGLUTBUFFER lutbuf;
	
	void GetNextPoint(double& dPosXDeg, double& dPosYDeg);
	int iOldLocIndex;
	int iSeqLocIndex;
	int iTotalCounter;

	double dFrameTime;

	double periphX, periphY;
};

#endif // REMEMBER_STIMULUS_H
