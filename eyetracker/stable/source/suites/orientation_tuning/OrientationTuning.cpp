/**
 * @file OrientationTuning.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "OrientationTuning.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

COrientationTuning::COrientationTuning() 
{
	//defaultFile = CString("C:\\Stimulus Settings\\OrientationTuning\\default.txt");
	fBarWidth = 1;
	fBarHeight = 2;
	fAngularStep = 45;
	fMotionSpeed = 1;
	iIntertrialTime = 500;
	iTrialDuration = 2000;
	iRepeats = 3;
	fXPosition = 0;
	fYPosition = 0;
	unitTag = CString("SMSR01_01_D001");
	blankPeriod = 0;
	iReceptiveField = FIELD_MANUAL;
	fStartAngle = 0;
	fEndAngle = 315;
	fFixX = 0;
	fFixY = 0;
	fFixD = 0;
	iFixColorR = 255;
	iFixColorG = 0;
	iFixColorB = 0;
	bUseFixation = true;

	LoadFile(true);
}

COrientationTuning::~COrientationTuning() {}

void COrientationTuning::LoadFile(bool bDefault) 
{
	CString defaultFile = CString("C:\\Stimulus Settings\\OrientationTuning\\default.txt");
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

	char buffer2[1024];
			
	fscanf(fp, "%f\n", &fBarWidth);
	fscanf(fp, "%f\n", &fBarHeight);
	fscanf(fp, "%f\n", &fAngularStep);
	fscanf(fp, "%f\n", &fMotionSpeed);
	fscanf(fp, "%d\n", &iIntertrialTime);
	fscanf(fp, "%d\n", &iTrialDuration);
	fscanf(fp, "%d\n", &iRepeats);
	fscanf(fp, "%f\n", &fXPosition);
	fscanf(fp, "%f\n", &fYPosition);
	
	fscanf(fp, "%[^\n]\n", &buffer2);
	unitTag = CString(buffer2);
	
	fscanf(fp, "%d\n", &blankPeriod);
	fscanf(fp, "%f\n", &fStartAngle);
	fscanf(fp, "%f\n", &fEndAngle);
	fscanf(fp, "%f\n", & fFixX);
	fscanf(fp, "%f\n", & fFixY);
	fscanf(fp, "%f\n", & fFixD);
	fscanf(fp, "%d\n", &iFixColorR);
	fscanf(fp, "%d\n", &iFixColorG);
	fscanf(fp, "%d\n", &iFixColorB);
	fscanf(fp, "%d\n", &bUseFixation);

	fclose(fp);
}

void COrientationTuning::SaveFile(bool bDefault) 
{
	CString defaultFile = CString("C:\\Stimulus Settings\\OrientationTuning\\default.txt");
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

	fprintf(fp, "%f\n", fBarWidth);
	fprintf(fp, "%f\n", fBarHeight);
	fprintf(fp, "%f\n", fAngularStep);
	fprintf(fp, "%f\n", fMotionSpeed);
	fprintf(fp, "%d\n", iIntertrialTime);
	fprintf(fp, "%d\n", iTrialDuration);
	fprintf(fp, "%d\n", iRepeats);
	fprintf(fp, "%f\n", fXPosition);
	fprintf(fp, "%f\n", fYPosition);
	
	fprintf(fp, "%s\n", unitTag);
			
	fprintf(fp, "%d\n", blankPeriod);
	fprintf(fp, "%f\n", fStartAngle);
	fprintf(fp, "%f\n", fEndAngle);
	fprintf(fp, "%f\n", fFixX);
	fprintf(fp, "%f\n", fFixY);
	fprintf(fp, "%f\n", fFixD);
	fprintf(fp, "%d\n", iFixColorR);
	fprintf(fp, "%d\n", iFixColorG);
	fprintf(fp, "%d\n", iFixColorB);
	fprintf(fp, "%d\n", bUseFixation);

	fclose(fp);
}
