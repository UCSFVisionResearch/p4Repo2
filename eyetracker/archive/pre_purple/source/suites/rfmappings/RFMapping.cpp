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
{
	fXPosition = 0;
	fYPosition = 0;
	fWidth = 2;
	fHeight = 2;
	fBarWidth = 2;
	fBarHeight = 6;
	fOrientation = 0;
	fCircleRadius = 2;

	bUseMouse = true;
	bUsePulse = false;
	bUseCircle = false;
	bUseBar = true;

	LoadFile(true);
}

CRFMapping::~CRFMapping() {}

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
	fscanf(fp, "%d\n", &bUsePulse);
	fscanf(fp, "%d\n", &bUseCircle);
	fscanf(fp, "%d\n", &bUseBar);
	fscanf(fp, "%f\n", &fBarWidth);
	fscanf(fp, "%f\n", &fBarHeight);
	fscanf(fp, "%f\n", &fOrientation);
	fscanf(fp, "%f\n", &fCircleRadius);
	
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
	fclose(fp);
}
