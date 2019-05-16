/**
 * @file Suppression.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "Suppression.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CSuppression::CSuppression() 
{
	defaultFile = CString("C:\\Stimulus Settings\\Suppression\\default.txt");
	sprintf(defaultFileC, "%s", defaultFile);//why does it crash when using defaultFile directly??????? a mystery!
	iGridSpace = 8;
	iGridSize = 5;
	iDelay = 200;
	iRedValue = 255;
	iBlueValue = 255;
	iTest = 1;
	iBgLightRedValue=250;
	iBgLightGreenValue=0;
	iBgLightBlueValue=250;
	iBgDarkRedValue=100;
	iBgDarkGreenValue=0;
	iBgDarkBlueValue=100;
	iDelayms=100;
	bRed=false;
	bBlue=false;
	bText=false;
	fStripeHeight=15.0;
	bAdjustBG=false;
	iDotTime = 1000;
	iDisplayTime = 500;
	iOffTime = 1000;
	fSpatialFreq = 1.0;
	iSineColor=0;
	iNumGratings=12;
	bMultipleFullBlue = false;

	LoadFile(true);
}

CSuppression::~CSuppression() {}

void CSuppression::LoadFile(bool bDefault) 
{
	CString filename = CString(defaultFileC);//defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(true, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	char buffer[1024];
	sprintf(buffer, "%s", filename);
	FILE* fp = fopen(filename, "rt");//filename
	if (!fp) {
		AfxMessageBox("Error loading file - file or folder not found");
		return;
	}
	fscanf(fp, "%d\n", &iGridSpace);
	fscanf(fp, "%d\n", &iGridSize);
	fscanf(fp, "%d\n", &iDelay);
	fscanf(fp, "%d\n", &iRedValue);
	fscanf(fp, "%d\n", &iBlueValue);
	fscanf(fp, "%d\n", &iTest);
	fscanf(fp, "%d\n", &iBgLightRedValue);
	fscanf(fp, "%d\n", &iBgLightGreenValue);
	fscanf(fp, "%d\n", &iBgLightBlueValue);
	fscanf(fp, "%d\n", &iBgDarkRedValue);
	fscanf(fp, "%d\n", &iBgDarkGreenValue);
	fscanf(fp, "%d\n", &iBgDarkBlueValue);
	fscanf(fp, "%d\n", &iDelayms);
	fscanf(fp, "%d\n", &bRed);
	fscanf(fp, "%d\n", &bBlue);
	fscanf(fp, "%d\n", &bText);
	fscanf(fp, "%f\n", &fStripeHeight);
	fscanf(fp, "%d\n", &bAdjustBG);
	fscanf(fp, "%d\n", &iDotTime);
	fscanf(fp, "%d\n", &iDisplayTime);
	fscanf(fp, "%d\n", &iOffTime);
	fscanf(fp, "%f\n", &fSpatialFreq);
	fscanf(fp, "%d\n", &iSineColor);
	fscanf(fp, "%d\n", &iNumGratings);
	
	fclose(fp);
}

void CSuppression::SaveFile(bool bDefault) 
{
	CString filename = CString(defaultFileC);//defaultFile;
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
	
	fprintf(fp, "%d\n", iGridSpace);
	fprintf(fp, "%d\n", iGridSize);
	fprintf(fp, "%d\n", iDelay);
	fprintf(fp, "%d\n", iRedValue);
	fprintf(fp, "%d\n", iBlueValue);
	fprintf(fp, "%d\n", iTest);
	fprintf(fp, "%d\n", iBgLightRedValue);
	fprintf(fp, "%d\n", iBgLightGreenValue);
	fprintf(fp, "%d\n", iBgLightBlueValue);
	fprintf(fp, "%d\n", iBgDarkRedValue);
	fprintf(fp, "%d\n", iBgDarkGreenValue);
	fprintf(fp, "%d\n", iBgDarkBlueValue);
	fprintf(fp, "%d\n", iDelayms);
	fprintf(fp, "%d\n", bRed);
	fprintf(fp, "%d\n", bBlue);
	fprintf(fp, "%d\n", bText);
	fprintf(fp, "%f\n", fStripeHeight);
	fprintf(fp, "%d\n", bAdjustBG);
	fprintf(fp, "%d\n", iDotTime);
	fprintf(fp, "%d\n", iDisplayTime);
	fprintf(fp, "%d\n", iOffTime);
	fprintf(fp, "%f\n", fSpatialFreq);
	fprintf(fp, "%d\n", iSineColor);
	fprintf(fp, "%d\n", iNumGratings);
	
	fclose(fp);
}