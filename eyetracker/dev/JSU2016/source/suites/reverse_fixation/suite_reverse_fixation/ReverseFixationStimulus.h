/**
 * @file CalibrationStimulus.h
 * @brief 
 */

#ifndef REVERSEFIXATION_STIMULUS_H
#define REVERSEFIXATION_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include <afxmt.h>
#include "suites/generic/GenericStimulus.h"
#include "utils/Timer.h"


/*****************************************************************************
 * Classes
 *****************************************************************************/

class CReverseFixationStimulus : public CGenericStimulus 
{
public:
	CReverseFixationStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CReverseFixationStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();

	void StopExecution();
	

private:
	void DrawStimulus();

	const long m_screenWidthPix;
	const long m_screenHeightPix;
	int m_currentPage;

	/* To cache the offset from g_pSettings */
	const double m_screenOffsetXDeg;
	const double m_screenOffsetYDeg;

	const double m_initLeftXDeg;
	const double m_initLeftYDeg;
	const double m_initRightXDeg;
	const double m_initRightYDeg;
	double m_stimXDeg;
	double m_stimYDeg;
	double m_stimXPix;
	double m_stimYPix;
	double m_stimSizePix;
	double m_stimSizeDeg;

	int m_pageNum;
	VSGTRIVAL m_bgColor;
	VSGTRIVAL m_PrimaryColor;
	VSGTRIVAL m_SecondaryColor;
	double m_frameRate;

	etu::CTimer m_timer;
	const int m_initLevel;
	const int m_mode;
	const int m_displayTime;

	const bool m_useWait;
	const float m_windowAllowancePix;
	bool targetAcquired;
	int m_currEye;

	bool m_messageSent;


};

#endif // REVERSEFIXATION_STIMULUS_H
