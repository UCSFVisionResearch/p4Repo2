/**
 * @file Fullfield.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include "Fullfield.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CFullfield::CFullfield() 
{
	defaultFile = CString("C:\\Stimulus Settings\\Fullfield\\default.txt");
	fBarWidth = 10;
	fDriftVelocity = 40;
	iDriftDirection = DIR_LT;
	bUseFixation = false;
	fixationRed = 255;
	fixationBlue = 0;
	fixationGreen = 0;
	fixationSize = 0.5;
	fixationXPos = 0;
	fixationYPos = 0;
	fSpatialFreq = 0.1f;
	fDuration = 30000;
	dContrast = 20;
	bNoise = false;
	bUseGlobalColors = false;
}

CFullfield::~CFullfield() {}

void CFullfield::LoadFile(bool bDefault) 
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
			
	fscanf(fp, "%f\n", &fBarWidth);
	fscanf(fp, "%f\n", &fDriftVelocity);
	fscanf(fp, "%d\n", &iDriftDirection);
	fscanf(fp, "%d\n", &bUseFixation);
	fscanf(fp, "%d\n", &fixationRed);
	fscanf(fp, "%d\n", &fixationBlue);
	fscanf(fp, "%d\n", &fixationGreen);
	fscanf(fp, "%f\n", &fixationSize);
	fscanf(fp, "%f\n", &fixationXPos);
	fscanf(fp, "%f\n", &fixationYPos);
	fscanf(fp, "%f\n", &fSpatialFreq);
	fscanf(fp, "%f\n", &fDuration);
	fscanf(fp, "%f\n", &dContrast);
	fscanf(fp, "%d\n", &bNoise);
	fscanf(fp, "%d\n", &bUseGlobalColors);

	fclose(fp);
}

void CFullfield::SaveFile(bool bDefault) 
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
	
	fprintf(fp, "%f\n", fBarWidth);
	fprintf(fp, "%f\n", fDriftVelocity);
	fprintf(fp, "%d\n", iDriftDirection);
	fprintf(fp, "%d\n", bUseFixation);
	fprintf(fp, "%d\n", fixationRed);
	fprintf(fp, "%d\n", fixationBlue);
	fprintf(fp, "%d\n", fixationGreen);
	fprintf(fp, "%f\n", fixationSize);
	fprintf(fp, "%f\n", fixationXPos);
	fprintf(fp, "%f\n", fixationYPos);
	fprintf(fp, "%f\n", fSpatialFreq);
	fprintf(fp, "%f\n", fDuration);
	fprintf(fp, "%f\n", dContrast);
	fprintf(fp, "%d\n", bNoise);
	fprintf(fp, "%d\n", bUseGlobalColors);

	fclose(fp);
}

void CFullfield::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);
	
	sprintf(buffer, "FullField;Noise;%d;%ld|0x%02x/\0", bNoise, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "FullField;GlobalColors;%d;%ld|0x%02x/\0", bUseGlobalColors, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "FullField;BarWidth;%.4f;%ld|0x%02x/\0", fBarWidth, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "FullField;DriftVelocity;%.4f;%ld|0x%02x/\0", fDriftVelocity, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "FullField;Duration;%.4f;%ld|0x%02x/\0", fDuration, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "FullField;Contrast;%f;%ld|0x%02x/\0", dContrast, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "FullField;DriftDirection;%d;%ld|0x%02x/\0", iDriftDirection, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}
