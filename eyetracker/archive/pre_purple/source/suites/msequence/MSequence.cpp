/**
 * @file MSequence.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "MSequence.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CMSequence::CMSequence() 
{
	defaultFile = CString("C:\\Stimulus Settings\\MSequence\\default.txt");
	iWidth=20;
	iOrder=7;
	iTap=3;
	iDelay=200;
	iNumBars=16;

	iHeight=300;
	dHCenter=0.0;
	dVCenter=0.0;
	iOrientation=0;
	bTextmarks=true;
	
	iCurrentOffset=0;
	iOrder2=7;
	iTap2=3;
	iOrder3=7;
	iTap3=3;
	iRedR = 255;
	iRedG = 0;
	iRedB = 0;
	iGreenR = 0;
	iGreenG = 255;
	iGreenB = 0;
	iBlueR = 0;
	iBlueG = 0;
	iBlueB = 255;

	LoadFile(true);
}

CMSequence::~CMSequence() {}

void CMSequence::LoadFile(bool bDefault) 
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

	fscanf(fp, "%d\n", &iWidth);
	fscanf(fp, "%d\n", &iOrder);
	fscanf(fp, "%d\n", &iTap);
	fscanf(fp, "%d\n", &iDelay);
	fscanf(fp, "%d\n", &iNumBars);

	fscanf(fp, "%d\n", &iHeight);
	fscanf(fp, "%f\n", &dHCenter);
	fscanf(fp, "%f\n", &dVCenter);
	fscanf(fp, "%d\n", &iOrientation);
	fscanf(fp, "%d\n", &bTextmarks);
	fscanf(fp, "%d\n", &iOrder2);
	fscanf(fp, "%d\n", &iTap2);
	fscanf(fp, "%d\n", &iOrder3);
	fscanf(fp, "%d\n", &iTap3);
	fscanf(fp, "%d\n", &iRedR);
	fscanf(fp, "%d\n", &iRedG);
	fscanf(fp, "%d\n", &iRedB);
	fscanf(fp, "%d\n", &iBlueR);
	fscanf(fp, "%d\n", &iBlueG);
	fscanf(fp, "%d\n", &iBlueB);
	fscanf(fp, "%d\n", &iGreenR);
	fscanf(fp, "%d\n", &iGreenB);
	fscanf(fp, "%d\n", &iGreenG);

	fclose(fp);
}

void CMSequence::SaveFile(bool bDefault) 
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
	
	fprintf(fp, "%d\n", iWidth);
	fprintf(fp, "%d\n", iOrder);
	fprintf(fp, "%d\n", iTap);
	fprintf(fp, "%d\n", iDelay);
	fprintf(fp, "%d\n", iNumBars);
	fprintf(fp, "%d\n", iHeight);
	fprintf(fp, "%f\n", dHCenter);
	fprintf(fp, "%f\n", dVCenter);
	fprintf(fp, "%d\n", iOrientation);
	fprintf(fp, "%d\n", bTextmarks);
	fprintf(fp, "%d\n", iOrder2);
	fprintf(fp, "%d\n", iTap2);
	fprintf(fp, "%d\n", iOrder3);
	fprintf(fp, "%d\n", iTap3);
	fprintf(fp, "%d\n", iRedR);
	fprintf(fp, "%d\n", iRedG);
	fprintf(fp, "%d\n", iRedB);
	fprintf(fp, "%d\n", iBlueR);
	fprintf(fp, "%d\n", iBlueG);
	fprintf(fp, "%d\n", iBlueB);
	fprintf(fp, "%d\n", iGreenR);
	fprintf(fp, "%d\n", iGreenG);
	fprintf(fp, "%d\n", iGreenB);

	fclose(fp);
}

int* CMSequence::generateSeq(int n, int t) 
{
	int q=(int)pow(2.0F,n-1);
	int r=(int)pow(2.0F,n);
	int * seq = (int*)malloc(sizeof(int)*r);

	for (int i=0; i<r; i++) {
		seq[i]=0;
	}

	seq[0]=1;
	for (int i=0; i<r-1; i++) {
		seq[i+1] = seq[i] << 1;
		if ((seq[i]&q) == q) {
			seq[i+1]= (seq[i+1] ^ t)%r;
		}
	}

	return seq;
}
