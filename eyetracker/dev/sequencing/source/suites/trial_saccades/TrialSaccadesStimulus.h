/**
 * @file TrialSaccadesStimulus.h
 * @brief 
 */

#ifndef TRIAL_SACCADES_STIMULUS_H
#define TRIAL_SACCADES_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"
#include "utils/Timer.h"
#include "vsgext/vsgext.h"
#include "SaccadeTrial.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CTrialSaccadesStimulus : public CGenericStimulus 
{
public:
	CTrialSaccadesStimulus();
	virtual ~CTrialSaccadesStimulus();
	
	virtual bool DisplayStimulus();
	virtual CString StimulusType();

private:
	/* Prep and Clean Up ************************************************** */
	void InitLutBuffer();
	void InitNoiseHandling();
	void InitGratings();
	void CleanUpGratings();
	void CleanUpNoiseHandling();

	/* Trial Prep and Clean Up ******************************************** */
	void SetupBufferForTrial(CSaccadeTrial& currentTrial);
	void SetupStimsForTrial(CSaccadeTrial& currentTrial, bool offset);

	void DrawCrossWithBG(double centerXDeg, double centerYDeg, 
		const CSaccadeTrial& trial);
	void DrawPeripheralStim(double xDeg, double yDeg, 
		const CSaccadeTrial& trial);

	/* Trial Sequencing ********************************** */
	void PresentStimCrossOnly(CSaccadeTrial& currentTrial);
	void PresentStimCrossDot(CSaccadeTrial& currentTrial);
	void PresentStimDotOnly(CSaccadeTrial& currentTrial);
	void PresentStimBgOnly(CSaccadeTrial& currentTrial, bool baited);
	void PresentStimTimeout(CSaccadeTrial& currentTrial);

	void FailTrial(CSaccadeTrial& currentTrail, long mstime);

	/* ********************************************************************* */

	bool isDebugResponserOn() const;

	double m_stimCenterXDeg, m_stimCenterYDeg; 
	double m_stimPeriphXDeg, m_stimPeriphYDeg;
	CString m_tagString;
	CString m_msgString;
	
	int  m_screenWidthPix; 
	int  m_screenHeightPix;
	int  m_currentPage;
	bool m_lastTrialNoise;
	bool m_useNoise;

	etu::CTimer  m_flashTimer;
	VSGLUTBUFFER m_lutbuf;
	VSGTRIVAL&   m_bgColor;
	VSGTRIVAL&   m_noiseColor;
	VsgExt::NoiseBgController* m_noiseBgController;
	VsgExt::NoiseColorMap*     m_noiseColorMap;
    VsgExt::IGrating* m_primarySpec;
	VsgExt::IGrating* m_secondarySpec;
	VsgExt::IGrating* m_tertiarySpec;
	VsgExt::IGrating* m_verticalGrating;
	VsgExt::IGrating* m_horizontalGrating;

	/* Not supported */
	CTrialSaccadesStimulus(const CTrialSaccadesStimulus&);
	CTrialSaccadesStimulus& operator=(const CTrialSaccadesStimulus&);
};


#endif // TRIAL_SACCADES_STIMULUS_H
