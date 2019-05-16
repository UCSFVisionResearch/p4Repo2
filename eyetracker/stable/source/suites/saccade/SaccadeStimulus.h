/**
 * @file SaccadeStimulus.h
 * @brief 
 */

#ifndef SACCADE_STIMULUS_H
#define SACCADE_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"
#include "utils/Timer.h"
#include "vsgext/vsgext.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSaccadeStimulus : public CGenericStimulus 
{
public:
	CSaccadeStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CSaccadeStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();

	void DrawDot(double dDotSize, double dPosXPix, double dPosYPix,
		double screenXOffset, double screenYOffset);
	void GetNextPoint(double& dPosXDeg, double& dPosYDeg, bool bEvenFrame);
	
#if 0 /* Compiled out 11/1/11 while hacking in BG. -JVB */
	void DrawLandoltC(double dDotSize, double dPosXPix, double dPosYPix, double screenXOffset, double screenYOffset, VSGTRIVAL primary, VSGTRIVAL secondary, VSGTRIVAL bgColor, double angle);
	void DrawE(double dDotSize, double dPosXPix, double dPosYPix, double screenXOffset, double screenYOffset, VSGTRIVAL primary, VSGTRIVAL secondary, VSGTRIVAL bgColor, double angle);
#endif

	virtual CString StimulusType();

private:
	void InitLutBuffer();
	void InitNoiseHandling();
	void CleanUpNoiseHandling();
	void InitGratings();
	void CleanUpGratings();

	void doOscillations(int iFrameNum);

	bool bEvenFrame;
	int iScrWidthPix, iScrHeightPix;
	int iOldLocIndex, iOlderLocIndex;
	int iPage;
	bool bDrawNext;
	double dPosXDeg, dPosYDeg;
	bool bWasPrimary;

	double dCalculatedFreq;
	double dAmplitudePix;
	int iFrameNum;
	etu::CTimer theMaxWaitTimer;

	VsgExt::IGrating* m_primarySpec;
	VsgExt::IGrating* m_secondarySpec;
	VsgExt::NoiseBgController* m_noiseBgController;
	VsgExt::NoiseColorMap* m_noiseColorMap;

	VSGLUTBUFFER m_lutBuffer;
	VSGTRIVAL m_bgColor;
	VSGTRIVAL m_noiseColor;
};

#endif // SACCADE_STIMULUS_H
