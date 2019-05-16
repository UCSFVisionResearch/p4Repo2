/**
 * @file OrientationTuningStimulus.h
 * @brief 
 */

#ifndef ORIENTATION_TUNING_STIMULUS_H
#define ORIENTATION_TUNING_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Typedefs and Structs
 *****************************************************************************/

typedef struct myOrientationTuningStruct 
{
	int index;
	float angle;
	int nRunsLeft;
	float dx, dy;
	float startX, startY;
	short* xTable;
	short* yTable;
	int nFrames;
	float barWidth, barHeight;
	CString field;
} OTTuningRun;

/*****************************************************************************
 * Classes
 *****************************************************************************/

class COrientationTuningStimulus : public CGenericStimulus 
{
public:
	COrientationTuningStimulus();

	static const int DIR_UP = 1;
	static const int DIR_DOWN = 2;
	static const int DIR_LEFT = 3;
	static const int DIR_RIGHT = 4;

	// overrides the destructor from CGenericStimulus
	virtual ~COrientationTuningStimulus();

	virtual bool Initialize();
	
	// displays the stimulus
	virtual bool DisplayStimulus();

	virtual CString StimulusType();

	virtual void StopExecution();

	// start the trials
	void StartTrials(bool);

	void DisplayTrials();
	void InitializeTrials();

private:
	int iScrWidthPix, iScrHeightPix;
	double dFrameRate;
	int iPage;
	bool bStartTrials;
	bool bTrialsInitialized;

	bool bPrimaryColor;
	VSGTRIVAL bgColor;
	VSGTRIVAL fixSpotColor;

	OTTuningRun* pTuningRuns;
	int numTuningRuns;

	int elapsedTime;
	void DrawBar(OTTuningRun currentRun);
	void MoveBar(OTTuningRun currentRun);
	void DrawMoveBarWithDot(OTTuningRun currentRun);

	int screenPixOffsetX, screenPixOffsetY;

	float calcNewStartX(OTTuningRun run);
	float calcNewStartY(OTTuningRun run);
	float calcNewNumFrames(OTTuningRun run);
	bool isOffscreen(OTTuningRun run);
	bool endsOffscreen(OTTuningRun run);
	double dScreenHeightDeg, dScreenWidthDeg;
};

#endif // ORIENTATION_TUNING_STIMULUS_H
