/**
 * @file ScotomaMapDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <deque>
#include "app/stdafx.h"
#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "ScotomaMapDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Defines
 *****************************************************************************/

#define MAX_STATUS_LINES (10)

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CScotomaMapDlg, CDialog)
CScotomaMapDlg::CScotomaMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScotomaMapDlg::IDD, pParent), response(0)
{}

CScotomaMapDlg::~CScotomaMapDlg() {}

void CScotomaMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PrimaryRed, m_PrimaryRed);
	DDX_Control(pDX, IDC_PrimaryGreen, m_PrimaryGreen);
	DDX_Control(pDX, IDC_PrimaryBlue, m_PrimaryBlue);
	DDX_Control(pDX, IDC_SecondaryRed, m_SecondaryRed);
	DDX_Control(pDX, IDC_SecondaryGreen, m_SecondaryGreen);
	DDX_Control(pDX, IDC_SecondaryBlue, m_SecondaryBlue);
	DDX_Control(pDX, IDC_TertiaryRed, m_TertiaryRed);
	DDX_Control(pDX, IDC_TertiaryGreen, m_TertiaryGreen);
	DDX_Control(pDX, IDC_TertiaryBlue, m_TertiaryBlue);
	DDX_Control(pDX, IDC_NoiseRed, m_NoiseRed);
	DDX_Control(pDX, IDC_NoiseGreen, m_NoiseGreen);
	DDX_Control(pDX, IDC_NoiseBlue, m_NoiseBlue);
	DDX_Control(pDX, IDC_ResponseDelay, m_ResponseDelay);
	DDX_Control(pDX, IDC_WrongDelay, m_WrongDelay);
	DDX_Control(pDX, IDC_NumRandomRepeats, m_NumRandomRepeats);
	DDX_Control(pDX, IDC_NoisePct, m_NoisePct);
	DDX_Control(pDX, IDC_NoiseSize, m_NoiseSize);
	DDX_Control(pDX, IDC_DisplayTime, m_DisplayTime);
	DDX_Control(pDX, IDC_DisplayRandMax, m_DisplayRandMax);
	DDX_Control(pDX, IDC_DisplayRandMin, m_DisplayRandMin);
	DDX_Control(pDX, IDC_FlashInterval, m_FlashInterval);
	DDX_Control(pDX, IDC_FlashIntervalRandMax, m_FlashIntervalRandMax);
	DDX_Control(pDX, IDC_FlashIntervalRandMin, m_FlashIntervalRandMin);
	DDX_Control(pDX, IDC_CrossPix, m_CrossPix);
	DDX_Control(pDX, IDC_AlternateBlockSize, m_AlternateBlockSize);
	DDX_Control(pDX, IDC_NumCatchTrials, m_NumCatchTrials);
	DDX_Control(pDX, IDC_NumCatchTrials2, m_NumCatchTrials2);
	DDX_Control(pDX, IDC_NumBlankTrials, m_NumBlankTrials);
	DDX_Control(pDX, IDC_NumStepsH, m_NumStepsH);
	DDX_Control(pDX, IDC_NumStepsV, m_NumStepsV);
	DDX_Control(pDX, IDC_PctTimeAtCenter, m_PctTimeAtCenter);
	DDX_Control(pDX, IDC_PctTimeAtPeriph, m_PctTimeAtPeriph);
	DDX_Control(pDX, IDC_UseNoise, m_UseNoise);
	DDX_Control(pDX, IDC_InterleaveNoise, m_InterleaveNoise);
	//DDX_Control(pDX, IDC_RampUp, m_RampUp);
	DDX_Control(pDX, IDC_HideCenter, m_HideCenter);
	DDX_Control(pDX, IDC_RandomizeFlashInterval, m_RandomizeFlashInterval);
	DDX_Control(pDX, IDC_RandomizeDisplayTime, m_RandomizeDisplayTime);
	DDX_Control(pDX, IDC_AvoidZero, m_AvoidZero);
	DDX_Control(pDX, IDC_CenterPoints, m_CenterPoints);
	DDX_Control(pDX, IDC_OffAxis, m_OffAxis);
	DDX_Control(pDX, IDC_CenterXPos, m_CenterXPos);
	DDX_Control(pDX, IDC_CenterYPos, m_CenterYPos);
	DDX_Control(pDX, IDC_CenterDotSize, m_CenterDotSize);
	DDX_Control(pDX, IDC_PeriphDotSize, m_PeriphDotSize);
	DDX_Control(pDX, IDC_StepSize, m_StepSize);

	DDX_Control(pDX, IDC_CenterXRadius, m_CenterXRadius);
	DDX_Control(pDX, IDC_CenterYRadius, m_CenterYRadius);
	DDX_Control(pDX, IDC_PeriphXRadius, m_PeriphXRadius);
	DDX_Control(pDX, IDC_PeriphYRadius, m_PeriphYRadius);
	DDX_Control(pDX, IDC_CenterRadioPrimary, m_CenterRadioPrimary);
	DDX_Control(pDX, IDC_CenterRadioSecondary, m_CenterRadioSecondary);
	DDX_Control(pDX, IDC_CenterRadioAlternate, m_CenterRadioAlternate);
	DDX_Control(pDX, IDC_CenterRadioRandomize, m_CenterRadioRandomize);
	DDX_Control(pDX, IDC_CenterRadioGlobalDot, m_CenterRadioGlobalDot);
	DDX_Control(pDX, IDC_PeriphRadioPrimary, m_PeriphRadioPrimary);
	DDX_Control(pDX, IDC_PeriphRadioSecondary, m_PeriphRadioSecondary);
	DDX_Control(pDX, IDC_PeriphRadioAlternate, m_PeriphRadioAlternate);
	DDX_Control(pDX, IDC_PeriphRadioTertiary, m_PeriphRadioTertiary);
	DDX_Control(pDX, IDC_PeriphRadioRandomize, m_PeriphRadioRandomize);
	DDX_Control(pDX, IDC_COUNTDOWN, m_Countdown);
	DDX_Control(pDX, IDC_PrimaryStatic, m_PrimaryStatic);
	DDX_Control(pDX, IDC_SecondaryStatic, m_SecondaryStatic);
	DDX_Control(pDX, IDC_NoiseStatic, m_NoiseStatic);
	DDX_Control(pDX, IDC_TertiaryStatic, m_TertiaryStatic);
	DDX_Control(pDX, IDC_Pause, m_Pause);
	//DDX_Control(pDX, IDC_RampDuration, m_RampDuration);
	DDX_Control(pDX, IDC_LeftEyeStatic, m_LeftEyeStatic);
	DDX_Control(pDX, IDC_RightEyeStatic, m_RightEyeStatic);
	DDX_Control(pDX, IDC_CatchPctR, m_CatchPctR);
	DDX_Control(pDX, IDC_CatchPctB, m_CatchPctB);
	DDX_Control(pDX, IDC_CatchPctN, m_CatchPctN);
	DDX_Control(pDX, IDC_COUNTDOWN2, m_Countdown2);
	DDX_Control(pDX, IDC_MisbehaviorDelay, m_MisbehaviorDelay);
	DDX_Control(pDX, IDC_TertiaryRewardPct, m_TertiaryRewardPct);
	DDX_Control(pDX, IDC_RewardTertiary, m_RewardTertiary);
	DDX_Control(pDX, IDC_RewardAny, m_RewardAny);
	DDX_Control(pDX, IDC_RewardCorrect, m_RewardCorrect);
	DDX_Control(pDX, IDC_RewardTrainingMode, m_RewardTrainingMode);
	DDX_Control(pDX, IDC_RewardDelay, m_RewardDelay);
	DDX_Control(pDX, IDC_DisplayDuringChoice, m_DisplayDuringChoice);
	DDX_Control(pDX, IDC_UseShapes, m_UseShapes);
	DDX_Control(pDX, IDC_SameBG, m_SameBG);

	DDX_Control(pDX, IDC_smStepSizeDeg, m_cStepSizeDeg);
	DDX_Control(pDX, IDC_smNumStepsX, m_cNumStepsX);
	DDX_Control(pDX, IDC_smNumStepsY, m_cNumStepsY);

	DDX_Control(pDX, IDC_OccludeBG, m_OccludeBG);
	DDX_Control(pDX, IDC_PctTimeAtEnriched, m_PctTimeAtEnriched);
	DDX_Control(pDX, IDC_EnrichedX, m_EnrichedX);
	DDX_Control(pDX, IDC_EnrichedY, m_EnrichedY);
	DDX_Control(pDX, IDC_EnrichedXRadius, m_EnrichedXRadius);
	DDX_Control(pDX, IDC_EnrichedYRadius, m_EnrichedYRadius);
	DDX_Control(pDX, IDC_DotGratings, m_DotGratings);
	DDX_Control(pDX, IDC_DotGratingContrast, m_DotGratingContrast);
	DDX_Control(pDX, IDC_DotGratingCycles, m_DotGratingCycles);
	DDX_Control(pDX, IDC_LeftEyeStaticR, m_LeftEyeStaticR);
	DDX_Control(pDX, IDC_RightEyeStaticR, m_RightEyeStaticR);
	DDX_Control(pDX, IDC_LeftEyeStaticB, m_LeftEyeStaticB);
	DDX_Control(pDX, IDC_RightEyeStaticB, m_RightEyeStaticB);
	DDX_Control(pDX, IDC_SeparationV, m_SeparationV);
	DDX_Control(pDX, IDC_UseKeyboard, m_UseKeyboard);
	DDX_Control(pDX, IDC_PurpleCount, m_PurpleCount);
	DDX_Control(pDX, IDC_TotalTrials, m_TotalTrials);
	DDX_Control(pDX, IDC_Pause2, m_Pause2);

	DDX_Control(pDX, IDC_DebugResponderOn, m_debugResponserOn);
	DDX_Control(pDX, IDC_DebugLabelsOn, m_debugLabelsOn);

	DDX_Control(pDX, IDC_smStatusDisplay, m_StatusDisplay);
}

BEGIN_MESSAGE_MAP(CScotomaMapDlg, CDialog)
	ON_BN_CLICKED(IDC_ButtonApply, OnBnClickedButtonapply)
	ON_BN_CLICKED(IDC_CenterRadioPrimary, OnBnClickedCenterradioprimary)
	ON_BN_CLICKED(IDC_CenterRadioSecondary, OnBnClickedCenterradiosecondary)
	ON_BN_CLICKED(IDC_CenterRadioAlternate, OnBnClickedCenterradioalternate)
	ON_BN_CLICKED(IDC_CenterRadioRandomize, OnBnClickedCenterradiorandomize)
	ON_BN_CLICKED(IDC_PeriphRadioPrimary, OnBnClickedPeriphradioprimary)
	ON_BN_CLICKED(IDC_PeriphRadioSecondary, OnBnClickedPeriphradiosecondary)
	ON_BN_CLICKED(IDC_PeriphRadioAlternate, OnBnClickedPeriphradioalternate)
	ON_BN_CLICKED(IDC_PeriphRadioRandomize, OnBnClickedPeriphradiorandomize)
	ON_BN_CLICKED(IDC_PeriphRadioTertiary, OnBnClickedPeriphradiotertiary)
	ON_BN_CLICKED(IDC_RandomizeDisplayTime, OnBnClickedRandomizedisplaytime)
	ON_BN_CLICKED(IDC_RandomizeFlashInterval, OnBnClickedRandomizeflashinterval)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_UseNoise, OnBnClickedUsenoise)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_ViewPurple, OnBnClickedViewpurple)
	ON_BN_CLICKED(IDC_ViewRed, OnBnClickedViewred)
	ON_BN_CLICKED(IDC_ViewBlue, OnBnClickedViewblue)
	ON_BN_CLICKED(IDC_RewardCorrect, OnBnClickedRewardcorrect)
	ON_BN_CLICKED(IDC_RewardAny, OnBnClickedRewardany)
	ON_BN_CLICKED(IDC_RewardTertiary, OnBnClickedRewardtertiary)
	ON_BN_CLICKED(IDC_CenterRadioGlobalDot, OnBnClickedCenterradioglobaldot)
	ON_BN_CLICKED(IDC_RewardTrainingMode, OnBnClickedRewardtrainingmode)
END_MESSAGE_MAP()

BOOL CScotomaMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	le_r = NULL;
	le_b = NULL;
	le_o = NULL;
	re_r = NULL;
	re_b = NULL;
	re_o = NULL;
	le_red_r = NULL;
	le_red_b = NULL;
	le_red_o = NULL;
	re_red_r = NULL;
	re_red_b = NULL;
	re_red_o = NULL;
	le_blue_r = NULL;
	le_blue_b = NULL;
	le_blue_o = NULL;
	re_blue_r = NULL;
	re_blue_b = NULL;
	re_blue_o = NULL;

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CScotomaMapDlg::load() 
{
	m_PrimaryRed.SetWindowText(etu::IntToString(g_pScotomaMap->getPrimaryRed()));
	m_PrimaryGreen.SetWindowText(etu::IntToString(g_pScotomaMap->getPrimaryGreen()));
	m_PrimaryBlue.SetWindowText(etu::IntToString(g_pScotomaMap->getPrimaryBlue()));
	m_SecondaryRed.SetWindowText(etu::IntToString(g_pScotomaMap->getSecondaryRed()));
	m_SecondaryGreen.SetWindowText(etu::IntToString(g_pScotomaMap->getSecondaryGreen()));
	m_SecondaryBlue.SetWindowText(etu::IntToString(g_pScotomaMap->getSecondaryBlue()));
	m_TertiaryRed.SetWindowText(etu::IntToString(g_pScotomaMap->getTertiaryRed()));
	m_TertiaryGreen.SetWindowText(etu::IntToString(g_pScotomaMap->getTertiaryGreen()));
	m_TertiaryBlue.SetWindowText(etu::IntToString(g_pScotomaMap->getTertiaryBlue()));
	m_NoiseRed.SetWindowText(etu::IntToString(g_pScotomaMap->getNoiseRed()));
	m_NoiseGreen.SetWindowText(etu::IntToString(g_pScotomaMap->getNoiseGreen()));
	m_NoiseBlue.SetWindowText(etu::IntToString(g_pScotomaMap->getNoiseBlue()));
	m_ResponseDelay.SetWindowText(etu::IntToString(g_pScotomaMap->getResponseDelay()));
	m_WrongDelay.SetWindowText(etu::IntToString(g_pScotomaMap->getWrongDelay()));
	m_NumRandomRepeats.SetWindowText(etu::IntToString(g_pScotomaMap->getNumRandomRepeats()));
	m_NoisePct.SetWindowText(etu::IntToString(g_pScotomaMap->getNoisePct()));
	m_NoiseSize.SetWindowText(etu::IntToString(g_pScotomaMap->getNoiseSize()));
	//m_RampDuration.SetWindowText(etu::IntToString(g_pScotomaMap->getRampDuration()));
	m_DisplayTime.SetWindowText(etu::IntToString(g_pScotomaMap->getDisplayTime()));
	m_MisbehaviorDelay.SetWindowText(etu::IntToString(g_pScotomaMap->getMisbehaviorDelay()));
	m_DisplayRandMax.SetWindowText(etu::IntToString(g_pScotomaMap->getDisplayRandMax()));
	m_DisplayRandMin.SetWindowText(etu::IntToString(g_pScotomaMap->getDisplayRandMin()));
	m_FlashInterval.SetWindowText(etu::IntToString(g_pScotomaMap->getFlashInterval()));
	m_FlashIntervalRandMax.SetWindowText(etu::IntToString(g_pScotomaMap->getFlashIntervalRandMax()));
	m_FlashIntervalRandMin.SetWindowText(etu::IntToString(g_pScotomaMap->getFlashIntervalRandMin()));
	m_CrossPix.SetWindowText(etu::IntToString(g_pScotomaMap->getCrossPix()));
	m_AlternateBlockSize.SetWindowText(etu::IntToString(g_pScotomaMap->getAlternateBlockSize()));
	m_NumCatchTrials.SetWindowText(etu::IntToString(g_pScotomaMap->getNumCatchTrials()));
	m_NumCatchTrials2.SetWindowText(etu::IntToString(g_pScotomaMap->getNumCatchTrials2()));
	m_NumBlankTrials.SetWindowText(etu::IntToString(g_pScotomaMap->getNumBlankTrials()));
	m_NumStepsH.SetWindowText(etu::IntToString(g_pScotomaMap->getNumStepsH()));
	m_NumStepsV.SetWindowText(etu::IntToString(g_pScotomaMap->getNumStepsV()));
	m_PctTimeAtCenter.SetWindowText(etu::IntToString(g_pScotomaMap->getPctTimeAtCenter()));
	m_PctTimeAtPeriph.SetWindowText(etu::IntToString(g_pScotomaMap->getPctTimeAtPeriph()));
	m_UseNoise.SetCheck((g_pScotomaMap->getUseNoise())? BST_CHECKED : BST_UNCHECKED);
	m_InterleaveNoise.SetCheck((g_pScotomaMap->getInterleaveNoise())? BST_CHECKED : BST_UNCHECKED);
	//m_RampUp.SetCheck((g_pScotomaMap->getRampUp())? BST_CHECKED : BST_UNCHECKED);
	m_HideCenter.SetCheck((g_pScotomaMap->getHideCenter())? BST_CHECKED : BST_UNCHECKED);
	m_RandomizeFlashInterval.SetCheck((g_pScotomaMap->getRandomizeFlashInterval())? BST_CHECKED : BST_UNCHECKED);
	m_RandomizeDisplayTime.SetCheck((g_pScotomaMap->getRandomizeDisplayTime())? BST_CHECKED : BST_UNCHECKED);
	m_AvoidZero.SetCheck((g_pScotomaMap->getAvoidZero())? BST_CHECKED : BST_UNCHECKED);
	m_CenterPoints.SetCheck((g_pScotomaMap->getCenterPoints())? BST_CHECKED : BST_UNCHECKED);
	m_OffAxis.SetCheck((g_pScotomaMap->getOffAxis())? BST_CHECKED : BST_UNCHECKED);
	m_CenterXPos.SetWindowText(etu::FloatToString(g_pScotomaMap->getCenterXPos()));
	m_CenterYPos.SetWindowText(etu::FloatToString(g_pScotomaMap->getCenterYPos()));
	m_cStepSizeDeg.SetWindowText(etu::FloatToString(g_pScotomaMap->getStepSizeDeg()));
	m_cNumStepsX.SetWindowText(etu::IntToString(g_pScotomaMap->getNumStepsX()));
	m_cNumStepsY.SetWindowText(etu::IntToString(g_pScotomaMap->getNumStepsY()));
	m_CenterDotSize.SetWindowText(etu::FloatToString(g_pScotomaMap->getCenterDotSize()));
	m_PeriphDotSize.SetWindowText(etu::FloatToString(g_pScotomaMap->getPeriphDotSize()));
	m_StepSize.SetWindowText(etu::FloatToString(g_pScotomaMap->getStepSize()));

	m_CenterXRadius.SetWindowText(etu::FloatToString(g_pScotomaMap->getCenterXRadius()));
	m_CenterYRadius.SetWindowText(etu::FloatToString(g_pScotomaMap->getCenterYRadius()));
	m_PeriphXRadius.SetWindowText(etu::FloatToString(g_pScotomaMap->getPeriphXRadius()));
	m_PeriphYRadius.SetWindowText(etu::FloatToString(g_pScotomaMap->getPeriphYRadius()));
	switch(g_pScotomaMap->getCenterColorRadio()) {
		case CScotomaMap::COLOR_PRIMARY: OnBnClickedCenterradioprimary(); break;
		case CScotomaMap::COLOR_SECONDARY: OnBnClickedCenterradiosecondary(); break;
		case CScotomaMap::COLOR_ALTERNATE: OnBnClickedCenterradioalternate(); break;
		case CScotomaMap::COLOR_RANDOM: OnBnClickedCenterradiorandomize(); break;
		default: OnBnClickedCenterradioglobaldot(); break;
	}
	switch(g_pScotomaMap->getPeriphColorRadio()) {
		case CScotomaMap::COLOR_PRIMARY: OnBnClickedPeriphradioprimary(); break;
		case CScotomaMap::COLOR_SECONDARY: OnBnClickedPeriphradiosecondary(); break;
		case CScotomaMap::COLOR_ALTERNATE: OnBnClickedPeriphradioalternate(); break;
		case CScotomaMap::COLOR_TERTIARY: OnBnClickedPeriphradiotertiary(); break;
		default: OnBnClickedPeriphradiorandomize(); break;
	}
	CRect rect;
	CPaintDC dc_p(&m_PrimaryStatic);
	m_PrimaryStatic.GetClientRect(rect);
	dc_p.FillSolidRect(rect, RGB(g_pScotomaMap->getPrimaryRed(),g_pScotomaMap->getPrimaryGreen(),g_pScotomaMap->getPrimaryBlue()));
	CPaintDC dc_s(&m_SecondaryStatic);
	m_SecondaryStatic.GetClientRect(rect);
	dc_s.FillSolidRect(rect, RGB(g_pScotomaMap->getSecondaryRed(),g_pScotomaMap->getSecondaryGreen(),g_pScotomaMap->getSecondaryBlue()));
	CPaintDC dc_t(&m_TertiaryStatic);
	m_TertiaryStatic.GetClientRect(rect);
	dc_t.FillSolidRect(rect, RGB(g_pScotomaMap->getTertiaryRed(),g_pScotomaMap->getTertiaryGreen(),g_pScotomaMap->getTertiaryBlue()));
	CPaintDC dc_n(&m_NoiseStatic);
	m_NoiseStatic.GetClientRect(rect);
	dc_n.FillSolidRect(rect, RGB(g_pScotomaMap->getNoiseRed(),g_pScotomaMap->getNoiseGreen(),g_pScotomaMap->getNoiseBlue()));

	//m_RampDuration.SetWindowText(etu::IntToString(g_pScotomaMap->getRampDuration()));

	SendDlgItemMessage(IDC_ViewPurple, BM_SETCHECK, BST_CHECKED, 0);

	switch(g_pScotomaMap->getRewardScheme()) {
		case CScotomaMap::REWARD_ANY: OnBnClickedRewardany(); break;
		case CScotomaMap::REWARD_TRAINING: OnBnClickedRewardtrainingmode(); break;
		case CScotomaMap::REWARD_TERTIARYRANDOM: OnBnClickedRewardtertiary(); break;
		default: OnBnClickedRewardcorrect(); break;
	}
	m_RewardDelay.SetWindowText(etu::IntToString(g_pScotomaMap->getRewardDelay()));
	m_TertiaryRewardPct.SetWindowText(etu::IntToString(g_pScotomaMap->getTertiaryRewardPct()));
	m_DisplayDuringChoice.SetCheck((g_pScotomaMap->getDisplayDuringChoice())? BST_CHECKED : BST_UNCHECKED);
	m_UseShapes.SetCheck((g_pScotomaMap->getUseShapes())? BST_CHECKED : BST_UNCHECKED);
	m_SameBG.SetCheck((g_pScotomaMap->getSameBG())? BST_CHECKED : BST_UNCHECKED);
	m_OccludeBG.SetCheck((g_pScotomaMap->getOccludeBG())? BST_CHECKED : BST_UNCHECKED);
	m_PctTimeAtEnriched.SetWindowText(etu::IntToString(g_pScotomaMap->getPctTimeAtEnriched()));
	m_EnrichedX.SetWindowText(etu::FloatToString(g_pScotomaMap->getEnrichedX()));
	m_EnrichedY.SetWindowText(etu::FloatToString(g_pScotomaMap->getEnrichedY()));
	m_EnrichedXRadius.SetWindowText(etu::FloatToString(g_pScotomaMap->getEnrichedXRadius()));
	m_EnrichedYRadius.SetWindowText(etu::FloatToString(g_pScotomaMap->getEnrichedYRadius()));
	m_DotGratings.SetCheck((g_pScotomaMap->getDotGratings())? BST_CHECKED : BST_UNCHECKED);
	m_DotGratingContrast.SetWindowText(etu::IntToString(g_pScotomaMap->getDotGratingContrast()));
	m_DotGratingCycles.SetWindowText(etu::FloatToString(g_pScotomaMap->getDotGratingCycles()));
	m_SeparationV.SetWindowText(etu::FloatToString(g_pScotomaMap->getSeparationV()));
	m_UseKeyboard.SetCheck((g_pScotomaMap->getUseKeyboard())? BST_CHECKED : BST_UNCHECKED);
}

void CScotomaMapDlg::save() 
{
	SmartLock scotomaMapLock(*g_pScotomaMap);
	SmartLock applyButtonLock(*this); /* Lock on own mutex as well. */

	char buffer[100];
	char debugbuffer[1024];

	m_PrimaryRed.GetWindowText(buffer, 100);
	g_pScotomaMap->setPrimaryRed(etu::StringToInt(buffer, g_pScotomaMap->getPrimaryRed()));
	m_PrimaryGreen.GetWindowText(buffer, 100);
	g_pScotomaMap->setPrimaryGreen(etu::StringToInt(buffer, g_pScotomaMap->getPrimaryGreen()));
	m_PrimaryBlue.GetWindowText(buffer, 100);
	g_pScotomaMap->setPrimaryBlue(etu::StringToInt(buffer, g_pScotomaMap->getPrimaryBlue()));
	m_SecondaryRed.GetWindowText(buffer, 100);
	g_pScotomaMap->setSecondaryRed(etu::StringToInt(buffer, g_pScotomaMap->getSecondaryRed()));
	m_SecondaryGreen.GetWindowText(buffer, 100);
	g_pScotomaMap->setSecondaryGreen(etu::StringToInt(buffer, g_pScotomaMap->getSecondaryGreen()));
	m_SecondaryBlue.GetWindowText(buffer, 100);
	g_pScotomaMap->setSecondaryBlue(etu::StringToInt(buffer, g_pScotomaMap->getSecondaryBlue()));
	m_TertiaryRed.GetWindowText(buffer, 100);
	g_pScotomaMap->setTertiaryRed(etu::StringToInt(buffer, g_pScotomaMap->getTertiaryRed()));
	m_TertiaryGreen.GetWindowText(buffer, 100);
	g_pScotomaMap->setTertiaryGreen(etu::StringToInt(buffer, g_pScotomaMap->getTertiaryGreen()));
	m_TertiaryBlue.GetWindowText(buffer, 100);
	g_pScotomaMap->setTertiaryBlue(etu::StringToInt(buffer, g_pScotomaMap->getTertiaryBlue()));
	m_NoiseRed.GetWindowText(buffer, 100);
	g_pScotomaMap->setNoiseRed(etu::StringToInt(buffer, g_pScotomaMap->getNoiseRed()));
	m_NoiseGreen.GetWindowText(buffer, 100);
	g_pScotomaMap->setNoiseGreen(etu::StringToInt(buffer, g_pScotomaMap->getNoiseGreen()));
	m_NoiseBlue.GetWindowText(buffer, 100);
	g_pScotomaMap->setNoiseBlue(etu::StringToInt(buffer, g_pScotomaMap->getNoiseBlue()));
	m_ResponseDelay.GetWindowText(buffer, 100);
	g_pScotomaMap->setResponseDelay(etu::StringToInt(buffer, g_pScotomaMap->getResponseDelay()));
	m_WrongDelay.GetWindowText(buffer, 100);
	g_pScotomaMap->setWrongDelay(etu::StringToInt(buffer, g_pScotomaMap->getWrongDelay()));
	m_MisbehaviorDelay.GetWindowText(buffer, 100);
	g_pScotomaMap->setMisbehaviorDelay(etu::StringToInt(buffer, g_pScotomaMap->getMisbehaviorDelay()));
	m_NumRandomRepeats.GetWindowText(buffer, 100);
	g_pScotomaMap->setNumRandomRepeats(etu::StringToInt(buffer, g_pScotomaMap->getNumRandomRepeats()));
	m_NoisePct.GetWindowText(buffer, 100);
	g_pScotomaMap->setNoisePct(etu::StringToInt(buffer, g_pScotomaMap->getNoisePct()));
	m_NoiseSize.GetWindowText(buffer, 100);
	g_pScotomaMap->setNoiseSize(etu::StringToInt(buffer, g_pScotomaMap->getNoiseSize()));
	//m_RampDuration.GetWindowText(buffer, 100);
	//g_pScotomaMap->setRampDuration(etu::StringToInt(buffer, g_pScotomaMap->getRampDuration()));
	m_DisplayTime.GetWindowText(buffer, 100);
	g_pScotomaMap->setDisplayTime(etu::StringToInt(buffer, g_pScotomaMap->getDisplayTime()));
	m_DisplayRandMax.GetWindowText(buffer, 100);
	g_pScotomaMap->setDisplayRandMax(etu::StringToInt(buffer, g_pScotomaMap->getDisplayRandMax()));
	m_DisplayRandMin.GetWindowText(buffer, 100);
	g_pScotomaMap->setDisplayRandMin(etu::StringToInt(buffer, g_pScotomaMap->getDisplayRandMin()));
	m_FlashInterval.GetWindowText(buffer, 100);
	g_pScotomaMap->setFlashInterval(etu::StringToInt(buffer, g_pScotomaMap->getFlashInterval()));
	m_FlashIntervalRandMax.GetWindowText(buffer, 100);
	g_pScotomaMap->setFlashIntervalRandMax(etu::StringToInt(buffer, g_pScotomaMap->getFlashIntervalRandMax()));
	m_FlashIntervalRandMin.GetWindowText(buffer, 100);
	g_pScotomaMap->setFlashIntervalRandMin(etu::StringToInt(buffer, g_pScotomaMap->getFlashIntervalRandMin()));
	m_CrossPix.GetWindowText(buffer, 100);
	g_pScotomaMap->setCrossPix(etu::StringToInt(buffer, g_pScotomaMap->getCrossPix()));
	if (m_CenterRadioPrimary.GetCheck() == BST_CHECKED) {
		g_pScotomaMap->setCenterColorRadio(CScotomaMap::COLOR_PRIMARY);
	} 
	else if (m_CenterRadioSecondary.GetCheck() == BST_CHECKED) {
		g_pScotomaMap->setCenterColorRadio(CScotomaMap::COLOR_SECONDARY);
	} 
	else if (m_CenterRadioAlternate.GetCheck() == BST_CHECKED) {
		g_pScotomaMap->setCenterColorRadio(CScotomaMap::COLOR_ALTERNATE);
	} 
	else if (m_CenterRadioRandomize.GetCheck() == BST_CHECKED) {
		g_pScotomaMap->setCenterColorRadio(CScotomaMap::COLOR_RANDOM);
	} 
	else {
		g_pScotomaMap->setCenterColorRadio(CScotomaMap::COLOR_GLOBAL);
	}
	m_AlternateBlockSize.GetWindowText(buffer, 100);
	g_pScotomaMap->setAlternateBlockSize(etu::StringToInt(buffer, g_pScotomaMap->getAlternateBlockSize()));
	if (m_PeriphRadioPrimary.GetCheck() == BST_CHECKED) {
		g_pScotomaMap->setPeriphColorRadio(CScotomaMap::COLOR_PRIMARY);
	} 
	else if (m_PeriphRadioSecondary.GetCheck() == BST_CHECKED) {
		g_pScotomaMap->setPeriphColorRadio(CScotomaMap::COLOR_SECONDARY);
	} 
	else if (m_PeriphRadioAlternate.GetCheck() == BST_CHECKED) {
		g_pScotomaMap->setPeriphColorRadio(CScotomaMap::COLOR_ALTERNATE);
	} 
	else if (m_PeriphRadioTertiary.GetCheck() == BST_CHECKED) {
		g_pScotomaMap->setPeriphColorRadio(CScotomaMap::COLOR_TERTIARY);
	} 
	else {
		g_pScotomaMap->setPeriphColorRadio(CScotomaMap::COLOR_RANDOM);
	}
	m_NumCatchTrials.GetWindowText(buffer, 100);//red
	g_pScotomaMap->setNumCatchTrials(etu::StringToInt(buffer, g_pScotomaMap->getNumCatchTrials()));
	m_NumCatchTrials2.GetWindowText(buffer, 100);//blue
	g_pScotomaMap->setNumCatchTrials2(atoi(buffer));
	m_NumBlankTrials.GetWindowText(buffer, 100);
	g_pScotomaMap->setNumBlankTrials(etu::StringToInt(buffer, g_pScotomaMap->getNumBlankTrials()));
	m_NumStepsH.GetWindowText(buffer, 100);
	g_pScotomaMap->setNumStepsH(etu::StringToInt(buffer, g_pScotomaMap->getNumStepsH()));
	m_NumStepsV.GetWindowText(buffer, 100);
	g_pScotomaMap->setNumStepsV(etu::StringToInt(buffer, g_pScotomaMap->getNumStepsV()));
	m_PctTimeAtCenter.GetWindowText(buffer, 100);
	g_pScotomaMap->setPctTimeAtCenter(etu::StringToInt(buffer, g_pScotomaMap->getPctTimeAtCenter()));
	m_PctTimeAtPeriph.GetWindowText(buffer, 100);
	g_pScotomaMap->setPctTimeAtPeriph(etu::StringToInt(buffer, g_pScotomaMap->getPctTimeAtPeriph()));
	g_pScotomaMap->setUseNoise(m_UseNoise.GetCheck()==BST_CHECKED);
	g_pScotomaMap->setInterleaveNoise(m_InterleaveNoise.GetCheck()==BST_CHECKED);
	//g_pScotomaMap->setRampUp(m_RampUp.GetCheck()==BST_CHECKED);
	g_pScotomaMap->setHideCenter(m_HideCenter.GetCheck()==BST_CHECKED);
	g_pScotomaMap->setRandomizeFlashInterval(m_RandomizeFlashInterval.GetCheck()==BST_CHECKED);
	g_pScotomaMap->setRandomizeDisplayTime(m_RandomizeDisplayTime.GetCheck()==BST_CHECKED);
	g_pScotomaMap->setAvoidZero(m_AvoidZero.GetCheck()==BST_CHECKED);
	g_pScotomaMap->setCenterPoints(m_CenterPoints.GetCheck()==BST_CHECKED);
	g_pScotomaMap->setOffAxis(m_OffAxis.GetCheck()==BST_CHECKED);

	m_CenterXPos.GetWindowText(buffer, 100);
	g_pScotomaMap->setCenterXPos(etu::StringToFloat(buffer, g_pScotomaMap->getCenterXPos()));
	m_CenterYPos.GetWindowText(buffer, 100);
	g_pScotomaMap->setCenterYPos(etu::StringToFloat(buffer, g_pScotomaMap->getCenterYPos()));
	m_CenterDotSize.GetWindowText(buffer, 100);
	g_pScotomaMap->setCenterDotSize(etu::StringToFloat(buffer, g_pScotomaMap->getCenterDotSize()));
	m_PeriphDotSize.GetWindowText(buffer, 100);
	g_pScotomaMap->setPeriphDotSize(etu::StringToFloat(buffer, g_pScotomaMap->getPeriphDotSize()));
	m_StepSize.GetWindowText(buffer, 100);
	g_pScotomaMap->setStepSize(etu::StringToFloat(buffer, g_pScotomaMap->getStepSize()));

	m_CenterXRadius.GetWindowText(buffer, 100);
	g_pScotomaMap->setCenterXRadius(etu::StringToFloat(buffer, g_pScotomaMap->getCenterXRadius()));
	m_CenterYRadius.GetWindowText(buffer, 100);
	g_pScotomaMap->setCenterYRadius(etu::StringToFloat(buffer, g_pScotomaMap->getCenterYRadius()));
	m_PeriphXRadius.GetWindowText(buffer, 100);
	g_pScotomaMap->setPeriphXRadius(etu::StringToFloat(buffer, g_pScotomaMap->getPeriphXRadius()));
	m_PeriphYRadius.GetWindowText(buffer, 100);
	g_pScotomaMap->setPeriphYRadius(etu::StringToFloat(buffer, g_pScotomaMap->getPeriphYRadius()));

	if (m_RewardTertiary.GetCheck() == BST_CHECKED) {
		g_pScotomaMap->setRewardScheme(CScotomaMap::REWARD_TERTIARYRANDOM);
	} 
	else if (m_RewardAny.GetCheck() == BST_CHECKED) {
		g_pScotomaMap->setRewardScheme(CScotomaMap::REWARD_ANY);
	} 
	else if (m_RewardTrainingMode.GetCheck() == BST_CHECKED) {
		g_pScotomaMap->setRewardScheme(CScotomaMap::REWARD_TRAINING);
	} 
	else {
		g_pScotomaMap->setRewardScheme(CScotomaMap::REWARD_RANDOM);
	}
	m_RewardDelay.GetWindowText(buffer, 100);
	g_pScotomaMap->setRewardDelay(etu::StringToInt(buffer, g_pScotomaMap->getRewardDelay()));
	m_TertiaryRewardPct.GetWindowText(buffer, 100);
	g_pScotomaMap->setTertiaryRewardPct(etu::StringToInt(buffer, g_pScotomaMap->getTertiaryRewardPct()));
	g_pScotomaMap->setDisplayDuringChoice(m_DisplayDuringChoice.GetCheck()==BST_CHECKED);
	g_pScotomaMap->setUseShapes(m_UseShapes.GetCheck()==BST_CHECKED);
	g_pScotomaMap->setSameBG(m_SameBG.GetCheck()==BST_CHECKED);
	
	m_cStepSizeDeg.GetWindowText(buffer, 100);
	g_pScotomaMap->setStepSizeDeg(etu::StringToFloat(buffer, g_pScotomaMap->getStepSizeDeg()));
		
	m_cNumStepsX.GetWindowText(buffer, 100);
	g_pScotomaMap->setNumStepsX(etu::StringToInt(buffer, g_pScotomaMap->getNumStepsX()));
		
	m_cNumStepsY.GetWindowText(buffer, 100);
	g_pScotomaMap->setNumStepsY(etu::StringToInt(buffer, g_pScotomaMap->getNumStepsY()));
	
	g_pScotomaMap->setOccludeBG(m_OccludeBG.GetCheck()==BST_CHECKED);

	m_PctTimeAtEnriched.GetWindowText(buffer, 100);
	g_pScotomaMap->setPctTimeAtEnriched(etu::StringToInt(buffer, g_pScotomaMap->getPctTimeAtEnriched()));
	m_EnrichedX.GetWindowText(buffer, 100);
	g_pScotomaMap->setEnrichedX(etu::StringToFloat(buffer, g_pScotomaMap->getEnrichedX()));
	m_EnrichedY.GetWindowText(buffer, 100);
	g_pScotomaMap->setEnrichedY(etu::StringToFloat(buffer, g_pScotomaMap->getEnrichedY()));
	m_EnrichedXRadius.GetWindowText(buffer, 100);
	g_pScotomaMap->setEnrichedXRadius(etu::StringToFloat(buffer, g_pScotomaMap->getEnrichedXRadius()));
	m_EnrichedYRadius.GetWindowText(buffer, 100);
	g_pScotomaMap->setEnrichedYRadius(etu::StringToFloat(buffer, g_pScotomaMap->getEnrichedYRadius()));
	g_pScotomaMap->setDotGratings(m_DotGratings.GetCheck()==BST_CHECKED);
	m_DotGratingContrast.GetWindowText(buffer, 100);
	g_pScotomaMap->setDotGratingContrast(etu::StringToInt(buffer, g_pScotomaMap->getDotGratingContrast()));
	m_DotGratingCycles.GetWindowText(buffer, 100);
	g_pScotomaMap->setDotGratingCycles(etu::StringToFloat(buffer, g_pScotomaMap->getDotGratingCycles()));
	m_SeparationV.GetWindowText(buffer, 100);
	g_pScotomaMap->setSeparationV(etu::StringToFloat(buffer, g_pScotomaMap->getSeparationV()));
	g_pScotomaMap->setUseKeyboard(m_UseKeyboard.GetCheck()==BST_CHECKED);

	g_pScotomaMap->sendSettings(g_pSettings->theSerialComm);
	
	/* Stimulus resets graphs and regenerates trials each time it's run */
}

void CScotomaMapDlg::resetGraphs() 
{
	int stephsize = 1+2*g_pScotomaMap->getNumStepsH();
	int stepvsize = 1+2*g_pScotomaMap->getNumStepsV();
	if (le_r != NULL) free(le_r);
	le_r = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (le_b != NULL) free(le_b);
	le_b = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (le_o != NULL) free(le_o);
	le_o = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (re_r != NULL) free(re_r);
	re_r = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (re_b != NULL) free(re_b);
	re_b = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (re_o != NULL) free(re_o);
	re_o = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	//
	if (re_red_r != NULL) free(re_red_r);
	re_red_r = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (re_red_b != NULL) free(re_red_b);
	re_red_b = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (re_red_o != NULL) free(re_red_o);
	re_red_o = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (re_blue_r != NULL) free(re_blue_r);
	re_blue_r = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (re_blue_b != NULL) free(re_blue_b);
	re_blue_b = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (re_blue_o != NULL) free(re_blue_o);
	re_blue_o = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	//
	if (le_red_r != NULL) free(le_red_r);
	le_red_r = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (le_red_b != NULL) free(le_red_b);
	le_red_b = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (le_red_o != NULL) free(le_red_o);
	le_red_o = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (le_blue_r != NULL) free(le_blue_r);
	le_blue_r = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (le_blue_b != NULL) free(le_blue_b);
	le_blue_b = (int*)malloc(sizeof(int)*stephsize*stepvsize);
	if (le_blue_o != NULL) free(le_blue_o);
	le_blue_o = (int*)malloc(sizeof(int)*stephsize*stepvsize);

	for (int i=0; i<stephsize; i++) {
		for (int j=0; j<stepvsize; j++) {
			le_r[i*stepvsize+j] = 0;
			le_b[i*stepvsize+j] = 0;
			le_o[i*stepvsize+j] = 0;
			re_r[i*stepvsize+j] = 0;
			re_b[i*stepvsize+j] = 0;
			re_o[i*stepvsize+j] = 0;
			//
			re_red_r[i*stepvsize+j] = 0;
			re_red_b[i*stepvsize+j] = 0;
			re_red_o[i*stepvsize+j] = 0;
			re_blue_r[i*stepvsize+j] = 0;
			re_blue_b[i*stepvsize+j] = 0;
			re_blue_o[i*stepvsize+j] = 0;
			//
			le_red_r[i*stepvsize+j] = 0;
			le_red_b[i*stepvsize+j] = 0;
			le_red_o[i*stepvsize+j] = 0;
			le_blue_r[i*stepvsize+j] = 0;
			le_blue_b[i*stepvsize+j] = 0;
			le_blue_o[i*stepvsize+j] = 0;
		}
	}
	catch_r_total = 0;
	catch_b_total = 0;
	catch_n_total = 0;
	catch_r = 0;
	catch_b = 0;
	catch_n = 0;
	reptotal = 0;
	leptotal = 0;
}

void CScotomaMapDlg::OnPaint() 
{
	CDialog::OnPaint();

	//CRect rect, r;
	CRect rect, r, rectlp, rectlr, rectlb, rectrp, rectrr, rectrb;

	CPaintDC dc_p(&m_PrimaryStatic);
	m_PrimaryStatic.GetClientRect(rect);
	dc_p.FillSolidRect(rect, RGB(g_pScotomaMap->getPrimaryRed(),g_pScotomaMap->getPrimaryGreen(),g_pScotomaMap->getPrimaryBlue()));

	CPaintDC dc_s(&m_SecondaryStatic);
	m_SecondaryStatic.GetClientRect(rect);
	dc_s.FillSolidRect(rect, RGB(g_pScotomaMap->getSecondaryRed(),g_pScotomaMap->getSecondaryGreen(),g_pScotomaMap->getSecondaryBlue()));

	CPaintDC dc_t(&m_TertiaryStatic);
	m_TertiaryStatic.GetClientRect(rect);
	dc_t.FillSolidRect(rect, RGB(g_pScotomaMap->getTertiaryRed(),g_pScotomaMap->getTertiaryGreen(),g_pScotomaMap->getTertiaryBlue()));

	CPaintDC dc_n(&m_NoiseStatic);
	m_NoiseStatic.GetClientRect(rect);
	dc_n.FillSolidRect(rect, RGB(g_pScotomaMap->getNoiseRed(),g_pScotomaMap->getNoiseGreen(),g_pScotomaMap->getNoiseBlue()));

	WaitForSingleObject(g_mutex, INFINITE);
	if (le_r != NULL && le_b != NULL && le_o != NULL &&
		le_red_r != NULL && le_red_b != NULL && le_red_o != NULL &&
		le_blue_r != NULL && le_blue_b != NULL && le_blue_o != NULL &&
		re_r != NULL && re_b != NULL && re_o != NULL &&
		re_red_r != NULL && re_red_b != NULL && re_red_o != NULL &&
		re_blue_r != NULL && re_blue_b != NULL && re_blue_o != NULL) {
		
		CPaintDC dc_lep(&m_LeftEyeStatic);
		m_LeftEyeStatic.GetClientRect(rectlp);
		CPaintDC dc_rep(&m_RightEyeStatic);
		m_RightEyeStatic.GetClientRect(rectrp);

		CPaintDC dc_ler(&m_LeftEyeStaticR);
		m_LeftEyeStaticR.GetClientRect(rectlr);
		CPaintDC dc_rer(&m_RightEyeStaticR);
		m_RightEyeStaticR.GetClientRect(rectrr);
		
		CPaintDC dc_leb(&m_LeftEyeStaticB);
		m_LeftEyeStaticB.GetClientRect(rectlb);
		CPaintDC dc_reb(&m_RightEyeStaticB);
		m_RightEyeStaticB.GetClientRect(rectrb);

		int stephsize = 1 + 2*g_pScotomaMap->getNumStepsH();
		int stepvsize = 1 + 2*g_pScotomaMap->getNumStepsV();
		for (int i=0; i<stephsize; i++) {
			for (int j=0; j<stepvsize; j++) {
				int total=0;
				//left eye purple,red,blue, then right eye purple,red,blue
				r.top = rectlp.bottom + (rectlp.top - rectlp.bottom) * j / stepvsize;
				r.bottom = rectlp.bottom + (rectlp.top - rectlp.bottom) * (j+1) / stepvsize;
				r.left = rectlp.left + (rectlp.right - rectlp.left) * i / stephsize;
				r.right = rectlp.left+ (rectlp.right - rectlp.left) * (i+1) / stephsize;
				total = le_r[i*stepvsize+j] + le_b[i*stepvsize+j] + le_o[i*stepvsize+j];
				dc_lep.FillSolidRect(r, (total == 0)? RGB(0,0,0) : RGB(
					255 * le_r[i*stepvsize+j] / total,	//answer red
					255 * le_o[i*stepvsize+j] / total,	//answer "both", n/a => green
					255 * le_b[i*stepvsize+j] / total));//answer blue

				r.top = rectlr.bottom + (rectlr.top - rectlr.bottom) * j / stepvsize;
				r.bottom = rectlr.bottom + (rectlr.top - rectlr.bottom) * (j+1) / stepvsize;
				r.left = rectlr.left + (rectlr.right - rectlr.left) * i / stephsize;
				r.right = rectlr.left+ (rectlr.right - rectlr.left) * (i+1) / stephsize;
				total = le_red_r[i*stepvsize+j] + le_red_b[i*stepvsize+j] + le_red_o[i*stepvsize+j];
				dc_ler.FillSolidRect(r, (total == 0)? RGB(0,0,0) : RGB(
					255 * le_red_r[i*stepvsize+j] / total,
					255 * le_red_o[i*stepvsize+j] / total,
					255 * le_red_b[i*stepvsize+j] / total));

				r.top = rectlb.bottom + (rectlb.top - rectlb.bottom) * j / stepvsize;
				r.bottom = rectlb.bottom + (rectlb.top - rectlb.bottom) * (j+1) / stepvsize;
				r.left = rectlb.left + (rectlb.right - rectlb.left) * i / stephsize;
				r.right = rectlb.left+ (rectlb.right - rectlb.left) * (i+1) / stephsize;
				total = le_blue_r[i*stepvsize+j] + le_blue_b[i*stepvsize+j] + le_blue_o[i*stepvsize+j];
				dc_leb.FillSolidRect(r, (total == 0)? RGB(0,0,0) : RGB(
					255 * le_blue_r[i*stepvsize+j] / total,
					255 * le_blue_o[i*stepvsize+j] / total,
					255 * le_blue_b[i*stepvsize+j] / total));

				r.top = rectrp.bottom + (rectrp.top - rectrp.bottom) * j / stepvsize;
				r.bottom = rectrp.bottom + (rectrp.top - rectrp.bottom) * (j+1) / stepvsize;
				r.left = rectrp.left + (rectrp.right - rectrp.left) * i / stephsize;
				r.right = rectrp.left+ (rectrp.right - rectrp.left) * (i+1) / stephsize;
				total = re_r[i*stepvsize+j] + re_b[i*stepvsize+j] + re_o[i*stepvsize+j];
				dc_rep.FillSolidRect(r, (total == 0)? RGB(0,0,0) : RGB(
					255 * re_r[i*stepvsize+j] / total,
					255 * re_o[i*stepvsize+j] / total,
					255 * re_b[i*stepvsize+j] / total));

				r.top = rectrr.bottom + (rectrr.top - rectrr.bottom) * j / stepvsize;
				r.bottom = rectrr.bottom + (rectrr.top - rectrr.bottom) * (j+1) / stepvsize;
				r.left = rectrr.left + (rectrr.right - rectrr.left) * i / stephsize;
				r.right = rectrr.left+ (rectrr.right - rectrr.left) * (i+1) / stephsize;
				total = re_red_r[i*stepvsize+j] + re_red_b[i*stepvsize+j] + re_red_o[i*stepvsize+j];
				dc_rer.FillSolidRect(r, (total == 0)? RGB(0,0,0) : RGB(
					255 * re_red_r[i*stepvsize+j] / total,
					255 * re_red_o[i*stepvsize+j] / total,
					255 * re_red_b[i*stepvsize+j] / total));

				r.top = rectrb.bottom + (rectrb.top - rectrb.bottom) * j / stepvsize;
				r.bottom = rectrb.bottom + (rectrb.top - rectrb.bottom) * (j+1) / stepvsize;
				r.left = rectrb.left + (rectrb.right - rectrb.left) * i / stephsize;
				r.right = rectrb.left+ (rectrb.right - rectrb.left) * (i+1) / stephsize;
				total = re_blue_r[i*stepvsize+j] + re_blue_b[i*stepvsize+j] + re_blue_o[i*stepvsize+j];
				dc_reb.FillSolidRect(r, (total == 0)? RGB(0,0,0) : RGB(
					255 * re_blue_r[i*stepvsize+j] / total,
					255 * re_blue_o[i*stepvsize+j] / total,
					255 * re_blue_b[i*stepvsize+j] / total));
			}
		}
		//white point at zero
		dc_lep.FillSolidRect(rectlp.left + (rectlp.right - rectlp.left)/2-1, rectlp.bottom + (rectlp.top - rectlp.bottom)/2-1, 2, 2, RGB(255,255,255));
		dc_rep.FillSolidRect(rectrp.left + (rectrp.right - rectrp.left)/2-1, rectrp.bottom + (rectrp.top - rectrp.bottom)/2-1, 2, 2, RGB(255,255,255));
		dc_ler.FillSolidRect(rectlr.left + (rectlr.right - rectlr.left)/2-1, rectlr.bottom + (rectlr.top - rectlr.bottom)/2-1, 2, 2, RGB(255,255,255));
		dc_rer.FillSolidRect(rectrr.left + (rectrr.right - rectrr.left)/2-1, rectrr.bottom + (rectrr.top - rectrr.bottom)/2-1, 2, 2, RGB(255,255,255));
		dc_leb.FillSolidRect(rectlb.left + (rectlb.right - rectlb.left)/2-1, rectlb.bottom + (rectlb.top - rectlb.bottom)/2-1, 2, 2, RGB(255,255,255));
		dc_reb.FillSolidRect(rectrb.left + (rectrb.right - rectrb.left)/2-1, rectrb.bottom + (rectrb.top - rectrb.bottom)/2-1, 2, 2, RGB(255,255,255));
	}

	ReleaseMutex(g_mutex);
}

void CScotomaMapDlg::OnBnClickedButtonapply()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "Scotoma Map Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
	g_pScotomaMapDlg->Invalidate(FALSE);
	g_pScotomaMap->GenerateCtrLocations();
}

void CScotomaMapDlg::OnBnClickedOk()
{	
	OnBnClickedButtonapply();
}

void CScotomaMapDlg::OnBnClickedCancel()
{}

void CScotomaMapDlg::OnBnClickedCenterradioprimary()
{
	m_CenterRadioPrimary.SetCheck(BST_CHECKED);
	m_CenterRadioSecondary.SetCheck(BST_UNCHECKED);
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
	m_CenterRadioGlobalDot.SetCheck(BST_UNCHECKED);
}

void CScotomaMapDlg::OnBnClickedCenterradiosecondary()
{
	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_CHECKED);
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
	m_CenterRadioGlobalDot.SetCheck(BST_UNCHECKED);
}

void CScotomaMapDlg::OnBnClickedCenterradioalternate()
{
	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_UNCHECKED);
	m_CenterRadioAlternate.SetCheck(BST_CHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
	m_CenterRadioGlobalDot.SetCheck(BST_UNCHECKED);
}

void CScotomaMapDlg::OnBnClickedCenterradiorandomize()
{
	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_UNCHECKED);
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_CHECKED);
	m_CenterRadioGlobalDot.SetCheck(BST_UNCHECKED);
}

void CScotomaMapDlg::OnBnClickedCenterradioglobaldot()
{
	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_UNCHECKED);
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
	m_CenterRadioGlobalDot.SetCheck(BST_CHECKED);
}

void CScotomaMapDlg::OnBnClickedPeriphradioprimary()
{
	m_PeriphRadioPrimary.SetCheck(BST_CHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_UNCHECKED);
}

void CScotomaMapDlg::OnBnClickedPeriphradiosecondary()
{
	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_CHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_UNCHECKED);
}

void CScotomaMapDlg::OnBnClickedPeriphradioalternate()
{
	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_CHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_UNCHECKED);
}

void CScotomaMapDlg::OnBnClickedPeriphradiorandomize()
{
	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_CHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_UNCHECKED);
}

void CScotomaMapDlg::OnBnClickedPeriphradiotertiary()
{
	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_CHECKED);
}

void CScotomaMapDlg::OnBnClickedRandomizedisplaytime()
{
	m_RandomizeDisplayTime.EnableWindow(true);
	m_DisplayRandMin.EnableWindow(m_RandomizeDisplayTime.GetCheck() == BST_CHECKED);
	m_DisplayRandMax.EnableWindow(m_RandomizeDisplayTime.GetCheck() == BST_CHECKED);
	m_DisplayTime.EnableWindow(m_RandomizeDisplayTime.GetCheck() == BST_UNCHECKED);
}

void CScotomaMapDlg::OnBnClickedRandomizeflashinterval()
{
	m_RandomizeFlashInterval.EnableWindow(true);
	m_FlashIntervalRandMin.EnableWindow(m_RandomizeFlashInterval.GetCheck() == BST_CHECKED);
	m_FlashIntervalRandMax.EnableWindow(m_RandomizeFlashInterval.GetCheck() == BST_CHECKED);
	m_FlashInterval.EnableWindow(m_RandomizeFlashInterval.GetCheck() == BST_UNCHECKED);
}

void CScotomaMapDlg::OnBnClickedUsenoise()
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

void CScotomaMapDlg::OnBnClickedButton2()
{
	/* Regenerate random sequence
	 * @TODO reinitialize online maps here; not important since button is 
	 * hidden*/
	g_pScotomaMap->GenerateTrials();
	((CScotomaMapStimulus*)g_pActiveStimulus)->ResetTrialSequence();
}

void CScotomaMapDlg::OnBnClickedButton6()
{
	/* @TODO Add your control notification handler code here; button is now
	 * hidden. */
	//g_pScotomaMap->GenerateCatchTrials();
}

void CScotomaMapDlg::OnBnClickedViewpurple()
{
	g_pScotomaMapDlg->Invalidate();
}

void CScotomaMapDlg::OnBnClickedViewred()
{
	g_pScotomaMapDlg->Invalidate();
}

void CScotomaMapDlg::OnBnClickedViewblue()
{
	g_pScotomaMapDlg->Invalidate();
}

void CScotomaMapDlg::OnBnClickedRewardcorrect()
{
	m_RewardTrainingMode.SetCheck(BST_UNCHECKED);
	m_RewardCorrect.SetCheck(BST_CHECKED);
	m_RewardAny.SetCheck(BST_UNCHECKED);
	m_RewardTertiary.SetCheck(BST_UNCHECKED);
}

void CScotomaMapDlg::OnBnClickedRewardany()
{
	m_RewardTrainingMode.SetCheck(BST_UNCHECKED);
	m_RewardCorrect.SetCheck(BST_UNCHECKED);
	m_RewardAny.SetCheck(BST_CHECKED);
	m_RewardTertiary.SetCheck(BST_UNCHECKED);
}

void CScotomaMapDlg::OnBnClickedRewardtertiary()
{
	m_RewardTrainingMode.SetCheck(BST_UNCHECKED);
	m_RewardCorrect.SetCheck(BST_UNCHECKED);
	m_RewardAny.SetCheck(BST_UNCHECKED);
	m_RewardTertiary.SetCheck(BST_CHECKED);
}

void CScotomaMapDlg::OnBnClickedRewardtrainingmode()
{
	m_RewardTrainingMode.SetCheck(BST_CHECKED);
	m_RewardCorrect.SetCheck(BST_UNCHECKED);
	m_RewardAny.SetCheck(BST_UNCHECKED);
	m_RewardTertiary.SetCheck(BST_UNCHECKED);
}

void CScotomaMapDlg::AddStatusLine(const char* line)
{
	std::deque<CString>::const_iterator iter;
	std::deque<CString>::const_iterator endIter;
	CString statusString;

	while (m_statusLines.size() >= MAX_STATUS_LINES) {
		m_statusLines.pop_front();
	}

	m_statusLines.push_back(CString(line));

	endIter = m_statusLines.end();
	for (iter = m_statusLines.begin(); iter < endIter; ++iter) {
		statusString.Append(*iter);
	}
	
	m_StatusDisplay.SetWindowText((LPCTSTR)statusString);

}

#endif
