/**
 * @file TrialSaccadesDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

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
: CDialog(CTrialSaccadesDlg::IDD, pParent), m_cRed_pRed_Right(0), 
  m_cRed_pRed_Wrong(0), m_cRed_pRed_Total(0), m_cRed_pBlue_Right(0), 
  m_cRed_pBlue_Wrong(0), m_cRed_pBlue_Total(0), m_cBlue_pRed_Right(0), 
  m_cBlue_pRed_Wrong(0), m_cBlue_pRed_Total(0), m_cBlue_pBlue_Right(0), 
  m_cBlue_pBlue_Wrong(0), m_cBlue_pBlue_Total(0), m_totalRight(0), 
  m_totalWrong(0), m_totalTotal(0)
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
	DDX_Control(pDX, IDC_tbsDebugResponderOn, m_debugResponderOn);
	DDX_Control(pDX, IDC_tbsDebugLabelsOn, m_debugLabelsOn);
	DDX_Control(pDX, IDC_tbsFixationFlashDelay, m_FixationFlashDelay);
	DDX_Control(pDX, IDC_tbsTargetActivePeriod, m_TargetActivePeriod);
	DDX_Control(pDX, IDC_tbsMisbehaviorDelay, m_MisbehaviorDelay);

	DDX_Control(pDX, IDC_tbsEyeCoverageRadioRandom, m_EyeCoverageRadioSpatiotopic);
	DDX_Control(pDX, IDC_tbsEyeCoverageRadioReciprocal, m_EyeCoverageRadioRetinotopic);
	DDX_Control(pDX, IDC_tbsUseApprox, m_UseApprox);
	DDX_Control(pDX, IDC_tbsEyeCoverageDescription, m_EyeCoverageDescription);

	DDX_Control(pDX, IDC_tbsInfoDisplay, m_InfoDisplay);
	DDX_Control(pDX, IDC_tbsEyePosnDisplayLX, m_EyePosnDisplayLX);
	DDX_Control(pDX, IDC_tbsEyePosnDisplayLY, m_EyePosnDisplayLY);
	DDX_Control(pDX, IDC_tbsEyePosnDisplayRX, m_EyePosnDisplayRX);
	DDX_Control(pDX, IDC_tbsEyePosnDisplayRY, m_EyePosnDisplayRY);

	DDX_Control(pDX, IDC_tbsSaccadeHoldPeriod, m_SaccadeHoldPeriod);
	DDX_Control(pDX, IDC_tbsWrongDelay, m_WrongDelay);
	DDX_Control(pDX, IDC_tbsRewardDelay, m_RewardDelay);
	DDX_Control(pDX, IDC_tbsPauseDotOff, m_PauseDotOff);
	DDX_Control(pDX, IDC_tbsPauseDotOn, m_PauseDotOn);
	DDX_Control(pDX, IDC_tbsCenterDotSize, m_CenterDotSize);
	DDX_Control(pDX, IDC_tbsCrossPix, m_CrossPix);
	DDX_Control(pDX, IDC_tbsCenterXPos, m_CenterXPos);
	DDX_Control(pDX, IDC_tbsCenterYPos, m_CenterYPos);
	DDX_Control(pDX, IDC_tbscStepSizeDeg, m_cStepSizeDeg);
	DDX_Control(pDX, IDC_tbsNumStepsX, m_cNumStepsX);
	DDX_Control(pDX, IDC_tbsNumStepsY, m_cNumStepsY);
	DDX_Control(pDX, IDC_tbsShowCenterFlash, m_ShowCenterFlash);
	DDX_Control(pDX, IDC_tbsShowCenterHold, m_ShowCenterHold);
	DDX_Control(pDX, IDC_tbsDisplayTime, m_DisplayTime);
	DDX_Control(pDX, IDC_tbsRandomizeDisplayTime, m_RandomizeDisplayTime);
	DDX_Control(pDX, IDC_tbsDisplayRandMax, m_DisplayRandMax);
	DDX_Control(pDX, IDC_tbsDisplayRandMin, m_DisplayRandMin);

	DDX_Control(pDX, IDC_tbsAdvanceEitherEye, m_AdvanceEitherEye);

	DDX_Control(pDX, IDC_tbsCenterRadioPrimary,   m_CenterRadioPrimary);
	DDX_Control(pDX, IDC_tbsCenterRadioSecondary, m_CenterRadioSecondary);
	DDX_Control(pDX, IDC_tbsCenterRadioAlternate, m_CenterRadioAlternate);
	DDX_Control(pDX, IDC_tbsAlternateBlockSize,   m_AlternateBlockSize);
	DDX_Control(pDX, IDC_tbsCenterRadioRandomize, m_CenterRadioRandomize);

	DDX_Control(pDX, IDC_tbsPeriphDotSize, m_PeriphDotSize);
	DDX_Control(pDX, IDC_tbsFlashInterval, m_FlashInterval);
	DDX_Control(pDX, IDC_tbsRandomizeFlashInterval, m_RandomizeFlashInterval);
	DDX_Control(pDX, IDC_tbsFlashIntervalRandMax, m_FlashIntervalRandMax);
	DDX_Control(pDX, IDC_tbsFlashIntervalRandMin, m_FlashIntervalRandMin);
	DDX_Control(pDX, IDC_tbsDisplayDuringHoldPeriod, m_DisplayDuringHoldPeriod);
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
	DDX_Control(pDX, IDC_tbsPeriphRadioAlternate, m_PeriphRadioAlternate);
	DDX_Control(pDX, IDC_tbsNumRedTrials,         m_NumRedTrials);
	DDX_Control(pDX, IDC_tbsPeriphRadioRandomize, m_PeriphRadioRandomize);

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

	DDX_Control(pDX, IDC_tbsTrialResults_BR_R, m_TrialResults_BR_R);
	DDX_Control(pDX, IDC_tbsTrialResults_BR_W, m_TrialResults_BR_W);
	DDX_Control(pDX, IDC_tbsTrialResults_BR_T, m_TrialResults_BR_T);

	DDX_Control(pDX, IDC_tbsTrialResults_BB_R, m_TrialResults_BB_R);
	DDX_Control(pDX, IDC_tbsTrialResults_BB_W, m_TrialResults_BB_W);
	DDX_Control(pDX, IDC_tbsTrialResults_BB_T, m_TrialResults_BB_T);

	DDX_Control(pDX, IDC_tbsTrialResults_R_T, m_TrialResults_R_T);
	DDX_Control(pDX, IDC_tbsTrialResults_W_T, m_TrialResults_W_T);
	DDX_Control(pDX, IDC_tbsTrialResults_Total, m_TrialResults_Total);
}

BEGIN_MESSAGE_MAP(CTrialSaccadesDlg, CDialog)
	ON_BN_CLICKED(IDC_tbsButtonApply, OnBnClickedApply)
	ON_BN_CLICKED(IDC_tbsCenterRadioPrimary, OnBnClickedCenterRadioPrimary)
	ON_BN_CLICKED(IDC_tbsEyeCoverageRadioRandom, OnBnClickedEyeCoverageRadioSpatiotopic)
	ON_BN_CLICKED(IDC_tbsEyeCoverageRadioReciprocal, OnBnClickedEyeCoverageRadioRetinotopic)
	ON_BN_CLICKED(IDC_tbsUseApprox, OnBnClickedUseApprox)
	ON_BN_CLICKED(IDC_tbsCenterRadioSecondary, OnBnClickedCenterRadioSecondary)
	ON_BN_CLICKED(IDC_tbsCenterRadioAlternate, OnBnClickedCenterRadioAlternate)
	ON_BN_CLICKED(IDC_tbsCenterRadioRandomize, OnBnClickedCenterRadioRandomize)
	ON_BN_CLICKED(IDC_tbsPeriphRadioPrimary, OnBnClickedPeriphRadioPrimary)
	ON_BN_CLICKED(IDC_tbsPeriphRadioSecondary, OnBnClickedPeriphRadioSecondary)
	ON_BN_CLICKED(IDC_tbsPeriphRadioAlternate, OnBnClickedPeriphRadioAlternate)
	ON_BN_CLICKED(IDC_tbsPeriphRadioRandomize, OnBnClickedPeriphRadioRandomize)
	ON_BN_CLICKED(IDC_tbsGridSettingsRadioRandom, OnBnClickedGridSettingsRadioRandom)
	ON_BN_CLICKED(IDC_tbsGridSettingsRadioCyclopean, OnBnClickedGridSettingsRadioCyclopean)
	ON_BN_CLICKED(IDC_tbsGridRadioCartesian, OnBnClickedGridRadioCartesian)
	ON_BN_CLICKED(IDC_tbsGridRadioRadial, OnBnClickedGridRadioRadial)
	ON_BN_CLICKED(IDC_tbsRandomizeDisplayTime, OnBnClickedRandomizeDisplayTime)
	ON_BN_CLICKED(IDC_tbsRandomizeFlashInterval, OnBnClickedRandomizeFlashInterval)
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
	m_NoiseRed.SetWindowText(etu::IntToString(g_pTrialSaccades->getNoiseRed()));
	m_NoiseGreen.SetWindowText(etu::IntToString(g_pTrialSaccades->getNoiseGreen()));
	m_NoiseBlue.SetWindowText(etu::IntToString(g_pTrialSaccades->getNoiseBlue()));
	m_FixationFlashDelay.SetWindowText(etu::IntToString(g_pTrialSaccades->getFixationFlashDelay()));
	m_TargetActivePeriod.SetWindowText(etu::IntToString(g_pTrialSaccades->getTargetActivePeriod()));
	m_WrongDelay.SetWindowText(etu::IntToString(g_pTrialSaccades->getWrongDelay()));
	m_NoisePct.SetWindowText(etu::IntToString(g_pTrialSaccades->getNoisePct()));
	m_NoiseSize.SetWindowText(etu::IntToString(g_pTrialSaccades->getNoiseSize()));
	m_DisplayTime.SetWindowText(etu::IntToString(g_pTrialSaccades->getDisplayTime()));
	m_MisbehaviorDelay.SetWindowText(etu::IntToString(g_pTrialSaccades->getMisbehaviorDelay()));
	m_DisplayRandMax.SetWindowText(etu::IntToString(g_pTrialSaccades->getDisplayRandMax()));
	m_DisplayRandMin.SetWindowText(etu::IntToString(g_pTrialSaccades->getDisplayRandMin()));
	m_FlashInterval.SetWindowText(etu::IntToString(g_pTrialSaccades->getFlashInterval()));
	m_FlashIntervalRandMax.SetWindowText(etu::IntToString(g_pTrialSaccades->getFlashIntervalRandMax()));
	m_FlashIntervalRandMin.SetWindowText(etu::IntToString(g_pTrialSaccades->getFlashIntervalRandMin()));
	m_CrossPix.SetWindowText(etu::IntToString(g_pTrialSaccades->getCrossPix()));
	m_AlternateBlockSize.SetWindowText(etu::IntToString(g_pTrialSaccades->getAlternateBlockSize()));
	m_NumRedTrials.SetWindowText(etu::IntToString(g_pTrialSaccades->getNumRedGrids()));
	m_UseNoise.SetCheck((g_pTrialSaccades->getUseNoise())? BST_CHECKED : BST_UNCHECKED);
	m_InterleaveNoise.SetCheck((g_pTrialSaccades->getInterleaveNoise())? BST_CHECKED : BST_UNCHECKED);
	m_ShowCenterFlash.SetCheck((g_pTrialSaccades->getShowCenterFlash())? BST_CHECKED : BST_UNCHECKED);
	m_ShowCenterHold.SetCheck((g_pTrialSaccades->getShowCenterHold())? BST_CHECKED : BST_UNCHECKED);
	m_RandomizeFlashInterval.SetCheck((g_pTrialSaccades->getRandomizeFlashInterval())? BST_CHECKED : BST_UNCHECKED);
	m_RandomizeDisplayTime.SetCheck((g_pTrialSaccades->getRandomizeDisplayTime())? BST_CHECKED : BST_UNCHECKED);
	m_CenterXPos.SetWindowText(etu::FloatToString(g_pTrialSaccades->getCenterXPos()));
	m_CenterYPos.SetWindowText(etu::FloatToString(g_pTrialSaccades->getCenterYPos()));
	m_cStepSizeDeg.SetWindowText(etu::FloatToString(g_pTrialSaccades->getCtrStepSize()));
	m_cNumStepsX.SetWindowText(etu::IntToString(g_pTrialSaccades->getNumStepsX()));
	m_cNumStepsY.SetWindowText(etu::IntToString(g_pTrialSaccades->getNumStepsY()));
	m_CenterDotSize.SetWindowText(etu::FloatToString(g_pTrialSaccades->getCenterDotSize()));
	m_PeriphDotSize.SetWindowText(etu::FloatToString(g_pTrialSaccades->getPeriphDotSize()));

	m_AdvanceEitherEye.SetCheck(g_pTrialSaccades->getAdvanceEitherEye() ? BST_CHECKED : BST_UNCHECKED);

	m_debugResponderOn.SetCheck(g_pTrialSaccades->getDebugResponderOn() ? BST_CHECKED : BST_UNCHECKED);
	m_debugResponderOn.SetCheck(g_pTrialSaccades->getDebugLabelsOn() ? BST_CHECKED : BST_UNCHECKED);

	/* Color radios depend on eye coverage.  Do color first so we overwrite 
	 * later. */
	switch (g_pTrialSaccades->getCenterColorRadio()) {
		case CTrialSaccades::COLOR_PRIMARY:   OnBnClickedCenterRadioPrimary();   break;
		case CTrialSaccades::COLOR_SECONDARY: OnBnClickedCenterRadioSecondary(); break;
		case CTrialSaccades::COLOR_ALTERNATE: OnBnClickedCenterRadioAlternate(); break;
		default: /* COLOR_RANDOM */           OnBnClickedCenterRadioRandomize(); break;
	}
	switch (g_pTrialSaccades->getPeriphColorRadio()) {
		case CTrialSaccades::COLOR_PRIMARY:   OnBnClickedPeriphRadioPrimary();   break;
		case CTrialSaccades::COLOR_SECONDARY: OnBnClickedPeriphRadioSecondary(); break;
		case CTrialSaccades::COLOR_ALTERNATE: OnBnClickedPeriphRadioAlternate(); break;
		default: /* COLOR_RANDOM */           OnBnClickedPeriphRadioRandomize(); break;
	}
	m_SaccadeHoldPeriod.SetWindowText(etu::IntToString(g_pTrialSaccades->getSaccadeHoldPeriod()));

	/* Grid settings radios depend on eye coverage.  Do color first so we 
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

	CRect rect;
	
	CPaintDC dc_p(&m_PrimaryStatic);
	m_PrimaryStatic.GetClientRect(rect);
	dc_p.FillSolidRect(rect, RGB(g_pTrialSaccades->getPrimaryRed(),g_pTrialSaccades->getPrimaryGreen(),g_pTrialSaccades->getPrimaryBlue()));
	
	CPaintDC dc_s(&m_SecondaryStatic);
	m_SecondaryStatic.GetClientRect(rect);
	dc_s.FillSolidRect(rect, RGB(g_pTrialSaccades->getSecondaryRed(),g_pTrialSaccades->getSecondaryGreen(),g_pTrialSaccades->getSecondaryBlue()));
	
	CPaintDC dc_n(&m_NoiseStatic);
	m_NoiseStatic.GetClientRect(rect);
	dc_n.FillSolidRect(rect, RGB(g_pTrialSaccades->getNoiseRed(),g_pTrialSaccades->getNoiseGreen(),g_pTrialSaccades->getNoiseBlue()));

	m_RewardDelay.SetWindowText(etu::IntToString(g_pTrialSaccades->getRewardDelay()));
	m_DisplayDuringHoldPeriod.SetCheck((g_pTrialSaccades->getDisplayDuringHoldPeriod())? BST_CHECKED : BST_UNCHECKED);
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
	m_NoiseRed.GetWindowText(buffer, 100);
	g_pTrialSaccades->setNoiseRed(etu::StringToInt(buffer, g_pTrialSaccades->getNoiseRed()));
	m_NoiseGreen.GetWindowText(buffer, 100);
	g_pTrialSaccades->setNoiseGreen(etu::StringToInt(buffer, g_pTrialSaccades->getNoiseGreen()));
	m_NoiseBlue.GetWindowText(buffer, 100);
	g_pTrialSaccades->setNoiseBlue(etu::StringToInt(buffer, g_pTrialSaccades->getNoiseBlue()));
	m_FixationFlashDelay.GetWindowText(buffer, 100);
	g_pTrialSaccades->setFixationFlashDelay(etu::StringToInt(buffer, g_pTrialSaccades->getFixationFlashDelay()));
	m_TargetActivePeriod.GetWindowText(buffer, 100);
	g_pTrialSaccades->setTargetActivePeriod(etu::StringToInt(buffer, g_pTrialSaccades->getTargetActivePeriod()));
	m_WrongDelay.GetWindowText(buffer, 100);
	g_pTrialSaccades->setWrongDelay(etu::StringToInt(buffer, g_pTrialSaccades->getWrongDelay()));
	m_MisbehaviorDelay.GetWindowText(buffer, 100);
	g_pTrialSaccades->setMisbehaviorDelay(etu::StringToInt(buffer, g_pTrialSaccades->getMisbehaviorDelay()));
	m_NoisePct.GetWindowText(buffer, 100);
	g_pTrialSaccades->setNoisePct(etu::StringToInt(buffer, g_pTrialSaccades->getNoisePct()));
	m_NoiseSize.GetWindowText(buffer, 100);
	g_pTrialSaccades->setNoiseSize(etu::StringToInt(buffer, g_pTrialSaccades->getNoiseSize()));
	m_DisplayTime.GetWindowText(buffer, 100);
	g_pTrialSaccades->setDisplayTime(etu::StringToInt(buffer, g_pTrialSaccades->getDisplayTime()));
	m_DisplayRandMax.GetWindowText(buffer, 100);
	g_pTrialSaccades->setDisplayRandMax(etu::StringToInt(buffer, g_pTrialSaccades->getDisplayRandMax()));
	m_DisplayRandMin.GetWindowText(buffer, 100);
	g_pTrialSaccades->setDisplayRandMin(etu::StringToInt(buffer, g_pTrialSaccades->getDisplayRandMin()));
	m_FlashInterval.GetWindowText(buffer, 100);
	g_pTrialSaccades->setFlashInterval(etu::StringToInt(buffer, g_pTrialSaccades->getFlashInterval()));
	m_FlashIntervalRandMax.GetWindowText(buffer, 100);
	g_pTrialSaccades->setFlashIntervalRandMax(etu::StringToInt(buffer, g_pTrialSaccades->getFlashIntervalRandMax()));
	m_FlashIntervalRandMin.GetWindowText(buffer, 100);
	g_pTrialSaccades->setFlashIntervalRandMin(etu::StringToInt(buffer, g_pTrialSaccades->getFlashIntervalRandMin()));
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

	g_pTrialSaccades->setAdvanceEitherEye(m_AdvanceEitherEye.GetCheck() == BST_CHECKED);

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
	else if (m_CenterRadioAlternate.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setCenterColorRadio(CTrialSaccades::COLOR_ALTERNATE);
	} 
	else {
		g_pTrialSaccades->setCenterColorRadio(CTrialSaccades::COLOR_RANDOM);
	}

	m_AlternateBlockSize.GetWindowText(buffer, 100);
	g_pTrialSaccades->setAlternateBlockSize(etu::StringToInt(buffer, g_pTrialSaccades->getAlternateBlockSize()));
	
	if (m_PeriphRadioPrimary.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setPeriphColorRadio(CTrialSaccades::COLOR_PRIMARY);
		g_pTrialSaccades->setNumRedGrids(100);
	} 
	else if (m_PeriphRadioSecondary.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setPeriphColorRadio(CTrialSaccades::COLOR_SECONDARY);
		g_pTrialSaccades->setNumRedGrids(0);
	} 
	else if (m_PeriphRadioAlternate.GetCheck() == BST_CHECKED) {
		g_pTrialSaccades->setPeriphColorRadio(CTrialSaccades::COLOR_ALTERNATE);
		g_pTrialSaccades->setNumRedGrids(50);
	} 
	else {
		int redGrids;
		g_pTrialSaccades->setPeriphColorRadio(CTrialSaccades::COLOR_RANDOM);
		m_NumRedTrials.GetWindowText(buffer, 100); 

		/* Use -1 as value for "invalid integer". Otherwise, let CTrialSaccades
		 * accept or reject value. */
		if (0 > (redGrids = etu::StringToInt(buffer, -1))) {
			redGrids = 50;
		}
		g_pTrialSaccades->setNumRedGrids(redGrids);
		redGrids = g_pTrialSaccades->getNumRedGrids();
		m_NumRedTrials.SetWindowTextA(etu::IntToString(redGrids));
	}


	m_SaccadeHoldPeriod.GetWindowText(buffer, 100);
	g_pTrialSaccades->setSaccadeHoldPeriod(etu::StringToInt(buffer, g_pTrialSaccades->getSaccadeHoldPeriod())); 

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
	g_pTrialSaccades->setShowCenterFlash(m_ShowCenterFlash.GetCheck() == BST_CHECKED);
	g_pTrialSaccades->setShowCenterHold(m_ShowCenterHold.GetCheck() == BST_CHECKED);
	g_pTrialSaccades->setRandomizeFlashInterval(m_RandomizeFlashInterval.GetCheck() == BST_CHECKED);
	g_pTrialSaccades->setRandomizeDisplayTime(m_RandomizeDisplayTime.GetCheck() == BST_CHECKED);

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
	g_pTrialSaccades->setDisplayDuringHoldPeriod(m_DisplayDuringHoldPeriod.GetCheck() == BST_CHECKED);
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

	//CRect rect, r;
	CRect rect, r, rectlp, rectlr, rectlb, rectrp, rectrr, rectrb;

	CPaintDC dc_p(&m_PrimaryStatic);
	m_PrimaryStatic.GetClientRect(rect);
	dc_p.FillSolidRect(rect, RGB(g_pTrialSaccades->getPrimaryRed(),
		g_pTrialSaccades->getPrimaryGreen(), g_pTrialSaccades->getPrimaryBlue()));

	CPaintDC dc_s(&m_SecondaryStatic);
	m_SecondaryStatic.GetClientRect(rect);
	dc_s.FillSolidRect(rect, RGB(g_pTrialSaccades->getSecondaryRed(),
		g_pTrialSaccades->getSecondaryGreen(), g_pTrialSaccades->getSecondaryBlue()));

	CPaintDC dc_n(&m_NoiseStatic);
	m_NoiseStatic.GetClientRect(rect);
	dc_n.FillSolidRect(rect, RGB(g_pTrialSaccades->getNoiseRed(),
		g_pTrialSaccades->getNoiseGreen(),g_pTrialSaccades->getNoiseBlue()));
}

void CTrialSaccadesDlg::ClearTrialCounts()
{
	m_cRed_pRed_Right = 0;
	m_cRed_pRed_Wrong = 0;
	m_cRed_pRed_Total = 0;
	m_cRed_pBlue_Right = 0;
	m_cRed_pBlue_Wrong = 0;
	m_cRed_pBlue_Total = 0;
	m_cBlue_pRed_Right = 0;
	m_cBlue_pRed_Wrong = 0;
	m_cBlue_pRed_Total = 0;
	m_cBlue_pBlue_Right = 0;
	m_cBlue_pBlue_Wrong = 0;
	m_cBlue_pBlue_Total = 0;
	m_totalRight = 0;
	m_totalWrong = 0;
	m_totalTotal = 0;
	RefreshCountsDisplay();
}

/**
 * @brief Increment total and right or wrong count for a single trial type.
 * 
 * Increments the count of the total number of trials and conditionally
 * increments the number of right trials or number of wrong trials, depending
 * on whether trial was performed correctly or incorrectly.  Takes parameters
 * by reference for in-place modification.
 * 
 * @param[out] total Total number of trials of given type; always 
 *     incremented.
 * @param[out] right Number of trials of given type performed correctly; 
 *     incremented if correct is true.
 * @param[out] wrong Number of trials of given type performed incorrectly; 
 *     incremented if correct is false.
 * @param correct Whether response was correct or incorrect.
 */
void CTrialSaccadesDlg::IncrementSingleCount(int& total, int& right, 
											 int& wrong, bool correct)
{
	++total;
	if (correct) ++right; else ++wrong;
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
	IncrementSingleCount(m_totalTotal, m_totalRight, m_totalWrong, correct);

	switch (code) {
		case CSaccadeTrial::CODE_C_RED_P_RED:
			IncrementSingleCount(m_cRed_pRed_Total, m_cRed_pRed_Right, 
				m_cRed_pRed_Wrong, correct);
			break;
		case CSaccadeTrial::CODE_C_RED_P_BLUE: 
			IncrementSingleCount(m_cRed_pBlue_Total, m_cRed_pBlue_Right, 
				m_cRed_pBlue_Wrong, correct);
			break;
		case CSaccadeTrial::CODE_C_BLUE_P_RED:
			IncrementSingleCount(m_cBlue_pRed_Total, m_cBlue_pRed_Right, 
				m_cBlue_pRed_Wrong, correct);
			break;
		case CSaccadeTrial::CODE_C_BLUE_P_BLUE:
			IncrementSingleCount(m_cBlue_pBlue_Total, m_cBlue_pBlue_Right, 
				m_cBlue_pBlue_Wrong, correct);
			break;
	}
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
 *
 * @param boxTotal Handle to CStatic display box for the total count.
 * @param boxRight Handle to CStatic display box for the count of right 
 *     answers.
 * @param boxWrong Handle to CStatic display box for the count of wrong 
 *     answers.
 * @param total The total count.
 * @param right The number of trials performed correctly, right answers.
 * @param wrong The number of trials performed incorrectly, wrong answers.
 */
void CTrialSaccadesDlg::RefreshCountDisplay(CStatic& boxTotal, CStatic& boxRight,
											CStatic& boxWrong, int total, 
											int right, int wrong)
{
	boxRight.SetWindowText(etu::IntToString(right));
	boxWrong.SetWindowText(etu::IntToString(wrong));
	boxTotal.SetWindowText(PercentString(total, m_totalTotal, '\n'));
}

/**
 * @brief Refreshes the display of all trial counts.
 */
void CTrialSaccadesDlg::RefreshCountsDisplay()
{
	RefreshCountDisplay(m_TrialResults_RR_T, m_TrialResults_RR_R, 
		m_TrialResults_RR_W, m_cRed_pRed_Total, m_cRed_pRed_Right, 
		m_cRed_pRed_Wrong);

	RefreshCountDisplay(m_TrialResults_RB_T, m_TrialResults_RB_R, 
		m_TrialResults_RB_W, m_cRed_pBlue_Total, m_cRed_pBlue_Right, 
		m_cRed_pBlue_Wrong);

	RefreshCountDisplay(m_TrialResults_BB_T, m_TrialResults_BB_R, 
		m_TrialResults_BB_W, m_cBlue_pBlue_Total, m_cBlue_pBlue_Right, 
		m_cBlue_pBlue_Wrong);

	RefreshCountDisplay(m_TrialResults_BR_T, m_TrialResults_BR_R, 
		m_TrialResults_BR_W, m_cBlue_pRed_Total, m_cBlue_pRed_Right, 
		m_cBlue_pRed_Wrong);

	m_TrialResults_R_T.SetWindowText(PercentString(m_totalRight, m_totalTotal, ' '));
	m_TrialResults_W_T.SetWindowText(PercentString(m_totalWrong, m_totalTotal, ' '));
	m_TrialResults_Total.SetWindowText(etu::IntToString(m_totalTotal));
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
	
	sprintf(buffer, "%.2f", lxDeg);
	m_EyePosnDisplayLX.SetWindowText(buffer);
	
	sprintf(buffer, "%.2f", lyDeg);
	m_EyePosnDisplayLY.SetWindowText(buffer);
	
	sprintf(buffer, "%.2f", rxDeg);
	m_EyePosnDisplayRX.SetWindowText(buffer);
	
	sprintf(buffer, "%.2f", ryDeg);
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
		OnBnClickedGridSettingsRadioRandom();
	}
	else {
		OnBnClickedCenterRadioRandomize();
		OnBnClickedPeriphRadioRandomize();
		m_NumRedTrials.SetWindowText("50");
		m_NumRedTrials.EnableWindow(false);
	}

	m_GridSettingsRadioCyclopean.EnableWindow(!isSpatiotopic);
	m_CenterRadioPrimary.EnableWindow(isSpatiotopic);
	m_CenterRadioSecondary.EnableWindow(isSpatiotopic);
	m_CenterRadioAlternate.EnableWindow(isSpatiotopic);
	m_CenterRadioRandomize.EnableWindow(isSpatiotopic);
	m_PeriphRadioPrimary.EnableWindow(isSpatiotopic);
	m_PeriphRadioSecondary.EnableWindow(isSpatiotopic);
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
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
	m_AlternateBlockSize.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedCenterRadioSecondary()
{
	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_CHECKED);
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
	m_AlternateBlockSize.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedCenterRadioAlternate()
{
	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_UNCHECKED);
	m_CenterRadioAlternate.SetCheck(BST_CHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
	m_AlternateBlockSize.EnableWindow(true);
}

void CTrialSaccadesDlg::OnBnClickedCenterRadioRandomize()
{
	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_UNCHECKED);
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_CHECKED);
	m_AlternateBlockSize.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedPeriphRadioPrimary()
{
	m_PeriphRadioPrimary.SetCheck(BST_CHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_NumRedTrials.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedPeriphRadioSecondary()
{
	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_CHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_NumRedTrials.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedPeriphRadioAlternate()
{
	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_CHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_NumRedTrials.EnableWindow(false);
}

void CTrialSaccadesDlg::OnBnClickedPeriphRadioRandomize()
{
	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_CHECKED);
	m_NumRedTrials.EnableWindow(true);
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

void CTrialSaccadesDlg::OnBnClickedRandomizeDisplayTime()
{
	m_RandomizeDisplayTime.EnableWindow(true);
	m_DisplayRandMin.EnableWindow(m_RandomizeDisplayTime.GetCheck() == BST_CHECKED);
	m_DisplayRandMax.EnableWindow(m_RandomizeDisplayTime.GetCheck() == BST_CHECKED);
	m_DisplayTime.EnableWindow(m_RandomizeDisplayTime.GetCheck() == BST_UNCHECKED);
}

void CTrialSaccadesDlg::OnBnClickedRandomizeFlashInterval()
{
	m_RandomizeFlashInterval.EnableWindow(true);
	m_FlashIntervalRandMin.EnableWindow(m_RandomizeFlashInterval.GetCheck() == BST_CHECKED);
	m_FlashIntervalRandMax.EnableWindow(m_RandomizeFlashInterval.GetCheck() == BST_CHECKED);
	m_FlashInterval.EnableWindow(m_RandomizeFlashInterval.GetCheck() == BST_UNCHECKED);
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

#endif
