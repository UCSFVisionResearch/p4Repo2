/**
 * @file GenericStimulus.h
 * @brief 
 */

#ifndef GENERIC_STIMULUS_H
#define GENERIC_STIMULUS_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "VSGV8.h"
#include "VSGV8AD.h"
#include "utils/Timer.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CGenericStimulus {
public:
	CGenericStimulus();
	virtual ~CGenericStimulus();
	virtual bool Initialize();
	virtual bool DisplayStimulus() =0;
	virtual CString StimulusType();
	virtual void StopExecution();

	bool Run();

protected:
	double SineWave(double x, double freq, double amplitude);
	double TriangleWave(double x, double freq, double amplitude);
	void WaitUntil(double dMilliSeconds, DWORD incremenent = 0);
	
	bool bEndThread;
	etu::CTimer* myTimer;
	VSGTRIVAL blackColor, dotColor;
	double xOffset, yOffset;
	double screenXOffset, screenYOffset;
	double dSize;
};

#endif // GENERIC_STIMULUS_H
