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
	float m_hSeparation, m_vSeparation;
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

	float getHSeparation() { return m_hSeparation; }
	void setHSeparation(float f) { m_hSeparation = f; }

	float getVSeparation() { return m_vSeparation; }
	void setVSeparation(float f) { m_vSeparation = f; }

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
