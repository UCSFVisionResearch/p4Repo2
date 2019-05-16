/**
 * @file PrefSelection.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "PrefSelection.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"
#include "suites/settings/StaircaseSettings.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CPrefSelection::CPrefSelection() 
{
	defaultFile = CString("C:\\Stimulus Settings\\ContrastSensitivity\\default.txt");
	
	iCorrectTimeOut = 1100;
	iTimeOut = 5000;
	iTargetDelay = 1000;
	strSpatFreqList = "51, 26, 13, 6, 4, 2, 1, .5, .25";
	strInitContrastList = "100, 100, 75, 75, 50, 50, 50, 50, 50";
	pStaircaseSettings = new CStaircaseSettings();
	pStaircaseSettings->setCoarseStep(-25);
	pStaircaseSettings->setMedStep(-5);
	pStaircaseSettings->setFineStep(-0.5f);
	pStaircaseSettings->setUberFineStep(-0.05f);
	pStaircaseSettings->setNUp(3);
	pStaircaseSettings->setNDown(2);
	pStaircaseSettings->setNumReversals(7);

	gratingHeight = 5;
	gratingWidth = 2.75;
	gratingPosition = 2.25;
	dotPosition = 4.75;
	dotSize = 0.15;
	iPctTrialsRight = 50;
	bTrainingMode = false;

	LoadFile(true);

	iNumLevels = 0;
	pLevels = NULL;
}

CPrefSelection::~CPrefSelection() 
{
	for (int i = 0; i < iNumLevels; i++) {
		if (pLevels[i].pStaircase)
			delete pLevels[i].pStaircase;
	}
	free(pLevels);
	pLevels = NULL;

	if (pStaircaseSettings) delete pStaircaseSettings;
	pStaircaseSettings = NULL;
}

void CPrefSelection::setSpatialFreqList(CString list) 
{
	// strip off excess commas
	list.Trim();
	if (list.ReverseFind(',') == list.GetLength() - 1) {
		list = list.Left(list.GetLength() - 1).Trim();
	}
	if (list.Find(',') == 0) {
		list = list.Right(list.GetLength() - 1).Trim();
	}
	strSpatFreqList = list; 

}

void CPrefSelection::setInitialContrastList(CString list) 
{
	// strip off excess commas
	list.Trim();
	if (list.ReverseFind(',') == list.GetLength() - 1) {
		list = list.Left(list.GetLength() - 1).Trim();
	}
	if (list.Find(',') == 0) {
		list = list.Right(list.GetLength() - 1).Trim();
	}
	strInitContrastList = list; 

}

int CPrefSelection::getNumListElements(CString list) 
{
	// number of elements = number of commas + 1
	int numElements = 1;
	int iPosition = list.Find(",", 0);
	while (iPosition > 0) {
		numElements++;
		iPosition = list.Find(",", iPosition+1);
	}

	return numElements;
}

void CPrefSelection::extractListElements(CString list, float* pElements, int iNumElements) 
{
	list = list + ",";
	
	int iPosition = list.Find(",", 0);
	int iLastPos = 0;
	int index = 0;

	while (iPosition > 0) {
		// extract number portion of hte list
        CString temp = list.Mid(iLastPos, iPosition - iLastPos);
		
		// convert to float
		char buffer[20];
		char* endptr;
		sprintf(buffer, "%s", temp);
		double data = strtod(buffer, &endptr); 
		if (strlen(endptr) > 0) {
			AfxMessageBox("Invalid floating point value in list");
			data = 0;
		}
		pElements[index] = data;

		// advance positions and counters
		index++;
		iLastPos = iPosition+1;
		if (iLastPos > list.GetLength()) {
			iPosition = -1;
		} 
		else {
			iPosition = list.Find(",", iLastPos);
		}
	}
}

void CPrefSelection::generateLevelsList() 
{
	// get number of elements in each list
	int iNumSpatialFreqElements = getNumListElements(strSpatFreqList);
	int iNumContrastLevels = getNumListElements(strInitContrastList);

	// create the indivudual list arrays
	float* spatialFreqLevels = (float*)malloc(iNumSpatialFreqElements * sizeof(float));
	float* initialContrastLevels = (float*)malloc(iNumContrastLevels * sizeof(float));

	// extract the elements of each individual list
	extractListElements(strSpatFreqList, spatialFreqLevels, iNumSpatialFreqElements);
	extractListElements(strInitContrastList, initialContrastLevels, iNumContrastLevels);

	// delete the levels array if it already exists
	if (pLevels) free(pLevels);
	
	// allocate the levels array and set the number elements in numLevels
	iNumLevels = iNumSpatialFreqElements;
	pLevels = (level*)malloc(iNumLevels * sizeof(level));

	// fill the levels array with the appropriate data
	for (int j = 0; j < iNumSpatialFreqElements; j++) {
		pLevels[j].pStaircase = NULL;
		pLevels[j].initialContrast = initialContrastLevels[j];
		pLevels[j].spatialFreq = spatialFreqLevels[j];
		
	}
	
	// destroy the individual lists
	free(spatialFreqLevels);
	free(initialContrastLevels);
}

void CPrefSelection::LoadFile(bool bDefault) 
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
	float f;
	int i;
	char buffer2[1024];
	fscanf(fp, "%d\n", &iCorrectTimeOut);
	fscanf(fp, "%d\n", &iTimeOut);
	fscanf(fp, "%d\n", &iTargetDelay);
	fscanf(fp, "%[^\n]\n", &buffer2);
	strSpatFreqList = CString(buffer2);
	fscanf(fp, "%[^\n]\n", &buffer2);
	strInitContrastList = CString(buffer2);

	fscanf(fp, "%f\n", &f);
	pStaircaseSettings->setCoarseStep(f);

	fscanf(fp, "%f\n", &f);
	pStaircaseSettings->setMedStep(f);

	fscanf(fp, "%f\n", &f);
	pStaircaseSettings->setFineStep(f);

	fscanf(fp, "%f\n", &f);
	pStaircaseSettings->setUberFineStep(f);

	fscanf(fp, "%d\n", &i);
	pStaircaseSettings->setNUp(i);

	fscanf(fp, "%d\n", &i);
	pStaircaseSettings->setNDown(i);

	fscanf(fp, "%d\n", &i);
	pStaircaseSettings->setNumReversals(i);

	fscanf(fp, "%f\n", &gratingHeight);
	fscanf(fp, "%f\n", &gratingWidth);
	fscanf(fp, "%f\n", &gratingPosition);
	fscanf(fp, "%f\n", &dotPosition);
	fscanf(fp, "%f\n", &dotSize);

	fscanf(fp, "%d\n", &bTrainingMode);
	fscanf(fp, "%d\n", &iPctTrialsRight);

	fclose(fp);
}

void CPrefSelection::SaveFile(bool bDefault) 
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
	
	fprintf(fp, "%d\n", iCorrectTimeOut);
	fprintf(fp, "%d\n", iTimeOut);
	fprintf(fp, "%d\n", iTargetDelay);
	fprintf(fp, "%s\n", strSpatFreqList);
	fprintf(fp, "%s\n", strInitContrastList);

	fprintf(fp, "%f\n", pStaircaseSettings->getCoarseStep());
	fprintf(fp, "%f\n", pStaircaseSettings->getMedStep());
	fprintf(fp, "%f\n", pStaircaseSettings->getFineStep());
	fprintf(fp, "%f\n", pStaircaseSettings->getUberFineStep());
	fprintf(fp, "%d\n", pStaircaseSettings->getNUp());
	fprintf(fp, "%d\n", pStaircaseSettings->getNDown());
	fprintf(fp, "%d\n", pStaircaseSettings->getNumReversals());
	
	fprintf(fp, "%f\n", gratingHeight);
	fprintf(fp, "%f\n", gratingWidth);
	fprintf(fp, "%f\n", gratingPosition);
	fprintf(fp, "%f\n", dotPosition);
	fprintf(fp, "%f\n", dotSize);

	fprintf(fp, "%d\n", bTrainingMode);
	fprintf(fp, "%d\n", iPctTrialsRight);

	fclose(fp);
}

void CPrefSelection::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "PrefSelection;CorrectTimeOut;%d;%ld|0x%02x/\0", iCorrectTimeOut, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "PrefSelection;TimeOut;%d;%ld|0x%02x/\0", iTimeOut, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "PrefSelection;TargetDelay;%d;%ld|0x%02x/\0", iTargetDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));

	sprintf(buffer, "PrefSelection;CoarseStep;%.4f;%ld|0x%02x/\0", pStaircaseSettings->getCoarseStep(), tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "PrefSelection;MedStep;%.4f;%ld|0x%02x/\0", pStaircaseSettings->getMedStep(), tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "PrefSelection;FineStep;%.4f;%ld|0x%02x/\0", pStaircaseSettings->getFineStep(), tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "PrefSelection;UberFineStep;%.4f;%ld|0x%02x/\0", pStaircaseSettings->getUberFineStep(), tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "PrefSelection;NUp;%d;%ld|0x%02x/\0", pStaircaseSettings->getNUp(), tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "PrefSelection;NDown;%d;%ld|0x%02x/\0", pStaircaseSettings->getNDown(), tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "PrefSelection;NumReversals;%d;%ld|0x%02x/\0", pStaircaseSettings->getNumReversals(), tm, code);		theComm.transmit(buffer, (int)strlen(buffer));

	sprintf(buffer, "PrefSelection;gratingHeight;%.4f;%ld|0x%02x/\0", gratingHeight, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "PrefSelection;gratingWidth;%.4f;%ld|0x%02x/\0", gratingWidth, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "PrefSelection;gratingPosition;%.4f;%ld|0x%02x/\0", gratingPosition, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "PrefSelection;dotPosition;%.4f;%ld|0x%02x/\0", dotPosition, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "PrefSelection;dotSize;%.4f;%ld|0x%02x/\0", dotSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));

	sprintf(buffer, "PrefSelection;TrainingMode;%d;%ld|0x%02x/\0", bTrainingMode, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "PrefSelection;PctTrialsRight;%d;%ld|0x%02x/\0", iPctTrialsRight, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}
