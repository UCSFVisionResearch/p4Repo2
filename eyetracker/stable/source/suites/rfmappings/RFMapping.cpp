/**
 * @file RFMapping.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "RFMapping.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CRFMapping::CRFMapping()
:   m_rfmCrossX(0.0),
    m_rfmCrossY(0.0),
	m_rfmCrossDiameter(0.0),
	m_rfmCrossThickness(0.0),
	bUseCircle(false),
	bUseBar(true),
	fCircleRadius(2.0),
	fBarWidth(2.0),
	fBarHeight(6.0),
	fOrientation(0.0),
	bUseMouse(true),
	bUseGrid(false),
	bUsePulse(false),
	m_stepSizeDeg(5),
	m_stepsX(6),
	m_stepsY(3),
	m_displayTimeMs(500),
	m_useBlanks(true),
	m_blankTimeMs(200),
	fXPosition(0.0),
	fYPosition(0.0),
	fWidth(2.0),
	fHeight(2.0)
{
	LoadFile(true);
}

CRFMapping::~CRFMapping() { /* Nothing to do. */ }

void CRFMapping::LoadFile(bool bDefault) 
{
	CString defaultFile = CString("C:\\Stimulus Settings\\RFMapping\\default.txt");
	if (!bDefault) {
		defaultFile = etu::BrowseForFile(true, ".txt", "Text Files (*.txt)|*.txt", defaultFile, false);
	}

	char buffer[1024];
	sprintf(buffer, "%s", defaultFile);
	FILE* fp = fopen(buffer, "rt");
	if (!fp) {
		AfxMessageBox("Error loading file - file or folder not found");
		return;
	}

	fscanf(fp, "%f\n", &fXPosition);
	fscanf(fp, "%f\n", &fYPosition);
	fscanf(fp, "%f\n", &fWidth);
	fscanf(fp, "%f\n", &fHeight);
	fscanf(fp, "%d\n", &bUseMouse);
	bUseGrid = !bUseMouse;
	fscanf(fp, "%d\n", &bUsePulse);
	fscanf(fp, "%d\n", &bUseCircle);
	fscanf(fp, "%d\n", &bUseBar);
	fscanf(fp, "%f\n", &fBarWidth);
	fscanf(fp, "%f\n", &fBarHeight);
	fscanf(fp, "%f\n", &fOrientation);
	fscanf(fp, "%f\n", &fCircleRadius);
	fscanf(fp, "%f\n", &m_rfmCrossX);
	fscanf(fp, "%f\n", &m_rfmCrossY);
	fscanf(fp, "%f\n", &m_rfmCrossDiameter);
	fscanf(fp, "%f\n", &m_rfmCrossThickness);

	fscanf(fp, "%f\n", &m_stepSizeDeg);
	fscanf(fp, "%d\n", &m_stepsX);
	fscanf(fp, "%d\n", &m_stepsY);
	fscanf(fp, "%d\n", &m_displayTimeMs);
	fscanf(fp, "%d\n", &m_useBlanks);
	fscanf(fp, "%d\n", &m_blankTimeMs);
	
	fclose(fp);
}

void CRFMapping::SaveFile(bool bDefault) 
{
	CString defaultFile = CString("C:\\Stimulus Settings\\RFMapping\\default.txt");
	if (!bDefault) {
		defaultFile = etu::BrowseForFile(false, ".txt", "Text Files (*.txt)|*.txt", defaultFile, false);
	}

	char buffer[1024];
	sprintf(buffer, "%s", defaultFile);
	FILE* fp = fopen(buffer, "wt");
	if (!fp) {
		AfxMessageBox("Error saving file - file or folder not found");
		return;
	}
	
	fprintf(fp, "%f\n", fXPosition);
	fprintf(fp, "%f\n", fYPosition);
	fprintf(fp, "%f\n", fWidth);
	fprintf(fp, "%f\n", fHeight);
	fprintf(fp, "%d\n", bUseMouse);
	fprintf(fp, "%d\n", bUsePulse);
	fprintf(fp, "%d\n", bUseCircle);
	fprintf(fp, "%d\n", bUseBar);
	fprintf(fp, "%f\n", fBarWidth);
	fprintf(fp, "%f\n", fBarHeight);
	fprintf(fp, "%f\n", fOrientation);
	fprintf(fp, "%f\n", fCircleRadius);
	fprintf(fp, "%f\n", m_rfmCrossX);
	fprintf(fp, "%f\n", m_rfmCrossY);
	fprintf(fp, "%f\n", m_rfmCrossDiameter);
	fprintf(fp, "%f\n", m_rfmCrossThickness);

	fprintf(fp, "%f\n", m_stepSizeDeg);
	fprintf(fp, "%d\n", m_stepsX);
	fprintf(fp, "%d\n", m_stepsY);
	fprintf(fp, "%d\n", m_displayTimeMs);
	fprintf(fp, "%d\n", m_useBlanks);
	fprintf(fp, "%d\n", m_blankTimeMs);

	fclose(fp);
}

/**
 * @brief Sends settings over serial port.
 * @param theComm Communications object to use for settings.
 */
void CRFMapping::sendSettings(CSerialComm& theComm) 
{
	char buffer[256];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "RF Mapping Started;%ld|0x%x/\0", tm, code); theComm.transmit(buffer, (int)strlen(buffer));
}