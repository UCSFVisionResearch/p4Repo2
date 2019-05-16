/**
 * @file ScotomaMapStimulus.h
 * @brief 
 */

#ifndef SCOTOMA_MAP_STIMULUS_H
#define SCOTOMA_MAP_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"
#include "utils/Timer.h"
#include "vsgext/vsgext.h"
//#include "ScotomaMap.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CScotomaMapStimulus : public CGenericStimulus 
{
public:
	CScotomaMapStimulus();
	virtual ~CScotomaMapStimulus();
	
	virtual bool DisplayStimulus();
	virtual CString StimulusType();
	
	void ResetTrialSequence();

private:
	void DrawCrossWithBG(bool drawPrimary, double centerXDeg, double centerYDeg);
	void DrawPeripheralStim(double dPixX, double dPixY, double dPixDiam, CScotomaTrial& trial);
	void HelperDrawPeripheralStim(double dPixX, double dPixY, double dPixDiam, CScotomaTrial& trial);
	void DoTimeout(long mstime);
	int  GetCenterTimePeriod();
	int  GetFlashTimePeriod();
	int  GetRandomizedTimePeriod(int min, int max);
	void UpdateGraphs(int responseCode, CScotomaTrial& trial, double xDeg, double yDeg);
	void InitLutBuffer();
	void InitNoiseHandling();
	void CleanUpNoiseHandling();
	void InitGratings();
	void CleanUpGratings();
	void SetupBufferForTrial(CScotomaTrial& currentTrial);
	CScotomaTrial* GetNextTrial();
	bool IsCorrect(CScotomaTrial& trial, bool movedRight);
	void EyePosResample(bool leftFixating);
	
	int  m_screenWidthPix; 
	int  m_screenHeightPix;
	int  m_currentPage;
	bool m_lastTrialNoise;
	bool m_useNoise;
	double m_periphXDeg;
	double m_periphYDeg;

	CScotomaMap::TrialPtrContainer_t::const_iterator m_currentTrial;
	
	etu::CTimer       m_flashTimer;
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
	VsgExt::IGrating* m_dualGrating;

	//eye position sample arrays
	double *pLCenterX, *pLCenterY, *pLPeriphX, *pLPeriphY,
		*pRCenterX, *pRCenterY, *pRPeriphX, *pRPeriphY;
	double alcx, alcy, alpx, alpy, arcx, arcy, arpx, arpy;
	int lcount, rcount;

	/* Not supported */
	CScotomaMapStimulus(const CScotomaMapStimulus&);
	CScotomaMapStimulus& operator=(const CScotomaMapStimulus&);
};

#endif // SCOTOMA_MAP_STIMULUS_H
