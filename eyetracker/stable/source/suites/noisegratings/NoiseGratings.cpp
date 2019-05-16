/**
 * @file NoiseGratings.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "NoiseGratings.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

CNoiseGratings::CNoiseGratings() 
{
	defaultFile = CString("C:\\Stimulus Settings\\NoiseGratings\\default.txt");
	iMeanLevel = 128;
	iAmplitude = 50;
	iType = NOISE_TYPE_WHITE;
	bUseMask = false;
	fMaskRadius = 5;
	iBlending = BLENDING_TYPE_LINEAR;
	bUseMouseControl = false;
	xDeg = 0; 
	yDeg = 0;
	iGratingType = GRATING_TYPE_SQUARE;
	fAutomationSpeed = 1;

	bAutoFreq = false;
	bAutoContrast = false;
	bAutoOrientation = false;
	bAutoWidth = false;
	bAutoHeight = false;
	bAutoPhase = false;

	fMinFreq = 1;
	fMaxFreq = 10;
	fStepFreq = 1;
	fSetPtFreq = 5;

	fMinOrientation = 1;
	fMaxOrientation = 10;
	fStepOrientation = 1;
	fSetPtOrientation = 5;

	fMinContrast = 1;
	fMaxContrast = 10;
	fStepContrast = 1;
	fSetPtContrast = 5;

	fMinWidth = 1;
	fMaxWidth = 10;
	fStepWidth = 1;
	fSetPtWidth = 5;

	fMinHeight = 1;
	fMaxHeight = 10;
	fStepHeight = 1;
	fSetPtHeight = 5;

	fMinPhase = 1;
	fMaxPhase = 10;
	fStepPhase = 1;
	fSetPtPhase = 5;

	LoadFile(true);
}

CNoiseGratings::~CNoiseGratings() {}

void CNoiseGratings::LoadFile(bool bDefault) 
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
	
	fscanf(fp, "%d\n", &iMeanLevel);
	fscanf(fp, "%d\n", &iAmplitude);
	fscanf(fp, "%d\n", &iType);
	fscanf(fp, "%d\n", &bUseMask);
	fscanf(fp, "%f\n", &fMaskRadius);
	fscanf(fp, "%d\n", &iBlending);
	fscanf(fp, "%d\n", &bUseMouseControl);
	fscanf(fp, "%f\n", &xDeg);
	fscanf(fp, "%f\n", &yDeg);
	fscanf(fp, "%d\n", &iGratingType);
	fscanf(fp, "%f\n", &fAutomationSpeed);
	
	fscanf(fp, "%d\n", &bAutoFreq);
	fscanf(fp, "%d\n", &bAutoContrast);
	fscanf(fp, "%d\n", &bAutoOrientation);
	fscanf(fp, "%d\n", &bAutoWidth);
	fscanf(fp, "%d\n", &bAutoHeight);
	fscanf(fp, "%d\n", &bAutoPhase);

	fscanf(fp, "%f\n", &fMinFreq);
	fscanf(fp, "%f\n", &fMaxFreq);
	fscanf(fp, "%f\n", &fStepFreq);
	fscanf(fp, "%f\n", &fSetPtFreq);

	fscanf(fp, "%f\n", &fMinContrast);
	fscanf(fp, "%f\n", &fMaxContrast);
	fscanf(fp, "%f\n", &fStepContrast);
	fscanf(fp, "%f\n", &fSetPtContrast);

	fscanf(fp, "%f\n", &fMinOrientation);
	fscanf(fp, "%f\n", &fMaxOrientation);
	fscanf(fp, "%f\n", &fStepOrientation);
	fscanf(fp, "%f\n", &fSetPtOrientation);

	fscanf(fp, "%f\n", &fMinWidth);
	fscanf(fp, "%f\n", &fMaxWidth);
	fscanf(fp, "%f\n", &fStepWidth);
	fscanf(fp, "%f\n", &fSetPtWidth);

	fscanf(fp, "%f\n", &fMinHeight);
	fscanf(fp, "%f\n", &fMaxHeight);
	fscanf(fp, "%f\n", &fStepHeight);
	fscanf(fp, "%f\n", &fSetPtHeight);

	fscanf(fp, "%f\n", &fMinPhase);
	fscanf(fp, "%f\n", &fMaxPhase);
	fscanf(fp, "%f\n", &fStepPhase);
	fscanf(fp, "%f\n", &fSetPtPhase);

	fclose(fp);
}

void CNoiseGratings::SaveFile(bool bDefault) 
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
	
	fprintf(fp, "%d\n", iMeanLevel);
	fprintf(fp, "%d\n", iAmplitude);
	fprintf(fp, "%d\n", iType);
	fprintf(fp, "%d\n", bUseMask);
	fprintf(fp, "%f\n", fMaskRadius);
	fprintf(fp, "%d\n", iBlending);
	fprintf(fp, "%d\n", bUseMouseControl);
	fprintf(fp, "%f\n", xDeg);
	fprintf(fp, "%f\n", yDeg);
	fprintf(fp, "%d\n", iGratingType);
	fprintf(fp, "%f\n", fAutomationSpeed);
	
	fprintf(fp, "%d\n", bAutoFreq);
	fprintf(fp, "%d\n", bAutoContrast);
	fprintf(fp, "%d\n", bAutoOrientation);
	fprintf(fp, "%d\n", bAutoWidth);
	fprintf(fp, "%d\n", bAutoHeight);
	fprintf(fp, "%d\n", bAutoPhase);

	fprintf(fp, "%f\n", fMinFreq);
	fprintf(fp, "%f\n", fMaxFreq);
	fprintf(fp, "%f\n", fStepFreq);
	fprintf(fp, "%f\n", fSetPtFreq);

	fprintf(fp, "%f\n", fMinContrast);
	fprintf(fp, "%f\n", fMaxContrast);
	fprintf(fp, "%f\n", fStepContrast);
	fprintf(fp, "%f\n", fSetPtContrast);

	fprintf(fp, "%f\n", fMinOrientation);
	fprintf(fp, "%f\n", fMaxOrientation);
	fprintf(fp, "%f\n", fStepOrientation);
	fprintf(fp, "%f\n", fSetPtOrientation);

	fprintf(fp, "%f\n", fMinWidth);
	fprintf(fp, "%f\n", fMaxWidth);
	fprintf(fp, "%f\n", fStepWidth);
	fprintf(fp, "%f\n", fSetPtWidth);

	fprintf(fp, "%f\n", fMinHeight);
	fprintf(fp, "%f\n", fMaxHeight);
	fprintf(fp, "%f\n", fStepHeight);
	fprintf(fp, "%f\n", fSetPtHeight);

	fprintf(fp, "%f\n", fMinPhase);
	fprintf(fp, "%f\n", fMaxPhase);
	fprintf(fp, "%f\n", fStepPhase);
	fprintf(fp, "%f\n", fSetPtPhase);

	fclose(fp);
}
