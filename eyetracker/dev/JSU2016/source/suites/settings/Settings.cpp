/**
 * @file Settings.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <time.h>
#include <math.h>
#include "VSGV8.h"
#include "VSGV8AD.h"
#include "Settings.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"
#include "utils/Math.h"

/*****************************************************************************
 * Extern
 *****************************************************************************/

/* Re-defined so we need not include AppCore.h. */
extern CSettings* g_pSettings;

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CSettings::CSettings()
{
	theSerialComm.init(false, CBR_115200, 1);//global theComm. only init once, here
	defaultFile = CString("C:\\Stimulus Settings\\Settings\\default.txt\0");

	iEyePosition = 370;
	iScreenPosition = 1256;
	iRed = 255;
	iGreen = 0;
	iBlue = 0;
	iRedBG = 0;
	iGreenBG = 0;
	iBlueBG = 0;
	fXOffset = 0;
	fYOffset = 0;
	fDegPerVolt = 10;
	fScreenXOffset = 0;
	fScreenYOffset = 0;
	fPixelSize = 141.0;
	iNSamplesEyePos = 5;
	LoadFile(true);
	sendSettings(theSerialComm);//send settings here because they can't be sent on stim construction (no stim)
}

CSettings::~CSettings()
{
	theSerialComm.stop();//global theComm. only stop once, here
}

void CSettings::LoadFile(bool bDefault)
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(true, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	FILE* fp = fopen(filename, "rt");
	if (!fp) {
		AfxMessageBox("Error loading file - file or folder not found");
		return;
	}
			
	fscanf(fp, "%d\n", &iEyePosition);
	fscanf(fp, "%d\n", &iScreenPosition);
	fscanf(fp, "%d\n", &iRed);
	fscanf(fp, "%d\n", &iGreen);
	fscanf(fp, "%d\n", &iBlue);
	fscanf(fp, "%d\n", &iRedBG);
	fscanf(fp, "%d\n", &iGreenBG);
	fscanf(fp, "%d\n", &iBlueBG);
	fscanf(fp, "%f\n", &fXOffset);
	fscanf(fp, "%f\n", &fYOffset);
	fscanf(fp, "%f\n", &fDegPerVolt);
	fscanf(fp, "%f\n", &fScreenXOffset);
	fscanf(fp, "%f\n", &fScreenYOffset);
	fscanf(fp, "%f\n", &fPixelSize);
	fscanf(fp, "%d\n", &iNSamplesEyePos);

	fclose(fp);
}

void CSettings::SaveFile(bool bDefault)
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(false, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	FILE* fp = fopen(filename, "wt");
	if (!fp) {
		AfxMessageBox("Error saving file - file or folder not found");
		return;
	}
	
	fprintf(fp, "%d\n", iEyePosition);
	fprintf(fp, "%d\n", iScreenPosition);
	fprintf(fp, "%d\n", iRed);
	fprintf(fp, "%d\n", iGreen);
	fprintf(fp, "%d\n", iBlue);
	fprintf(fp, "%d\n", iRedBG);
	fprintf(fp, "%d\n", iGreenBG);
	fprintf(fp, "%d\n", iBlueBG);
	fprintf(fp, "%f\n", fXOffset);
	fprintf(fp, "%f\n", fYOffset);
	fprintf(fp, "%f\n", fDegPerVolt);
	fprintf(fp, "%f\n", fScreenXOffset);
	fprintf(fp, "%f\n", fScreenYOffset);
	fprintf(fp, "%f\n", fPixelSize);
	fprintf(fp, "%d\n", iNSamplesEyePos);

	fclose(fp);
}

/**
 * @brief Convert quantity from one unit to another using output variable.
 *
 * Uses tangent screen approximation w/o correction.
 *
 * @param[in] unit1 Units to convert from.
 * @param[in] val1 Quantity to convert.
 * @param[in] unit2 Units to convert to.
 * @param[out] val2 The converted value.
 */
void CSettings::tanScreenUnit2Unit(int unit1, double val1, int unit2, double *val2) const
{
	vsgUnit2Unit(unit1, val1, unit2, val2);

	/* This block will be removed by the compiler's dead code detection logic,
	 * since its condition evaluates to a constant false.  It's left in 
	 *     1. Because it will be lexed and parsed, helping to prevent bit rot;
	 *     2. To demonstrate the correction for the tangent screen; and
	 *     3. To remind future programmers explicity that we do *not* make
	 *        that correction, but could choose to do so in the future.
	 */
	if (false) {
		if (unit1 == unit2) {
			(*val2) = val1;
		}
		else {
			/* Use tan(a) = opposite(a)/adjacent(a).  Adjacent is the
			 * viewing distance. */;
			const double viewdistmm = vsgGetViewDistMM();
			double mmPerPixel = fPixelSize / 100.0;
			
			if (unit1 == vsgDEGREEUNIT && unit2 == vsgPIXELUNIT) {
				(*val2) = tan(val1 * PI_DOUBLE / 180.0) * viewdistmm / mmPerPixel;
			}
			else if (unit1 == vsgPIXELUNIT && unit2 == vsgDEGREEUNIT) {
				(*val2) = atan(val1 / (viewdistmm / mmPerPixel)) * 180.0 / PI_DOUBLE;
			}
			/* Ignore other conversion units */
		}
	} 
}

/**
 * @brief Convert quantity from one unit to another, returning converted val.
 *
 * Uses tangent screen approximation w/o correction.
 *
 * @param[in] unit1 Units to convert from.
 * @param[in] val1 Quantity to convert.
 * @param[in] unit2 Units to convert to.
 * @return The converted value.
 */
double CSettings::tanScreenUnit2Unit(int unit1, double val1, int unit2) const
{
	double retVal;
	this->tanScreenUnit2Unit(unit1, val1, unit2, &retVal);
	return retVal;
}

void CSettings::sendSettings(CSerialComm& theComm)
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "Settings;EyePos;%d;%ld|0x%02x/\0", iEyePosition, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;ScreenPos;%d;%ld|0x%02x/\0", iScreenPosition, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;ViewDist;%d;%ld|0x%02x/\0", iScreenPosition-iEyePosition, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;StimXOffset;%.4f;%ld|0x%02x/\0", fXOffset, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;StimYOffset;%.4f;%ld|0x%02x/\0", fYOffset, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;ScreenXOffset;%.4f;%ld|0x%02x/\0", fScreenXOffset, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;ScreenYOffset;%.4f;%ld|0x%02x/\0", fScreenYOffset, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;DotColorR;%d;%ld|0x%02x/\0", iRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;DotColorG;%d;%ld|0x%02x/\0", iGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;DotColorB;%d;%ld|0x%02x/\0", iBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;BGColorR;%d;%ld|0x%02x/\0", iRedBG, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;BGColorG;%d;%ld|0x%02x/\0", iGreenBG, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;BGColorB;%d;%ld|0x%02x/\0", iBlueBG, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;NSamples;%d;%ld|0x%02x/\0", iNSamplesEyePos, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}

/*****************************************************************************/

/**
 * @brief Create (0, 0) coordinate, useful for default initialization.
 */
CSmartCoord::CSmartCoord()
{
	set(0.0, 0.0);
}

/**
 * @brief Create coordinate given X and Y values in degrees.
 * @param xx X coordinate in degrees.
 * @param yy Y coordinate in degrees.
 */
CSmartCoord::CSmartCoord(double xx, double yy)
{
	set(xx, yy);
}

/**
 * @brief Create coordinate given pair of doubles, X and Y values in degrees.
 *
 * Constructor is intended to enable conversion from std::pair<double, double>
 * and thus is not declared "explicit".
 *
 * @param dPair dbl_pair representing X and Y coordinate in degrees.
 */
CSmartCoord::CSmartCoord(dbl_pair dPair)
{
	set(dPair.first, dPair.second);
}

/**
 * @brief Create coordinate by copying given coordinate.
 * @param other A coordinate whose X and Y values are copied.
 */
CSmartCoord::CSmartCoord(const CSmartCoord& other)
{
	set(other.m_xDeg, other.m_yDeg);
}

/** @brief Destructor. */
CSmartCoord::~CSmartCoord() { /* Empty. */ }

/**
 * @brief Set state to given values, and calculate and set pixel values.
 * @param xx X coordinate in degrees.
 * @param yy Y coordinate in degrees.
 */
void CSmartCoord::set(double xx, double yy)
{
	m_xDeg = xx;
	m_yDeg = yy;
	m_xStimDeg = xx + g_pSettings->getXOffset();
	m_yStimDeg = yy + g_pSettings->getYOffset();
	m_xScreenDeg = m_xStimDeg + g_pSettings->getScreenXOffset();
	m_yScreenDeg = m_yStimDeg + g_pSettings->getScreenYOffset();

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,  m_xScreenDeg, 
		vsgPIXELUNIT, &m_xScreenPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -m_yScreenDeg, 
		vsgPIXELUNIT, &m_yScreenPix);
}

/**
 * @brief Copy state of other coordinate into this one.
 *
 * Perform memberwise copy.
 *
 * @param other A coordinate whose X and Y values are copied.
 * @return Mutable reference to self, to allow (a = b) = c constructions.
 */
CSmartCoord& CSmartCoord::operator=(const CSmartCoord& other)
{
	if (this != &other) {
		set(other.m_xDeg, other.m_yDeg);
	}
	return (*this);
}

/**
 * @brief Add coordinate to this one and store result as this coordinate.
 *
 * Does NOT perform memberwise addition; that would multiply add the
 * offsets, which is incorrect.  Instead, only add the other coordinate's
 * unmodified X and Y values.  This allows vector arithmetic operations on 
 * coordinates while preserving the offsets.
 *
 * @param other A coordinate whose unmodified X and Y values are added to this
 *        coordinate.
 * @return Mutable reference to self, to allow (a += b) += c constructions.
 */
CSmartCoord& CSmartCoord::operator+=(const CSmartCoord& other)
{
	set(m_xDeg + other.m_xDeg, m_yDeg + other.m_yDeg);
	return (*this);
}

/**
 * @brief Subtract coordinate from this one and store result as this 
 * coordinate.
 *
 * Does NOT perform memberwise subtraction; that would multiply subtract the
 * offsets, which is incorrect.  Instead, only subtract the other coordinate's
 * unmodified X and Y values.  This allows vector arithmetic operations on 
 * coordinates while preserving the offsets.
 *
 * @param other A coordinate whose unmodified X and Y values are subtracted from 
 *        this coordinate.
 * @return Mutable reference to self, to allow (a -= b) += c constructions.
 */
CSmartCoord& CSmartCoord::operator-=(const CSmartCoord& other)
{
	set(m_xDeg - other.m_xDeg, m_yDeg - other.m_yDeg);
	return (*this);
}

/**
 * @brief Add two coordinates and return result by value.
 *
 * Not a friend of the CSmartCoord class because it uses the public += 
 * operator, thereby inheriting its behavior.
 *
 * @param left Coordinate from lhs.
 * @param right Coordinate from rhs.
 * @return New coordinate by constant value to prevent (a + b) += c 
 * constructions.
 */
const CSmartCoord operator+(const CSmartCoord& left, const CSmartCoord& right)
{
	CSmartCoord copy(left);
	copy += right;
	return copy;
}

/**
 * @brief Subtract two coordinates and return result by value.
 *
 * Not a friend of the CSmartCoord class because it uses the public -= 
 * operator, thereby inheriting its behavior.
 *
 * @param left Coordinate from lhs.
 * @param right Coordinate from rhs.
 * @return New coordinate by constant value to prevent (a - b) -= c 
 * constructions.
 */
const CSmartCoord operator-(const CSmartCoord& left, const CSmartCoord& right)
{
	CSmartCoord copy(left);
	copy -= right;
	return copy;
}