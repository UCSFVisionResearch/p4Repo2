/**
 * @file RFMapping.h
 * @brief 
 */

#ifndef RFMAPPING_H
#define RFMAPPING_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"
#include "RFTrial.h"
#include "utils/Grid.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CRFMapping 
{
public:
	CRFMapping();
	~CRFMapping();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);

	void sendSettings(CSerialComm& theComm);

	void GenerateTrials();
	CRFTrial* GetNextTrial();

	/* Accessors ********************************************************/
	inline float getFixX();
	inline void setFixX(float f);

	inline float getFixY();
	inline void setFixY(float f);
	
	inline float getCrossX();
	inline void setCrossX(float f);

	inline float getCrossY();
	inline void setCrossY(float f);

	inline bool getUseBinocular();
	inline void setUseBinocular(bool);

	inline float getCrossX2();
	inline void setCrossX2(float f);

	inline float getCrossY2();
	inline void setCrossY2(float f);

	inline int getSwapInt();
	inline void setSwapInt(int i);

	inline float getCrossDiameter();
	inline void setCrossDiameter(float f);

	inline float getCrossThickness();
	inline void setCrossThickness(float f);

	inline CRFTrial::RF_MODE getEyeMode();
	inline void setEyeMode(CRFTrial::RF_MODE);

	inline CRFTrial::EYE_CODE getStimEye();
	inline void setStimEye(CRFTrial::EYE_CODE);

	inline int getPrimaryRed();
	inline void setPrimaryRed(int);
	inline int getPrimaryGreen();
	inline void setPrimaryGreen(int);
	inline int getPrimaryBlue();
	inline void setPrimaryBlue(int);

	inline int getSecondaryRed();
	inline void setSecondaryRed(int);
	inline int getSecondaryGreen();
	inline void setSecondaryGreen(int);
	inline int getSecondaryBlue();
	inline void setSecondaryBlue(int);

	inline int getTertiaryRed();
	inline void setTertiaryRed(int);
	inline int getTertiaryGreen();
	inline void setTertiaryGreen(int);
	inline int getTertiaryBlue();
	inline void setTertiaryBlue(int);

	inline int getNoiseRed();
	inline void setNoiseRed(int);
	inline int getNoiseGreen();
	inline void setNoiseGreen(int);
	inline int getNoiseBlue();
	inline void setNoiseBlue(int);

	inline bool usePrimaryL();
	inline bool usePrimaryR();
	inline bool useSecondaryL();
	inline bool useSecondaryR();
	inline bool useTertiaryL();
	inline bool useTertiaryR();
	inline void setPrimaryL(bool);
	inline void setPrimaryR(bool);
	inline void setSecondaryL(bool);
	inline void setSecondaryR(bool);
	inline void setTertiaryL(bool);
	inline void setTertiaryR(bool);

	inline bool useCircle();
	inline void setCircle();

	inline bool useBar();
	inline void setBar();

	inline float getCircleRadius();
	inline void setCircleRadius(float f);

	inline float getBarWidth();
	inline void setBarWidth(float f);

	inline float getBarHeight();
	inline void setBarHeight(float f);

	inline float getOrientation();
	inline void setOrientation(float f);

	inline float getStepSizeDeg();
	inline void setStepSizeDeg(float step);

	inline int getStepsX();
	inline void setStepsX(int steps);

	inline int getStepsY();
	inline void setStepsY(int steps);

	inline int getDisplayTimeMs();
	inline void setDisplayTimeMs(int timeMs);

	inline bool getUseBlanks();
	inline void setUseBlanks(bool);

	inline int getBlankTimeMs();
	inline void setBlankTimeMs(int);

	inline bool getUseMouse();
	inline void setUseMouse();

	inline bool getUseGrid();
	inline void setUseGrid();

	inline bool usePulse();
	inline void setPulse(bool b);

	inline float getXPosition();
	inline void setXPosition(float f);

	inline float getYPosition();
	inline void setYPosition(float f);

	inline float getWidth();
	inline void setWidth(float f);

	inline float getHeight();
	inline void setHeight(float f);

	inline float getWindowAllowancePix();
	inline void setWindowAllowancePix(float f);

	/* Noise handling *********************/
	inline bool getUseNoise() const;
	inline void setUseNoise(bool b);
	
	inline bool getInterleaveNoise() const;
	inline void setInterleaveNoise(bool b);

	inline bool getSameBG() const;
	inline void setSameBG(bool b);
	
	inline bool getOccludeBG() const;
	inline void setOccludeBG(bool b);

	inline int getNoisePct() const;
	inline void setNoisePct(int i);

	inline int getNoiseSize() const;
	inline void setNoiseSize(int i);

	inline int getStimColor() const;
	inline void setStimColor(int i);

	inline int getCrossColor() const;
	inline void setCrossColor(int i);

	inline bool getUseWait();
	inline void setUseWait(bool b);

private:

	void CleanUpTrials();
	float m_CrossX;
	float m_CrossY;
	float m_CrossDiameter;
	float m_CrossThickness;

	bool m_useBinocular;
	CRFTrial::RF_MODE m_eyeMode;
	CRFTrial::EYE_CODE m_stimEye;
	CRFTrial::STIM_COLOR m_colorMode;
	float m_CrossX2;
	float m_CrossY2;
	int m_SwapInt;
	
	//trialcrossPosition
	float m_fixX;
	float m_fixY;


	int m_primaryRed, m_primaryGreen, m_primaryBlue;
	int m_secondaryRed, m_secondaryGreen, m_secondaryBlue;
	int m_tertiaryRed, m_tertiaryGreen, m_tertiaryBlue;
	int m_noiseRed, m_noiseGreen, m_noiseBlue;
	int m_NoisePct, m_NoiseSize;
	int m_stimColor, m_crossColor;
	bool bUsePrimaryL, bUsePrimaryR, bUseSecondaryL, bUseSecondaryR, bUseTertiaryL, bUseTertiaryR;

	bool bUseCircle;
	bool bUseBar;
	float fCircleRadius;
	float fBarWidth;
	float fBarHeight;
	float fOrientation;
	bool bUseMouse;
	bool bUseGrid;
	bool bUsePulse;
	bool bUseNoise, bInterleaveNoise, bSameBG, bOccludeBG;

	float m_stepSizeDeg;
	int m_stepsX;
	int m_stepsY;
	int m_displayTimeMs;
	bool m_useBlanks;
	int m_blankTimeMs;

	
	//Peripheral Stimulus
	float fXPosition;
	float fYPosition;
	float fWidth;
	float fHeight;

	float fWindowAllowancePix;
	bool m_useWait;

	CRFTrialGenerator* m_trialGenerator;
	RFTrialPtrContainer_t m_trials;
	RFTrialPtrContainer_t::const_iterator m_currentTrial;
};

/*****************************************************************************
 * Inline Member Functions
 *****************************************************************************/
float CRFMapping::getFixX() {return m_fixX;}
void CRFMapping::setFixX(float f) { m_fixX = f;}

float CRFMapping::getFixY() {return m_fixY;}
void CRFMapping::setFixY(float f) { m_fixY = f;}

CRFTrial::RF_MODE CRFMapping::getEyeMode(){return m_eyeMode;}
void CRFMapping::setEyeMode(CRFTrial::RF_MODE vv) {m_eyeMode =vv;}

float CRFMapping::getCrossX() { return m_CrossX; }
void CRFMapping::setCrossX(float f) { m_CrossX = f; }

float CRFMapping::getCrossY() { return m_CrossY; }
void CRFMapping::setCrossY(float f) { m_CrossY = f; }

float CRFMapping::getCrossDiameter() { return m_CrossDiameter; }
void CRFMapping::setCrossDiameter(float f) { m_CrossDiameter = f; }

float CRFMapping::getCrossThickness() { return m_CrossThickness; }
void CRFMapping::setCrossThickness(float f) { m_CrossThickness = f; }

bool CRFMapping::getUseBinocular() { return m_useBinocular; }
void CRFMapping::setUseBinocular(bool bb) { m_useBinocular = bb; }

float CRFMapping::getCrossX2() { return m_CrossX2; }
void CRFMapping::setCrossX2(float f) { m_CrossX2 = f; }

float CRFMapping::getCrossY2() { return m_CrossY2; }
void CRFMapping::setCrossY2(float f) { m_CrossY2 = f; }

int CRFMapping::getSwapInt() { return m_SwapInt; }
void CRFMapping::setSwapInt(int i) { m_SwapInt = i; }

CRFTrial::EYE_CODE CRFMapping::getStimEye() { return m_stimEye; }
void CRFMapping::setStimEye(CRFTrial::EYE_CODE vv) { m_stimEye = vv; }

int CRFMapping::getPrimaryRed() { return m_primaryRed; }
void CRFMapping::setPrimaryRed(int vv) { m_primaryRed = vv; }
int CRFMapping::getPrimaryGreen() { return m_primaryGreen; }
void CRFMapping::setPrimaryGreen(int vv) { m_primaryGreen = vv; }
int CRFMapping::getPrimaryBlue() { return m_primaryBlue; }
void CRFMapping::setPrimaryBlue(int vv) { m_primaryBlue = vv; };

int CRFMapping::getSecondaryRed() { return m_secondaryRed; }
void CRFMapping::setSecondaryRed(int vv) { m_secondaryRed = vv; }
int CRFMapping::getSecondaryGreen() { return m_secondaryGreen; }
void CRFMapping::setSecondaryGreen(int vv) { m_secondaryGreen = vv; }
int CRFMapping::getSecondaryBlue() { return m_secondaryBlue; }
void CRFMapping::setSecondaryBlue(int vv) { m_secondaryBlue = vv; };

int CRFMapping::getTertiaryRed() { return m_tertiaryRed; }
void CRFMapping::setTertiaryRed(int vv) { m_tertiaryRed = vv; }
int CRFMapping::getTertiaryGreen() { return m_tertiaryGreen; }
void CRFMapping::setTertiaryGreen(int vv) { m_tertiaryGreen = vv; }
int CRFMapping::getTertiaryBlue() { return m_tertiaryBlue; }
void CRFMapping::setTertiaryBlue(int vv) { m_tertiaryBlue = vv; };

int CRFMapping::getNoiseRed() { return m_noiseRed; }
void CRFMapping::setNoiseRed(int vv) { m_noiseRed = vv; }
int CRFMapping::getNoiseGreen() { return m_noiseGreen; }
void CRFMapping::setNoiseGreen(int vv) { m_noiseGreen = vv; }
int CRFMapping::getNoiseBlue() { return m_noiseBlue; }
void CRFMapping::setNoiseBlue(int vv) { m_noiseBlue = vv; };

bool CRFMapping::usePrimaryL(){return bUsePrimaryL;}
bool CRFMapping::usePrimaryR(){return bUsePrimaryR;}
bool CRFMapping::useSecondaryL(){return bUseSecondaryL;}
bool CRFMapping::useSecondaryR(){return bUseSecondaryR;}
bool CRFMapping::useTertiaryL(){return bUseTertiaryL;}
bool CRFMapping::useTertiaryR(){return bUseTertiaryR;}

void CRFMapping::setPrimaryL(bool bb){ bUsePrimaryL = bb;}
void CRFMapping::setPrimaryR(bool bb){ bUsePrimaryR = bb;}
void CRFMapping::setSecondaryL(bool bb){ bUseSecondaryL = bb;}
void CRFMapping::setSecondaryR(bool bb){ bUseSecondaryR = bb;}
void CRFMapping::setTertiaryL(bool bb){ bUseTertiaryL = bb;}
void CRFMapping::setTertiaryR(bool bb){ bUseTertiaryR = bb;}

bool CRFMapping::useCircle() { return bUseCircle; }
void CRFMapping::setCircle() { bUseCircle = true; bUseBar = false; }

bool CRFMapping::useBar() { return bUseBar; }
void CRFMapping::setBar() { bUseCircle = false; bUseBar = true; }

float CRFMapping::getCircleRadius() { return fCircleRadius; }
void CRFMapping::setCircleRadius(float f) { fCircleRadius = f; }

float CRFMapping::getBarWidth() { return fBarWidth; }
void CRFMapping::setBarWidth(float f) { fBarWidth = f;}

float CRFMapping::getBarHeight() { return fBarHeight; }
void CRFMapping::setBarHeight(float f) { fBarHeight = f;}

float CRFMapping::getOrientation() { return fOrientation; }
void CRFMapping::setOrientation(float f) { fOrientation = f; }

bool CRFMapping::getUseMouse() { return bUseMouse; }
void CRFMapping::setUseMouse() { bUseMouse = true; bUseGrid = false; }

float CRFMapping::getStepSizeDeg() { return m_stepSizeDeg; }
void CRFMapping::setStepSizeDeg(float step) { m_stepSizeDeg = step; }

int CRFMapping::getStepsX() { return m_stepsX; }
void CRFMapping::setStepsX(int steps) { m_stepsX = steps; }

int CRFMapping::getStepsY() { return m_stepsY; }
void CRFMapping::setStepsY(int steps) { m_stepsY = steps; }

int CRFMapping::getDisplayTimeMs() { return m_displayTimeMs; }
void CRFMapping::setDisplayTimeMs(int timeMs) { m_displayTimeMs = timeMs; }

bool CRFMapping::getUseBlanks() { return m_useBlanks; }
void CRFMapping::setUseBlanks(bool bb) { m_useBlanks = bb; }

int CRFMapping::getBlankTimeMs() { return m_blankTimeMs; }
void CRFMapping::setBlankTimeMs(int timeMs) { m_blankTimeMs = timeMs; }

bool CRFMapping::getUseGrid() { return bUseGrid; }
void CRFMapping::setUseGrid() { bUseGrid = true; bUseMouse = false; }

bool CRFMapping::usePulse() { return bUsePulse; }
void CRFMapping::setPulse(bool b) { bUsePulse = b; }

float CRFMapping::getXPosition() { return fXPosition; }
void CRFMapping::setXPosition(float f) { fXPosition = f; }

float CRFMapping::getYPosition() { return fYPosition; }
void CRFMapping::setYPosition(float f) { fYPosition = f; }

float CRFMapping::getWidth() { return fWidth; }
void CRFMapping::setWidth(float f) { fWidth = f; }

float CRFMapping::getWindowAllowancePix() { return fWindowAllowancePix; }
void CRFMapping::setWindowAllowancePix(float f) {fWindowAllowancePix = f; }

bool CRFMapping::getUseWait() {return m_useWait;}
void CRFMapping::setUseWait(bool b) {m_useWait = b; }

float CRFMapping::getHeight() { return fHeight; }
void CRFMapping::setHeight(float f) { fHeight = f; }


bool CRFMapping::getUseNoise() const { return bUseNoise; }
void CRFMapping::setUseNoise(bool b) { bUseNoise= b; }

bool CRFMapping::getInterleaveNoise() const { return bInterleaveNoise; }
void CRFMapping::setInterleaveNoise(bool b) { bInterleaveNoise= b; }

bool CRFMapping::getSameBG() const { return bSameBG; }
void CRFMapping::setSameBG(bool b) { bSameBG = b; }

bool CRFMapping::getOccludeBG() const { return bOccludeBG; }
void CRFMapping::setOccludeBG(bool b) { bOccludeBG = b; }

int CRFMapping::getNoisePct() const { return m_NoisePct; }
void CRFMapping::setNoisePct(int i) { m_NoisePct = i; }

int CRFMapping::getNoiseSize() const { return m_NoiseSize; }
void CRFMapping::setNoiseSize(int i) { m_NoiseSize= i; }

int CRFMapping::getStimColor() const { return m_stimColor; }
void CRFMapping::setStimColor(int i) { m_stimColor= i; }

int CRFMapping::getCrossColor() const { return m_crossColor; }
void CRFMapping::setCrossColor(int i) { m_crossColor= i; } 

#endif
