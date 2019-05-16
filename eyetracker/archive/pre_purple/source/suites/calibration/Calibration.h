/**
 * @file Calibration.h
 * @brief 
 */

#ifndef CALIBRATION_H
#define CALIBRATION_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CCalibration 
{
protected:
	float fSeparation;
	float fSize;
	int iEye;
	int acceptDelay;
		
	bool bShowAll;
	bool bCycle;	

public:
	CString defaultFile;

	const static int EYE_LT = 0;
	const static int EYE_RT = 1;
	const static int EYE_BOTH = 2;
	
	CCalibration();
	~CCalibration();
	
	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm);

	float getSeparation() { return fSeparation; }
	void setSeparation(float f) { fSeparation = f; }

	float getSize() { return fSize; }
	void setSize(float f) { fSize = f; }

	bool showAll() { return bShowAll; }
	void setShowAll(bool b) { bShowAll = b; }

	int getEye() { return iEye; }
	void setEye(int i) { iEye = i; }

	int getAcceptDelay() { return acceptDelay; }
	void setAcceptDelay(int i) { acceptDelay = i;}
	
	void setCycle(bool b) { bCycle = b; }
	bool getCycle() { return bCycle; }
};

#endif
