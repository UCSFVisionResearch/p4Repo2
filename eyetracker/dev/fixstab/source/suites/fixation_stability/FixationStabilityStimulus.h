/**
 * @file FixationStabilityStimulus.h
 * @brief 
 */

#ifndef FIXATION_STABILITY_STIMULUS_H
#define FIXATION_STABILITY_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include <afxmt.h>
#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Typedefs and Structs
 *****************************************************************************/

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CFixationStabilityStimulus : public CGenericStimulus 
{
public:
	CFixationStabilityStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CFixationStabilityStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();

private:
	void NewDrawingPage(bool noise);
	void DrawCross(fstab::EYE eye, double centerXDeg, double centerYDeg);
	void InitLutBuffer();
	void InitNoiseHandling();
	void InitGratings();
	void InitShutters();
	void SetShutters(bool occludeLeft, bool occludeRight);
	void ShutterDelay();
	void CleanUpNoiseHandling();
	void CleanUpGratings();

	int m_screenWidthPix;
	int m_screenHeightPix;
	
	int m_currentPage;
	bool m_useNoise;
	bool m_lastTrialNoise;
	etu::CTimer m_flashTimer;
	VSGLUTBUFFER m_lutbuf;

	bool m_leftClosed, m_rightClosed;
	
	VSGTRIVAL&   m_bgColor;
	VSGTRIVAL&   m_noiseColor;
	VsgExt::NoiseBgController* m_noiseBgController;
	VsgExt::NoiseColorMap*     m_noiseColorMap;
    VsgExt::IGrating* m_leftEyeSpec;
	VsgExt::IGrating* m_rightEyeSpec;

	/* Not supported */
	CFixationStabilityStimulus(const CFixationStabilityStimulus&);
	CFixationStabilityStimulus& operator=(const CFixationStabilityStimulus&);
};

#endif // FIXATION_STABILITY_STIMULUS_H
