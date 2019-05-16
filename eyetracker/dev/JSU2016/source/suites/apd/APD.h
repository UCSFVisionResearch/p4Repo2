/**
 * @file APD.h
 * @brief 
 */

#ifndef APD_H
#define APD_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CAPD 
{
protected:	
	bool bShowFixation;
	int iOnTime;
	int iOffTime;
	float fFixationSize;
	float fFixationPosX, fFixationPosY;
	int iFixationRed, iFixationGreen, iFixationBlue;
	int iOnRed, iOnGreen, iOnBlue;
	int iOffRed, iOffGreen, iOffBlue;
	int iField;
	int iIntertrialDelay;
	double dFieldWidth, dFieldHeight;
	double dCenterXLeft, dCenterYLeft;
	double dCenterXRight, dCenterYRight;
	int iFixationDelay;
	bool bBehavior;

public:
	const static int FIELD_LT = 0;
	const static int FIELD_RT = 1;
	const static int FIELD_ALTERNATE = 2;
	const static int FIELD_FULLSCREEN = 3;

	CString defaultFile;
	
	CAPD();
	~CAPD();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm);

	float getFixationSize() { return fFixationSize; }
	void setFixationSize(float f) { fFixationSize = f;}

	float getFixationPosX() { return fFixationPosX; }
	void setFixationPosX(float f) { fFixationPosX = f; }

	float getFixationPosY() { return fFixationPosY; }
	void setFixationPosY(float f) { fFixationPosY = f; }

	int getOnTime() { return iOnTime; }
	void setOnTime(int i) { iOnTime = i;}

	int getOffTime() { return iOffTime; }
	void setOffTime(int i) { iOffTime = i;}

	int getFixationRed() { return iFixationRed; }
	void setFixationRed(int i) { iFixationRed = i; }

	int getFixationGreen() { return iFixationGreen; }
	void setFixationGreen(int i) { iFixationGreen = i; }

	int getFixationBlue() { return iFixationBlue; }
	void setFixationBlue(int i) { iFixationBlue = i; }

	int getOnRed() { return iOnRed; }
	void setOnRed(int i) { iOnRed = i; }

	int getOnGreen() { return iOnGreen; }
	void setOnGreen(int i) { iOnGreen = i; }

	int getOnBlue() { return iOnBlue; }
	void setOnBlue(int i) { iOnBlue = i; }

	int getOffRed() { return iOffRed; }
	void setOffRed(int i) { iOffRed = i; }

	int getOffGreen() { return iOffGreen; }
	void setOffGreen(int i) { iOffGreen = i; }

	int getOffBlue() { return iOffBlue; }
	void setOffBlue(int i) { iOffBlue = i; }

	bool getShowFixation() { return bShowFixation; }
	void setShowFixation(bool b) { bShowFixation = b; }

	int getField() { return iField; }
	void setField(int i) {iField = i; }

	int getIntertrialDelay() { return iIntertrialDelay; }
	void setIntertrialDelay(int i) { iIntertrialDelay = i; }

	int getFixationDelay() { return iFixationDelay; }
	void setFixationDelay(int i) { iFixationDelay = i; }

	double getFieldWidth() { return dFieldWidth; }
	void setFieldWidth(double d) { dFieldWidth = d; }

	double getFieldHeight() { return dFieldHeight; }
	void setFieldHeight(double d) { dFieldHeight = d; }

	double getCenterXLeft() { return dCenterXLeft; }
	void setCenterXLeft(double d) { dCenterXLeft = d; }
	
	double getCenterYLeft() { return dCenterYLeft; }
	void setCenterYLeft(double d) { dCenterYLeft = d; }

	double getCenterXRight() { return dCenterXRight; }
	void setCenterXRight(double d) { dCenterXRight = d; }
	
	double getCenterYRight() { return dCenterYRight; }
	void setCenterYRight(double d) { dCenterYRight = d; }

	bool useBehavior() { return bBehavior; }
	void setBehavior(bool b) { bBehavior =b; }
};

#endif
