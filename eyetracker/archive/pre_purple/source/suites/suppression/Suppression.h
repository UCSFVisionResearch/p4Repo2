/**
 * @file Suppression.h
 * @brief 
 */

#ifndef SUPPRESSION_H
#define SUPPRESSION_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSuppression {
protected:
	int iGridSize, iDelay, iRedValue, iBlueValue, iTest, iBgLightRedValue, iBgLightGreenValue, iBgLightBlueValue,
		iBgDarkRedValue, iBgDarkGreenValue, iBgDarkBlueValue, iDelayms, iGridSpace, iDotTime, iDisplayTime, iOffTime,
		iSineColor, iNumGratings;
	float fStripeHeight, fSpatialFreq;
	bool bRed, bBlue, bText, bAdjustBG, bMultipleFullBlue;
	
	CString defaultFile;
	char defaultFileC[1024];

public:
	CSuppression();
	~CSuppression();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	
	int getGridSpace() {return iGridSpace;}
	void setGridSpace(int i) {iGridSpace=i;}

	int getGridSize() {return iGridSize;}
	void setGridSize(int i) {iGridSize=i;}

	int getDelay() {return iDelay;}
	void setDelay(int i) {iDelay=i;}

	int getRedValue() {return iRedValue;}
	void setRedValue(int i) {iRedValue=i;}

	int getBlueValue() {return iBlueValue;}
	void setBlueValue(int i) {iBlueValue=i;}

	int getTest() {return iTest;}
	void setTest(int i) {iTest=i;}
	
	int getBgLightRedValue() {return iBgLightRedValue;}
	void setBgLightRedValue(int i) {iBgLightRedValue=i;}

	int getBgLightGreenValue() {return iBgLightGreenValue;}
	void setBgLightGreenValue(int i) {iBgLightGreenValue=i;}

	int getBgLightBlueValue() {return iBgLightBlueValue;}
	void setBgLightBlueValue(int i) {iBgLightBlueValue=i;}

	int getBgDarkRedValue() {return iBgDarkRedValue;}
	void setBgDarkRedValue(int i) {iBgDarkRedValue=i;}

	int getBgDarkGreenValue() {return iBgDarkGreenValue;}
	void setBgDarkGreenValue(int i) {iBgDarkGreenValue=i;}

	int getBgDarkBlueValue() {return iBgDarkBlueValue;}
	void setBgDarkBlueValue(int i) {iBgDarkBlueValue=i;}

	int getDelayms() {return iDelayms;}
	void setDelayms(int i) {iDelayms=i;}

	bool getRedBlock() {return bRed;}
	void setRedBlock(bool b) {bRed=b;}
	bool getBlueBlock() {return bBlue;}
	void setBlueBlock(bool b) {bBlue=b;}
	bool getTextBlock() {return bText;}
	void setTextBlock(bool b) {bText=b;}

	double getStripeHeight() {return (double)fStripeHeight;}
	void setStripeHeight(double d) {fStripeHeight=(float)d;}

	bool getAdjustBG() {return bAdjustBG;}
	void setAdjustBG(bool b) {bAdjustBG=b;}

	int getDotTime() {return iDotTime;}
	void setDotTime(int i) {iDotTime=i;}

	int getDisplayTime() {return iDisplayTime;}
	void setDisplayTime(int i) {iDisplayTime=i;}

	int getOffTime() {return iOffTime;}
	void setOffTime(int i) {iOffTime=i;}

	double getSpatialFreq() {return (double)fSpatialFreq;}
	void setSpatialFreq(double d) {fSpatialFreq=(float)d;}

	int getSineColor() {return iSineColor;}
	void setSineColor(int i) {iSineColor=i;}

	int getNumGratings() {return iNumGratings;}
	void setNumGratings(int i) {iNumGratings=i;}

	//not saved
	bool getMultipleFullBlue() {return bMultipleFullBlue;}
	void setMultipleFullBlue(bool b) {bMultipleFullBlue=b;}
};

#endif // SUPPRESSION_H
