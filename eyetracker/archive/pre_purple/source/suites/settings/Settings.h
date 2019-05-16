/**
 * @file Settings.h
 * @brief 
 */

#ifndef SETTINGS_H
#define SETTINGS_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <time.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSettings 
{
protected:
	int iEyePosition, iScreenPosition;
	int iRed, iGreen, iBlue;
	int iRedBG, iGreenBG, iBlueBG, iNSamplesEyePos;
	float fXOffset, fYOffset;
	float fDegPerVolt;
	float fScreenXOffset, fScreenYOffset;
	float fPixelSize;

public:
	CString defaultFile;
	CSerialComm theSerialComm;

	CSettings();
	~CSettings();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void tanScreenUnit2Unit(int unit1, double val1, int unit2, double *val2);
	void sendSettings(CSerialComm& theComm);
	
	int getViewDistance() { return iScreenPosition - iEyePosition; }

	int getEyePosition() { return iEyePosition; }
	void setEyePosition(int i) { iEyePosition = i; }

	int getScreenPosition() { return iScreenPosition; }
	void setScreenPosition(int i) { iScreenPosition = i; }

	int getRed() { return iRed; }
	void setRed(int i) { iRed = i; }

	int getGreen() { return iGreen; }
	void setGreen(int i) { iGreen = i; }

	int getBlue() { return iBlue; }
	void setBlue(int i) { iBlue = i; }

	int getRedBG() { return iRedBG; }
	void setRedBG(int i) { iRedBG = i; }

	int getGreenBG() { return iGreenBG; }
	void setGreenBG(int i) { iGreenBG = i; }

	int getBlueBG() { return iBlueBG; }
	void setBlueBG(int i) { iBlueBG = i; }

	float getXOffset() { return fXOffset; }
	void setXOffset(float f) {fXOffset = f; }

	float getYOffset() { return fYOffset; }
	void setYOffset(float f) {fYOffset = f; }

	float getDegPerVolt() { return fDegPerVolt; }
	void setDegPerVolt(float f) {fDegPerVolt = f; }

	float getScreenXOffset() { return fScreenXOffset; }
	void setScreenXOffset(float f) {fScreenXOffset = f; }

	float getScreenYOffset() { return fScreenYOffset; }
	void setScreenYOffset(float f) {fScreenYOffset = f; }

	float getPixelSize() { return fPixelSize; }
	void setPixelSize(float f) {fPixelSize = f; }

	int getNSamples() { return iNSamplesEyePos; }
	void setNSamples(int i) { iNSamplesEyePos = i; }
};

#endif
