/**
 * @file SparseNoiseStimulus.h
 * @brief 
 */

#ifndef SPARSE_NOISE_STIMULUS_H
#define SPARSE_NOISE_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <afx.h>
#include "comm/SerialComm.h"
#include "suites/generic/GenericStimulus.h"
#include "utils/Timer.h"
#include "utils/Grid.h"
#include <vector>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSparseNoiseStimulus : public CGenericStimulus 
{
public:
	CSparseNoiseStimulus();
	virtual ~CSparseNoiseStimulus();
	
	/* Displays the stimulus */
	virtual bool DisplayStimulus();
	
	virtual CString StimulusType();

	void StopExecution();
	enum EYE_CODE {
		EYE_RIGHT = 0,  /**< Right eye was stimulated. */
		EYE_LEFT  = 1,  /**< Left eye was stimulated. */
	};

	enum FIX_COUNT {
		CODE_UNI		  = 1, /**< Single Fixation Point		*/
		CODE_BI			  = 2, /**< Two Fixation Points		*/
	};


	
private:
	void NewDrawingPage();
	void CreateStimuli();
	void PrepNextFrame();

	bool m_messageSent;

	const long m_screenWidthPix;
	const long m_screenHeightPix;

	/* To cache the offset from g_pSettings */
	const double m_screenOffsetXDeg;
	const double m_screenOffsetYDeg;

	double m_crossXDeg;
	double m_crossYDeg;
	double m_crossX2Deg;
	double m_crossY2Deg;
	double m_crossXPix;
	double m_crossYPix;
	double m_crossX2Pix;
	double m_crossY2Pix;
	double m_crossDiameterPix;
	const double m_crossThicknessPix;
	bool m_useBinocular;
	bool m_shuttered;
	const int m_swapInt;
	int m_swapCounter;
	EYE_CODE m_fixEye;
	double m_fixX, m_fixY;
	bool targetAcquired;
	bool m_useWait;
	const double m_windowAllowancePix;
	bool frameSyncHigh;

	char m_folderName[512];
	char m_fileName[512];
	int m_numFrames;
	double m_stimWidth;
	double m_minDist;
	int m_primaryCount;
	int m_secondaryCount;
	int m_tertiaryCount;


	int m_pageNum;

	VSGTRIVAL m_bgColor;
	VSGTRIVAL m_primaryColor;
	VSGTRIVAL m_secondaryColor;
	VSGTRIVAL m_tertiaryColor;
	VSGTRIVAL m_crossColor;
	double m_frameRate;
	double widthPix;
	double psuedoPixPerDeg;
	double fPixelSize;

	const bool m_useGrid;
	
	etu::CTimer m_sequenceTimer;
	etu::CTimer m_pulseTimer;

	
	const int m_displayTimeMs;
	FILE* primaryFile;
	FILE* secondaryFile;
	FILE* tertiaryFile;

	etu::SPoint m_fixOffsetPt;
	double m_fixCounter;

	
	class StimLocationGenerator
	{
	public:
		StimLocationGenerator();
		etu::SPoint next();
		int remainder();
		int size();
		void shuffle();
		etu::SPoint getFixationLocation(); //Gets next location in preshuffled Grid (only shuffled once to keep images same despite changing Grid Size) 
	private:
		etu::pGrid_t m_pGrid;
		etu::GridIter_t m_iter;
		etu::GridIter_t m_end;
		etu::pGrid_t m_fixationGrid;
		etu::GridIter_t m_fixIter;
		etu::GridIter_t m_fixEnd;
		bool m_random;
		/* Not supported */
		StimLocationGenerator(const StimLocationGenerator&);
		StimLocationGenerator& operator=(const StimLocationGenerator);
	};

	CSparseNoiseStimulus::StimLocationGenerator* m_pStimLocationGenerator;
	double m_frameCounter;

	std::deque<etu::SPoint> primaryFramePts;
	std::deque<etu::SPoint> secondaryFramePts;
	std::deque<etu::SPoint> tertiaryFramePts;

	etu::SPoint newPt;
	etu::SPoint addPt;


	
};

#endif // SPARSE_NOISE_STIMULUS_H
