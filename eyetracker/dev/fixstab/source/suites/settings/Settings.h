/**
 * @file Settings.h
 * @brief 
 */

#ifndef SETTINGS_H
#define SETTINGS_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <time.h>
#include <utility>  /* std::pair */
#include "comm/SerialComm.h"

/*****************************************************************************
 * Typedefs
 *****************************************************************************/

typedef std::pair<double, double> dbl_pair;

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSettings 
{
protected:
	int iEyePosition, iScreenPosition;
	int iRed, iGreen, iBlue;
	int iRedBG, iGreenBG, iBlueBG, iNSamplesEyePos;
	float fXOffset, fYOffset;
	float fDegPerVolt;
	float fScreenXOffset, fScreenYOffset;
	float fPixelSize;

public:
	CString defaultFile;
	CSerialComm theSerialComm;

	CSettings();
	~CSettings();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void tanScreenUnit2Unit(int unit1, double val1, int unit2, double *val2) const;
	double tanScreenUnit2Unit(int unit1, double val1, int unit2) const;
	void sendSettings(CSerialComm& theComm);

	inline int getViewDistance() const;

	inline int getEyePosition() const;
	inline void setEyePosition(int i);

	inline int getScreenPosition() const;
	inline void setScreenPosition(int i);

	inline int getRed() const;
	inline void setRed(int i);

	inline int getGreen() const;
	inline void setGreen(int i);

	inline int getBlue() const;
	inline void setBlue(int i);

	inline int getRedBG() const;
	inline void setRedBG(int i);

	inline int getGreenBG() const;
	inline void setGreenBG(int i);

	inline int getBlueBG() const;
	inline void setBlueBG(int i);

	inline float getXOffset() const;
	inline void setXOffset(float f);

	inline float getYOffset() const;
	inline void setYOffset(float f);

	inline float getDegPerVolt() const;
	inline void setDegPerVolt(float f);

	inline float getScreenXOffset() const;
	inline void setScreenXOffset(float f);

	inline float getScreenYOffset() const;
	inline void setScreenYOffset(float f);

	inline float getPixelSize() const;
	inline void setPixelSize(float f);

	inline int getNSamples() const;
	inline void setNSamples(int i);
};

/****************************************************************************/

/**
 * @brief Object to manage a coordinate in degrees and pixels.
 *
 * The stim X and Y are the position with stim offset added but without the
 * screen offset added.  The screen X and Y are the position with both offsets
 * added.
 *
 * Using this class helps to ensure that all math is done in degrees before a
 * single, final conversion to pixels.  Converting multiple degree values to
 * pixels and then performing arithmetic on them yields incorrect results,
 * introducing about 5 percent error.
 */
class CCoordinate {
public:
	CCoordinate();
	CCoordinate(double xx, double yy);
	CCoordinate(dbl_pair dPair);
	CCoordinate(const CCoordinate& other);
	~CCoordinate();

	/* Accessors */
	inline double xDeg() const;
	inline double yDeg() const;
	inline double xStimDeg() const;
	inline double yStimDeg() const;
	inline double xScreenDeg() const;
	inline double yScreenDeg() const;
	inline double xScreenPix() const;
	inline double yScreenPix() const;

	/* Math operations */
	CCoordinate& operator=(const CCoordinate& other);
	CCoordinate& operator+=(const CCoordinate& other);
	CCoordinate& operator-=(const CCoordinate& other);

private:
	void set(double xx, double yy);

	double m_xDeg,       m_yDeg;
	double m_xStimDeg,   m_yStimDeg;
	double m_xScreenDeg, m_yScreenDeg;
	double m_xScreenPix, m_yScreenPix;
};

/*****************************************************************************
 * Functions
 *****************************************************************************/

/* Operators for CCoordinate */
const CCoordinate operator+(const CCoordinate& left, const CCoordinate& right);
const CCoordinate operator-(const CCoordinate& left, const CCoordinate& right);

/*****************************************************************************
 * Inline member function definitions
 *****************************************************************************/

int CSettings::getViewDistance() const { return iScreenPosition - iEyePosition; }

int CSettings::getEyePosition() const { return iEyePosition; }
void CSettings::setEyePosition(int i) { iEyePosition = i; }

int CSettings::getScreenPosition() const { return iScreenPosition; }
void CSettings::setScreenPosition(int i) { iScreenPosition = i; }

int CSettings::getRed() const { return iRed; }
void CSettings::setRed(int i) { iRed = i; }

int CSettings::getGreen() const { return iGreen; }
void CSettings::setGreen(int i) { iGreen = i; }

int CSettings::getBlue() const { return iBlue; }
void CSettings::setBlue(int i) { iBlue = i; }

int CSettings::getRedBG() const { return iRedBG; }
void CSettings::setRedBG(int i) { iRedBG = i; }

int CSettings::getGreenBG() const { return iGreenBG; }
void CSettings::setGreenBG(int i) { iGreenBG = i; }

int CSettings::getBlueBG() const { return iBlueBG; }
void CSettings::setBlueBG(int i) { iBlueBG = i; }

float CSettings::getXOffset() const { return fXOffset; }
void CSettings::setXOffset(float f) {fXOffset = f; }

float CSettings::getYOffset() const { return fYOffset; }
void CSettings::setYOffset(float f) {fYOffset = f; }

float CSettings::getDegPerVolt() const { return fDegPerVolt; }
void CSettings::setDegPerVolt(float f) {fDegPerVolt = f; }

float CSettings::getScreenXOffset() const { return fScreenXOffset; }
void CSettings::setScreenXOffset(float f) {fScreenXOffset = f; }

float CSettings::getScreenYOffset() const { return fScreenYOffset; }
void CSettings::setScreenYOffset(float f) {fScreenYOffset = f; }

float CSettings::getPixelSize() const { return fPixelSize; }
void CSettings::setPixelSize(float f) {fPixelSize = f; }

int CSettings::getNSamples() const { return iNSamplesEyePos; }
void CSettings::setNSamples(int i) { iNSamplesEyePos = i; }

/****************************************************************************/

double CCoordinate::xDeg() const { return m_xDeg; }
double CCoordinate::yDeg() const { return m_yDeg; }
double CCoordinate::xStimDeg() const { return m_xStimDeg; }
double CCoordinate::yStimDeg() const { return m_yStimDeg; }
double CCoordinate::xScreenDeg() const { return m_xScreenDeg; }
double CCoordinate::yScreenDeg() const { return m_yScreenDeg; }
double CCoordinate::xScreenPix() const { return m_xScreenPix; }
double CCoordinate::yScreenPix() const { return m_yScreenPix; }

#endif
