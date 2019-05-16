/**
 * @file APDStimulus.h
 * @brief 
 */

#ifndef APD_STIMULUS_H
#define APD_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CAPDStimulus : public CGenericStimulus 
{
public:
	CAPDStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CAPDStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();

private:
	int iScrWidthPix, iScrHeightPix;
	double pointX;
	double pointY;
	
	int iPage;
	etu::CTimer* waitTimer;
	bool bOn;
	bool bLeft;

	//CSerialComm theComm;

	void DrawLeftField();
	void DrawRightField();
	void DrawFullScreen();

	double dFrameTime;
};

#endif // APD_STIMULUS_H
