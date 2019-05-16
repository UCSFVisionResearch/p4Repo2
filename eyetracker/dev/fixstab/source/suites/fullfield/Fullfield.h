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
	int iContrast;
	bool bNoise;

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
	
	float getBarWidth() { return fBarWidth; }
	void setBarWidth(float f) { fBarWidth = f;}

	float getDriftVelocity() { return fDriftVelocity; }
	void setDriftVelocity(float f) { fDriftVelocity = f; }

	float getFixationSize() { return fixationSize; }
	void setFixationSize(float f) { fixationSize = f; }

	int getFixationRed() { return fixationRed; }
	void setFixationRed(int f) { fixationRed = f;}

	int getFixationGreen() { return fixationGreen; }
	void setFixationGreen(int f) { fixationGreen = f;}

	int getFixationBlue() { return fixationBlue; }
	void setFixationBlue(int f) { fixationBlue = f;}

	int getDriftDirection() { return iDriftDirection; }
	void setDriftDirection(int i) { iDriftDirection = i; }

	bool useFixation() { return bUseFixation; }
	void setUseFixation (bool b) { bUseFixation = b; }

	float getFixationXPos() { return fixationXPos; }
	void setFixationXPos(float f) { fixationXPos = f; }

	float getFixationYPos() { return fixationYPos; }
	void setFixationYPos(float f) { fixationYPos = f; }

	float getSpatialFreq() { return fSpatialFreq; }
	void setSpatialFreq(float f) { fSpatialFreq = f; }

	float getDuration() { return fDuration / 1000; }
	float getDurationMSec() { return fDuration; }
	void setDuration(float f) { fDuration = f * 1000; }

	int getContrast() { return iContrast; }
	void setContrast(int i) { iContrast = i; }

	bool getNoise() { return bNoise; }
	void setNoise(bool b) { bNoise = b; }
};

#endif
