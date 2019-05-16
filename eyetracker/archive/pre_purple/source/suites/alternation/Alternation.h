/**
 * @file Alternation.h
 * @brief 
 */

#ifndef ALTERNATION_H
#define ALTERNATION_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CAlternation 
{
protected:
	double fDotSize1, fDotSize2, fXPos1, fYPos1, fXPos2, fYPos2, fDiamond;
	int iRed1, iGreen1, iBlue1, iRed2, iGreen2, iBlue2;
	
public:
	CString defaultFile;

	CAlternation();
	~CAlternation();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm);

	double getDotSize1() {return fDotSize1;}
	void setDotSize1(double d) {fDotSize1=d;}

	double getDotSize2() {return fDotSize2;}
	void setDotSize2(double d) {fDotSize2=d;}

	double getXPos1() {return fXPos1;}
	void setXPos1(double d) {fXPos1=d;}

	double getYPos1() {return fYPos1;}
	void setYPos1(double d) {fYPos1=d;}

	double getXPos2() {return fXPos2;}
	void setXPos2(double d) {fXPos2=d;}

	double getYPos2() {return fYPos2;}
	void setYPos2(double d) {fYPos2=d;}

	int getRed1() {return iRed1;}
	void setRed1(int i) {iRed1=i;}

	int getGreen1() {return iGreen1;}
	void setGreen1(int i) {iGreen1=i;}

	int getBlue1() {return iBlue1;}
	void setBlue1(int i) {iBlue1=i;}

	int getRed2() {return iRed2;}
	void setRed2(int i) {iRed2=i;}

	int getGreen2() {return iGreen2;}
	void setGreen2(int i) {iGreen2=i;}

	int getBlue2() {return iBlue2;}
	void setBlue2(int i) {iBlue2=i;}

	double getDiamond() {return fDiamond;}
	void setDiamond(double d) {fDiamond=d;}
};

#endif
