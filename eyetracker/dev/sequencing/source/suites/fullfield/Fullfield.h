/**
 * @file Fullfield.h
 * @brief 
 */

#ifndef FULLFIELD_H
#define FULLFIELD_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CFullfield 
{
protected:
	float fBarWidth;
	float fDriftVelocity;
	int iDriftDirection;
	bool bUseFixation;
	int fixationRed, fixationBlue, fixationGreen;
	float fixationSize;
	float fixationXPos, fixationYPos;
	float fSpatialFreq;
	float fDuration;
	double dContrast;
	bool bNoise;
	bool bUseGlobalColors;

public:
	CString defaultFile;
	const static int DIR_UP = 1;
	const static int DIR_DN = 2;
	const static int DIR_LT = 3;
	const static int DIR_RT = 4;
	
	CFullfield();
	~CFullfield();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm);

	/* Accessors */
	inline float getBarWidth();
	inline void setBarWidth(float f);

	inline float getDriftVelocity();
	inline void setDriftVelocity(float f);

	inline float getFixationSize();
	inline void setFixationSize(float f);

	inline int getFixationRed();
	inline void setFixationRed(int f);

	inline int getFixationGreen();
	inline void setFixationGreen(int f);

	inline int getFixationBlue();
	inline void setFixationBlue(int f);

	inline int getDriftDirection();
	inline void setDriftDirection(int i);

	inline bool useFixation();
	inline void setUseFixation (bool b);

	inline float getFixationXPos();
	inline void setFixationXPos(float f);

	inline float getFixationYPos();
	inline void setFixationYPos(float f);

	inline float getSpatialFreq();
	inline void setSpatialFreq(float f);

	inline float getDuration();
	inline float getDurationMSec();
	inline void setDuration(float f);

	inline double getContrast();
	inline void setContrast(double d);

	inline bool getNoise();
	inline void setNoise(bool b);

	inline bool getUseGlobalColors();
	inline void setUseGlobalColors(bool b);
};

/* Accessor bodies. */
float CFullfield::getBarWidth() { return fBarWidth; }
void CFullfield::setBarWidth(float f) { fBarWidth = f;}

float CFullfield::getDriftVelocity() { return fDriftVelocity; }
void CFullfield::setDriftVelocity(float f) { fDriftVelocity = f; }

float CFullfield::getFixationSize() { return fixationSize; }
void CFullfield::setFixationSize(float f) { fixationSize = f; }

int CFullfield::getFixationRed() { return fixationRed; }
void CFullfield::setFixationRed(int f) { fixationRed = f;}

int CFullfield::getFixationGreen() { return fixationGreen; }
void CFullfield::setFixationGreen(int f) { fixationGreen = f;}

int CFullfield::getFixationBlue() { return fixationBlue; }
void CFullfield::setFixationBlue(int f) { fixationBlue = f;}

int CFullfield::getDriftDirection() { return iDriftDirection; }
void CFullfield::setDriftDirection(int i) { iDriftDirection = i; }

bool CFullfield::useFixation() { return bUseFixation; }
void CFullfield::setUseFixation (bool b) { bUseFixation = b; }

float CFullfield::getFixationXPos() { return fixationXPos; }
void CFullfield::setFixationXPos(float f) { fixationXPos = f; }

float CFullfield::getFixationYPos() { return fixationYPos; }
void CFullfield::setFixationYPos(float f) { fixationYPos = f; }

float CFullfield::getSpatialFreq() { return fSpatialFreq; }
void CFullfield::setSpatialFreq(float f) { fSpatialFreq = f; }

float CFullfield::getDuration() { return fDuration / 1000; }
float CFullfield::getDurationMSec() { return fDuration; }
void CFullfield::setDuration(float f) { fDuration = f * 1000; }

double CFullfield::getContrast() { return dContrast; }
void CFullfield::setContrast(double d) { dContrast = d; }

bool CFullfield::getNoise() { return bNoise; }
void CFullfield::setNoise(bool b) { bNoise = b; }

bool CFullfield::getUseGlobalColors() { return bUseGlobalColors; }
void CFullfield::setUseGlobalColors(bool b) { bUseGlobalColors = b; }

#endif
