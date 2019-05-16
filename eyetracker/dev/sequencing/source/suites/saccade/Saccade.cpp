/**
 * @file Saccade.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "Saccade.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"
#include "utils/Math.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CSaccade::CSaccade()
: m_displayNoiseBg(false), m_sameBgTrial(false), m_alternateNoise(false), 
  m_occludeBg(false), m_noiseRed(0), m_noiseGreen(0), m_noiseBlue(0), 
  m_noisePercent(0), m_noiseSize(0)
{
	defaultFile = CString("C:\\Stimulus Settings\\Saccade\\default.txt");
	bReturnToPt=false;
	bUseAllDirs=true;
	iDisplayDuration=2000;
	fDotSize=0.25f;
	fPointX=0;
	fPointY=0;
	fStepSize=5;
	iNumStepsX=2;
	iNumStepsY=2;
	bStepHorizontal = true;
	bStepVertical = false;
	bStep45Deg = false;
	bStep135Deg = false;
	iDistType = DIST_TYPE_RADIAL;
	pLocationsX = NULL;
	pLocationsY = NULL;
	bRandomDurations = false;
	iMinDuration = 2000;
	iMaxDuration = 5000;
	iMaxDisplayDuration = 10000;
	iBlankTime = 0;

	bEnableOscillations = false;
	fOscillationAmplitude = 1;
	fOscillationFreq = 5;
	iOscillationDirection = ORIENT_HORIZ;
	bBehavior = false;

	iRedPrimary = 255;
	iGreenPrimary = 255;
	iBluePrimary = 255;

	iRedSecondary = 255;
	iGreenSecondary = 0;
	iBlueSecondary = 0;

	bGridSteps=false;

	iColorSetting = COLOR_PRIMARY;
	iPointType = 0;
	bSequentialOrder = false;

	LoadFile(true);

	GenerateLocations();
}

CSaccade::~CSaccade() {}

int CSaccade::getRunTimeDisplayDuration() 
{
	if (bRandomDurations) {
		int iRange = iMaxDuration - iMinDuration;
		return iMinDuration + (rand() % iRange);
	} else {
		return iDisplayDuration;
	}
}

void CSaccade::GenerateLocations() 
{
	bool bNeedZeroLocation = false;
	int iNumDirections = 0;
	int iCurrLocation = 0;
	iNumLocations = 0;
	int iNumDiagSteps = 0;
	
	// calculate number of directions used
	if (bUseAllDirs) {
		if (iNumStepsX < iNumStepsY) {
			iNumDiagSteps = iNumStepsX;
		} 
		else {
			iNumDiagSteps = iNumStepsY;
		}
		iNumLocations = iNumStepsX * 2 + iNumStepsY * 2 + iNumDiagSteps * 4;
	} 
	else {
		if (bStepHorizontal) iNumLocations += iNumStepsX *2;
		if (bStepVertical) iNumLocations += iNumStepsY * 2;
		if (bStep45Deg) {
			if (iNumStepsX < iNumStepsY) {
				iNumLocations += iNumStepsX * 2;
				iNumDiagSteps = iNumStepsX;
			} 
			else {
				iNumLocations += iNumStepsY * 2;
				iNumDiagSteps = iNumStepsY;
			}
		}
		if (bStep135Deg) {
			if (iNumStepsX < iNumStepsY) {
				iNumLocations += iNumStepsX * 2;
				iNumDiagSteps = iNumStepsX;
			} 
			else {
				iNumLocations += iNumStepsY * 2;
				iNumDiagSteps = iNumStepsY;
			}
		}
	}

	// grid
	if (bGridSteps) iNumLocations = ((iNumStepsX * 2) + 1) * ((iNumStepsY * 2) + 1) - 1;
	
	if (!bAvoidZero) iNumLocations++;

	// free the memory if necessary
	if (pLocationsX) free(pLocationsX);
	if (pLocationsY) free(pLocationsY);

	// allocate the necessary memory
	pLocationsX = (double*)calloc(iNumLocations, sizeof(double));
	pLocationsY = (double*)calloc(iNumLocations, sizeof(double));

	if (!bAvoidZero) {
		pLocationsX[0] = 0;
		pLocationsY[0] = 0;
		iCurrLocation++;
	}
	
	if (bGridSteps) {
		// fill in everything on the grid except 0
		for (int i = -iNumStepsX; i < iNumStepsX+1; i++) {
			for (int j = -iNumStepsY; j < iNumStepsY+1; j++) {
				if (!(i==0 && j==0)) {
					pLocationsX[iCurrLocation] = i * fStepSize;
					pLocationsY[iCurrLocation] = j * fStepSize;
					iCurrLocation++;
				}
			}
		}
	} 
	else {
		// fill the horizontal locations
		if (bStepHorizontal || bUseAllDirs) {
			for (int i = 1; i < iNumStepsX+1; i++) {
				pLocationsX[iCurrLocation] = i * fStepSize;
				pLocationsY[iCurrLocation] = 0;
				iCurrLocation++;
			}
			for (int i = 1; i < iNumStepsX+1; i++) {
				pLocationsX[iCurrLocation] = -i * fStepSize;
				pLocationsY[iCurrLocation] = 0;
				iCurrLocation++;
			}
		}

		// fill the vertical locations
		if (bStepVertical || bUseAllDirs) {
			for (int i = 1; i < iNumStepsY+1; i++) {
				pLocationsY[iCurrLocation] = i * fStepSize;
				pLocationsX[iCurrLocation] = 0;
				iCurrLocation++;
			}
			for (int i = 1; i < iNumStepsY+1; i++) {
				pLocationsY[iCurrLocation] = -i * fStepSize;
				pLocationsX[iCurrLocation] = 0;
				iCurrLocation++;
			}
		}

		// fill the 45 degree locations
		if (bStep45Deg || bUseAllDirs) {
			// if we are using XY steps for oblique distances
			if (iDistType == DIST_TYPE_XY) {
				for (int i = 1; i < iNumDiagSteps+1; i++) {
					pLocationsX[iCurrLocation] = i * fStepSize;
					pLocationsY[iCurrLocation] = i * fStepSize;
					iCurrLocation++;
				}
				for (int i = 1; i < iNumDiagSteps+1; i++) {
					pLocationsX[iCurrLocation] = -i * fStepSize;
					pLocationsY[iCurrLocation] = -i * fStepSize;
					iCurrLocation++;
				}
			} 
			else {
				// using radial distance for oblique distances
				for (int i = 1; i < iNumDiagSteps+1; i++) {
					pLocationsX[iCurrLocation] = i * fStepSize * SQRT2_DOUBLE / 2;
					pLocationsY[iCurrLocation] = i * fStepSize * SQRT2_DOUBLE / 2;
					iCurrLocation++;
				}
				for (int i = 1; i < iNumDiagSteps+1; i++) {
					pLocationsX[iCurrLocation] = -i * fStepSize * SQRT2_DOUBLE / 2;
					pLocationsY[iCurrLocation] = -i * fStepSize * SQRT2_DOUBLE / 2;
					iCurrLocation++;
				}
			}
		}

		// fill the 135 degree locations
		if (bStep135Deg || bUseAllDirs) {
			if (iDistType == DIST_TYPE_XY) {
				// using XY steps for oblique distances
				for (int i = 1; i < iNumDiagSteps+1; i++) {
					pLocationsX[iCurrLocation] = i * fStepSize;
					pLocationsY[iCurrLocation] = -i * fStepSize;
					iCurrLocation++;
				}
				for (int i = 1; i < iNumDiagSteps+1; i++) {
					pLocationsX[iCurrLocation] = -i * fStepSize;
					pLocationsY[iCurrLocation] = i * fStepSize;
					iCurrLocation++;
				}
			} 
			else {
				// using radial distance for oblique distances
				for (int i = 1; i < iNumDiagSteps+1; i++) {
					pLocationsX[iCurrLocation] = i * fStepSize * SQRT2_DOUBLE / 2;
					pLocationsY[iCurrLocation] = -i * fStepSize * SQRT2_DOUBLE / 2;
					iCurrLocation++;
				}
				for (int i = 1; i < iNumDiagSteps+1; i++) {
					pLocationsX[iCurrLocation] = -i * fStepSize * SQRT2_DOUBLE / 2;
					pLocationsY[iCurrLocation] = i * fStepSize * SQRT2_DOUBLE / 2;
					iCurrLocation++;
				}
			}
		}
	}
}

void CSaccade::LoadFile(bool bDefault) 
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

	fscanf(fp, "%d\n", &bReturnToPt);
	fscanf(fp, "%d\n", &bUseAllDirs);
	fscanf(fp, "%d\n", &iDisplayDuration);
	fscanf(fp, "%f\n", &fDotSize);
	fscanf(fp, "%f\n", &fPointX);
	fscanf(fp, "%f\n", &fPointY);
	fscanf(fp, "%f\n", &fStepSize);
	fscanf(fp, "%d\n", &iNumStepsX);
	fscanf(fp, "%d\n", &bStepHorizontal);
	fscanf(fp, "%d\n", &bStepVertical);
	fscanf(fp, "%d\n", &bStep45Deg);
	fscanf(fp, "%d\n", &bStep135Deg);
	fscanf(fp, "%d\n", &iDistType);
	fscanf(fp, "%d\n", &bRandomDurations);
	fscanf(fp, "%d\n", &iMinDuration);
	fscanf(fp, "%d\n", &iMaxDuration);
	fscanf(fp, "%d\n", &bEnableOscillations);
	fscanf(fp, "%f\n", &fOscillationAmplitude);
	fscanf(fp, "%f\n", &fOscillationFreq);
	fscanf(fp, "%d\n", &iOscillationDirection);
	fscanf(fp, "%d\n", &bBehavior);
	fscanf(fp, "%d\n", &iMaxDisplayDuration);
	fscanf(fp, "%d\n", &iJitterX);
	fscanf(fp, "%d\n", &iJitterY);
	fscanf(fp, "%d\n", &bAvoidZero);
	fscanf(fp, "%d\n", &iRedPrimary);
	fscanf(fp, "%d\n", &iGreenPrimary);
	fscanf(fp, "%d\n", &iBluePrimary);
	fscanf(fp, "%d\n", &iRedSecondary);
	fscanf(fp, "%d\n", &iGreenSecondary);
	fscanf(fp, "%d\n", &iBlueSecondary);
	fscanf(fp, "%d\n", &iColorSetting);
	fscanf(fp, "%d\n", &iNumStepsY);
	fscanf(fp, "%d\n", &iPointType);
	fscanf(fp, "%d\n", &bSequentialOrder);

	fscanf(fp, "%d\n", &m_displayNoiseBg);
	fscanf(fp, "%d\n", &m_sameBgTrial);
	fscanf(fp, "%d\n", &m_alternateNoise);
	fscanf(fp, "%d\n", &m_occludeBg);
	fscanf(fp, "%d\n", &m_noiseRed);
	fscanf(fp, "%d\n", &m_noiseGreen);
	fscanf(fp, "%d\n", &m_noiseBlue);
	fscanf(fp, "%d\n", &m_noisePercent);
	fscanf(fp, "%d\n", &m_noiseSize);

	fclose(fp);
}

void CSaccade::SaveFile(bool bDefault) 
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
	
	fprintf(fp, "%d\n", bReturnToPt);
	fprintf(fp, "%d\n", bUseAllDirs);
	fprintf(fp, "%d\n", iDisplayDuration);
	fprintf(fp, "%f\n", fDotSize);
	fprintf(fp, "%f\n", fPointX);
	fprintf(fp, "%f\n", fPointY);
	fprintf(fp, "%f\n", fStepSize);
	fprintf(fp, "%d\n", iNumStepsX);
	fprintf(fp, "%d\n", bStepHorizontal);
	fprintf(fp, "%d\n", bStepVertical);
	fprintf(fp, "%d\n", bStep45Deg);
	fprintf(fp, "%d\n", bStep135Deg);
	fprintf(fp, "%d\n", iDistType);
	fprintf(fp, "%d\n", bRandomDurations);
	fprintf(fp, "%d\n", iMinDuration);
	fprintf(fp, "%d\n", iMaxDuration);
	fprintf(fp, "%d\n", bEnableOscillations);
	fprintf(fp, "%f\n", fOscillationAmplitude);
	fprintf(fp, "%f\n", fOscillationFreq);
	fprintf(fp, "%d\n", iOscillationDirection);
	fprintf(fp, "%d\n", bBehavior);
	fprintf(fp, "%d\n", iMaxDisplayDuration);
	fprintf(fp, "%d\n", iJitterX);
	fprintf(fp, "%d\n", iJitterY);
	fprintf(fp, "%d\n", bAvoidZero);
	fprintf(fp, "%d\n", iRedPrimary);
	fprintf(fp, "%d\n", iGreenPrimary);
	fprintf(fp, "%d\n", iBluePrimary);
	fprintf(fp, "%d\n", iRedSecondary);
	fprintf(fp, "%d\n", iGreenSecondary);
	fprintf(fp, "%d\n", iBlueSecondary);
	fprintf(fp, "%d\n", iColorSetting);
	fprintf(fp, "%d\n", iNumStepsY);
	fprintf(fp, "%d\n", iPointType);
	fprintf(fp, "%d\n", bSequentialOrder);

	fprintf(fp, "%d\n", m_displayNoiseBg);
	fprintf(fp, "%d\n", m_sameBgTrial);
	fprintf(fp, "%d\n", m_alternateNoise);
	fprintf(fp, "%d\n", m_occludeBg);
	fprintf(fp, "%d\n", m_noiseRed);
	fprintf(fp, "%d\n", m_noiseGreen);
	fprintf(fp, "%d\n", m_noiseBlue);
	fprintf(fp, "%d\n", m_noisePercent);
	fprintf(fp, "%d\n", m_noiseSize);

	fclose(fp);
}

void CSaccade::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);
	
	sprintf(buffer, "Saccade;DotSize;%.4f;%ld|0x%02x/\0", fDotSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Saccade;StepSize;%.4f;%ld|0x%02x/\0", fStepSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Saccade;NumStepsX;%d;%ld|0x%02x/\0", iNumStepsX, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Saccade;NumStepsY;%d;%ld|0x%02x/\0", iNumStepsY, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Saccade;BlankTime;%d;%ld|0x%02x/\0", iBlankTime, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Saccade;Behavior;%d;%ld|0x%02x/\0", bBehavior, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Saccade;DisplayDuration;%d;%ld|0x%02x/\0", iDisplayDuration, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Saccade;ReturnToPt;%d;%ld|0x%02x/\0", bReturnToPt, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Saccade;PointX;%.4f;%ld|0x%02x/\0", fPointX, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Saccade;PointY;%.4f;%ld|0x%02x/\0", fPointY, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Saccade;SequentialOrder;%d;%ld|0x%02x/\0", bSequentialOrder, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}
