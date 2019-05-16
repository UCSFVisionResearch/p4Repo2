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
#include "utils/Grid.h"
#include "RFTrial.h"

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
	void DrawStimulus();
//new drawing functions to replace previous 2 lines
	void DrawCrossWithBG(double centerXDeg, double centerYDeg, CRFTrial::EYE_CODE eye);
	void DrawPeripheralStim(double xDeg, double yDeg, const CRFTrial& trial);
	void InitLutBuffer();
	void InitNoiseHandling();
	void CleanUpNoiseHandling();
	void SetupBufferForTrial(CRFTrial& currentTrial,CRFTrial::EYE_CODE eyeCode);
	void NewDrawingPage(bool noise);
	void Clear(bool noise);
	void InitGratings();
	void CleanUpGratings();

	bool m_messageSent;

	const long m_screenWidthPix;
	const long m_screenHeightPix;
	int m_currentPage;
	bool m_lastTrialNoise;
	bool m_useNoise;

	/* To cache the offset from g_pSettings */
	const double m_screenOffsetXDeg;
	const double m_screenOffsetYDeg;
	const float m_windowAllowancePix;
	bool m_useWait;

	const double m_crossXDeg;
	const double m_crossYDeg;
	double m_crossXPix;
	double m_crossYPix;
	double m_crossDiameterPix;
	const double m_crossThicknessPix;

	const bool m_useBinocular;
	const double m_crossX2Deg;
	const double m_crossY2Deg;
	const int m_swapInt;
	int m_swapCounter;
	CRFTrial::EYE_CODE m_fixEye;

	int m_pageNum;

	VSGTRIVAL& m_bgColor;
//	VSGTRIVAL m_crossColor;
//	VSGTRIVAL m_stimColor;
//new vsg LUT setup to replace previous 3 lines
	VSGLUTBUFFER m_lutbuf;
//	VSGTRIVAL& m_bgColor;
	VSGTRIVAL& m_noiseColor;
	VsgExt::NoiseBgController* m_noiseBgController;
	VsgExt::NoiseColorMap* m_noiseColorMap;
	VsgExt::IGrating* m_primarySpec;
	VsgExt::IGrating* m_secondarySpec;
	VsgExt::IGrating* m_tertiarySpec;
	
	double m_frameRate;


	etu::CTimer m_sequenceTimer;
	etu::CTimer m_pulseTimer;

	const bool m_useGrid;
	const int m_displayTimeMs;
	const bool m_useBlanks;
	const int m_blankTimeMs;


	const bool m_usePulse;
	bool m_pulsed;
	bool m_pulseStateHigh;
	const int m_pulseTime;

	bool m_stimulusChanged;
	bool targetAcquired;

	class StimLocationGenerator
	{
	public:
		StimLocationGenerator();
		etu::SPoint next();
	private:
		etu::pGrid_t m_pGrid;
		etu::GridIter_t m_iter;
		etu::GridIter_t m_end;
		bool m_random;
		/* Not supported */
		StimLocationGenerator(const StimLocationGenerator&);
		StimLocationGenerator& operator=(const StimLocationGenerator);
	};

	StimLocationGenerator* m_pStimLocationGenerator;
	CRFTrialGenerator* m_rfTrialGeneraotr;
	
};

#endif // RF_MAPPING_STIMULUS_H
