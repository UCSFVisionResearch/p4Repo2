/**
 * @file SparseNoise.h
 * @brief 
 */

#ifndef SPARSENOISE_H
#define SPARSENOISE_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"
#include "utils/Grid.h"
#include "SparseNoiseStimulus.h"


/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSparseNoise 
{
public:
	CSparseNoise();
	~CSparseNoise();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void CreateStimFolder();
	void LoadStimFolder();

	void sendSettings(CSerialComm& theComm);

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

	inline bool getUseShutters();
	inline void setUseShutters(bool);

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

	inline CSparseNoiseStimulus::EYE_CODE getStimEye();
	inline void setStimEye(CSparseNoiseStimulus::EYE_CODE);

	inline float getCircleRadius();
	inline void setCircleRadius(float f);

	inline float getMinDistDeg();
	inline void setMinDistDeg(float step);

	inline int getDisplayTimeMs();
	inline void setDisplayTimeMs(int timeMs);

	inline float getXPosition();
	inline void setXPosition(float f);

	inline float getYPosition();
	inline void setYPosition(float f);

	inline float getWindowAllowancePix();
	inline void setWindowAllowancePix(float f);

	inline bool getUseWait();
	inline void setUseWait(bool b);

	inline CString getFolderName();
	inline void setFolderName(CString fn);
	inline double getNumFrames();
	inline void setNumFrames(double i);
	inline float getStimWidth();
	inline void setStimWidth(float f);
	inline float getMinDist();
	inline void setMinDist(float f);
	inline int getPrimaryCount();
	inline void setPrimaryCount(int);
	inline int getSecondaryCount();
	inline void setSecondaryCount(int);
	inline int getTertiaryCount();
	inline void setTertiaryCount(int);

	inline bool getCreateFrames();
	inline void setCreateFrames(bool b);

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
	
	inline int getBGRed();
	inline void setBGRed(int);
	inline int getBGGreen();
	inline void setBGGreen(int);
	inline int getBGBlue();
	inline void setBGBlue(int);


	inline int getSeed();
	inline void setSeed(int);

	inline float getStepSizeDeg();
	inline void setStepSizeDeg(float step);

	inline int getStepsX();
	inline void setStepsX(int steps);

	inline int getStepsY();
	inline void setStepsY(int steps);
	
	inline bool getUseGrid();
	inline void setUseGrid(bool b);

private:

	float m_CrossX;
	float m_CrossY;
	float m_CrossDiameter;
	float m_CrossThickness;

	bool m_useBinocular;
	bool m_useShutters;
	float m_CrossX2;
	float m_CrossY2;
	int m_SwapInt;

	CSparseNoiseStimulus::EYE_CODE m_stimEye;
	
	//trialcrossPosition
	float m_fixX;
	float m_fixY;


	
	int m_displayTimeMs;
	
	float fWindowAllowancePix;
	bool m_useWait;

	CString m_FolderName;
	float m_NumFrames;
	float m_StimWidth;
	float m_MinDist;
	int m_PrimaryCount;
	int m_SecondaryCount;
	int m_TertiaryCount;
	int m_Seed;

	bool m_CreateFrames;

	int m_primaryRed, m_primaryGreen, m_primaryBlue;
	int m_secondaryRed, m_secondaryGreen, m_secondaryBlue;
	int m_tertiaryRed, m_tertiaryGreen, m_tertiaryBlue;
	int m_bgRed, m_bgGreen, m_bgBlue;

	float m_stepSizeDeg;
	int m_stepsX;
	int m_stepsY;
	bool bUseGrid;

};

/*****************************************************************************
 * Inline Member Functions
 *****************************************************************************/
float CSparseNoise::getFixX() {return m_fixX;}
void CSparseNoise::setFixX(float f) { m_fixX = f;}

float CSparseNoise::getFixY() {return m_fixY;}
void CSparseNoise::setFixY(float f) { m_fixY = f;}


float CSparseNoise::getCrossX() { return m_CrossX; }
void CSparseNoise::setCrossX(float f) { m_CrossX = f; }

float CSparseNoise::getCrossY() { return m_CrossY; }
void CSparseNoise::setCrossY(float f) { m_CrossY = f; }

float CSparseNoise::getCrossDiameter() { return m_CrossDiameter; }
void CSparseNoise::setCrossDiameter(float f) { m_CrossDiameter = f; }

float CSparseNoise::getCrossThickness() { return m_CrossThickness; }
void CSparseNoise::setCrossThickness(float f) { m_CrossThickness = f; }

bool CSparseNoise::getUseBinocular() { return m_useBinocular; }
void CSparseNoise::setUseBinocular(bool bb) { m_useBinocular = bb; }

bool CSparseNoise::getUseShutters() { return m_useShutters; }
void CSparseNoise::setUseShutters(bool bb) { m_useShutters = bb; }

float CSparseNoise::getCrossX2() { return m_CrossX2; }
void CSparseNoise::setCrossX2(float f) { m_CrossX2 = f; }

float CSparseNoise::getCrossY2() { return m_CrossY2; }
void CSparseNoise::setCrossY2(float f) { m_CrossY2 = f; }

int CSparseNoise::getSwapInt() { return m_SwapInt; }
void CSparseNoise::setSwapInt(int i) { m_SwapInt = i; }

CSparseNoiseStimulus::EYE_CODE CSparseNoise::getStimEye() { return m_stimEye; }
void CSparseNoise::setStimEye(CSparseNoiseStimulus::EYE_CODE vv) { m_stimEye = vv; }


int CSparseNoise::getDisplayTimeMs() { return m_displayTimeMs; }
void CSparseNoise::setDisplayTimeMs(int timeMs) { m_displayTimeMs = timeMs; }

float CSparseNoise::getWindowAllowancePix() { return fWindowAllowancePix; }
void CSparseNoise::setWindowAllowancePix(float f) {fWindowAllowancePix = f; }

bool CSparseNoise::getUseWait() {return m_useWait;}
void CSparseNoise::setUseWait(bool b) {m_useWait = b; }

CString CSparseNoise::getFolderName() {return m_FolderName;}
void CSparseNoise::setFolderName(CString fn) {m_FolderName = fn; }

double CSparseNoise::getNumFrames() {return m_NumFrames;}
void CSparseNoise::setNumFrames(double i) {m_NumFrames = i; }

float CSparseNoise::getStimWidth() {return m_StimWidth;}
void CSparseNoise::setStimWidth(float f) {m_StimWidth = f; }

float CSparseNoise::getMinDist() {return m_MinDist;}
void CSparseNoise::setMinDist(float f) {m_MinDist = f; }

int CSparseNoise::getPrimaryCount() {return m_PrimaryCount;}
void CSparseNoise::setPrimaryCount(int vv) {m_PrimaryCount = vv; }

int CSparseNoise::getSecondaryCount() {return m_SecondaryCount;}
void CSparseNoise::setSecondaryCount(int vv) {m_SecondaryCount = vv; }

int CSparseNoise::getTertiaryCount() {return m_TertiaryCount;}
void CSparseNoise::setTertiaryCount(int vv) {m_TertiaryCount = vv; }

bool CSparseNoise::getCreateFrames() {return m_CreateFrames;}
void CSparseNoise::setCreateFrames(bool b) {m_CreateFrames = b; }

int CSparseNoise::getPrimaryRed() { return m_primaryRed; }
void CSparseNoise::setPrimaryRed(int vv) { m_primaryRed = vv; }
int CSparseNoise::getPrimaryGreen() { return m_primaryGreen; }
void CSparseNoise::setPrimaryGreen(int vv) { m_primaryGreen = vv; }
int CSparseNoise::getPrimaryBlue() { return m_primaryBlue; }
void CSparseNoise::setPrimaryBlue(int vv) { m_primaryBlue = vv; };

int CSparseNoise::getSecondaryRed() { return m_secondaryRed; }
void CSparseNoise::setSecondaryRed(int vv) { m_secondaryRed = vv; }
int CSparseNoise::getSecondaryGreen() { return m_secondaryGreen; }
void CSparseNoise::setSecondaryGreen(int vv) { m_secondaryGreen = vv; }
int CSparseNoise::getSecondaryBlue() { return m_secondaryBlue; }
void CSparseNoise::setSecondaryBlue(int vv) { m_secondaryBlue = vv; };

int CSparseNoise::getTertiaryRed() { return m_tertiaryRed; }
void CSparseNoise::setTertiaryRed(int vv) { m_tertiaryRed = vv; }
int CSparseNoise::getTertiaryGreen() { return m_tertiaryGreen; }
void CSparseNoise::setTertiaryGreen(int vv) { m_tertiaryGreen = vv; }
int CSparseNoise::getTertiaryBlue() { return m_tertiaryBlue; }
void CSparseNoise::setTertiaryBlue(int vv) { m_tertiaryBlue = vv; };

int CSparseNoise::getBGRed() { return m_bgRed; }
void CSparseNoise::setBGRed(int vv) { m_bgRed = vv; }
int CSparseNoise::getBGGreen() { return m_bgGreen; }
void CSparseNoise::setBGGreen(int vv) { m_bgGreen = vv; }
int CSparseNoise::getBGBlue() { return m_bgBlue; }
void CSparseNoise::setBGBlue(int vv) { m_bgBlue = vv; };

int CSparseNoise::getSeed() {return m_Seed;}
void CSparseNoise::setSeed(int vv) {m_Seed = vv;};

float CSparseNoise::getStepSizeDeg() { return m_stepSizeDeg; }
void CSparseNoise::setStepSizeDeg(float step) { m_stepSizeDeg = step; }

int CSparseNoise::getStepsX() { return m_stepsX; }
void CSparseNoise::setStepsX(int steps) { m_stepsX = steps; }

int CSparseNoise::getStepsY() { return m_stepsY; }
void CSparseNoise::setStepsY(int steps) { m_stepsY = steps; }

bool CSparseNoise::getUseGrid() { return bUseGrid; }
void CSparseNoise::setUseGrid(bool b) { bUseGrid = b;}

#endif
