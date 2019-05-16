/**
 * @file CircleAnnulusStimulus.h
 * @brief 
 */

#ifndef CIRCLE_ANNULUS_STIMULUS_H
#define CIRCLE_ANNULUS_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Typedefs and Structs
 *****************************************************************************/

typedef struct myAnnulusTesting {
	float circleRadius;
	float annulusWidth;
	bool bCircle;
	VSGTRIVAL color;
	int nRunsLeft;
	int index;
	double xLocation;
	double yLocation;
} AnnulusTestingRun;

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CCircleAnnulusStimulus : public CGenericStimulus 
{
public:
	CCircleAnnulusStimulus();

	static const int DIR_UP = 1;
	static const int DIR_DOWN = 2;
	static const int DIR_LEFT = 3;
	static const int DIR_RIGHT = 4;

	// overrides the destructor from CGenericStimulus
	virtual ~CCircleAnnulusStimulus();

	virtual bool Initialize();
	
	// displays the stimulus
	virtual bool DisplayStimulus();

	virtual CString StimulusType();

	virtual void StopExecution();

	// start the trials
	void StartTrials(bool);

	void DisplayTrials();
	bool InitializeTrials();
	void DrawCircle(AnnulusTestingRun currentRun);
	void DrawAnnulus(AnnulusTestingRun currentRun);

	void DisplayTrialsOnOff();
	bool InitializeTrialsOnOff();

private:
	int iScrWidthPix, iScrHeightPix;
	double dFrameRate;
	int iPage;
	bool bStartTrials;
	bool bTrialsInitialized;

	bool bPrimaryColor;
	VSGTRIVAL bgColor;
	int elapsedTime;
	//CSerialComm theComm;
	int screenPixOffsetX, screenPixOffsetY;
	AnnulusTestingRun* pAnnulusTesting;
	int iNumAnnulusRuns;
	double dFrameTime;
};

#endif // CIRCLE_ANNULUS_STIMULUS_H
