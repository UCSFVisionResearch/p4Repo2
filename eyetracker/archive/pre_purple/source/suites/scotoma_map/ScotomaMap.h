/**
 * @file ScotomaMap.cpp
 * @brief 
 */

#ifndef SCOTOMAMAP_H
#define SCOTOMAMAP_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include <deque>
#include "comm/SerialComm.h"
#include "sys/sys_concurrency.h"
#include "ScotomaTrial.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CScotomaMap : public Mixin_ThreadAware 
{
public:
	CString defaultFile;

	typedef std::deque<CScotomaTrial*> TrialPtrContainer_t;
	
	const static int COLOR_PRIMARY = 0;
	const static int COLOR_SECONDARY = 1;
	const static int COLOR_TERTIARY = 8;
	const static int COLOR_ALTERNATE = 2;
	const static int COLOR_RANDOM = 3;
	const static int COLOR_GLOBAL = 4;

	const static int REWARD_ANY = 0;
	const static int REWARD_RANDOM = 1;
	const static int REWARD_TERTIARYRANDOM = 2;
	const static int REWARD_TRAINING = 3;

	CScotomaMap();
	~CScotomaMap();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm); 

	void GenerateCtrLocations();

	void getRandomCtrLocation(double* outX, double* outY);

	void GenerateTrials();

	/*** Getters and setters below ******************************************/
	const TrialPtrContainer_t& getTrials();

	long getNumLocations();

	int getPrimaryRed();
	void setPrimaryRed(int i);

	int getPrimaryGreen();
	void setPrimaryGreen(int i);
	
	int getPrimaryBlue();
	void setPrimaryBlue(int i);

	int getSecondaryRed();
	void setSecondaryRed(int i);

	int getSecondaryGreen();
	void setSecondaryGreen(int i);

	int getSecondaryBlue();
	void setSecondaryBlue(int i);

	int getTertiaryRed();
	void setTertiaryRed(int i);
	
	int getTertiaryGreen();
	void setTertiaryGreen(int i);
		
	int getTertiaryBlue();
	void setTertiaryBlue(int i);

	int getNoiseRed();
	void setNoiseRed(int i);
		
	int getNoiseGreen();
	void setNoiseGreen(int i);
		
	int getNoiseBlue();
	void setNoiseBlue(int i);

	int getResponseDelay();
	void setResponseDelay(int i);

	int getWrongDelay();
	void setWrongDelay(int i);

	int getMisbehaviorDelay();
	void setMisbehaviorDelay(int i);

	int getNumRandomRepeats();
	void setNumRandomRepeats(int i);
	
	int getNoisePct();
	void setNoisePct(int i);

	int getNoiseSize();
	void setNoiseSize(int i);

	int getRampDuration();
	void setRampDuration(int i);

	int getDisplayTime();
	void setDisplayTime(int i);

	int getDisplayRandMax();
	void setDisplayRandMax(int i);

	int getDisplayRandMin();
	void setDisplayRandMin(int i);

	int getFlashInterval();
	void setFlashInterval(int i);

	int getFlashIntervalRandMax();
	void setFlashIntervalRandMax(int i);

	int getFlashIntervalRandMin();
	void setFlashIntervalRandMin(int i);

	int getCrossPix();
	void setCrossPix(int i);

	int getCenterColorRadio();
	bool setCenterColorRadio(int i);

	int getAlternateBlockSize();
	bool setAlternateBlockSize(int i);

	int getPeriphColorRadio();
	bool setPeriphColorRadio(int i);

	int getNumCatchTrials();
	bool setNumCatchTrials(int i);
		
	int getNumCatchTrials2();
	bool setNumCatchTrials2(int i);

	int getNumBlankTrials();
	bool setNumBlankTrials(int i);

	int getNumStepsH();
	bool setNumStepsH(int i);

	int getNumStepsV();
	bool setNumStepsV(int i);
	
	int getPctTimeAtCenter();
	bool setPctTimeAtCenter(int i);

	int getPctTimeAtPeriph();
	bool setPctTimeAtPeriph(int i);

	bool getUseNoise();
	void setUseNoise(bool b);
	
	bool getInterleaveNoise();
	void setInterleaveNoise(bool b);

	bool getRampUp();
	void setRampUp(bool b);

	bool getHideCenter();
	void setHideCenter(bool b);

	bool getRandomizeFlashInterval();
	void setRandomizeFlashInterval(bool b);
	
	bool getRandomizeDisplayTime();
	void setRandomizeDisplayTime(bool b);
	
	bool getAvoidZero();
	void setAvoidZero(bool b);

	bool getCenterPoints();
	void setCenterPoints(bool b);

	bool getOffAxis();
	void setOffAxis(bool b);

	float getCenterXPos();
	void setCenterXPos(float f);

	float getCenterYPos();
	void setCenterYPos(float f);

	float getCenterDotSize();
	void setCenterDotSize(float f);

	float getPeriphDotSize();
	void setPeriphDotSize(float f);
	
	float getStepSize();
	bool setStepSize(float f);

	float getCenterXRadius();
	bool setCenterXRadius(float f);
	
	float getCenterYRadius();
	bool setCenterYRadius(float f);

	float getPeriphXRadius();
	bool setPeriphXRadius(float f);
	
	float getPeriphYRadius();
	bool setPeriphYRadius(float f);
	
	int getRewardScheme();
	void setRewardScheme(int i);

	int getTertiaryRewardPct();
	void setTertiaryRewardPct(int i);
	
	int getRewardDelay();
	void setRewardDelay(int i);

	bool getDisplayDuringChoice();
	void setDisplayDuringChoice(bool b);

	bool getUseShapes();
	void setUseShapes(bool b);

	bool getSameBG();
	void setSameBG(bool b);
	
	bool getOccludeBG();
	void setOccludeBG(bool b);

	int getPctTimeAtEnriched();
	bool setPctTimeAtEnriched(int i);

	float getEnrichedX();
	bool setEnrichedX(float f);

	float getEnrichedY();
	bool setEnrichedY(float f);

	float getEnrichedXRadius();
	bool setEnrichedXRadius(float f);

	float getEnrichedYRadius();
	bool setEnrichedYRadius(float f);

	bool getDotGratings();
	void setDotGratings(bool b);

	int getDotGratingContrast();
	void setDotGratingContrast(int i);

	float getDotGratingCycles();
	void setDotGratingCycles(float f);

	float getSeparationV();
	void setSeparationV(float f);

	bool getUseKeyboard();
	void setUseKeyboard(bool b);

	int getNumCtrLocations();

	void setStepSizeDeg(float stepSizeDeg);
	float getStepSizeDeg();
	
	void setNumStepsX(int numStepsX);
	int getNumStepsX();
	
	void setNumStepsY(int numStepsY);
	int getNumStepsY();

private:
	void GenerateLocationsByCode(CScotomaTrial::STIM_CODE stimCode, 
		TrialPtrContainer_t& trials, long numGrids, long numDevFov, 
		long numCentFov, long numEn);
	void ZipContainers(int method, int blockSize, TrialPtrContainer_t& inLeft, 
		TrialPtrContainer_t& inRight, TrialPtrContainer_t& outZipped);
	void CleanUpTrials();
	void CleanUpCtrLocations();
	void CalcPercents(double& redPct, double& bluePct, double& blankPct, 
		double& purplePct, double& gridPct, double& fixatingPct, 
		double& deviatedPct, double& enrichedPct);

	int iPrimaryRed, iPrimaryGreen, iPrimaryBlue,		//red
		iSecondaryRed, iSecondaryGreen, iSecondaryBlue,	//blue
		iTertiaryRed, iTertiaryGreen, iTertiaryBlue,	//purple
		iNoiseRed, iNoiseGreen, iNoiseBlue,				//noise purple
		iResponseDelay, iWrongDelay, iMisbehaviorDelay,	//times for blank screens
		iNumRandomRepeats,
		iNoisePct, iNoiseSize,
		iRampDuration,
		iDisplayTime, iDisplayRandMax, iDisplayRandMin,	//display crosshair time
		iFlashInterval, iFlashIntervalRandMax, iFlashIntervalRandMin,	//periph flash time
		iCrossPix,										//crosshair thickness
		iCenterColorRadio, iAlternateBlockSize,			//center color options
		iPeriphColorRadio, iNumCatchTrials, iNumCatchTrials2, iNumBlankTrials,	//periph color options
		iNumStepsH, iNumStepsV,							//grid size
		iPctTimeAtCenter, iPctTimeAtPeriph,				//enrichment percentages
		iRewardScheme, iTertiaryRewardPct, iRewardDelay,
		iPctTimeAtEnriched, iDotGratingContrast;
	long iNumLocations;									//int iNumLocations

	bool bUseNoise, bInterleaveNoise,					//noise options
		bRampUp, bHideCenter, bRandomizeFlashInterval, bRandomizeDisplayTime,
		bAvoidZero, bCenterPoints, bOffAxis, bDisplayDuringChoice, bUseShapes, bSameBG, bOccludeBG, bDotGratings, bUseKeyboard;

	float fCenterXPos, fCenterYPos,
		fCenterDotSize,									//crosshair diameter
		fPeriphDotSize,									//periph dot radius
		fStepSize,										//grid spacing
		fCenterXRadius, fCenterYRadius,
		fPeriphXRadius, fPeriphYRadius,					//enrichment area
		fEnrichedX, fEnrichedY, fEnrichedXRadius, fEnrichedYRadius,
		fDotGratingCycles, fSeparationV;

	float m_stepSizeDeg;
	int m_numStepsX;
	int m_numStepsY;
	int m_numCtrLocations;

	double* m_ctrLocationsX;
	double* m_ctrLocationsY;

	TrialPtrContainer_t m_trials;

	/* Not supported. */
	CScotomaMap(const CScotomaMap& other);
	CScotomaMap& operator=(const CScotomaMap& other);
};

#endif
