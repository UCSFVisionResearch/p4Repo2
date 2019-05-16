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

	bool m_messageSent;

	const long m_screenWidthPix;
	const long m_screenHeightPix;

	/* To cache the offset from g_pSettings */
	const double m_screenOffsetXDeg;
	const double m_screenOffsetYDeg;

	const double m_crossXDeg;
	const double m_crossYDeg;
	double m_crossXPix;
	double m_crossYPix;
	double m_crossDiameterPix;
	const double m_crossThicknessPix;

	int m_pageNum;

	VSGTRIVAL m_bgColor;
	VSGTRIVAL m_crossColor;
	VSGTRIVAL m_stimColor;
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
};

#endif // RF_MAPPING_STIMULUS_H
