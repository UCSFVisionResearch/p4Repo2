/**
 * @file Saccade.h
 * @brief 
 */
#ifndef SACCADE_H
#define SACCADE_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"
#include "utils/Math.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSaccade 
{
public:
	CString defaultFile;

	const static int DIST_TYPE_RADIAL = 1;
	const static int DIST_TYPE_XY = 2;

	const static int ORIENT_HORIZ = 1;
	const static int ORIENT_VERT = 2;
	const static int ORIENT_45DEG = 3;
	const static int ORIENT_135DEG = 4;

	const static int COLOR_PRIMARY = 1;
	const static int COLOR_SECONDARY = 2;
	const static int COLOR_ALTERNATE = 3;
	const static int COLOR_RANDOM = 4;

	CSaccade();
	~CSaccade();

	int getRunTimeDisplayDuration();
	void GenerateLocations();
	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm);
	
	float getDotSize() { return fDotSize; }
	void setDotSize(float fDotSize) { this->fDotSize=fDotSize; }

	float getStepSize() { return fStepSize;}
	void setStepSize(float fStepSize) { this->fStepSize=fStepSize;}

	int getNumSteps() { return iNumStepsX; }
	void setNumSteps(int iNumStepsX) { this->iNumStepsX=iNumStepsX; }

	int getNumStepsY() { return iNumStepsY; }
	void setNumStepsY(int iNumStepsY) { this->iNumStepsY=iNumStepsY; }

	int getDisplayDuration() { return iDisplayDuration; }
	void setDisplayDuration(int iDisplayDuration) { this->iDisplayDuration=iDisplayDuration; }

	float getPointX() { return fPointX; }
	void setPointX(float ptX) { fPointX=ptX;}

	float getPointY() { return fPointY; }
	void setPointY(float ptY) { fPointY=ptY; }

	int getDistType() { return iDistType; }
	void setDistType(int iDistType) { this->iDistType = iDistType; }

	bool isReturnToPt() { return bReturnToPt; }
	void setReturnToPt(bool bReturnToPt) { this->bReturnToPt=bReturnToPt; }

	bool useAllDirs() { return bUseAllDirs; }
	void setUseAllDirs(bool bUseAllDirs) { this->bUseAllDirs = bUseAllDirs; }

	bool getStepHorizontal() { return bStepHorizontal; }
	void setStepHorizontal(bool b) { bStepHorizontal = b;}

	bool getStepVertical() { return bStepVertical; }
	void setStepVertical(bool b) { bStepVertical = b;}

	bool getStep45Deg() { return bStep45Deg; }
	void setStep45Deg(bool b) { bStep45Deg = b;}

	bool getStep135Deg() { return bStep135Deg; }
	void setStep135Deg(bool b) { bStep135Deg = b;}

	int getNumLocations() { return iNumLocations; }
	double getLocationX(int iIndex) { return pLocationsX[iIndex]; }
	double getLocationY(int iIndex) { return pLocationsY[iIndex]; }

	bool getRandomDurations() { return bRandomDurations; }
	void setRandomDurations(bool b) { bRandomDurations = b; }

	int getMinDuration() { return iMinDuration; }
	void setMinDuration(int i) { iMinDuration = i; }

	int getMaxDuration() { return iMaxDuration; }
	void setMaxDuration(int i) { iMaxDuration = i; }

	bool isOscillationEnabled() { return bEnableOscillations; }
	void setOsciallationEnabled(bool b) { bEnableOscillations = b; }

	float getOscillationAmplitude() { return fOscillationAmplitude; }
	void setOscillationAmplitude(float f) { fOscillationAmplitude = f; }

	float getOscillationFreq() { return fOscillationFreq; }
	void setOscillationFreq(float f) { fOscillationFreq = f; }

	int getOscillationDirection() { return iOscillationDirection; }
	void setOscillationDirection(int i) { iOscillationDirection = i; }

	bool useBehavior() { return bBehavior; }
	void setBehavior(bool b) { bBehavior = b; }

	bool getGridSteps() { return bGridSteps; }
	void setGridSteps(bool b) { bGridSteps = b; }

	int getMaxDisplayDuration() { return iMaxDisplayDuration; }
	void setMaxDisplayDuration(int i) { iMaxDisplayDuration = i; }
	
	int getBlankTime() { return iBlankTime; }
	void setBlankTime(int i) { iBlankTime = i; }

	int getJitterX() { return iJitterX; }
	void setJitterX(int i) { iJitterX = i; }

	int getJitterY() { return iJitterY; }
	void setJitterY(int i) { iJitterY = i; }

	bool avoidZero() { return bAvoidZero; }
	void setAvoidZero(bool b) { bAvoidZero = b; }

	int getRedPrimary() { return iRedPrimary; }
	void setRedPrimary(int i) { iRedPrimary = i; }

	int getGreenPrimary() { return iGreenPrimary; }
	void setGreenPrimary(int i) { iGreenPrimary = i; }

	int getBluePrimary() { return iBluePrimary; }
	void setBluePrimary(int i) { iBluePrimary = i; }

	int getRedSecondary() { return iRedSecondary; }
	void setRedSecondary(int i) { iRedSecondary = i; }

	int getGreenSecondary() { return iGreenSecondary; }
	void setGreenSecondary(int i) { iGreenSecondary = i; }

	int getBlueSecondary() { return iBlueSecondary; }
	void setBlueSecondary(int i) { iBlueSecondary = i; }

	int getColorSetting() { return iColorSetting; }
	void setColorSetting(int i ) { iColorSetting = i; }

	int getPointType() { return iPointType; }
	void setPointType(int i ) { iPointType = i; }

	bool getSequentialOrder() { return bSequentialOrder; }
	void setSequentialOrder(bool b) { bSequentialOrder = b; }

	bool getDisplayNoiseBg() { return m_displayNoiseBg; }
	void setDisplayNoiseBg(bool noiseOn) { m_displayNoiseBg = noiseOn; }

	bool getSameBgTrial() { return m_sameBgTrial; }
	void setSameBgTrial(bool sameBg) { m_sameBgTrial = sameBg; }

	bool getAlternateNoise() { return m_alternateNoise; }
	void setAlternateNoise(bool alternateNoise) { m_alternateNoise = alternateNoise; }

	bool getOccludeBg() { return m_occludeBg; }
	void setOccludeBg(bool occludeBg) { m_occludeBg = occludeBg; }

	int getRedNoise() { return m_noiseRed; }
	void setRedNoise(int redNoise) { m_noiseRed = redNoise; }

	int getGreenNoise() { return m_noiseGreen; }
	void setGreenNoise(int greenNoise) { m_noiseGreen = greenNoise; }

	int getBlueNoise() { return m_noiseBlue; }
	void setBlueNoise(int blueNoise) { m_noiseBlue = blueNoise; }

	int getNoisePercent() { return m_noisePercent; }
	void setNoisePercent(int noisePct) { m_noisePercent = noisePct; }

	int getNoiseSize() { return m_noiseSize; }
	void setNoiseSize(int noiseSize) { m_noiseSize = noiseSize; }

private:
	float fDotSize;		// deg
	float fStepSize;	// deg
	int iNumStepsX;	
	int iNumStepsY;	
	int iDisplayDuration; // msec
	bool bReturnToPt;
	float fPointX, fPointY;
	bool bUseAllDirs;
	bool bStepHorizontal;
	bool bStepVertical;
	bool bStep45Deg;
	bool bStep135Deg;
	int iDistType;
	double* pLocationsX;
	double* pLocationsY;
	int iNumLocations;
	bool bRandomDurations;
	int iMinDuration;
	int iMaxDuration;
	int iBlankTime;
	int iJitterX;
	int iJitterY;
	bool bAvoidZero, bGridSteps;

	bool bEnableOscillations;
	float fOscillationAmplitude;
	float fOscillationFreq;
	int iOscillationDirection;
	bool bBehavior;
	int iMaxDisplayDuration;

	int iRedPrimary, iGreenPrimary, iBluePrimary;
	int iRedSecondary, iGreenSecondary, iBlueSecondary;

	int iColorSetting;
	int iPointType;
	bool bSequentialOrder;

	bool m_displayNoiseBg;
	bool m_sameBgTrial;
	bool m_alternateNoise;
	bool m_occludeBg;
	int m_noiseRed;
	int m_noiseGreen;
	int m_noiseBlue;
	int m_noisePercent;
	int m_noiseSize;
};

#endif // SACCADE_H
