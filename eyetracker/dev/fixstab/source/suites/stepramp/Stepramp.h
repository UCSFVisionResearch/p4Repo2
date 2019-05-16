/**
 * @file Stepramp.h
 * @brief 
 */

#ifndef STEPRAMP_H
#define STEPRAMP_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CStepRamp 
{
protected:
	float fStepSize;
	float fVelocity;
	float fExcursion;
	int iDirection;
	int iOnDelay;
	int iOffDelay;

	float fDotSize;
	float fStartXDeg, fStartYDeg;
	float fStepX, fStepY;
	int iPlateau;

	bool bStepUp;
	bool bStepDown;
	bool bStepRight;
	bool bStepLeft;

	float fStartXUp, fStartYUp,
		fStartXDown, fStartYDown,
		fStartXLeft, fStartYLeft,
		fStartXRight, fStartYRight;
	float fFixationX, fFixationY;

	bool bBehavior;

public:
	CString defaultFile;
	const static int DIR_UP = 1;
	const static int DIR_DN = 2;
	const static int DIR_LT = 3;
	const static int DIR_RT = 4;
	
	CStepRamp();
	~CStepRamp();

	float getFixationX();
	float getFixationY();
	void chooseDirection(); 
	void setDirection(int i);
	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm);

	float getDotSize() { return fDotSize; }
	void setDotSize(float f) { fDotSize = f;}

	float getVelocity() { return fVelocity; }
	void setVelocity(float f) { fVelocity = f; }

	float getExcursion() { return fExcursion; }
	void setExcursion(float f) { fExcursion = f; }

	float getStepSize() { return fStepSize; }
	void setStepSize(float f) { fStepSize = f;}

	int getOnDelay() { return iOnDelay; }
	void setOnDelay(int i) { iOnDelay = i;}

	int getOffDelay() { return iOffDelay; }
	void setOffDelay(int i) { iOffDelay = i;}

	bool getStepUp() { return bStepUp; }
	void setStepUp(bool b) { bStepUp = b; }

	bool getStepDown() { return bStepDown; }
	void setStepDown(bool b) { bStepDown = b; }
	
	bool getStepLeft() { return bStepLeft; }
	void setStepLeft(bool b) { bStepLeft = b; }
	
	bool getStepRight() { return bStepRight; }
	void setStepRight(bool b) { bStepRight = b; }

	bool useBehavior() { return bBehavior; }
	void setBehavior(bool b) { bBehavior = b; }

	float getStartXUp() { return fStartXUp; }
	void setStartXUp(float f) { fStartXUp = f; }

	float getStartYUp() { return fStartYUp; }
	void setStartYUp(float f) { fStartYUp = f; }

	float getStartXDown() { return fStartXDown; }
	void setStartXDown(float f) { fStartXDown = f; }

	float getStartYDown() { return fStartYDown; }
	void setStartYDown(float f) { fStartYDown = f; }

	float getStartXRight() { return fStartXRight; }
	void setStartXRight(float f) { fStartXRight = f; }
	
	float getStartYRight() { return fStartYRight; }
	void setStartYRight(float f) { fStartYRight = f; }
	
	float getStartXLeft() { return fStartXLeft; }
	void setStartXLeft(float f) { fStartXLeft = f; }
	
	float getStartYLeft() { return fStartYLeft; }
	void setStartYLeft(float f) { fStartYLeft = f; }

	float getStepX() { return fStepX; }
	float getStepY() { return fStepY; }
	float getStartPointX() { return fStartXDeg; }
	float getStartPointY() { return fStartYDeg; }

	int getPlateau() { return iPlateau; }
	void setPlateau(int i) { iPlateau = i;}
};

#endif
