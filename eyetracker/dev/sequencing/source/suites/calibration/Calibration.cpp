/**
 * @file Calibration.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include "Calibration.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CCalibration::CCalibration()
{
	defaultFile = CString("C:\\Stimulus Settings\\Calibration\\default.txt");
	m_hSeparation = 30.0f;
	m_vSeparation = 15.0f;
	fSize = 0.5f;
	iEye = EYE_LT;
	bShowAll = false;
	acceptDelay = 1000;
	bCycle = false;

	LoadFile(true);
}

CCalibration::~CCalibration() {}

void CCalibration::LoadFile(bool bDefault) 
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(true, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	char buffer[1024];
	sprintf(buffer, "%s", filename);
	FILE* fp = fopen(filename, "rt");
	if (!fp) {
		AfxMessageBox("Error loading file - file or folder not found");
		return;
	}
	
	fscanf(fp, "%f\n", &m_hSeparation);
	fscanf(fp, "%f\n", &fSize);
	fscanf(fp, "%d\n", &iEye);
	fscanf(fp, "%d\n", &bShowAll);
	fscanf(fp, "%d\n", &acceptDelay);
	fscanf(fp, "%f\n", &m_vSeparation);

	fclose(fp);
}

void CCalibration::SaveFile(bool bDefault) 
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(false, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	char buffer[1024];
	sprintf(buffer, "%s", filename);
	FILE* fp = fopen(filename, "wt");
	if (!fp) {
		AfxMessageBox("Error saving file - file or folder not found");
		return;
	}
	
	fprintf(fp, "%f\n", m_hSeparation);
	fprintf(fp, "%f\n", fSize);
	fprintf(fp, "%d\n", iEye);
	fprintf(fp, "%d\n", bShowAll);
	fprintf(fp, "%d\n", acceptDelay);
	fprintf(fp, "%f\n", m_vSeparation);

	fclose(fp);
}

void CCalibration::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "Calibration;DotSize;%.4f;%ld|0x%02x/\0", fSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}
