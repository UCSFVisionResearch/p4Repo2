/**
 * @file SuppressionStimulus.h
 * @brief 
 */

#ifndef SUPPRESSION_STIMULUS_H
#define SUPPRESSION_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"
#include "utils/Timer.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSuppressionStimulus : public CGenericStimulus 
{
public:
	CSuppressionStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CSuppressionStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();
	
private:
	etu::CTimer DotTimer, DisplayTimer;
	int iScrWidthPix, iScrHeightPix;
	double pointX;
	double pointY;

	int iGridSize, iDelay;
	double GridHeight;
	int GridSpace;
	int iPage;
	bool bOn;
	int iOn;

	CSerialComm theComm;
	
	int iOldLocIndex;

	double dFrameTime;
};

#endif // SUPPRESSION_STIMULUS_H
