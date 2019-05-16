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
	virtual ~CRFMappingStimulus();
	
	/* Displays the stimulus */
	virtual bool DisplayStimulus();
	
	virtual CString StimulusType();

	void updatePosition(float xPct, float yPct);
	void updateStimulus(int pixelsX, int pixelsY, bool bLeftButton);

	void StopExecution();
	
private:
	void NewDrawingPage();
	void DrawStimulus();

	const long m_screenWidthPix;
	const long m_screenHeightPix;

	double m_crossXPix;
	double m_crossYPix;
	double m_crossDiameterPix;
	const double m_crossThicknessPix;

	int m_pageNum;

	VSGTRIVAL m_bgColor;
	double m_frameRate;
	
	etu::CTimer m_pulseTimer;
	bool m_usePulse;
	bool m_pulsed;
	bool m_pulseStateHigh;

	bool m_stimulusChanged;
};

#endif // RF_MAPPING_STIMULUS_H
