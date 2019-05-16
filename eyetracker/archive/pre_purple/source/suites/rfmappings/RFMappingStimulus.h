/**
 * @file RFMappingStimulus.h
 * @brief 
 */

#ifndef RF_MAPPING_STIMULUS_H
#define RF_MAPPING_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"
#include "utils/Timer.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CRFMappingStimulus : public CGenericStimulus 
{
public:
	CRFMappingStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CRFMappingStimulus();
	
	// displays the stimulus
	virtual bool Initialize();
	virtual bool DisplayStimulus();
	void DrawStimulus();
	void DrawCircle();
	void DrawBar();
	virtual CString StimulusType();

	void updatePosition(float x, float y);
	void updateStimulus(int pixelsX, int pixelsY, bool bLeftButton);

	void StopExecution();
	
private:
	bool bDrawStimulus;
	bool bUpdatePosition;
	
	int iScrWidthPix, iScrHeightPix;
	int iPage;
	
	VSGTRIVAL bgColor;
	double dFrameRate;
	int screenPixOffsetX, screenPixOffsetY;

	double startX, startY;
	etu::CTimer pulseTimer;
	bool bPulse;
	bool bPulseState;
};

#endif // RF_MAPPING_STIMULUS_H
