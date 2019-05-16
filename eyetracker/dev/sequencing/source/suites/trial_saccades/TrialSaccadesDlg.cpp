/**
 * @file TrialSaccadesDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <algorithm>
#include "app/stdafx.h"
#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "TrialSaccadesDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CTrialSaccadesDlg, CDialog)
CTrialSaccadesDlg::CTrialSaccadesDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTrialSaccadesDlg::IDD, pParent),
  m_trialTrackers(CSaccadeTrial::NUM_CODES),
  m_crossGraphicState(NULL),
  m_dotGraphicState(NULL)
{}

CTrialSaccadesDlg::~CTrialSaccadesDlg() {}

void CTrialSaccadesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_tbsPrimaryRed, m_PrimaryRed);
	DDX_Control(pDX, IDC_tbsPrimaryGreen, m_PrimaryGreen);
	DDX_Control(pDX, IDC_tbsPrimaryBlue, m_PrimaryBlue);
	DDX_Control(pDX, IDC_tbsPrimaryStatic, m_PrimaryStatic);

	DDX_Control(pDX, IDC_tbsSecondaryRed, m_SecondaryRed);
	DDX_Control(pDX, IDC_tbsSecondaryGreen, m_SecondaryGreen);
	DDX_Control(pDX, IDC_tbsSecondaryBlue, m_SecondaryBlue);
	DDX_Control(pDX, IDC_tbsSecondaryStatic, m_SecondaryStatic);

	DDX_Control(pDX, IDC_tbsTertiaryRed, m_TertiaryRed);
	DDX_Control(pDX, IDC_tbsTertiaryGreen, m_TertiaryGreen);
	DDX_Control(pDX, IDC_tbsTertiaryBlue, m_TertiaryBlue);
	DDX_Control(pDX, IDC_tbsTertiaryStatic, m_TertiaryStatic);

	DDX_Control(pDX, IDC_tbsFixationFlashDelay, m_FixationFlashDelay);
	DDX_Control(pDX, IDC_tbsMisbehaviorDelay, m_MisbehaviorDelay);
	DDX_Control(pDX, IDC_tbsWrongDelay, m_WrongDelay);
	DDX_Control(pDX, IDC_tbsRewardDelay, m_RewardDelay);
	DDX_Control(pDX, IDC_tbsBlankTimeOutScreen, m_BlankTimeOutScreen);

	DDX_Control(pDX, IDC_tbsEyeCoverageRadioSpatiotopic, m_EyeCoverageRadioSpatiotopic);
	DDX_Control(pDX, IDC_tbsEyeCoverageRadioRetinotopic, m_EyeCoverageRadioRetinotopic);
	DDX_Control(pDX, IDC_tbsUseApprox, m_UseApprox);
	DDX_Control(pDX, IDC_tbsEyeCoverageDescription, m_EyeCoverageDescription);

	DDX_Control(pDX, IDC_tbsInfoDisplay, m_InfoDisplay);
	DDX_Control(pDX, IDC_tbsEyePosnDisplayLX, m_EyePosnDisplayLX);
	DDX_Control(pDX, IDC_tbsEyePosnDisplayLY, m_EyePosnDisplayLY);
	DDX_Control(pDX, IDC_tbsEyePosnDisplayRX, m_EyePosnDisplayRX);
	DDX_Control(pDX, IDC_tbsEyePosnDisplayRY, m_EyePosnDisplayRY);
	
	DDX_Control(pDX, IDC_tbsPauseDotOff, m_PauseDotOff);
	DDX_Control(pDX, IDC_tbsPauseDotOn, m_PauseDotOn);
	DDX_Control(pDX, IDC_tbsDebugResponderOn, m_debugResponderOn);
	DDX_Control(pDX, IDC_tbsDebugLabelsOn, m_debugLabelsOn);
	DDX_Control(pDX, IDC_tbsCenterDotSize, m_CenterDotSize);
	DDX_Control(pDX, IDC_tbsCrossPix, m_CrossPix);
	DDX_Control(pDX, IDC_tbsCenterXPos, m_CenterXPos);
	DDX_Control(pDX, IDC_tbsCenterYPos, m_CenterYPos);
	DDX_Control(pDX, IDC_tbscStepSizeDeg, m_cStepSizeDeg);
	DDX_Control(pDX, IDC_tbsNumStepsX, m_cNumStepsX);
	DDX_Control(pDX, IDC_tbsNumStepsY, m_cNumStepsY);

	DDX_Control(pDX, IDC_tbsCenterRadioPrimary,   m_CenterRadioPrimary);
	DDX_Control(pDX, IDC_tbsCenterRadioSecondary, m_CenterRadioSecondary);
	DDX_Control(pDX, IDC_tbsCenterRadioTertiary,  m_CenterRadioTertiary);
	DDX_Control(pDX, IDC_tbsCenterRadioAlternate, m_CenterRadioAlternate);
	DDX_Control(pDX, IDC_tbsAlternateBlockSize,   m_AlternateBlockSize);
	DDX_Control(pDX, IDC_tbsAlternateWithPurple,  m_AlternateWithPurple);
	DDX_Control(pDX, IDC_tbsCenterRadioRandomize, m_CenterRadioRandomize);
	DDX_Control(pDX, IDC_tbsPctRedCtr,            m_PctRedCtr);
	DDX_Control(pDX, IDC_tbsPctBlueCtr,           m_PctBlueCtr);

	/* Handling of saccade target sequence and what triggers saccade. */
	DDX_Control(pDX, IDC_tbsTrialSequenceImgCross, m_TrialSequenceImgCross);
	DDX_Control(pDX, IDC_tbsTrialSequenceImgDot, m_TrialSequenceImgDot);
	DDX_Control(pDX, IDC_tbsTrialSequenceImgTimeScale, m_TrialSequenceImgTimeScale);
	DDX_Control(pDX, IDC_tbsKeyDisplayedStatic, m_KeyDisplayedStatic);
	DDX_Control(pDX, IDC_tbsKeyBaitedStatic, m_KeyBaitedStatic);
	DDX_Control(pDX, IDC_tbsInitTimeMin, m_InitTimeMin);
	DDX_Control(pDX, IDC_tbsInitTimeMax, m_InitTimeMax);
	DDX_Control(pDX, IDC_tbsFlashTimeMin, m_FlashTimeMin);
	DDX_Control(pDX, IDC_tbsFlashTimeMax, m_FlashTimeMax);
	DDX_Control(pDX, IDC_tbsDelayTimeMin, m_DelayTimeMin);
	DDX_Control(pDX, IDC_tbsDelayTimeMax, m_DelayTimeMax);
	DDX_Control(pDX, IDC_tbsBaitedTime, m_BaitedTime);
	DDX_Control(pDX, IDC_tbsHoldTime, m_HoldTime);
	DDX_Control(pDX, IDC_tbsAdvanceEitherEye, m_AdvanceEitherEye);
	
	DDX_Control(pDX, IDC_tbsPeriphDotSize, m_PeriphDotSize);
	DDX_Control(pDX, IDC_tbsUseShapes, m_UseShapes);
	DDX_Control(pDX, IDC_tbsDotGratings, m_DotGratings);
	DDX_Control(pDX, IDC_tbsDotGratingContrast, m_DotGratingContrast);
	DDX_Control(pDX, IDC_tbsDotGratingCycles, m_DotGratingCycles);

	/* Handling of grid for fixation */
	DDX_Control(pDX, IDC_tbsGridSettingsRadioRandom, m_GridSettingsRadioRandom);
	DDX_Control(pDX, IDC_tbsGridSettingsRadioCyclopean, m_GridSettingsRadioCyclopean);
	DDX_Control(pDX, IDC_tbsGridSettingsDescription, m_GridSettingsDescription);
	DDX_Control(pDX, IDC_tbsDeviationAngle, m_DeviationAngle);

	DDX_Control(pDX, IDC_tbsPeriphRadioPrimary,   m_PeriphRadioPrimary);
	DDX_Control(pDX, IDC_tbsPeriphRadioSecondary, m_PeriphRadioSecondary);
	DDX_Control(pDX, IDC_tbsPeriphRadioTertiary,  m_PeriphRadioTertiary);
	DDX_Control(pDX, IDC_tbsPeriphRadioAlternate, m_PeriphRadioAlternate);
	DDX_Control(pDX, IDC_tbsPeriphRadioRandomize, m_PeriphRadioRandomize);
	DDX_Control(pDX, IDC_tbsPctRedTrials,         m_PctRedTrials);
	DDX_Control(pDX, IDC_tbsPctBlueTrials,        m_PctBlueTrials);

	/* Cartesian Grid controls */
	DDX_Control(pDX, IDC_tbsGridRadioCartesian, m_GridRadioCartesian);
	DDX_Control(pDX, IDC_tbsNumStepsH, m_NumStepsH);
	DDX_Control(pDX, IDC_tbsNumStepsV, m_NumStepsV);
	DDX_Control(pDX, IDC_tbsStepSize, m_StepSize);
	DDX_Control(pDX, IDC_tbsAvoidZero, m_AvoidZero);

	/* Radial grid controls */
	DDX_Control(pDX, IDC_tbsGridRadioRadial, m_GridRadioRadial);
	DDX_Control(pDX, IDC_tbsAngleOffset, m_AngleOffset);
	DDX_Control(pDX, IDC_tbsAngle0, m_Angle0);
	DDX_Control(pDX, IDC_tbsAngle45, m_Angle45);
	DDX_Control(pDX, IDC_tbsAngle90, m_Angle90);
	DDX_Control(pDX, IDC_tbsAngle135, m_Angle135);
	DDX_Control(pDX, IDC_tbsAngle180, m_Angle180);
	DDX_Control(pDX, IDC_tbsAngle225, m_Angle225);
	DDX_Control(pDX, IDC_tbsAngle270, m_Angle270);
	DDX_Control(pDX, IDC_tbsAngle315, m_Angle315);
	DDX_Control(pDX, IDC_tbsRadiusOffset, m_RadiusOffset);
	DDX_Control(pDX, IDC_tbsRadiusOrigin, m_RadiusOrigin);
	DDX_Control(pDX, IDC_tbsRadius0, m_Radius0);
	DDX_Control(pDX, IDC_tbsRadius5, m_Radius5);
	DDX_Control(pDX, IDC_tbsRadius10, m_Radius10);
	DDX_Control(pDX, IDC_tbsRadius15, m_Radius15);
	DDX_Control(pDX, IDC_tbsRadius20, m_Radius20);
	DDX_Control(pDX, IDC_tbsRadius25, m_Radius25);
	DDX_Control(pDX, IDC_tbsRadius30, m_Radius30);

	/* Noise controls */
	DDX_Control(pDX, IDC_tbsUseNoise, m_UseNoise);
	DDX_Control(pDX, IDC_tbsSameBG, m_SameBG);
	DDX_Control(pDX, IDC_tbsInterleaveNoise, m_InterleaveNoise);
	DDX_Control(pDX, IDC_tbsOccludeBG, m_OccludeBG);
	DDX_Control(pDX, IDC_tbsNoiseSize, m_NoiseSize);
	DDX_Control(pDX, IDC_tbsNoisePct, m_NoisePct);
	DDX_Control(pDX, IDC_tbsNoiseRed, m_NoiseRed);
	DDX_Control(pDX, IDC_tbsNoiseGreen, m_NoiseGreen);
	DDX_Control(pDX, IDC_tbsNoiseBlue, m_NoiseBlue);
	DDX_Control(pDX, IDC_tbsNoiseStatic, m_NoiseStatic);

	DDX_Control(pDX, IDC_tbsTrialResults_RR_R, m_TrialResults_RR_R);
	DDX_Control(pDX, IDC_tbsTrialResults_RR_W, m_TrialResults_RR_W);
	DDX_Control(pDX, IDC_tbsTrialResults_RR_T, m_TrialResults_RR_T);

	DDX_Control(pDX, IDC_tbsTrialResults_RB_R, m_TrialResults_RB_R);
	DDX_Control(pDX, IDC_tbsTrialResults_RB_W, m_TrialResults_RB_W);
	DDX_Control(pDX, IDC_tbsTrialResults_RB_T, m_TrialResults_RB_T);

	DDX_Control(pDX, IDC_tbsTrialResults_RP_R, m_TrialResults_RP_R);
	DDX_Control(pDX, IDC_tbsTrialResults_RP_W, m_TrialResults_RP_W);
	DDX_Control(pDX, IDC_tbsTrialResults_RP_T, m_TrialResults_RP_T);

	DDX_Control(pDX, IDC_tbsTrialResults_BR_R, m_TrialResults_BR_R);
	DDX_Control(pDX, IDC_tbsTrialResults_BR_W, m_TrialResults_BR_W);
	DDX_Control(pDX, IDC_tbsTrialResults_BR_T, m_TrialResults_BR_T);

	DDX_Control(pDX, IDC_tbsTrialResults_BB_R, m_TrialResults_BB_R);
	DDX_Control(pDX, IDC_tbsTrialResults_BB_W, m_TrialResults_BB_W);
	DDX_Control(pDX, IDC_tbsTrialResults_BB_T, m_TrialResults_BB_T);

	DDX_Control(pDX, IDC_tbsTrialResults_BP_R, m_TrialResults_BP_R);
	DDX_Control(pDX, IDC_tbsTrialResults_BP_W, m_TrialResults_BP_W);
	DDX_Control(pDX, IDC_tbsTrialResults_BP_T, m_TrialResults_BP_T);

	DDX_Control(pDX, IDC_tbsTrialResults_PR_R, m_TrialResults_PR_R);
	DDX_Control(pDX, IDC_tbsTrialResults_PR_W, m_TrialResults_PR_W);
	DDX_Control(pDX, IDC_tbsTrialResults_PR_T, m_TrialResults_PR_T);

	DDX_Control(pDX, IDC_tbsTrialResults_PB_R, m_TrialResults_PB_R);
	DDX_Control(pDX, IDC_tbsTrialResults_PB_W, m_TrialResults_PB_W);
	DDX_Control(pDX, IDC_tbsTrialResults_PB_T, m_TrialResults_PB_T);

	DDX_Control(pDX, IDC_tbsTrialResults_PP_R, m_TrialResults_PP_R);
	DDX_Control(pDX, IDC_tbsTrialResults_PP_W, m_TrialResults_PP_W);
	DDX_Control(pDX, IDC_tbsTrialResults_PP_T, m_TrialResults_PP_T);

	DDX_Control(pDX, IDC_tbsTrialResults_R_T, m_TrialResults_R_T);
	DDX_Control(pDX, IDC_tbsTrialResults_W_T, m_TrialResults_W_T);
	DDX_Control(pDX, IDC_tbsTrialResults_Total, m_TrialResults_Total);

	DDX_Control(pDX, IDC_tbsTrialResultsBorderPrimary, m_TrialResultsBorderPrimary);
	DDX_Control(pDX, IDC_tbsTrialResultsBorderSecondary, m_TrialResultsBorderSecondary);
	DDX_Control(pDX, IDC_tbsTrialResultsBorderTertiary, m_TrialResultsBorderTertiary);
}

BEGIN_MESSAGE_MAP(CTrialSaccadesDlg, CDialog)
	ON_BN_CLICKED(IDC_tbsButtonApply, OnBnClickedApply)
	ON_BN_CLICKED(IDC_tbsEyeCoverageRadioSpatiotopic, OnBnClickedEyeCoverageRadioSpatiotopic)
	ON_BN_CLICKED(IDC_tbsEyeCoverageRadioRetinotopic, OnBnClickedEyeCoverageRadioRetinotopic)
	ON_BN_CLICKED(IDC_tbsUseApprox, OnBnClickedUseApprox)
	ON_BN_CLICKED(IDC_tbsCenterRadioPrimary,   OnBnClickedCenterRadioPrimary)
	ON_BN_CLICKED(IDC_tbsCenterRadioSecondary, OnBnClickedCenterRadioSecondary)
	ON_BN_CLICKED(IDC_tbsCenterRadioTertiary,  OnBnClickedCenterRadioTertiary)
	ON_BN_CLICKED(IDC_tbsCenterRadioAlternate, OnBnClickedCenterRadioAlternate)
	ON_BN_CLICKED(IDC_tbsCenterRadioRandomize, OnBnClickedCenterRadioRandomize)
	ON_BN_CLICKED(IDC_tbsPeriphRadioPrimary,   OnBnClickedPeriphRadioPrimary)
	ON_BN_CLICKED(IDC_tbsPeriphRadioSecondary, OnBnClickedPeriphRadioSecondary)
	ON_BN_CLICKED(IDC_tbsPeriphRadioTertiary,  OnBnClickedPeriphRadioTertiary)
	ON_BN_CLICKED(IDC_tbsPeriphRadioAlternate, OnBnClickedPeriphRadioAlternate)
	ON_BN_CLICKED(IDC_tbsPeriphRadioRandomize, OnBnClickedPeriphRadioRandomize)
	ON_BN_CLICKED(IDC_tbsGridSettingsRadioRandom, OnBnClickedGridSettingsRadioRandom)
	ON_BN_CLICKED(IDC_tbsGridSettingsRadioCyclopean, OnBnClickedGridSettingsRadioCyclopean)
	ON_BN_CLICKED(IDC_tbsGridRadioCartesian, OnBnClickedGridRadioCartesian)
	ON_BN_CLICKED(IDC_tbsGridRadioRadial, OnBnClickedGridRadioRadial)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_tbsUseNoise, OnBnClickedUseNoise)
END_MESSAGE_MAP()

BOOL CTrialSaccadesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTrialSaccadesDlg::load() 
{
	m_PrimaryRed.SetWindowText(etu::IntToString(g_pTrialSaccades->getPrimaryRed()));
	m_PrimaryGreen.SetWindowText(etu::IntToString(g_pTrialSaccades->getPrimaryGreen()));
	m_PrimaryBlue.SetWindowText(etu::IntToString(g_pTrialSaccades->getPrimaryBlue()));
	m_SecondaryRed.SetWindowText(etu::IntToString(g_pTrialSaccades->getSecondaryRed()));
	m_SecondaryGreen.SetWindowText(etu::IntToString(g_pTrialSaccades->getSecondaryGreen()));
	m_SecondaryBlue.SetWindowText(etu::IntToString(g_pTrialSaccades->getSecondaryBlue()));
	m_TertiaryRed.SetWindowText(etu::IntToString(g_pTrialSaccades->getTertiaryRed()));
	m_TertiaryGreen.SetWindowText(etu::IntToString(g_pTrialSaccades->getTertiaryGreen()));
	m_TertiaryBlue.SetWindowText(etu::IntToString(g_pTrialSaccades->getTertiaryBlue()));
	m_NoiseRed.SetWindowText(etu::IntToString(g_pTrialSaccades->getNoiseRed()));
	m_NoiseGreen.SetWindowText(etu::IntToString(g_pTrialSaccades->getNoiseGreen()));
	m_NoiseBlue.SetWindowText(etu::IntToString(g_pTrialSaccades->getNoiseBlue()));
	
	m_BlankTimeOutScreen.SetCheck(g_pTrialSaccades->getBlankTimeOutScreen() ? BST_CHECKED : BST_UNCHECKED);
	
	m_WrongDelay.SetWindowText(etu::IntToString(g_pTrialSaccades->getWrongDelay()));
	m_NoisePct.SetWindowText(etu::IntToString(g_pTrialSaccades->getNoisePct()));
	m_NoiseSize.SetWindowText(etu::IntToString(g_pTrialSaccades->getNoiseSize()));
	m_MisbehaviorDelay.SetWindowText(etu::IntToString(g_pTrialSaccades->getMisbehaviorDelay()));

	m_InitTimeMin.SetWindowText(etu::IntToString(g_pTrialSaccades->getInitTimeMin()));
	m_InitTimeMax.SetWindowText(etu::IntToString(g_pTrialSaccades->getInitTimeMax()));
	m_FlashTimeMin.SetWindowText(etu::IntToString(g_pTrialSaccades->getFlashTimeMin()));
	m_FlashTimeMax.SetWindowText(etu::IntToString(g_pTrialSaccades->getFlashTimeMax()));
	m_DelayTimeMin.SetWindowText(etu::IntToString(g_pTrialSaccades->getDelayTimeMin()));
	m_DelayTimeMax.SetWindowText(etu::IntToString(g_pTrialSaccades->getDelayTimeMax()));
	m_BaitedTime.SetWindowText(etu::IntToString(g_pTrialSaccades->getBaitedTime()));
	m_HoldTime.SetWindowText(etu::IntToString(g_pTrialSaccades->getHoldTime()));
	m_AdvanceEitherEye.SetCheck(g_pTrialSaccades->getAdvanceEitherEye() ? BST_CHECKED : BST_UNCHECKED);

	m_CrossPix.SetWindowText(etu::IntToString(g_pTrialSaccades->getCrossPix()));
	m_UseNoise.SetCheck((g_pTrialSaccades->getUseNoise())? BST_CHECKED : BST_UNCHECKED);
	m_InterleaveNoise.SetCheck((g_pTrialSaccades->getInterleaveNoise())? BST_CHECKED : BST_UNCHECKED);
	
	m_CenterXPos.SetWindowText(etu::FloatToString(g_pTrialSaccades->getCenterXPos()));
	m_CenterYPos.SetWindowText(etu::FloatToString(g_pTrialSaccades->getCenterYPos()));
	m_cStepSizeDeg.SetWindowText(etu::FloatToString(g_pTrialSaccades->getCtrStepSize()));
	m_cNumStepsX.SetWindowText(etu::IntToString(g_pTrialSaccades->getNumStepsX()));
	m_cNumStepsY.SetWindowText(etu::IntToString(g_pTrialSaccades->getNumStepsY()));
	m_CenterDotSize.SetWindowText(etu::FloatToString(g_pTrialSaccades->getCenterDotSize()));
	m_PeriphDotSize.SetWindowText(etu::FloatToString(g_pTrialSaccades->getPeriphDotSize()));

	m_debugResponderOn.SetCheck(g_pTrialSaccades->getDebugResponderOn() ? BST_CHECKED : BST_UNCHECKED);
	m_debugResponderOn.SetCheck(g_pTrialSaccades->getDebugLabelsOn() ? BST_CHECKED : BST_UNCHECKED);

	/* Color radios depend on grid settings.  Do color first so we overwrite 
	 * later. */
	switch (g_pTrialSaccades->getCenterColorRadio()) {
		case CTrialSaccades::COLOR_PRIMARY:   OnBnClickedCenterRadioPrimary();   break;
		case CTrialSaccades::COLOR_SECONDARY: OnBnClickedCenterRadioSecondary(); break;
		case CTrialSaccades::COLOR_TERTIARY:  OnBnClickedCenterRadioTertiary();  break;
		case CTrialSaccades::COLOR_ALTERNATE: OnBnClickedCenterRadioAlternate(); break;
		default: /* COLOR_RANDOM */           OnBnClickedCenterRadioRandomize(); break;
	}

	m_AlternateBlockSize.SetWindowText(etu::IntToString(g_pTrialSaccades->getAlternateBlockSize()));
	m_AlternateWithPurple.SetCheck(g_pTrialSaccades->getAlternateWithPurple() ? BST_CHECKED : BST_UNCHECKED);

	m_PctRedCtr.SetWindowText(etu::IntToString(g_pTrialSaccades->getPctRedCtr()));
	m_PctBlueCtr.SetWindowText(etu::IntToString(g_pTrialSaccades->getPctBlueCtr()));

	switch (g_pTrialSaccades->getPeriphColorRadio()) {
		case CTrialSaccades::COLOR_PRIMARY:   OnBnClickedPeriphRadioPrimary();   break;
		case CTrialSaccades::COLOR_SECONDARY: OnBnClickedPeriphRadioSecondary(); break;
		case CTrialSaccades::COLOR_TERTIARY:  OnBnClickedPeriphRadioTertiary();  break;
		case CTrialSaccades::COLOR_ALTERNATE: OnBnClickedPeriphRadioAlternate(); break;
		default: /* COLOR_RANDOM */           OnBnClickedPeriphRadioRandomize(); break;
	}

	m_PctRedTrials.SetWindowText(etu::IntToString(g_pTrialSaccades->getPctRedGrids()));
	m_PctBlueTrials.SetWindowText(etu::IntToString(g_pTrialSaccades->getPctBlueGrids()));

	/* Grid settings radios depend on eye coverage.  Do grid now so we 
	 * overwrite later. */
	switch (g_pTrialSaccades->getFixationEye()) {
		case CTrialSaccades::FIXATION_CYCLOPEAN: OnBnClickedGridSettingsRadioCyclopean(); break;
		default: /* FIXATION_RANDOM */           OnBnClickedGridSettingsRadioRandom();    break;
	}

	/* Eye coverage done last to overwrite. */
	m_DeviationAngle.SetWindowText(etu::FloatToString(g_pTrialSaccades->getDeviationAngle()));

	switch (g_pTrialSaccades->getEyeCoverage()) {
		case CTrialSaccades::COVERAGE_RETINOTOPIC: OnBnClickedEyeCoverageRadioRetinotopic(); break;
		default: /* COVERAGE_SPATIOTOPIC */        OnBnClickedEyeCoverageRadioSpatiotopic(); break;
	}

	m_UseApprox.SetCheck(g_pTrialSaccades->getUseApprox() ? BST_CHECKED : BST_UNCHECKED);
	
	/* Grid handling ********************************************************/
	m_NumStepsH.SetWindowText(etu::IntToString(g_pTrialSaccades->getNumStepsH()));
	m_NumStepsV.SetWindowText(etu::IntToString(g_pTrialSaccades->getNumStepsV()));
	m_StepSize.SetWindowText(etu::FloatToString(g_pTrialSaccades->getStepSize()));
	m_AvoidZero.SetCheck((g_pTrialSaccades->getAvoidZero())? BST_CHECKED : BST_UNCHECKED);

	m_AngleOffset.SetWindowText(etu::IntToString(g_pTrialSaccades->getAngleOffset()));
	m_Angle0.SetCheck((g_pTrialSaccades->getAngle0()) ? BST_CHECKED : BST_UNCHECKED);
	m_Angle45.SetCheck((g_pTrialSaccades->getAngle45()) ? BST_CHECKED : BST_UNCHECKED);
	m_Angle90.SetCheck((g_pTrialSaccades->getAngle90()) ? BST_CHECKED : BST_UNCHECKED);
	m_Angle135.SetCheck((g_pTrialSaccades->getAngle135()) ? BST_CHECKED : BST_UNCHECKED);
	m_Angle180.SetCheck((g_pTrialSaccades->getAngle180()) ? BST_CHECKED : BST_UNCHECKED);
	m_Angle225.SetCheck((g_pTrialSaccades->getAngle225()) ? BST_CHECKED : BST_UNCHECKED);
	m_Angle270.SetCheck((g_pTrialSaccades->getAngle270()) ? BST_CHECKED : BST_UNCHECKED);
	m_Angle315.SetCheck((g_pTrialSaccades->getAngle315()) ? BST_CHECKED : BST_UNCHECKED);
	m_RadiusOffset.SetWindowText(etu::IntToString(g_pTrialSaccades->getRadiusOffset()));
	m_RadiusOrigin.SetCheck((g_pTrialSaccades->getRadiusOrigin()) ? BST_CHECKED : BST_UNCHECKED);
	m_Radius0.SetCheck((g_pTrialSaccades->getRadius0()) ? BST_CHECKED : BST_UNCHECKED);
	m_Radius5.SetCheck((g_pTrialSaccades->getRadius5()) ? BST_CHECKED : BST_UNCHECKED);
	m_Radius10.SetCheck((g_pTrialSaccades->getRadius10()) ? BST_CHECKED : BST_UNCHECKED);
	m_Radius15.SetCheck((g_pTrialSaccades->getRadius15()) ? BST_CHECKED : BST_UNCHECKED);
	m_Radius20.SetCheck((g_pTrialSaccades->getRadius20()) ? BST_CHECKED : BST_UNCHECKED);
	m_Radius25.SetCheck((g_pTrialSaccades->getRadius25()) ? BST_CHECKED : BST_UNCHECKED);
	m_Radius30.SetCheck((g_pTrialSaccades->getRadius30()) ? BST_CHECKED : BST_UNCHECKED);

	switch (g_pTrialSaccades->getGridType()) {
		case CTrialSaccades::GRID_CARTESIAN: OnBnClickedGridRadioCartesian(); break;
		case CTrialSaccades::GRID_RADIAL: OnBnClickedGridRadioRadial(); break;
	}
	/* End of grid handling *************************************************/

	PaintColorStatics();

	m_RewardDelay.SetWindowText(etu::IntToString(g_pTrialSaccades->getRewardDelay()));
	m_UseShapes.SetCheck((g_pTrialSaccades->getUseShapes())? BST_CHECKED : BST_UNCHECKED);
	m_SameBG.SetCheck((g_pTrialSaccades->getSameBG())? BST_CHECKED : BST_UNCHECKED);
	m_OccludeBG.SetCheck((g_pTrialSaccades->getOccludeBG())? BST_CHECKED : BST_UNCHECKED);
	m_DotGratings.SetCheck((g_pTrialSaccades->getDotGratings())? BST_CHECKED : BST_UNCHECKED);
	m_DotGratingContrast.SetWindowText(etu::IntToString(g_pTrialSaccades->getDotGratingContrast()));
	m_DotGratingCycles.SetWindowText(etu::FloatToString(g_pTrialSaccades->getDotGratingCycles()));
}

void CTrialSaccadesDlg::save() 
{
	SmartLock saccadeTrialLock(*g_pTrialSaccades);
	SmartLock applyButtonLock(*this); /* Lock on own mutex as well. */

	char buffer[100];
	char debugbuffer[1024];

	m_PrimaryRed.GetWindowText(buffer, 100);
	g_pTrialSaccades->setPrimaryRed(etu::StringToInt(buffer, g_pTrialSaccades->getPrimaryRed()));
	m_PrimaryGreen.GetWindowText(buffer, 100);
	g_pTrialSaccades->setPrimaryGreen(etu::StringToInt(buffer, g_pTrialSaccades->getPrimaryGreen()));
	m_PrimaryBlue.GetWindowText(buffer, 100);
	g_pTrialSaccades->setPrimaryBlue(etu::StringToInt(buffer, g_pTrialSaccades->getPrimaryBlue()));

	m_SecondaryRed.GetWindowText(buffer, 100);
	g_pTrialSaccades->setSecondaryRed(etu::StringToInt(buffer, g_pTrialSaccades->getSecondaryRed()));
	m_SecondaryGreen.GetWindowText(buffer, 100);
	g_pTrialSaccades->setSecondaryGreen(etu::StringToInt(buffer, g_pTrialSaccades->getSecondaryGreen()));
	m_SecondaryBlue.GetWindowText(buffer, 100);
	g_pTrialSaccades->setSecondaryBlue(etu::StringToInt(buffer, g_pTrialSaccades->getSecondaryBlue()));

	m_TertiaryRed.GetWindowText(buffer, 100);
	g_pTrialSaccades->setTertiaryRed(etu::StringToInt(buffer, g_pTrialSaccades->getTertiaryRed()));
	m_TertiaryGreen.GetWindowText(buffer, 100);
	g_pTrialSaccades->setTertiaryGreen(etu::StringToInt(buffer, g_pTrialSaccades->getTertiaryGreen()));
	m_TertiaryBlue.GetWindowText(buffer, 100);
	g_pTrialSaccades->setTertiaryBlue(etu::StringToInt(buffer, g_pTrialSaccades->getTertiaryBlue()));

	m_NoiseRed.GetWindowText(buffer, 100);
	g_pTrialSaccades->setNoiseRed(etu::StringToInt(buffer, g_pTrialSaccades->getNoiseRed()));
	m_NoiseGreen.GetWindowText(buffer, 100);
	g_pTrialSaccades->setNoiseGreen(etu::StringToInt(buffer, g_pTrialSaccades->getNoiseGreen()));
	m_NoiseBlue.GetWindowText(buffer, 100);
	g_pTrialSaccades->setNoiseBlue(etu::StringToInt(buffer, g_pTrialSaccades->getNoiseBlue()));

	g_pTrialSaccades->setBlankTimeOutScreen(m_BlankTimeOutScreen.GetCheck() == BST_CHECKED);
	
	m_WrongDelay.GetWindowText(buffer, 100);
	g_pTrialSaccades->setWrongDelay(etu::StringToInt(buffer, g_pTrialSaccades->getWrongDelay()));
	m_MisbehaviorDelay.GetWindowText(buffer, 100);
	g_pTrialSaccades->setMisbehaviorDelay(etu::StringToInt(buffer, g_pTrialSaccades->getMisbehaviorDelay()));
	m_NoisePct.GetWindowText(buffer, 100);
	g_pTrialSaccades->setNoisePct(etu::StringToInt(buffer, g_pTrialSaccades->getNoisePct()));
	m_NoiseSize.GetWindowText(buffer, 100);
	g_pTrialSaccades->setNoiseSize(etu::StringToInt(buffer, g_pTrialSaccades->getNoiseSize()));
	
	m_InitTimeMin.GetWindowText(buffer, 100);
	g_pTrialSaccades->setInitTimeMin(etu::StringToInt(buffer, g_pTrialSaccades->getInitTimeMin()));
	m_InitTimeMax.GetWindowText(buffer, 100);
	g_pTrialSaccades->setInitTimeMax(etu::StringToInt(buffer, g_pTrialSaccades->getInitTimeMax()));

	m_FlashTimeMin.GetWindowText(buffer, 100);
	g_pTrialSaccades->setFlashTimeMin(etu::StringToInt(buffer, g_pTrialSaccades->getFlashTimeMin()));
	m_FlashTimeMax.GetWindowText(buffer, 100);
	g_pTrialSaccades->setFlashTimeMax(etu::StringToInt(buffer, g_pTrialSaccades->getFlashTimeMax()));

	m_DelayTimeMin.GetWindowText(buffer, 100);
	g_pTrialSaccades->setDelayTimeMin(etu::StringToInt(buffer, g_pTrialSaccades->getDelayTimeMin()));
	m_DelayTimeMax.GetWindowText(buffer, 100);
	g_pTrialSaccades->setDelayTimeMax(etu::StringToInt(buffer, g_pTrialSaccades->getDelayTimeMax()));

	m_BaitedTime.GetWindowText(buffer, 100);
	g_pTrialSaccades->setBaitedTime(etu::StringToInt(buffer, g_pTrialSaccades->getBaitedTime()));
	m_HoldTime.GetWindowText(buffer, 100);
	g_pTrialSaccades->setHoldTime(etu::StringToInt(buffer, g_pTrialSaccades->getHoldTime()));

	g_pTrialSaccades->setAdvanceEitherEye(m_AdvanceEitherEye.GetCheck() == BST_CHECKED);

	m_CrossPix.GetWindowText(buffer, 100);
	g_pTrialSaccades->setCrossPix(etu::StringToInt(buffer, g_pTrialSaccades->getCrossPix()));

	g_pTrialSaccades->setDebugResponderOn(m_debugResponderOn.GetCheck() == BST_CHECKED);
	g_pTrialSaccades->setDebugLabelsOn(m_debugLabelsOn.GetCheck() == BST_CHECKED);

	if (m_EyeCoverageRadioSpatiotopic.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setEyeCoverage(CTrialSaccades::COVERAGE_SPATIOTOPIC);
	}
	else {
		g_pTrialSaccades->setEyeCoverage(CTrialSaccades::COVERAGE_RETINOTOPIC);
	}

	g_pTrialSaccades->setUseApprox(m_UseApprox.GetCheck() == BST_CHECKED);

	m_DeviationAngle.GetWindowText(buffer, 100);
	g_pTrialSaccades->setDeviationAngle(etu::StringToFloat(buffer, g_pTrialSaccades->getDeviationAngle()));

	if (m_GridSettingsRadioRandom.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setFixationEye(CTrialSaccades::FIXATION_RANDOM);
	}
	else {
		g_pTrialSaccades->setFixationEye(CTrialSaccades::FIXATION_CYCLOPEAN);
	}

	if (m_CenterRadioPrimary.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setCenterColorRadio(CTrialSaccades::COLOR_PRIMARY);
	} 
	else if (m_CenterRadioSecondary.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setCenterColorRadio(CTrialSaccades::COLOR_SECONDARY);
	}
	else if (m_CenterRadioTertiary.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setCenterColorRadio(CTrialSaccades::COLOR_TERTIARY);
	}
	else if (m_CenterRadioAlternate.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setCenterColorRadio(CTrialSaccades::COLOR_ALTERNATE);
	} 
	else {
		int pctRedCtr;
		int pctBlueCtr;

		g_pTrialSaccades->setCenterColorRadio(CTrialSaccades::COLOR_RANDOM);

		m_PctRedCtr.GetWindowText(buffer, 100); 
		if (0 > (pctRedCtr = etu::StringToInt(buffer, -1))) {
			pctRedCtr = 50;
		}

		m_PctBlueCtr.GetWindowText(buffer, 100); 
		if (0 > (pctBlueCtr = etu::StringToInt(buffer, -1))) {
			pctBlueCtr = 50;
		}

		/* Have g_pTrialSaccades->setCenterPercents(...) perform acceptance 
		 * logic on values.  Reset display to the values actually accepted. */
		g_pTrialSaccades->setCenterPercents(pctRedCtr, pctBlueCtr);
		pctRedCtr = g_pTrialSaccades->getPctRedCtr();
		pctBlueCtr = g_pTrialSaccades->getPctBlueCtr();
		m_PctRedCtr.SetWindowTextA(etu::IntToString(pctRedCtr));
		m_PctBlueCtr.SetWindowTextA(etu::IntToString(pctBlueCtr));
	}

	m_AlternateBlockSize.GetWindowText(buffer, 100);
	g_pTrialSaccades->setAlternateBlockSize(etu::StringToInt(buffer, g_pTrialSaccades->getAlternateBlockSize()));

	g_pTrialSaccades->setAlternateWithPurple(m_AlternateWithPurple.GetCheck() == BST_CHECKED);

	if (m_PeriphRadioPrimary.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setPeriphColorRadio(CTrialSaccades::COLOR_PRIMARY);
		g_pTrialSaccades->setGridPercents(100, 0);
	} 
	else if (m_PeriphRadioSecondary.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setPeriphColorRadio(CTrialSaccades::COLOR_SECONDARY);
		g_pTrialSaccades->setGridPercents(0, 100);
	}
	else if (m_PeriphRadioTertiary.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setPeriphColorRadio(CTrialSaccades::COLOR_TERTIARY);
		g_pTrialSaccades->setGridPercents(0, 0);
	} 
	else if (m_PeriphRadioAlternate.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setPeriphColorRadio(CTrialSaccades::COLOR_ALTERNATE);
		g_pTrialSaccades->setGridPercents(33, 33);
	} 
	else {
		int pctRedGrids;
		int pctBlueGrids;

		g_pTrialSaccades->setPeriphColorRadio(CTrialSaccades::COLOR_RANDOM);

		m_PctRedTrials.GetWindowText(buffer, 100); 
		if (0 > (pctRedGrids = etu::StringToInt(buffer, -1))) {
			pctRedGrids = 50;
		}

		m_PctBlueTrials.GetWindowText(buffer, 100); 
		if (0 > (pctBlueGrids = etu::StringToInt(buffer, -1))) {
			pctBlueGrids = 50;
		}

		/* Have g_pTrialSaccades->setGridPercents(...) perform acceptance 
		 * logic on values.  Reset display to the values actually accepted. */
		g_pTrialSaccades->setGridPercents(pctRedGrids, pctBlueGrids);
		pctRedGrids = g_pTrialSaccades->getPctRedGrids();
		pctBlueGrids = g_pTrialSaccades->getPctBlueGrids();
		m_PctRedTrials.SetWindowTextA(etu::IntToString(pctRedGrids));
		m_PctBlueTrials.SetWindowTextA(etu::IntToString(pctBlueGrids));
	}

	if (m_GridRadioCartesian.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setGridType(CTrialSaccades::GRID_CARTESIAN);
		m_NumStepsH.GetWindowText(buffer, 100);
		g_pTrialSaccades->setNumStepsH(etu::StringToInt(buffer, g_pTrialSaccades->getNumStepsH()));
		m_NumStepsV.GetWindowText(buffer, 100);
		g_pTrialSaccades->setNumStepsV(etu::StringToInt(buffer, g_pTrialSaccades->getNumStepsV()));
		m_StepSize.GetWindowText(buffer, 100);
		g_pTrialSaccades->setStepSize(etu::StringToFloat(buffer, g_pTrialSaccades->getStepSize()));
		g_pTrialSaccades->setAvoidZero(m_AvoidZero.GetCheck() == BST_CHECKED);
	}
	else if (m_GridRadioRadial.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setGridType(CTrialSaccades::GRID_RADIAL);

		m_AngleOffset.GetWindowText(buffer, 100);
		g_pTrialSaccades->setAngleOffset(etu::StringToInt(buffer, g_pTrialSaccades->getAngleOffset()));

		g_pTrialSaccades->setAngle0(m_Angle0.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setAngle45(m_Angle45.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setAngle90(m_Angle90.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setAngle135(m_Angle135.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setAngle180(m_Angle180.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setAngle225(m_Angle225.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setAngle270(m_Angle270.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setAngle315(m_Angle315.GetCheck() == BST_CHECKED);

		m_RadiusOffset.GetWindowText(buffer, 100);
		g_pTrialSaccades->setRadiusOffset(etu::StringToInt(buffer, g_pTrialSaccades->getRadiusOffset()));

		g_pTrialSaccades->setRadiusOrigin(m_RadiusOrigin.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setRadius0(m_Radius0.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setRadius5(m_Radius5.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setRadius10(m_Radius10.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setRadius15(m_Radius15.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setRadius20(m_Radius20.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setRadius25(m_Radius25.GetCheck() == BST_CHECKED);
		g_pTrialSaccades->setRadius30(m_Radius30.GetCheck() == BST_CHECKED);
	}

	g_pTrialSaccades->setUseNoise(m_UseNoise.GetCheck() == BST_CHECKED);
	g_pTrialSaccades->setInterleaveNoise(m_InterleaveNoise.GetCheck() == BST_CHECKED);

	m_CenterXPos.GetWindowText(buffer, 100);
	g_pTrialSaccades->setCenterXPos(etu::StringToFloat(buffer, g_pTrialSaccades->getCenterXPos()));
	m_CenterYPos.GetWindowText(buffer, 100);
	g_pTrialSaccades->setCenterYPos(etu::StringToFloat(buffer, g_pTrialSaccades->getCenterYPos()));
	m_CenterDotSize.GetWindowText(buffer, 100);
	g_pTrialSaccades->setCenterDotSize(etu::StringToFloat(buffer, g_pTrialSaccades->getCenterDotSize()));
	m_PeriphDotSize.GetWindowText(buffer, 100);
	g_pTrialSaccades->setPeriphDotSize(etu::StringToFloat(buffer, g_pTrialSaccades->getPeriphDotSize()));

	m_RewardDelay.GetWindowText(buffer, 100);
	g_pTrialSaccades->setRewardDelay(etu::StringToInt(buffer, g_pTrialSaccades->getRewardDelay()));
	g_pTrialSaccades->setUseShapes(m_UseShapes.GetCheck() == BST_CHECKED);
	g_pTrialSaccades->setSameBG(m_SameBG.GetCheck() == BST_CHECKED);
	
	m_cStepSizeDeg.GetWindowText(buffer, 100);
	g_pTrialSaccades->setCtrStepSize(etu::StringToFloat(buffer, g_pTrialSaccades->getCtrStepSize()));
		
	m_cNumStepsX.GetWindowText(buffer, 100);
	g_pTrialSaccades->setNumStepsX(etu::StringToInt(buffer, g_pTrialSaccades->getNumStepsX()));
		
	m_cNumStepsY.GetWindowText(buffer, 100);
	g_pTrialSaccades->setNumStepsY(etu::StringToInt(buffer, g_pTrialSaccades->getNumStepsY()));
	
	g_pTrialSaccades->setOccludeBG(m_OccludeBG.GetCheck() == BST_CHECKED);

	g_pTrialSaccades->setDotGratings(m_DotGratings.GetCheck() == BST_CHECKED);
	m_DotGratingContrast.GetWindowText(buffer, 100);
	g_pTrialSaccades->setDotGratingContrast(etu::StringToInt(buffer, g_pTrialSaccades->getDotGratingContrast()));
	m_DotGratingCycles.GetWindowText(buffer, 100);
	g_pTrialSaccades->setDotGratingCycles(etu::StringToFloat(buffer, g_pTrialSaccades->getDotGratingCycles()));

	g_pTrialSaccades->sendSettings(g_pSettings->theSerialComm);
	
	/* Stimulus regenerates trials each time it's run */
	ClearTrialCounts();
}

void CTrialSaccadesDlg::OnPaint() 
{
	CDialog::OnPaint();
	PaintColorStatics();
	PaintTrialSequenceStatics();
}

void CTrialSaccadesDlg::PaintColorStatics()
{
	CRect rect;
	DWORD primaryColor, secondaryColor, tertiaryColor;
	DWORD noiseColor;

	primaryColor = RGB(g_pTrialSaccades->getPrimaryRed(), 
		g_pTrialSaccades->getPrimaryGreen(), g_pTrialSaccades->getPrimaryBlue());
	secondaryColor = RGB(g_pTrialSaccades->getSecondaryRed(),
		g_pTrialSaccades->getSecondaryGreen(), g_pTrialSaccades->getSecondaryBlue());
	tertiaryColor = RGB(g_pTrialSaccades->getTertiaryRed(),
		g_pTrialSaccades->getTertiaryGreen(), g_pTrialSaccades->getTertiaryBlue());
	noiseColor = RGB(g_pTrialSaccades->getNoiseRed(),
		g_pTrialSaccades->getNoiseGreen(),g_pTrialSaccades->getNoiseBlue());

	CPaintDC dc_p(&m_PrimaryStatic);
	m_PrimaryStatic.GetClientRect(rect);
	dc_p.FillSolidRect(rect, primaryColor);

	CPaintDC dc_s(&m_SecondaryStatic);
	m_SecondaryStatic.GetClientRect(rect);
	dc_s.FillSolidRect(rect, secondaryColor);

	CPaintDC dc_t(&m_TertiaryStatic);
	m_TertiaryStatic.GetClientRect(rect);
	dc_t.FillSolidRect(rect, tertiaryColor);

	CPaintDC dc_n(&m_NoiseStatic);
	m_NoiseStatic.GetClientRect(rect);
	dc_n.FillSolidRect(rect, noiseColor);

	CPaintDC dc_trbp(&m_TrialResultsBorderPrimary);
	m_TrialResultsBorderPrimary.GetClientRect(rect);
	dc_trbp.FillSolidRect(rect, primaryColor);
	
	CPaintDC dc_trbs(&m_TrialResultsBorderSecondary);
	m_TrialResultsBorderSecondary.GetClientRect(rect);
	dc_trbs.FillSolidRect(rect, secondaryColor);
	
	CPaintDC dc_trbt(&m_TrialResultsBorderTertiary);
	m_TrialResultsBorderTertiary.GetClientRect(rect);
	dc_trbt.FillSolidRect(rect, tertiaryColor);
}

static const int s_tsMinorTics = 100;  /* ms */
static const int s_tsMajorTics = 500; /* ms */
static const CString s_tsMinorTicStr(".");
static const CString s_tsMajorTicStr("|");
static const DWORD s_crossStartColor = RGB(128, 128, 255); /* Dark purple  */
static const DWORD s_crossFlashColor = RGB(159, 159, 223); /* Light purple  */
static const DWORD s_crossDelayColor = RGB(128, 128, 255); /* Dark purple  */
static const DWORD s_dotCueColor = RGB(159, 159, 223); /* Light purple  */
static const DWORD s_dotLingerColor = RGB(128, 128, 255); /* Dark purple  */

static const DWORD s_displayedColor = RGB(159, 159, 223); 
static const DWORD s_hatchColor = RGB(0, 255, 128);
static const DWORD s_solidWhite = RGB(255, 255, 255);
static const DWORD s_solidGray = RGB(214, 211, 206);

/** Fill cross and dot timeline rectangles proportionate to the times. */
void CTrialSaccadesDlg::PaintTrialSequenceStatics()
{
	CRect crossRect, dotRect, tsRect;
	CRect keyDisplayedRect, keyBaitedRect;
	CPoint crossTopLeft, dotTopLeft;
	CBrush hatchBrush;
	int totalTime, initTime, flashTime, delayTime, baitedTime;
	int totalLength, initLength, flashLength, delayLength, baitedLength;
	double scaleFactor;

	/* Get painting contexts; needed later. */
	CPaintDC crossPaint(&m_TrialSequenceImgCross);
	CPaintDC dotPaint(&m_TrialSequenceImgDot);
	CPaintDC tsPaint(&m_TrialSequenceImgTimeScale);
	CPaintDC keyDisplayedPaint(&m_KeyDisplayedStatic);
	CPaintDC keyBaitedPaint(&m_KeyBaitedStatic);

	/* Get handles to rectangles and blank out previous illustration. */
	m_TrialSequenceImgCross.GetClientRect(crossRect);
	m_TrialSequenceImgDot.GetClientRect(dotRect);
	m_TrialSequenceImgTimeScale.GetClientRect(tsRect);
	m_KeyDisplayedStatic.GetClientRect(keyDisplayedRect);
	m_KeyBaitedStatic.GetClientRect(keyBaitedRect);

	crossPaint.FillSolidRect(crossRect, s_solidWhite);
	dotPaint.FillSolidRect(dotRect, s_solidWhite);
	tsPaint.FillSolidRect(tsRect, s_solidGray);
	
	hatchBrush.CreateHatchBrush(HS_DIAGCROSS, s_hatchColor);

	keyDisplayedPaint.FillSolidRect(keyDisplayedRect, s_displayedColor);
	keyBaitedPaint.FillSolidRect(keyBaitedRect, s_solidWhite);
	keyBaitedPaint.FillRect(keyBaitedRect, &hatchBrush);

	initTime = GetAverageTime(m_InitTimeMin, m_InitTimeMax);
	flashTime = GetAverageTime(m_FlashTimeMin, m_FlashTimeMax);
	delayTime = GetAverageTime(m_DelayTimeMin, m_DelayTimeMax);
	baitedTime = GetAverageTime(m_BaitedTime, m_BaitedTime);
	totalTime = initTime + delayTime + baitedTime;

	totalLength = crossRect.Width();

	scaleFactor = (double)totalLength / totalTime;
	initLength = (int)(scaleFactor * initTime);
	flashLength = (int)(scaleFactor * flashTime);
	delayLength = (int)(scaleFactor * delayTime);
	baitedLength = (int)(scaleFactor * baitedTime);

	/* Paint cross bar from top left */
	crossRect.right = crossRect.left + initLength + delayLength;
	crossPaint.FillSolidRect(crossRect, s_displayedColor);
	crossPaint.FillRect(crossRect, &hatchBrush);
	crossRect.right = crossRect.left + initLength;
	crossPaint.DrawTextA("a", -1, crossRect, DT_CENTER);
	if (delayLength > 0) {
		crossRect.left = crossRect.right;
		crossRect.right = crossRect.left + 1;
		crossPaint.FillSolidRect(crossRect, s_solidGray);
		crossRect.left = crossRect.right;
		crossRect.right = crossRect.left + delayLength - 1;
		crossPaint.SetBkColor(s_displayedColor);
		crossPaint.DrawTextA("c", -1, crossRect, DT_CENTER);
	}

	/* Paint dot bar in sections, always from top left */
	dotRect.left += initLength;
	dotRect.right = dotRect.left + flashLength;
	dotPaint.FillSolidRect(dotRect, s_displayedColor);
	dotPaint.DrawTextA("b", -1, dotRect, DT_CENTER);
	if (delayLength < flashLength) {
		/* Ensure a dot rect w/positive length */
		dotRect.left += delayLength;
		dotPaint.FillRect(dotRect, &hatchBrush);
	}

	dotRect.left = dotRect.right + ETU_MAX(0, delayLength - flashLength);
	dotRect.right = (dotRect.left + baitedLength 
		- ETU_MAX(0, flashLength - delayLength));
	dotPaint.SetBkColor(s_solidWhite); // Reset it from s_displayedColor;
	dotPaint.FillRect(dotRect, &hatchBrush);
	dotRect.left -= ETU_MAX(0, flashLength - delayLength);
	dotPaint.DrawTextA("d", -1, dotRect, DT_CENTER);

	/* TODO: Just make two drawing calls to each time scale. */
	for (int currMs = s_tsMinorTics; currMs < totalTime; currMs += s_tsMinorTics) {
		int hOffset = (int)(currMs * scaleFactor);

		tsRect.SetRect(hOffset, (int)tsRect.top, (int)tsRect.right, 
			(int)tsRect.bottom);

		tsPaint.DrawTextA(
			((currMs % s_tsMajorTics) == 0) ? s_tsMajorTicStr : s_tsMinorTicStr, 
			tsRect, 0);
	}
}

int CTrialSaccadesDlg::GetAverageTime(const CEdit& min, const CEdit& max)
{
	int minTime, maxTime;
	char buffer[100];

	min.GetWindowText(buffer, 100);
	minTime = etu::StringToInt(buffer, 0);

	max.GetWindowText(buffer, 100);
	maxTime = etu::StringToInt(buffer, 0);

	if (minTime < 0) minTime = 0;
	if (maxTime < 0) maxTime = 0;
	if (minTime == maxTime) return minTime;

	return etu::Round((minTime + maxTime) / 2.0);
}

void CTrialSaccadesDlg::ClearTrialCounts()
{
	for (int ix = 0; ix < m_trialTrackers.size(); ++ix) {
		m_trialTrackers[ix].Reset();
	}
	m_totalsTracker.Reset();
	RefreshCountsDisplay();
}

/**
 * @brief Increment the trial counts for given trial.
 *
 * Increments the appropriate counts for a trial of the given type, based on
 * whether the trial was performed correctly or incorrectly.
 *
 * @param code Code for trial being counted.
 * @param correct True if trial was performed correctly; false otherwise.
 */
void CTrialSaccadesDlg::IncrementCount(CSaccadeTrial::STIM_CODE code, bool correct)
{
	m_totalsTracker.Incr(correct);
	m_trialTrackers.at(code).Incr(correct);

	RefreshCountsDisplay();
}

/**
 * @brief Get a string object formatted to display count and percent.
 *
 * Returns a string object to display a number and the percent that number 
 * constitutes of a whole.  The separator between the number and percent is
 * configurable.
 *
 * @param num The number portion to include in the string.
 * @param whole The divisor for the percentage calculation.
 * @param sep Configurable separator character, likely a space or newline.
 * @return String of the format "0x(0%)", where 'x' represents the 
 *     configurable separator.
 */
CString CTrialSaccadesDlg::PercentString(int num, int whole, char sep)
{
	int pct = (whole ? ((int)((((double)(num)) / ((double)(whole))) * 100.0)) : 0);
	CString out("");
	CString number = etu::IntToString(num);
	CString percent = etu::IntToString(pct);

	out.Format("%s%c(%s%%)", (LPCTSTR)number, sep, (LPCTSTR)percent);

	return out;
}

/**
 * @brief Refreshes the display of counts pertaining to a single trial type.
 * 
 * @param boxRight Handle to CStatic display box for the count of right 
 *     answers.
 * @param boxWrong Handle to CStatic display box for the count of wrong 
 *     answers.
 * @param boxTotal Handle to CStatic display box for the total count.
 * @param tracker The totals to display.
 */
void CTrialSaccadesDlg::RefreshCountDisplay(CStatic& boxRight, CStatic& boxWrong, 
											CStatic& boxTotal, 
											CTrialSaccadesDlg::TrialTracker& tracker)
{
	boxRight.SetWindowText(etu::IntToString(tracker.Right()));
	boxWrong.SetWindowText(etu::IntToString(tracker.Wrong()));
	boxTotal.SetWindowText(PercentString(tracker.Total(), 
		m_totalsTracker.Total(), '\n'));
}

/**
 * @brief Refreshes the display of all trial counts.
 */
void CTrialSaccadesDlg::RefreshCountsDisplay()
{
	RefreshCountDisplay(m_TrialResults_RR_R, m_TrialResults_RR_W, 
		m_TrialResults_RR_T, m_trialTrackers[CSaccadeTrial::CODE_C_RED_P_RED]);
	RefreshCountDisplay(m_TrialResults_RB_R, m_TrialResults_RB_W, 
		m_TrialResults_RB_T, m_trialTrackers[CSaccadeTrial::CODE_C_RED_P_BLUE]);
	RefreshCountDisplay(m_TrialResults_RP_R, m_TrialResults_RP_W, 
		m_TrialResults_RP_T, m_trialTrackers[CSaccadeTrial::CODE_C_RED_P_PURPLE]);
	RefreshCountDisplay(m_TrialResults_BR_R, m_TrialResults_BR_W, 
		m_TrialResults_BR_T, m_trialTrackers[CSaccadeTrial::CODE_C_BLUE_P_RED]);
	RefreshCountDisplay(m_TrialResults_BB_R, m_TrialResults_BB_W, 
		m_TrialResults_BB_T, m_trialTrackers[CSaccadeTrial::CODE_C_BLUE_P_BLUE]);
	RefreshCountDisplay(m_TrialResults_BP_R, m_TrialResults_BP_W, 
		m_TrialResults_BP_T, m_trialTrackers[CSaccadeTrial::CODE_C_BLUE_P_PURPLE]);
	RefreshCountDisplay(m_TrialResults_PR_R, m_TrialResults_PR_W, 
		m_TrialResults_PR_T, m_trialTrackers[CSaccadeTrial::CODE_C_PURPLE_P_RED]);
	RefreshCountDisplay(m_TrialResults_PB_R, m_TrialResults_PB_W, 
		m_TrialResults_PB_T, m_trialTrackers[CSaccadeTrial::CODE_C_PURPLE_P_BLUE]);
	RefreshCountDisplay(m_TrialResults_PP_R, m_TrialResults_PP_W, 
		m_TrialResults_PP_T, m_trialTrackers[CSaccadeTrial::CODE_C_PURPLE_P_PURPLE]);

	m_TrialResults_R_T.SetWindowText(PercentString(m_totalsTracker.Right(), 
		m_totalsTracker.Total(), ' '));
	m_TrialResults_W_T.SetWindowText(PercentString(m_totalsTracker.Wrong(), 
		m_totalsTracker.Total(), ' '));
	m_TrialResults_Total.SetWindowText(etu::IntToString(m_totalsTracker.Total()));
}

/**
 * @brief Updates dialog's display of eye positions.
 * @param lxDeg Left eye X position in degrees.
 * @param lyDeg Left eye Y position in degrees.
 * @param rxDeg Right eye X position in degrees.
 * @param ryDeg Right eye Y position in degrees.
 */
void CTrialSaccadesDlg::UpdateEyePositions(double lxDeg, double lyDeg, 
										   double rxDeg, double ryDeg)
{
	char buffer[16];
	
	sprintf(buffer, "% 4.1f", lxDeg);
	m_EyePosnDisplayLX.SetWindowText(buffer);
	
	sprintf(buffer, "% 4.1f", lyDeg);
	m_EyePosnDisplayLY.SetWindowText(buffer);
	
	sprintf(buffer, "% 4.1f", rxDeg);
	m_EyePosnDisplayRX.SetWindowText(buffer);
	
	sprintf(buffer, "% 4.1f", ryDeg);
	m_EyePosnDisplayRY.SetWindowText(buffer);
}

void CTrialSaccadesDlg::OnBnClickedApply()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "Trial-based Saccade Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
	g_pTrialSaccadesDlg->Invalidate(FALSE);
}

void CTrialSaccadesDlg::OnBnClickedOk()
{	
	OnBnClickedApply();
}

void CTrialSaccadesDlg::OnBnClickedCancel()
{}

/**
 * @param spatiotopic
 *            -- true  = Spatiotopic coverage
 *            -- false = Retinotopic coverage
 */
void CTrialSaccadesDlg::EnableControlsForEyeCoverageSpatiotopic(bool isSpatiotopic)
{
	if (isSpatiotopic) {
		m_PctRedCtr.SetWindowText(
			etu::IntToString(g_pTrialSaccades->getPctRedCtr()));
		m_PctBlueCtr.SetWindowText(
			etu::IntToString(g_pTrialSaccades->getPctBlueCtr()));
		m_PctRedTrials.SetWindowText(
			etu::IntToString(g_pTrialSaccades->getPctRedGrids()));
		m_PctBlueTrials.SetWindowText(
			etu::IntToString(g_pTrialSaccades->getPctBlueGrids()));
		OnBnClickedGridSettingsRadioRandom();

		m_PctRedCtr.EnableWindow(true);
		m_PctBlueCtr.EnableWindow(true);
		m_PctRedTrials.EnableWindow(true);
		m_PctBlueTrials.EnableWindow(true);
	}
	else {
		m_PctRedCtr.SetWindowText("50");
		m_PctBlueCtr.SetWindowText("50");
		m_PctRedTrials.SetWindowText("50");
		m_PctBlueTrials.SetWindowText("50");
		OnBnClickedCenterRadioRandomize();
		OnBnClickedPeriphRadioRandomize();
		m_PctRedCtr.EnableWindow(false);
		m_PctBlueCtr.EnableWindow(false);
		m_PctRedTrials.EnableWindow(false);
		m_PctBlueTrials.EnableWindow(false);
	}

	m_GridSettingsRadioCyclopean.EnableWindow(!isSpatiotopic);
	m_CenterRadioPrimary.EnableWindow(isSpatiotopic);
	m_CenterRadioSecondary.EnableWindow(isSpatiotopic);
	m_CenterRadioTertiary.EnableWindow(isSpatiotopic);
	m_CenterRadioAlternate.EnableWindow(isSpatiotopic);
	m_CenterRadioRandomize.EnableWindow(isSpatiotopic);
	m_PeriphRadioPrimary.EnableWindow(isSpatiotopic);
	m_PeriphRadioSecondary.EnableWindow(isSpatiotopic);
	m_PeriphRadioTertiary.EnableWindow(isSpatiotopic);
	m_PeriphRadioAlternate.EnableWindow(isSpatiotopic);
	m_PeriphRadioRandomize.EnableWindow(isSpatiotopic);
}

void CTrialSaccadesDlg::OnBnClickedEyeCoverageRadioSpatiotopic()
{
	m_EyeCoverageRadioSpatiotopic.SetCheck(BST_CHECKED);
	m_EyeCoverageRadioRetinotopic.SetCheck(BST_UNCHECKED);
	m_UseApprox.EnableWindow(false);
	m_EyeCoverageDescription.SetWindowText("Description:\nOriginal behavior; "\
		"display peripheral target at random points on peripheral target "\
		"grid with no dependence on fixating eye or fixation location.");

	EnableControlsForEyeCoverageSpatiotopic(true);	
}

void CTrialSaccadesDlg::OnBnClickedEyeCoverageRadioRetinotopic()
{
	m_EyeCoverageRadioSpatiotopic.SetCheck(BST_UNCHECKED);
	m_EyeCoverageRadioRetinotopic.SetCheck(BST_CHECKED);
	m_UseApprox.EnableWindow(true);
	m_EyeCoverageDescription.SetWindowText("Description:\nDisplay targets on "\
		"separate peripheral target grids relative to location of eye being "
		"stimulated.");

	EnableControlsForEyeCoverageSpatiotopic(false);	
}

void CTrialSaccadesDlg::OnBnClickedUseApprox()
{
	HandleDeviationAngleBox();
}

void CTrialSaccadesDlg::OnBnClickedGridSettingsRadioRandom()
{
	m_GridSettingsRadioRandom.SetCheck(BST_CHECKED);
	m_GridSettingsRadioCyclopean.SetCheck(BST_UNCHECKED);
	HandleDeviationAngleBox();
	m_GridSettingsDescription.SetWindowText("Description:\nAlign fixating "\
		"eye randomly according to center target grid.");
}

void CTrialSaccadesDlg::OnBnClickedGridSettingsRadioCyclopean()
{
	m_GridSettingsRadioRandom.SetCheck(BST_UNCHECKED);
	m_GridSettingsRadioCyclopean.SetCheck(BST_CHECKED);
	HandleDeviationAngleBox();
	m_GridSettingsDescription.SetWindowText("Description:\nAlign cyclopean "\
		"eye randomly according to center target grid; adjust actual fixation "\
		"target to left for left eye targets, right for right eye targets, "\
		"using approximate deviation angle.");
}

void CTrialSaccadesDlg::HandleDeviationAngleBox()
{
	bool checked = ((m_UseApprox.GetCheck() == BST_CHECKED) ||
		(m_GridSettingsRadioCyclopean.GetCheck() == BST_CHECKED));
	m_DeviationAngle.EnableWindow(checked);
}

void CTrialSaccadesDlg::OnBnClickedCenterRadioPrimary()
{
	m_CenterRadioPrimary.SetCheck(BST_CHECKED);
	m_CenterRadioSecondary.SetCheck(BST_UNCHECKED);
	m_CenterRadioTertiary.SetCheck(BST_UNCHECKED);
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
	m_AlternateBlockSize.EnableWindow(false);
	m_AlternateWithPurple.EnableWindow(false);
	m_PctRedCtr.EnableWindow(false);
	m_PctBlueCtr.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedCenterRadioSecondary()
{
	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_CHECKED);
	m_CenterRadioTertiary.SetCheck(BST_UNCHECKED);
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
	m_AlternateBlockSize.EnableWindow(false);
	m_AlternateWithPurple.EnableWindow(false);
	m_PctRedCtr.EnableWindow(false);
	m_PctBlueCtr.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedCenterRadioTertiary()
{
	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_UNCHECKED);
	m_CenterRadioTertiary.SetCheck(BST_CHECKED);
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
	m_AlternateBlockSize.EnableWindow(false);
	m_AlternateWithPurple.EnableWindow(false);
	m_PctRedCtr.EnableWindow(false);
	m_PctBlueCtr.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedCenterRadioAlternate()
{
	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_UNCHECKED);
	m_CenterRadioTertiary.SetCheck(BST_UNCHECKED);
	m_CenterRadioAlternate.SetCheck(BST_CHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
	m_AlternateBlockSize.EnableWindow(true);
	m_AlternateWithPurple.EnableWindow(true);
	m_PctRedCtr.EnableWindow(false);
	m_PctBlueCtr.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedCenterRadioRandomize()
{
	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_UNCHECKED);
	m_CenterRadioTertiary.SetCheck(BST_UNCHECKED);
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_CHECKED);
	m_AlternateBlockSize.EnableWindow(false);
	m_AlternateWithPurple.EnableWindow(false);
	m_PctRedCtr.EnableWindow(true);
	m_PctBlueCtr.EnableWindow(true);
}

void CTrialSaccadesDlg::OnBnClickedPeriphRadioPrimary()
{
	m_PeriphRadioPrimary.SetCheck(BST_CHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_PctRedTrials.EnableWindow(false);
	m_PctBlueTrials.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedPeriphRadioSecondary()
{
	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_CHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_PctRedTrials.EnableWindow(false);
	m_PctBlueTrials.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedPeriphRadioTertiary()
{
	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_CHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_PctRedTrials.EnableWindow(false);
	m_PctBlueTrials.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedPeriphRadioAlternate()
{
	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_CHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_PctRedTrials.EnableWindow(false);
	m_PctBlueTrials.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedPeriphRadioRandomize()
{
	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_CHECKED);
	m_PctRedTrials.EnableWindow(true);
	m_PctBlueTrials.EnableWindow(true);
}

void CTrialSaccadesDlg::OnBnClickedGridRadioCartesian()
{
	EnableGridCartesianControls(true);
	EnableGridRadialControls(false);
}

void CTrialSaccadesDlg::OnBnClickedGridRadioRadial()
{
	EnableGridCartesianControls(false);
	EnableGridRadialControls(true);
}

void CTrialSaccadesDlg::EnableGridCartesianControls(bool enable)
{
	m_GridRadioCartesian.SetCheck(enable ? BST_CHECKED : BST_UNCHECKED);
	m_NumStepsH.EnableWindow(enable);
	m_NumStepsV.EnableWindow(enable);
	m_StepSize.EnableWindow(enable);
	m_AvoidZero.EnableWindow(enable);
}

void CTrialSaccadesDlg::EnableGridRadialControls(bool enable)
{
	m_GridRadioRadial.SetCheck(enable ? BST_CHECKED : BST_UNCHECKED);
	m_AngleOffset.EnableWindow(enable);
	m_Angle0.EnableWindow(enable);
	m_Angle45.EnableWindow(enable);
	m_Angle90.EnableWindow(enable);
	m_Angle135.EnableWindow(enable);
	m_Angle180.EnableWindow(enable);
	m_Angle225.EnableWindow(enable);
	m_Angle270.EnableWindow(enable);
	m_Angle315.EnableWindow(enable);
	m_RadiusOffset.EnableWindow(enable);
	m_RadiusOrigin.EnableWindow(enable);
	m_Radius0.EnableWindow(enable);
	m_Radius5.EnableWindow(enable);
	m_Radius10.EnableWindow(enable);
	m_Radius15.EnableWindow(enable);
	m_Radius20.EnableWindow(enable);
	m_Radius25.EnableWindow(enable);
	m_Radius30.EnableWindow(enable);
}

void CTrialSaccadesDlg::OnBnClickedUseNoise()
{
	m_InterleaveNoise.EnableWindow(m_UseNoise.GetCheck() == BST_CHECKED);
	m_SameBG.EnableWindow(m_UseNoise.GetCheck() == BST_CHECKED);
	m_OccludeBG.EnableWindow(m_UseNoise.GetCheck() == BST_CHECKED);
	m_NoiseSize.EnableWindow(m_UseNoise.GetCheck() == BST_CHECKED);
	m_NoisePct.EnableWindow(m_UseNoise.GetCheck() == BST_CHECKED);
	m_NoiseRed.EnableWindow(m_UseNoise.GetCheck() == BST_CHECKED);
	m_NoiseGreen.EnableWindow(m_UseNoise.GetCheck() == BST_CHECKED);
	m_NoiseBlue.EnableWindow(m_UseNoise.GetCheck() == BST_CHECKED);
}

void CTrialSaccadesDlg::AddInfoText(LPCTSTR text)
{
	CString current;
	m_InfoDisplay.GetWindowText(current);
	current.AppendFormat("%s", text);
	m_InfoDisplay.SetWindowText((LPCTSTR)current);
}

void CTrialSaccadesDlg::ClearInfoText()
{
	m_InfoDisplay.SetWindowText("");
}

void CTrialSaccadesDlg::SetInfoText(LPCTSTR text)
{
	m_InfoDisplay.SetWindowText(text);
}

/****************************************************************************/

/** Zero-initialize the tracker. */
CTrialSaccadesDlg::TrialTracker::TrialTracker() 
: m_right(0), m_wrong(0), m_total(0)
{ /* empty */ }

/** Nothing to do. */
CTrialSaccadesDlg::TrialTracker::~TrialTracker() { /* empty */ }

/** Increment the total count and successful or failed trial count. */
void CTrialSaccadesDlg::TrialTracker::Incr(bool right) 
{
	++m_total;
	if (right) ++m_right; else ++m_wrong;
}

/** Increment the total count and successful trial count. */
void CTrialSaccadesDlg::TrialTracker::IncrRight() { Incr(true); }

/** Increment the total count and failed trial count. */
void CTrialSaccadesDlg::TrialTracker::IncrWrong() { Incr(false); }

/** Get the count of successful trials. */
int CTrialSaccadesDlg::TrialTracker::Right() { return m_right; }

/** Get the count of failed trials. */
int CTrialSaccadesDlg::TrialTracker::Wrong() { return m_wrong; }

/** Get the total count of trials. */
int CTrialSaccadesDlg::TrialTracker::Total() { return m_total; }

/** Set all counts to 0. */
void CTrialSaccadesDlg::TrialTracker::Reset()
{
	m_right = 0;
	m_wrong = 0;
	m_total = 0;
}
/****************************************************************************/

/** Construct a state tracker for given paint obj. */
CTrialSaccadesDlg::SequenceGraphicState::SequenceGraphicState(CPaintDC& paintDC)
: m_color(paintDC.GetBkColor())
{}

/** Construct a state tracker and copy state from rhs. */
CTrialSaccadesDlg::SequenceGraphicState::SequenceGraphicState(
	const SequenceGraphicState& rhs)
: m_color(rhs.m_color)
{}

/** Destroy the tracker */
CTrialSaccadesDlg::SequenceGraphicState::~SequenceGraphicState() { /* empty */ }

/** Get the saved bk color state. */
DWORD CTrialSaccadesDlg::SequenceGraphicState::GetColor() const
{ return m_color; }

#endif
