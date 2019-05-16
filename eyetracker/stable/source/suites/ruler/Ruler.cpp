/**
 * @file Ruler.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "Ruler.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CRuler::CRuler()
{
	defaultFile = CString("C:\\Stimulus Settings\\Ruler\\default.txt");
	
	iNumRings = 6;
	bBox = false;
	
	LoadFile(true);
}

CRuler::~CRuler()
{
	defaultFile = CString("C:\\Stimulus Settings\\Ruler\\default.txt");
	
	iNumRings = 6;
	bBox = false;
	
	LoadFile(true);
}

void CRuler::LoadFile(bool bDefault) 
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

	fscanf(fp, "%d\n", &iNumRings);
	fscanf(fp, "%d\n", &bBox);
	
	fclose(fp);
}

void CRuler::SaveFile(bool bDefault) 
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

	fprintf(fp, "%d\n", iNumRings);
	fprintf(fp, "%d\n", bBox);

	fclose(fp);
}