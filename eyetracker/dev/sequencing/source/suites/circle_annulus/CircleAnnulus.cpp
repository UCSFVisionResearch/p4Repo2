/**
 * @file CircleAnnulus.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include "CircleAnnulus.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CCircleAnnulus::CCircleAnnulus() 
{
	iIntertrialDuration = 500;
	iTrialDuration = 2000;
	iRepeats = 3;
	fXPosition = 0;
	fYPosition = 0;
	unitTag = CString("SMSR01_01_D001");
	pColorList = NULL;
	pSecondaryColorList = NULL;
	iNumColors = 0;
	iColorListLength = 0;
	iNumSecondaryColors = 0;
	iSecondaryColorListLength = 0;
	
	blankPeriod = 0;
	bOnOff = false;
	iReceptiveField = FIELD_MANUAL;
	iShape = SHAPES_CIRCLES;
	LoadFile(true);
}

CCircleAnnulus::~CCircleAnnulus() 
{
	clearColors();
	clearSecondaryColors();
}

void CCircleAnnulus::addColor(float red, float green, float blue) 
{
	for (int i = 0; i < iNumColors; i++) {
		if (red == pColorList[i].red && green == pColorList[i].green && blue == pColorList[i].blue) {
			return;
		}
	}
	
	// check the array size before addign elements
	if (iNumColors == iColorListLength) {
		if (iColorListLength > 0) {
			iColorListLength += 5;
			pColorList = (ColorListItem*)realloc(pColorList, sizeof(ColorListItem) * iColorListLength);
		} 
		else {
			iColorListLength += 5;
			pColorList = (ColorListItem*)calloc(iColorListLength, sizeof(ColorListItem));
		}
	}
	
	// add new element
	pColorList[iNumColors].red = red;
	pColorList[iNumColors].green = green;
	pColorList[iNumColors].blue = blue;
	
	sprintf(pColorList[iNumColors].listString, "R-%.3f, G-%.3f, B-%.3f", red, green, blue);
	iNumColors++;
}

void CCircleAnnulus::removeColor(int i) 
{
	if (i < iNumColors) {
		for (int j=i+1; j < iNumColors; j++) {
			pColorList[j-1].red = pColorList[j].red;
			pColorList[j-1].green = pColorList[j].green;
			pColorList[j-1].blue = pColorList[j].blue;
			strcpy(pColorList[j-1].listString,pColorList[j].listString);
		}
		iNumColors--;
	}
}

void CCircleAnnulus::clearColors() 
{
	iNumColors = 0;
	iColorListLength = 0;
	free(pColorList);
}

CString CCircleAnnulus::getColorString(int i) 
{
	if (i < iNumColors) {
		return pColorList[i].listString;
	}

	return CString("");
}

ColorListItem CCircleAnnulus::getColor(int i) 
{
	ColorListItem c;
	c.red = 0;
	c.green = 0;
	c.blue = 0;
	c.listString[0]=0;
	if (i < iNumColors) {
		return pColorList[i];
	} 
	else 
		return c;
}

void CCircleAnnulus::addSecondaryColor(float red, float green, float blue) 
{
	for (int i = 0; i < iNumSecondaryColors; i++) {
		if (red == pSecondaryColorList[i].red && green == pSecondaryColorList[i].green && blue == pSecondaryColorList[i].blue) {
			return;
		}
	}
	
	// check the array size before addign elements
	if (iNumSecondaryColors == iSecondaryColorListLength) {
		if (iSecondaryColorListLength > 0) {
			iSecondaryColorListLength += 5;
			pSecondaryColorList = (ColorListItem*)realloc(pSecondaryColorList, sizeof(ColorListItem) * iSecondaryColorListLength);
		} 
		else {
			iSecondaryColorListLength += 5;
			pSecondaryColorList = (ColorListItem*)calloc(iSecondaryColorListLength, sizeof(ColorListItem));
		}
	}

	// add new element
	pSecondaryColorList[iNumSecondaryColors].red = red;
	pSecondaryColorList[iNumSecondaryColors].green = green;
	pSecondaryColorList[iNumSecondaryColors].blue = blue;
	
	sprintf(pSecondaryColorList[iNumSecondaryColors].listString, "R-%.3f, G-%.3f, B-%.3f", red, green, blue);
	iNumSecondaryColors++;
}

void CCircleAnnulus::removeSecondaryColor(int i) 
{
	if (i < iNumSecondaryColors) {
		for (int j=i+1; j < iNumSecondaryColors; j++) {
			pSecondaryColorList[j-1].red = pSecondaryColorList[j].red;
			pSecondaryColorList[j-1].green = pSecondaryColorList[j].green;
			pSecondaryColorList[j-1].blue = pSecondaryColorList[j].blue;
			strcpy(pSecondaryColorList[j-1].listString,pSecondaryColorList[j].listString);
		}

		iNumSecondaryColors--;
	}
}

void CCircleAnnulus::clearSecondaryColors() 
{
	iNumSecondaryColors = 0;
	iSecondaryColorListLength = 0;
	free(pSecondaryColorList);
}

CString CCircleAnnulus::getSecondaryColorString(int i) 
{
	if (i < iNumSecondaryColors) {
		return pSecondaryColorList[i].listString;
	}
	return CString("");
}

ColorListItem CCircleAnnulus::getSecondaryColor(int i) 
{
	ColorListItem c;
	c.red = 0;
	c.green = 0;
	c.blue = 0;
	c.listString[0]=0;
	if (i < iNumSecondaryColors) {
		return pSecondaryColorList[i];
	} else return c;
}

void CCircleAnnulus::LoadFile(bool bDefault) 
{
	CString defaultFile = CString("C:\\Stimulus Settings\\CircleAnnulus\\default.txt");
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
	
	fscanf(fp, "%d\n", &blankPeriod);
	fscanf(fp, "%d\n", &iIntertrialDuration);
	fscanf(fp, "%d\n", &iTrialDuration);
	fscanf(fp, "%d\n", &iRepeats);
	fscanf(fp, "%f\n", &fXPosition);
	fscanf(fp, "%f\n", &fYPosition);
	fscanf(fp, "%[^\n]\n", &buffer2);
	unitTag = CString(buffer2);
	fscanf(fp, "%d\n", &bOnOff);
	fscanf(fp, "%f\n", &circleRadiusMin);
	fscanf(fp, "%f\n", &circleRadiusMax);
	fscanf(fp, "%f\n", &circleRadiusStep);
	fscanf(fp, "%f\n", &annulusWidthMin);
	fscanf(fp, "%f\n", &annulusWidthMax);
	fscanf(fp, "%f\n", &annulusWidthStep);
	fscanf(fp, "%d\n", &iReceptiveField);
	fscanf(fp, "%d\n", &iShape);
	fscanf(fp, "%d\n", &iNumColors);
	fscanf(fp, "%d\n", &iColorListLength);
	if (iColorListLength > 0) {
		pColorList = (ColorListItem*)calloc(iColorListLength, sizeof(ColorListItem));
		for (int i = 0; i < iNumColors; i++) {
			fscanf(fp, "%f\n", &(pColorList[i].red));
			fscanf(fp, "%f\n", &(pColorList[i].green));
			fscanf(fp, "%f\n", &(pColorList[i].blue));
			fscanf(fp, "%[^\n]\n", &buffer2);
			strcpy(pColorList[i].listString, buffer2);
		}
	}

	fscanf(fp, "%d\n", &iNumSecondaryColors);
	fscanf(fp, "%d\n", &iSecondaryColorListLength);
	if (iSecondaryColorListLength > 0) {
		pSecondaryColorList = (ColorListItem*)calloc(iSecondaryColorListLength, sizeof(ColorListItem));
		for (int i = 0; i < iNumSecondaryColors; i++) {
			fscanf(fp, "%f\n", &(pSecondaryColorList[i].red));
			fscanf(fp, "%f\n", &(pSecondaryColorList[i].green));
			fscanf(fp, "%f\n", &(pSecondaryColorList[i].blue));
			fscanf(fp, "%[^\n]\n", &buffer2);
			strcpy(pSecondaryColorList[i].listString, buffer2);
		}
	}

	fclose(fp);
}

void CCircleAnnulus::SaveFile(bool bDefault) 
{
	CString defaultFile = CString("C:\\Stimulus Settings\\CircleAnnulus\\default.txt");
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

	fprintf(fp, "%d\n", blankPeriod);
	fprintf(fp, "%d\n", iIntertrialDuration);
	fprintf(fp, "%d\n", iTrialDuration);
	fprintf(fp, "%d\n", iRepeats);
	fprintf(fp, "%f\n", fXPosition);
	fprintf(fp, "%f\n", fYPosition);
	fprintf(fp, "%s\n", unitTag);
	fprintf(fp, "%d\n", bOnOff);
	fprintf(fp, "%f\n", circleRadiusMin);
	fprintf(fp, "%f\n", circleRadiusMax);
	fprintf(fp, "%f\n", circleRadiusStep);
	fprintf(fp, "%f\n", annulusWidthMin);
	fprintf(fp, "%f\n", annulusWidthMax);
	fprintf(fp, "%f\n", annulusWidthStep);
	fprintf(fp, "%d\n", iReceptiveField);
	fprintf(fp, "%d\n", iShape);
	fprintf(fp, "%d\n", iNumColors);
	fprintf(fp, "%d\n", iColorListLength);
	for (int i = 0; i < iNumColors; i++) {
		fprintf(fp, "%f\n", pColorList[i].red);
		fprintf(fp, "%f\n", pColorList[i].green);
		fprintf(fp, "%f\n", pColorList[i].blue);
		fprintf(fp, "%s\n", pColorList[i].listString);
	}

	fprintf(fp, "%d\n", iNumSecondaryColors);
	fprintf(fp, "%d\n", iSecondaryColorListLength);
	for (int i = 0; i < iNumSecondaryColors; i++) {
		fprintf(fp, "%f\n", pSecondaryColorList[i].red);
		fprintf(fp, "%f\n", pSecondaryColorList[i].green);
		fprintf(fp, "%f\n", pSecondaryColorList[i].blue);
		fprintf(fp, "%s\n", pSecondaryColorList[i].listString);
	}

	fclose(fp);
}
