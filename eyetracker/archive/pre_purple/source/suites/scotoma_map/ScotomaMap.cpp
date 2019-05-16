/**
 * @file ScotomaMap.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include <deque>
#include <iterator>
#include <algorithm>
#include <fstream>
#include "ScotomaMap.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"
#include "utils/Asserts.h"

/*****************************************************************************
 * Defines
 *****************************************************************************/

/**
 * @brief Force input percent to be in range [0.0, 1.0].
 * @param pct Any number, a double.
 * @return 0.0 if pct < 0.0; 1.0 if pct > 1.0; pct otherwise.
 */
#define NORMALIZE_PERCENT(pct)       \
	((pct) < 0.0 ? 0.0 :             \
		((pct) > 1.0 ? 1.0 : (pct)))

/*****************************************************************************
 * Statics and Globals
 *****************************************************************************/

/**
 * @brief Type of trial, given by peripheral color and point generation.  
 *
 * Colors are red, blue, blank, and purple.  Point generations types are grid,
 * peripheral foveal enrichment, central foveal enrichment, and user-specified
 * foveal enrichment.
 */
static enum TRIAL_TYPE {
	TT_RED_GRID = 0,  /**< Red peripheral target from grid. */
	TT_RED_PF,        /**< Red target from peripheral foveal enrichment */
	TT_RED_CF,        /**< Red target from central foveal enrichment */
	TT_RED_E,         /**< Red target for user-specified foveal enrichment */
	TT_BLUE_GRID,     /**< Blue peripheral target from grid. */
	TT_BLUE_PF,       /**< Blue target from peripheral foveal enrichment */
	TT_BLUE_CF,       /**< Blue target from central foveal enrichment */
	TT_BLUE_E,        /**< Blue target for user-specified foveal enrichment */
	TT_BLANK_GRID,    /**< Blank peripheral target from grid. */
	TT_BLANK_PF,      /**< Blank target from peripheral foveal enrichment */
	TT_BLANK_CF,      /**< Blank target from central foveal enrichment */
	TT_BLANK_E,       /**< Blank target for user-specified foveal enrichment */
	TT_PURPLE_GRID,   /**< Purple peripheral target from grid. */
	TT_PURPLE_PF,     /**< Purple target from peripheral foveal enrichment */
	TT_PURPLE_CF,     /**< Purple target from central foveal enrichment */
	TT_PURPLE_E,      /**< Purple target for user-specified foveal enrichment */
	NUM_TRIAL_TYPES   /**< Count of number of types */
};

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/* text label coding:
 * code	center	periph
 * 0x0		pri		pri
 * 0x1		pri		sec
 * 0x2		sec		pri
 * 0x3		sec		sec
 * 
 * 0x4		pri		ter
 * 0x5		sec		ter
 * 
 * 0x6		pri		blank
 * 0x7		sec		blank
 */

CScotomaMap::CScotomaMap()
{
	defaultFile = CString("C:\\Stimulus Settings\\ScotomaMap\\default.txt");
	
	iPrimaryRed = 255;
	iPrimaryGreen = 0;
	iPrimaryBlue = 0;
	iSecondaryRed = 0;
	iSecondaryGreen = 0;
	iSecondaryBlue = 255;
	iTertiaryRed = 255;
	iTertiaryGreen = 0;
	iTertiaryBlue = 255;

	m_stepSizeDeg = 0;
	m_numStepsX = 0;
	m_numStepsY = 0;
	m_numCtrLocations = 0;
	m_ctrLocationsX = NULL;
	m_ctrLocationsY = NULL;
	
	iNoiseRed = 50;
	iNoiseGreen = 0;
	iNoiseBlue = 50;
	
	iResponseDelay = 1000;
	iWrongDelay = 5000;
	
	iNumRandomRepeats = 10;
	
	iNoisePct = 50;
	iNoiseSize = 6;
	iRampDuration = 0;
	
	iDisplayTime = 500;
	iDisplayRandMax = 100;
	iDisplayRandMin = 500;
	iFlashInterval = 100;
	iFlashIntervalRandMax = 500;
	iFlashIntervalRandMin = 250;
	iCrossPix = 3;
	
	iCenterColorRadio = COLOR_ALTERNATE;
	iAlternateBlockSize = 10;
	
	iPeriphColorRadio = COLOR_RANDOM;
	iNumCatchTrials = 50;
	iNumCatchTrials2 = 50;
	iNumBlankTrials = 0;
	
	iNumStepsH = 4;
	iNumStepsV = 3;
	
	iPctTimeAtCenter = 0;
	iPctTimeAtPeriph = 0;
	
	bUseNoise = true;
	bInterleaveNoise = false;
	
	bRampUp = false;
	bHideCenter = false;
	bRandomizeFlashInterval = false;
	bRandomizeDisplayTime = false;
	bAvoidZero = true;
	bCenterPoints = false;
	bOffAxis = false;

	fCenterXPos = 0.0;
	fCenterYPos = 0.0;
	fCenterDotSize = 1.0;
	fPeriphDotSize = 1.0;
	fStepSize = 5.0;

	fCenterXRadius = 2.5;
	fCenterYRadius = 2.5;
	fPeriphXRadius = 2.5;
	fPeriphYRadius = 2.5;
	iMisbehaviorDelay = 500;
	iRewardScheme = 0;
	iTertiaryRewardPct = 50;
	iRewardDelay = 500;
	bDisplayDuringChoice = false;
	bUseShapes = false;
	bSameBG = true;
	bOccludeBG = true;
	iPctTimeAtEnriched = 0;
	fEnrichedX = 20.0;
	fEnrichedY = 0.0;
	fEnrichedXRadius = 5.0;
	fEnrichedYRadius = 5.0;
	bDotGratings = false;
	iDotGratingContrast = 90;
	fDotGratingCycles = 3.0;
	fSeparationV = 0.0;
	bUseKeyboard = false;

	iNumLocations = 0;
	LoadFile(true);
}

/**
 * @brief Clean up trials and center locations.
 */
CScotomaMap::~CScotomaMap() 
{
	CleanUpTrials();
	CleanUpCtrLocations();
}

/**
 * @brief Load settings from file.
 * @param bDefault True to use default file, false to prompt user for a file.
 */
void CScotomaMap::LoadFile(bool bDefault) 
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(true, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	FILE* fp = fopen(filename, "rt");
	if (!fp) {
		AfxMessageBox("Error loading file - file or folder not found");
		return;
	}

	fscanf(fp, "%d\n", &iPrimaryRed);
	fscanf(fp, "%d\n", &iPrimaryGreen);
	fscanf(fp, "%d\n", &iPrimaryBlue);
	fscanf(fp, "%d\n", &iSecondaryRed);
	fscanf(fp, "%d\n", &iSecondaryGreen);
	fscanf(fp, "%d\n", &iSecondaryBlue);
	fscanf(fp, "%d\n", &iTertiaryRed);
	fscanf(fp, "%d\n", &iTertiaryGreen);
	fscanf(fp, "%d\n", &iTertiaryBlue);
	
	fscanf(fp, "%d\n", &iNoiseRed);
	fscanf(fp, "%d\n", &iNoiseGreen);
	fscanf(fp, "%d\n", &iNoiseBlue);
	
	fscanf(fp, "%d\n", &iResponseDelay);
	fscanf(fp, "%d\n", &iWrongDelay);
	
	fscanf(fp, "%d\n", &iNumRandomRepeats);
	
	fscanf(fp, "%d\n", &iNoisePct);
	fscanf(fp, "%d\n", &iNoiseSize);
	fscanf(fp, "%d\n", &iRampDuration);
	
	fscanf(fp, "%d\n", &iDisplayTime);
	fscanf(fp, "%d\n", &iDisplayRandMax);
	fscanf(fp, "%d\n", &iDisplayRandMin);
	fscanf(fp, "%d\n", &iFlashInterval);
	fscanf(fp, "%d\n", &iFlashIntervalRandMax);
	fscanf(fp, "%d\n", &iFlashIntervalRandMin);
	fscanf(fp, "%d\n", &iCrossPix);
	
	fscanf(fp, "%d\n", &iCenterColorRadio);
	fscanf(fp, "%d\n", &iAlternateBlockSize);
	
	fscanf(fp, "%d\n", &iPeriphColorRadio);
	fscanf(fp, "%d\n", &iNumCatchTrials);
	fscanf(fp, "%d\n", &iNumCatchTrials2);
	fscanf(fp, "%d\n", &iNumBlankTrials);
	
	fscanf(fp, "%d\n", &iNumStepsH);
	fscanf(fp, "%d\n", &iNumStepsV);
	
	fscanf(fp, "%d\n", &iPctTimeAtCenter);
	fscanf(fp, "%d\n", &iPctTimeAtPeriph);
	
	fscanf(fp, "%d\n", &bUseNoise);
	fscanf(fp, "%d\n", &bInterleaveNoise);
	
	fscanf(fp, "%d\n", &bRampUp);
	fscanf(fp, "%d\n", &bHideCenter);
	fscanf(fp, "%d\n", &bRandomizeFlashInterval);
	fscanf(fp, "%d\n", &bRandomizeDisplayTime);
	fscanf(fp, "%d\n", &bAvoidZero);
	fscanf(fp, "%d\n", &bCenterPoints);
	fscanf(fp, "%d\n", &bOffAxis);

	fscanf(fp, "%f\n", &fCenterXPos);
	fscanf(fp, "%f\n", &fCenterYPos);
	fscanf(fp, "%f\n", &fCenterDotSize);
	fscanf(fp, "%f\n", &fPeriphDotSize);
	fscanf(fp, "%f\n", &fStepSize);

	fscanf(fp, "%f\n", &m_stepSizeDeg);
	fscanf(fp, "%d\n", &m_numStepsX);
	fscanf(fp, "%d\n", &m_numStepsY);

	fscanf(fp, "%f\n", &fCenterXRadius);
	fscanf(fp, "%f\n", &fCenterYRadius);
	fscanf(fp, "%f\n", &fPeriphXRadius);
	fscanf(fp, "%f\n", &fPeriphYRadius);
	fscanf(fp, "%d\n", &iMisbehaviorDelay);
	fscanf(fp, "%d\n", &iRewardScheme);
	fscanf(fp, "%d\n", &iTertiaryRewardPct);
	fscanf(fp, "%d\n", &iRewardDelay);
	fscanf(fp, "%d\n", &bDisplayDuringChoice);
	fscanf(fp, "%d\n", &bUseShapes);
	fscanf(fp, "%d\n", &bSameBG);
	fscanf(fp, "%d\n", &bOccludeBG);
	fscanf(fp, "%d\n", &iPctTimeAtEnriched);
	fscanf(fp, "%f\n", &fEnrichedX);
	fscanf(fp, "%f\n", &fEnrichedY);
	fscanf(fp, "%f\n", &fEnrichedXRadius);
	fscanf(fp, "%f\n", &fEnrichedYRadius);
	fscanf(fp, "%d\n", &bDotGratings);
	fscanf(fp, "%d\n", &iDotGratingContrast);
	fscanf(fp, "%f\n", &fDotGratingCycles);
	fscanf(fp, "%f\n", &fSeparationV);
	fscanf(fp, "%d\n", &bUseKeyboard);

	fclose(fp);
}

/**
 * @brief Save settings to file.
 * @param bDefault True to use default file, false to prompt user for a file.
 */
void CScotomaMap::SaveFile(bool bDefault) 
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(false, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	FILE* fp = fopen(filename, "wt");
	if (!fp) {
		AfxMessageBox("Error saving file - file or folder not found");
		return;
	}
	
	fprintf(fp, "%d\n", iPrimaryRed);
	fprintf(fp, "%d\n", iPrimaryGreen);
	fprintf(fp, "%d\n", iPrimaryBlue);
	fprintf(fp, "%d\n", iSecondaryRed);
	fprintf(fp, "%d\n", iSecondaryGreen);
	fprintf(fp, "%d\n", iSecondaryBlue);
	fprintf(fp, "%d\n", iTertiaryRed);
	fprintf(fp, "%d\n", iTertiaryGreen);
	fprintf(fp, "%d\n", iTertiaryBlue);
	
	fprintf(fp, "%d\n", iNoiseRed);
	fprintf(fp, "%d\n", iNoiseGreen);
	fprintf(fp, "%d\n", iNoiseBlue);
	
	fprintf(fp, "%d\n", iResponseDelay);
	fprintf(fp, "%d\n", iWrongDelay);
	
	fprintf(fp, "%d\n", iNumRandomRepeats);
	
	fprintf(fp, "%d\n", iNoisePct);
	fprintf(fp, "%d\n", iNoiseSize);
	fprintf(fp, "%d\n", iRampDuration);
	
	fprintf(fp, "%d\n", iDisplayTime);
	fprintf(fp, "%d\n", iDisplayRandMax);
	fprintf(fp, "%d\n", iDisplayRandMin);
	fprintf(fp, "%d\n", iFlashInterval);
	fprintf(fp, "%d\n", iFlashIntervalRandMax);
	fprintf(fp, "%d\n", iFlashIntervalRandMin);
	fprintf(fp, "%d\n", iCrossPix);
	
	fprintf(fp, "%d\n", iCenterColorRadio);
	fprintf(fp, "%d\n", iAlternateBlockSize);
	
	fprintf(fp, "%d\n", iPeriphColorRadio);
	fprintf(fp, "%d\n", iNumCatchTrials);
	fprintf(fp, "%d\n", iNumCatchTrials2);
	fprintf(fp, "%d\n", iNumBlankTrials);
	
	fprintf(fp, "%d\n", iNumStepsH);
	fprintf(fp, "%d\n", iNumStepsV);
	
	fprintf(fp, "%d\n", iPctTimeAtCenter);
	fprintf(fp, "%d\n", iPctTimeAtPeriph);
	
	fprintf(fp, "%d\n", bUseNoise);
	fprintf(fp, "%d\n", bInterleaveNoise);
	
	fprintf(fp, "%d\n", bRampUp);
	fprintf(fp, "%d\n", bHideCenter);
	fprintf(fp, "%d\n", bRandomizeFlashInterval);
	fprintf(fp, "%d\n", bRandomizeDisplayTime);
	fprintf(fp, "%d\n", bAvoidZero);
	fprintf(fp, "%d\n", bCenterPoints);
	fprintf(fp, "%d\n", bOffAxis);

	fprintf(fp, "%f\n", fCenterXPos);
	fprintf(fp, "%f\n", fCenterYPos);
	fprintf(fp, "%f\n", fCenterDotSize);
	fprintf(fp, "%f\n", fPeriphDotSize);
	fprintf(fp, "%f\n", fStepSize);

	fprintf(fp, "%f\n", m_stepSizeDeg);
	fprintf(fp, "%d\n", m_numStepsX);
	fprintf(fp, "%d\n", m_numStepsY);

	fprintf(fp, "%f\n", fCenterXRadius);
	fprintf(fp, "%f\n", fCenterYRadius);
	fprintf(fp, "%f\n", fPeriphXRadius);
	fprintf(fp, "%f\n", fPeriphYRadius);
	fprintf(fp, "%d\n", iMisbehaviorDelay);
	fprintf(fp, "%d\n", iRewardScheme);
	fprintf(fp, "%d\n", iTertiaryRewardPct);
	fprintf(fp, "%d\n", iRewardDelay);
	fprintf(fp, "%d\n", bDisplayDuringChoice);
	fprintf(fp, "%d\n", bUseShapes);
	fprintf(fp, "%d\n", bSameBG);
	fprintf(fp, "%d\n", bOccludeBG);
	fprintf(fp, "%d\n", iPctTimeAtEnriched);
	fprintf(fp, "%f\n", fEnrichedX);
	fprintf(fp, "%f\n", fEnrichedY);
	fprintf(fp, "%f\n", fEnrichedXRadius);
	fprintf(fp, "%f\n", fEnrichedYRadius);
	fprintf(fp, "%d\n", bDotGratings);
	fprintf(fp, "%d\n", iDotGratingContrast);
	fprintf(fp, "%f\n", fDotGratingCycles);
	fprintf(fp, "%f\n", fSeparationV);
	fprintf(fp, "%d\n", bUseKeyboard);
	
	fclose(fp);
}

/**
 * @brief Sends settings over serial port.
 * @param theComm Communications object to use for settings.
 */
void CScotomaMap::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "Scotoma Monkey;PrimaryRed;%d;%ld|0x%02x/\0", iPrimaryRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;PrimaryGreen;%d;%ld|0x%02x/\0", iPrimaryGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;PrimaryBlue;%d;%ld|0x%02x/\0", iPrimaryBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;SecondaryRed;%d;%ld|0x%02x/\0", iSecondaryRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;SecondaryGreen;%d;%ld|0x%02x/\0", iSecondaryGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;SecondaryBlue;%d;%ld|0x%02x/\0", iSecondaryBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;TertiaryRed;%d;%ld|0x%02x/\0", iTertiaryRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;TertiaryGreen;%d;%ld|0x%02x/\0", iTertiaryGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;TertiaryBlue;%d;%ld|0x%02x/\0", iTertiaryBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NoiseRed;%d;%ld|0x%02x/\0", iNoiseRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NoiseGreen;%d;%ld|0x%02x/\0", iNoiseGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NoiseBlue;%d;%ld|0x%02x/\0", iNoiseBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;ResponseDelay;%d;%ld|0x%02x/\0", iResponseDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;WrongDelay;%d;%ld|0x%02x/\0", iWrongDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NumRandomRepeats;%d;%ld|0x%02x/\0", iNumRandomRepeats, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NoisePct;%d;%ld|0x%02x/\0", iNoisePct, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NoiseSize;%d;%ld|0x%02x/\0", iNoiseSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;RampDuration;%d;%ld|0x%02x/\0", iRampDuration, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;DisplayTime;%d;%ld|0x%02x/\0", iDisplayTime, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;DisplayRandMax;%d;%ld|0x%02x/\0", iDisplayRandMax, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;DisplayRandMin;%d;%ld|0x%02x/\0", iDisplayRandMin, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;FlashInterval;%d;%ld|0x%02x/\0", iFlashInterval, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;FlashIntervalRandMax;%d;%ld|0x%02x/\0", iFlashIntervalRandMax, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;FlashIntervalRandMin;%d;%ld|0x%02x/\0", iFlashIntervalRandMin, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;CrossPix;%d;%ld|0x%02x/\0", iCrossPix, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;CenterColorRadio;%d;%ld|0x%02x/\0", iCenterColorRadio, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;AlternateBlockSize;%d;%ld|0x%02x/\0", iAlternateBlockSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;PeriphColorRadio;%d;%ld|0x%02x/\0", iPeriphColorRadio, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NumCatchTrials;%d;%ld|0x%02x/\0", iNumCatchTrials, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NumCatchTrials2;%d;%ld|0x%02x/\0", iNumCatchTrials2, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NumBlankTrials;%d;%ld|0x%02x/\0", iNumBlankTrials, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NumStepsH;%d;%ld|0x%02x/\0", iNumStepsH, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NumStepsV;%d;%ld|0x%02x/\0", iNumStepsV, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;PctTimeAtCenter;%d;%ld|0x%02x/\0", iPctTimeAtCenter, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;PctTimeAtPeriph;%d;%ld|0x%02x/\0", iPctTimeAtPeriph, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NumLocations;%ld;%ld|0x%02x/\0", iNumLocations, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;UseNoise;%d;%ld|0x%02x/\0", bUseNoise, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;InterleaveNoise;%d;%ld|0x%02x/\0", bInterleaveNoise, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;RampUp;%d;%ld|0x%02x/\0", bRampUp, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;HideCenter;%d;%ld|0x%02x/\0", bHideCenter, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;RandomizeFlashInterval;%d;%ld|0x%02x/\0", bRandomizeFlashInterval, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;RandomizeDisplayTime;%d;%ld|0x%02x/\0", bRandomizeDisplayTime, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;AvoidZero;%d;%ld|0x%02x/\0", bAvoidZero, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;CenterPoints;%d;%ld|0x%02x/\0", bCenterPoints, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;OffAxis;%d;%ld|0x%02x/\0", bOffAxis, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;CenterXPos;%.4f;%ld|0x%02x/\0", fCenterXPos, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;CenterYPos;%.4f;%ld|0x%02x/\0", fCenterYPos, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));

	sprintf(buffer, "Scotoma Monkey;StepSizeDeg;%.4f;%ld|0x%02x/\0", m_stepSizeDeg, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NumStepsX;%d;%ld|0x%02x/\0", m_numStepsX, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;NumStepsY;%d;%ld|0x%02x/\0", m_numStepsY, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	
	sprintf(buffer, "Scotoma Monkey;CenterDotSize;%.4f;%ld|0x%02x/\0", fCenterDotSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;PeriphDotSize;%.4f;%ld|0x%02x/\0", fPeriphDotSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;StepSize;%.4f;%ld|0x%02x/\0", fStepSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;CenterXRadius;%.4f;%ld|0x%02x/\0", fCenterXRadius, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;CenterYRadius;%.4f;%ld|0x%02x/\0", fCenterYRadius, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;PeriphXRadius;%.4f;%ld|0x%02x/\0", fPeriphXRadius, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;PeriphYRadius;%.4f;%ld|0x%02x/\0", fPeriphYRadius, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;MisbehaviorDelay;%d;%ld|0x%02x/\0", iMisbehaviorDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;RewardScheme;%d;%ld|0x%02x/\0", iRewardScheme, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;TertiaryRewardPct;%d;%ld|0x%02x/\0", iTertiaryRewardPct, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;RewardDelay;%d;%ld|0x%02x/\0", iRewardDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;DisplayDuringChoice;%d;%ld|0x%02x/\0", bDisplayDuringChoice, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;UseShapes;%d;%ld|0x%02x/\0", bUseShapes, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;SameBG;%d;%ld|0x%02x/\0", bSameBG, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;OccludeBG;%d;%ld|0x%02x/\0", bOccludeBG, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;PctTimeAtEnriched;%d;%ld|0x%02x/\0", iPctTimeAtEnriched, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;EnrichedX;%.4f;%ld|0x%02x/\0", fEnrichedX, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;EnrichedY;%.4f;%ld|0x%02x/\0", fEnrichedY, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;EnrichedXRadius;%.4f;%ld|0x%02x/\0", fEnrichedXRadius, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;EnrichedYRadius;%.4f;%ld|0x%02x/\0", fEnrichedYRadius, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;DotGratings;%d;%ld|0x%02x/\0", bDotGratings, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;DotGratingContrast;%d;%ld|0x%02x/\0", iDotGratingContrast, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;DotGratingCycles;%.4f;%ld|0x%02x/\0", fDotGratingCycles, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;SeparationV;%.4f;%ld|0x%02x/\0", fSeparationV, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Scotoma Monkey;UseKeyboard;%d;%ld|0x%02x/\0", bUseKeyboard, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}

/**
 * @brief Generates member array of locations for the center fixation target.
 *
 * Very similar to function in saccade model; this one is simplified and 
 * supports only X and Y steps in a grid w/zero point.
 */
void CScotomaMap::GenerateCtrLocations() 
{
	int currLoc;
	
	/* Steps are from center position in a given direction.  Thus total side 
	 * length is 2s + 1, where s is the number of steps.  Total area is 
	 * (length)x(width). */
	m_numCtrLocations = ((2 * m_numStepsX) + 1) * ((2 * m_numStepsY) + 1);
	
	/* Free the memory if necessary */
	CleanUpCtrLocations();

	/* Allocate the necessary memory */
	m_ctrLocationsX = new double[m_numCtrLocations];
	m_ctrLocationsY = new double[m_numCtrLocations];

	/* Fill in the grid w/each point advanced by the step size. */
	currLoc = 0;
	for (int xx = -m_numStepsX; xx <= m_numStepsX; xx++) {
		for (int yy = -m_numStepsY; yy <= m_numStepsY; yy++) {
			ETU_ASSERT(currLoc < m_numCtrLocations);
			m_ctrLocationsX[currLoc] = (xx * m_stepSizeDeg) + fCenterXPos;
			m_ctrLocationsY[currLoc] = (yy * m_stepSizeDeg) + fCenterYPos;
			currLoc++;
		}
	}

	ETU_DEBUG_BLOCK {
		FILE* fp = fopen("C:\\temp\\positions.txt", "w");
		CString foo("Locations:\n");
		/* Wrap after each X value is exhaustively printed */
		const int newlineEvery = (2 * m_numStepsY) + 1;
		for (currLoc = 0; currLoc < m_numCtrLocations; currLoc++) {
			foo.AppendFormat("(%6.2f, %6.2f), ", m_ctrLocationsX[currLoc], m_ctrLocationsY[currLoc]);
			/* Add 1 to currLoc so the first case doesn't get a \n */
			if ((currLoc + 1) % newlineEvery == 0) {
				foo.Append("\n");
			}
		}

		if (fp) {
			fprintf(fp, "%s\n", (LPCTSTR)foo);
			fflush(fp);
			fclose(fp);
		}
		else {
			AfxMessageBox((LPCTSTR)foo);
		}
	}
}

/**
 * @brief Get a random location for the center fixation target.
 * @param[out] outX The X location, in degrees.
 * @param[out] outY The Y location, in degrees.
 */
void CScotomaMap::getRandomCtrLocation(double* outX, double* outY)
{
	int randX = etu::ChooseRandom(0, m_numCtrLocations - 1);
	int randY = etu::ChooseRandom(0, m_numCtrLocations - 1);

	*outX = m_ctrLocationsX[randX];
	*outY = m_ctrLocationsY[randY];
}

/**
 * @brief Generates the trials according to dialog settings.
 *
 * Must be called before getting any trials for stimulus display.  Subsequent 
 * calls regenerate the entire sequence.
 */
void CScotomaMap::GenerateTrials() 
{
	long numPoints[NUM_TRIAL_TYPES];
	double redPct = 0.0;
	double bluePct = 0.0;
	double blankPct = 0.0;
	double purplePct = 0.0;
	double gridPct = 0.0;
	double fixatingPct = 0.0;
	double deviatedPct = 0.0;
	double enrichedPct = 0.0;

	/* If user requested alternation of central color, peripheral color, or 
	 * both, we need to segregate trials. May use fewer than all of these, but
	 * tough to code w/o them. 
	 */
	TrialPtrContainer_t trials_redC_redP;
	TrialPtrContainer_t trials_redC_blueP;
	TrialPtrContainer_t trials_redC_blankP;
	TrialPtrContainer_t trials_redC_purpleP;
	TrialPtrContainer_t trials_blueC_redP;
	TrialPtrContainer_t trials_blueC_blueP;
	TrialPtrContainer_t trials_blueC_blankP;
	TrialPtrContainer_t trials_blueC_purpleP;
	TrialPtrContainer_t trials_redC_all;
	TrialPtrContainer_t trials_blueC_all;

	
	CleanUpTrials();

	for (int ix = 0; ix < NUM_TRIAL_TYPES; ix++) {
		numPoints[ix] = 0;
	}
	
	/* Steps are as follows:
	 * 1. First compute percentages
	 * 2. Then compute actual number of trials
	 * 3. Generate lists of actual trial points
	 * 4. Sort lists according to settings 
	 *
	 * Step 1. compute percentages 
	 */
	CalcPercents(redPct, bluePct, blankPct, purplePct, gridPct, fixatingPct, 
		deviatedPct, enrichedPct);

	/* Step 2. Compute actual number of trials
	 * 
	 * Lose the idea of repeats and layers and just make a grid for each 
	 * percentage point, then toss the foveal enrichment trials on top. This 
	 * way there are many more trials before repeating any spatio-temporal 
	 * sequence.  
	 */
	if (gridPct) {
		const long pointsOnGrid = (2 * iNumStepsH + 1) * (2 * iNumStepsV + 1) - 1;
		const long stdGridOnly = 100 * pointsOnGrid;
		const long numGridPoints = (long)(gridPct * stdGridOnly);

		/* We assume a standardized grid-only mode of 100 passes through
		 * the entire grid, and partition those passes among the colors 
		 * according to color percentages. E.g., for 20 percent red, we 
		 * assume 20 peripheral-red grids.  Then we scale by the actual grid 
		 * percent, e.g. 60 percent grid and 40 percent foveal enrichment 
		 * gives us 12 peripheral-red grids instead of 20.
		 *
		 * For the enrichment, we allocate a scaled number of points.  That 
		 * is, 40 percent are enrichment, so 40 percent of 20 red grids is 8 
		 * grids worth of red enrichment points.  If the grid has 8 points, 
		 * that's 64 points of red enrichment.
		 *
		 * The calcs below are simplified.  E.g., numPoints[0] is given by
		 *     numPoints[0] = 100 grids * redPct * gridPct * pointsOnGrid
		 *
		 * For numPoints[1], we want
		 *     numPoints[1] = 100 grids * redPct * deviatedPct * pointsOnGrid
		 *
		 * Thus
		 *     numPoints[0] = 100 grids * redPct * gridPct * pointsOnGrid
		 *     numPoints[0] / gridPct = 100 grids * redPct * pointsOnGrid
		 *
		 *     numPoints[1] = 100 grids * redPct * deviatedPct * pointsOnGrid
		 *                  = deviatedPct * numPoints[0] / gridPct
		 */

		/* Grid */
		numPoints[TT_RED_GRID]    = (long)(numGridPoints * redPct);
		numPoints[TT_BLUE_GRID]   = (long)(numGridPoints * bluePct);
		numPoints[TT_BLANK_GRID]  = (long)(numGridPoints * blankPct);
		numPoints[TT_PURPLE_GRID] = (long)(numGridPoints * purplePct);
		/* Peripheral foveal */
		numPoints[TT_RED_PF]    = (long)((deviatedPct / gridPct) * numPoints[TT_RED_GRID]);
		numPoints[TT_BLUE_PF]   = (long)((deviatedPct / gridPct) * numPoints[TT_BLUE_GRID]);
		numPoints[TT_BLANK_PF]  = (long)((deviatedPct / gridPct) * numPoints[TT_BLANK_GRID]);
		numPoints[TT_PURPLE_PF] = (long)((deviatedPct / gridPct) * numPoints[TT_PURPLE_GRID]);
		/* Central foveal */
		numPoints[TT_RED_CF]    = (long)((fixatingPct / gridPct) * numPoints[TT_RED_GRID]);
		numPoints[TT_BLUE_CF]   = (long)((fixatingPct / gridPct) * numPoints[TT_BLUE_GRID]);
		numPoints[TT_BLANK_CF]  = (long)((fixatingPct / gridPct) * numPoints[TT_BLANK_GRID]);
		numPoints[TT_PURPLE_CF] = (long)((fixatingPct / gridPct) * numPoints[TT_PURPLE_GRID]);
		/* Enriched */
		numPoints[TT_RED_E]    = (long)((enrichedPct / gridPct) * numPoints[TT_RED_GRID]);
		numPoints[TT_BLUE_E]   = (long)((enrichedPct / gridPct) * numPoints[TT_BLUE_GRID]);
		numPoints[TT_BLANK_E]  = (long)((enrichedPct / gridPct) * numPoints[TT_BLANK_GRID]);
		numPoints[TT_PURPLE_E] = (long)((enrichedPct / gridPct) * numPoints[TT_PURPLE_GRID]);
	}
	else {
		/* If there are no grid points and everything is foveal enrichment,
		 * use an arbitrary standard number of samples, pointsOnFovea. 
		 */
		double pointsOnFovea = 2000;

		/* Peripheral foveal */
		numPoints[TT_RED_PF]    = (long)(deviatedPct * redPct    * pointsOnFovea);
		numPoints[TT_BLUE_PF]   = (long)(deviatedPct * bluePct   * pointsOnFovea);
		numPoints[TT_BLANK_PF]  = (long)(deviatedPct * blankPct  * pointsOnFovea);
		numPoints[TT_PURPLE_PF] = (long)(deviatedPct * purplePct * pointsOnFovea);
		/* Central foveal*/
		numPoints[TT_RED_CF]    = (long)(fixatingPct * redPct    * pointsOnFovea);
		numPoints[TT_BLUE_CF]   = (long)(fixatingPct * bluePct   * pointsOnFovea);
		numPoints[TT_BLANK_CF]  = (long)(fixatingPct * blankPct  * pointsOnFovea);
		numPoints[TT_PURPLE_CF] = (long)(fixatingPct * purplePct * pointsOnFovea);
		/* Enriched */
		numPoints[TT_RED_E]     = (long)(enrichedPct * redPct    * pointsOnFovea);
		numPoints[TT_BLUE_E]    = (long)(enrichedPct * bluePct   * pointsOnFovea);
		numPoints[TT_BLANK_E]   = (long)(enrichedPct * blankPct  * pointsOnFovea);
		numPoints[TT_PURPLE_E]  = (long)(enrichedPct * purplePct * pointsOnFovea);
	}

	iNumLocations = 0;
	for (int ix = 0; ix < NUM_TRIAL_TYPES; ix++) {
		iNumLocations += numPoints[ix];
	}

	if ((iCenterColorRadio == CScotomaMap::COLOR_RANDOM) || 
		(iCenterColorRadio == CScotomaMap::COLOR_ALTERNATE)) 
	{
		/* Twice as many points, one set for each color fixation cross */
		iNumLocations *= 2;
	}

	ETU_DEBUG_BLOCK {
		CString msg;
		msg.Format("red %.2f %d\nblue %.2f %d\npurple %.2f\nblank %.2f %d\0", 
			redPct, iNumCatchTrials, bluePct, iNumCatchTrials2, purplePct, blankPct, 
			iNumBlankTrials);
		AfxMessageBox((LPCTSTR)msg);
		msg.Format(   "\tRed\tBlu\tBla\tPrp\n"  \
			       "Gri\t%ld\t%ld\t%ld\t%ld\n"  \
				   "Pfv\t%ld\t%ld\t%ld\t%ld\n"  \
				   "Cfv\t%ld\t%ld\t%ld\t%ld\n"  \
				   "Enr\t%ld\t%ld\t%ld\t%ld\0",
			numPoints[TT_RED_GRID], numPoints[TT_BLUE_GRID], numPoints[TT_BLANK_GRID], numPoints[TT_PURPLE_GRID], 
			numPoints[TT_RED_PF], numPoints[TT_BLUE_PF], numPoints[TT_BLANK_PF], numPoints[TT_PURPLE_PF], 
			numPoints[TT_RED_CF], numPoints[TT_BLUE_CF], numPoints[TT_BLANK_CF], numPoints[TT_BLANK_CF],
			numPoints[TT_RED_E], numPoints[TT_BLUE_E], numPoints[TT_BLANK_E], numPoints[TT_PURPLE_E]);
		AfxMessageBox((LPCTSTR)msg);
	}

	/* Step 3. Generate list of actual trial points */
	bool genRedC = (((iCenterColorRadio == CScotomaMap::COLOR_PRIMARY)) ||
		(iCenterColorRadio == CScotomaMap::COLOR_GLOBAL) ||
		(iCenterColorRadio == CScotomaMap::COLOR_RANDOM) || 
		(iCenterColorRadio == CScotomaMap::COLOR_ALTERNATE));
	bool genBlueC = ((iCenterColorRadio == CScotomaMap::COLOR_SECONDARY) ||
		(iCenterColorRadio == CScotomaMap::COLOR_RANDOM) || 
		(iCenterColorRadio == CScotomaMap::COLOR_ALTERNATE));

	/* Red central target */
	if (genRedC) {
		/* Red peripheral target */
		GenerateLocationsByCode(CScotomaTrial::CODE_C_RED_P_RED, 
			trials_redC_redP, (long)(gridPct * redPct * 100.0), 
			numPoints[TT_RED_PF], numPoints[TT_RED_CF], numPoints[TT_RED_E]);

		/* Blue peripheral target */
		GenerateLocationsByCode(CScotomaTrial::CODE_C_RED_P_BLUE, 
			trials_redC_blueP, (long)(gridPct * bluePct * 100.0), 
			numPoints[TT_BLUE_PF], numPoints[TT_BLUE_CF], numPoints[TT_BLUE_E]);

		/* Blank peripheral target */
		GenerateLocationsByCode(CScotomaTrial::CODE_C_RED_P_BLANK, 
			trials_redC_blankP, (long)(gridPct * blankPct * 100.0), 
			numPoints[TT_BLANK_PF], numPoints[TT_BLANK_CF], numPoints[TT_BLANK_E]);

		/* Purple peripheral target */
		GenerateLocationsByCode(CScotomaTrial::CODE_C_RED_P_PURPLE, 
			trials_redC_purpleP, (long)(gridPct * purplePct * 100.0), 
			numPoints[TT_PURPLE_PF], numPoints[TT_PURPLE_CF], numPoints[TT_PURPLE_E]);
			
	}

	/* Blue central target */
	if (genBlueC) {
		/* Red peripheral target */
		GenerateLocationsByCode(CScotomaTrial::CODE_C_BLUE_P_RED, 
			trials_blueC_redP, (long)(gridPct * redPct * 100.0), 
			numPoints[TT_RED_PF], numPoints[TT_RED_CF], numPoints[TT_RED_E]);
		
		/* Blue peripheral target */
		GenerateLocationsByCode(CScotomaTrial::CODE_C_BLUE_P_BLUE, 
			trials_blueC_blueP, (long)(gridPct * bluePct * 100.0), 
			numPoints[TT_BLUE_PF], numPoints[TT_BLUE_CF], numPoints[TT_BLUE_E]);
	
		/* Blank peripheral target */
		GenerateLocationsByCode(CScotomaTrial::CODE_C_BLUE_P_BLANK, 
			trials_blueC_blankP, (long)(gridPct * blankPct * 100.0), 
			numPoints[TT_BLANK_PF], numPoints[TT_BLANK_CF], numPoints[TT_BLANK_E]);

		/* Purple peripheral target */
		GenerateLocationsByCode(CScotomaTrial::CODE_C_BLUE_P_PURPLE, 
			trials_blueC_purpleP, (long)(gridPct * purplePct * 100.0), 
			numPoints[TT_PURPLE_PF], numPoints[TT_PURPLE_CF], numPoints[TT_PURPLE_E]);
	}

	/* Step 4. Sort list according to settings 
	 *
	 * We now have six containers of trials, with each container randomized.
	 * We now stitch them together by alternating or randomizing.  We use the
	 * two _all containers to aggregate.
	 */
	ETU_DEBUG_BLOCK {
		if (iPeriphColorRadio == CScotomaMap::COLOR_ALTERNATE) {
			/* Peripheral alternation is between primary and secondary only; no 
			 * blank or purple trials to this point.  */
			ETU_ASSERT(trials_redC_blankP.size() == 0);
			ETU_ASSERT(trials_redC_purpleP.size() == 0);
			ETU_ASSERT(trials_blueC_blankP.size() == 0);
			ETU_ASSERT(trials_blueC_purpleP.size() == 0);
		}
	}
	
	/* Prepare red C trials. */
	{
		TrialPtrContainer_t tempRB;
		TrialPtrContainer_t tempPB;
		ZipContainers(iPeriphColorRadio, 1, trials_redC_redP, trials_redC_blueP, tempRB);
		ZipContainers(iPeriphColorRadio, 1, trials_redC_blankP, trials_redC_purpleP, tempPB);
		ZipContainers(iPeriphColorRadio, 1, tempRB, tempPB, trials_redC_all);
	}

	/* Prepare blue C trials */
	{
		TrialPtrContainer_t tempRB;
		TrialPtrContainer_t tempPB;
		ZipContainers(iPeriphColorRadio, 1, trials_blueC_redP, trials_blueC_blueP, tempRB);
		ZipContainers(iPeriphColorRadio, 1, trials_blueC_blankP, trials_blueC_purpleP, tempPB);
		ZipContainers(iPeriphColorRadio, 1, tempRB, tempPB, trials_blueC_all);
	}

	ZipContainers(iCenterColorRadio, iAlternateBlockSize, trials_redC_all, trials_blueC_all, m_trials);

	ETU_DEBUG_BLOCK {
		TrialPtrContainer_t::iterator end = m_trials.end();
		std::ofstream fileout("C:\\temp\\trials.txt");
		for (TrialPtrContainer_t::iterator iter = m_trials.begin(); iter < end;
			 ++iter)
		{
			fileout << (**iter);
		}
		fileout.flush();
		fileout.close();
	}
}

/**
 * @brief Generate the four types of trials.
 * @param stimCode The stimulus code for the center and peripheral colors.
 * @param trials The container in which to store the generated trials.
 * @param numGrids Number of full grids to generate.
 * @param numDevFov Number of trials to generate for deviated foveal enrichment.
 * @param numCentFov Number of trials to generate for central foveal enrichment.
 * @param numEn Number of trials to generate for user-specified foveal
 * enrichment.
 */
void CScotomaMap::GenerateLocationsByCode(CScotomaTrial::STIM_CODE stimCode, 
										  TrialPtrContainer_t& trials, long numGrids,
										  long numDevFov, long numCentFov, long numEn)
{
	TrialPtrContainer_t tempGrid;
	TrialPtrContainer_t tempEnrichment;

	for (int count = 0; count < numGrids; count++) {
		TrialPtrContainer_t newGrid;

		generateByGrid(stimCode, iNumStepsH, iNumStepsV, fStepSize, 
			std::back_inserter(newGrid));

		std::random_shuffle(newGrid.begin(), newGrid.end());
		std::copy(newGrid.begin(), newGrid.end(), std::back_inserter(tempGrid));
	}
	
	generateByClass(stimCode, CScotomaTrial::CLASS_FOVEA_DEVIATED, 
		fPeriphXRadius, fPeriphYRadius, numDevFov, 
		std::back_inserter(tempEnrichment));
	
	generateByClass(stimCode, CScotomaTrial::CLASS_FOVEA_FIXATING, 
		fCenterXRadius, fCenterYRadius, numCentFov, 
		std::back_inserter(tempEnrichment));
	
	generateByClass(stimCode, CScotomaTrial::CLASS_ENRICHED_LOCATION, 
		fEnrichedXRadius, fEnrichedYRadius, numEn, 
		std::back_inserter(tempEnrichment), fEnrichedX, fEnrichedY);

	std::random_shuffle(tempEnrichment.begin(), tempEnrichment.end());
	ZipContainers(COLOR_RANDOM, 1, tempGrid, tempEnrichment, trials);
}

/**
 * @brief Merge two trial containers together in user-specified fashion.
 * 
 * @param method One of the symbolic integers defined as COLOR_ in CScotomaMap.
 *        -- COLOR_ALTERNATE: Function takes blockSize elements from left 
 *               container, then blockSize elements from right container, etc.
 *        -- COLOR_RANDOM: Function copies left and right containers to output,
 *               then shuffles the output container.
 *        -- Any other: Function copies left and right containers to output.
 * @param blockSize Size of blocks to alternate, if method is COLOR_ALTERNATE; 
 * ignored otherwise.
 * @param inLeft Left container.
 * @param inRight Right container.
 * @param outZipped The results.
 */
void CScotomaMap::ZipContainers(int method, int blockSize, TrialPtrContainer_t& inLeft, 
		TrialPtrContainer_t& inRight, TrialPtrContainer_t& outZipped)
{
	if ((method == COLOR_ALTERNATE) || (method == COLOR_RANDOM)) {
		TrialPtrContainer_t::iterator leftIter = inLeft.begin();
		TrialPtrContainer_t::iterator leftEnd = inLeft.end();
		TrialPtrContainer_t::iterator rightIter = inRight.begin();
		TrialPtrContainer_t::iterator rightEnd = inRight.end();
		std::back_insert_iterator<TrialPtrContainer_t> outIter = std::back_inserter(outZipped);

		if (method == COLOR_ALTERNATE) {
			while ((leftIter < leftEnd) || (rightIter < rightEnd)) {
				for (int count = 0; leftIter < leftEnd && count < blockSize; 
					++count, ++leftIter, ++outIter) 
				{
					(*outIter) = (*leftIter);
				}

				for (int count = 0; rightIter < rightEnd && count < blockSize; 
					++count, ++rightIter, ++outIter) 
				{
					(*outIter) = (*rightIter);
				}
			}
		}
		else if (method == COLOR_RANDOM) {
			double prob = ((double)inLeft.size())/(inLeft.size() + inRight.size());

			while ((leftIter < leftEnd) && (rightIter < rightEnd)) {
				if (etu::RandomChoice(prob)) {
					(*outIter) = (*leftIter);
					++leftIter;
				}
				else {
					(*outIter) = (*rightIter);
					++rightIter;
				}
				++outIter;
			}

			if (leftIter < leftEnd) {
				std::copy(leftIter, leftEnd, outIter);
			}
			else if (rightIter < rightEnd) {
				std::copy(rightIter, rightEnd, outIter);
			}
		}
	}
	else {
		std::copy(inLeft.begin(), inLeft.end(), std::back_inserter(outZipped));
		std::copy(inRight.begin(), inRight.end(), std::back_inserter(outZipped));
	}
}

/**
 * @brief Free memory from all trials in trials container, then empties it.
 */
void CScotomaMap::CleanUpTrials()
{
	TrialPtrContainer_t::iterator iter = m_trials.begin();
	TrialPtrContainer_t::iterator endIter = m_trials.end();
	for ( ; iter != endIter; ++iter) {
		delete (*iter);
	}
	m_trials.clear();
}

/**
 * @brief Reclaim memory from center locations arrays.
 */
void CScotomaMap::CleanUpCtrLocations()
{
	if (m_ctrLocationsX) {
		delete[] m_ctrLocationsX;
	}

	if (m_ctrLocationsY) {
		delete[] m_ctrLocationsY;
	}

	m_ctrLocationsX = NULL;
	m_ctrLocationsY = NULL;
}

const CScotomaMap::TrialPtrContainer_t& CScotomaMap::getTrials() 
{ return m_trials; }

int CScotomaMap::getPrimaryRed() {return iPrimaryRed;}
void CScotomaMap::setPrimaryRed(int i) {iPrimaryRed=i;}

int CScotomaMap::getPrimaryGreen() {return iPrimaryGreen;}
void CScotomaMap::setPrimaryGreen(int i) { iPrimaryGreen=i;}

int CScotomaMap::getPrimaryBlue() {return iPrimaryBlue;}
void CScotomaMap::setPrimaryBlue(int i) {iPrimaryBlue=i;}

int CScotomaMap::getSecondaryRed() {return iSecondaryRed;}
void CScotomaMap::setSecondaryRed(int i) {iSecondaryRed=i;}

int CScotomaMap::getSecondaryGreen() {return iSecondaryGreen;}
void CScotomaMap::setSecondaryGreen(int i) {iSecondaryGreen=i;}

int CScotomaMap::getSecondaryBlue() {return iSecondaryBlue;}
void CScotomaMap::setSecondaryBlue(int i) {iSecondaryBlue=i;}

int CScotomaMap::getTertiaryRed() { return iTertiaryRed; }
void CScotomaMap::setTertiaryRed(int i) { iTertiaryRed= i; }

int CScotomaMap::getTertiaryGreen() { return iTertiaryGreen; }
void CScotomaMap::setTertiaryGreen(int i) { iTertiaryGreen= i; }

int CScotomaMap::getTertiaryBlue() { return iTertiaryBlue; }
void CScotomaMap::setTertiaryBlue(int i) { iTertiaryBlue= i; }

int CScotomaMap::getNoiseRed() { return iNoiseRed; }
void CScotomaMap::setNoiseRed(int i) { iNoiseRed= i; }

int CScotomaMap::getNoiseGreen() { return iNoiseGreen; }
void CScotomaMap::setNoiseGreen(int i) { iNoiseGreen= i; }

int CScotomaMap::getNoiseBlue() { return iNoiseBlue; }
void CScotomaMap::setNoiseBlue(int i) { iNoiseBlue= i; }

int CScotomaMap::getResponseDelay() { return iResponseDelay; }
void CScotomaMap::setResponseDelay(int i) { iResponseDelay= i; }

int CScotomaMap::getWrongDelay() { return iWrongDelay; }
void CScotomaMap::setWrongDelay(int i) { iWrongDelay= i; }

int CScotomaMap::getMisbehaviorDelay() { return iMisbehaviorDelay; }
void CScotomaMap::setMisbehaviorDelay(int i) { iMisbehaviorDelay= i; }

int CScotomaMap::getNumRandomRepeats() { return iNumRandomRepeats; }
void CScotomaMap::setNumRandomRepeats(int i) { iNumRandomRepeats= i; }

int CScotomaMap::getNoisePct() { return iNoisePct; }
void CScotomaMap::setNoisePct(int i) { iNoisePct = i; }

int CScotomaMap::getNoiseSize() { return iNoiseSize; }
void CScotomaMap::setNoiseSize(int i) { iNoiseSize= i; }

int CScotomaMap::getRampDuration() { return iRampDuration; }
void CScotomaMap::setRampDuration(int i) { iRampDuration= i; }

int CScotomaMap::getDisplayTime() { return iDisplayTime; }
void CScotomaMap::setDisplayTime(int i) { iDisplayTime= i; }

int CScotomaMap::getDisplayRandMax() { return iDisplayRandMax; }
void CScotomaMap::setDisplayRandMax(int i) { iDisplayRandMax= i; }

int CScotomaMap::getDisplayRandMin() { return iDisplayRandMin; }
void CScotomaMap::setDisplayRandMin(int i) { iDisplayRandMin= i; }

int CScotomaMap::getFlashInterval() { return iFlashInterval; }
void CScotomaMap::setFlashInterval(int i) { iFlashInterval= i; }

int CScotomaMap::getFlashIntervalRandMax() { return iFlashIntervalRandMax; }
void CScotomaMap::setFlashIntervalRandMax(int i) { iFlashIntervalRandMax= i; }

int CScotomaMap::getFlashIntervalRandMin() { return iFlashIntervalRandMin; }
void CScotomaMap::setFlashIntervalRandMin(int i) { iFlashIntervalRandMin= i; }

int CScotomaMap::getCrossPix() { return iCrossPix; }
void CScotomaMap::setCrossPix(int i) { iCrossPix= i; }

int CScotomaMap::getCenterColorRadio() {return iCenterColorRadio;}
bool CScotomaMap::setCenterColorRadio(int i) {bool changed = iCenterColorRadio!=i; iCenterColorRadio=i; return changed;}

int CScotomaMap::getAlternateBlockSize() {return iAlternateBlockSize;}
bool CScotomaMap::setAlternateBlockSize(int i) {bool changed = iAlternateBlockSize!=i; iAlternateBlockSize=i; return changed;}

int CScotomaMap::getPeriphColorRadio() {return iPeriphColorRadio;}
bool CScotomaMap::setPeriphColorRadio(int i) {bool changed = iPeriphColorRadio!=i; iPeriphColorRadio=i; return changed;}

int CScotomaMap::getNumCatchTrials() { return iNumCatchTrials; }
bool CScotomaMap::setNumCatchTrials(int i) {bool changed = iNumCatchTrials != i; iNumCatchTrials = i; return changed;}
	
int CScotomaMap::getNumCatchTrials2() { return iNumCatchTrials2; }
bool CScotomaMap::setNumCatchTrials2(int i) {bool changed = iNumCatchTrials2 != i; iNumCatchTrials2 = i; return changed;}

int CScotomaMap::getNumBlankTrials() { return iNumBlankTrials; }
bool CScotomaMap::setNumBlankTrials(int i) {bool changed = iNumBlankTrials != i; iNumBlankTrials = i; return changed;}

int CScotomaMap::getNumStepsH() {return iNumStepsH;}
bool CScotomaMap::setNumStepsH(int i) {bool changed = iNumStepsH!=i; iNumStepsH=i; return changed;}

int CScotomaMap::getNumStepsV() {return iNumStepsV;}
bool CScotomaMap::setNumStepsV(int i) {bool changed = iNumStepsV!=i; iNumStepsV=i; return changed;}

int CScotomaMap::getPctTimeAtCenter() { return iPctTimeAtCenter; }
bool CScotomaMap::setPctTimeAtCenter(int i) {bool changed = iPctTimeAtCenter!=i; iPctTimeAtCenter=i; return changed;}

int CScotomaMap::getPctTimeAtPeriph() { return iPctTimeAtPeriph; }
bool CScotomaMap::setPctTimeAtPeriph(int i) {bool changed = iPctTimeAtPeriph!=i; iPctTimeAtPeriph=i; return changed;}

bool CScotomaMap::getUseNoise() { return bUseNoise; }
void CScotomaMap::setUseNoise(bool b) { bUseNoise= b; }

bool CScotomaMap::getInterleaveNoise() { return bInterleaveNoise; }
void CScotomaMap::setInterleaveNoise(bool b) { bInterleaveNoise= b; }

bool CScotomaMap::getRampUp() { return bRampUp; }
void CScotomaMap::setRampUp(bool b) { bRampUp= b; }

bool CScotomaMap::getHideCenter() { return bHideCenter; }
void CScotomaMap::setHideCenter(bool b) { bHideCenter= b; }

bool CScotomaMap::getRandomizeFlashInterval() {return bRandomizeFlashInterval;}
void CScotomaMap::setRandomizeFlashInterval(bool b) {bRandomizeFlashInterval=b;}

bool CScotomaMap::getRandomizeDisplayTime() {return bRandomizeDisplayTime;}
void CScotomaMap::setRandomizeDisplayTime(bool b) {bRandomizeDisplayTime=b;}

bool CScotomaMap::getAvoidZero() {return bAvoidZero;}
void CScotomaMap::setAvoidZero(bool b) {bAvoidZero=b;}

bool CScotomaMap::getCenterPoints() { return bCenterPoints; }
void CScotomaMap::setCenterPoints(bool b) { bCenterPoints= b; }

bool CScotomaMap::getOffAxis() { return bOffAxis; }
void CScotomaMap::setOffAxis(bool b) { bOffAxis= b; }

float CScotomaMap::getCenterXPos() {return fCenterXPos;}
void CScotomaMap::setCenterXPos(float f) {fCenterXPos = f;}

float CScotomaMap::getCenterYPos() {return fCenterYPos;}
void CScotomaMap::setCenterYPos(float f) { fCenterYPos = f; }

float CScotomaMap::getCenterDotSize() {return fCenterDotSize;}
void CScotomaMap::setCenterDotSize(float f) { fCenterDotSize =f; }

float CScotomaMap::getPeriphDotSize() {return fPeriphDotSize;}
void CScotomaMap::setPeriphDotSize(float f) { fPeriphDotSize =f; }

float CScotomaMap::getStepSize() {return fStepSize;}
bool CScotomaMap::setStepSize(float f) { bool changed = fStepSize!=f; fStepSize =f; return changed;}

float CScotomaMap::getCenterXRadius() { return fCenterXRadius; }
bool CScotomaMap::setCenterXRadius(float f) { bool changed = fCenterXRadius!=f; fCenterXRadius=f; return changed;}

float CScotomaMap::getCenterYRadius() { return fCenterYRadius; }
bool CScotomaMap::setCenterYRadius(float f) { bool changed = fCenterYRadius!=f; fCenterYRadius=f; return changed;}

float CScotomaMap::getPeriphXRadius() { return fPeriphXRadius; }
bool CScotomaMap::setPeriphXRadius(float f) { bool changed = fPeriphXRadius!=f; fPeriphXRadius=f; return changed;}

float CScotomaMap::getPeriphYRadius() { return fPeriphYRadius; }
bool CScotomaMap::setPeriphYRadius(float f) { bool changed = fPeriphYRadius!=f; fPeriphYRadius=f; return changed;}

int CScotomaMap::getRewardScheme() { return iRewardScheme; }
void CScotomaMap::setRewardScheme(int i) { iRewardScheme = i; }

int CScotomaMap::getTertiaryRewardPct() { return iTertiaryRewardPct; }
void CScotomaMap::setTertiaryRewardPct(int i) { iTertiaryRewardPct = i; }

int CScotomaMap::getRewardDelay() { return iRewardDelay; }
void CScotomaMap::setRewardDelay(int i) { iRewardDelay= i; }

bool CScotomaMap::getDisplayDuringChoice() { return bDisplayDuringChoice; }
void CScotomaMap::setDisplayDuringChoice(bool b) { bDisplayDuringChoice= b; }

bool CScotomaMap::getUseShapes() { return bUseShapes; }
void CScotomaMap::setUseShapes(bool b) { bUseShapes = b; }

bool CScotomaMap::getSameBG() { return bSameBG; }
void CScotomaMap::setSameBG(bool b) { bSameBG = b; }

bool CScotomaMap::getOccludeBG() { return bOccludeBG; }
void CScotomaMap::setOccludeBG(bool b) { bOccludeBG = b; }

int CScotomaMap::getPctTimeAtEnriched() { return iPctTimeAtEnriched; }
bool CScotomaMap::setPctTimeAtEnriched(int i) { bool changed = iPctTimeAtEnriched!=i; iPctTimeAtEnriched= i; return changed;}

float CScotomaMap::getEnrichedX() { return fEnrichedX; }
bool CScotomaMap::setEnrichedX(float f) { bool changed = fEnrichedX!=f; fEnrichedX=f; return changed;}

float CScotomaMap::getEnrichedY() { return fEnrichedY; }
bool CScotomaMap::setEnrichedY(float f) { bool changed = fEnrichedY!=f; fEnrichedY=f; return changed;}

float CScotomaMap::getEnrichedXRadius() { return fEnrichedXRadius; }
bool CScotomaMap::setEnrichedXRadius(float f) { bool changed = fEnrichedXRadius!=f; fEnrichedXRadius=f; return changed;}

float CScotomaMap::getEnrichedYRadius() { return fEnrichedYRadius; }
bool CScotomaMap::setEnrichedYRadius(float f) { bool changed = fEnrichedYRadius!=f; fEnrichedYRadius=f; return changed;}

bool CScotomaMap::getDotGratings() { return bDotGratings; }
void CScotomaMap::setDotGratings(bool b) { bDotGratings= b; }

int CScotomaMap::getDotGratingContrast() { return iDotGratingContrast; }
void CScotomaMap::setDotGratingContrast(int i) { iDotGratingContrast= i; }

float CScotomaMap::getDotGratingCycles() { return fDotGratingCycles; }
void CScotomaMap::setDotGratingCycles(float f) { fDotGratingCycles= f; }

float CScotomaMap::getSeparationV() { return fSeparationV; }
void CScotomaMap::setSeparationV(float f) { fSeparationV= f; }

bool CScotomaMap::getUseKeyboard() { return bUseKeyboard; }
void CScotomaMap::setUseKeyboard(bool b) { bUseKeyboard= b; }

int CScotomaMap::getNumCtrLocations() { return m_numCtrLocations; }

void CScotomaMap::setStepSizeDeg(float stepSizeDeg) { m_stepSizeDeg = stepSizeDeg; }
float CScotomaMap::getStepSizeDeg() { return m_stepSizeDeg; }

void CScotomaMap::setNumStepsX(int numStepsX) { m_numStepsX = numStepsX; }
int CScotomaMap::getNumStepsX() { return m_numStepsX; }

void CScotomaMap::setNumStepsY(int numStepsY) { m_numStepsY = numStepsY; }
int CScotomaMap::getNumStepsY() { return m_numStepsY; }

long CScotomaMap::getNumLocations() { return iNumLocations; }

/**
 * @brief Calculate percentages of trials and store in output variables.
 * @param[out] redPct Percent of grid trials that are red.
 * @param[out] bluePct Percent of grid  trials that are blue.
 * @param[out] blankPct Percent of grid  trials that are blank.
 * @param[out] purplePct Percent of grid trials that are purple.
 * @param[out] gridPct Percent of trials that comprise grid points.
 * @param[out] fixatingPct Percent of trials from enrichment of fixating eye.
 * @param[out] deviatedPct Percent of trials from enrichment of deviated eye.
 * @param[out] enrichedPct Percent of trials from enrichment of elected point.
 */
void CScotomaMap::CalcPercents(double& redPct, double& bluePct, 
							   double& blankPct, double& purplePct, 
							   double& gridPct, double& fixatingPct, 
							   double& deviatedPct, double& enrichedPct)
{
	if (iPeriphColorRadio == COLOR_PRIMARY) {
		redPct = 1.0;
	} 
	else if (iPeriphColorRadio == COLOR_SECONDARY) {
		bluePct = 1.0;
	} 
	else if (iPeriphColorRadio == COLOR_TERTIARY) {
		purplePct = 1.0;
	} 
	else if (iPeriphColorRadio == COLOR_ALTERNATE) {
		/* Alternate primary and secondary only. */
		redPct = 0.5;
		bluePct = 0.5;
	}
	else { /* Random or Alternate */
		redPct = ((double)iNumCatchTrials) / 100.0;
		bluePct = ((double)iNumCatchTrials2) / 100.0;
		blankPct = ((double)iNumBlankTrials) / 100.0;
		purplePct = ((double)(100 - iNumCatchTrials - iNumCatchTrials2 - iNumBlankTrials)) / 100.0;
	}

	fixatingPct = ((double)iPctTimeAtCenter)/100.0;
	deviatedPct = ((double)iPctTimeAtPeriph)/100.0;
	enrichedPct = ((double)iPctTimeAtEnriched)/100.0;
	gridPct = ((double)(100 - iPctTimeAtCenter - iPctTimeAtPeriph - iPctTimeAtEnriched)) / 100.0;

	/* Scale all to [0.0, 1.0] */
	gridPct     = NORMALIZE_PERCENT(gridPct);
	fixatingPct = NORMALIZE_PERCENT(fixatingPct);
	deviatedPct = NORMALIZE_PERCENT(deviatedPct);
	enrichedPct = NORMALIZE_PERCENT(enrichedPct);
	purplePct   = NORMALIZE_PERCENT(purplePct);
	redPct      = NORMALIZE_PERCENT(redPct);
	bluePct     = NORMALIZE_PERCENT(bluePct);
	blankPct    = NORMALIZE_PERCENT(blankPct);
}

#endif // APP_EYE_TRACKING
