/**
 * @file RFMapping.h
 * @brief 
 */

#ifndef RFMAPPING_H
#define RFMAPPING_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CRFMapping 
{
protected:
	float fXPosition;
	float fYPosition;
	float fWidth;
	float fHeight;
	float fBarWidth;
	float fBarHeight;
	float fOrientation;
	float fCircleRadius;
	
	bool bUseMouse;
	bool bUsePulse;
	bool bUseCircle;
	bool bUseBar;

public:
	CRFMapping();
	~CRFMapping();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);

	float getXPosition() { return fXPosition; }
	void setXPosition(float f) { fXPosition = f;}

	float getYPosition() { return fYPosition; }
	void setYPosition(float f) { fYPosition = f;}

	float getWidth() { return fWidth; }
	void setWidth(float f) { fWidth = f;}

	float getHeight() { return fHeight; }
	void setHeight(float f) { fHeight = f;}

	float getBarWidth() { return fBarWidth; }
	void setBarWidth(float f) { fBarWidth = f;}

	float getBarHeight() { return fBarHeight; }
	void setBarHeight(float f) { fBarHeight = f;}

	float getOrientation() { return fOrientation; }
	void setOrientation(float f) { fOrientation = f; }

	float getCircleRadius() { return fCircleRadius; }
	void setCircleRadius(float f) { fCircleRadius = f; }

	bool useMouse() { return bUseMouse; }
	void setMouse(bool b) { bUseMouse = b; }

	bool usePulse() { return bUsePulse; }
	void setPulse(bool b) { bUsePulse = b; }

	bool useCircle() { return bUseCircle; }
	void setCircle(bool b) { bUseCircle = b; bUseBar = !b; }

	bool useBar() { return bUseBar; }
	void setBar(bool b) { bUseCircle = !b; bUseBar = b; }
};

#endif
