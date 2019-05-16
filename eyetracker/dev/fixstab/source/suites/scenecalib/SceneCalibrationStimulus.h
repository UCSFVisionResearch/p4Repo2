/**
 * @file SceneCalibrationStimulus.h
 * @brief 
 */

#ifndef SCENE_CALIBRATION_STIMULUS_H
#define SCENE_CALIBRATION_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSceneCalibrationStimulus : public CGenericStimulus 
{
public:
	CSceneCalibrationStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CSceneCalibrationStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();
	virtual void StopExecution();
	
	void setDrawingState(bool b);
	
private:
	int iScrWidthPix, iScrHeightPix;
	int iPage;
	bool bDrawn;
};

#endif // SCENE_CALIBRATION_STIMULUS_H
