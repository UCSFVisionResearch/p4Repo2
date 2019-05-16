/**
 * @file RandomDotAnaglyph.h
 * @brief 
 */

#ifndef RANDOMDOTANAGLYPH_H
#define RANDOMDOTANAGLYPH_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CRandomDotAnaglyph 
{
protected:
	int iRed, iBlue,
		iRadio, iBGDisparity, iNoise, iTextDisparity, iMaxDisparity;

	CString displaytext;
	CString imagepath;

public:
	bool bStarted;

	CString defaultFile;
	
	CRandomDotAnaglyph();
	~CRandomDotAnaglyph();
		
	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm);
	
	int getRed() {return iRed;}
	void setRed(int i) {iRed=i;}

	int getBlue() {return iBlue;}
	void setBlue(int i) {iBlue=i;}

	int getRadio() {return iRadio;}
	void setRadio(int i) {iRadio=i;}

	void setText(CString str) { displaytext = str; }
	CString getText() { return displaytext; }
	
	void setImagePath(CString str) { imagepath = str; }
	CString getImagePath() { return imagepath; }

	int getBGDisparity() {return iBGDisparity;}
	void setBGDisparity(int i) {iBGDisparity=i;}

	int getNoise() {return iNoise;}
	void setNoise(int i) {iNoise=i;}

	int getTextDisparity() {return iTextDisparity;}
	void setTextDisparity(int i) {iTextDisparity=i;}

	int getMaxDisparity() {return iMaxDisparity;}
	void setMaxDisparity(int i) {iMaxDisparity=i;}
};

#endif
