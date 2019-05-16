/**
 * @file Calibration.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "ReverseFixation.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

#define MODE_FOVEA (0)
#define MODE_INIT (1)
#define MODE_TEST (2)


/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CReverseFixation::CReverseFixation()
:	m_primaryRed(255), m_primaryGreen(0), m_primaryBlue(0),
	m_secondaryRed(0), m_secondaryGreen(0), m_secondaryBlue(255),
	bInitLeft(false),
	fInitLeftX(-20.0), fInitLeftY(0), fInitRightX(20.0), fInitRightY(0),
	fDotSize(3.0), fDisplayTime(500),
	m_mode(MODE_TEST),
	fWindowAllowancePix(10.0),
	bUseWait(false)//NotImplemented
{
	
	defaultFile = CString("C:\\Stimulus Settings\\ReverseFixation\\default.txt");
	LoadFile(true);
}

CReverseFixation::~CReverseFixation() {}

void CReverseFixation::LoadFile(bool bDefault) 
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
	
	fscanf(fp, "%d\n", &bInitLeft);
	fscanf(fp, "%f\n", &fInitLeftX);
	fscanf(fp, "%f\n", &fInitLeftY);
	fscanf(fp, "%f\n", &fInitRightX);
	fscanf(fp, "%f\n", &fInitRightY);
	fscanf(fp, "%f\n", &fDotSize);
	fscanf(fp, "%f\n", &fDisplayTime);
	fscanf(fp, "%d\n", &m_mode);
	fscanf(fp, "%f\n", &fWindowAllowancePix);
	fscanf(fp, "%d\n", &bUseWait);

	fclose(fp);
}

void CReverseFixation::SaveFile(bool bDefault) 
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
	
	fprintf(fp, "%d\n", bInitLeft);
	fprintf(fp, "%f\n", fInitLeftX);
	fprintf(fp, "%f\n", fInitLeftY);
	fprintf(fp, "%f\n", fInitRightX);
	fprintf(fp, "%f\n", fInitRightY);
	fprintf(fp, "%f\n", fDotSize);
	fprintf(fp, "%f\n", fDisplayTime);
	fprintf(fp, "%d\n", m_mode);
	fprintf(fp, "%f\n", fWindowAllowancePix);
	fprintf(fp, "%d\n", bUseWait);

	fclose(fp);
}

void CReverseFixation::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "ReverseFixation;mode;%d;%ld|0x%x/\0", m_mode, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}
