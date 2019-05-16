/**
 * @file Remember.h
 * @brief 
 */

#ifndef REMEMBER_H
#define REMEMBER_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"
#include "utils/Math.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CRemember 
{
protected:
	float fCenterXPos, fCenterYPos, fStepSize, fPeriphDotSize, fCenterDotSize,
		fPeriphXPos, fPeriphYPos, fPeriphXRadius, fPeriphYRadius, fPeriphXPosL, fPeriphYPosL,
		fCenterXRadius, fCenterYRadius;

	int iFlashInterval, iFlashDuration,
		iCenterColorRadio, iPeriphColorRadio,
		iCenterPrimaryRed, iCenterPrimaryGreen, iCenterPrimaryBlue,
		iCenterSecondaryRed, iCenterSecondaryGreen, iCenterSecondaryBlue,
		iPeriphPrimaryRed, iPeriphPrimaryGreen, iPeriphPrimaryBlue,
		iPeriphSecondaryRed, iPeriphSecondaryGreen, iPeriphSecondaryBlue,
		iNumStepsH, iNumStepsV, iNumLocations,
		iDistType, iAlternateBlockSize, iMaxDelay, 
		iStimType, iPostFlashDelay, iIntertrialDelay,
		iPostFlashRandMax, iPostFlashRandMin, iFlashIntervalRandMax, iFlashIntervalRandMin,
		iPctTimeAtPeriph,
		iPeriphTertiaryRed, iPeriphTertiaryGreen, iPeriphTertiaryBlue,
		iNoiseRed, iNoiseGreen, iNoiseBlue,
		iNumRandomRepeats, iNumCatchTrials, iNoiseSize,
		iCrossPix, iNumBlankTrials, iNoisePct, iDimOnsetMin, iDimOnsetMax, iDimTimeout, iAnswerPeriod, iDimBrightness,
		iResponseTimeout, iRampDuration, iNumCatchTrials2, iPctTimeAtCenter;

	bool bBehaviorCheck,
		bStepHoriz, bStepVert, bStep45, bStep135,
		bAvoidZero, bRedBlue,
		bRandomizeFlashInterval, bRandomizePostFlash,
		bSeqRandom,
		bUseNoise, bGridSteps, bUseDim, bHideCenter, bInterleaveNoise, bCenterPoints, bOffAxis,
		bResponse, bRampUp, bAutoPosition;

	double *pLocationsX, *pLocationsY;
	int *pLocationsType, *pLocationsIndex, *pLocationsClass;

public:
	CString defaultFile;
	
	const static int COLOR_PRIMARY = 0;
	const static int COLOR_SECONDARY = 1;
	const static int COLOR_ALTERNATE = 2;
	const static int COLOR_RANDOM = 3;
	const static int COLOR_TERTIARY = 8;
	const static int DIST_TYPE_XY = 4;
	const static int DIST_TYPE_RADIAL = 5;

	const static int STIM_MEMORY_GUIDED = 6;
	const static int STIM_PERIPHERAL_FLASH = 7;

	CRemember();
	~CRemember();
	
	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void GenerateLocations(); 
	void GenerateCatchTrials(); 
	void GenerateLocations2();
	bool readTag(FILE *fp, char *name, char *value, int size);
	void LoadXMLFile(bool bDefault);
	void SaveXMLFile(bool bDefault);
	void SendPrefsTextmark(CSerialComm &theComm);
	void sendSettings(CSerialComm& theComm);
	
	float getCenterXPos() {return fCenterXPos;}
	void setCenterXPos(float f) {fCenterXPos = f;}

	float getCenterYPos() {return fCenterYPos;}
	void setCenterYPos(float f) { fCenterYPos = f; }

	float getStepSize() {return fStepSize;}
	void setStepSize(float f) { fStepSize =f; }

	int getNumStepsH() {return iNumStepsH;}
	void setNumStepsH(int i) { iNumStepsH=i; }
	int getNumStepsV() {return iNumStepsV;}
	void setNumStepsV(int i) { iNumStepsV=i; }
	
	float getCenterDotSize() {return fCenterDotSize;}
	void setCenterDotSize(float f) { fCenterDotSize =f; }

	float getPeriphDotSize() {return fPeriphDotSize;}
	void setPeriphDotSize(float f) { fPeriphDotSize =f; }
	
	int getFlashInterval() {return iFlashInterval;}
	void setFlashInterval(int i) { iFlashInterval =i; }
	
	int getFlashDuration() {return iFlashDuration;}
	void setFlashDuration(int i) { iFlashDuration =i; }

	int getCenterPrimaryRed() {return iCenterPrimaryRed;}
	void setCenterPrimaryRed(int i) {iCenterPrimaryRed=i;}

	int getCenterPrimaryGreen() {return iCenterPrimaryGreen;}
	void setCenterPrimaryGreen(int i) {iCenterPrimaryGreen=i;}

	int getCenterPrimaryBlue() {return iCenterPrimaryBlue;}
	void setCenterPrimaryBlue(int i) {iCenterPrimaryBlue=i;}

	int getCenterSecondaryRed() {return iCenterSecondaryRed;}
	void setCenterSecondaryRed(int i) {iCenterSecondaryRed=i;}

	int getCenterSecondaryGreen() {return iCenterSecondaryGreen;}
	void setCenterSecondaryGreen(int i) {iCenterSecondaryGreen=i;}

	int getCenterSecondaryBlue() {return iCenterSecondaryBlue;}
	void setCenterSecondaryBlue(int i) {iCenterSecondaryBlue=i;}

	int getPeriphPrimaryRed() {return iPeriphPrimaryRed;}
	void setPeriphPrimaryRed(int i) {iPeriphPrimaryRed=i;}

	int getPeriphPrimaryGreen() {return iPeriphPrimaryGreen;}
	void setPeriphPrimaryGreen(int i) {iPeriphPrimaryGreen=i;}
	
	int getPeriphPrimaryBlue() {return iPeriphPrimaryBlue;}
	void setPeriphPrimaryBlue(int i) {iPeriphPrimaryBlue=i;}

	int getPeriphSecondaryRed() {return iPeriphSecondaryRed;}
	void setPeriphSecondaryRed(int i) {iPeriphSecondaryRed=i;}

	int getPeriphSecondaryGreen() {return iPeriphSecondaryGreen;}
	void setPeriphSecondaryGreen(int i) {iPeriphSecondaryGreen=i;}

	int getPeriphSecondaryBlue() {return iPeriphSecondaryBlue;}
	void setPeriphSecondaryBlue(int i) {iPeriphSecondaryBlue=i;}

	int getCenterColorRadio() {return iCenterColorRadio;}
	void setCenterColorRadio(int i) {iCenterColorRadio=i;}

	int getPeriphColorRadio() {return iPeriphColorRadio;}
	void setPeriphColorRadio(int i) {iPeriphColorRadio=i;}

	int getStepVert() {return bStepVert;}
	void setStepVert(int i) {bStepVert=(i != 0);}

	int getStepHoriz() {return bStepHoriz;}
	void setStepHoriz(int i) {bStepHoriz=(i != 0);}

	int getStep45() {return bStep45;}
	void setStep45(int i) {bStep45=(i != 0);}

	int getStep135() {return bStep135;}
	void setStep135(int i) {bStep135=(i != 0);}

	int getDistType() {return iDistType;}
	void setDistType(int i) {iDistType=i;}

	int getAlternateBlockSize() {return iAlternateBlockSize;}
	void setAlternateBlockSize(int i) {iAlternateBlockSize=i;}

	bool useBehavior() {return bBehaviorCheck;}
	void setBehavior(bool b) {bBehaviorCheck=b;}

	bool getAvoidZero() {return bAvoidZero;}
	void setAvoidZero(bool b) {bAvoidZero=b;}

	int getMaxDelay() {return iMaxDelay;}
	void setMaxDelay(int i) {iMaxDelay=i;}

	int getStimType() { return iStimType; }
	void setStimType(int i) { iStimType = i; }

	int getPostFlashDelay() { return iPostFlashDelay; }
	void setPostFlashDelay(int i) { iPostFlashDelay = i; }

	int getIntertrialDelay() { return iIntertrialDelay; }
	void setIntertrialDelay(int i) { iIntertrialDelay = i; }
	
	bool getRedBlue() {return bRedBlue;}
	void setRedBlue(bool b) {bRedBlue=b;}

	bool useSeqRandom() {return bSeqRandom;}
	void setSeqRandom(bool b) {bSeqRandom=b;}

	bool getRandomizeFlashInterval() {return bRandomizeFlashInterval;}
	void setRandomizeFlashInterval(bool b) {bRandomizeFlashInterval=b;}
	
	bool getRandomizePostFlash() {return bRandomizePostFlash;}
	void setRandomizePostFlash(bool b) {bRandomizePostFlash=b;}
	
	int getPostFlashRandMax() { return iPostFlashRandMax; }
	void setPostFlashRandMax(int i) { iPostFlashRandMax = i; }
		
	int getPostFlashRandMin() { return iPostFlashRandMin; }
	void setPostFlashRandMin(int i) { iPostFlashRandMin = i; }
		
	int getFlashIntervalRandMax() { return iFlashIntervalRandMax; }
	void setFlashIntervalRandMax(int i) { iFlashIntervalRandMax = i; }
	
	int getFlashIntervalRandMin() { return iFlashIntervalRandMin; }
	void setFlashIntervalRandMin(int i) { iFlashIntervalRandMin = i; }
	
	int getPctTimeAtPeriph() { return iPctTimeAtPeriph; }
	void setPctTimeAtPeriph(int i) { iPctTimeAtPeriph= i; }

	float getPeriphXPos() { return fPeriphXPos; }
	void setPeriphXPos(float f) { fPeriphXPos= f; }

	float getPeriphYPos() { return fPeriphYPos; }
	void setPeriphYPos(float f) { fPeriphYPos= f; }

	float getPeriphXRadius() { return fPeriphXRadius; }
	void setPeriphXRadius(float f) { fPeriphXRadius= f; }
	
	float getPeriphYRadius() { return fPeriphYRadius; }
	void setPeriphYRadius(float f) { fPeriphYRadius= f; }

	int getPeriphTertiaryRed() { return iPeriphTertiaryRed; }
	void setPeriphTertiaryRed(int i) { iPeriphTertiaryRed= i; }
	
	int getPeriphTertiaryGreen() { return iPeriphTertiaryGreen; }
	void setPeriphTertiaryGreen(int i) { iPeriphTertiaryGreen= i; }
		
	int getPeriphTertiaryBlue() { return iPeriphTertiaryBlue; }
	void setPeriphTertiaryBlue(int i) { iPeriphTertiaryBlue= i; }
		
	int getNoiseRed() { return iNoiseRed; }
	void setNoiseRed(int i) { iNoiseRed= i; }
		
	int getNoiseGreen() { return iNoiseGreen; }
	void setNoiseGreen(int i) { iNoiseGreen= i; }
		
	int getNoiseBlue() { return iNoiseBlue; }
	void setNoiseBlue(int i) { iNoiseBlue= i; }
		
	int getNumRandomRepeats() { return iNumRandomRepeats; }
	void setNumRandomRepeats(int i) { iNumRandomRepeats= i; }
		
	bool getUseNoise() { return bUseNoise; }
	void setUseNoise(bool b) { bUseNoise= b; }
		
	int getNumCatchTrials() { return iNumCatchTrials; }
	void setNumCatchTrials(int i) { iNumCatchTrials = i; }
		
	bool getGridSteps() { return bGridSteps; }
	void setGridSteps(bool b) { bGridSteps= b; }
	
	int getNoiseSize() { return iNoiseSize; }
	void setNoiseSize(int i) { iNoiseSize= i; }

	int getCrossPix() { return iCrossPix; }
	void setCrossPix(int i) { iCrossPix= i; }

	int getNumBlankTrials() { return iNumBlankTrials; }
	void setNumBlankTrials(int i) { iNumBlankTrials= i; }

	int getNoisePct() { return iNoisePct; }
	void setNoisePct(int i) { iNoisePct = i; }

	bool getUseDim() { return bUseDim; }
	void setUseDim(bool b) { bUseDim= b; }
	
	int getDimOnsetMin() { return iDimOnsetMin; }
	void setDimOnsetMin(int i) { iDimOnsetMin= i; }

	int getDimOnsetMax() { return iDimOnsetMax; }
	void setDimOnsetMax(int i) { iDimOnsetMax= i; }

	int getDimTimeout() { return iDimTimeout; }
	void setDimTimeout(int i) { iDimTimeout= i; }

	int getAnswerPeriod() { return iAnswerPeriod; }
	void setAnswerPeriod(int i) { iAnswerPeriod= i; }

	int getDimBrightness() { return iDimBrightness; }
	void setDimBrightness(int i) { iDimBrightness= i; }

	bool getHideCenter() { return bHideCenter; }
	void setHideCenter(bool b) { bHideCenter= b; }

	bool getInterleaveNoise() { return bInterleaveNoise; }
	void setInterleaveNoise(bool b) { bInterleaveNoise= b; }

	bool getCenterPoints() { return bCenterPoints; }
	void setCenterPoints(bool b) { bCenterPoints= b; }

	bool getOffAxis() { return bOffAxis; }
	void setOffAxis(bool b) { bOffAxis= b; }

	bool getResponse() { return bResponse; }
	void setResponse(bool b) { bResponse= b; }

	int getResponseTimeout() { return iResponseTimeout; }
	void setResponseTimeout(int i) { iResponseTimeout= i; }
	
	float getPeriphXPosL() { return fPeriphXPosL; }
	void setPeriphXPosL(float f) { fPeriphXPosL= f; }

	float getPeriphYPosL() { return fPeriphYPosL; }
	void setPeriphYPosL(float f) { fPeriphYPosL= f; }

	bool getRampUp() { return bRampUp; }
	void setRampUp(bool b) { bRampUp= b; }

	int getRampDuration() { return iRampDuration; }
	void setRampDuration(int i) { iRampDuration= i; }

	int getNumCatchTrials2() { return iNumCatchTrials2; }
	void setNumCatchTrials2(int i) {iNumCatchTrials2 = i; }

	int getPctTimeAtCenter() { return iPctTimeAtCenter; }
	void setPctTimeAtCenter(int i) {iPctTimeAtCenter= i; }

	float getCenterXRadius() { return fCenterXRadius; }
	void setCenterXRadius(float f) { fCenterXRadius= f; }

	float getCenterYRadius() { return fCenterYRadius; }
	void setCenterYRadius(float f) { fCenterYRadius= f; }

	bool getAutoPosition() { return bAutoPosition; }
	void setAutoPosition(bool b) { bAutoPosition = b; }

	double getLocationX(int iIndex) { return pLocationsX[iIndex]; }
	double getLocationY(int iIndex) { return pLocationsY[iIndex]; }

	int getLocationIndex(int iIndex) { return pLocationsIndex[iIndex]; }
	int getLocationType(int iIndex) { return pLocationsType[iIndex]; }
	int getLocationClass(int iIndex) { return pLocationsClass[iIndex]; }

	int getNumLocations() { return iNumLocations; }
};

#endif
