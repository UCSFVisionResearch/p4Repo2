/**
 * @file AppCore.cpp
 * @brief Declarations for AppCore.h
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "AppCore.h"
#include "EyeTrackingDlg.h"
#include "comm/SMISerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Statics and Globals
 *****************************************************************************/
HANDLE g_mutex;

/* dialog declarations */
CEyeTrackingDlg* g_pEyeTrackingDlg;
CCalibrationDlg* g_pCalibrationDlg;
CSettingsDlg* g_pSettingsDlg;
CPursuitDlg* g_pPursuitDlg;

#ifdef APP_EYE_TRACKING
CSaccadeDlg* g_pSaccadeDlg;
CFullfieldDlg* g_pFullfieldDlg;
CStepRampDlg* g_pStepRampDlg;
CSceneCalibrationDlg* g_pSceneCalibrationDlg;
COcularFollowingDlg* g_pOcularFollowingDlg;
CIsoluminantPatchDlg* g_pIsoluminantPatchDlg;
COrientationTuningDlg* g_pOrientationTuningDlg;
CRFMappingDlg* g_pRFMappingDlg;
CCircleAnnulusDlg* g_pCircleAnnulusDlg;
CAPDDlg* g_pAPDDlg;
CRememberDlg* g_pRememberDlg;
CSuppressionDlg* g_pSuppressionDlg;
CAlternationDlg* g_pAlternationDlg;
CMSequenceDlg* g_pMSequenceDlg;
CRulerDlg* g_pRulerDlg;
CAmblyoscopeDlg* g_pAmblyoscopeDlg;
CScotomaMapDlg* g_pScotomaMapDlg;
CTrialSaccadesDlg* g_pTrialSaccadesDlg;
CRandomDotAnaglyphDlg* g_pRandomDotAnaglyphDlg;
CGratingsDlg* g_pGratingsDlg;
CFixationStabilityDlg* g_pFixationStabilityDlg;
CReverseFixationDlg* g_pReverseFixationDlg;
CSparseNoiseDlg* g_pSparseNoiseDlg;
#endif

#ifdef APP_ACUITY
CPrefSelectionDlg* g_pPrefSelectionDlg;
CNoiseGratingsDlg* g_pNoiseGratingsDlg;
#endif

CDialog* pDialog = NULL;

/* container declarations */
CCalibration* g_pCalibration = NULL;
CSettings* g_pSettings = NULL;
CPursuit* g_pPursuit = NULL;

#ifdef APP_EYE_TRACKING
CSaccade* g_pSaccade = NULL;
CFullfield* g_pFullfield = NULL;
CStepRamp* g_pStepRamp = NULL;
CSceneCalibration* g_pSceneCalibration = NULL;
COcularFollowing* g_pOcularFollowing = NULL;
CIsoluminantPatch* g_pIsoluminantPatch = NULL;
COrientationTuning* g_pOrientationTuning = NULL;
CRFMapping* g_pRFMapping = NULL;
CCircleAnnulus* g_pCircleAnnulus = NULL;
CAPD* g_pAPD = NULL;
CRemember* g_pRemember = NULL;
CSuppression* g_pSuppression = NULL;
CAlternation* g_pAlternation = NULL;
CMSequence* g_pMSequence = NULL;
CRuler* g_pRuler = NULL;
CAmblyoscope* g_pAmblyoscope = NULL;
CScotomaMap* g_pScotomaMap = NULL;
CTrialSaccades* g_pTrialSaccades = NULL;
CRandomDotAnaglyph* g_pRandomDotAnaglyph = NULL;
CGratings* g_pGratings = NULL;
CFixationStabilityModel* g_pFixationStabilityModel = NULL;
CReverseFixation* g_pReverseFixation = NULL;
CSparseNoise* g_pSparseNoise = NULL;
#endif

#ifdef APP_ACUITY
CPrefSelection* g_pPrefSelection = NULL;
CNoiseGratings* g_pNoiseGratings = NULL;
#endif

/* global vars */
CGenericStimulus* g_pActiveStimulus = NULL;
DWORD g_dHalfFrameInterval = 0;
double g_dSleepThreshold = 0;
bool g_bIsInitialized = false;
VSGOBJHANDLE g_stim;
VSGOBJHANDLE g_fixation;
VSGOBJHANDLE g_grating;
VSGOBJHANDLE g_stim2;
double g_dFramerate = 0;
CSMISerialComm g_serialCommLeft;	
CSMISerialComm g_serialCommRight;	
CReceptiveField g_receptiveFieldRight;
CReceptiveField g_receptiveFieldLeft;

/*****************************************************************************
 * Defines
 *****************************************************************************/

/** Allocate dialog and call its Create member function */
#define ET_CREATE(cls, obj, dlg) \
	obj = new cls(); \
	obj->Create(dlg, NULL);
	
/*****************************************************************************
 * Functions
 *****************************************************************************/

/** Initialize the application */
bool InitApp() {
	srand((unsigned)time(NULL));

	/* create all container objects */
	g_pCalibration = new CCalibration();
	g_pSettings = new CSettings();// contains buffer overrun!!
	g_pPursuit = new CPursuit();

    #ifdef APP_EYE_TRACKING
		/* 2/27/2008 rearranged these constructors to solve memory problems? */
		g_pScotomaMap = new CScotomaMap();
		g_pSaccade = new CSaccade();
		g_pFullfield = new CFullfield();
		g_pStepRamp = new CStepRamp();
		g_pSceneCalibration = new CSceneCalibration();
		g_pOcularFollowing = new COcularFollowing();
		g_pIsoluminantPatch = new CIsoluminantPatch();
		g_pAPD = new CAPD();
		g_pRemember = new CRemember();
		g_pAmblyoscope = new CAmblyoscope();
		g_pGratings = new CGratings();//5/19/08 rearrange again? CString access violation for default settings button
		g_pRandomDotAnaglyph = new CRandomDotAnaglyph();

		g_pSuppression = new CSuppression();
		g_pAlternation = new CAlternation();
		g_pMSequence = new CMSequence();
		g_pRuler = new CRuler();

		g_pOrientationTuning = new COrientationTuning();
		g_pRFMapping = new CRFMapping();	
		g_pCircleAnnulus = new CCircleAnnulus();
		g_pTrialSaccades = new CTrialSaccades();
		g_pFixationStabilityModel = new CFixationStabilityModel();
		g_pReverseFixation = new CReverseFixation();
		g_pSparseNoise = new CSparseNoise();
    #endif

    #ifdef APP_ACUITY
	    g_pPrefSelection = new CPrefSelection();
    	g_pNoiseGratings = new CNoiseGratings();
    #endif

	int CheckCard;

	if (!g_bIsInitialized) {
		CheckCard = vsgInit("");
		if (CheckCard < 0) return false;

		/* added 10/23/2007 MKF
		 * before doing anything else, prevent accidental auto-reward */
		vsgIOWriteDigitalOut(0x0000, vsgDIG0+vsgDIG1); // no trial, no reset pulse
		vsgIOWriteDigitalOut(0x0000, vsgDIG2); // enable normal mode during scotoma mapping
		vsgIOWriteDigitalOut(0x0000, vsgDIG7); // prevent remote reward

		g_bIsInitialized = true;
		CheckCard = vsgSetVideoMode(vsgPANSCROLLMODE);
	}

	vsgIOReadADC(8);

	/* create the main eyeTracking Dlg */
	ET_CREATE(CEyeTrackingDlg, g_pEyeTrackingDlg, IDD_EYETRACKING_DIALOG);
	
	#ifndef _NO_ANALOG_IO
		/* configure the io board to run +- 10 volts */
		AO_Configure(1,0,0,0,10.0,1);
		AO_Configure(1,1,0,0,10.0,1);
		AO_Configure(1,2,0,0,10.0,1);
		AO_Configure(1,3,0,0,10.0,1);
		AO_Configure(1,4,0,0,10.0,1);
		AO_Configure(1,5,0,0,10.0,1);
		AO_Configure(1,6,0,0,10.0,1);
		AO_Configure(1,7,0,0,10.0,1);
	#endif

	return true;
}	

/** 
 * @brief Diplay given dialog after hiding current dialog, if any. 
 * @param pNewDlg May be NULL.
 */
bool SetActiveDialog(CDialog* pNewDlg) {
	/* Hide current, if any */
	if (pDialog) pDialog->ShowWindow(SW_HIDE); 

	/* Show new dialog, if any */
	if (!pNewDlg) {
		pDialog = NULL;
	}
	else {
		pDialog = pNewDlg;
		pDialog->ShowWindow(SW_SHOW);
	}

	return true;
}

/** Display the main dialog to run application. */
bool RunApp() {
	g_mutex = CreateMutex(NULL, false, NULL);
	SetActiveDialog(g_pEyeTrackingDlg);
	return true;
}

/** Clean up before exit. */
bool ExitApp() {
	CloseHandle(g_mutex);
	AfxGetApp()->ExitInstance();
	exit(0);
	return true;
}
