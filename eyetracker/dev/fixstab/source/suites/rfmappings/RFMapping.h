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
public:
	CRFMapping();
	~CRFMapping();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);

	/* Accessors ********************************************************/
	inline float getCrossX();
	inline void setCrossX(float f);

	inline float getCrossY();
	inline void setCrossY(float f);

	inline float getCrossDiameter();
	inline void setCrossDiameter(float f);

	inline float getCrossThickness();
	inline void setCrossThickness(float f);

	inline bool useCircle();
	inline void setCircle(bool b);

	inline bool useBar();
	inline void setBar(bool b);

	inline float getCircleRadius();
	inline void setCircleRadius(float f);

	inline float getBarWidth();
	inline void setBarWidth(float f);

	inline float getBarHeight();
	inline void setBarHeight(float f);

	inline float getOrientation();
	inline void setOrientation(float f);

	inline bool useMouse();
	inline void setMouse(bool b);

	inline bool usePulse();
	inline void setPulse(bool b);

	inline float getXPosition();
	inline void setXPosition(float f);

	inline float getYPosition();
	inline void setYPosition(float f);

	inline float getWidth();
	inline void setWidth(float f);

	inline float getHeight();
	inline void setHeight(float f);

private:
	float m_rfmCrossX;
	float m_rfmCrossY;
	float m_rfmCrossDiameter;
	float m_rfmCrossThickness;

	bool bUseCircle;
	bool bUseBar;
	float fCircleRadius;
	float fBarWidth;
	float fBarHeight;
	float fOrientation;
	bool bUseMouse;
	bool bUsePulse;
	
	float fXPosition;
	float fYPosition;
	float fWidth;
	float fHeight;
};

/*****************************************************************************
 * Inline Member Functions
 *****************************************************************************/

float CRFMapping::getCrossX() { return m_rfmCrossX; }
void CRFMapping::setCrossX(float f) { m_rfmCrossX = f; }

float CRFMapping::getCrossY() { return m_rfmCrossY; }
void CRFMapping::setCrossY(float f) { m_rfmCrossY = f; }

float CRFMapping::getCrossDiameter() { return m_rfmCrossDiameter; }
void CRFMapping::setCrossDiameter(float f) { m_rfmCrossDiameter = f; }

float CRFMapping::getCrossThickness() { return m_rfmCrossThickness; }
void CRFMapping::setCrossThickness(float f) { m_rfmCrossThickness = f; }

bool CRFMapping::useCircle() { return bUseCircle; }
void CRFMapping::setCircle(bool b) { bUseCircle = b; bUseBar = !b; }

bool CRFMapping::useBar() { return bUseBar; }
void CRFMapping::setBar(bool b) { bUseCircle = !b; bUseBar = b; }

float CRFMapping::getCircleRadius() { return fCircleRadius; }
void CRFMapping::setCircleRadius(float f) { fCircleRadius = f; }

float CRFMapping::getBarWidth() { return fBarWidth; }
void CRFMapping::setBarWidth(float f) { fBarWidth = f;}

float CRFMapping::getBarHeight() { return fBarHeight; }
void CRFMapping::setBarHeight(float f) { fBarHeight = f;}

float CRFMapping::getOrientation() { return fOrientation; }
void CRFMapping::setOrientation(float f) { fOrientation = f; }

bool CRFMapping::useMouse() { return bUseMouse; }
void CRFMapping::setMouse(bool b) { bUseMouse = b; }

bool CRFMapping::usePulse() { return bUsePulse; }
void CRFMapping::setPulse(bool b) { bUsePulse = b; }

float CRFMapping::getXPosition() { return fXPosition; }
void CRFMapping::setXPosition(float f) { fXPosition = f; }

float CRFMapping::getYPosition() { return fYPosition; }
void CRFMapping::setYPosition(float f) { fYPosition = f; }

float CRFMapping::getWidth() { return fWidth; }
void CRFMapping::setWidth(float f) { fWidth = f; }

float CRFMapping::getHeight() { return fHeight; }
void CRFMapping::setHeight(float f) { fHeight = f; }

#endif
