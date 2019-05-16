/**
 * @file TrialSaccades.h
 * @brief 
 */

#ifndef TRIALSACCADES_H
#define TRIALSACCADES_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"
#include "sys/sys_concurrency.h"
#include "utils/Asserts.h"
#include "utils/Grid.h"
#include "SaccadeTrial.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CTrialSaccades : public Mixin_ThreadAware 
{
public:
	CString defaultFile;
	
	/**
	 * @brief Values for eye coverage radio selection.
	 */
	enum EYE_COVERAGE {
		COVERAGE_SPATIOTOPIC, /**< Random coverage across the eyes. */
		COVERAGE_RETINOTOPIC  /**< Equal, analogous coverage for each eye. */
	};

	/**
	 * @brief Values for fixation eye radio buttons.
	 */
	enum FIXATION_EYE {
		FIXATION_RANDOM,    /**< Select left or right eye to fixate. */
		FIXATION_CYCLOPEAN  /**< Eyes straddle fixation point. */
	};

	/** 
	 * @brief Values for radio button selections for center and peripheral 
	 * targets.
	 */
	enum COLOR_RADIO_VALUE {
		COLOR_PRIMARY   = 1,  /**< Primary color only. */
		COLOR_SECONDARY = 2,  /**< Secondary color only. */
		COLOR_TERTIARY  = 5,  /**< Tertiary color only. */
		COLOR_ALTERNATE = 3,  /**< Alternate, with an optional block size. */
		COLOR_RANDOM    = 4  /**< Randomly between primary and secondary. */
	};

	/**
	 * @brief Values for radio button selections of when the subject is
	 * triggered to make a saccade.
	 */
	enum SACCADE_TRIGGER {
		TRIGGER_FLASH,	  /**< Saccades triggered by target flash. */
		TRIGGER_SUBJECT,  /**< Saccades occur only when subject volunteers. */
		TRIGGER_CUE       /**< Saccades triggered by cross cue. */
	};

	/**
	 * @brief Values for radio button selection for grid type.
	 */
	enum GRID_TYPE {
		GRID_CARTESIAN,  /**< Specified by step size and number of steps. */
		GRID_RADIAL      /**< Specified by polar and eccentricity angles. */
	};

	/**
	 * @brief The number and difficulty of cross-over saccades prompted.
	 * Has guaranteed -- and thus must always guarantee -- the following:
	 *   - Starts at 0;
	 *   - Contiguous;
	 *   - Increasing in difficulty;
	 *   - Easiest level is 'DIFFICULTY_NOXO' with no cross-overs; and
	 *   - Hardest level is 'DIFFICULTY_STANDARD' without removing any 
	 *     cross-overs.
	 * Avoid direct reference to DIFFICULTY_ levels other than NOXO and 
	 * STANDARD, as the intervening constants may change.
	 */
	enum DIFFICULTY_LEVEL {
		DIFFICULTY_NOXO = 0,  /**< No midline-spanning cross-overs. */
		DIFFICULTY_25,        /**< Few midline-spanning cross-overs. */
		DIFFICULTY_50,        /**< Half midline-spanning cross-overs. */
		DIFFICULTY_75,        /**< Most midline-spanning cross-overs. */
		DIFFICULTY_STANDARD,  /**< Fully randomized including all cross overs. */
		NUM_DIFFICULTY_LEVELS
	};

	CTrialSaccades();
	~CTrialSaccades();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm); 

	void GenerateTrials();
	CSaccadeTrial* GetNextTrial();

	/*** Getters and setters below ******************************************/
	const TrialPtrContainer_t& getTrials() const;

	EYE_COVERAGE getEyeCoverage() const;
	void setEyeCoverage(EYE_COVERAGE eyeCoverage);

	bool getUseApprox() const;
	void setUseApprox(bool useApprox);

	double getDeviationAngle() const;
	void setDeviationAngle(double deviationAngle);

	FIXATION_EYE getFixationEye() const;
	void setFixationEye(FIXATION_EYE fixationEye);

	long getNumLocations() const;

	int getPrimaryRed() const;
	void setPrimaryRed(int i);

	int getPrimaryGreen() const;
	void setPrimaryGreen(int i);
	
	int getPrimaryBlue() const;
	void setPrimaryBlue(int i);

	int getSecondaryRed() const;
	void setSecondaryRed(int i);

	int getSecondaryGreen() const;
	void setSecondaryGreen(int i);

	int getSecondaryBlue() const;
	void setSecondaryBlue(int i);

	int getTertiaryRed() const;
	void setTertiaryRed(int i);

	int getTertiaryGreen() const;
	void setTertiaryGreen(int i);

	int getTertiaryBlue() const;
	void setTertiaryBlue(int i);

	int getFixationFlashDelay() const;
	void setFixationFlashDelay(int i);

	bool getBlankTimeOutScreen() const;
	void setBlankTimeOutScreen(bool bb);

	int getTargetActivePeriod() const;
	void setTargetActivePeriod(int i);

	int getWrongDelay() const;
	void setWrongDelay(int i);

	int getMisbehaviorDelay() const;
	void setMisbehaviorDelay(int i);

	int getRampDuration() const;
	void setRampDuration(int i);

	int getDisplayTime() const;
	void setDisplayTime(int i);

	int getDisplayRandMax() const;
	void setDisplayRandMax(int i);

	int getDisplayRandMin() const;
	void setDisplayRandMin(int i);

	SACCADE_TRIGGER getSaccadeTrigger() const;
	void setSaccadeTrigger(SACCADE_TRIGGER trigger);

	int getFlashInterval() const;
	void setFlashInterval(int i);

	int getFlashIntervalRandMax() const;
	void setFlashIntervalRandMax(int i);

	int getFlashIntervalRandMin() const;
	void setFlashIntervalRandMin(int i);

	int getCueTimeMin() const;
	void setCueTimeMin(int cue);

	int getCueTimeMax() const;
	void setCueTimeMax(int cue);

	int getDelayTimeMin() const;
	void setDelayTimeMin(int delay);

	int getDelayTimeMax() const;
	void setDelayTimeMax(int delay);

	int getLingerTimeMin() const;
	void setLingerTimeMin(int linger);

	int getLingerTimeMax() const;
	void setLingerTimeMax(int linger);

	int getCrossPix() const;
	void setCrossPix(int i);

	COLOR_RADIO_VALUE getCenterColorRadio() const;
	void setCenterColorRadio(COLOR_RADIO_VALUE crv);

	int getAlternateBlockSize() const;
	bool setAlternateBlockSize(int i);

	int getAlternateWithPurple() const;
	void setAlternateWithPurple(bool withP);

	int getPctRedCtr() const;
	int getPctBlueCtr() const;
	int getPctPurpleCtr() const;
	bool setCenterPercents(int pctRed, int pctBlue);

	COLOR_RADIO_VALUE getPeriphColorRadio() const;
	void setPeriphColorRadio(COLOR_RADIO_VALUE crv);

	int getPctRedGrids() const;
	int getPctBlueGrids() const;
	int getPctPurpleGrids() const;
	bool setGridPercents(int pctRed, int pctBlue);

	GRID_TYPE getGridType() const;
	void setGridType(GRID_TYPE gridType);

	/* Cartesian accessors ****************/
	int getNumStepsH() const;
	bool setNumStepsH(int i);

	int getNumStepsV() const;
	bool setNumStepsV(int i);

	float getStepSize() const;
	bool setStepSize(float f);

	/* Radial accessors *******************/
	int getAngleOffset() const;
	void setAngleOffset(int angleOffset);

	bool getAngle0() const;
	bool getAngle45() const;
	bool getAngle90() const;
	bool getAngle135() const;
	bool getAngle180() const;
	bool getAngle225() const;
	bool getAngle270() const;
	bool getAngle315() const;

	void setAngle0(bool checked);
	void setAngle45(bool checked);
	void setAngle90(bool checked);
	void setAngle135(bool checked);
	void setAngle180(bool checked);
	void setAngle225(bool checked);
	void setAngle270(bool checked);
	void setAngle315(bool checked);

	int getRadiusOffset() const;
	void setRadiusOffset(int radiusOffset);

	bool getRadiusOrigin() const;
	bool getRadius0() const;
	bool getRadius5() const;
	bool getRadius10() const;
	bool getRadius15() const;
	bool getRadius20() const;
	bool getRadius25() const;
	bool getRadius30() const;

	void setRadiusOrigin(bool checked);
	void setRadius0(bool checked);
	void setRadius5(bool checked);
	void setRadius10(bool checked);
	void setRadius15(bool checked);
	void setRadius20(bool checked);
	void setRadius25(bool checked);
	void setRadius30(bool checked);

	/* Noise handling *********************/
	bool getUseNoise() const;
	void setUseNoise(bool b);
	
	bool getInterleaveNoise() const;
	void setInterleaveNoise(bool b);

	bool getSameBG() const;
	void setSameBG(bool b);
	
	bool getOccludeBG() const;
	void setOccludeBG(bool b);

	int getNoisePct() const;
	void setNoisePct(int i);

	int getNoiseSize() const;
	void setNoiseSize(int i);

	int getNoiseRed() const;
	void setNoiseRed(int i);
		
	int getNoiseGreen() const;
	void setNoiseGreen(int i);
		
	int getNoiseBlue() const;
	void setNoiseBlue(int i);
	/**************************************/

	bool getRampUp() const;
	void setRampUp(bool b);

	bool getShowCenterFlash() const;
	void setShowCenterFlash(bool b);

	bool getShowCenterHold() const;
	void setShowCenterHold(bool b);

	bool getAdvanceEitherEye() const;
	void setAdvanceEitherEye(bool b);

	bool getRandomizeFlashInterval() const;
	void setRandomizeFlashInterval(bool b);
	
	bool getRandomizeDisplayTime() const;
	void setRandomizeDisplayTime(bool b);
	
	bool getAvoidZero() const;
	void setAvoidZero(bool b);

	bool getCenterPoints() const;
	void setCenterPoints(bool b);

	bool getOffAxis() const;
	void setOffAxis(bool b);

	float getCenterXPosPrim() const;
	void setCenterXPosPrim(float f);

	float getCenterYPosPrim() const;
	void setCenterYPosPrim(float f);

	float getCenterXPosSec() const;
	void setCenterXPosSec(float f);

	float getCenterYPosSec() const;
	void setCenterYPosSec(float f);

	float getCenterDotSize() const;
	void setCenterDotSize(float f);

	float getPeriphDotSize() const;
	void setPeriphDotSize(float f);

	int getSaccadeHoldPeriod() const;
	void setSaccadeHoldPeriod(int thp);
	
	int getRewardDelay() const;
	void setRewardDelay(int i);

	bool getDisplayDuringHoldPeriod() const;
	void setDisplayDuringHoldPeriod(bool b);

	DIFFICULTY_LEVEL getDifficultyLevel() const;
	void setDifficultyLevel(DIFFICULTY_LEVEL dl);

	bool getUseShapes() const;
	void setUseShapes(bool b);

	bool getDotGratings() const;
	void setDotGratings(bool b);

	int getDotGratingContrast() const;
	void setDotGratingContrast(int i);

	float getDotGratingCycles() const;
	void setDotGratingCycles(float f);

	float getSeparationV() const;
	void setSeparationV(float f);

	int getNumCtrLocations() const;

	void setCtrStepSize(float stepSizeDeg);
	float getCtrStepSize() const;
	
	void setNumStepsX(int numStepsX);
	int getNumStepsX() const;
	
	void setNumStepsY(int numStepsY);
	int getNumStepsY() const;

	bool getDebugResponderOn() const;
	void setDebugResponderOn(bool onOff);

	bool getDebugLabelsOn() const;
	void setDebugLabelsOn(bool onOff);

private:
	etu::pGrid_t MakePeripheralGrid();
	void CleanUpTrials();
	void setPercents(int pctRed, int pctBlue, int& whichRed, int& whichBlue, 
		int& whichPurple);

	int iPrimaryRed, iPrimaryGreen, iPrimaryBlue,		//red
		iSecondaryRed, iSecondaryGreen, iSecondaryBlue,	//blue
		iTertiaryRed, iTertiaryGreen, iTertiaryBlue,	//purple
		iNoiseRed, iNoiseGreen, iNoiseBlue,				//noise purple
		iTargetActivePeriod, iWrongDelay, iMisbehaviorDelay,	//times for blank screens
		iNoisePct, iNoiseSize,
		iRampDuration,
		iDisplayTime, iDisplayRandMax, iDisplayRandMin,	//display crosshair time
		iFlashInterval, iFlashIntervalRandMax, iFlashIntervalRandMin,	//periph flash time
		iCrossPix,										//crosshair thickness
		iNumStepsH, iNumStepsV,							//grid size
		iRewardDelay, m_fixationFlashDelay,
		iDotGratingContrast;

	COLOR_RADIO_VALUE m_centerColorRadio;
	int iAlternateBlockSize;
	bool m_alternateWithPurple;
	int m_pctRedCtr;
	int m_pctBlueCtr;
	int m_pctPurpleCtr;

	COLOR_RADIO_VALUE m_periphColorRadio;
	int m_pctRedGrids;
	int m_pctBlueGrids;
	int m_pctPurpleGrids;

	EYE_COVERAGE m_eyeCoverage;
	FIXATION_EYE m_fixationEye;
	double m_deviationAngle;
	bool m_useApprox;

	int m_saccadeHoldPeriod;

	bool bUseNoise, bInterleaveNoise,					//noise options
		bRampUp, bRandomizeFlashInterval, bRandomizeDisplayTime,
		bAvoidZero, bCenterPoints, bOffAxis, bDisplayDuringHoldPeriod, bUseShapes, bSameBG, bOccludeBG, bDotGratings;

	bool m_showCenterFlash;
	bool m_showCenterHold;
	bool m_advanceEitherEye;
	
	bool m_debugResponderOn;
	bool m_debugLabelsOn;

	bool m_blankTimeOutScreen;

	SACCADE_TRIGGER m_saccadeTrigger;
	int m_cueTimeMin;
	int m_cueTimeMax;
	int m_delayTimeMin;
	int m_delayTimeMax;
	int m_lingerTimeMin;
	int m_lingerTimeMax;

	DIFFICULTY_LEVEL m_difficultyLevel;

	float fCenterXPosPrim, fCenterYPosPrim, fCenterXPosSec, fCenterYPosSec,
		fCenterDotSize,									//crosshair diameter
		fPeriphDotSize,									//periph dot radius
		fStepSize,										//grid spacing
		fDotGratingCycles, fSeparationV;

	float m_stepSizeCtrDeg;
	int m_numCtrStepsX;
	int m_numCtrStepsY;
	int m_numCtrLocations;

	GRID_TYPE m_gridType;

	int m_angleOffset;
	bool m_angle0;
	bool m_angle45;
	bool m_angle90;
	bool m_angle135;
	bool m_angle180;
	bool m_angle225;
	bool m_angle270;
	bool m_angle315;

	int m_radiusOffset;
	bool m_radiusOrigin;
	bool m_radius0;
	bool m_radius5;
	bool m_radius10;
	bool m_radius15;
	bool m_radius20;
	bool m_radius25;
	bool m_radius30;

	CSaccadeTrialGenerator* m_trialGenerator;

	/* Declare m_trials first so it is constructed first by initializer 
	 * list. */
	TrialPtrContainer_t m_trials;
	TrialPtrContainer_t::const_iterator m_currentTrial;

	/* Not supported. */
	CTrialSaccades(const CTrialSaccades& other);
	CTrialSaccades& operator=(const CTrialSaccades& other);
};

#endif
