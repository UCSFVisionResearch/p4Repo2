/**
 * @file AppCore.h
 * @brief 
 */

#ifndef APPCORE_H
#define APPCORE_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

/* Win32 includes */
#include <afxwin.h>
#include <afxdlgs.h>
#include <math.h>
#include <afxmt.h>

/* VSG includes */
#include "VSGV8.h"//version 8
#include "VSGV8AD.h"

/* NI Data Aq includes */
#ifndef _NO_ANALOG_IO
#include <nidaq.h>
#include <nidaqex.h>
#endif

/* Resources */
#include "res/resource.h"

/* General dialogs */
#include "suites/settings/SettingsDlg.h"
#include "suites/calibration/CalibrationDlg.h"
//#include "EyeTrackingDlg.h"
#include "suites/pursuit/PursuitDlg.h"

/* Eye tracking dialogs */
#ifdef APP_EYE_TRACKING
#include "ReceptiveField.h"
#include "suites/stepramp/StepRampDlg.h"
#include "suites/fullfield/FullfieldDlg.h"
#include "suites/saccade/SaccadeDlg.h"
#include "suites/scenecalib/SceneCalibrationDlg.h"
#include "suites/ocular_following/OcularFollowingDlg.h"
#include "suites/isoluminant/IsoluminantPatchDlg.h"
#include "suites/orientation_tuning/OrientationTuningDlg.h"
#include "suites/rfmappings/RFMappingDlg.h"
#include "suites/circle_annulus/CircleAnnulusDlg.h"
#include "suites/apd/APDDlg.h"
#include "suites/remember/RememberDlg.h"
#include "suites/suppression/SuppressionDlg.h"
#include "suites/alternation/AlternationDlg.h"
#include "suites/msequence/MSequenceDlg.h"
#include "suites/ruler/RulerDlg.h"
#include "suites/amblyoscope/AmblyoscopeDlg.h"
#include "suites/scotoma_map/ScotomaMapDlg.h"
#include "suites/trial_saccades/TrialSaccadesDlg.h"
#include "suites/random_dot/RandomDotAnaglyphDlg.h"
#include "suites/gratings/GratingsDlg.h"
#endif

/* Acuity dialogs */
#ifdef APP_ACUITY
#include "NoiseGratingsDlg.h"
#include "PrefSelectionDlg.h"
#endif

/* Container classes */
#include "suites/calibration/Calibration.h"
#include "suites/settings/Settings.h"
#include "suites/settings/StaircaseSettings.h"
#include "suites/pursuit/Pursuit.h"

/* Eye tracking models */
#ifdef APP_EYE_TRACKING
#include "suites/stepramp/Stepramp.h"
#include "suites/fullfield/Fullfield.h"
#include "suites/saccade/Saccade.h"
#include "suites/scenecalib/SceneCalibration.h"
#include "suites/ocular_following/OcularFollowing.h"
#include "suites/isoluminant/IsoluminantPatch.h"
#include "suites/orientation_tuning/OrientationTuning.h"
#include "suites/rfmappings/RFMapping.h"
#include "suites/circle_annulus/CircleAnnulus.h"
#include "suites/apd/APD.h"
#include "suites/remember/Remember.h"
#include "suites/suppression/Suppression.h"
#include "suites/alternation/Alternation.h"
#include "suites/msequence/MSequence.h"
#include "suites/ruler/Ruler.h"
#include "suites/amblyoscope/Amblyoscope.h"
#include "suites/trial_saccades/TrialSaccades.h"
#include "suites/scotoma_map/ScotomaMap.h"
#include "suites/random_dot/RandomDotAnaglyph.h"
#include "suites/gratings/Gratings.h"
#endif

/* Acuity models */
#ifdef APP_ACUITY
#include "PrefSelection.h"
#include "NoiseGratings.h"
#endif

/* Experiment Paradigms */
//#include "utils/ConstantStimuli.h"
#include "suites/settings/Staircase.h"
#include "suites/scotoma_map/ScotomaTrial.h"
#include "suites/gratings/GratingsTrial.h"

/* Runtime classes */
#include "suites/generic/GenericStimulus.h"
#include "suites/calibration/CalibrationStimulus.h"
//#include "comm/SerialComm.h"
//#include "comm/SMISerialComm.h"
#include "suites/pursuit/PursuitStimulus.h"

/* Eye tracking runtime */
#ifdef APP_EYE_TRACKING
#include "suites/saccade/SaccadeStimulus.h"
#include "suites/fullfield/FullfieldStimulus.h"
#include "suites/stepramp/StepRampStimulus.h"
#include "suites/scenecalib/SceneCalibrationStimulus.h"
#include "suites/ocular_following/OcularFollowingStimulus.h"
#include "suites/isoluminant/IsoluminantPatchStimulus.h"
#include "suites/orientation_tuning/OrientationTuningStimulus.h"
#include "suites/rfmappings/RFMappingStimulus.h"
#include "suites/circle_annulus/CircleAnnulusStimulus.h"
#include "suites/apd/APDStimulus.h"
#include "suites/remember/RememberStimulus.h"
#include "suites/suppression/SuppressionStimulus.h"
#include "suites/alternation/AlternationStimulus.h"
#include "suites/msequence/MSequenceStimulus.h"
#include "suites/ruler/RulerStimulus.h"
#include "suites/amblyoscope/AmblyoscopeStimulus.h"
#include "suites/scotoma_map/ScotomaMapStimulus.h"
#include "suites/trial_saccades/TrialSaccadesStimulus.h"
#include "suites/random_dot/RandomDotAnaglyphStimulus.h"
#include "suites/gratings/GratingsStimulus.h"
#endif

/* Acuity runtime */
#ifdef APP_ACUITY
#include "PrefSelectionStimulus.h"
#include "NoiseGratingsStimulus.h"
#endif

/*****************************************************************************
 * Forward declarations
 *****************************************************************************/

class CEyeTrackingDlg;
class CSMISerialComm;

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

bool InitApp();
bool RunApp();
bool ExitApp();
bool SetActiveDialog(CDialog* pNewDialog);

/*****************************************************************************
 * Statics and Globals
 *****************************************************************************/

extern CEyeTrackingDlg* g_pEyeTrackingDlg;
extern CCalibrationDlg* g_pCalibrationDlg;
extern CSettingsDlg* g_pSettingsDlg;
extern CPursuitDlg* g_pPursuitDlg;

#ifdef APP_EYE_TRACKING
extern CSaccadeDlg* g_pSaccadeDlg;
extern CFullfieldDlg* g_pFullfieldDlg;
extern CStepRampDlg* g_pStepRampDlg;
extern CSceneCalibrationDlg* g_pSceneCalibrationDlg;
extern COcularFollowingDlg* g_pOcularFollowingDlg;
extern CIsoluminantPatchDlg* g_pIsoluminantPatchDlg;
extern COrientationTuningDlg* g_pOrientationTuningDlg;
extern CRFMappingDlg* g_pRFMappingDlg;
extern CCircleAnnulusDlg* g_pCircleAnnulusDlg;
extern CAPDDlg* g_pAPDDlg;
extern CRememberDlg* g_pRememberDlg;
extern CSuppressionDlg* g_pSuppressionDlg;
extern CAlternationDlg* g_pAlternationDlg;
extern CMSequenceDlg* g_pMSequenceDlg;
extern CRulerDlg* g_pRulerDlg;
extern CAmblyoscopeDlg* g_pAmblyoscopeDlg;
extern CScotomaMapDlg* g_pScotomaMapDlg;
extern CTrialSaccadesDlg* g_pTrialSaccadesDlg;
extern CRandomDotAnaglyphDlg* g_pRandomDotAnaglyphDlg;
extern CGratingsDlg* g_pGratingsDlg;
#endif

#ifdef APP_ACUITY
extern CPrefSelectionDlg* g_pPrefSelectionDlg;
extern CNoiseGratingsDlg* g_pNoiseGratingsDlg;
#endif 

extern CCalibration* g_pCalibration;
extern CSettings* g_pSettings;
extern CPursuit* g_pPursuit;

#ifdef APP_EYE_TRACKING
extern CSaccade* g_pSaccade;
extern CFullfield* g_pFullfield;
extern CStepRamp* g_pStepRamp;
extern CSceneCalibration* g_pSceneCalibration;
extern COcularFollowing* g_pOcularFollowing;
extern CIsoluminantPatch* g_pIsoluminantPatch;
extern COrientationTuning* g_pOrientationTuning;
extern CRFMapping* g_pRFMapping;
extern CCircleAnnulus* g_pCircleAnnulus;
extern CAPD* g_pAPD;
extern CRemember* g_pRemember;
extern CSuppression* g_pSuppression;
extern CAlternation* g_pAlternation;
extern CMSequence* g_pMSequence;
extern CRuler* g_pRuler;
extern CAmblyoscope* g_pAmblyoscope;
extern CScotomaMap* g_pScotomaMap;
extern CTrialSaccades* g_pTrialSaccades;
extern CRandomDotAnaglyph* g_pRandomDotAnaglyph;
extern CGratings* g_pGratings;
#endif

#ifdef APP_ACUITY
extern CPrefSelection* g_pPrefSelection;
extern CNoiseGratings* g_pNoiseGratings;
#endif

// global vars
extern CGenericStimulus* g_pActiveStimulus;
extern DWORD g_dHalfFrameInterval;
extern double g_dSleepThreshold;
extern bool g_bIsInitialized;
extern VSGOBJHANDLE g_stim;
extern VSGOBJHANDLE g_fixation;
extern VSGOBJHANDLE g_grating;
extern VSGOBJHANDLE g_stim2;
extern double g_dFramerate;
extern CSMISerialComm g_serialCommLeft;	
extern CSMISerialComm g_serialCommRight;
extern CReceptiveField g_receptiveFieldRight;
extern CReceptiveField g_receptiveFieldLeft;

extern HANDLE g_mutex;

#endif // APPCORE_H
