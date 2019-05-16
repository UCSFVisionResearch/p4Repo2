/**
 * @file RandomDotAnaglyphStimulus.h
 * @brief 
 */

#ifndef RDA_STIMULUS_H
#define RDA_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CRandomDotAnaglyphStimulus : public CGenericStimulus 
{
public:
	CRandomDotAnaglyphStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CRandomDotAnaglyphStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();
	
private:
	int iScrWidthPix, iScrHeightPix;
	double pointX;
	double pointY;

	int iPage;
};

#endif // RDA_STIMULUS_H
