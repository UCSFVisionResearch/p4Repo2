/**
 * @file Amblyoscope.h
 * @brief 
 */

#ifndef AMBLYOSCOPE_H
#define AMBLYOSCOPE_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CAmblyoscope
{
protected:
	int iXOffset, iYOffset, iPicture, iLRed, iLGreen, iLBlue, iRRed, iRGreen, iRBlue, iPctMonocular,
		iFlashDuration, iBlankDuration, iFlashCenter, iFlashBlocks, iArmLength, iArmWidth, iSpaceSize,
		iBGBrightness;
	float fDeviatedRightX, fDeviatedRightY, fDeviatedLeftX, fDeviatedLeftY,
		fFixationX, fFixationY, fFixationSize, fCenterX, fCenterY;
	bool bBackground, bFlashTest, bFixationDot, bAlternate;

public:
	CString defaultFile;

	CAmblyoscope();
	~CAmblyoscope();
	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm);

	int getXOffset() {return iXOffset;}
	void setXOffset(int i) {iXOffset=i;}

	int getYOffset() {return iYOffset;}
	void setYOffset(int i) {iYOffset=i;}

	int getPicture() {return iPicture;}
	void setPicture(int i) {iPicture=i;}

	int getLRed() {return iLRed;}
	void setLRed(int i) {iLRed=i;}

	int getLGreen() {return iLGreen;}
	void setLGreen(int i) {iLGreen=i;}

	int getLBlue() {return iLBlue;}
	void setLBlue(int i) {iLBlue=i;}

	int getRRed() {return iRRed;}
	void setRRed(int i) {iRRed=i;}

	int getRGreen() {return iRGreen;}
	void setRGreen(int i) {iRGreen=i;}

	int getRBlue() {return iRBlue;}
	void setRBlue(int i) {iRBlue=i;}
	
	bool getBackground() {return bBackground;}
	void setBackground(bool b) {bBackground=b;}

	bool getFlashTest() {return bFlashTest;}
	void setFlashTest(bool b) {bFlashTest=b;}
	
	int getPctMonocular() {return iPctMonocular;}
	void setPctMonocular(int i) {iPctMonocular=i;}
	
	int getFlashDuration() {return iFlashDuration;}
	void setFlashDuration(int i) {iFlashDuration=i;}

	int getBlankDuration() {return iBlankDuration;}
	void setBlankDuration(int i) {iBlankDuration=i;}

	float getDeviatedRightX() {return fDeviatedRightX;}
	void setDeviatedRightX(float f) {fDeviatedRightX=f;}

	float getDeviatedRightY() {return fDeviatedRightY;}
	void setDeviatedRightY(float f) {fDeviatedRightY=f;}

	float getDeviatedLeftX() {return fDeviatedLeftX;}
	void setDeviatedLeftX(float f) {fDeviatedLeftX=f;}

	float getDeviatedLeftY() {return fDeviatedLeftY;}
	void setDeviatedLeftY(float f) {fDeviatedLeftY=f;}

	int getFlashCenter() {return iFlashCenter;}
	void setFlashCenter(int i) {iFlashCenter=i;}

	int getFlashBlocks() {return iFlashBlocks;}
	void setFlashBlocks(int i) {iFlashBlocks=i;}

	int getArmLength() {return iArmLength;}
	void setArmLength(int i) {iArmLength=i;}

	int getArmWidth() {return iArmWidth;}
	void setArmWidth(int i) {iArmWidth=i;}

	int getSpaceSize() {return iSpaceSize;}
	void setSpaceSize(int i) {iSpaceSize=i;}

	bool getFixationDot() {return bFixationDot;}
	void setFixationDot(bool b) {bFixationDot=b;}

	float getFixationX() {return fFixationX;}
	void setFixationX(float f) {fFixationX=f;}

	float getFixationY() {return fFixationY;}
	void setFixationY(float f) {fFixationY=f;}

	float getFixationSize() {return fFixationSize;}
	void setFixationSize(float f) {fFixationSize=f;}

	float getCenterX() {return fCenterX;}
	void setCenterX(float f) {fCenterX=f;}

	float getCenterY() {return fCenterY;}
	void setCenterY(float f) {fCenterY=f;}

	int getBGBrightness() {return iBGBrightness;}
	void setBGBrightness(int i) {iBGBrightness=i;}

	bool getAlternate() {return bAlternate;}
	void setAlternate(bool b) {bAlternate=b;}
};

#endif
