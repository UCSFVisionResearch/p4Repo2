/**
 * @file Pursuit.h
 * @brief 
 */


#ifndef PURSUIT_H
#define PURSUIT_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CPursuit 
{
protected:
	float fFrequency;
	float fAmplitude;
	float fSize;
	int iOrientation;
	int iWaveform;
	bool bHighFreqEnabled;
	float fHighAmplitude;
	float fHighFrequency;

	bool bUseText, bAlternating;

	float fEccentricity;
	int iChangeR, iChangeG, iChangeB;
	CString sTextString;//getTextString() fails pretty hard when this variable is protected

public:
	CString defaultFile;
	bool redrawBG;

	const static int ORIENT_HORIZ = 1;
	const static int ORIENT_VERT = 2;
	const static int ORIENT_45DEG = 3;
	const static int ORIENT_135DEG = 4;

	const static int WAVEFORM_SINE = 1;
	const static int WAVEFORM_TRIANGLE = 2;

	CPursuit();
	~CPursuit();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm);
	
	float getFrequency() { return fFrequency; }
	void setFrequency(float f) { fFrequency = f;}

	float getAmplitude() { return fAmplitude; }
	void setAmplitude(float f) { fAmplitude = f; }

	float getSize() { return fSize; }
	void setSize(float f) { fSize = f; }

	int getOrientation() { return iOrientation; }
	void setOrientation(int i) { iOrientation = i;}

	int getWaveform() { return iWaveform; }
	void setWaveform(int i) { iWaveform = i; }

	bool isHighFreqEnabled() { return bHighFreqEnabled; }
	void setHighFreqEnabled(bool b) { bHighFreqEnabled = b; }

	float getHighFrequency() { return fHighFrequency; }
	void setHighFrequency(float f) { fHighFrequency = f;}

	float getHighAmplitude() { return fHighAmplitude; }
	void setHighAmplitude(float f) { fHighAmplitude = f; }

	bool getUseText() { return bUseText; }
	void setUseText(bool b) { bUseText= b; }

	CString getTextString() { return CString(sTextString); }
	void setTextString(CString s) { sTextString = s; }

	float getEccentricity() { return fEccentricity; }
	void setEccentricity(float f) { fEccentricity = f; }

	int getChangeR() { return iChangeR; }
	void setChangeR(int i) { iChangeR= i;}

	int getChangeG() { return iChangeG; }
	void setChangeG(int i) { iChangeG= i;}

	int getChangeB() { return iChangeB; }
	void setChangeB(int i) { iChangeB= i;}

	bool getAlternating() { return bAlternating; }
	void setAlternating(bool b) { bAlternating = b; }
};

#endif
