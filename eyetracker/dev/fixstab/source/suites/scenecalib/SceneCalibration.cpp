/**
 * @file SceneCalibration.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "SceneCalibration.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CSceneCalibration::CSceneCalibration() 
{
	defaultFile = CString("C:\\Stimulus Settings\\SceneCalibration\\default.txt");
	strImagePath = CString("C:\\docume~1\\Amar.HORTONLAB\\Desktop\\waldo.bmp");
	xPosition = 0;
	yPosition = 0;

	LoadFile(true);	
}

CSceneCalibration::~CSceneCalibration() {}

void CSceneCalibration::LoadFile(bool bDefault) 
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
	strImagePath = CString(buffer2);
			
	fscanf(fp, "%f\n", &xPosition);
	fscanf(fp, "%f\n", &yPosition);
	
	fclose(fp);
}

void CSceneCalibration::SaveFile(bool bDefault) 
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
	
	fprintf(fp, "%s\n", strImagePath);
	fprintf(fp, "%f\n", xPosition);
	fprintf(fp, "%f\n", yPosition);

	fclose(fp);
}
