/**
 * @file ETUtils.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>		/* CString */
#include <afxwin.h>		/* AfxMessageBox */
#include <afxdlgs.h>	/* CFileDialog */ 

#ifndef _NO_ANALOG_IO
#include <nidaq.h>		/* AO_ functions, i16 */
#include <nidaqex.h>	/* NIDAQErrorHandler */
#endif

#include <cstdlib>		/* rand, srand */
#include <ctime>		/* time */
#include "vsgext/vsgext.h"
#include "utils/Math.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Functions
 *****************************************************************************/

namespace etu {

/**
 * @brief Round the input value to nearest integer, up or down.
 * Decimals round to the next integer of greater magnitude at magnitude 0.5. 
 * Examples:
 * <ul>
 * <li><b>1.2</b> rounds to 1 </li>
 * <li><b>1.5</b> rounds to 2 </li>
 * <li><b>1.7</b> rounds to 2 </li>
 * <li><b>-1.2</b> rounds to -1 </li>
 * <li><b>-1.5</b> rounds to -2 </li>
 * <li><b>-1.7</b> rounds to -2 </li>
 * </ul>
 * @param dd Double to be rounded
 * @return Rounded (not merely truncated) integer.
 */
int Round(double dd) 
{
	double trunc = (int)dd;
	double dec = dd - trunc;
	if (dd < 0) {
		/* dec < 0 too */
		return (int)trunc + ((dec > -0.5)? 0 : -1);
	} 
	else {
		return (int)trunc + ((dec >= 0.5)? 1 : 0);
	}
}

/**
 * @brief Convert string to integer w/validity check.
 * If buffer is not a valid integer string, oldValue is returned.
 * @param buffer String representing an integer.
 * @param oldValue Value to return if buffer is not a valid int.
 * @return Buffer string as an int, or oldValue.
 */
int StringToInt(char* buffer, int oldValue) 
{
	char* endPtr;
	int ii = (int)(strtol(buffer, &endPtr, 10));
	if (strlen(endPtr) > 0) {
		AfxMessageBox("Invalid integer value");
		ii = oldValue;
	}
	return ii;
}

/**
 * @brief Convert integer value to a string
 * For some int ix, the following holds:
 *     ix == StringToInt(IntToString(ix))
 * @param ii Integer.
 * @return String representation.
 */
CString IntToString(int ii) 
{
	char buffer[100];
	sprintf(buffer, "%d\0", ii);
	return CString(buffer);
}

/**
 * @brief Convert string to float w/validity check.
 * If buffer is not a valid float string, oldValue is returned.
 * @param buffer String representing a float.
 * @param oldValue Value to return if buffer is not a valid float.
 * @return Buffer string as a double, or oldValue.
 */
double StringToFloat(char* buffer, double oldValue) 
{
	char * endPtr;
	double ff = (double)strtod(buffer, &endPtr);
	if (strlen(endPtr) > 0) {
		AfxMessageBox("Invalid floating point value");
		ff = oldValue;
	}
	return ff;
}

/**
 * @brief Convert angle from degrees to radians.
 * There are 2pi radians in 360 degrees.
 * @param degrees Angle in degrees; positive and negative values work.
 * @return Angle converted to radians.
 */
double DegToRad(double degrees) {
	return (double)(degrees / 180.0f * PI_FLOAT);
}

/**
 * @brief Convert float value to a string
 * For some float fx, the following holds:
 *     fx == StringToInt(IntToString(fx))
 * @param ff Float.
 * @return String representation.
 */
CString FloatToString(double ff) 
{
	char buffer[100];
	sprintf(buffer, "%f\0", ff);
	return CString(buffer);
}

/** 
 * @brief Convert an RGB triple into a VSG trival struct.
 * @param rr Red value, range [0,255].
 * @param gg Green value, range [0,255].
 * @param bb Blue value, range [0,255].
 * @return Instance of a VSG trival for assignment to a LHS.
 */
VSGTRIVAL RgbToTrival(int rr, int gg, int bb)
{
	VSGTRIVAL output;
	output.a = rr / 255.0;
	output.b = gg / 255.0;
	output.c = bb / 255.0;
	return output;
}

/** 
 * @brief Convert an MFC COLORREF type into a VSG trival struct.
 * @param cc The COLORREF instance.
 * @return Instance of a VSG trival for assignment to a LHS.
 */
VSGTRIVAL ColorrefToTrival(COLORREF cc)
{
	return RgbToTrival(GetRValue(cc), GetGValue(cc), GetBValue(cc));
}

/** 
 * @brief Convert an RGB triple into a VSG trival struct.
 * @param vt Reference to the trival to convert.
 * @param[out] rr Returned red value, range [0,255].
 * @param[out] gg Returned green value, range [0,255].
 * @param[out] bb Returned Returned blue value, range [0,255].
 */
void TrivalToRGB(VSGTRIVAL& vt, int& rr, int& gg, int& bb)
{
	rr = (Round(vt.a * 255.0) & 0xff);
	gg = (Round(vt.b * 255.0) & 0xff);
	bb = (Round(vt.c * 255.0) & 0xff);
}

/**
 * @brief Opens file browser window for given file.
 * @param bOpen True if browsing for a file to open and thus which must 
 *     already exist.  False to create file.
 * @param extension Extension of files to browse for or create.
 * @param fileType
 * @param oldFileName
 * @param bUseOldFile
 * @return
 */
CString BrowseForFile(bool bOpen, CString extension, CString fileType, 
	CString oldFileName, bool bUseOldFile) 
{
	CFileDialog* fileOpen;
	DWORD dwFlags;
	if (bOpen) {
		dwFlags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	} 
	else {
		dwFlags = OFN_OVERWRITEPROMPT;
	}
	if (oldFileName.GetLength() > 0) {
		fileOpen = new CFileDialog(bOpen, extension, oldFileName, dwFlags, 
			fileType, AfxGetMainWnd(), sizeof(OPENFILENAME));
	} 
	else {
		fileOpen = new CFileDialog(bOpen, extension, NULL, dwFlags, 
			fileType, AfxGetMainWnd(), sizeof(OPENFILENAME));
	}
	CString fileName = CString("");
	if (fileOpen->DoModal() == IDOK) {
		fileName = fileOpen->GetPathName();
	} 
	else {
		if (bUseOldFile) fileName = oldFileName;
	}

	delete fileOpen;

	return fileName;
}

/**
 * @brief Set voltage on analog output line.
 * @param iChannel Which line to set.
 * @param fVoltage Voltage to maintain on the line.
 * @param bUpdate Whether to record the success or failure; not used.
 */
void AnalogOut(int iChannel, float fVoltage, bool bUpdate) 
{
#ifndef _NO_ANALOG_IO
	i16 iStatus = 0;
	i16 iRetVal = 0;
	i16 iIgnoreWarning = 0;

	iStatus = AO_VWrite(1, iChannel, fVoltage);
	if (bUpdate)
		iStatus = AO_Update(1);
	iRetVal = NIDAQErrorHandler(iStatus, "AO_VWrite", iIgnoreWarning);
#endif
}

/**
 * @brief Generate a random int in the range [min,max].
 * @param min Inclusive minimum value.
 * @param max Inclusive maximum value.
 * @return Random integer in the closed (inclusive) range [min,max].
 */
int RandChoice(int min, int max)
{
	int randIx, range, adjRandMax;

	if (min == max) return min;
	if (max < min) {
		int temp = max;
		max = min;
		min = temp;
	}

	/* Output must be inclusive of min and max. */
	range = max - min + 1;
	/* Range arithmetic ensures a uniform distribution. Otherwise, the lower
	 * values from (min + 0) to (min + (RAND_MAX % range)) are 
	 * overrepresented. */
	adjRandMax = RAND_MAX - (RAND_MAX % range) - 1;
	while ((randIx = rand()) > adjRandMax) ;
	return ((randIx % range) + min);
}

/**
 * @brief Generate a random choice with prob of being true.
 * @param prob Probability of true.
 * @return True with probability prob; or false with probability (1 - prob).
 */
bool RandCoinFlip(double prob)
{
	int partition;

	if (prob >= 1.0) return true;
	if (prob <= 0.0) return false;

	partition = Round(prob * RAND_MAX);
	return (rand() < partition);
}

}; // namespace etu
