/**
 * @file RulerStimulus.h
 * @brief 
 */

#ifndef RULER_STIMULUS_H
#define RULER_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CRulerStimulus : public CGenericStimulus 
{
public:
	CRulerStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CRulerStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();

private:
	int iScrWidthPix, iScrHeightPix;
	double pointX;
	double pointY;

	double l_eye_x;
	double l_eye_y;
	double r_eye_x;
	double r_eye_y;
	double timestamp;

	int iPage;
};

#endif // RULER_STIMULUS_H
