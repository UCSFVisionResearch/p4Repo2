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
#include <nidaq.h>		/* AO_ functions, i16 */
#include <nidaqex.h>	/* NIDAQErrorHandler */
#include <cstdlib>		/* rand, srand */
#include <ctime>		/* time */
#include "utils/Math.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Functions
 *****************************************************************************/

namespace etu {

/**
 * @brief Round the input value to nearest integer, up or down.
 * Decimals round to the next integer at abs(0.5).
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
 * @return Buffer string as a float, or oldValue.
 */
float StringToFloat(char* buffer, float oldValue) 
{
	char * endPtr;
	float ff = (float)strtod(buffer, &endPtr);
	if (strlen(endPtr) > 0) {
		AfxMessageBox("Invalid floating point value");
		ff = oldValue;
	}
	return ff;
}

/**
 * @brief Convert float value to a string
 * For some float fx, the following holds:
 *     fx == StringToInt(IntToString(fx))
 * @param ff Float.
 * @return String representation.
 */
CString FloatToString(float ff) 
{
	char buffer[100];
	sprintf(buffer, "%f\0", ff);
	return CString(buffer);
}

/**
 * @brief Opens the given file.
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
 * @brief 
 * @param iChannel
 * @param fVoltage
 * @param bUpdate
 * @return
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
 * @brief Convert angle from degrees to radians.
 * There are 2pi radians in 360 degrees.
 * @param degrees Angle in degrees; positive and negative values work.
 * @return Angle converted to radians.
 */
float DegToRad(float degrees) {
	return (float)(degrees / 180.0f * PI_FLOAT);
}

/**
 * @brief Generate a random int in the range [min,max].
 * @param min Inclusive minimum value.
 * @param max Inclusive maximum value.
 * @return Random integer in the closed (inclusive) range [min,max].
 */
int ChooseRandom(int min, int max)
{
	/* Using 'max - min + 1' ensures we'll include the max value */
	return ((rand() % (max - min + 1)) + min);
}

/**
 * @brief Generate a random choice with prob of being true.
 * @param prob Probability of true.
 * @return True with probability prob; or false with probability (1 - prob).
 */
bool RandomChoice(double prob)
{
	int partition;

	if (prob >= 1.0) return true;
	if (prob <= 0.0) return false;

	partition = (int)((prob * RAND_MAX) + 1.0);
	return (rand() < partition);
}

}; // namespace etu
