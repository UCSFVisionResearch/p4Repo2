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
	void DrawCrossWithBG(double centerXDeg, double centerYDeg, 
		const CSaccadeTrial& trial);
	void DrawPeripheralStim(double xDeg, double yDeg, 
		const CSaccadeTrial& trial);
	int  GetCenterTimePeriod();
	int  GetFlashTimePeriod();
	int  GetRandomizedTimePeriod(int min, int max);
	void InitLutBuffer();
	void InitNoiseHandling();
	void CleanUpNoiseHandling();
	void InitGratings();
	void CleanUpGratings();
	void SetupBufferForTrial(CSaccadeTrial& currentTrial);

	bool FlashTriggeredTargetSequence(double centerXDeg, double centerYDeg, 
		char* buffer, size_t stringSize, const CSaccadeTrial* currentTrial, 
		long& mstime);
	bool SubjectTriggeredTargetSequence(double centerXDeg, double centerYDeg, 
		char* buffer, size_t stringSize, const CSaccadeTrial* currentTrial, 
		long& mstime);
	bool CueTriggeredTargetSequence(double centerXDeg, double centerYDeg, 
		char* buffer, size_t stringSize, const CSaccadeTrial* currentTrial, 
		long& mstime);

	void NewDrawingPage(bool noise);
	void FailTrial(CSaccadeTrial& currentTrail, long mstime);
	
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
