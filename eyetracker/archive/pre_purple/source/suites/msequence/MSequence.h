/**
 * @file MSequence.h
 * @brief 
 */

#ifndef MSEQUENCE_H
#define MSEQUENCE_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CMSequence 
{
protected:
	int iWidth, iOrder, iTap, iDelay, iNumBars, iHeight,
		iCurrentOffset, iOrder2, iTap2, iOrder3, iTap3, iOrientation,
		iRedR, iRedG, iRedB, iGreenR, iGreenG, iGreenB, iBlueR, iBlueG, iBlueB;
	double dHCenter, dVCenter;
	bool bTextmarks;

public:
	CString defaultFile;
	
	CMSequence();
	~CMSequence();
	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	int *generateSeq(int n, int t);

	int getWidth() {return iWidth;}
	void setWidth(int i) {iWidth=i;}

	int getOrder() {return iOrder;}
	void setOrder(int i) {iOrder=i;}

	int getTap() {return iTap;}
	void setTap(int i) {iTap=i;}

	int getDelay() {return iDelay;}
	void setDelay(int i) {iDelay=i;}

	int getNumBars() {return iNumBars;}
	void setNumBars(int i) {iNumBars=i;}

	int getHeight() {return iHeight;}
	void setHeight(int i) {iHeight=i;}

	double getHCenter() {return dHCenter;}
	void setHCenter(double d) {dHCenter=d;}

	double getVCenter() {return dVCenter;}
	void setVCenter(double d) {dVCenter=d;}

	int getOrientation() {return iOrientation;}
	void setOrientation(int i) {iOrientation=i;}

	int getCurrentOffset() {return iCurrentOffset;}
	void setCurrentOffset(int i) {iCurrentOffset=i;}

	bool getTextmarks() {return bTextmarks;}
	void setTextmarks(bool b) {bTextmarks=b;}

	int getOrder2() {return iOrder2;}
	void setOrder2(int i) {iOrder2=i;}

	int getTap2() {return iTap2;}
	void setTap2(int i) {iTap2=i;}

	int getOrder3() {return iOrder3;}
	void setOrder3(int i) {iOrder3=i;}

	int getTap3() {return iTap3;}
	void setTap3(int i) {iTap3=i;}

	int getRedR() {return iRedR;}
	void setRedR(int i) {iRedR=i;}

	int getRedG() {return iRedG;}
	void setRedG(int i) {iRedG=i;}

	int getRedB() {return iRedB;}
	void setRedB(int i) {iRedB=i;}

	int getGreenR() {return iGreenR;}
	void setGreenR(int i) {iGreenR=i;}

	int getGreenG() {return iGreenG;}
	void setGreenG(int i) {iGreenG=i;}

	int getGreenB() {return iGreenB;}
	void setGreenB(int i) {iGreenB=i;}

	int getBlueR() {return iBlueR;}
	void setBlueR(int i) {iBlueR=i;}

	int getBlueG() {return iBlueG;}
	void setBlueG(int i) {iBlueG=i;}

	int getBlueB() {return iBlueB;}
	void setBlueB(int i) {iBlueB=i;}
};

#endif // MSEQUENCE_H



