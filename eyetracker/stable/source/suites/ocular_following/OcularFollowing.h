/**
 * @file OcularFollowing.h
 * @brief 
 */

#ifndef OCULARFOLLOWING_H
#define OCULARFOLLOWING_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class COcularFollowing 
{
protected:
	float fSize;
	float xPosition, yPosition, fMinRadius, fMaxRadius;
	int iBkgSpeed, iTimeOut, iLatency, iBkgDuration, iNoiseSize, iNoisePct, iNoiseContrast;
	bool bDirections[4], bBehavior, bRandom;

public:
	CString defaultFile;
	const static int DIR_UP = 0;
	const static int DIR_DOWN = 1;
	const static int DIR_LEFT = 2;
	const static int DIR_RIGHT = 3;

	COcularFollowing();
	~COcularFollowing();

	int getDirection();
	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm);

	float getSize() { return fSize; }
	void setSize(float f) { fSize = f; }
	
	void setDirectionUp(bool b) { bDirections[0] = b; }
	void setDirectionDown(bool b) { bDirections[1] = b; }
	void setDirectionLeft(bool b) { bDirections[2] = b; }
	void setDirectionRight(bool b) { bDirections[3] = b; }

	bool getDirectionUp() { return bDirections[0]; }
	bool getDirectionDown() { return bDirections[1]; }
	bool getDirectionLeft() { return bDirections[2]; }
	bool getDirectionRight() { return bDirections[3]; }

	float getXPosition() { return xPosition; }
	void setXPosition(float x) { xPosition = x;}

	float getYPosition() { return yPosition; }
	void setYPosition(float y) { yPosition = y;}

	int getBackgroundSpeed() { return iBkgSpeed; }
	void setBackgroundSpeed(int i) { iBkgSpeed = i; }

	int getTimeOut() { return iTimeOut; }
	void setTimeOut(int i) { iTimeOut = i; }

	int getBackgroundDuration() { return iBkgDuration; }
	void setBackgroundDuration(int i) { iBkgDuration = i; }

	int getLatency() { return iLatency; }
	void setLatency(int i) { iLatency = i; }

	float getMinRadius() { return fMinRadius; }
	void setMinRadius(float f) { fMinRadius = f; }

	float getMaxRadius() { return fMaxRadius; }
	void setMaxRadius(float f) { fMaxRadius = f; }

	bool getBehavior() { return bBehavior; }
	void setBehavior(bool b) { bBehavior=b; }

	bool getRandom() { return bRandom; }
	void setRandom(bool b) { bRandom=b; }

	int getNoiseSize() { return iNoiseSize; }
	void setNoiseSize(int i) { iNoiseSize = i; }

	int getNoisePct() { return iNoisePct; }
	void setNoisePct(int i) { iNoisePct = i; }

	int getNoiseContrast() { return iNoiseContrast; }
	void setNoiseContrast(int i) { iNoiseContrast = i; }
};

#endif
