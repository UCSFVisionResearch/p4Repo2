/**
 * @file GratingsStimulus.h
 * @brief 
 */

#ifndef GRATINGS_STIMULUS_H
#define GRATINGS_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"
#include "utils/Timer.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CGratingsStimulus : public CGenericStimulus 
{
public:
	CGratingsStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CGratingsStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();

private:
	void drawBlankScreen(bool dot, bool leftfixation);
	void behaviorResetPulse();
	int changeShutters(CGratingsTrial *trial);
	void drawGrating(CGratingsTrial *trial, double tmpAvgLX, 
		double tmpAvgLY, double tmpAvgRX, double tmpAvgRY);

	VSGTRIVAL m_bgColor;
	VSGLUTBUFFER m_lutBuffer;

	int m_pageIx;
	int m_screenHeightPix;
	int m_screenWidthPix;
	int m_totalPresentations;
	double m_angle, m_lastAngle;
	double m_phase, m_lastPhase;
	double m_gridXDeg, m_gridYDeg;
	double m_gridXPix, m_gridYPix;
	int m_trialIndex;
	double m_alternateSide;
	double m_gCounter;
	int m_interleavedCounter;
	double m_interleavedState;
	etu::CTimer m_frameTimer;
	etu::CTimer m_fixationTimer;
	bool m_bReset;
	bool m_firstInit;
	int m_savedShutterState;

	double m_rfCurrentWidth;
	double m_rfCurrentX;
	double m_rfCurrentY;
};

#endif // GRATINGS_STIMULUS_H
