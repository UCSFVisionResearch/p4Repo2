/**
 * @file TrialSaccades.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <deque>

#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

#include "comm/SerialComm.h"
#include "utils/Utils.h"
#include "utils/Asserts.h"
#include "utils/Trace.h"
#include "TrialSaccades.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/* text label coding:
 * code	center	periph
 * 0x0		pri		pri
 * 0x1		pri		sec
 * 0x2		sec		pri
 * 0x3		sec		sec
 */

CTrialSaccades::CTrialSaccades()
: defaultFile("C:\\Stimulus Settings\\TrialSaccades\\default.txt"), 
  iPrimaryRed(160), iPrimaryGreen(0),   iPrimaryBlue(0),
  iSecondaryRed(0), iSecondaryGreen(0), iSecondaryBlue(255),
  m_stepSizeCtrDeg(15), m_numCtrStepsX(1), m_numCtrStepsY(0), 
  m_numCtrLocations(0), m_eyeCoverage(COVERAGE_SPATIOTOPIC), 
  iNoiseRed(32), iNoiseGreen(0), iNoiseBlue(50), iNoisePct(50), iNoiseSize(2),
  bUseNoise(true), bInterleaveNoise(false), bSameBG(true), bOccludeBG(false),
  iMisbehaviorDelay(1000),
  iWrongDelay(1500), iRewardDelay(750), m_fixationEye(FIXATION_RANDOM),
  iCrossPix(2), m_centerColorRadio(COLOR_RANDOM), iAlternateBlockSize(10),
  m_periphColorRadio(COLOR_RANDOM), m_pctRedGrids(50), m_pctBlueGrids(50),
  m_pctRedCtr(50), m_pctBlueCtr(50), m_pctPurpleCtr(0), m_pctPurpleGrids(0),
  m_blankTimeOutScreen(true), m_alternateWithPurple(true),
  iNumStepsH(4), iNumStepsV(3),
  bAvoidZero(true), bCenterPoints(false),
  bOffAxis(false), fCenterXPos(0.0), fCenterYPos(0.0), fCenterDotSize(1.0),
  fPeriphDotSize(1.0), fStepSize(5.0), m_useApprox(false),
  bUseShapes(false), bDotGratings(false), 
  iDotGratingContrast(90), fDotGratingCycles(3.0), fSeparationV(0.0), 
  m_gridType(GRID_CARTESIAN), m_angleOffset(0), 
  m_angle0(false), m_angle45(false), m_angle90(false), m_angle135(false), 
  m_angle180(false), m_angle225(false), m_angle270(false), m_angle315(false), 
  m_radiusOffset(0), m_radius0(false), m_radius5(false), m_radius10(false), 
  m_radius15(false), m_radius20(false), m_radius25(false), m_radius30(false), 
  m_radiusOrigin(false), m_trialGenerator(NULL), m_deviationAngle(0.0),
  m_advanceEitherEye(false), m_initTimeMin(200), m_initTimeMax(500),
  m_delayTimeMin(0), m_delayTimeMax(0), m_flashTimeMin(200), m_flashTimeMax(200),
  m_baitedTime(500), m_holdTime(100)
{
	m_currentTrial = m_trials.begin();
	LoadFile(true);
}

/**
 * @brief Clean up trials and center locations.
 */
CTrialSaccades::~CTrialSaccades() 
{
	CleanUpTrials();
	delete m_trialGenerator;
	m_trialGenerator = NULL;
}

/**
 * @brief Load settings from file.
 * @param bDefault True to use default file, false to prompt user for a file.
 */
void CTrialSaccades::LoadFile(bool bDefault) 
{
	int discardInt;
	double discardDouble;

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
	
	fscanf(fp, "%d\n", &iNoiseRed);
	fscanf(fp, "%d\n", &iNoiseGreen);
	fscanf(fp, "%d\n", &iNoiseBlue);
	
	fscanf(fp, "%d\n", &discardInt); // Removed iTargetActivePeriod);
	fscanf(fp, "%d\n", &iWrongDelay);
	
	fscanf(fp, "%d\n", &discardInt);
	
	fscanf(fp, "%d\n", &iNoisePct);
	fscanf(fp, "%d\n", &iNoiseSize);
	fscanf(fp, "%d\n", &discardInt); // Removed iRampDuration);
	
	fscanf(fp, "%d\n", &discardInt); // Removed iDisplayTime
	fscanf(fp, "%d\n", &discardInt); // Removed iDisplayRandMax);
	fscanf(fp, "%d\n", &discardInt); // Removed iDisplayRandMin);
	fscanf(fp, "%d\n", &discardInt); // Removed iFlashInterval);
	fscanf(fp, "%d\n", &discardInt); // Removed iFlashIntervalRandMax);
	fscanf(fp, "%d\n", &discardInt); // Removed iFlashIntervalRandMin);
	fscanf(fp, "%d\n", &iCrossPix);
	
	fscanf(fp, "%d\n", &m_centerColorRadio);
	fscanf(fp, "%d\n", &iAlternateBlockSize);
	
	fscanf(fp, "%d\n", &m_periphColorRadio);
	fscanf(fp, "%d\n", &m_pctRedGrids);
	fscanf(fp, "%d\n", &m_pctBlueGrids);
	
	fscanf(fp, "%d\n", &iNumStepsH);
	fscanf(fp, "%d\n", &iNumStepsV);
	
	fscanf(fp, "%d\n", &bUseNoise);
	fscanf(fp, "%d\n", &bInterleaveNoise);
	
	fscanf(fp, "%d\n", &discardInt); // Removed bRampUp);
	fscanf(fp, "%d\n", &discardInt); // Remove m_showCenterFlash
	fscanf(fp, "%d\n", &discardInt); // Remove m_showCenterHold
	fscanf(fp, "%d\n", &discardInt); // Removed bRandomizeFlashInterval);
	fscanf(fp, "%d\n", &discardInt); // Remove bRandomizeDisplayTime
	fscanf(fp, "%d\n", &bAvoidZero);
	fscanf(fp, "%d\n", &bCenterPoints);
	fscanf(fp, "%d\n", &bOffAxis);

	fscanf(fp, "%f\n", &fCenterXPos);
	fscanf(fp, "%f\n", &fCenterYPos);
	fscanf(fp, "%f\n", &fCenterDotSize);
	fscanf(fp, "%f\n", &fPeriphDotSize);
	fscanf(fp, "%f\n", &fStepSize);

	fscanf(fp, "%f\n", &m_stepSizeCtrDeg);
	fscanf(fp, "%d\n", &m_numCtrStepsX);
	fscanf(fp, "%d\n", &m_numCtrStepsY);

	fscanf(fp, "%d\n", &iMisbehaviorDelay);
	fscanf(fp, "%d\n", &discardInt); // Removed &m_rewardScheme
	fscanf(fp, "%d\n", &iRewardDelay);
	fscanf(fp, "%d\n", &discardInt); // Removed &bDisplayDuringHoldPeriod);
	fscanf(fp, "%d\n", &bUseShapes);
	fscanf(fp, "%d\n", &bSameBG);
	fscanf(fp, "%d\n", &bOccludeBG);
	fscanf(fp, "%d\n", &bDotGratings);
	fscanf(fp, "%d\n", &iDotGratingContrast);
	fscanf(fp, "%f\n", &fDotGratingCycles);
	fscanf(fp, "%f\n", &fSeparationV);

	fscanf(fp, "%d\n", &discardInt); // Removed m_fixationFlashDelay);
	fscanf(fp, "%d\n", &discardInt); // Removed m_saccadeHoldPeriod);

	fscanf(fp, "%d\n", (int*)&m_gridType);
	fscanf(fp, "%d\n", &m_angleOffset);
	fscanf(fp, "%d\n", &m_angle0);
	fscanf(fp, "%d\n", &m_angle45);
	fscanf(fp, "%d\n", &m_angle90);
	fscanf(fp, "%d\n", &m_angle135);
	fscanf(fp, "%d\n", &m_angle180);
	fscanf(fp, "%d\n", &m_angle225);
	fscanf(fp, "%d\n", &m_angle270);
	fscanf(fp, "%d\n", &m_angle315);
	fscanf(fp, "%d\n", &m_radiusOffset);
	fscanf(fp, "%d\n", &m_radiusOrigin);
	fscanf(fp, "%d\n", &m_radius0);
	fscanf(fp, "%d\n", &m_radius5);
	fscanf(fp, "%d\n", &m_radius10);
	fscanf(fp, "%d\n", &m_radius15);
	fscanf(fp, "%d\n", &m_radius20);
	fscanf(fp, "%d\n", &m_radius25);
	fscanf(fp, "%d\n", &m_radius30);

	fscanf(fp, "%d\n", &m_eyeCoverage);
	fscanf(fp, "%f\n", &discardDouble); // Old deviation angle
	fscanf(fp, "%d\n", &m_fixationEye);

	fscanf(fp, "%d\n", &m_useApprox);

	fscanf(fp, "%d\n", &m_advanceEitherEye);
	fscanf(fp, "%d\n", &iTertiaryRed);
	fscanf(fp, "%d\n", &iTertiaryGreen);
	fscanf(fp, "%d\n", &iTertiaryBlue);
	fscanf(fp, "%d\n", &m_pctPurpleGrids);
	fscanf(fp, "%d\n", &m_pctPurpleCtr);
	fscanf(fp, "%d\n", &m_alternateWithPurple);

	fscanf(fp, "%d\n", &discardInt); // Removed m_saccadeTrigger);
	fscanf(fp, "%d\n", &discardInt); // Removed m_cueTimeMin);
	fscanf(fp, "%d\n", &discardInt); // Removed m_cueTimeMax);

	fscanf(fp, "%f\n", &m_deviationAngle);

	fscanf(fp, "%d\n", &discardInt); // Removed old m_delayTimeMin);
	fscanf(fp, "%d\n", &discardInt); // Removed old m_delayTimeMax);
	fscanf(fp, "%d\n", &discardInt); // Removed m_lingerTimeMin);
	fscanf(fp, "%d\n", &discardInt); // Removed m_lingerTimeMax);

	fscanf(fp, "%d\n", &m_pctRedCtr);
	fscanf(fp, "%d\n", &m_pctBlueCtr);

	fscanf(fp, "%d\n", &m_initTimeMin);
	fscanf(fp, "%d\n", &m_initTimeMax);
	fscanf(fp, "%d\n", &m_flashTimeMin);
	fscanf(fp, "%d\n", &m_flashTimeMax);
	fscanf(fp, "%d\n", &m_delayTimeMin);
	fscanf(fp, "%d\n", &m_delayTimeMax);
	fscanf(fp, "%d\n", &m_holdTime);
	fscanf(fp, "%d\n", &m_baitedTime);

	fclose(fp);
}

/**
 * @brief Save settings to file.
 * @param bDefault True to use default file, false to prompt user for a file.
 */
void CTrialSaccades::SaveFile(bool bDefault) 
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
	
	fprintf(fp, "%d\n", iNoiseRed);
	fprintf(fp, "%d\n", iNoiseGreen);
	fprintf(fp, "%d\n", iNoiseBlue);
	
	fprintf(fp, "9999\n"); // Removed iTargetActivePeriod
	fprintf(fp, "%d\n", iWrongDelay);

	fprintf(fp, "9999\n"); // Place-holder
	
	fprintf(fp, "%d\n", iNoisePct);
	fprintf(fp, "%d\n", iNoiseSize);
	fprintf(fp, "9999\n"); // Removed iRampDuration
	
	fprintf(fp, "9999\n"); // Removed iDisplayTime
	fprintf(fp, "9999\n"); // Removed iDisplayRandMax
	fprintf(fp, "9999\n"); // Removed iDisplayRandMin
	fprintf(fp, "9999\n"); // Removed iFlashInterval
	fprintf(fp, "9999\n"); // Removed iFlashIntervalRandMax
	fprintf(fp, "9999\n"); // Removed iFlashIntervalRandMin
	fprintf(fp, "%d\n", iCrossPix);
	
	fprintf(fp, "%d\n", m_centerColorRadio);
	fprintf(fp, "%d\n", iAlternateBlockSize);
	
	fprintf(fp, "%d\n", m_periphColorRadio);
	fprintf(fp, "%d\n", m_pctRedGrids);
	fprintf(fp, "%d\n", m_pctBlueGrids);
	
	fprintf(fp, "%d\n", iNumStepsH);
	fprintf(fp, "%d\n", iNumStepsV);
	
	fprintf(fp, "%d\n", bUseNoise);
	fprintf(fp, "%d\n", bInterleaveNoise);
	
	fprintf(fp, "9999\n"); // Removed bRampUp
	fprintf(fp, "9999\n"); // Removed m_showCenterFlash
	fprintf(fp, "9999\n"); // Removed m_showCenterHold
	fprintf(fp, "9999\n"); // Removed bRandomizeFlashInterval
	fprintf(fp, "9999\n"); // Removed bRandomizeDisplayTime
	fprintf(fp, "%d\n", bAvoidZero);
	fprintf(fp, "%d\n", bCenterPoints);
	fprintf(fp, "%d\n", bOffAxis);

	fprintf(fp, "%f\n", fCenterXPos);
	fprintf(fp, "%f\n", fCenterYPos);
	fprintf(fp, "%f\n", fCenterDotSize);
	fprintf(fp, "%f\n", fPeriphDotSize);
	fprintf(fp, "%f\n", fStepSize);

	fprintf(fp, "%f\n", m_stepSizeCtrDeg);
	fprintf(fp, "%d\n", m_numCtrStepsX);
	fprintf(fp, "%d\n", m_numCtrStepsY);

	fprintf(fp, "%d\n", iMisbehaviorDelay);
	fprintf(fp, "9999\n"); // Removed (int)m_rewardScheme
	fprintf(fp, "%d\n", iRewardDelay);
	fprintf(fp, "9999\n"); // Removed bDisplayDuringHoldPeriod);
	fprintf(fp, "%d\n", bUseShapes);
	fprintf(fp, "%d\n", bSameBG);
	fprintf(fp, "%d\n", bOccludeBG);

	fprintf(fp, "%d\n", bDotGratings);
	fprintf(fp, "%d\n", iDotGratingContrast);
	fprintf(fp, "%f\n", fDotGratingCycles);
	fprintf(fp, "%f\n", fSeparationV);

	fprintf(fp, "9999\n"); // Removed m_fixationFlashDelay
	fprintf(fp, "9999\n"); // Removed m_saccadeHoldPeriod

	fprintf(fp, "%d\n", (int)m_gridType);
	fprintf(fp, "%d\n", m_angleOffset);
	fprintf(fp, "%d\n", m_angle0);
	fprintf(fp, "%d\n", m_angle45);
	fprintf(fp, "%d\n", m_angle90);
	fprintf(fp, "%d\n", m_angle135);
	fprintf(fp, "%d\n", m_angle180);
	fprintf(fp, "%d\n", m_angle225);
	fprintf(fp, "%d\n", m_angle270);
	fprintf(fp, "%d\n", m_angle315);
	fprintf(fp, "%d\n", m_radiusOffset);
	fprintf(fp, "%d\n", m_radiusOrigin);
	fprintf(fp, "%d\n", m_radius0);
	fprintf(fp, "%d\n", m_radius5);
	fprintf(fp, "%d\n", m_radius10);
	fprintf(fp, "%d\n", m_radius15);
	fprintf(fp, "%d\n", m_radius20);
	fprintf(fp, "%d\n", m_radius25);
	fprintf(fp, "%d\n", m_radius30);

	fprintf(fp, "%d\n", m_eyeCoverage);
	fprintf(fp, "%f\n", m_deviationAngle);
	fprintf(fp, "%d\n", m_fixationEye);

	fprintf(fp, "%d\n", m_useApprox);

	fprintf(fp, "%d\n", m_advanceEitherEye);
	fprintf(fp, "%d\n", iTertiaryRed);
	fprintf(fp, "%d\n", iTertiaryGreen);
	fprintf(fp, "%d\n", iTertiaryBlue);
	fprintf(fp, "%d\n", m_pctPurpleGrids);
	fprintf(fp, "%d\n", m_pctPurpleCtr);
	fprintf(fp, "%d\n", m_alternateWithPurple);

	fprintf(fp, "9999\n"); // Removed m_saccadeTrigger
	fprintf(fp, "9999\n"); // Removed m_cueTimeMin
	fprintf(fp, "9999\n"); // Removed m_cueTimeMax

	fprintf(fp, "%.4f\n", m_deviationAngle);

	fprintf(fp, "9999\n"); // Removed m_delayTimeMin
	fprintf(fp, "9999\n"); // Removed m_delayTimeMax
	fprintf(fp, "9999\n"); // Removed m_lingerTimeMin
	fprintf(fp, "9999\n"); // Removed m_lingerTimeMax

	fprintf(fp, "%d\n", m_pctRedCtr);
	fprintf(fp, "%d\n", m_pctBlueCtr);

	fprintf(fp, "%d\n", m_initTimeMin);
	fprintf(fp, "%d\n", m_initTimeMax);
	fprintf(fp, "%d\n", m_flashTimeMin);
	fprintf(fp, "%d\n", m_flashTimeMax);
	fprintf(fp, "%d\n", m_delayTimeMin);
	fprintf(fp, "%d\n", m_delayTimeMax);
	fprintf(fp, "%d\n", m_holdTime);
	fprintf(fp, "%d\n", m_baitedTime);

	fclose(fp);
}

/**
 * @brief Sends settings over serial port.
 * @param theComm Communications object to use for settings.
 */
void CTrialSaccades::sendSettings(CSerialComm& theComm) 
{
	char buffer[256];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "Trial Saccades Monkey;PrimaryRed;%d;%ld|0x%x/\0", iPrimaryRed, tm, code);		                   theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;PrimaryGreen;%d;%ld|0x%x/\0", iPrimaryGreen, tm, code);		               theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;PrimaryBlue;%d;%ld|0x%x/\0", iPrimaryBlue, tm, code);		               theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;SecondaryRed;%d;%ld|0x%x/\0", iSecondaryRed, tm, code);		               theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;SecondaryGreen;%d;%ld|0x%x/\0", iSecondaryGreen, tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;SecondaryBlue;%d;%ld|0x%x/\0", iSecondaryBlue, tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;NoiseRed;%d;%ld|0x%x/\0", iNoiseRed, tm, code);		                       theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;NoiseGreen;%d;%ld|0x%x/\0", iNoiseGreen, tm, code);		                   theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;NoiseBlue;%d;%ld|0x%x/\0", iNoiseBlue, tm, code);		                   theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;WrongDelay;%d;%ld|0x%x/\0", iWrongDelay, tm, code);		                   theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;NoisePct;%d;%ld|0x%x/\0", iNoisePct, tm, code);		                       theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;NoiseSize;%d;%ld|0x%x/\0", iNoiseSize, tm, code);		                   theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;CrossPix;%d;%ld|0x%x/\0", iCrossPix, tm, code);		                       theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;CenterColorRadio;%d;%ld|0x%x/\0", m_centerColorRadio, tm, code);		       theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;AlternateBlockSize;%d;%ld|0x%x/\0", iAlternateBlockSize, tm, code);		   theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;AlternateWithPurple;%d;%ld|0x%x/\0", m_alternateWithPurple, tm, code);	   theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;PctPurpleCtr;%d;%ld|0x%x/\0", m_pctPurpleCtr, tm, code);					   theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;PeriphColorRadio;%d;%ld|0x%x/\0", m_periphColorRadio, tm, code);		       theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;PctRedTrials;%d;%ld|0x%x/\0", m_pctRedGrids, tm, code);		               theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;PctBlueTrials;%d;%ld|0x%x/\0", m_pctBlueGrids, tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;PctPurpleTrials;%d;%ld|0x%x/\0", m_pctPurpleGrids, tm, code);		       theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;UseCartGrid;%d;%ld|0x%x/\0", (m_gridType == GRID_CARTESIAN), tm, code);     theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;NumStepsH;%d;%ld|0x%x/\0", iNumStepsH, tm, code);		                   theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;NumStepsV;%d;%ld|0x%x/\0", iNumStepsV, tm, code);		                   theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;StepSize;%f;%ld|0x%x/\0", fStepSize, tm, code);		                       theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;UseRadialGrid;%d;%ld|0x%x/\0", (m_gridType == GRID_RADIAL), tm, code);      theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;AngleOffset;%d;%ld|0x%x/\0", m_angleOffset, tm, code);                      theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Angle0;%d;%ld|0x%x/\0", m_angle0, tm, code);                                theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Angle45;%d;%ld|0x%x/\0", m_angle45, tm, code);                              theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Angle90;%d;%ld|0x%x/\0", m_angle90, tm, code);                              theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Angle135;%d;%ld|0x%x/\0", m_angle135, tm, code);                            theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Angle180;%d;%ld|0x%x/\0", m_angle180, tm, code);                            theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Angle215;%d;%ld|0x%x/\0", m_angle225, tm, code);                            theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Angle270;%d;%ld|0x%x/\0", m_angle270, tm, code);                            theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Angle315;%d;%ld|0x%x/\0", m_angle315, tm, code);                            theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;RadiusOffset;%d;%ld|0x%x/\0", m_radiusOffset, tm, code);                    theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Radius0rigin;%d;%ld|0x%x/\0", m_radiusOrigin, tm, code);                    theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Radius0;%d;%ld|0x%x/\0", m_radius0, tm, code);                              theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Radius5;%d;%ld|0x%x/\0", m_radius5, tm, code);                              theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Radius10;%d;%ld|0x%x/\0", m_radius10, tm, code);                            theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Radius15;%d;%ld|0x%x/\0", m_radius15, tm, code);                            theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Radius20;%d;%ld|0x%x/\0", m_radius20, tm, code);                            theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Radius25;%d;%ld|0x%x/\0", m_radius25, tm, code);                            theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;Radius30;%d;%ld|0x%x/\0", m_radius30, tm, code);                            theComm.transmit(buffer, (int)strlen(buffer));

	sprintf(buffer, "Trial Saccades Monkey;NumLocations;%ld;%ld|0x%x/\0", m_trials.size(), tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;UseNoise;%d;%ld|0x%x/\0", bUseNoise, tm, code);		                       theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;InterleaveNoise;%d;%ld|0x%x/\0", bInterleaveNoise, tm, code);		       theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;AvoidZero;%d;%ld|0x%x/\0", bAvoidZero, tm, code);		                   theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;CenterPoints;%d;%ld|0x%x/\0", bCenterPoints, tm, code);		               theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;OffAxis;%d;%ld|0x%x/\0", bOffAxis, tm, code);		                       theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;CenterXPos;%.4f;%ld|0x%x/\0", fCenterXPos, tm, code);		               theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;CenterYPos;%.4f;%ld|0x%x/\0", fCenterYPos, tm, code);		               theComm.transmit(buffer, (int)strlen(buffer));

	sprintf(buffer, "Trial Saccades Monkey;CtrStepSizeDeg;%.4f;%ld|0x%x/\0", m_stepSizeCtrDeg, tm, code);		       theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;CtrNumStepsX;%d;%ld|0x%x/\0", m_numCtrStepsX, tm, code);		               theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;CtrNumStepsY;%d;%ld|0x%x/\0", m_numCtrStepsY, tm, code);		               theComm.transmit(buffer, (int)strlen(buffer));
	
	sprintf(buffer, "Trial Saccades Monkey;CenterDotSize;%.4f;%ld|0x%x/\0", fCenterDotSize, tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;PeriphDotSize;%.4f;%ld|0x%x/\0", fPeriphDotSize, tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;StepSize;%.4f;%ld|0x%x/\0", fStepSize, tm, code);                           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;MisbehaviorDelay;%d;%ld|0x%x/\0", iMisbehaviorDelay, tm, code);		       theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;RewardDelay;%d;%ld|0x%x/\0", iRewardDelay, tm, code);		               theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;UseShapes;%d;%ld|0x%x/\0", bUseShapes, tm, code);                           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;SameBG;%d;%ld|0x%x/\0", bSameBG, tm, code);                                 theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;OccludeBG;%d;%ld|0x%x/\0", bOccludeBG, tm, code);                           theComm.transmit(buffer, (int)strlen(buffer));

	sprintf(buffer, "Trial Saccades Monkey;DotGratings;%d;%ld|0x%x/\0", bDotGratings, tm, code);                       theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;DotGratingContrast;%d;%ld|0x%x/\0", iDotGratingContrast, tm, code);         theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;DotGratingCycles;%.4f;%ld|0x%x/\0", fDotGratingCycles, tm, code);           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;SeparationV;%.4f;%ld|0x%x/\0", fSeparationV, tm, code);                     theComm.transmit(buffer, (int)strlen(buffer));

	sprintf(buffer, "Trial Saccades Monkey;AdvanceEitherEye;%d;%ld|0x%x/\0", m_advanceEitherEye, tm, code);                       theComm.transmit(buffer, (int)strlen(buffer));

	sprintf(buffer, "Trial Saccades Monkey;InitTimeMin;%d;%ld|0x%x/\0", m_initTimeMin, tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;InitTimeMax;%d;%ld|0x%x/\0", m_initTimeMax, tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;FlashTimeMin;%d;%ld|0x%x/\0", m_flashTimeMin, tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;FlashTimeMax;%d;%ld|0x%x/\0", m_flashTimeMax, tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;DelayTimeMin;%d;%ld|0x%x/\0", m_delayTimeMin, tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;DelayTimeMax;%d;%ld|0x%x/\0", m_delayTimeMax, tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;BaitedTime;%d;%ld|0x%x/\0", m_baitedTime, tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Trial Saccades Monkey;HoldTime;%d;%ld|0x%x/\0", m_holdTime, tm, code);		           theComm.transmit(buffer, (int)strlen(buffer));

}
/**
 * @brief Allocate and return a Grid_t for peripheral targets.
 */
etu::pGrid_t CTrialSaccades::MakePeripheralGrid()
{
	switch (m_gridType) {
		case GRID_RADIAL: {
			etu::int_deque_t angles;
			etu::int_deque_t radii;

			if (m_angle0) angles.push_back(0);
			if (m_angle45) angles.push_back(45);
			if (m_angle90) angles.push_back(90);
			if (m_angle135) angles.push_back(135);
			if (m_angle180) angles.push_back(180);
			if (m_angle225) angles.push_back(225);
			if (m_angle270) angles.push_back(270);
			if (m_angle315) angles.push_back(315);

			if (m_radius0) radii.push_back(0);
			if (m_radius5) radii.push_back(5);
			if (m_radius10) radii.push_back(10);
			if (m_radius15) radii.push_back(15);
			if (m_radius20) radii.push_back(20);
			if (m_radius25) radii.push_back(25);
			if (m_radius30) radii.push_back(30);

			return etu::createRadialGrid(m_angleOffset, angles, 
				m_radiusOffset, radii, m_radiusOrigin);
	    } 
		/* return instead of break */
		
		case GRID_CARTESIAN:
		default:
			return etu::createCartesianGrid(iNumStepsH, iNumStepsV, 
				fStepSize, !bAvoidZero);
	}
}

/**
 * @brief Generates the trials according to dialog settings.
 *
 * Must be called before getting any trials for stimulus display.  Subsequent 
 * calls regenerate the entire sequence.
 */
void CTrialSaccades::GenerateTrials()
{
	std::auto_ptr<CSaccadeTrialGenerator> generator;
	etu::pGrid_t fixationGrid(etu::createCartesianGrid(m_numCtrStepsX, 
		m_numCtrStepsY, m_stepSizeCtrDeg, true, fCenterXPos, fCenterYPos));
	etu::pGrid_t peripheralGrid(MakePeripheralGrid());

	CleanUpTrials();

	generator.reset(CSaccadeTrialGenerator::GetGenerator(*this, fixationGrid, 
		peripheralGrid));

	generator->GenerateTrials(m_trials);
	m_currentTrial = m_trials.begin();

	{
		std::ofstream debugOut("C:\\temp\\trials.txt");
		//if (debugOut) 
		{
			TrialPtrContainer_t::const_iterator iter = m_trials.begin();
			TrialPtrContainer_t::const_iterator end = m_trials.end();
			for ( ; iter < end ; ++iter) {
				debugOut << (**iter);
			}
			debugOut.flush();
			debugOut.close();
		}
	}
}

/**
 * @brief Get next trial in previously-generated list.
 *
 * Iterates through all trials cyclicly.  Must call GenerateTrials first.
 * Each call moves the iteration, so the next call will recall the next trial.
 * Caller owns the memory and must delete it.
 *
 * @return Pointer to next trial, or NULL if no trials generated.
 */
CSaccadeTrial* CTrialSaccades::GetNextTrial()
{
	CSaccadeTrial* retval = NULL;

	if (m_trials.size()) {
		if (m_currentTrial >= m_trials.end()) {
			m_currentTrial = m_trials.begin();
		}
		
		retval = (*m_currentTrial);
		++m_currentTrial;
	} 
	
	return retval;
}

/**
 * @brief Free memory from all trials in trials container, then empties it.
 */
void CTrialSaccades::CleanUpTrials()
{
	TrialPtrContainer_t::iterator iter = m_trials.begin();
	TrialPtrContainer_t::iterator endIter = m_trials.end();
	for ( ; iter != endIter; ++iter) {
		delete (*iter);
	}
	m_trials.clear();
}

const TrialPtrContainer_t& CTrialSaccades::getTrials() const
{ return m_trials; }

CTrialSaccades::EYE_COVERAGE CTrialSaccades::getEyeCoverage() const 
{ return m_eyeCoverage; }
void CTrialSaccades::setEyeCoverage(CTrialSaccades::EYE_COVERAGE eyeCoverage) 
{ m_eyeCoverage = eyeCoverage; }

bool CTrialSaccades::getUseApprox() const { return m_useApprox; }
void CTrialSaccades::setUseApprox(bool useApprox) { m_useApprox = useApprox; }

double CTrialSaccades::getDeviationAngle() const { return m_deviationAngle; }
void CTrialSaccades::setDeviationAngle(double deviationAngle) 
{ m_deviationAngle = deviationAngle; }

CTrialSaccades::FIXATION_EYE CTrialSaccades::getFixationEye() const 
{ return m_fixationEye; }
void CTrialSaccades::setFixationEye(CTrialSaccades::FIXATION_EYE fixationEye) 
{ m_fixationEye = fixationEye; }

int CTrialSaccades::getPrimaryRed() const {return iPrimaryRed;}
void CTrialSaccades::setPrimaryRed(int i) {iPrimaryRed=i;}

int CTrialSaccades::getPrimaryGreen() const {return iPrimaryGreen;}
void CTrialSaccades::setPrimaryGreen(int i) { iPrimaryGreen=i;}

int CTrialSaccades::getPrimaryBlue() const {return iPrimaryBlue;}
void CTrialSaccades::setPrimaryBlue(int i) {iPrimaryBlue=i;}

int CTrialSaccades::getSecondaryRed() const {return iSecondaryRed;}
void CTrialSaccades::setSecondaryRed(int i) {iSecondaryRed=i;}

int CTrialSaccades::getSecondaryGreen() const {return iSecondaryGreen;}
void CTrialSaccades::setSecondaryGreen(int i) {iSecondaryGreen=i;}

int CTrialSaccades::getSecondaryBlue() const {return iSecondaryBlue;}
void CTrialSaccades::setSecondaryBlue(int i) {iSecondaryBlue=i;}

int CTrialSaccades::getTertiaryRed() const {return iTertiaryRed;}
void CTrialSaccades::setTertiaryRed(int i) {iTertiaryRed=i;}

int CTrialSaccades::getTertiaryGreen() const {return iTertiaryGreen;}
void CTrialSaccades::setTertiaryGreen(int i) {iTertiaryGreen=i;}

int CTrialSaccades::getTertiaryBlue() const {return iTertiaryBlue;}
void CTrialSaccades::setTertiaryBlue(int i) {iTertiaryBlue=i;}

int CTrialSaccades::getNoiseRed() const { return iNoiseRed; }
void CTrialSaccades::setNoiseRed(int i) { iNoiseRed= i; }

int CTrialSaccades::getNoiseGreen() const { return iNoiseGreen; }
void CTrialSaccades::setNoiseGreen(int i) { iNoiseGreen= i; }

int CTrialSaccades::getNoiseBlue() const { return iNoiseBlue; }
void CTrialSaccades::setNoiseBlue(int i) { iNoiseBlue= i; }

bool CTrialSaccades::getBlankTimeOutScreen() const { return m_blankTimeOutScreen; }
void CTrialSaccades::setBlankTimeOutScreen(bool bb) { m_blankTimeOutScreen = bb; }

int CTrialSaccades::getWrongDelay() const { return iWrongDelay; }
void CTrialSaccades::setWrongDelay(int i) { iWrongDelay= i; }

int CTrialSaccades::getMisbehaviorDelay() const { return iMisbehaviorDelay; }
void CTrialSaccades::setMisbehaviorDelay(int i) { iMisbehaviorDelay= i; }

int CTrialSaccades::getNoisePct() const { return iNoisePct; }
void CTrialSaccades::setNoisePct(int i) { iNoisePct = i; }

int CTrialSaccades::getNoiseSize() const { return iNoiseSize; }
void CTrialSaccades::setNoiseSize(int i) { iNoiseSize= i; }

int CTrialSaccades::getInitTimeMin() const { return m_initTimeMin; }
void CTrialSaccades::setInitTimeMin(int init) { m_initTimeMin = init; }

int CTrialSaccades::getInitTimeMax() const { return m_initTimeMax; }
void CTrialSaccades::setInitTimeMax(int init) { m_initTimeMax = init; }

int CTrialSaccades::getFlashTimeMin() const { return m_flashTimeMin; }
void CTrialSaccades::setFlashTimeMin(int flash) { m_flashTimeMin = flash; }

int CTrialSaccades::getFlashTimeMax() const { return m_flashTimeMax; }
void CTrialSaccades::setFlashTimeMax(int flash) { m_flashTimeMax = flash; }

int CTrialSaccades::getDelayTimeMin() const { return m_delayTimeMin; }
void CTrialSaccades::setDelayTimeMin(int delay) { m_delayTimeMin = delay; }

int CTrialSaccades::getDelayTimeMax() const { return m_delayTimeMax; }
void CTrialSaccades::setDelayTimeMax(int delay) { m_delayTimeMax = delay; }

int CTrialSaccades::getHoldTime() const { return m_holdTime;}
void CTrialSaccades::setHoldTime(int i) { m_holdTime = i; }

int CTrialSaccades::getBaitedTime() const { return m_baitedTime; }
void CTrialSaccades::setBaitedTime(int i) { m_baitedTime = i; }

int CTrialSaccades::getCrossPix() const { return iCrossPix; }
void CTrialSaccades::setCrossPix(int i) { iCrossPix= i; }

CTrialSaccades::COLOR_RADIO_VALUE CTrialSaccades::getCenterColorRadio() const
{ return m_centerColorRadio; }
void CTrialSaccades::setCenterColorRadio(CTrialSaccades::COLOR_RADIO_VALUE crv) 
{ m_centerColorRadio = crv; }

int CTrialSaccades::getAlternateBlockSize() const {return iAlternateBlockSize;}
bool CTrialSaccades::setAlternateBlockSize(int i) {bool changed = iAlternateBlockSize!=i; iAlternateBlockSize=i; return changed;}

int CTrialSaccades::getAlternateWithPurple() const { return m_alternateWithPurple; }
void CTrialSaccades::setAlternateWithPurple(bool withP) { m_alternateWithPurple = withP; }

int CTrialSaccades::getPctRedCtr() const { return m_pctRedCtr; }
int CTrialSaccades::getPctBlueCtr() const { return m_pctBlueCtr; }
int CTrialSaccades::getPctPurpleCtr() const { return m_pctPurpleCtr; }
bool CTrialSaccades::setCenterPercents(int pctRed, int pctBlue) 
{
	setPercents(pctRed, pctBlue, m_pctRedCtr, m_pctBlueCtr, m_pctPurpleCtr);
	return true;
}

CTrialSaccades::COLOR_RADIO_VALUE CTrialSaccades::getPeriphColorRadio() 
const { return m_periphColorRadio; }
void CTrialSaccades::setPeriphColorRadio(CTrialSaccades::COLOR_RADIO_VALUE crv) 
{ m_periphColorRadio = crv; }

int CTrialSaccades::getPctRedGrids() const { return m_pctRedGrids; }
int CTrialSaccades::getPctBlueGrids() const { return m_pctBlueGrids; }
int CTrialSaccades::getPctPurpleGrids() const { return m_pctPurpleGrids; }
bool CTrialSaccades::setGridPercents(int pctRed, int pctBlue) 
{
	setPercents(pctRed, pctBlue, m_pctRedGrids, m_pctBlueGrids, 
		m_pctPurpleGrids);
	return true;
}

CTrialSaccades::GRID_TYPE CTrialSaccades::getGridType() const { return m_gridType; }
void CTrialSaccades::setGridType(CTrialSaccades::GRID_TYPE gridType) {m_gridType = gridType; }

int CTrialSaccades::getNumStepsH() const {return iNumStepsH;}
bool CTrialSaccades::setNumStepsH(int i) {bool changed = iNumStepsH!=i; iNumStepsH=i; return changed;}

int CTrialSaccades::getNumStepsV() const {return iNumStepsV;}
bool CTrialSaccades::setNumStepsV(int i) {bool changed = iNumStepsV!=i; iNumStepsV=i; return changed;}

int CTrialSaccades::getAngleOffset() const { return m_angleOffset; }
void CTrialSaccades::setAngleOffset(int angleOffset) { m_angleOffset = angleOffset; }

bool CTrialSaccades::getAngle0() const { return m_angle0; }
bool CTrialSaccades::getAngle45() const { return m_angle45; }
bool CTrialSaccades::getAngle90() const { return m_angle90; }
bool CTrialSaccades::getAngle135() const { return m_angle135; }
bool CTrialSaccades::getAngle180() const { return m_angle180; }
bool CTrialSaccades::getAngle225() const { return m_angle225; }
bool CTrialSaccades::getAngle270() const { return m_angle270; }
bool CTrialSaccades::getAngle315() const { return m_angle315; }

void CTrialSaccades::setAngle0(bool checked) { m_angle0 = checked; }
void CTrialSaccades::setAngle45(bool checked) { m_angle45 = checked; }
void CTrialSaccades::setAngle90(bool checked) { m_angle90 = checked; }
void CTrialSaccades::setAngle135(bool checked) { m_angle135 = checked; }
void CTrialSaccades::setAngle180(bool checked) { m_angle180 = checked; }
void CTrialSaccades::setAngle225(bool checked) { m_angle225 = checked; }
void CTrialSaccades::setAngle270(bool checked) { m_angle270 = checked; }
void CTrialSaccades::setAngle315(bool checked) { m_angle315 = checked; }

int CTrialSaccades::getRadiusOffset() const { return m_radiusOffset; }
void CTrialSaccades::setRadiusOffset(int radiusOffset) { m_radiusOffset = radiusOffset; }

bool CTrialSaccades::getRadiusOrigin() const { return m_radiusOrigin; }
bool CTrialSaccades::getRadius0() const { return m_radius0; }
bool CTrialSaccades::getRadius5() const { return m_radius5; }
bool CTrialSaccades::getRadius10() const { return m_radius10; }
bool CTrialSaccades::getRadius15() const { return m_radius15; }
bool CTrialSaccades::getRadius20() const { return m_radius20; }
bool CTrialSaccades::getRadius25() const { return m_radius25; }
bool CTrialSaccades::getRadius30() const { return m_radius30; }

void CTrialSaccades::setRadiusOrigin(bool checked) { m_radiusOrigin = checked; }
void CTrialSaccades::setRadius0(bool checked) { m_radius0 = checked; }
void CTrialSaccades::setRadius5(bool checked) { m_radius5 = checked; }
void CTrialSaccades::setRadius10(bool checked) { m_radius10 = checked; }
void CTrialSaccades::setRadius15(bool checked) { m_radius15 = checked; }
void CTrialSaccades::setRadius20(bool checked) { m_radius20 = checked; }
void CTrialSaccades::setRadius25(bool checked) { m_radius25 = checked; }
void CTrialSaccades::setRadius30(bool checked) { m_radius30 = checked; }

bool CTrialSaccades::getUseNoise() const { return bUseNoise; }
void CTrialSaccades::setUseNoise(bool b) { bUseNoise= b; }

bool CTrialSaccades::getInterleaveNoise() const { return bInterleaveNoise; }
void CTrialSaccades::setInterleaveNoise(bool b) { bInterleaveNoise= b; }

bool CTrialSaccades::getAdvanceEitherEye() const { return m_advanceEitherEye; }
void CTrialSaccades::setAdvanceEitherEye(bool b) { m_advanceEitherEye = b; }

bool CTrialSaccades::getAvoidZero() const {return bAvoidZero;}
void CTrialSaccades::setAvoidZero(bool b) {bAvoidZero=b;}

bool CTrialSaccades::getCenterPoints() const { return bCenterPoints; }
void CTrialSaccades::setCenterPoints(bool b) { bCenterPoints= b; }

bool CTrialSaccades::getOffAxis() const { return bOffAxis; }
void CTrialSaccades::setOffAxis(bool b) { bOffAxis= b; }

float CTrialSaccades::getCenterXPos() const {return fCenterXPos;}
void CTrialSaccades::setCenterXPos(float f) {fCenterXPos = f;}

float CTrialSaccades::getCenterYPos() const {return fCenterYPos;}
void CTrialSaccades::setCenterYPos(float f) { fCenterYPos = f; }

float CTrialSaccades::getCenterDotSize() const {return fCenterDotSize;}
void CTrialSaccades::setCenterDotSize(float f) { fCenterDotSize =f; }

float CTrialSaccades::getPeriphDotSize() const {return fPeriphDotSize;}
void CTrialSaccades::setPeriphDotSize(float f) { fPeriphDotSize =f; }

float CTrialSaccades::getStepSize() const {return fStepSize;}
bool CTrialSaccades::setStepSize(float f) { bool changed = fStepSize!=f; fStepSize =f; return changed;}

int CTrialSaccades::getRewardDelay() const { return iRewardDelay; }
void CTrialSaccades::setRewardDelay(int i) { iRewardDelay= i; }

bool CTrialSaccades::getUseShapes() const { return bUseShapes; }
void CTrialSaccades::setUseShapes(bool b) { bUseShapes = b; }

bool CTrialSaccades::getSameBG() const { return bSameBG; }
void CTrialSaccades::setSameBG(bool b) { bSameBG = b; }

bool CTrialSaccades::getOccludeBG() const { return bOccludeBG; }
void CTrialSaccades::setOccludeBG(bool b) { bOccludeBG = b; }

bool CTrialSaccades::getDotGratings() const { return bDotGratings; }
void CTrialSaccades::setDotGratings(bool b) { bDotGratings= b; }

int CTrialSaccades::getDotGratingContrast() const { return iDotGratingContrast; }
void CTrialSaccades::setDotGratingContrast(int i) { iDotGratingContrast= i; }

float CTrialSaccades::getDotGratingCycles() const { return fDotGratingCycles; }
void CTrialSaccades::setDotGratingCycles(float f) { fDotGratingCycles= f; }

int CTrialSaccades::getNumCtrLocations() const { return m_numCtrLocations; }

void CTrialSaccades::setCtrStepSize(float stepSizeDeg) { m_stepSizeCtrDeg = stepSizeDeg; }
float CTrialSaccades::getCtrStepSize() const { return m_stepSizeCtrDeg; }

void CTrialSaccades::setNumStepsX(int numStepsX) { m_numCtrStepsX = numStepsX; }
int CTrialSaccades::getNumStepsX() const { return m_numCtrStepsX; }

void CTrialSaccades::setNumStepsY(int numStepsY) { m_numCtrStepsY = numStepsY; }
int CTrialSaccades::getNumStepsY() const { return m_numCtrStepsY; }

long CTrialSaccades::getNumLocations() const { return m_trials.size(); }

bool CTrialSaccades::getDebugResponderOn() const { return m_debugResponderOn; }
void CTrialSaccades::setDebugResponderOn(bool onOff) { m_debugResponderOn = onOff; }

bool CTrialSaccades::getDebugLabelsOn() const { return m_debugLabelsOn; }
void CTrialSaccades::setDebugLabelsOn(bool onOff) { m_debugLabelsOn = onOff; }

/**
 * @brief Set three percents that must sum to 100 percent.
 *
 * Set red, blue, and purple percentages guaranteed to sum to 100 percent even
 * in case of invalid input from the user.  When normalizing inputs, red is
 * taken first, then blue, then purple.  Thus "40, 70, 0" normalizes to
 * "40, 60, 0" and not "30, 70, 0".
 *
 * There is one exception to the promised sum of 100 percent: the function
 * preserves the 1/3 - 1/3 - 1/3, setting "33, 33, 33".
 *
 * Accept references so either of two sets of object variables can be set.
 *
 * @param pctRed The user-entered red percent, possibly out of range.
 * @param pctBlue The user-entered blue percent, possibly out of range.
 * @param[out] whichRed Reference to the integer which receives the validated
 *             percent of red.
 * @param[out] whichBlue Reference to the integer which receives the validated
 *             percent of blue.
 * @param[out] whichPurple Reference to the integer which receives the 
 *             validated percent of purple.
 */
void CTrialSaccades::setPercents(int pctRed, int pctBlue, int& whichRed, 
								 int& whichBlue, int& whichPurple)
{
	/* Maintain invariant that sum of percents = 100 %.  Give priority to the
	 * red parameter, then the blue param.  Purple is always the slave.
	 * However, round for 1/3 - 1/3 - 1/3, so the gcf calculation works. */
	if ((pctRed == 33) && (pctBlue == 33)) {
		whichRed = 33;
		whichBlue = 33;
		whichPurple = 33;
	}
	else {
		int leftover;
		whichRed = ((pctRed > 100) ? 100 : ((pctRed < 0) ? 0 : pctRed));
		leftover = 100 - whichRed;
		whichBlue = (pctBlue >= 0) ? ETU_MIN(pctBlue, leftover) : 0; 
		leftover -= whichBlue;
		whichPurple = leftover;
	}
}

#endif // APP_EYE_TRACKING
