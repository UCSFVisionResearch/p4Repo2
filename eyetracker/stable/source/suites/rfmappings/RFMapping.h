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

	void sendSettings(CSerialComm& theComm);

	/* Accessors ********************************************************/
	inline float getCrossX();
	inline void setCrossX(float f);

	inline float getCrossY();
	inline void setCrossY(float f);

	inline float getCrossDiameter();
	inline void setCrossDiameter(float f);

	inline float getCrossThickness();
	inline void setCrossThickness(float f);

	inline int getStimRed();
	inline void setStimRed(int);

	inline int getStimGreen();
	inline void setStimGreen(int);

	inline int getStimBlue();
	inline void setStimBlue(int);

	inline bool useCircle();
	inline void setCircle();

	inline bool useBar();
	inline void setBar();

	inline float getCircleRadius();
	inline void setCircleRadius(float f);

	inline float getBarWidth();
	inline void setBarWidth(float f);

	inline float getBarHeight();
	inline void setBarHeight(float f);

	inline float getOrientation();
	inline void setOrientation(float f);

	inline float getStepSizeDeg();
	inline void setStepSizeDeg(float step);

	inline int getStepsX();
	inline void setStepsX(int steps);

	inline int getStepsY();
	inline void setStepsY(int steps);

	inline int getDisplayTimeMs();
	inline void setDisplayTimeMs(int timeMs);

	inline bool getUseBlanks();
	inline void setUseBlanks(bool);

	inline int getBlankTimeMs();
	inline void setBlankTimeMs(int);

	inline bool getUseMouse();
	inline void setUseMouse();

	inline bool getUseGrid();
	inline void setUseGrid();

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

	int m_stimRed, m_stimGreen, m_stimBlue;

	bool bUseCircle;
	bool bUseBar;
	float fCircleRadius;
	float fBarWidth;
	float fBarHeight;
	float fOrientation;
	bool bUseMouse;
	bool bUseGrid;
	bool bUsePulse;

	float m_stepSizeDeg;
	int m_stepsX;
	int m_stepsY;
	int m_displayTimeMs;
	bool m_useBlanks;
	int m_blankTimeMs;
	
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

int CRFMapping::getStimRed() { return m_stimRed; }
void CRFMapping::setStimRed(int vv) { m_stimRed = vv; }

int CRFMapping::getStimGreen() { return m_stimGreen; }
void CRFMapping::setStimGreen(int vv) { m_stimGreen = vv; }

int CRFMapping::getStimBlue() { return m_stimBlue; }
void CRFMapping::setStimBlue(int vv) { m_stimBlue = vv; };

bool CRFMapping::useCircle() { return bUseCircle; }
void CRFMapping::setCircle() { bUseCircle = true; bUseBar = false; }

bool CRFMapping::useBar() { return bUseBar; }
void CRFMapping::setBar() { bUseCircle = false; bUseBar = true; }

float CRFMapping::getCircleRadius() { return fCircleRadius; }
void CRFMapping::setCircleRadius(float f) { fCircleRadius = f; }

float CRFMapping::getBarWidth() { return fBarWidth; }
void CRFMapping::setBarWidth(float f) { fBarWidth = f;}

float CRFMapping::getBarHeight() { return fBarHeight; }
void CRFMapping::setBarHeight(float f) { fBarHeight = f;}

float CRFMapping::getOrientation() { return fOrientation; }
void CRFMapping::setOrientation(float f) { fOrientation = f; }

bool CRFMapping::getUseMouse() { return bUseMouse; }
void CRFMapping::setUseMouse() { bUseMouse = true; bUseGrid = false; }

float CRFMapping::getStepSizeDeg() { return m_stepSizeDeg; }
void CRFMapping::setStepSizeDeg(float step) { m_stepSizeDeg = step; }

int CRFMapping::getStepsX() { return m_stepsX; }
void CRFMapping::setStepsX(int steps) { m_stepsX = steps; }

int CRFMapping::getStepsY() { return m_stepsY; }
void CRFMapping::setStepsY(int steps) { m_stepsY = steps; }

int CRFMapping::getDisplayTimeMs() { return m_displayTimeMs; }
void CRFMapping::setDisplayTimeMs(int timeMs) { m_displayTimeMs = timeMs; }

bool CRFMapping::getUseBlanks() { return m_useBlanks; }
void CRFMapping::setUseBlanks(bool bb) { m_useBlanks = bb; }

int CRFMapping::getBlankTimeMs() { return m_blankTimeMs; }
void CRFMapping::setBlankTimeMs(int timeMs) { m_blankTimeMs = timeMs; }

bool CRFMapping::getUseGrid() { return bUseGrid; }
void CRFMapping::setUseGrid() { bUseGrid = true; bUseMouse = false; }

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
