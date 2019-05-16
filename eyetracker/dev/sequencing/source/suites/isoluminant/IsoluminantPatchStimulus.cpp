/**
 * @file IsoluminantPatchStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "IsoluminantPatchStimulus.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	VSG IN 1:	0x002	correct/incorrect
	VSG IN 2:	0x004	next stim

	VSG Out 0:	repeat/unique
	VSG Out 1:	behavior count timer
	VSG Out 2:	f-noise
	VSG Out 3:	trial end
	VSG Out 4:	trial start

	Analog Out 0:	Horizontal fixation target location
	Analog Out 1:	Vertical fixation target location
	Analog Out 4:	luminance
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CIsoluminantPatchStimulus::CIsoluminantPatchStimulus() 
{
	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();

	paletteColors = new VSGTRIVAL[3];
	iFixationLoc = 0;
	iPage = 0;
	bUpdatePosition = true;

	Initialize();
}

CIsoluminantPatchStimulus::~CIsoluminantPatchStimulus() 
{
	if (pRepeatSequence) free(pRepeatSequence);
	if (pUniqueSequences) {
		for (int i = 0; i < iNumUniques; i++) {
			if (pUniqueSequences[i]) {
				free(pUniqueSequences[i]);
				pUniqueSequences[i] = NULL;
			}
		}
		free(pUniqueSequences);
	}

	if (bUniquesComplete) free(bUniquesComplete);
	
	delete [] paletteColors;

	pRepeatSequence = NULL;
	pUniqueSequences = NULL;
}

bool CIsoluminantPatchStimulus::Initialize() 
{
	bEndThread=false;
	iLoopNumber = 0;

	pRepeatSequence = NULL;
	pUniqueSequences = NULL;
			
	int CheckCard;
 	
	//Initialise the vsg card then check that it initialised O.K.
	if (!g_bIsInitialized) {
		CheckCard = vsgInit("");
		if (CheckCard < 0) return false;
		g_bIsInitialized = true;
		CheckCard=vsgSetVideoMode(vsgPANSCROLLMODE);
	}

	vsgMoveScreen(0,0);
	vsgSetCommand(vsgPALETTECLEAR);	

	double dFrameTime = vsgGetSystemAttribute(vsgFRAMETIME);
	g_dFramerate = 1000000.0 / dFrameTime;
	g_dHalfFrameInterval = DWORD(floor(dFrameTime / 2000.0f));
	g_dSleepThreshold = (double)g_dHalfFrameInterval / 2.0f;
	g_dFramerate = 1000000.0f / vsgGetSystemAttribute(vsgFRAMETIME);

	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();

	// set the view distance
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);	

	iSequenceNumber = 0;
	iSequenceLength = 512;

	// read input file
	ReadInput();

	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	UpdatePosition();
			
	return true;
}

bool CIsoluminantPatchStimulus::DisplayStimulus() 
{
	VSGLUTBUFFER lutValues;

	// set target voltages to fixation point
	if (iFixationLoc != 0) {
		etu::AnalogOut(0, (g_pIsoluminantPatch->GetFixationPointX() + g_pSettings->getXOffset()) / g_pSettings->getDegPerVolt());
		etu::AnalogOut(1, (g_pIsoluminantPatch->GetFixationPointY() + g_pSettings->getYOffset()) / g_pSettings->getDegPerVolt());

	} 
	else {
		etu::AnalogOut(0, (g_pIsoluminantPatch->GetFixationPointX2() + g_pSettings->getXOffset()) / g_pSettings->getDegPerVolt());
		etu::AnalogOut(1, (g_pIsoluminantPatch->GetFixationPointY2() + g_pSettings->getYOffset()) / g_pSettings->getDegPerVolt());

	}

	bool bUnique = false;
	int iUniqueNumber = 0;
	
	int* pLuminanceValues;
	double* pLuminanceValuesDouble;

	pLuminanceValuesDouble = (double*)malloc(iSequenceLength * sizeof(double));

	if (g_pIsoluminantPatch->isFNoise() && iSequenceNumber % 2 == 0 && iNumUniquesComplete < iNumUniques && !g_pIsoluminantPatch->RepeatsOnly()) {
		bUnique = true;
		iUniqueNumber = 0;
		while ((iUniqueNumber < iNumUniques) && bUniquesComplete[iUniqueNumber]) {
			iUniqueNumber++;
		}
		pLuminanceValues = pUniqueSequences[iUniqueNumber];
		vsgIOWriteDigitalOut(0xFFFF, vsgDIG1);
	} 
	else {
		pLuminanceValues = pRepeatSequence;
		vsgIOWriteDigitalOut(0x0000, vsgDIG1);
	}
	
	// tell the 1401 whether this is an FNOISE
	if (g_pIsoluminantPatch->isFNoise()) {
        vsgIOWriteDigitalOut(0xFFFF, vsgDIG2);
	} 
	else {
		vsgIOWriteDigitalOut(0x0000, vsgDIG2);
	}
	
	// wait for trial start pulse
	int iTrialStart = 0;
	while (iTrialStart == 0) {
		iTrialStart = vsgIOReadDigitalIn() & 0x0004;
		Sleep(1);
		if (bEndThread) return true;
	}

	double luminance;
	double balance;
	double scale, offset, mid;
	scale = (g_pIsoluminantPatch->GetMaxLuminance() - g_pIsoluminantPatch->GetMinLuminance()) / 255.0f;
	offset = g_pIsoluminantPatch->GetMinLuminance() / 255.0f;
	mid = offset + (scale / 2);
	double meanLuminance = ((double)(g_pIsoluminantPatch->GetMeanLuminance())) / 255.0f;
	
	for (int i = 0; i < iSequenceLength; i++) {
		pLuminanceValuesDouble[i] = ((double)pLuminanceValues[i]) / 255.0f;
	}
	
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	// sync the trial on pulse with 1 frame before the start of the trial
	vsgFrameSync();
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG4);
	for (int i = 0; i < iSequenceLength; i++) {
		luminance = pLuminanceValuesDouble[i] * scale + offset;
		balance = mid - (luminance - mid);
		// set the lumiannce value for the next frame
		lutValues[0].a = luminance;
		lutValues[0].b = luminance;
		lutValues[0].c = luminance;

		lutValues[1].a = balance;
		lutValues[1].b = balance;
		lutValues[1].c = balance;

		// wait for frame sync
		vsgFrameSync();

		// change display
		vsgPaletteWrite(&lutValues, 0, 2);

		// update analog out
		etu::AnalogOut(4, (float)luminance * g_pIsoluminantPatch->GetVoltageRange() + g_pIsoluminantPatch->GetVoltageOffset());
	}
	// sync turning off the trial on pulse with one frame after the trial end.
	vsgFrameSync();
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG3);
	vsgIOWriteDigitalOut(0x0000, vsgDIG4);
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	vsgPaletteSet(0, 1, &(paletteColors[2]));
	etu::AnalogOut(4, (float)meanLuminance  * g_pIsoluminantPatch->GetVoltageRange() + g_pIsoluminantPatch->GetVoltageOffset());
	vsgIOWriteDigitalOut(0x0000, vsgDIG3);

	// wait for correct / incorrect pulse
	int time = 0;
	int response = 0;
	while ((time < 50) && (response < 1)) {
		response = vsgIOReadDigitalIn() & 0x0002;
		WaitUntil(1);
		time += 1;
	}

	free(pLuminanceValuesDouble);

	// keep track of corrects and incorrects;
	if (response > 0) {
		if (bUnique) {
			bUniquesComplete[iUniqueNumber] = true;
			iNumUniquesComplete++;
		} 
		else {
			iNumRepeatsComplete++;
		}

		g_pIsoluminantPatchDlg->UpdateStatus(iNumRepeatsComplete, iNumUniquesComplete);

		iSequenceNumber++;

		Sleep(1);
	}

	// end case
	if (g_pIsoluminantPatch->isFNoise()) {
        if (iNumUniquesComplete == iNumUniques && iNumRepeatsComplete >= iNumUniques && !g_pIsoluminantPatch->RepeatsOnly()) {
			bEndThread = true;
		}
	} 
	else {
		if (iNumRepeatsComplete >= g_pIsoluminantPatch->GetNumRepeats()) {
			bEndThread = true;
		}
	}
	
	UpdatePosition();
	return true;
}

void CIsoluminantPatchStimulus::DisplayStimulusCycles() {}

void CIsoluminantPatchStimulus::ReadInput() 
{
	// read file input
	FILE* fp = fopen(g_pIsoluminantPatch->GetFileName(), "rt");
	if (fp) {
		// read first value
		// sequence length
		float f;
		fscanf(fp, "%f\n", &f);
		int iNumLuminanceValues  = (int)f;

		if (pUniqueSequences) {
			for (int i = 0; i < iNumUniques; i++) {
				if (pUniqueSequences[i]) {
					free(pUniqueSequences[i]);
				}
			}
			free(pUniqueSequences);
		}

		if (pRepeatSequence) free(pRepeatSequence);
		if (bUniquesComplete) free(bUniquesComplete);
		
		if (g_pIsoluminantPatch->isFNoise()) {
			// for FNoise -- read in numUniques
			fscanf(fp, "%f\n", &f);
			iNumUniques = (int)f;
			
		    // allocate space for all arrays
			iSequenceLength = iNumLuminanceValues;
			pRepeatSequence = (int*)calloc(iSequenceLength, sizeof(int));;
			pUniqueSequences = (int**)calloc(iNumUniques, sizeof(int*));
			bUniquesComplete = (bool*)calloc(iNumUniques, sizeof(bool*));
			iNumRepeatsComplete = 0;
			iNumUniquesComplete = 0;
			for (int i = 0; i < iNumUniques; i++) {
				pUniqueSequences[i]  = (int*)calloc(iSequenceLength, sizeof(int));;
			}

			// read in the repeat sequence
			for (int i = 0; i < iSequenceLength; i++) {
				fscanf(fp, "%f\n", &f);
				pRepeatSequence[i] = (int)f;
			}
			// read in all the uniques
			for (int j = 0; j < iNumUniques; j++) {
				int i;
				for (i = 0; i < iSequenceLength; i++) {
					fscanf(fp, "%f\n", &f);
					pUniqueSequences[j][i] = (int)f;
				}
			}
		} 
		else {  /// is not fNoise
			iSequenceLength = iNumLuminanceValues;

			// allocate space
			pRepeatSequence = (int*)calloc(iSequenceLength, sizeof(int));
			iNumRepeatsComplete = 0;
			iNumUniquesComplete = 0;
			pUniqueSequences = NULL;
			bUniquesComplete = NULL;

			// read sequence
			for (int i = 0; i < iSequenceLength; i++) {
				fscanf(fp, "%f\n", &f);
				pRepeatSequence[i] = (int)f;
			}
		}
		fclose(fp);
	} 
	else {
		AfxMessageBox("Invalid Input File Name");
		bEndThread = true;
	}
}

void CIsoluminantPatchStimulus::GenerateLUT() {}

CString CIsoluminantPatchStimulus::StimulusType() 
{
	return "Isoluminant Patch Stimulus";
}

void CIsoluminantPatchStimulus::UpdatePosition() 
{
	for (int i = 0; i < 255; i++) {
		if (i < 10) {
			buff[i].a = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;
			buff[i].b = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;
			buff[i].c = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;
		} 
		else if (i > 250) {
			buff[i].a = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;
			buff[i].b = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;
			buff[i].c = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;
		} 
		else {
			buff[i].a = 1.0f;
			buff[i].b = 0;
			buff[i].c = 0;
		}
	}

	vsgSetBackgroundColour(&buff[vsgBACKGROUND]);
	vsgSetFixationColour(&buff[vsgFIXATION]);
	vsgLUTBUFFERWrite(0, &buff);
	vsgLUTBUFFERtoPalette(0);

	bUpdatePosition = false;
	paletteColors[0].a = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;
	paletteColors[0].b = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;
	paletteColors[0].c = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;

	paletteColors[1].a = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;
	paletteColors[1].b = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;
	paletteColors[1].c = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;

    double offsetX, offsetY;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset() + g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &offsetX);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenYOffset()  + g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &offsetY);

	vsgSetBackgroundColour(&paletteColors[0]);
	vsgSetDrawPage(vsgVIDEOPAGE, 0, vsgBACKGROUND);

	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix  /2, iScrHeightPix / 2);
	int iSide = 1;
	if (g_pIsoluminantPatch->useLeftHemifield()) {
		iSide = -1;
	}

	vsgSetPen1(0);
	vsgDrawRect(iSide * iScrWidthPix / 4+offsetX,0, iScrWidthPix / 2, iScrHeightPix);
	vsgPresent();

	vsgSetPen1(1);
	vsgDrawRect(-iSide * iScrWidthPix / 4 + offsetX,0, iScrWidthPix / 2, iScrHeightPix);
	vsgPresent();

	double fixationSize, fixationX, fixationY;
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pIsoluminantPatch->GetFixationSize(), vsgPIXELUNIT, &fixationSize);
	if (iFixationLoc == 0) {
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pIsoluminantPatch->GetFixationPointX() + g_pSettings->getScreenXOffset() + g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &fixationX);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pIsoluminantPatch->GetFixationPointY()  + g_pSettings->getScreenYOffset()  + g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &fixationY);
	} 
	else {
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pIsoluminantPatch->GetFixationPointX2() + g_pSettings->getScreenXOffset() + g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &fixationX);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pIsoluminantPatch->GetFixationPointY2()  + g_pSettings->getScreenYOffset()  + g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &fixationY);
	}

	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix  /2, iScrHeightPix / 2);
	
	vsgSetPen1(128);
	vsgDrawOval(fixationX, -fixationY, fixationSize * 2, fixationSize * 2);
	vsgPresent();

	paletteColors[2].a = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;
	paletteColors[2].b = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;
	paletteColors[2].c = ((double)g_pIsoluminantPatch->GetMeanLuminance())/255.0f;
	
	vsgPaletteSet(0, 0, &(paletteColors[0]));
	vsgPaletteSet(1, 1, &(paletteColors[1]));

	bUpdatePosition = false;
	iFixationLoc = 1 - iFixationLoc;
}

void CIsoluminantPatchStimulus::UpdatePositionFlag() 
{
	bUpdatePosition = true;
}

#endif // APP_EYE_TRACKING
