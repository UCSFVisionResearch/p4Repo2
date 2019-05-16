/**
 * @file Calibration.h
 * @brief 
 */

#ifndef REVERSEFIXATION_H
#define REVERSEFIXATION_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CReverseFixation 
{

public:
	CString defaultFile;

	CReverseFixation();
	~CReverseFixation();
	
	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm);

	/*Accessors**********************************************************************/
	inline int getPrimaryRed();
	inline void setPrimaryRed(int);
	inline int getPrimaryGreen();
	inline void setPrimaryGreen(int);
	inline int getPrimaryBlue();
	inline void setPrimaryBlue(int);

	inline int getSecondaryRed();
	inline void setSecondaryRed(int);
	inline int getSecondaryGreen();
	inline void setSecondaryGreen(int);
	inline int getSecondaryBlue();
	inline void setSecondaryBlue(int);

	inline bool getInitLeft();
	inline void setInitLeft(bool);

	inline float getInitLeftX();
	inline void setInitLeftX(float);
	inline float getInitLeftY();
	inline void setInitLeftY(float);

	inline float getInitRightX();
	inline void setInitRightX(float);
	inline float getInitRightY();
	inline void setInitRightY(float);

	inline float getDotSize();
	inline void setDotSize(float);
	inline float getDisplayTime();
	inline void setDisplayTime(float);

	inline int getMode();
	inline void setMode(int);

	inline float getWindowAllowancePix();
	inline void setWindowAllowancePix(float f);

	inline bool getUseWait();
	inline void setUseWait(bool b);

private:
	int m_primaryRed, m_primaryGreen, m_primaryBlue;
	int m_secondaryRed, m_secondaryGreen, m_secondaryBlue;

	bool bInitLeft;
	float fInitLeftX, fInitLeftY, fInitRightX, fInitRightY;
	float fDotSize, fDisplayTime;
	int m_mode;
	float fWindowAllowancePix;
	bool bUseWait;


};

/*****************************************************************************
 * Inline Member Functions
 *****************************************************************************/

int CReverseFixation::getPrimaryRed() { return m_primaryRed; }
void CReverseFixation::setPrimaryRed(int vv) { m_primaryRed = vv; }
int CReverseFixation::getPrimaryGreen() { return m_primaryGreen; }
void CReverseFixation::setPrimaryGreen(int vv) { m_primaryGreen = vv; }
int CReverseFixation::getPrimaryBlue() { return m_primaryBlue; }
void CReverseFixation::setPrimaryBlue(int vv) { m_primaryBlue = vv; };

int CReverseFixation::getSecondaryRed() { return m_secondaryRed; }
void CReverseFixation::setSecondaryRed(int vv) { m_secondaryRed = vv; }
int CReverseFixation::getSecondaryGreen() { return m_secondaryGreen; }
void CReverseFixation::setSecondaryGreen(int vv) { m_secondaryGreen = vv; }
int CReverseFixation::getSecondaryBlue() { return m_secondaryBlue; }
void CReverseFixation::setSecondaryBlue(int vv) { m_secondaryBlue = vv; };

bool CReverseFixation::getInitLeft() { return bInitLeft; }
void CReverseFixation::setInitLeft(bool bb) { bInitLeft =bb; }

float CReverseFixation::getInitLeftX() { return fInitLeftX; }
void CReverseFixation::setInitLeftX(float ff) { fInitLeftX = ff; }
float CReverseFixation::getInitLeftY() { return fInitLeftY; }
void CReverseFixation::setInitLeftY(float ff) { fInitLeftY = ff; }

float CReverseFixation::getInitRightX() { return fInitRightX; }
void CReverseFixation::setInitRightX(float ff) { fInitRightX = ff; }
float CReverseFixation::getInitRightY() { return fInitRightY; }
void CReverseFixation::setInitRightY(float ff) { fInitRightY = ff; }

float CReverseFixation::getDotSize() { return fDotSize; }
void CReverseFixation::setDotSize(float ff) { fDotSize= ff; }
float CReverseFixation::getDisplayTime() { return fDisplayTime; }
void CReverseFixation::setDisplayTime(float ff) { fDisplayTime = ff; }

int CReverseFixation::getMode() { return m_mode; }
void CReverseFixation::setMode(int vv) { m_mode = vv; }
float CReverseFixation::getWindowAllowancePix() { return fWindowAllowancePix; }
void CReverseFixation::setWindowAllowancePix(float f) {fWindowAllowancePix = f; }


bool CReverseFixation::getUseWait() { return bUseWait; }
void CReverseFixation::setUseWait(bool bb) { bUseWait =bb; }

#endif
