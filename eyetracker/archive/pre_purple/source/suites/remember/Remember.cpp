/**
 * @file Remember.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "Remember.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CRemember::CRemember()
{
	defaultFile = CString("C:\\Stimulus Settings\\Remember\\default.txt");
	
	iCenterColorRadio = COLOR_PRIMARY;
	iPeriphColorRadio = COLOR_PRIMARY;

	fCenterXPos = 0.0;
	fCenterYPos = 0.0;

	fStepSize = 5.0;
	iNumStepsH = 5;
	iNumStepsV = 4;

	fPeriphDotSize = 0.5;
	fCenterDotSize = 0.5;

	iFlashInterval = 3000;
	iFlashDuration = 250;

	iCenterPrimaryRed = 255;
	iCenterPrimaryGreen = 0;
	iCenterPrimaryBlue = 0;

	iCenterSecondaryRed = 0;
	iCenterSecondaryGreen = 0;
	iCenterSecondaryBlue = 255;

	iPeriphPrimaryRed = 0;
	iPeriphPrimaryGreen = 0;
	iPeriphPrimaryBlue = 255;

	iPeriphSecondaryRed = 255;
	iPeriphSecondaryGreen = 0;
	iPeriphSecondaryBlue = 0;

	bBehaviorCheck=false;

	bStepVert = true;
	bStepHoriz = true;
	bStep45 = true;
	bStep135 = true;

	iDistType = DIST_TYPE_RADIAL;
	iAlternateBlockSize = 1;
	iMaxDelay=8000;

	iStimType = STIM_PERIPHERAL_FLASH;
	iPostFlashDelay = 500;
	iIntertrialDelay = 1000;

	bAvoidZero=false;
	bRedBlue=false;

	bRandomizeFlashInterval = true;
	bRandomizePostFlash = false;
	iPostFlashRandMax = 4000;
	iPostFlashRandMin = 1000;
	iFlashIntervalRandMax = 4000;
	iFlashIntervalRandMin = 1000;

	fPeriphXPos = 0;
	fPeriphYPos = 0;
	fPeriphXRadius = 10;
	fPeriphYRadius = 10;
	iPctTimeAtPeriph = 0;

	bSeqRandom=false;

	iPeriphTertiaryRed=255;
	iPeriphTertiaryGreen=0;
	iPeriphTertiaryBlue=255;
	iNoiseRed=100;
	iNoiseGreen=0;
	iNoiseBlue=100;
	iNumRandomRepeats=5;
	bUseNoise=true;
	iNumCatchTrials=10;

	bGridSteps=true;
	iNoiseSize=2;
	iCrossPix=1;
	iNumBlankTrials=10;
	iNoisePct=50;
	bUseDim=false;
	iDimOnsetMin=0;
	iDimOnsetMax=1000;
	iDimTimeout=3000;
	iAnswerPeriod=1000;
	iDimBrightness=50;
	bHideCenter=false;
	bInterleaveNoise=false;
	bCenterPoints=false;
	bOffAxis=false;
	bResponse=false;
	fPeriphXPosL=0;
	fPeriphYPosL=0;
	iResponseTimeout=5000;
	bRampUp=false;
	iRampDuration=200;
	iNumCatchTrials2=5;
	iPctTimeAtCenter=5;
	fCenterXRadius=2.5;
	fCenterYRadius=2.5;
	bAutoPosition = true;

	LoadFile(true);

	pLocationsX=NULL;
	pLocationsY=NULL;
	pLocationsType=NULL;
	pLocationsIndex=NULL;
	pLocationsClass=NULL;
	GenerateLocations2();
}

CRemember::~CRemember() {}

void CRemember::LoadFile(bool bDefault) 
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

	fscanf(fp, "%f\n", &fCenterXPos);
	fscanf(fp, "%f\n", &fCenterYPos);
	fscanf(fp, "%f\n", &fStepSize);
	fscanf(fp, "%d\n", &iNumStepsH);
	fscanf(fp, "%d\n", &iNumStepsV);
	fscanf(fp, "%f\n", &fPeriphDotSize);
	fscanf(fp, "%f\n", &fCenterDotSize);
	fscanf(fp, "%d\n", &iFlashInterval);
	fscanf(fp, "%d\n", &iFlashDuration);
	fscanf(fp, "%d\n", &iCenterColorRadio);
	fscanf(fp, "%d\n", &iPeriphColorRadio);
	fscanf(fp, "%d\n", &iCenterPrimaryRed);
	fscanf(fp, "%d\n", &iCenterPrimaryGreen);
	fscanf(fp, "%d\n", &iCenterPrimaryBlue);
	fscanf(fp, "%d\n", &iCenterSecondaryRed);
	fscanf(fp, "%d\n", &iCenterSecondaryGreen);
	fscanf(fp, "%d\n", &iCenterSecondaryBlue);
	fscanf(fp, "%d\n", &iPeriphPrimaryRed);
	fscanf(fp, "%d\n", &iPeriphPrimaryGreen);
	fscanf(fp, "%d\n", &iPeriphPrimaryBlue);
	fscanf(fp, "%d\n", &iPeriphSecondaryRed);
	fscanf(fp, "%d\n", &iPeriphSecondaryGreen);
	fscanf(fp, "%d\n", &iPeriphSecondaryBlue);
	fscanf(fp, "%d\n", &bBehaviorCheck);
	fscanf(fp, "%d\n", &bStepHoriz);
	fscanf(fp, "%d\n", &bStepVert);
	fscanf(fp, "%d\n", &bStep45);
	fscanf(fp, "%d\n", &bStep135);
	fscanf(fp, "%d\n", &iDistType);
	fscanf(fp, "%d\n", &iAlternateBlockSize);
	fscanf(fp, "%d\n", &iMaxDelay);
	fscanf(fp, "%d\n", &iStimType);
	fscanf(fp, "%d\n", &iPostFlashDelay);
	fscanf(fp, "%d\n", &iIntertrialDelay);
	fscanf(fp, "%d\n", &bAvoidZero);
	fscanf(fp, "%d\n", &bRedBlue);
	fscanf(fp, "%d\n", &bRandomizeFlashInterval);
	fscanf(fp, "%d\n", &bRandomizePostFlash);
	fscanf(fp, "%d\n", &iPostFlashRandMax);
	fscanf(fp, "%d\n", &iPostFlashRandMin);
	fscanf(fp, "%d\n", &iFlashIntervalRandMax);
	fscanf(fp, "%d\n", &iFlashIntervalRandMin);

	fscanf(fp, "%f\n", &fPeriphXPos);
	fscanf(fp, "%f\n", &fPeriphYPos);
	fscanf(fp, "%f\n", &fPeriphXRadius);
	fscanf(fp, "%f\n", &fPeriphYRadius);
	fscanf(fp, "%d\n", &iPctTimeAtPeriph);

	fscanf(fp, "%d\n", &bSeqRandom);

	fscanf(fp, "%d\n", &iPeriphTertiaryRed);
	fscanf(fp, "%d\n", &iPeriphTertiaryGreen);
	fscanf(fp, "%d\n", &iPeriphTertiaryBlue);
	fscanf(fp, "%d\n", &iNoiseRed);
	fscanf(fp, "%d\n", &iNoiseGreen);
	fscanf(fp, "%d\n", &iNoiseBlue);
	fscanf(fp, "%d\n", &iNumRandomRepeats);
	fscanf(fp, "%d\n", &bUseNoise);
	fscanf(fp, "%d\n", &iNumCatchTrials);

	fscanf(fp, "%d\n", &bGridSteps);
	fscanf(fp, "%d\n", &iNoiseSize);
	fscanf(fp, "%d\n", &iCrossPix);
	fscanf(fp, "%d\n", &iNumBlankTrials);
	fscanf(fp, "%d\n", &iNoisePct);
	fscanf(fp, "%d\n", &bUseDim);
	fscanf(fp, "%d\n", &iDimOnsetMin);
	fscanf(fp, "%d\n", &iDimOnsetMax);
	fscanf(fp, "%d\n", &iDimTimeout);
	fscanf(fp, "%d\n", &iAnswerPeriod);
	fscanf(fp, "%d\n", &iDimBrightness);
	fscanf(fp, "%d\n", &bHideCenter);
	fscanf(fp, "%d\n", &bInterleaveNoise);
	fscanf(fp, "%d\n", &bCenterPoints);
	fscanf(fp, "%d\n", &bOffAxis);
	fscanf(fp, "%d\n", &bResponse);
	fscanf(fp, "%f\n", &fPeriphXPosL);
	fscanf(fp, "%f\n", &fPeriphYPosL);
	fscanf(fp, "%d\n", &iResponseTimeout);
	fscanf(fp, "%d\n", &bRampUp);
	fscanf(fp, "%d\n", &iRampDuration);
	fscanf(fp, "%d\n", &iNumCatchTrials2);
	fscanf(fp, "%d\n", &iPctTimeAtCenter);
	fscanf(fp, "%f\n", &fCenterXRadius);
	fscanf(fp, "%f\n", &fCenterYRadius);
	fscanf(fp, "%d\n", &bAutoPosition);

	fclose(fp);
}

void CRemember::SaveFile(bool bDefault) 
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
	
	fprintf(fp, "%f\n", fCenterXPos);
	fprintf(fp, "%f\n", fCenterYPos);
	fprintf(fp, "%f\n", fStepSize);
	fprintf(fp, "%d\n", iNumStepsH);
	fprintf(fp, "%d\n", iNumStepsV);
	fprintf(fp, "%f\n", fPeriphDotSize);
	fprintf(fp, "%f\n", fCenterDotSize);
	fprintf(fp, "%d\n", iFlashInterval);
	fprintf(fp, "%d\n", iFlashDuration);
	fprintf(fp, "%d\n", iCenterColorRadio);
	fprintf(fp, "%d\n", iPeriphColorRadio);
	fprintf(fp, "%d\n", iCenterPrimaryRed);
	fprintf(fp, "%d\n", iCenterPrimaryGreen);
	fprintf(fp, "%d\n", iCenterPrimaryBlue);
	fprintf(fp, "%d\n", iCenterSecondaryRed);
	fprintf(fp, "%d\n", iCenterSecondaryGreen);
	fprintf(fp, "%d\n", iCenterSecondaryBlue);
	fprintf(fp, "%d\n", iPeriphPrimaryRed);
	fprintf(fp, "%d\n", iPeriphPrimaryGreen);
	fprintf(fp, "%d\n", iPeriphPrimaryBlue);
	fprintf(fp, "%d\n", iPeriphSecondaryRed);
	fprintf(fp, "%d\n", iPeriphSecondaryGreen);
	fprintf(fp, "%d\n", iPeriphSecondaryBlue);
	fprintf(fp, "%d\n", bBehaviorCheck);
	fprintf(fp, "%d\n", bStepHoriz);
	fprintf(fp, "%d\n", bStepVert);
	fprintf(fp, "%d\n", bStep45);
	fprintf(fp, "%d\n", bStep135);
	fprintf(fp, "%d\n", iDistType);
	fprintf(fp, "%d\n", iAlternateBlockSize);
	fprintf(fp, "%d\n", iMaxDelay);
	fprintf(fp, "%d\n", iStimType);
	fprintf(fp, "%d\n", iPostFlashDelay);
	fprintf(fp, "%d\n", iIntertrialDelay);
	fprintf(fp, "%d\n", bAvoidZero);
	fprintf(fp, "%d\n", bRedBlue);
	fprintf(fp, "%d\n", bRandomizeFlashInterval);
	fprintf(fp, "%d\n", bRandomizePostFlash);
	fprintf(fp, "%d\n", iPostFlashRandMax);
	fprintf(fp, "%d\n", iPostFlashRandMin);
	fprintf(fp, "%d\n", iFlashIntervalRandMax);
	fprintf(fp, "%d\n", iFlashIntervalRandMin);

	fprintf(fp, "%f\n", fPeriphXPos);
	fprintf(fp, "%f\n", fPeriphYPos);
	fprintf(fp, "%f\n", fPeriphXRadius);
	fprintf(fp, "%f\n", fPeriphYRadius);
	fprintf(fp, "%d\n", iPctTimeAtPeriph);

	fprintf(fp, "%d\n", bSeqRandom);

	fprintf(fp, "%d\n", iPeriphTertiaryRed);
	fprintf(fp, "%d\n", iPeriphTertiaryGreen);
	fprintf(fp, "%d\n", iPeriphTertiaryBlue);
	fprintf(fp, "%d\n", iNoiseRed);
	fprintf(fp, "%d\n", iNoiseGreen);
	fprintf(fp, "%d\n", iNoiseBlue);
	fprintf(fp, "%d\n", iNumRandomRepeats);
	fprintf(fp, "%d\n", bUseNoise);
	fprintf(fp, "%d\n", iNumCatchTrials);

	fprintf(fp, "%d\n", bGridSteps);
	fprintf(fp, "%d\n", iNoiseSize);
	fprintf(fp, "%d\n", iCrossPix);
	fprintf(fp, "%d\n", iNumBlankTrials);
	fprintf(fp, "%d\n", iNoisePct);
	fprintf(fp, "%d\n", bUseDim);
	fprintf(fp, "%d\n", iDimOnsetMin);
	fprintf(fp, "%d\n", iDimOnsetMax);
	fprintf(fp, "%d\n", iDimTimeout);
	fprintf(fp, "%d\n", iAnswerPeriod);
	fprintf(fp, "%d\n", iDimBrightness);
	fprintf(fp, "%d\n", bHideCenter);
	fprintf(fp, "%d\n", bInterleaveNoise);
	fprintf(fp, "%d\n", bCenterPoints);
	fprintf(fp, "%d\n", bOffAxis);
	fprintf(fp, "%d\n", bResponse);
	fprintf(fp, "%f\n", fPeriphXPosL);
	fprintf(fp, "%f\n", fPeriphYPosL);
	fprintf(fp, "%d\n", iResponseTimeout);
	fprintf(fp, "%d\n", bRampUp);
	fprintf(fp, "%d\n", iRampDuration);
	fprintf(fp, "%d\n", iNumCatchTrials2);
	fprintf(fp, "%d\n", iPctTimeAtCenter);
	fprintf(fp, "%f\n", fCenterXRadius);
	fprintf(fp, "%f\n", fCenterYRadius);
	fprintf(fp, "%d\n", bAutoPosition);
	
	fclose(fp);
}

void CRemember::GenerateLocations() 
{
	int iNumDirections = 0;
	int iCurrLocation = 0;
	int iNumStepsSmall = (iNumStepsV < iNumStepsH)? iNumStepsV : iNumStepsH;
	
	// calculate number of directions used
	
	iNumLocations=0;
	if (bStepHoriz) iNumLocations += iNumStepsH*2;
	if (bStepVert) iNumLocations += iNumStepsV*2;
	if (bStep45) iNumLocations += iNumStepsSmall*2;
	if (bStep135) iNumLocations += iNumStepsSmall*2;

	//grid?
	if (bOffAxis) {
		if (bGridSteps) iNumLocations = (iNumStepsH*2) * (iNumStepsV*2);
	} 
	else {
		if (bGridSteps) iNumLocations = (iNumStepsH*2+1) * (iNumStepsV*2+1) -1;
	}

	//don't include center point
	if (!bAvoidZero && !bOffAxis) iNumLocations++;
	//include extra points around center fovea?
	if (bCenterPoints) iNumLocations += 4;
	
	// free the memory if necessary
	if (pLocationsX) free(pLocationsX);
	if (pLocationsY) free(pLocationsY);
	if (pLocationsType) free(pLocationsType);
	if (pLocationsIndex) free(pLocationsIndex);

	// allocate the necessary memory
	pLocationsX = (double*)calloc(iNumLocations, sizeof(double));
	pLocationsY = (double*)calloc(iNumLocations, sizeof(double));

	int numCombos=(iCenterColorRadio==COLOR_PRIMARY || iCenterColorRadio==COLOR_SECONDARY)? 1 : 2;
	numCombos *= (iPeriphColorRadio==COLOR_ALTERNATE)? 2 : 1;
	int iNumDisplays = iNumRandomRepeats*(iNumLocations*numCombos + ((iPeriphColorRadio==COLOR_RANDOM)? iNumCatchTrials+iNumBlankTrials : 0));
	int loc_com_catch = iNumDisplays/iNumRandomRepeats;//=locations*combos+catch_trials
	pLocationsType = (int*)calloc(iNumDisplays, sizeof(int));
	pLocationsIndex = (int*)calloc(iNumDisplays, sizeof(int));
	
	//for each repeat, fill in randomized locations
	for (int r=0; r<iNumRandomRepeats; r++) {
		int *pPriPri = (int*)calloc(iNumLocations, sizeof(int));
		int ipp=0;
		int *pPriSec = (int*)calloc(iNumLocations, sizeof(int));
		int ips=0;
		int *pPriTer = (int*)calloc(iNumLocations, sizeof(int));
		int ipt=0;
		int *pSecPri = (int*)calloc(iNumLocations, sizeof(int));
		int isp=0;
		int *pSecSec = (int*)calloc(iNumLocations, sizeof(int));
		int iss=0;
		int *pSecTer = (int*)calloc(iNumLocations, sizeof(int));
		int ist=0;
		int *pPriBnk = (int*)calloc(iNumLocations, sizeof(int));
		int ipb=0;
		int *pSecBnk = (int*)calloc(iNumLocations, sizeof(int));
		int isb=0;
		int iBlank=0;
		int iCatch=0;
		//generate location lists
		for (int i=0; i<iNumLocations; i++) {
			pPriPri[i]=i;
			pPriSec[i]=i;
			pPriTer[i]=i;
			pPriBnk[i]=i;
			pSecPri[i]=i;
			pSecSec[i]=i;
			pSecTer[i]=i;
			pSecBnk[i]=i;
		}
		//randomize location lists
		for (int i=0; i<iNumLocations; i++) {
			//swap random location for each combo

			int randindex=rand()%(iNumLocations);
			int temp = pPriPri[randindex];
			pPriPri[randindex] = pPriPri[i];
			pPriPri[i]=temp;

			randindex=rand()%(iNumLocations);
			temp = pPriSec[randindex];
			pPriSec[randindex] = pPriSec[i];
			pPriSec[i]=temp;

			randindex=rand()%(iNumLocations);
			temp = pPriTer[randindex];
			pPriTer[randindex] = pPriTer[i];
			pPriTer[i]=temp;

			randindex=rand()%(iNumLocations);
			temp = pSecPri[randindex];
			pSecPri[randindex] = pSecPri[i];
			pSecPri[i]=temp;

			randindex=rand()%(iNumLocations);
			temp = pSecSec[randindex];
			pSecSec[randindex] = pSecSec[i];
			pSecSec[i]=temp;

			randindex=rand()%(iNumLocations);
			temp = pSecTer[randindex];
			pSecTer[randindex] = pSecTer[i];
			pSecTer[i]=temp;

			randindex=rand()%(iNumLocations);
			temp = pPriBnk[randindex];
			pPriBnk[randindex] = pPriBnk[i];
			pPriBnk[i]=temp;

			randindex=rand()%(iNumLocations);
			temp = pSecBnk[randindex];
			pSecBnk[randindex] = pSecBnk[i];
			pSecBnk[i]=temp;

		}
		//fill in thing
		for (int i=0; i<loc_com_catch; i++) {
			switch(iCenterColorRadio) {
				case COLOR_PRIMARY:
					switch(iPeriphColorRadio) {
						case COLOR_PRIMARY:
							pLocationsType[r*loc_com_catch+i]=0;
							pLocationsIndex[r*loc_com_catch+i]=pPriPri[ipp%iNumLocations]; ipp++;
							break;
						case COLOR_SECONDARY:
							pLocationsType[r*loc_com_catch+i]=1;
							pLocationsIndex[r*loc_com_catch+i]=pPriSec[ips%iNumLocations]; ips++;
							break;
						case COLOR_TERTIARY:
							pLocationsType[r*loc_com_catch+i]=4;
							pLocationsIndex[r*loc_com_catch+i]=pPriTer[ipt%iNumLocations]; ipt++;
							break;
						case COLOR_ALTERNATE:
							if ((i/iAlternateBlockSize)%2==0) {
								pLocationsType[r*loc_com_catch+i]=0;
								pLocationsIndex[r*loc_com_catch+i]=pPriPri[ipp%iNumLocations]; ipp++;
							} 
							else {
								pLocationsType[r*loc_com_catch+i]=1;
								pLocationsIndex[r*loc_com_catch+i]=pPriSec[ips%iNumLocations]; ips++;
							}
							break;
						case COLOR_RANDOM:
							if (  (iCatch < iNumCatchTrials) && (rand()%loc_com_catch<iNumCatchTrials) ) {
								//random catch trial!
								if (rand()%2==0) {
									pLocationsType[r*loc_com_catch+i]=0;
									pLocationsIndex[r*loc_com_catch+i]=pPriPri[ipp%iNumLocations]; ipp++;
								} 
								else {
									pLocationsType[r*loc_com_catch+i]=1;
									pLocationsIndex[r*loc_com_catch+i]=pPriSec[ips%iNumLocations]; ips++;
								}
								iCatch++;
							} 
							else if (  (iBlank < iNumBlankTrials) && (rand()%loc_com_catch<iNumBlankTrials) ) {
								//random blank catch trial
								pLocationsType[r*loc_com_catch+i]=6;
								pLocationsIndex[r*loc_com_catch+i]=pPriBnk[ipb%iNumLocations]; ipb++;
								iBlank++;
							} 
							else {
								//"purple" as usual
								pLocationsType[r*loc_com_catch+i]=4;
								pLocationsIndex[r*loc_com_catch+i]=pPriTer[ipt%iNumLocations]; ipt++;
							}
							break;
					}
					break;
				case COLOR_SECONDARY:
					switch(iPeriphColorRadio) {
						case COLOR_PRIMARY:
							pLocationsType[r*loc_com_catch+i]=2;
							pLocationsIndex[r*loc_com_catch+i]=pSecPri[isp%iNumLocations]; isp++;
							break;
						case COLOR_SECONDARY:
							pLocationsType[r*loc_com_catch+i]=3;
							pLocationsIndex[r*loc_com_catch+i]=pSecSec[iss%iNumLocations]; iss++;
							break;
						case COLOR_TERTIARY:
							pLocationsType[r*loc_com_catch+i]=5;
							pLocationsIndex[r*loc_com_catch+i]=pSecTer[ist%iNumLocations]; ist++;
							break;
						case COLOR_ALTERNATE:
							if (i%2==0) {
								pLocationsType[r*loc_com_catch+i]=2;
								pLocationsIndex[r*loc_com_catch+i]=pSecPri[isp%iNumLocations]; isp++;
							} 
							else {
								pLocationsType[r*loc_com_catch+i]=3;
								pLocationsIndex[r*loc_com_catch+i]=pSecSec[iss%iNumLocations]; iss++;
							}
							break;
						case COLOR_RANDOM:
							if (  (iCatch < iNumCatchTrials) && (rand()%loc_com_catch<iNumCatchTrials) ) {
								//random catch trial!
								if (rand()%2==0) {
									pLocationsType[r*loc_com_catch+i]=2;
									pLocationsIndex[r*loc_com_catch+i]=pSecPri[isp%iNumLocations]; isp++;
								} 
								else {
									pLocationsType[r*loc_com_catch+i]=3;
									pLocationsIndex[r*loc_com_catch+i]=pSecSec[iss%iNumLocations]; iss++;
								}
								iCatch++;
							} 
							else if (  (iBlank < iNumBlankTrials) && (rand()%loc_com_catch<iNumBlankTrials) ) {
								//random blank catch trial
								pLocationsType[r*loc_com_catch+i]=7;
								pLocationsIndex[r*loc_com_catch+i]=pSecBnk[isb%iNumLocations]; isb++;
								iBlank++;
							} 
							else {
								//"purple" as usual
								pLocationsType[r*loc_com_catch+i]=5;
								pLocationsIndex[r*loc_com_catch+i]=pSecTer[ist%iNumLocations]; ist++;
							}
							break;
					}
					break;
				case COLOR_ALTERNATE:
					switch(iPeriphColorRadio) {
						case COLOR_PRIMARY:
							if ((i/iAlternateBlockSize)%2==0) {
								pLocationsType[r*loc_com_catch+i]=0;
								pLocationsIndex[r*loc_com_catch+i]=pPriPri[ipp%iNumLocations]; ipp++;
							} 
							else {
								pLocationsType[r*loc_com_catch+i]=2;
								pLocationsIndex[r*loc_com_catch+i]=pSecPri[isp%iNumLocations]; isp++;
							}
							break;
						case COLOR_SECONDARY:
							if ((i/iAlternateBlockSize)%2==0) {
								pLocationsType[r*loc_com_catch+i]=1;
								pLocationsIndex[r*loc_com_catch+i]=pPriSec[ips%iNumLocations]; ips++;
							} 
							else {
								pLocationsType[r*loc_com_catch+i]=3;
								pLocationsIndex[r*loc_com_catch+i]=pSecSec[iss%iNumLocations]; iss++;
							}
							break;
						case COLOR_TERTIARY:
							if ((i/iAlternateBlockSize)%2==0) {
								pLocationsType[r*loc_com_catch+i]=4;
								pLocationsIndex[r*loc_com_catch+i]=pPriTer[ipt%iNumLocations]; ipt++;
							} 
							else {
								pLocationsType[r*loc_com_catch+i]=5;
								pLocationsIndex[r*loc_com_catch+i]=pSecTer[ist%iNumLocations]; ist++;
							}
							break;
						case COLOR_ALTERNATE:
							if ((i/iAlternateBlockSize)%2==0) {
								if (i%2==0) {
									pLocationsType[r*loc_com_catch+i]=0;
									pLocationsIndex[r*loc_com_catch+i]=pPriPri[ipp%iNumLocations]; ipp++;
								} 
								else {
									pLocationsType[r*loc_com_catch+i]=1;
									pLocationsIndex[r*loc_com_catch+i]=pPriSec[ips%iNumLocations]; ips++;
								}
							} else {
								if (i%2==0) {
									pLocationsType[r*loc_com_catch+i]=2;
									pLocationsIndex[r*loc_com_catch+i]=pSecPri[isp%iNumLocations]; isp++;
								} 
								else {
									pLocationsType[r*loc_com_catch+i]=3;
									pLocationsIndex[r*loc_com_catch+i]=pSecSec[iss%iNumLocations]; iss++;
								}
							}
							break;
						case COLOR_RANDOM:
							if ((i/iAlternateBlockSize)%2==0) {
								if (  (iCatch < iNumCatchTrials) && (rand()%loc_com_catch<iNumCatchTrials) ) {
									//random catch trial!
									if (rand()%2==0) {
										pLocationsType[r*loc_com_catch+i]=0;
										pLocationsIndex[r*loc_com_catch+i]=pPriPri[ipp%iNumLocations]; ipp++;
									} 
									else {
										pLocationsType[r*loc_com_catch+i]=1;
										pLocationsIndex[r*loc_com_catch+i]=pPriSec[ips%iNumLocations]; ips++;
									}
									iCatch++;
								} 
								else if (  (iBlank < iNumBlankTrials) && (rand()%loc_com_catch<iNumBlankTrials) ) {
									//random blank catch trial
									pLocationsType[r*loc_com_catch+i]=6;
									pLocationsIndex[r*loc_com_catch+i]=pPriBnk[ipb%iNumLocations]; ipb++;
									iBlank++;
								} 
								else {
									//"purple" as usual
									pLocationsType[r*loc_com_catch+i]=4;
									pLocationsIndex[r*loc_com_catch+i]=pPriTer[ipt%iNumLocations]; ipt++;
								}
							} 
							else {
								if (  (iCatch < iNumCatchTrials) && (rand()%loc_com_catch<iNumCatchTrials) ) {
									//random catch trial!
									if (rand()%2==0) {
										pLocationsType[r*loc_com_catch+i]=2;
										pLocationsIndex[r*loc_com_catch+i]=pSecPri[isp%iNumLocations]; isp++;
									} 
									else {
										pLocationsType[r*loc_com_catch+i]=3;
										pLocationsIndex[r*loc_com_catch+i]=pSecSec[iss%iNumLocations]; iss++;
									}
									iCatch++;
								} 
								else if (  (iBlank < iNumBlankTrials) && (rand()%loc_com_catch<iNumBlankTrials) ) {
									//random blank catch trial
									pLocationsType[r*loc_com_catch+i]=7;
									pLocationsIndex[r*loc_com_catch+i]=pSecBnk[isb%iNumLocations]; isb++;
									iBlank++;
								} 
								else {
									//"purple" as usual
									pLocationsType[r*loc_com_catch+i]=5;
									pLocationsIndex[r*loc_com_catch+i]=pSecTer[ist%iNumLocations]; ist++;
								}
							}
							break;
					}
					break;
				case COLOR_RANDOM:
					switch(iPeriphColorRadio) {
						case COLOR_PRIMARY:
							if (rand()%2==0) {
								pLocationsType[r*loc_com_catch+i]=0;
								pLocationsIndex[r*loc_com_catch+i]=pPriPri[ipp%iNumLocations]; ipp++;
							} 
							else {
								pLocationsType[r*loc_com_catch+i]=2;
								pLocationsIndex[r*loc_com_catch+i]=pSecPri[isp%iNumLocations]; isp++;
							}
							break;
						case COLOR_SECONDARY:
							if (rand()%2==0) {
								pLocationsType[r*loc_com_catch+i]=1;
								pLocationsIndex[r*loc_com_catch+i]=pPriSec[ips%iNumLocations]; ips++;
							} 
							else {
								pLocationsType[r*loc_com_catch+i]=3;
								pLocationsIndex[r*loc_com_catch+i]=pSecSec[iss%iNumLocations]; iss++;
							}
							break;
						case COLOR_TERTIARY:
							if (rand()%2==0) {
								pLocationsType[r*loc_com_catch+i]=4;
								pLocationsIndex[r*loc_com_catch+i]=pPriTer[ipt%iNumLocations]; ipt++;
							} 
							else {
								pLocationsType[r*loc_com_catch+i]=5;
								pLocationsIndex[r*loc_com_catch+i]=pSecTer[ist%iNumLocations]; ist++;
							}
							break;
						case COLOR_ALTERNATE:
							if (rand()%2==0) {
								if (i%2==0) {
									pLocationsType[r*loc_com_catch+i]=0;
									pLocationsIndex[r*loc_com_catch+i]=pPriPri[ipp%iNumLocations]; ipp++;
								} 
								else {
									pLocationsType[r*loc_com_catch+i]=1;
									pLocationsIndex[r*loc_com_catch+i]=pPriSec[ips%iNumLocations]; ips++;
								}
							} 
							else {
								if (i%2==0) {
									pLocationsType[r*loc_com_catch+i]=2;
									pLocationsIndex[r*loc_com_catch+i]=pSecPri[isp%iNumLocations]; isp++;
								} 
								else {
									pLocationsType[r*loc_com_catch+i]=3;
									pLocationsIndex[r*loc_com_catch+i]=pSecSec[iss%iNumLocations]; iss++;
								}
							}
							break;
						case COLOR_RANDOM:
							if (rand()%2==0) {
								if (  (iCatch < iNumCatchTrials) && (rand()%loc_com_catch<iNumCatchTrials) ) {
									//random catch trial!
									if (rand()%2==0) {
										pLocationsType[r*loc_com_catch+i]=0;
										pLocationsIndex[r*loc_com_catch+i]=pPriPri[ipp%iNumLocations]; ipp++;
									} 
									else {
										pLocationsType[r*loc_com_catch+i]=1;
										pLocationsIndex[r*loc_com_catch+i]=pPriSec[ips%iNumLocations]; ips++;
									}
									iCatch++;
								} 
								else if (  (iBlank < iNumBlankTrials) && (rand()%loc_com_catch<iNumBlankTrials) ) {
									//random blank catch trial
									pLocationsType[r*loc_com_catch+i]=6;
									pLocationsIndex[r*loc_com_catch+i]=pPriBnk[ipb%iNumLocations]; ipb++;
									iBlank++;
								} 
								else {
									//"purple" as usual
									pLocationsType[r*loc_com_catch+i]=4;
									pLocationsIndex[r*loc_com_catch+i]=pPriTer[ipt%iNumLocations]; ipt++;
								}
							} 
							else {
								if (  (iCatch < iNumCatchTrials) && (rand()%loc_com_catch<iNumCatchTrials) ) {
									//random catch trial!
									if (rand()%2==0) {
										pLocationsType[r*loc_com_catch+i]=2;
										pLocationsIndex[r*loc_com_catch+i]=pSecPri[isp%iNumLocations]; isp++;
									} 
									else {
										pLocationsType[r*loc_com_catch+i]=3;
										pLocationsIndex[r*loc_com_catch+i]=pSecSec[iss%iNumLocations]; iss++;
									}
									iCatch++;
								} 
								else if (  (iBlank < iNumBlankTrials) && (rand()%loc_com_catch<iNumBlankTrials) ) {
									//random blank catch trial
									pLocationsType[r*loc_com_catch+i]=7;
									pLocationsIndex[r*loc_com_catch+i]=pSecBnk[isb%iNumLocations]; isb++;
									iBlank++;
								} 
								else {
									//"purple" as usual
									pLocationsType[r*loc_com_catch+i]=5;
									pLocationsIndex[r*loc_com_catch+i]=pSecTer[ist%iNumLocations]; ist++;
								}
							}
							break;
					}
					break;
			}
		}
		//clean up
		free(pPriPri);
		free(pPriSec);
		free(pPriTer);
		free(pSecPri);
		free(pSecSec);
		free(pSecTer);
		free(pPriBnk);
		free(pSecBnk);
	}

	iCurrLocation=0;

	if (!bAvoidZero && !bOffAxis) {
		pLocationsX[0] = 0;
		pLocationsY[0] = 0;
		iCurrLocation++;
	}

	if (bCenterPoints) {
		//right
		pLocationsX[iCurrLocation] = 1.0;//fStepSize/2.0
		pLocationsY[iCurrLocation] = 0;
		iCurrLocation++;
		//left
		pLocationsX[iCurrLocation] = -1.0;
		pLocationsY[iCurrLocation] = 0;
		iCurrLocation++;
		//up
		pLocationsX[iCurrLocation] = 0;
		pLocationsY[iCurrLocation] = 1.0;
		iCurrLocation++;
		//down
		pLocationsX[iCurrLocation] = 0;
		pLocationsY[iCurrLocation] = -1.0;
		iCurrLocation++;
	}
	
	if (bGridSteps) {
		//fill in everything on the grid except 0
		if (bOffAxis) {
			//off axis grid
			for (int i = -iNumStepsH; i < iNumStepsH; i++) {
				for (int j = -iNumStepsV; j < iNumStepsV; j++) {
					pLocationsX[iCurrLocation] = i * fStepSize + fStepSize/2.0;
					pLocationsY[iCurrLocation] = j * fStepSize + fStepSize/2.0;
					iCurrLocation++;
				}
			}
		} 
		else {
			//on axis grid
			for (int i = -iNumStepsH; i < iNumStepsH+1; i++) {
				for (int j = -iNumStepsV; j < iNumStepsV+1; j++) {
					if (!(i==0 && j==0)) {
						pLocationsX[iCurrLocation] = i * fStepSize;
						pLocationsY[iCurrLocation] = j * fStepSize;
						iCurrLocation++;
					}
				}
			}
		}
	} 
	else {
		//fill in points along axes (no grid)

		// fill the horizontal locations
		if (bStepHoriz) {
			for (int i = 1; i < iNumStepsH+1; i++) {
				pLocationsX[iCurrLocation] = i * fStepSize;
				pLocationsY[iCurrLocation] = 0;
				iCurrLocation++;
			}
			for (int i = 1; i < iNumStepsH+1; i++) {
				pLocationsX[iCurrLocation] = -i * fStepSize;
				pLocationsY[iCurrLocation] = 0;
				iCurrLocation++;
			}
		}

		// fill the vertical locations
		if (bStepVert) {
			for (int i = 1; i < iNumStepsV+1; i++) {
				pLocationsY[iCurrLocation] = i * fStepSize;
				pLocationsX[iCurrLocation] = 0;
				iCurrLocation++;
			}
			for (int i = 1; i < iNumStepsV+1; i++) {
				pLocationsY[iCurrLocation] = -i * fStepSize;
				pLocationsX[iCurrLocation] = 0;
				iCurrLocation++;
			}
		}

		// fill the 45 degree locations
		//direction "bug" -- 45 and 135 appear switched because
		//y axis pixel values increase "downward" on screen
		if (bStep45) {
			// if we are using XY steps for oblique distances
			if (iDistType == DIST_TYPE_XY) {
				for (int i = 1; i < iNumStepsSmall+1; i++) {
					pLocationsX[iCurrLocation] = i * fStepSize;
					pLocationsY[iCurrLocation] = i * fStepSize;
					iCurrLocation++;
				}
				for (int i = 1; i < iNumStepsSmall+1; i++) {
					pLocationsX[iCurrLocation] = -i * fStepSize;
					pLocationsY[iCurrLocation] = -i * fStepSize;
					iCurrLocation++;
				}
			} 
			else {
				// using radial distance for oblique distances
				for (int i = 1; i < iNumStepsSmall+1; i++) {
					pLocationsX[iCurrLocation] = i * fStepSize * SQRT2_DOUBLE / 2;
					pLocationsY[iCurrLocation] = i * fStepSize * SQRT2_DOUBLE / 2;
					iCurrLocation++;
				}
				for (int i = 1; i < iNumStepsSmall+1; i++) {
					pLocationsX[iCurrLocation] = -i * fStepSize * SQRT2_DOUBLE / 2;
					pLocationsY[iCurrLocation] = -i * fStepSize * SQRT2_DOUBLE / 2;
					iCurrLocation++;
				}
			}
		}

		// fill the 135 degree locations
		if (bStep135) {
			if (iDistType == DIST_TYPE_XY) {
				// using XY steps for oblique distances
				for (int i = 1; i < iNumStepsSmall+1; i++) {
					pLocationsX[iCurrLocation] = i * fStepSize;
					pLocationsY[iCurrLocation] = -i * fStepSize;
					iCurrLocation++;
				}
				for (int i = 1; i < iNumStepsSmall+1; i++) {
					pLocationsX[iCurrLocation] = -i * fStepSize;
					pLocationsY[iCurrLocation] = i * fStepSize;
					iCurrLocation++;
				}
			} 
			else {
				// using radial distance for oblique distances
				for (int i = 1; i < iNumStepsSmall+1; i++) {
					pLocationsX[iCurrLocation] = i * fStepSize * SQRT2_DOUBLE / 2;
					pLocationsY[iCurrLocation] = -i * fStepSize * SQRT2_DOUBLE / 2;
					iCurrLocation++;
				}
				for (int i = 1; i < iNumStepsSmall+1; i++) {
					pLocationsX[iCurrLocation] = -i * fStepSize * SQRT2_DOUBLE / 2;
					pLocationsY[iCurrLocation] = i * fStepSize * SQRT2_DOUBLE / 2;
					iCurrLocation++;
				}
			}
		}
	}

	iNumLocations = iNumDisplays;//change this value for later use
}

void CRemember::GenerateCatchTrials() 
{
	int iNumDirections = 0;
	int iCurrLocation = 0;
	int iNumStepsSmall = (iNumStepsV < iNumStepsH)? iNumStepsV : iNumStepsH;
	
	// calculate number of directions used
	
	iNumLocations=0;
	if (bStepHoriz) iNumLocations += iNumStepsH*2;
	if (bStepVert) iNumLocations += iNumStepsV*2;
	if (bStep45) iNumLocations += iNumStepsSmall*2;
	if (bStep135) iNumLocations += iNumStepsSmall*2;

	//grid?
	if (bOffAxis) {
		if (bGridSteps) iNumLocations = (iNumStepsH*2) * (iNumStepsV*2);
	} 
	else {
		if (bGridSteps) iNumLocations = (iNumStepsH*2+1) * (iNumStepsV*2+1) -1;
	}

	//don't include center point
	if (!bAvoidZero && !bOffAxis) iNumLocations++;
	//include extra points around center fovea?
	if (bCenterPoints) iNumLocations += 4;
	
	// free the memory if necessary
	if (pLocationsX) free(pLocationsX);
	if (pLocationsY) free(pLocationsY);
	if (pLocationsType) free(pLocationsType);
	if (pLocationsIndex) free(pLocationsIndex);

	// allocate the necessary memory
	pLocationsX = (double*)calloc(iNumLocations, sizeof(double));
	pLocationsY = (double*)calloc(iNumLocations, sizeof(double));

	int numCombos=4;
	int loc_com_catch = iNumLocations*numCombos + iNumBlankTrials;//=locations*combos+catch_trials
	int iNumDisplays = iNumRandomRepeats*loc_com_catch;
	pLocationsType = (int*)calloc(iNumDisplays, sizeof(int));
	pLocationsIndex = (int*)calloc(iNumDisplays, sizeof(int));

	//for each repeat, fill in randomized locations
	for (int r=0; r<iNumRandomRepeats; r++) {
		int *pPriPri = (int*)calloc(iNumLocations, sizeof(int));
		int ipp=0;
		int *pPriSec = (int*)calloc(iNumLocations, sizeof(int));
		int ips=0;
		int *pSecPri = (int*)calloc(iNumLocations, sizeof(int));
		int isp=0;
		int *pSecSec = (int*)calloc(iNumLocations, sizeof(int));
		int iss=0;
		int *pPriBnk = (int*)calloc(iNumLocations, sizeof(int));
		int ipb=0;
		int *pSecBnk = (int*)calloc(iNumLocations, sizeof(int));
		int isb=0;
		int iBlank=0;
		//generate location lists
		for (int i=0; i<iNumLocations; i++) {
			pPriPri[i]=i;
			pPriSec[i]=i;
			pPriBnk[i]=i;
			pSecPri[i]=i;
			pSecSec[i]=i;
			pSecBnk[i]=i;
		}
		//randomize location lists
		for (int i=0; i<iNumLocations; i++) {
			//swap random location for each combo

			int randindex=rand()%(iNumLocations);
			int temp = pPriPri[randindex];
			pPriPri[randindex] = pPriPri[i];
			pPriPri[i]=temp;

			randindex=rand()%(iNumLocations);
			temp = pPriSec[randindex];
			pPriSec[randindex] = pPriSec[i];
			pPriSec[i]=temp;

			randindex=rand()%(iNumLocations);
			temp = pSecPri[randindex];
			pSecPri[randindex] = pSecPri[i];
			pSecPri[i]=temp;

			randindex=rand()%(iNumLocations);
			temp = pSecSec[randindex];
			pSecSec[randindex] = pSecSec[i];
			pSecSec[i]=temp;

			randindex=rand()%(iNumLocations);
			temp = pPriBnk[randindex];
			pPriBnk[randindex] = pPriBnk[i];
			pPriBnk[i]=temp;

			randindex=rand()%(iNumLocations);
			temp = pSecBnk[randindex];
			pSecBnk[randindex] = pSecBnk[i];
			pSecBnk[i]=temp;

		}
		//fill in thing
		for (int i=0; i<loc_com_catch; i++) {
			//hard code alternating blocks for center target
			//hard code random assignment for periph target

			if ((i/iAlternateBlockSize)%2==0) {
				 if (  (iBlank < iNumBlankTrials) && (rand()%loc_com_catch<iNumBlankTrials) ) {
					//random blank catch trial
					pLocationsType[r*loc_com_catch+i]=6;
					pLocationsIndex[r*loc_com_catch+i]=pPriBnk[ipb%iNumLocations]; ipb++;
					iBlank++;
				 } 
				 else {
					//catch trial as usual
					if (rand()%2==0) {
						pLocationsType[r*loc_com_catch+i]=0;
						pLocationsIndex[r*loc_com_catch+i]=pPriPri[ipp%iNumLocations]; ipp++;
					} else {
						pLocationsType[r*loc_com_catch+i]=1;
						pLocationsIndex[r*loc_com_catch+i]=pPriSec[ips%iNumLocations]; ips++;
					}
				}
			} 
			else {
				if (  (iBlank < iNumBlankTrials) && (rand()%loc_com_catch<iNumBlankTrials) ) {
					//random blank catch trial
					pLocationsType[r*loc_com_catch+i]=7;
					pLocationsIndex[r*loc_com_catch+i]=pSecBnk[isb%iNumLocations]; isb++;
					iBlank++;
				} 
				else {
					//catch trial as usual
					if (rand()%2==0) {
						pLocationsType[r*loc_com_catch+i]=2;
						pLocationsIndex[r*loc_com_catch+i]=pSecPri[isp%iNumLocations]; isp++;
					} 
					else {
						pLocationsType[r*loc_com_catch+i]=3;
						pLocationsIndex[r*loc_com_catch+i]=pSecSec[iss%iNumLocations]; iss++;
					}
				}
			}
		}
		//clean up
		free(pPriPri);
		free(pPriSec);
		free(pSecPri);
		free(pSecSec);
		free(pPriBnk);
		free(pSecBnk);
	}

	iCurrLocation=0;

	if (!bAvoidZero && !bOffAxis) {
		pLocationsX[0] = 0;
		pLocationsY[0] = 0;
		iCurrLocation++;
	}

	if (bCenterPoints) {
		//right
		pLocationsX[iCurrLocation] = 1.0;//fStepSize/2.0
		pLocationsY[iCurrLocation] = 0;
		iCurrLocation++;
		//left
		pLocationsX[iCurrLocation] = -1.0;
		pLocationsY[iCurrLocation] = 0;
		iCurrLocation++;
		//up
		pLocationsX[iCurrLocation] = 0;
		pLocationsY[iCurrLocation] = 1.0;
		iCurrLocation++;
		//down
		pLocationsX[iCurrLocation] = 0;
		pLocationsY[iCurrLocation] = -1.0;
		iCurrLocation++;
	}
	
	//fill in everything on the grid except 0
	if (bOffAxis) {
		//off axis grid
		for (int i = -iNumStepsH; i < iNumStepsH; i++) {
			for (int j = -iNumStepsV; j < iNumStepsV; j++) {
				pLocationsX[iCurrLocation] = i * fStepSize + fStepSize/2.0;
				pLocationsY[iCurrLocation] = j * fStepSize + fStepSize/2.0;
				iCurrLocation++;
			}
		}
	} 
	else {
		//on axis grid
		for (int i = -iNumStepsH; i < iNumStepsH+1; i++) {
			for (int j = -iNumStepsV; j < iNumStepsV+1; j++) {
				if (!(i==0 && j==0)) {
					pLocationsX[iCurrLocation] = i * fStepSize;
					pLocationsY[iCurrLocation] = j * fStepSize;
					iCurrLocation++;
				}
			}
		}
	}

	iNumLocations = iNumDisplays;//change this value for later use
}

//revised sampling scheme
void CRemember::GenerateLocations2() 
{
	bool debug = false;
	bool info = false;
	int iCurrLocation = 0;

	// 12 types of trials. location x color
	//
	//					red		blue	blank	purple
	// grid				(0)		(3)		(6)		(9)
	// periph fovea		(1)		(4)		(7)		(10)
	// center fovea		(2)		(5)		(8)		(11)
	//
	// numLocations = sum((0),...,(11))
	//
	// given (9) and percentages for red/blue/blank and enrich_periph/enrich_center
	// so we can calculate percentages for grid and purple
	// then we can get the number of points in other categories besides (9) because
	// we know the pct of points in (9)
	//
	// pLocationsClass is the intended class of location:
	// 1 grid
	// 2 periph fovea
	// 3 center fovea
	//
	int numpoints[12]; for (int i=0; i<12; i++) numpoints[i]=0;
	double redpct = ((double)iNumCatchTrials)/100.0;
	double bluepct = ((double)iNumCatchTrials2)/100.0;
	double blankpct = ((double)iNumBlankTrials)/100.0;
	double fixatingpct = ((double)iPctTimeAtCenter)/100.0;
	double deviatedpct = ((double)iPctTimeAtPeriph)/100.0;
	double gridpct = ((double)(100-iPctTimeAtCenter-iPctTimeAtPeriph))/100.0;
	double purplepct = ((double)( 100-iNumCatchTrials-iNumCatchTrials2-iNumBlankTrials ))/100.0;
	gridpct = (gridpct<0.0)? 0.0: gridpct; gridpct = (gridpct>100.0)? 100.0: gridpct;
	fixatingpct = (fixatingpct<0.0)? 0.0: fixatingpct; fixatingpct = (fixatingpct>100.0)? 100.0: fixatingpct;
	deviatedpct = (deviatedpct<0.0)? 0.0: deviatedpct; deviatedpct = (deviatedpct>100.0)? 100.0: deviatedpct;
	purplepct = (purplepct<0.0)? 0.0: purplepct; purplepct = (purplepct>100.0)? 100.0: purplepct;
	redpct = (redpct<0.0)? 0.0: redpct; redpct = (redpct>100.0)? 100.0: redpct;
	bluepct = (bluepct<0.0)? 0.0: bluepct; bluepct = (bluepct>100.0)? 100.0: bluepct;
	blankpct = (blankpct<0.0)? 0.0: blankpct; blankpct = (blankpct>100.0)? 100.0: blankpct;
	if (debug) {
		//---------------------------
		char buffer[1024];
		sprintf(buffer, "red=%f blue=%f blank=%f purple=%f", redpct, bluepct, blankpct, purplepct);
		AfxMessageBox(buffer);
		//---------------------------
	}
	//grid counts
	if (purplepct > 0.0) {
		numpoints[9] = 2*((iNumStepsH*2+1) * (iNumStepsV*2+1) -1);// r/b fixation point, step in each direction, not zero
		numpoints[0] = (int)(redpct*((double)numpoints[9])/purplepct);
		numpoints[3] = (int)(bluepct*((double)numpoints[9])/purplepct);
		numpoints[6] = (int)(blankpct*((double)numpoints[9])/purplepct);
	} 
	else {
		//catch trial ONLY run
		numpoints[9] = 0;
		if (redpct > 0.0) {
			numpoints[0] = (int)(redpct*((double)(2*((iNumStepsH*2+1) * (iNumStepsV*2+1) -1))));
			numpoints[3] = (int)(bluepct*((double)numpoints[0])/redpct);
			numpoints[6] = (int)(blankpct*((double)numpoints[0])/redpct);
		} 
		else if (bluepct > 0) {
			numpoints[3] = (int)(bluepct*((double)(2*((iNumStepsH*2+1) * (iNumStepsV*2+1) -1))));
			numpoints[0] = (int)(redpct*((double)numpoints[3])/bluepct);
			numpoints[6] = (int)(blankpct*((double)numpoints[3])/bluepct);
		} 
		else {
			numpoints[6] = (int)(blankpct*((double)(2*((iNumStepsH*2+1) * (iNumStepsV*2+1) -1))));
			numpoints[0] = (int)(redpct*((double)numpoints[6])/blankpct);
			numpoints[3] = (int)(bluepct*((double)numpoints[6])/blankpct);
		}
	}
	if (gridpct > 0.0) {
		//periph fovea counts
		numpoints[1] = (int)(deviatedpct*((double)numpoints[0])/gridpct);
		numpoints[4] = (int)(bluepct*((double)numpoints[1])/redpct);
		numpoints[7] = (int)(blankpct*((double)numpoints[1])/redpct);
		numpoints[10] = (int)(purplepct*((double)numpoints[1])/redpct);
		//center fovea counts
		numpoints[2] = (int)(fixatingpct*((double)numpoints[0])/gridpct);
		numpoints[5] = (int)(bluepct*((double)numpoints[2])/redpct);
		numpoints[8] = (int)(blankpct*((double)numpoints[2])/redpct);
		numpoints[11] = (int)(purplepct*((double)numpoints[2])/redpct);
	} 
	else {
		//enrichment is 100% of trials! no grid!
		//assume number of trials on grid will be number for fovea with greater %
		if (deviatedpct > fixatingpct) {
			//move points down
			numpoints[1] = numpoints[0]; numpoints[0] = 0;
			numpoints[4] = numpoints[3]; numpoints[3] = 0;
			numpoints[7] = numpoints[6]; numpoints[6] = 0;
			numpoints[10] = numpoints[9]; numpoints[9] = 0;
			//center fovea counts
			numpoints[2] = (int)(fixatingpct*((double)numpoints[1])/deviatedpct);
			numpoints[5] = (int)(fixatingpct*((double)numpoints[4])/deviatedpct);
			numpoints[8] = (int)(fixatingpct*((double)numpoints[7])/deviatedpct);
			numpoints[11] = (int)(fixatingpct*((double)numpoints[10])/deviatedpct);
		} 
		else {
			//move points down
			numpoints[2] = numpoints[0]; numpoints[0] = 0;
			numpoints[5] = numpoints[3]; numpoints[3] = 0;
			numpoints[8] = numpoints[6]; numpoints[6] = 0;
			numpoints[11] = numpoints[9]; numpoints[9] = 0;
			//periph fovea counts
			numpoints[1] = (int)(deviatedpct*((double)numpoints[2])/fixatingpct);
			numpoints[4] = (int)(deviatedpct*((double)numpoints[5])/fixatingpct);
			numpoints[7] = (int)(deviatedpct*((double)numpoints[8])/fixatingpct);
			numpoints[10] = (int)(deviatedpct*((double)numpoints[11])/fixatingpct);
		}
	}
	//total count
	iNumLocations = 0;//number of displays in one "layer" or repeat
	for (int i=0; i<12; i++) iNumLocations += numpoints[i];

	if (debug||info) {
		//---------------------------
		char buffer[1024];
		sprintf(buffer, "\tRed\tBlu\tBla\tPrp\nGri\t%d\t%d\t%d\t%d\nPfv\t%d\t%d\t%d\t%d\nCfv\t%d\t%d\t%d\t%d", numpoints[0], numpoints[3], numpoints[6], numpoints[9], numpoints[1], numpoints[4], numpoints[7], numpoints[10], numpoints[2], numpoints[5], numpoints[8], numpoints[11]);
		AfxMessageBox(buffer);
		//---------------------------
	}

	// free the memory if necessary
	if (pLocationsX) free(pLocationsX);
	if (pLocationsY) free(pLocationsY);
	if (pLocationsType) free(pLocationsType);
	if (pLocationsIndex) free(pLocationsIndex);
	if (pLocationsClass) free(pLocationsClass);
	// allocate the necessary memory
	pLocationsX = (double*)calloc(iNumLocations, sizeof(double));
	pLocationsY = (double*)calloc(iNumLocations, sizeof(double));
	int iNumDisplays = iNumRandomRepeats*iNumLocations;
	pLocationsType = (int*)calloc(iNumDisplays, sizeof(int));
	pLocationsIndex = (int*)calloc(iNumDisplays, sizeof(int));
	pLocationsClass = (int*)calloc(iNumDisplays, sizeof(int));

	//plot X and Y locations in array
	iCurrLocation=0;
	//fill in purple points on the grid except 0
	for (int i = -iNumStepsH; i < iNumStepsH+1; i++) {
		for (int j = -iNumStepsV; j < iNumStepsV+1; j++) {
			if (!(i==0 && j==0)) {
				//twice
				pLocationsX[iCurrLocation] = i * fStepSize;
				pLocationsY[iCurrLocation] = j * fStepSize;
				iCurrLocation++;
			}
		}
	}
	for (int i = -iNumStepsH; i < iNumStepsH+1; i++) {
		for (int j = -iNumStepsV; j < iNumStepsV+1; j++) {
			if (!(i==0 && j==0)) {
				//twice
				pLocationsX[iCurrLocation] = i * fStepSize;
				pLocationsY[iCurrLocation] = j * fStepSize;
				iCurrLocation++;
			}
		}
	}
	//fill in ALL catch trials on grid
	for (int i=0; i<numpoints[0]+numpoints[3]; i++) {
		int x = rand()%(2*iNumStepsH+1) - iNumStepsH;
		int y = rand()%(2*iNumStepsV+1) - iNumStepsV;
		while (x==0&&y==0) {
			x = rand()%(2*iNumStepsH+1) - iNumStepsH;
			y = rand()%(2*iNumStepsV+1) - iNumStepsV;
		}
		pLocationsX[iCurrLocation] = x * fStepSize;
		pLocationsY[iCurrLocation] = y * fStepSize;
		iCurrLocation++;
	}
	for (int i=0; i<numpoints[6]; i++) {//blank
		pLocationsX[iCurrLocation] = 0.0;
		pLocationsY[iCurrLocation] = 0.0;
		iCurrLocation++;
	}

	//fill in ALL points on deviated fovea
	for (int i=0; i<numpoints[1]+numpoints[4]+numpoints[10]; i++) {
		//this is an offset from the deviated location, which could be for either eye
		//so the actual location will be the loc of the appropriate deviation plus this value
		//make sure to add the offset when assigning types!! (after we know which eye to use)
		double rand_pct = 0.01*((double)(rand()%100));
		pLocationsX[iCurrLocation] = (rand_pct*fPeriphXRadius*2.0)-fPeriphXRadius;
		rand_pct = 0.01*((double)(rand()%100));
		pLocationsY[iCurrLocation] = (rand_pct*fPeriphYRadius*2.0)-fPeriphYRadius;
		iCurrLocation++;
	}
	for (int i=0; i<numpoints[7]; i++) {//blank
		pLocationsX[iCurrLocation] = 0.0;
		pLocationsY[iCurrLocation] = 0.0;
		iCurrLocation++;
	}
	//fill in ALL points on fixating fovea
	double minr = (fCenterDotSize/2.0)+fPeriphDotSize;
	if (debug) {
		//---------------------------
		char buffer[1024];
		sprintf(buffer, "minr=%f fCenterXRadius=%f, fCenterYRadius=%f", minr, fCenterXRadius, fCenterYRadius);
		AfxMessageBox(buffer);
		//---------------------------
	}
	for (int i=0; i<numpoints[2]+numpoints[5]+numpoints[11]; i++) {
		if (bAvoidZero) {
			//keep points outside of center fixation point (never overlap cross)
			/*double minr = (fCenterDotSize+fPeriphDotSize)/2.0;
			double xr = (double)(rand()%100) * (fCenterXRadius-minr) * 0.01;
			double yr = (double)(rand()%100) * (fCenterYRadius-minr) * 0.01;
			pLocationsX[iCurrLocation] = (((rand()%2)==0)? 1.0 : -1.0) * (minr+xr);
			pLocationsY[iCurrLocation] = (((rand()%2)==0)? 1.0 : -1.0) * (minr+yr);*/
			double xr,yr;
			if (fCenterXRadius < minr || fCenterYRadius < minr) {//"cannot" avoid zero
				xr = fCenterXRadius;
				yr = fCenterYRadius;
			} 
			else {											//randomly in allowed range
				xr = ((double)(rand()%100)) * (fCenterXRadius) * 0.01;
				yr = ((double)(rand()%100)) * (fCenterYRadius) * 0.01;
				while ( (xr<minr) && (yr < minr) ) {
					xr = ((double)(rand()%100)) * (fCenterXRadius) * 0.01;
					yr = ((double)(rand()%100)) * (fCenterYRadius) * 0.01;
				}
			}
			pLocationsX[iCurrLocation] = (((rand()%2)==0)? -1.0 : 1.0 ) * xr;
			pLocationsY[iCurrLocation] = (((rand()%2)==0)? -1.0 : 1.0 ) * yr;

		} 
		else {
			pLocationsX[iCurrLocation] = ((double)(rand()%100)) * (fCenterXRadius*2.0) * 0.01 - fCenterXRadius;
			pLocationsY[iCurrLocation] = ((double)(rand()%100)) * (fCenterYRadius*2.0) * 0.01 - fCenterYRadius;
		}
		iCurrLocation++;
	}
	for (int i=0; i<numpoints[8]; i++) {//blank
		pLocationsX[iCurrLocation] = 0.0;
		pLocationsY[iCurrLocation] = 0.0;
		iCurrLocation++;
	}

	if (debug) {
		//---------------------------
		char buffer[1024];
		sprintf(buffer, "filled in locations");
		AfxMessageBox(buffer);
		//---------------------------
	}
	//for each repeat, shuffle points and fill in type and index of location
	//also fill in class, for later reference
	for (int r=0; r<iNumRandomRepeats; r++) {
		// ----- enter in all types and location for one layer -----
		//purple grid
		for (int i=0; i<numpoints[9]/2; i++) {
			pLocationsType[r*iNumLocations+i] = 4;//red,purple
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 1;//1 grid
		}
		for (int i=numpoints[9]/2; i<numpoints[9]; i++) {
			pLocationsType[r*iNumLocations+i] = 5;//blue,purple
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 1;//1 grid
		}
		int sum = numpoints[9];
		//red grid
		for (int i=sum; i<sum+numpoints[0]/2; i++) {
			pLocationsType[r*iNumLocations+i] = 0;//red,red
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 1;//1 grid
		}
		for (int i=sum+numpoints[0]/2; i<sum+numpoints[0]; i++) {
			pLocationsType[r*iNumLocations+i] = 2;//blue,red
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 1;//1 grid
		}
		sum += numpoints[0];
		//blue grid
		for (int i=sum; i<sum+numpoints[3]/2; i++) {
			pLocationsType[r*iNumLocations+i] = 1;//red,blue
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 1;//1 grid
		}
		for (int i=sum+numpoints[3]/2; i<sum+numpoints[3]; i++) {
			pLocationsType[r*iNumLocations+i] = 3;//blue,blue
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 1;//1 grid
		}
		sum += numpoints[3];
		//blank grid
		for (int i=sum; i<sum+numpoints[6]/2; i++) {
			pLocationsType[r*iNumLocations+i] = 6;//red,blank
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 1;//1 grid
		}
		for (int i=sum+numpoints[6]/2; i<sum+numpoints[6]; i++) {
			pLocationsType[r*iNumLocations+i] = 7;//blue,blank
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 1;//1 grid
		}
		sum += numpoints[6];
		//purple periph fovea
		for (int i=sum; i<sum+numpoints[10]/2; i++) {
			pLocationsType[r*iNumLocations+i] = 4;//red,purple
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 2;//2 periph fovea
			//modify location
			pLocationsX[i] += fPeriphXPosL;///2.0
			pLocationsY[i] += fPeriphYPosL;///2.0
		}
		for (int i=sum+numpoints[10]/2; i<sum+numpoints[10]; i++) {
			pLocationsType[r*iNumLocations+i] = 5;//blue,purple
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 2;//2 periph fovea
			//modify location
			pLocationsX[i] += fPeriphXPos;///2.0
			pLocationsY[i] += fPeriphYPos;///2.0
		}
		sum += numpoints[10];
		//red periph fovea
		for (int i=sum; i<sum+numpoints[1]/2; i++) {
			pLocationsType[r*iNumLocations+i] = 0;//red,red
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 2;//2 periph fovea
			//modify location
			pLocationsX[i] += fPeriphXPosL;///2.0
			pLocationsY[i] += fPeriphYPosL;///2.0
		}
		for (int i=sum+numpoints[1]/2; i<sum+numpoints[1]; i++) {
			pLocationsType[r*iNumLocations+i] = 2;//blue,red
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 2;//2 periph fovea
			//modify location
			pLocationsX[i] += fPeriphXPos;///2.0
			pLocationsY[i] += fPeriphYPos;///2.0
		}
		sum += numpoints[1];
		//blue periph fovea
		for (int i=sum; i<sum+numpoints[4]/2; i++) {
			pLocationsType[r*iNumLocations+i] = 1;//red,blue
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 2;//2 periph fovea
			//modify location
			pLocationsX[i] += fPeriphXPosL;///2.0
			pLocationsY[i] += fPeriphYPosL;///2.0
		}
		for (int i=sum+numpoints[4]/2; i<sum+numpoints[4]; i++) {
			pLocationsType[r*iNumLocations+i] = 3;//blue,blue
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 2;//2 periph fovea
			//modify location
			pLocationsX[i] += fPeriphXPos;///2.0
			pLocationsY[i] += fPeriphYPos;///2.0
		}
		sum += numpoints[4];
		//blank periph fovea (no need to adjust locations (already at 0)
		for (int i=sum; i<sum+numpoints[7]/2; i++) {
			pLocationsType[r*iNumLocations+i] = 6;//red,blank
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 2;//2 periph fovea
		}
		for (int i=sum+numpoints[7]/2; i<sum+numpoints[7]; i++) {
			pLocationsType[r*iNumLocations+i] = 7;//blue,blank
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 2;//2 periph fovea
		}
		sum += numpoints[7];
		//purple center fovea
		for (int i=sum; i<sum+numpoints[11]/2; i++) {
			pLocationsType[r*iNumLocations+i] = 4;//red,purple
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 3;//3 center fovea
		}
		for (int i=sum+numpoints[11]/2; i<sum+numpoints[11]; i++) {
			pLocationsType[r*iNumLocations+i] = 5;//blue,purple
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 3;//3 center fovea
		}
		sum += numpoints[11];
		//red center fovea 2
		for (int i=sum; i<sum+numpoints[2]/2; i++) {
			pLocationsType[r*iNumLocations+i] = 0;//red,red
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 3;//3 center fovea
		}
		for (int i=sum+numpoints[2]/2; i<sum+numpoints[2]; i++) {
			pLocationsType[r*iNumLocations+i] = 2;//blue,red
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 3;//3 center fovea
		}
		sum += numpoints[2];
		//blue center fovea
		for (int i=sum; i<sum+numpoints[5]/2; i++) {
			pLocationsType[r*iNumLocations+i] = 1;//red,blue
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 3;//3 center fovea
		}
		for (int i=sum+numpoints[5]/2; i<sum+numpoints[5]; i++) {
			pLocationsType[r*iNumLocations+i] = 3;//blue,blue
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 3;//3 center fovea
		}
		sum += numpoints[5];
		//blank center fovea
		for (int i=sum; i<sum+numpoints[8]/2; i++) {
			pLocationsType[r*iNumLocations+i] = 6;//red,blank
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 3;//3 center fovea
		}
		for (int i=sum+numpoints[8]/2; i<sum+numpoints[8]; i++) {
			pLocationsType[r*iNumLocations+i] = 7;//blue,blank
			pLocationsIndex[r*iNumLocations+i] = i;
			pLocationsClass[r*iNumLocations+i] = 3;//3 center fovea
		}

		if (debug) {
			//---------------------------
			char buffer[1024];
			sprintf(buffer, "repeat %d, finished type and index", r);
			AfxMessageBox(buffer);
			//---------------------------
		}
		// ----- randomize them -----
		for (int i=0; i<iNumLocations; i++) {
			int randindex = rand()%iNumLocations;
			//swap
			int t = pLocationsType[r*iNumLocations+i];
			pLocationsType[r*iNumLocations+i] = pLocationsType[r*iNumLocations+randindex];
			pLocationsType[r*iNumLocations+randindex] = t;
			t = pLocationsIndex[r*iNumLocations+i];
			pLocationsIndex[r*iNumLocations+i] = pLocationsIndex[r*iNumLocations+randindex];
			pLocationsIndex[r*iNumLocations+randindex] = t;
			t = pLocationsClass[r*iNumLocations+i];
			pLocationsClass[r*iNumLocations+i] = pLocationsClass[r*iNumLocations+randindex];
			pLocationsClass[r*iNumLocations+randindex] = t;
			
		}
		if (debug) {
			//---------------------------
			char buffer[1024];
			sprintf(buffer, "repeat %d, randomized", r);
			AfxMessageBox(buffer);
			//---------------------------
		}

		// ----- make sure they are in center target fixation blocks -----
		for (int i=0; i<iNumLocations; i++) {
			//if not what we want here, then greedily swap it
			int t = pLocationsType[r*iNumLocations+i];
			bool centerIsRed = (t==0) || (t==1) || (t==4) || (t==6);
			bool shouldBeRed = i%(2*iAlternateBlockSize) < iAlternateBlockSize;
			
			if ((!centerIsRed && shouldBeRed) || (centerIsRed && !shouldBeRed)) {
				//scan ahead to swap
				int j=i+1;
				bool found=false;
				while (j<iNumLocations && !found) {
					t = pLocationsType[r*iNumLocations+j];
					bool thisOneIsRed = (t==0) || (t==1) || (t==4) || (t==6);
					if ((thisOneIsRed && shouldBeRed) || (!thisOneIsRed && !shouldBeRed)) {
						//swap i and j
						t = pLocationsType[r*iNumLocations+i];
						pLocationsType[r*iNumLocations+i] = pLocationsType[r*iNumLocations+j];
						pLocationsType[r*iNumLocations+j] = t;
						t = pLocationsIndex[r*iNumLocations+i];
						pLocationsIndex[r*iNumLocations+i] = pLocationsIndex[r*iNumLocations+j];
						pLocationsIndex[r*iNumLocations+j] = t;
						found = true;
					}
					j++;
				}
			}
		}
	}

	/*
	text label coding:
	code	center	periph
	0x0		pri		pri
	0x1		pri		sec
	0x2		sec		pri
	0x3		sec		sec

	0x4		pri		ter
	0x5		sec		ter

	0x6		pri		blank
	0x7		sec		blank
	*/

	iNumLocations = iNumDisplays;//change this value for later use
}

bool CRemember::readTag(FILE *fp, char *name, char *value, int size) 
{
	char c;
	c = fgetc(fp);
	bool debug=false;

	//read whitespace
	while (isspace(c)) c = fgetc(fp);

	//read <p
	if (c=='<') {
		c = fgetc(fp);
		if (c=='p') {
			c = fgetc(fp);
			while (isspace(c)) c = fgetc(fp);
		} 
		else {
			if (debug) AfxMessageBox("readTag failed: <p");
			return false;
		}
	} 
	else {
		if (debug) AfxMessageBox("readTag failed: <");
		return false;
	}

	//read name
	if (c=='n') {
		c = fgetc(fp);
		if (c=='a') {
			c = fgetc(fp);
			if (c=='m') {
				c = fgetc(fp);
				if (c=='e') {
					c = fgetc(fp);
					if (c=='=') {
						c = fgetc(fp);
						if (c=='\"') {
							c = fgetc(fp);
							int i=0;
							while (c != '\"' && i < size) {
								name[i] = c;
								c = fgetc(fp);
								i++;
							}
							name[i]=0;
							if (debug) {
								char buffer[1024];
								sprintf(buffer, "read name=%s with i=%d c=%c", name, i, c);
								AfxMessageBox(buffer);
							}
						} 
						else {
							if (debug) AfxMessageBox("readTag failed: <p name=\"");
							return false;
						}
					} 
					else {
						if (debug) AfxMessageBox("readTag failed: <p name=");
						return false;
					}
				} 
				else {
					if (debug) AfxMessageBox("readTag failed: <p name");
					return false;
				}
			} 
			else {
				if (debug) AfxMessageBox("readTag failed: <p nam");
				return false;
			}
		} 
		else {
			if (debug) AfxMessageBox("readTag failed: <p na");
			return false;
		}
	} 
	else {
		if (debug) AfxMessageBox("readTag failed: <p n");
		return false;
	}

	if (c=='\"') c = fgetc(fp);
	else return false;
	while (isspace(c)) c = fgetc(fp);
	//read value
	if (c=='v') {
		c = fgetc(fp);
		if (c=='a') {
			c = fgetc(fp);
			if (c=='l') {
				c = fgetc(fp);
				if (c=='u') {
					c = fgetc(fp);
					if (c=='e') {
						c = fgetc(fp);
						if (c=='=') {
							c = fgetc(fp);
							if (c=='\"') {
								c = fgetc(fp);
								int i=0;
								while (c != '\"' && i < size) {
									value[i] = c;
									c = fgetc(fp);
									i++;
								}
								value[i]=0;
							} 
							else {
								if (debug) AfxMessageBox("readTag failed: <p name=\"blah\" value=\"");
								return false;
							}
						} 
						else {
							if (debug) AfxMessageBox("readTag failed: <p name=\"blah\" value=");
							return false;
						}
					} 
					else {
						if (debug) AfxMessageBox("readTag failed: <p name=\"blah\" value");
						return false;
					}
				} 
				else {
					if (debug) AfxMessageBox("readTag failed: <p name=\"blah\" valu");
					return false;
				}
			} 
			else {
				if (debug) AfxMessageBox("readTag failed: <p name=\"blah\" val");
				return false;
			}
		} 
		else {
			if (debug) AfxMessageBox("readTag failed: <p name=\"blah\" va");
			return false;
		}
	} 
	else {
		if (debug) AfxMessageBox("readTag failed: <p name=\"blah\" v");
		if (debug) {
			char buffer[1024];
			sprintf(buffer, "c=%c", c);
			AfxMessageBox(buffer);
		}
		return false;
	}
	if (c=='\"') c = fgetc(fp);
	else return false;
	while (isspace(c)) c = fgetc(fp);
	//read />
	if (c=='/') {
		c = fgetc(fp);
		if (c=='>') {
			return true;
		} 
		else {
			if (debug) AfxMessageBox("readTag failed: <p name=\"blah\" value=\"blah\" />");
			return false;
		}
	} 
	else {
		if (debug) AfxMessageBox("readTag failed: <p name=\"blah\" value=\"blah\" /");
		return false;
	}
}

void CRemember::LoadXMLFile(bool bDefault) 
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(true, ".xml", "XML Files (*.xml)|*.xml||ASCII Text Files (*.txt)|*.txt", filename, false);
	}

	char buffer[1024];
	sprintf(buffer, "%s", filename);
	FILE* fp = fopen(filename, "rt");
	if (!fp) {
		AfxMessageBox("Error loading file - file or folder not found");
		return;
	}

	//check for <preferences> tag and name=Remember
	char buff[2048]; buff[0]=0;
	fscanf(fp, "<preferences name=\"%s\">\n", buff);
	if (strchr(buff, '\"')>0) strchr(buff, '\"')[0]=0;//cut off at quote
	if (strcmp(buff, "Remember") != 0) {
		//AfxMessageBox("Please load a preferences file for the Saccade To Remember stimulus.");
		//AfxMessageBox(buff);
		return;
	}
	char name[1024];
	char value[1024];
	while (readTag(fp, name, value, 1024)) {
		if      (!strcmp(name, "fCenterXPos")) {				fCenterXPos = (float)atof(value);			}
		else if (!strcmp(name, "fCenterYPos")) {				fCenterYPos = (float)atof(value);			}
		else if (!strcmp(name, "fStepSize")) {					fStepSize = (float)atof(value);				}
		else if (!strcmp(name, "iNumStepsH")) {					iNumStepsH = (int)atoi(value);				}
		else if (!strcmp(name, "iNumStepsV")) {					iNumStepsV = (int)atoi(value);				}
		else if (!strcmp(name, "fPeriphDotSize")) {				fPeriphDotSize = (float)atof(value);		}
		else if (!strcmp(name, "fCenterDotSize")) {				fCenterDotSize = (float)atof(value);		}
		else if (!strcmp(name, "iFlashInterval")) {				iFlashInterval = (int)atoi(value);			}
		else if (!strcmp(name, "iFlashDuration")) {				iFlashDuration = (int)atoi(value);			}
		else if (!strcmp(name, "iCenterColorRadio")) {			iCenterColorRadio = (int)atoi(value);		}
		else if (!strcmp(name, "iPeriphColorRadio")) {			iPeriphColorRadio = (int)atoi(value);		}
		else if (!strcmp(name, "iCenterPrimaryRed")) {			iCenterPrimaryRed = (int)atoi(value);		}
		else if (!strcmp(name, "iCenterPrimaryGreen")) {		iCenterPrimaryGreen = (int)atoi(value);		}
		else if (!strcmp(name, "iCenterPrimaryBlue")) {			iCenterPrimaryBlue = (int)atoi(value);		}
		else if (!strcmp(name, "iCenterSecondaryRed")) {		iCenterSecondaryRed = (int)atoi(value);		}
		else if (!strcmp(name, "iCenterSecondaryGreen")) {		iCenterSecondaryGreen = (int)atoi(value);	}
		else if (!strcmp(name, "iCenterSecondaryBlue")) {		iCenterSecondaryBlue = (int)atoi(value);	}
		else if (!strcmp(name, "iPeriphPrimaryRed")) {			iPeriphPrimaryRed = (int)atoi(value);		}
		else if (!strcmp(name, "iPeriphPrimaryGreen")) {		iPeriphPrimaryGreen = (int)atoi(value);		}
		else if (!strcmp(name, "iPeriphPrimaryBlue")) {			iPeriphPrimaryBlue = (int)atoi(value);		}
		else if (!strcmp(name, "iPeriphSecondaryRed")) {		iPeriphSecondaryRed = (int)atoi(value);		}
		else if (!strcmp(name, "iPeriphSecondaryGreen")) {		iPeriphSecondaryGreen = (int)atoi(value);	}
		else if (!strcmp(name, "iPeriphSecondaryBlue")) {		iPeriphSecondaryBlue = (int)atoi(value);	}
		else if (!strcmp(name, "bBehaviorCheck")) {				bBehaviorCheck = 1==atoi(value);			}
		else if (!strcmp(name, "bStepHoriz")) {					bStepHoriz = 1==atoi(value);				}
		else if (!strcmp(name, "bStepVert")) {					bStepVert = 1==atoi(value);					}
		else if (!strcmp(name, "bStep45")) {					bStep45 = 1==atoi(value);					}
		else if (!strcmp(name, "bStep135")) {					bStep135 = 1==atoi(value);					}
		else if (!strcmp(name, "iDistType")) {					iDistType = (int)atoi(value);				}
		else if (!strcmp(name, "iAlternateBlockSize")) {		iAlternateBlockSize = (int)atoi(value);		}
		else if (!strcmp(name, "iMaxDelay")) {					iMaxDelay = (int)atoi(value);				}
		else if (!strcmp(name, "iStimType")) {					iStimType = (int)atoi(value);				}
		else if (!strcmp(name, "iPostFlashDelay")) {			iPostFlashDelay = (int)atoi(value);			}
		else if (!strcmp(name, "iIntertrialDelay")) {			iIntertrialDelay = (int)atoi(value);		}
		else if (!strcmp(name, "bAvoidZero")) {					bAvoidZero = 1==atoi(value);				}
		else if (!strcmp(name, "bRedBlue")) {					bRedBlue = 1==atoi(value);					}
		else if (!strcmp(name, "bRandomizeFlashInterval")) {	bRandomizeFlashInterval = 1==atoi(value);	}
		else if (!strcmp(name, "bRandomizePostFlash")) {		bRandomizePostFlash = 1==atoi(value);		}
		else if (!strcmp(name, "iPostFlashRandMax")) {			iPostFlashRandMax = (int)atoi(value);		}
		else if (!strcmp(name, "iPostFlashRandMin")) {			iPostFlashRandMin = (int)atoi(value);		}
		else if (!strcmp(name, "iFlashIntervalRandMax")) {		iFlashIntervalRandMax = (int)atoi(value);	}
		else if (!strcmp(name, "iFlashIntervalRandMin")) {		iFlashIntervalRandMin = (int)atoi(value);	}
		else if (!strcmp(name, "fPeriphXPos")) {				fPeriphXPos = (float)atof(value);			}
		else if (!strcmp(name, "fPeriphYPos")) {				fPeriphYPos = (float)atof(value);			}
		else if (!strcmp(name, "fPeriphXRadius")) {				fPeriphXRadius = (float)atof(value);		}
		else if (!strcmp(name, "fPeriphYRadius")) {				fPeriphYRadius = (float)atof(value);		}
		else if (!strcmp(name, "iPctTimeAtPeriph")) {			iPctTimeAtPeriph = (int)atoi(value);		}
		else if (!strcmp(name, "bSeqRandom")) {					bSeqRandom = 1==atoi(value);				}
		else if (!strcmp(name, "iPeriphTertiaryRed")) {			iPeriphTertiaryRed = (int)atoi(value);		}
		else if (!strcmp(name, "iPeriphTertiaryGreen")) {		iPeriphTertiaryGreen = (int)atoi(value);	}
		else if (!strcmp(name, "iPeriphTertiaryBlue")) {		iPeriphTertiaryBlue = (int)atoi(value);		}
		else if (!strcmp(name, "iNoiseRed")) {					iNoiseRed = (int)atoi(value);				}
		else if (!strcmp(name, "iNoiseGreen")) {				iNoiseGreen = (int)atoi(value);				}
		else if (!strcmp(name, "iNoiseBlue")) {					iNoiseBlue = (int)atoi(value);				}
		else if (!strcmp(name, "iNumRandomRepeats")) {			iNumRandomRepeats = (int)atoi(value);		}
		else if (!strcmp(name, "bUseNoise")) {					bUseNoise = 1==atoi(value);					}
		else if (!strcmp(name, "iNumCatchTrials")) {			iNumCatchTrials = (int)atoi(value);			}
		else if (!strcmp(name, "bGridSteps")) {					bGridSteps = 1==atoi(value);				}
		else if (!strcmp(name, "iNoiseSize")) {					iNoiseSize = (int)atoi(value);				}
		else if (!strcmp(name, "iCrossPix")) {					iCrossPix = (int)atoi(value);				}
		else if (!strcmp(name, "iNumBlankTrials")) {			iNumBlankTrials = (int)atoi(value);			}
		else if (!strcmp(name, "iNoisePct")) {					iNoisePct = (int)atoi(value);				}
		else if (!strcmp(name, "bUseDim")) {					bUseDim = 1==atoi(value);					}
		else if (!strcmp(name, "iDimOnsetMin")) {				iDimOnsetMin = (int)atoi(value);			}
		else if (!strcmp(name, "iDimOnsetMax")) {				iDimOnsetMax = (int)atoi(value);			}
		else if (!strcmp(name, "iDimTimeout")) {				iDimTimeout = (int)atoi(value);				}
		else if (!strcmp(name, "iAnswerPeriod")) {				iAnswerPeriod = (int)atoi(value);			}
		else if (!strcmp(name, "iDimBrightness")) {				iDimBrightness = (int)atoi(value);			}
		else if (!strcmp(name, "bHideCenter")) {				bHideCenter = 1==atoi(value);				}
		else if (!strcmp(name, "bInterleaveNoise")) {			bInterleaveNoise = 1==atoi(value);			}
		else if (!strcmp(name, "bCenterPoints")) {				bCenterPoints = 1==atoi(value);				}
		else if (!strcmp(name, "bOffAxis")) {					bOffAxis = 1==atoi(value);					}
		else if (!strcmp(name, "bResponse")) {					bResponse= 1==atoi(value);					}
		else if (!strcmp(name, "fPeriphXPosL")) {				fPeriphXPosL = (float)atof(value);			}
		else if (!strcmp(name, "fPeriphYPosL")) {				fPeriphYPosL = (float)atof(value);			}
		else if (!strcmp(name, "iResponseTimeout")) {			iResponseTimeout = (int)atoi(value);		}
		else if (!strcmp(name, "bRampUp")) {					bRampUp= 1==atoi(value);					}
		else if (!strcmp(name, "iRampDuration")) {				iRampDuration = (int)atoi(value);			}
	}
	//check for </preferences> tag
	fscanf(fp, "</preferences>");
	
	fclose(fp);
}

void CRemember::SaveXMLFile(bool bDefault) 
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
	fprintf(fp, "<preferences name=\"Remember\">\n");
	
	fprintf(fp, "\t<p name=\"fCenterXPos\" value=\"%f\" />\n", fCenterXPos);
	fprintf(fp, "\t<p name=\"fCenterYPos\" value=\"%f\" />\n", fCenterYPos);
	fprintf(fp, "\t<p name=\"fStepSize\" value=\"%f\" />\n", fStepSize);
	fprintf(fp, "\t<p name=\"iNumStepsH\" value=\"%d\" />\n", iNumStepsH);
	fprintf(fp, "\t<p name=\"iNumStepsV\" value=\"%d\" />\n", iNumStepsV);
	fprintf(fp, "\t<p name=\"fPeriphDotSize\" value=\"%f\" />\n", fPeriphDotSize);
	fprintf(fp, "\t<p name=\"fCenterDotSize\" value=\"%f\" />\n", fCenterDotSize);
	fprintf(fp, "\t<p name=\"iFlashInterval\" value=\"%d\" />\n", iFlashInterval);
	fprintf(fp, "\t<p name=\"iFlashDuration\" value=\"%d\" />\n", iFlashDuration);
	fprintf(fp, "\t<p name=\"iCenterColorRadio\" value=\"%d\" />\n", iCenterColorRadio);
	fprintf(fp, "\t<p name=\"iPeriphColorRadio\" value=\"%d\" />\n", iPeriphColorRadio);
	fprintf(fp, "\t<p name=\"iCenterPrimaryRed\" value=\"%d\" />\n", iCenterPrimaryRed);
	fprintf(fp, "\t<p name=\"iCenterPrimaryGreen\" value=\"%d\" />\n", iCenterPrimaryGreen);
	fprintf(fp, "\t<p name=\"iCenterPrimaryBlue\" value=\"%d\" />\n", iCenterPrimaryBlue);
	fprintf(fp, "\t<p name=\"iCenterSecondaryRed\" value=\"%d\" />\n", iCenterSecondaryRed);
	fprintf(fp, "\t<p name=\"iCenterSecondaryGreen\" value=\"%d\" />\n", iCenterSecondaryGreen);
	fprintf(fp, "\t<p name=\"iCenterSecondaryBlue\" value=\"%d\" />\n", iCenterSecondaryBlue);
	fprintf(fp, "\t<p name=\"iPeriphPrimaryRed\" value=\"%d\" />\n", iPeriphPrimaryRed);
	fprintf(fp, "\t<p name=\"iPeriphPrimaryGreen\" value=\"%d\" />\n", iPeriphPrimaryGreen);
	fprintf(fp, "\t<p name=\"iPeriphPrimaryBlue\" value=\"%d\" />\n", iPeriphPrimaryBlue);
	fprintf(fp, "\t<p name=\"iPeriphSecondaryRed\" value=\"%d\" />\n", iPeriphSecondaryRed);
	fprintf(fp, "\t<p name=\"iPeriphSecondaryGreen\" value=\"%d\" />\n", iPeriphSecondaryGreen);
	fprintf(fp, "\t<p name=\"iPeriphSecondaryBlue\" value=\"%d\" />\n", iPeriphSecondaryBlue);
	fprintf(fp, "\t<p name=\"bBehaviorCheck\" value=\"%d\" />\n", bBehaviorCheck);
	fprintf(fp, "\t<p name=\"bStepHoriz\" value=\"%d\" />\n", bStepHoriz);
	fprintf(fp, "\t<p name=\"bStepVert\" value=\"%d\" />\n", bStepVert);
	fprintf(fp, "\t<p name=\"bStep45\" value=\"%d\" />\n", bStep45);
	fprintf(fp, "\t<p name=\"bStep135\" value=\"%d\" />\n", bStep135);
	fprintf(fp, "\t<p name=\"iDistType\" value=\"%d\" />\n", iDistType);
	fprintf(fp, "\t<p name=\"iAlternateBlockSize\" value=\"%d\" />\n", iAlternateBlockSize);
	fprintf(fp, "\t<p name=\"iMaxDelay\" value=\"%d\" />\n", iMaxDelay);
	fprintf(fp, "\t<p name=\"iStimType\" value=\"%d\" />\n", iStimType);
	fprintf(fp, "\t<p name=\"iPostFlashDelay\" value=\"%d\" />\n", iPostFlashDelay);
	fprintf(fp, "\t<p name=\"iIntertrialDelay\" value=\"%d\" />\n", iIntertrialDelay);
	fprintf(fp, "\t<p name=\"bAvoidZero\" value=\"%d\" />\n", bAvoidZero);
	fprintf(fp, "\t<p name=\"bRedBlue\" value=\"%d\" />\n", bRedBlue);
	fprintf(fp, "\t<p name=\"bRandomizeFlashInterval\" value=\"%d\" />\n", bRandomizeFlashInterval);
	fprintf(fp, "\t<p name=\"bRandomizePostFlash\" value=\"%d\" />\n", bRandomizePostFlash);
	fprintf(fp, "\t<p name=\"iPostFlashRandMax\" value=\"%d\" />\n", iPostFlashRandMax);
	fprintf(fp, "\t<p name=\"iPostFlashRandMin\" value=\"%d\" />\n", iPostFlashRandMin);
	fprintf(fp, "\t<p name=\"iFlashIntervalRandMax\" value=\"%d\" />\n", iFlashIntervalRandMax);
	fprintf(fp, "\t<p name=\"iFlashIntervalRandMin\" value=\"%d\" />\n", iFlashIntervalRandMin);
	fprintf(fp, "\t<p name=\"fPeriphXPos\" value=\"%f\" />\n", fPeriphXPos);
	fprintf(fp, "\t<p name=\"fPeriphYPos\" value=\"%f\" />\n", fPeriphYPos);
	fprintf(fp, "\t<p name=\"fPeriphXRadius\" value=\"%f\" />\n", fPeriphXRadius);
	fprintf(fp, "\t<p name=\"fPeriphYRadius\" value=\"%f\" />\n", fPeriphYRadius);
	fprintf(fp, "\t<p name=\"iPctTimeAtPeriph\" value=\"%d\" />\n", iPctTimeAtPeriph);
	fprintf(fp, "\t<p name=\"bSeqRandom\" value=\"%d\" />\n", bSeqRandom);
	fprintf(fp, "\t<p name=\"iPeriphTertiaryRed\" value=\"%d\" />\n", iPeriphTertiaryRed);
	fprintf(fp, "\t<p name=\"iPeriphTertiaryGreen\" value=\"%d\" />\n", iPeriphTertiaryGreen);
	fprintf(fp, "\t<p name=\"iPeriphTertiaryBlue\" value=\"%d\" />\n", iPeriphTertiaryBlue);
	fprintf(fp, "\t<p name=\"iNoiseRed\" value=\"%d\" />\n", iNoiseRed);
	fprintf(fp, "\t<p name=\"iNoiseGreen\" value=\"%d\" />\n", iNoiseGreen);
	fprintf(fp, "\t<p name=\"iNoiseBlue\" value=\"%d\" />\n", iNoiseBlue);
	fprintf(fp, "\t<p name=\"iNumRandomRepeats\" value=\"%d\" />\n", iNumRandomRepeats);
	fprintf(fp, "\t<p name=\"bUseNoise\" value=\"%d\" />\n", bUseNoise);
	fprintf(fp, "\t<p name=\"iNumCatchTrials\" value=\"%d\" />\n", iNumCatchTrials);
	fprintf(fp, "\t<p name=\"bGridSteps\" value=\"%d\" />\n", bGridSteps);
	fprintf(fp, "\t<p name=\"iNoiseSize\" value=\"%d\" />\n", iNoiseSize);
	fprintf(fp, "\t<p name=\"iCrossPix\" value=\"%d\" />\n", iCrossPix);
	fprintf(fp, "\t<p name=\"iNumBlankTrials\" value=\"%d\" />\n", iNumBlankTrials);
	fprintf(fp, "\t<p name=\"iNoisePct\" value=\"%d\" />\n", iNoisePct);
	fprintf(fp, "\t<p name=\"bUseDim\" value=\"%d\" />\n", bUseDim);
	fprintf(fp, "\t<p name=\"iDimOnsetMin\" value=\"%d\" />\n", iDimOnsetMin);
	fprintf(fp, "\t<p name=\"iDimOnsetMax\" value=\"%d\" />\n", iDimOnsetMax);
	fprintf(fp, "\t<p name=\"iDimTimeout\" value=\"%d\" />\n", iDimTimeout);
	fprintf(fp, "\t<p name=\"iAnswerPeriod\" value=\"%d\" />\n", iAnswerPeriod);
	fprintf(fp, "\t<p name=\"iDimBrightness\" value=\"%d\" />\n", iDimBrightness);
	fprintf(fp, "\t<p name=\"bHideCenter\" value=\"%d\" />\n", bHideCenter);
	fprintf(fp, "\t<p name=\"bInterleaveNoise\" value=\"%d\" />\n", bInterleaveNoise);
	fprintf(fp, "\t<p name=\"bCenterPoints\" value=\"%d\" />\n", bCenterPoints);
	fprintf(fp, "\t<p name=\"bOffAxis\" value=\"%d\" />\n", bOffAxis);
	fprintf(fp, "\t<p name=\"bResponse\" value=\"%d\" />\n", bResponse);
	fprintf(fp, "\t<p name=\"fPeriphXPosL\" value=\"%f\" />\n", fPeriphXPosL);
	fprintf(fp, "\t<p name=\"fPeriphYPosL\" value=\"%f\" />\n", fPeriphYPosL);
	fprintf(fp, "\t<p name=\"iResponseTimeout\" value=\"%d\" />\n", iResponseTimeout);
	fprintf(fp, "\t<p name=\"bRampUp\" value=\"%d\" />\n", bRampUp);
	fprintf(fp, "\t<p name=\"iRampDuration\" value=\"%d\" />\n", iRampDuration);

	fprintf(fp, "</preferences>");
	fclose(fp);
}

void CRemember::SendPrefsTextmark(CSerialComm &theComm) 
{
	char buffer[80];
	int i=0;
	sprintf(buffer, "Remember Stimulus Preferences|0x%02x/\0", i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	
	sprintf(buffer, "name=\"fCenterXPos\" value=\"%f\"|0x%02x/\0", fCenterXPos, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"fCenterYPos\" value=\"%f\"|0x%02x/\0", fCenterYPos, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"fStepSize\" value=\"%f\"|0x%02x/\0", fStepSize, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iNumStepsH\" value=\"%d\"|0x%02x/\0", iNumStepsH, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iNumStepsV\" value=\"%d\"|0x%02x/\0", iNumStepsV, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"fPeriphDotSize\" value=\"%f\"|0x%02x/\0", fPeriphDotSize, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"fCenterDotSize\" value=\"%f\"|0x%02x/\0", fCenterDotSize, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iFlashInterval\" value=\"%d\"|0x%02x/\0", iFlashInterval, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iFlashDuration\" value=\"%d\"|0x%02x/\0", iFlashDuration, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iCenterColorRadio\" value=\"%d\"|0x%02x/\0", iCenterColorRadio, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPeriphColorRadio\" value=\"%d\"|0x%02x/\0", iPeriphColorRadio, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iCenterPrimaryRed\" value=\"%d\"|0x%02x/\0", iCenterPrimaryRed, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iCenterPrimaryGreen\" value=\"%d\"|0x%02x/\0", iCenterPrimaryGreen, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iCenterPrimaryBlue\" value=\"%d\"|0x%02x/\0", iCenterPrimaryBlue, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iCenterSecondaryRed\" value=\"%d\"|0x%02x/\0", iCenterSecondaryRed, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iCenterSecondaryGreen\" value=\"%d\"|0x%02x/\0", iCenterSecondaryGreen, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iCenterSecondaryBlue\" value=\"%d\"|0x%02x/\0", iCenterSecondaryBlue, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPeriphPrimaryRed\" value=\"%d\"|0x%02x/\0", iPeriphPrimaryRed, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPeriphPrimaryGreen\" value=\"%d\"|0x%02x/\0", iPeriphPrimaryGreen, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPeriphPrimaryBlue\" value=\"%d\"|0x%02x/\0", iPeriphPrimaryBlue, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPeriphSecondaryRed\" value=\"%d\"|0x%02x/\0", iPeriphSecondaryRed, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPeriphSecondaryGreen\" value=\"%d\"|0x%02x/\0", iPeriphSecondaryGreen, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPeriphSecondaryBlue\" value=\"%d\"|0x%02x/\0", iPeriphSecondaryBlue, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bBehaviorCheck\" value=\"%d\"|0x%02x/\0", bBehaviorCheck, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bStepHoriz\" value=\"%d\"|0x%02x/\0", bStepHoriz, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bStepVert\" value=\"%d\"|0x%02x/\0", bStepVert, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bStep45\" value=\"%d\"|0x%02x/\0", bStep45, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bStep135\" value=\"%d\"|0x%02x/\0", bStep135, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iDistType\" value=\"%d\"|0x%02x/\0", iDistType, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iAlternateBlockSize\" value=\"%d\"|0x%02x/\0", iAlternateBlockSize, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iMaxDelay\" value=\"%d\"|0x%02x/\0", iMaxDelay, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iStimType\" value=\"%d\"|0x%02x/\0", iStimType, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPostFlashDelay\" value=\"%d\"|0x%02x/\0", iPostFlashDelay, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iIntertrialDelay\" value=\"%d\"|0x%02x/\0", iIntertrialDelay, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bAvoidZero\" value=\"%d\"|0x%02x/\0", bAvoidZero, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bRedBlue\" value=\"%d\"|0x%02x/\0", bRedBlue, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bRandomizeFlashInterval\" value=\"%d\"|0x%02x/\0", bRandomizeFlashInterval, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bRandomizePostFlash\" value=\"%d\"|0x%02x/\0", bRandomizePostFlash, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPostFlashRandMax\" value=\"%d\"|0x%02x/\0", iPostFlashRandMax, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPostFlashRandMin\" value=\"%d\"|0x%02x/\0", iPostFlashRandMin, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iFlashIntervalRandMax\" value=\"%d\"|0x%02x/\0", iFlashIntervalRandMax, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iFlashIntervalRandMin\" value=\"%d\"|0x%02x/\0", iFlashIntervalRandMin, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"fPeriphXPos\" value=\"%f\"|0x%02x/\0", fPeriphXPos, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"fPeriphYPos\" value=\"%f\"|0x%02x/\0", fPeriphYPos, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"fPeriphXRadius\" value=\"%f\"|0x%02x/\0", fPeriphXRadius, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"fPeriphYRadius\" value=\"%f\"|0x%02x/\0", fPeriphYRadius, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPctTimeAtPeriph\" value=\"%d\"|0x%02x/\0", iPctTimeAtPeriph, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bSeqRandom\" value=\"%d\"|0x%02x/\0", bSeqRandom, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPeriphTertiaryRed\" value=\"%d\"|0x%02x/\0", iPeriphTertiaryRed, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPeriphTertiaryGreen\" value=\"%d\"|0x%02x/\0", iPeriphTertiaryGreen, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPeriphTertiaryBlue\" value=\"%d\"|0x%02x/\0", iPeriphTertiaryBlue, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iNoiseRed\" value=\"%d\"|0x%02x/\0", iNoiseRed, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iNoiseGreen\" value=\"%d\"|0x%02x/\0", iNoiseGreen, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iNoiseBlue\" value=\"%d\"|0x%02x/\0", iNoiseBlue, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iNumRandomRepeats\" value=\"%d\"|0x%02x/\0", iNumRandomRepeats, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bUseNoise\" value=\"%d\"|0x%02x/\0", bUseNoise, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iNumCatchTrials\" value=\"%d\"|0x%02x/\0", iNumCatchTrials, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bGridSteps\" value=\"%d\"|0x%02x/\0", bGridSteps, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iNoiseSize\" value=\"%d\"|0x%02x/\0", iNoiseSize, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iCrossPix\" value=\"%d\"|0x%02x/\0", iCrossPix, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iNumBlankTrials\" value=\"%d\"|0x%02x/\0", iNumBlankTrials, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iNoisePct\" value=\"%d\"|0x%02x/\0", iNoisePct, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bUseDim\" value=\"%d\"|0x%02x/\0", bUseDim, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iDimOnsetMin\" value=\"%d\"|0x%02x/\0", iDimOnsetMin, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iDimOnsetMax\" value=\"%d\"|0x%02x/\0", iDimOnsetMax, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iDimTimeout\" value=\"%d\"|0x%02x/\0", iDimTimeout, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iAnswerPeriod\" value=\"%d\"|0x%02x/\0", iAnswerPeriod, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iDimBrightness\" value=\"%d\"|0x%02x/\0", iDimBrightness, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bHideCenter\" value=\"%d\"|0x%02x/\0", bHideCenter, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bInterleaveNoise\" value=\"%d\"|0x%02x/\0", bInterleaveNoise, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bCenterPoints\" value=\"%d\"|0x%02x/\0", bCenterPoints, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bOffAxis\" value=\"%d\"|0x%02x/\0", bOffAxis, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bResponse\" value=\"%d\"|0x%02x/\0", bResponse, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"fPeriphXPosL\" value=\"%f\"|0x%02x/\0", fPeriphXPosL, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"fPeriphYPosL\" value=\"%f\"|0x%02x/\0", fPeriphYPosL, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iResponseTimeout\" value=\"%d\"|0x%02x/\0", iResponseTimeout, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"bRampUp\" value=\"%d\"|0x%02x/\0", bRampUp, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iRampDuration\" value=\"%d\"|0x%02x/\0", iRampDuration, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iNumCatchTrials2\" value=\"%d\"|0x%02x/\0", iNumCatchTrials2, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"iPctTimeAtCenter\" value=\"%d\"|0x%02x/\0", iPctTimeAtCenter, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"fCenterXRadius\" value=\"%d\"|0x%02x/\0", fCenterXRadius, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
	sprintf(buffer, "name=\"fCenterYRadius\" value=\"%d\"|0x%02x/\0", fCenterYRadius, i);theComm.transmit(buffer, (int)strlen(buffer)); i++;
}

void CRemember::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "Scotoma Mapping;CenterXPos;%.4f;%ld|0x%02x/\0", fCenterXPos, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;CenterYPos;%.4f;%ld|0x%02x/\0", fCenterYPos, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;StepSize;%.4f;%ld|0x%02x/\0", fStepSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;NumStepsH;%d;%ld|0x%02x/\0", iNumStepsH, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;NumStepsV;%d;%ld|0x%02x/\0", iNumStepsV, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphDotSize;%.4f;%ld|0x%02x/\0", fPeriphDotSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;CenterDotSize;%.4f;%ld|0x%02x/\0", fCenterDotSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;FlashInterval;%d;%ld|0x%02x/\0", iFlashInterval, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;FlashDuration;%d;%ld|0x%02x/\0", iFlashDuration, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;CenterColorRadio;%d;%ld|0x%02x/\0", iCenterColorRadio, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphColorRadio;%d;%ld|0x%02x/\0", iPeriphColorRadio, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;CenterPrimaryRed;%d;%ld|0x%02x/\0", iCenterPrimaryRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;CenterPrimaryGreen;%d;%ld|0x%02x/\0", iCenterPrimaryGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;CenterPrimaryBlue;%d;%ld|0x%02x/\0", iCenterPrimaryBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;CenterSecondaryRed;%d;%ld|0x%02x/\0", iCenterSecondaryRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;CenterSecondaryGreen;%d;%ld|0x%02x/\0", iCenterSecondaryGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;CenterSecondaryBlue;%d;%ld|0x%02x/\0", iCenterSecondaryBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphPrimaryRed;%d;%ld|0x%02x/\0", iPeriphPrimaryRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphPrimaryGreen;%d;%ld|0x%02x/\0", iPeriphPrimaryGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphPrimaryBlue;%d;%ld|0x%02x/\0", iPeriphPrimaryBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphSecondaryRed;%d;%ld|0x%02x/\0", iPeriphSecondaryRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphSecondaryGreen;%d;%ld|0x%02x/\0", iPeriphSecondaryGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphSecondaryBlue;%d;%ld|0x%02x/\0", iPeriphSecondaryBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;BehaviorCheck;%d;%ld|0x%02x/\0", bBehaviorCheck, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;StepHoriz;%d;%ld|0x%02x/\0", bStepHoriz, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;StepVert;%d;%ld|0x%02x/\0", bStepVert, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;Step45;%d;%ld|0x%02x/\0", bStep45, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;Step135;%d;%ld|0x%02x/\0", bStep135, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;DistType;%d;%ld|0x%02x/\0", iDistType, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;AlternateBlockSize;%d;%ld|0x%02x/\0", iAlternateBlockSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;MaxDelay;%d;%ld|0x%02x/\0", iMaxDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;StimType;%d;%ld|0x%02x/\0", iStimType, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PostFlashDelay;%d;%ld|0x%02x/\0", iPostFlashDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;IntertrialDelay;%d;%ld|0x%02x/\0", iIntertrialDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;AvoidZero;%d;%ld|0x%02x/\0", bAvoidZero, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;RedBlue;%d;%ld|0x%02x/\0", bRedBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;RandomizeFlashInterval;%d;%ld|0x%02x/\0", bRandomizeFlashInterval, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;RandomizePostFlash;%d;%ld|0x%02x/\0", bRandomizePostFlash, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PostFlashRandMax;%d;%ld|0x%02x/\0", iPostFlashRandMax, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PostFlashRandMin;%d;%ld|0x%02x/\0", iPostFlashRandMin, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;FlashIntervalRandMax;%d;%ld|0x%02x/\0", iFlashIntervalRandMax, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;FlashIntervalRandMin;%d;%ld|0x%02x/\0", iFlashIntervalRandMin, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphXPos;%.4f;%ld|0x%02x/\0", fPeriphXPos, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphYPos;%.4f;%ld|0x%02x/\0", fPeriphYPos, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphXRadius;%.4f;%ld|0x%02x/\0", fPeriphXRadius, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphYRadius;%.4f;%ld|0x%02x/\0", fPeriphYRadius, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PctTimeAtPeriph;%d;%ld|0x%02x/\0", iPctTimeAtPeriph, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;SeqRandom;%d;%ld|0x%02x/\0", bSeqRandom, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;SeqRandom;%d;%ld|0x%02x/\0", bSeqRandom, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphTertiaryRed;%d;%ld|0x%02x/\0", iPeriphTertiaryRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphTertiaryRed;%d;%ld|0x%02x/\0", iPeriphTertiaryRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphTertiaryGreen;%d;%ld|0x%02x/\0", iPeriphTertiaryGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphTertiaryBlue;%d;%ld|0x%02x/\0", iPeriphTertiaryBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;NoiseRed;%d;%ld|0x%02x/\0", iNoiseRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;NoiseGreen;%d;%ld|0x%02x/\0", iNoiseGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;NoiseBlue;%d;%ld|0x%02x/\0", iNoiseBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;NumRandomRepeats;%d;%ld|0x%02x/\0", iNumRandomRepeats, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;UseNoise;%d;%ld|0x%02x/\0", bUseNoise, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;NumCatchTrials;%d;%ld|0x%02x/\0", iNumCatchTrials, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;GridSteps;%d;%ld|0x%02x/\0", bGridSteps, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;NoiseSize;%d;%ld|0x%02x/\0", iNoiseSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;CrossPix;%d;%ld|0x%02x/\0", iCrossPix, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;NumBlankTrials;%d;%ld|0x%02x/\0", iNumBlankTrials, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;NoisePct;%d;%ld|0x%02x/\0", iNoisePct, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;UseDim;%d;%ld|0x%02x/\0", bUseDim, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;DimOnsetMin;%d;%ld|0x%02x/\0", iDimOnsetMin, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;DimOnsetMax;%d;%ld|0x%02x/\0", iDimOnsetMax, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;DimTimeout;%d;%ld|0x%02x/\0", iDimTimeout, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;AnswerPeriod;%d;%ld|0x%02x/\0", iAnswerPeriod, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;DimBrightness;%d;%ld|0x%02x/\0", iDimBrightness, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;HideCenter;%d;%ld|0x%02x/\0", bHideCenter, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;InterleaveNoise;%d;%ld|0x%02x/\0", bInterleaveNoise, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;CenterPoints;%d;%ld|0x%02x/\0", bCenterPoints, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;OffAxis;%d;%ld|0x%02x/\0", bOffAxis, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;Response;%d;%ld|0x%02x/\0", bResponse, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphXPosL;%.4f;%ld|0x%02x/\0", fPeriphXPosL, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PeriphYPosL;%.4f;%ld|0x%02x/\0", fPeriphYPosL, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;ResponseTimeout;%d;%ld|0x%02x/\0", iResponseTimeout, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;RampUp;%d;%ld|0x%02x/\0", bRampUp, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;RampDuration;%d;%ld|0x%02x/\0", iRampDuration, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;NumCatchTrials2;%d;%ld|0x%02x/\0", iNumCatchTrials2, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;PctTimeAtCenter;%d;%ld|0x%02x/\0", iPctTimeAtCenter, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;CenterXRadius;%.4f;%ld|0x%02x/\0", fCenterXRadius, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;CenterYRadius;%.4f;%ld|0x%02x/\0", fCenterYRadius, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Mapping;AutoPosition;%d;%ld|0x%02x/\0", bAutoPosition, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}
