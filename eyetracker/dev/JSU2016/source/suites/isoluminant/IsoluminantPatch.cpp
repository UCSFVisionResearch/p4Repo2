/**
 * @file IsoluminantPatch.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include "IsoluminantPatch.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CIsoluminantPatch::CIsoluminantPatch() 
{
	defaultFile = CString("C:\\Stimulus Settings\\IsoluminantPatch\\default.txt");
	cstrFileName = "";
	iNumCycles = 10;
	bLoop = true;
	fVoltageRange = 9.95f;
	fVoltageOffset = 0;
	circleRadius = 100;
	circleXPosition = 0;
	circleYPosition = 0;
	bSearching = false;
	bHighestPriority = true;
	bUseLeft = true;
	fixationSize = 1;
	fixationX = 0;
	fixationY = 0;
	fixationX2 = 0;
	fixationY2 = 5;
	meanLuminance = 68;
	iNumRepeats = 8;
	bFNoise = true;
	bRepeatsOnly = false;
	minLuminance = 0;
	maxLuminance = 255;

	LoadFile(true);
}

CIsoluminantPatch::~CIsoluminantPatch() {}

void CIsoluminantPatch::LoadFile(bool bDefault) 
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

	char buffer2[1024];
	
	fscanf(fp, "%[^\n]\n", &buffer2);
	cstrFileName = CString(buffer2);
	
	fscanf(fp, "%d\n", &iNumCycles);
	fscanf(fp, "%d\n", &bLoop);
	fscanf(fp, "%f\n", &fVoltageRange);
	fscanf(fp, "%f\n", &fVoltageOffset);
	fscanf(fp, "%d\n", &circleRadius);
	fscanf(fp, "%d\n", &circleXPosition);
	fscanf(fp, "%d\n", &circleYPosition);
	fscanf(fp, "%d\n", &bSearching);
	fscanf(fp, "%d\n", &bHighestPriority);
	fscanf(fp, "%d\n", &bUseLeft);
	fscanf(fp, "%f\n", &fixationSize);
	fscanf(fp, "%f\n", &fixationX);
	fscanf(fp, "%f\n", &fixationY);
	fscanf(fp, "%d\n", &meanLuminance);
	fscanf(fp, "%d\n", &iNumRepeats);
	fscanf(fp, "%d\n", &bFNoise);
	fscanf(fp, "%f\n", &fixationX2);
	fscanf(fp, "%f\n", &fixationY2);
	fscanf(fp, "%d\n", &bRepeatsOnly);
	fscanf(fp, "%d\n", &minLuminance);
	fscanf(fp, "%d\n", &maxLuminance);

	fclose(fp);
}

void CIsoluminantPatch::SaveFile(bool bDefault) 
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
	
	fprintf(fp, "%s \n", cstrFileName);
	fprintf(fp, "%d\n", iNumCycles);
	fprintf(fp, "%d\n", bLoop);
	fprintf(fp, "%f\n", fVoltageRange);
	fprintf(fp, "%f\n", fVoltageOffset);
	fprintf(fp, "%d\n", circleRadius);
	fprintf(fp, "%d\n", circleXPosition);
	fprintf(fp, "%d\n", circleYPosition);
	fprintf(fp, "%d\n", bSearching);
	fprintf(fp, "%d\n", bHighestPriority);
	fprintf(fp, "%d\n", bUseLeft);
	fprintf(fp, "%f\n", fixationSize);
	fprintf(fp, "%f\n", fixationX);
	fprintf(fp, "%f\n", fixationY);
	fprintf(fp, "%d\n", meanLuminance);
	fprintf(fp, "%d\n", iNumRepeats);
	fprintf(fp, "%d\n", bFNoise);
	fprintf(fp, "%f\n", fixationX2);
	fprintf(fp, "%f\n", fixationY2);
	fprintf(fp, "%d\n", bRepeatsOnly);
	fprintf(fp, "%d\n", minLuminance);
	fprintf(fp, "%d\n", maxLuminance);

	fclose(fp);
}