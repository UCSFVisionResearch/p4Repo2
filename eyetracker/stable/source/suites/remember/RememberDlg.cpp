/**
 * @file RememberDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "app/AppHandler.h"
#include "app/EyeTrackingDlg.h"
#include "RememberDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CRememberDlg, CDialog)
CRememberDlg::CRememberDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRememberDlg::IDD, pParent)
{}

CRememberDlg::~CRememberDlg()
{}

void CRememberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EditDotSize, m_DotSize);
	DDX_Control(pDX, IDC_CenterXPos, m_CenterXPos);
	DDX_Control(pDX, IDC_CenterYPos, m_CenterYPos);
	DDX_Control(pDX, IDC_StepSize, m_StepSize);
	DDX_Control(pDX, IDC_NumStepsH, m_NumStepsH);
	DDX_Control(pDX, IDC_NumStepsV, m_NumStepsV);
	DDX_Control(pDX, IDC_PeriphDotSize, m_PeriphDotSize);
	DDX_Control(pDX, IDC_FlashInterval, m_FlashInterval);
	DDX_Control(pDX, IDC_FlashDuration, m_FlashDuration);
	DDX_Control(pDX, IDC_CenterPrimaryRed, m_CenterPrimaryRed);
	DDX_Control(pDX, IDC_CenterPrimaryGreen, m_CenterPrimaryGreen);
	DDX_Control(pDX, IDC_CenterPrimaryBlue, m_CenterPrimaryBlue);
	DDX_Control(pDX, IDC_CenterSecondaryRed, m_CenterSecondaryRed);
	DDX_Control(pDX, IDC_CenterSecondaryGreen, m_CenterSecondaryGreen);
	DDX_Control(pDX, IDC_CenterSecondaryBlue, m_CenterSecondaryBlue);
	DDX_Control(pDX, IDC_PeriphPrimaryRed, m_PeriphPrimaryRed);
	DDX_Control(pDX, IDC_PeriphPrimaryGreen, m_PeriphPrimaryGreen);
	DDX_Control(pDX, IDC_PeriphPrimaryBlue, m_PeriphPrimaryBlue);
	DDX_Control(pDX, IDC_PeriphSecondaryRed, m_PeriphSecondaryRed);
	DDX_Control(pDX, IDC_PeriphSecondaryGreen, m_PeriphSecondaryGreen);
	DDX_Control(pDX, IDC_PeriphSecondaryBlue, m_PeriphSecondaryBlue);

	DDX_Control(pDX, IDC_CenterRadioPrimary, m_CenterRadioPrimary);
	DDX_Control(pDX, IDC_CenterRadioSecondary, m_CenterRadioSecondary);
	DDX_Control(pDX, IDC_CenterRadioAlternate, m_CenterRadioAlternate);
	DDX_Control(pDX, IDC_CenterRadioRandomize, m_CenterRadioRandomize);

	DDX_Control(pDX, IDC_PeriphRadioPrimary, m_PeriphRadioPrimary);
	DDX_Control(pDX, IDC_PeriphRadioSecondary, m_PeriphRadioSecondary);
	DDX_Control(pDX, IDC_PeriphRadioAlternate, m_PeriphRadioAlternate);
	DDX_Control(pDX, IDC_PeriphRadioRandomize, m_PeriphRadioRandomize);
	DDX_Control(pDX, IDC_PeriphRadioTertiary, m_PeriphRadioTertiary);

	//behavior check
	DDX_Control(pDX, IDC_BEHAVIOR, m_BehaviorCheck);
	DDX_Control(pDX, IDC_MaxDelay, m_MaxDelay);

	DDX_Control(pDX, IDC_StepHoriz, m_StepHoriz);
	DDX_Control(pDX, IDC_StepVert, m_StepVert);
	DDX_Control(pDX, IDC_Step45, m_Step45);
	DDX_Control(pDX, IDC_Step135, m_Step135);
	DDX_Control(pDX, IDC_DistRadial, m_DistRadial);
	DDX_Control(pDX, IDC_DistXY, m_DistXY);
	DDX_Control(pDX, IDC_AlternateBlockSize, m_AlternateBlockSize);
	DDX_Control(pDX, IDC_MEMORY_GUIDED, m_MemoryGuided);
	DDX_Control(pDX, IDC_PERIPHERAL_FLASH, m_PeripheralFlash);
	DDX_Control(pDX, IDC_POST_FLASH_DELAY, m_PostFlashDelay);
	DDX_Control(pDX, IDC_INTERTRIAL_DELAY, m_IntertrialDelay);

	DDX_Control(pDX, IDC_AvoidZero, m_AvoidZero);
	DDX_Control(pDX, IDC_RedBlue, m_RedBlue);
	DDX_Control(pDX, IDC_RandomizePostFlash, m_RandomizePostFlash);
	DDX_Control(pDX, IDC_PostFlashRandMin, m_PostFlashRandMin);
	DDX_Control(pDX, IDC_PostFlashRandMax, m_PostFlashRandMax);
	DDX_Control(pDX, IDC_RandomizeFlashInterval, m_RandomizeFlashInterval);
	DDX_Control(pDX, IDC_FlashIntervalRandMin, m_FlashIntervalRandMin);
	DDX_Control(pDX, IDC_FlashIntervalRandMax, m_FlashIntervalRandMax);
	DDX_Control(pDX, IDC_CenterPrimaryStatic, m_CenterPrimaryStatic);
	DDX_Control(pDX, IDC_CenterSecondaryStatic, m_CenterSecondaryStatic);
	DDX_Control(pDX, IDC_PeriphPrimaryStatic, m_PeriphPrimaryStatic);
	DDX_Control(pDX, IDC_PeriphSecondaryStatic, m_PeriphSecondaryStatic);
	DDX_Control(pDX, IDC_PeriphXPosR, m_PeriphXPos);
	DDX_Control(pDX, IDC_PeriphYPosR, m_PeriphYPos);
	DDX_Control(pDX, IDC_PeriphXRadius, m_PeriphXRadius);
	DDX_Control(pDX, IDC_PeriphYRadius, m_PeriphYRadius);
	DDX_Control(pDX, IDC_PctTimeAtPeriph, m_PctTimeAtPeriph);
	DDX_Control(pDX, IDC_CHECK_SEQ_RANDOM, m_CheckSeqRandom);
	DDX_Control(pDX, IDC_NumRandomRepeats, m_NumRandomRepeats);
	DDX_Control(pDX, IDC_COUNTDOWN, m_Countdown);
	DDX_Control(pDX, IDC_UseNoise, m_UseNoise);
	DDX_Control(pDX, IDC_NoiseRed, m_NoiseRed);
	DDX_Control(pDX, IDC_NoiseGreen, m_NoiseGreen);
	DDX_Control(pDX, IDC_NoiseBlue, m_NoiseBlue);
	DDX_Control(pDX, IDC_NoiseStatic, m_NoiseStatic);
	DDX_Control(pDX, IDC_PeriphTertiaryRed, m_PeriphTertiaryRed);
	DDX_Control(pDX, IDC_PeriphTertiaryGreen, m_PeriphTertiaryGreen);
	DDX_Control(pDX, IDC_PeriphTertiaryBlue, m_PeriphTertiaryBlue);
	DDX_Control(pDX, IDC_PeriphTertiaryStatic, m_PeriphTertiaryStatic);
	DDX_Control(pDX, IDC_NumCatchTrials, m_NumCatchTrials);
	DDX_Control(pDX, IDC_GridSteps, m_GridSteps);
	DDX_Control(pDX, IDC_NoiseSize, m_NoiseSize);
	DDX_Control(pDX, IDC_CrossPix, m_CrossPix);
	DDX_Control(pDX, IDC_NumBlankTrials, m_NumBlankTrials);
	DDX_Control(pDX, IDC_NoisePct, m_NoisePct);
	DDX_Control(pDX, IDC_UseDim, m_UseDim);
	DDX_Control(pDX, IDC_DimOnsetMin, m_DimOnsetMin);
	DDX_Control(pDX, IDC_DimOnsetMax, m_DimOnsetMax);
	DDX_Control(pDX, IDC_DimTimeout, m_DimTimeout);
	DDX_Control(pDX, IDC_AnswerPeriod, m_AnswerPeriod);
	DDX_Control(pDX, IDC_DimBrightness, m_DimBrightness);
	DDX_Control(pDX, IDC_HideCenter, m_HideCenter);
	DDX_Control(pDX, IDC_InterleaveNoise, m_InterleaveNoise);
	DDX_Control(pDX, IDC_CenterPoints, m_CenterPoints);
	DDX_Control(pDX, IDC_Pause, m_Pause);
	DDX_Control(pDX, IDC_OffAxis, m_OffAxis);
	DDX_Control(pDX, IDC_Response, m_Response);
	DDX_Control(pDX, IDC_ResponseTimeout, m_ResponseTimeout);
	DDX_Control(pDX, IDC_PeriphXPosL, m_PeriphXPosL);
	DDX_Control(pDX, IDC_PeriphYPosL, m_PeriphYPosL);
	DDX_Control(pDX, IDC_RampUp, m_RampUp);
	DDX_Control(pDX, IDC_RampDuration, m_RampDuration);
	DDX_Control(pDX, IDC_LeftEyeStatic, m_LeftEyeStatic);
	DDX_Control(pDX, IDC_RightEyeStatic, m_RightEyeStatic);
	DDX_Control(pDX, IDC_CatchPctR, m_CatchPctR);
	DDX_Control(pDX, IDC_CatchPctB, m_CatchPctB);
	DDX_Control(pDX, IDC_CatchPctN, m_CatchPctN);
	DDX_Control(pDX, IDC_PctTimeAtCenter, m_PctTimeAtCenter);
	DDX_Control(pDX, IDC_NumCatchTrials2, m_NumCatchTrials2);
	DDX_Control(pDX, IDC_CenterXRadius, m_CenterXRadius);
	DDX_Control(pDX, IDC_CenterYRadius, m_CenterYRadius);
	DDX_Control(pDX, IDC_COUNTDOWN2, m_Countdown2);
	DDX_Control(pDX, IDC_AutoPosition, m_AutoPosition);
}

BEGIN_MESSAGE_MAP(CRememberDlg, CDialog)
	ON_BN_CLICKED(IDC_ButtonApply, OnBnClickedButtonapply)
	ON_BN_CLICKED(IDC_BEHAVIOR, OnBnClickedBehavior)
	ON_BN_CLICKED(IDC_CenterRadioPrimary, OnBnClickedCenterradioprimary)
	ON_BN_CLICKED(IDC_CenterRadioSecondary, OnBnClickedCenterradiosecondary)
	ON_BN_CLICKED(IDC_CenterRadioAlternate, OnBnClickedCenterradioalternate)
	ON_BN_CLICKED(IDC_CenterRadioRandomize, OnBnClickedCenterradiorandomize)
	ON_BN_CLICKED(IDC_PeriphRadioPrimary, OnBnClickedPeriphradioprimary)
	ON_BN_CLICKED(IDC_PeriphRadioSecondary, OnBnClickedPeriphradiosecondary)
	ON_BN_CLICKED(IDC_PeriphRadioAlternate, OnBnClickedPeriphradioalternate)
	ON_BN_CLICKED(IDC_PeriphRadioRandomize, OnBnClickedPeriphradiorandomize)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_DistRadial, OnBnClickedDistradial)
	ON_BN_CLICKED(IDC_DistXY, OnBnClickedDistxy)
	ON_BN_CLICKED(IDC_MEMORY_GUIDED, OnBnClickedMemoryGuided)
	ON_BN_CLICKED(IDC_PERIPHERAL_FLASH, OnBnClickedPeripheralFlash)
	ON_BN_CLICKED(IDC_RedBlue, OnBnClickedRedblue)
	ON_BN_CLICKED(IDC_RandomizePostFlash, OnBnClickedRandomizepostflash)
	ON_BN_CLICKED(IDC_RandomizeFlashInterval, OnBnClickedRandomizeflashinterval)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PeriphRadioTertiary, OnBnClickedPeriphradiotertiary)
	ON_BN_CLICKED(IDC_CHECK_SEQ_RANDOM, OnBnClickedCheckSeqRandom)
	ON_BN_CLICKED(IDC_UseNoise, OnBnClickedUsenoise)
	ON_BN_CLICKED(IDC_UseDim, OnBnClickedUsedim)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_ViewPurple, OnBnClickedViewpurple)
	ON_BN_CLICKED(IDC_ViewRed, OnBnClickedViewred)
	ON_BN_CLICKED(IDC_ViewBlue, OnBnClickedViewblue)
END_MESSAGE_MAP()

BOOL CRememberDlg::OnInitDialog()
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

void CRememberDlg::load() 
{	
	m_DotSize.SetWindowText(etu::FloatToString(g_pRemember->getCenterDotSize()));
	m_CenterXPos.SetWindowText(etu::FloatToString(g_pRemember->getCenterXPos()));
	m_CenterYPos.SetWindowText(etu::FloatToString(g_pRemember->getCenterYPos()));
	m_StepSize.SetWindowText(etu::FloatToString(g_pRemember->getStepSize()));
	m_NumStepsH.SetWindowText(etu::IntToString(g_pRemember->getNumStepsH()));
	m_NumStepsV.SetWindowText(etu::IntToString(g_pRemember->getNumStepsV()));
	m_PeriphDotSize.SetWindowText(etu::FloatToString(g_pRemember->getPeriphDotSize()));
	m_FlashInterval.SetWindowText(etu::IntToString(g_pRemember->getFlashInterval()));
	m_FlashDuration.SetWindowText(etu::IntToString(g_pRemember->getFlashDuration()));
	m_CenterPrimaryRed.SetWindowText(etu::IntToString(g_pRemember->getCenterPrimaryRed()));
	m_CenterPrimaryGreen.SetWindowText(etu::IntToString(g_pRemember->getCenterPrimaryGreen()));
	m_CenterPrimaryBlue.SetWindowText(etu::IntToString(g_pRemember->getCenterPrimaryBlue()));
	m_CenterSecondaryRed.SetWindowText(etu::IntToString(g_pRemember->getCenterSecondaryRed()));
	m_CenterSecondaryGreen.SetWindowText(etu::IntToString(g_pRemember->getCenterSecondaryGreen()));
	m_CenterSecondaryBlue.SetWindowText(etu::IntToString(g_pRemember->getCenterSecondaryBlue()));
	m_PeriphPrimaryRed.SetWindowText(etu::IntToString(g_pRemember->getPeriphPrimaryRed()));
	m_PeriphPrimaryGreen.SetWindowText(etu::IntToString(g_pRemember->getPeriphPrimaryGreen()));
	m_PeriphPrimaryBlue.SetWindowText(etu::IntToString(g_pRemember->getPeriphPrimaryBlue()));
	m_PeriphSecondaryRed.SetWindowText(etu::IntToString(g_pRemember->getPeriphSecondaryRed()));
	m_PeriphSecondaryGreen.SetWindowText(etu::IntToString(g_pRemember->getPeriphSecondaryGreen()));
	m_PeriphSecondaryBlue.SetWindowText(etu::IntToString(g_pRemember->getPeriphSecondaryBlue()));
	m_PostFlashDelay.SetWindowText(etu::IntToString(g_pRemember->getPostFlashDelay()));
	m_IntertrialDelay.SetWindowText(etu::IntToString(g_pRemember->getIntertrialDelay()));

	switch(g_pRemember->getCenterColorRadio()) {
		case CRemember::COLOR_PRIMARY: OnBnClickedCenterradioprimary(); break;
		case CRemember::COLOR_SECONDARY: OnBnClickedCenterradiosecondary(); break;
		case CRemember::COLOR_ALTERNATE: OnBnClickedCenterradioalternate(); break;
		default: OnBnClickedCenterradiorandomize(); break;
	}

	switch(g_pRemember->getPeriphColorRadio()) {
		case CRemember::COLOR_PRIMARY: OnBnClickedPeriphradioprimary(); break;
		case CRemember::COLOR_SECONDARY: OnBnClickedPeriphradiosecondary(); break;
		case CRemember::COLOR_ALTERNATE: OnBnClickedPeriphradioalternate(); break;
		case CRemember::COLOR_TERTIARY: OnBnClickedPeriphradiotertiary(); break;
		default: OnBnClickedPeriphradiorandomize(); break;
	}

	//step orientation boxes
	m_StepVert.SetCheck( (g_pRemember->getStepVert() == 0)? BST_UNCHECKED : BST_CHECKED );
	m_StepHoriz.SetCheck( (g_pRemember->getStepHoriz() == 0)? BST_UNCHECKED : BST_CHECKED );
	m_Step45.SetCheck( (g_pRemember->getStep45() == 0)? BST_UNCHECKED : BST_CHECKED );
	m_Step135.SetCheck( (g_pRemember->getStep135() == 0)? BST_UNCHECKED : BST_CHECKED );

	m_DistType = g_pRemember->getDistType();
	if (m_DistType == CRemember::DIST_TYPE_XY) {
		m_DistXY.SetCheck(BST_CHECKED);
		m_DistRadial.SetCheck(BST_UNCHECKED);
	} 
	else {
		m_DistXY.SetCheck(BST_UNCHECKED);
		m_DistRadial.SetCheck(BST_CHECKED);
	}

	m_AlternateBlockSize.SetWindowText(etu::IntToString(g_pRemember->getAlternateBlockSize()));

	m_BehaviorCheck.SetCheck((g_pRemember->useBehavior())? BST_CHECKED : BST_UNCHECKED);
	OnBnClickedBehavior();
	m_MaxDelay.SetWindowText(etu::IntToString(g_pRemember->getMaxDelay()));

	m_AvoidZero.SetCheck((g_pRemember->getAvoidZero())? BST_CHECKED : BST_UNCHECKED);

	m_RedBlue.SetCheck((g_pRemember->getRedBlue())? BST_CHECKED : BST_UNCHECKED);
	OnBnClickedRedblue();

	m_RandomizePostFlash.SetCheck((g_pRemember->getRandomizePostFlash())? BST_CHECKED : BST_UNCHECKED);
	m_PostFlashRandMin.SetWindowText(etu::IntToString(g_pRemember->getPostFlashRandMin()));
	m_PostFlashRandMax.SetWindowText(etu::IntToString(g_pRemember->getPostFlashRandMax()));
	OnBnClickedRandomizepostflash();

	m_RandomizeFlashInterval.SetCheck((g_pRemember->getRandomizeFlashInterval())? BST_CHECKED : BST_UNCHECKED);
	m_FlashIntervalRandMin.SetWindowText(etu::IntToString(g_pRemember->getFlashIntervalRandMin()));
	m_FlashIntervalRandMax.SetWindowText(etu::IntToString(g_pRemember->getFlashIntervalRandMax()));
	OnBnClickedRandomizeflashinterval();

	switch (g_pRemember->getStimType()) {
		case CRemember::STIM_MEMORY_GUIDED: OnBnClickedMemoryGuided(); break;
		case CRemember::STIM_PERIPHERAL_FLASH: OnBnClickedPeripheralFlash(); break;
		default: OnBnClickedPeripheralFlash(); break;
	} 

	CRect rect;

	CPaintDC dc_cp(&m_CenterPrimaryStatic);
	m_CenterPrimaryStatic.GetClientRect(rect);
	dc_cp.FillSolidRect(rect, RGB(g_pRemember->getCenterPrimaryRed(),g_pRemember->getCenterPrimaryGreen(),g_pRemember->getCenterPrimaryBlue()));

	CPaintDC dc_cs(&m_CenterSecondaryStatic);
	m_CenterSecondaryStatic.GetClientRect(rect);
	dc_cs.FillSolidRect(rect, RGB(g_pRemember->getCenterSecondaryRed(),g_pRemember->getCenterSecondaryGreen(),g_pRemember->getCenterSecondaryBlue()));

	CPaintDC dc_pp(&m_PeriphPrimaryStatic);
	m_PeriphPrimaryStatic.GetClientRect(rect);
	dc_pp.FillSolidRect(rect, RGB(g_pRemember->getPeriphPrimaryRed(),g_pRemember->getPeriphPrimaryGreen(),g_pRemember->getPeriphPrimaryBlue()));

	CPaintDC dc_ps(&m_PeriphSecondaryStatic);
	m_PeriphSecondaryStatic.GetClientRect(rect);
	dc_ps.FillSolidRect(rect, RGB(g_pRemember->getPeriphSecondaryRed(),g_pRemember->getPeriphSecondaryGreen(),g_pRemember->getPeriphSecondaryBlue()));

	m_PeriphXPos.SetWindowText(etu::FloatToString(g_pRemember->getPeriphXPos()));
	m_PeriphYPos.SetWindowText(etu::FloatToString(g_pRemember->getPeriphYPos()));
	m_PeriphXRadius.SetWindowText(etu::FloatToString(g_pRemember->getPeriphXRadius()));
	m_PeriphYRadius.SetWindowText(etu::FloatToString(g_pRemember->getPeriphYRadius()));
	m_PctTimeAtPeriph.SetWindowText(etu::IntToString(g_pRemember->getPctTimeAtPeriph()));

	m_CheckSeqRandom.SetCheck((g_pRemember->useSeqRandom())? BST_CHECKED : BST_UNCHECKED);
	OnBnClickedCheckSeqRandom();

	m_PeriphTertiaryRed.SetWindowText(etu::IntToString(g_pRemember->getPeriphTertiaryRed()));
	m_PeriphTertiaryGreen.SetWindowText(etu::IntToString(g_pRemember->getPeriphTertiaryGreen()));
	m_PeriphTertiaryBlue.SetWindowText(etu::IntToString(g_pRemember->getPeriphTertiaryBlue()));
	m_NoiseRed.SetWindowText(etu::IntToString(g_pRemember->getNoiseRed()));
	m_NoiseGreen.SetWindowText(etu::IntToString(g_pRemember->getNoiseGreen()));
	m_NoiseBlue.SetWindowText(etu::IntToString(g_pRemember->getNoiseBlue()));
	m_NumRandomRepeats.SetWindowText(etu::IntToString(g_pRemember->getNumRandomRepeats()));
	m_UseNoise.SetCheck((g_pRemember->getUseNoise())? BST_CHECKED : BST_UNCHECKED);
	m_NumCatchTrials.SetWindowText(etu::IntToString(g_pRemember->getNumCatchTrials()));

	m_GridSteps.SetCheck((g_pRemember->getGridSteps())? BST_CHECKED : BST_UNCHECKED);
	m_NoiseSize.SetWindowText(etu::IntToString(g_pRemember->getNoiseSize()));
	m_CrossPix.SetWindowText(etu::IntToString(g_pRemember->getCrossPix()));
	m_NumBlankTrials.SetWindowText(etu::IntToString(g_pRemember->getNumBlankTrials()));
	m_NoisePct.SetWindowText(etu::IntToString(g_pRemember->getNoisePct()));
	OnBnClickedUsenoise();

	m_UseDim.SetCheck((g_pRemember->getUseDim())? BST_CHECKED : BST_UNCHECKED);
	m_DimTimeout.SetWindowText(etu::IntToString(g_pRemember->getDimTimeout()));
	m_DimOnsetMin.SetWindowText(etu::IntToString(g_pRemember->getDimOnsetMin()));
	m_DimOnsetMax.SetWindowText(etu::IntToString(g_pRemember->getDimOnsetMax()));
	m_AnswerPeriod.SetWindowText(etu::IntToString(g_pRemember->getAnswerPeriod()));
	m_DimBrightness.SetWindowText(etu::IntToString(g_pRemember->getDimBrightness()));
	OnBnClickedUsedim();

	m_HideCenter.SetCheck((g_pRemember->getHideCenter())? BST_CHECKED : BST_UNCHECKED);
	m_InterleaveNoise.SetCheck((g_pRemember->getInterleaveNoise())? BST_CHECKED : BST_UNCHECKED);
	m_CenterPoints.SetCheck((g_pRemember->getCenterPoints())? BST_CHECKED : BST_UNCHECKED);
	m_OffAxis.SetCheck((g_pRemember->getOffAxis())? BST_CHECKED : BST_UNCHECKED);
	
	m_Response.SetCheck((g_pRemember->getResponse())? BST_CHECKED : BST_UNCHECKED);
	m_PeriphXPosL.SetWindowText(etu::FloatToString(g_pRemember->getPeriphXPosL()));
	m_PeriphYPosL.SetWindowText(etu::FloatToString(g_pRemember->getPeriphYPosL()));
	m_ResponseTimeout.SetWindowText(etu::IntToString(g_pRemember->getResponseTimeout()));
	m_RampUp.SetCheck((g_pRemember->getRampUp())? BST_CHECKED : BST_UNCHECKED);
	m_RampDuration.SetWindowText(etu::IntToString(g_pRemember->getRampDuration()));
	m_PctTimeAtCenter.SetWindowText(etu::IntToString(g_pRemember->getPctTimeAtCenter()));
	m_NumCatchTrials2.SetWindowText(etu::IntToString(g_pRemember->getNumCatchTrials2()));

	m_CenterXRadius.SetWindowText(etu::FloatToString(g_pRemember->getCenterXRadius()));
	m_CenterYRadius.SetWindowText(etu::FloatToString(g_pRemember->getCenterYRadius()));
	m_AutoPosition.SetCheck((g_pRemember->getAutoPosition())? BST_CHECKED : BST_UNCHECKED);
	
	SendDlgItemMessage(IDC_ViewPurple, BM_SETCHECK, BST_CHECKED, 0);
}

void CRememberDlg::save() 
{
	char buffer[100];

	m_DotSize.GetWindowText(buffer, 100);
	g_pRemember->setCenterDotSize(etu::StringToFloat(buffer, g_pRemember->getCenterDotSize()));

	m_CenterXPos.GetWindowText(buffer, 100);
	g_pRemember->setCenterXPos(etu::StringToFloat(buffer, g_pRemember->getCenterXPos()));

	m_CenterYPos.GetWindowText(buffer, 100);
	g_pRemember->setCenterYPos(etu::StringToFloat(buffer, g_pRemember->getCenterYPos()));

	m_StepSize.GetWindowText(buffer, 100);
	g_pRemember->setStepSize(etu::StringToFloat(buffer, g_pRemember->getStepSize()));

	m_NumStepsH.GetWindowText(buffer, 100);
	g_pRemember->setNumStepsH(etu::StringToInt(buffer, g_pRemember->getNumStepsH()));
	m_NumStepsV.GetWindowText(buffer, 100);
	g_pRemember->setNumStepsV(etu::StringToInt(buffer, g_pRemember->getNumStepsV()));

	m_PeriphDotSize.GetWindowText(buffer, 100);
	g_pRemember->setPeriphDotSize(etu::StringToFloat(buffer, g_pRemember->getPeriphDotSize()));

	m_FlashInterval.GetWindowText(buffer, 100);
	g_pRemember->setFlashInterval(etu::StringToInt(buffer, g_pRemember->getFlashInterval()));

	m_FlashDuration.GetWindowText(buffer, 100);
	g_pRemember->setFlashDuration(etu::StringToInt(buffer, g_pRemember->getFlashDuration()));

	g_pRemember->setBehavior(m_BehaviorCheck.GetCheck()==BST_CHECKED);

	g_pRemember->setAvoidZero(m_AvoidZero.GetCheck()==BST_CHECKED);

	m_MaxDelay.GetWindowText(buffer, 100);
	g_pRemember->setMaxDelay(etu::StringToInt(buffer, g_pRemember->getMaxDelay()));

	m_CenterPrimaryRed.GetWindowText(buffer, 100);
	g_pRemember->setCenterPrimaryRed(etu::StringToInt(buffer, g_pRemember->getCenterPrimaryRed()));

	m_CenterPrimaryGreen.GetWindowText(buffer, 100);
	g_pRemember->setCenterPrimaryGreen(etu::StringToInt(buffer, g_pRemember->getCenterPrimaryGreen()));

	m_CenterPrimaryBlue.GetWindowText(buffer, 100);
	g_pRemember->setCenterPrimaryBlue(etu::StringToInt(buffer, g_pRemember->getCenterPrimaryBlue()));

	m_CenterSecondaryRed.GetWindowText(buffer, 100);
	g_pRemember->setCenterSecondaryRed(etu::StringToInt(buffer, g_pRemember->getCenterSecondaryRed()));

	m_CenterSecondaryGreen.GetWindowText(buffer, 100);
	g_pRemember->setCenterSecondaryGreen(etu::StringToInt(buffer, g_pRemember->getCenterSecondaryGreen()));

	m_CenterSecondaryBlue.GetWindowText(buffer, 100);
	g_pRemember->setCenterSecondaryBlue(etu::StringToInt(buffer, g_pRemember->getCenterSecondaryBlue()));
	
	m_PeriphPrimaryRed.GetWindowText(buffer, 100);
	g_pRemember->setPeriphPrimaryRed(etu::StringToInt(buffer, g_pRemember->getPeriphPrimaryRed()));

	m_PeriphPrimaryGreen.GetWindowText(buffer, 100);
	g_pRemember->setPeriphPrimaryGreen(etu::StringToInt(buffer, g_pRemember->getPeriphPrimaryGreen()));

	m_PeriphPrimaryBlue.GetWindowText(buffer, 100);
	g_pRemember->setPeriphPrimaryBlue(etu::StringToInt(buffer, g_pRemember->getPeriphPrimaryBlue()));

	m_PeriphSecondaryRed.GetWindowText(buffer, 100);
	g_pRemember->setPeriphSecondaryRed(etu::StringToInt(buffer, g_pRemember->getPeriphSecondaryRed()));

	m_PeriphSecondaryGreen.GetWindowText(buffer, 100);
	g_pRemember->setPeriphSecondaryGreen(etu::StringToInt(buffer, g_pRemember->getPeriphSecondaryGreen()));

	m_PeriphSecondaryBlue.GetWindowText(buffer, 100);
	g_pRemember->setPeriphSecondaryBlue(etu::StringToInt(buffer, g_pRemember->getPeriphSecondaryBlue()));

	m_PostFlashDelay.GetWindowText(buffer, 100);
	g_pRemember->setPostFlashDelay(etu::StringToInt(buffer, g_pRemember->getPostFlashDelay()));

	m_IntertrialDelay.GetWindowText(buffer, 100);
	g_pRemember->setIntertrialDelay(etu::StringToInt(buffer, g_pRemember->getIntertrialDelay()));


	if (m_CenterRadioPrimary.GetCheck() == BST_CHECKED) {
		g_pRemember->setCenterColorRadio(CRemember::COLOR_PRIMARY);
	} 
	else if (m_CenterRadioSecondary.GetCheck() == BST_CHECKED) {
		g_pRemember->setCenterColorRadio(CRemember::COLOR_SECONDARY);
	} 
	else if (m_CenterRadioAlternate.GetCheck() == BST_CHECKED) {
		g_pRemember->setCenterColorRadio(CRemember::COLOR_ALTERNATE);
	} 
	else {
		g_pRemember->setCenterColorRadio(CRemember::COLOR_RANDOM);
	}

	if (m_PeriphRadioPrimary.GetCheck() == BST_CHECKED) {
		g_pRemember->setPeriphColorRadio(CRemember::COLOR_PRIMARY);
	} 
	else if (m_PeriphRadioSecondary.GetCheck() == BST_CHECKED) {
		g_pRemember->setPeriphColorRadio(CRemember::COLOR_SECONDARY);
	} 
	else if (m_PeriphRadioAlternate.GetCheck() == BST_CHECKED) {
		g_pRemember->setPeriphColorRadio(CRemember::COLOR_ALTERNATE);
	} 
	else if (m_PeriphRadioTertiary.GetCheck() == BST_CHECKED) {
		g_pRemember->setPeriphColorRadio(CRemember::COLOR_TERTIARY);
	} 
	else {
		g_pRemember->setPeriphColorRadio(CRemember::COLOR_RANDOM);
	}

	if (m_MemoryGuided.GetCheck() == BST_CHECKED) {
		g_pRemember->setStimType(CRemember::STIM_MEMORY_GUIDED);
	} 
	else {
		g_pRemember->setStimType(CRemember::STIM_PERIPHERAL_FLASH);
	}

	g_pRemember->setStepVert( m_StepVert.GetCheck() == BST_CHECKED);
	g_pRemember->setStepHoriz( m_StepHoriz.GetCheck() == BST_CHECKED);
	g_pRemember->setStep45( m_Step45.GetCheck() == BST_CHECKED);
	g_pRemember->setStep135( m_Step135.GetCheck() == BST_CHECKED);
	
	if (m_DistXY.GetCheck() == BST_CHECKED) {
		g_pRemember->setDistType(CRemember::DIST_TYPE_XY);
	} 
	else {
		g_pRemember->setDistType(CRemember::DIST_TYPE_RADIAL);
	}

	m_AlternateBlockSize.GetWindowText(buffer, 100);
	g_pRemember->setAlternateBlockSize(etu::StringToInt(buffer, g_pRemember->getAlternateBlockSize()));

	g_pRemember->setRedBlue( m_RedBlue.GetCheck() == BST_CHECKED);

	g_pRemember->setRandomizePostFlash( m_RandomizePostFlash.GetCheck() == BST_CHECKED);
	m_PostFlashRandMin.GetWindowText(buffer, 100);
	g_pRemember->setPostFlashRandMin(etu::StringToInt(buffer, g_pRemember->getPostFlashRandMin()));
	m_PostFlashRandMax.GetWindowText(buffer, 100);
	g_pRemember->setPostFlashRandMax(etu::StringToInt(buffer, g_pRemember->getPostFlashRandMax()));

	g_pRemember->setRandomizeFlashInterval( m_RandomizeFlashInterval.GetCheck() == BST_CHECKED);
	m_FlashIntervalRandMin.GetWindowText(buffer, 100);
	g_pRemember->setFlashIntervalRandMin(etu::StringToInt(buffer, g_pRemember->getFlashIntervalRandMin()));
	m_FlashIntervalRandMax.GetWindowText(buffer, 100);
	g_pRemember->setFlashIntervalRandMax(etu::StringToInt(buffer, g_pRemember->getFlashIntervalRandMax()));
	
	m_PeriphXPos.GetWindowText(buffer, 100);
	g_pRemember->setPeriphXPos(etu::StringToFloat(buffer, g_pRemember->getPeriphXPos()));

	m_PeriphYPos.GetWindowText(buffer, 100);
	g_pRemember->setPeriphYPos(etu::StringToFloat(buffer, g_pRemember->getPeriphYPos()));

	m_PeriphXRadius.GetWindowText(buffer, 100);
	g_pRemember->setPeriphXRadius(etu::StringToFloat(buffer, g_pRemember->getPeriphXRadius()));

	m_PeriphYRadius.GetWindowText(buffer, 100);
	g_pRemember->setPeriphYRadius(etu::StringToFloat(buffer, g_pRemember->getPeriphYRadius()));

	m_PctTimeAtPeriph.GetWindowText(buffer, 100);
	g_pRemember->setPctTimeAtPeriph(etu::StringToInt(buffer, g_pRemember->getPctTimeAtPeriph()));

	g_pRemember->setSeqRandom( m_CheckSeqRandom.GetCheck() == BST_CHECKED);
	
	m_PeriphTertiaryRed.GetWindowText(buffer, 100);
	g_pRemember->setPeriphTertiaryRed(etu::StringToInt(buffer, g_pRemember->getPeriphTertiaryRed()));
	m_PeriphTertiaryGreen.GetWindowText(buffer, 100);
	g_pRemember->setPeriphTertiaryGreen(etu::StringToInt(buffer, g_pRemember->getPeriphTertiaryGreen()));
	m_PeriphTertiaryBlue.GetWindowText(buffer, 100);
	g_pRemember->setPeriphTertiaryBlue(etu::StringToInt(buffer, g_pRemember->getPeriphTertiaryBlue()));
	m_NoiseRed.GetWindowText(buffer, 100);
	g_pRemember->setNoiseRed(etu::StringToInt(buffer, g_pRemember->getNoiseRed()));
	m_NoiseGreen.GetWindowText(buffer, 100);
	g_pRemember->setNoiseGreen(etu::StringToInt(buffer, g_pRemember->getNoiseGreen()));
	m_NoiseBlue.GetWindowText(buffer, 100);
	g_pRemember->setNoiseBlue(etu::StringToInt(buffer, g_pRemember->getNoiseBlue()));
	m_NumRandomRepeats.GetWindowText(buffer, 100);
	g_pRemember->setNumRandomRepeats(etu::StringToInt(buffer, g_pRemember->getNumRandomRepeats()));
	g_pRemember->setUseNoise( m_UseNoise.GetCheck() == BST_CHECKED);
	m_NumCatchTrials.GetWindowText(buffer, 100);
	g_pRemember->setNumCatchTrials(etu::StringToInt(buffer, g_pRemember->getNumCatchTrials()));

	g_pRemember->setGridSteps( m_GridSteps.GetCheck() == BST_CHECKED);

	m_NoiseSize.GetWindowText(buffer, 100);
	g_pRemember->setNoiseSize(etu::StringToInt(buffer, g_pRemember->getNoiseSize()));

	m_CrossPix.GetWindowText(buffer, 100);
	g_pRemember->setCrossPix(etu::StringToInt(buffer, g_pRemember->getCrossPix()));

	m_NumBlankTrials.GetWindowText(buffer, 100);
	g_pRemember->setNumBlankTrials(etu::StringToInt(buffer, g_pRemember->getNumBlankTrials()));

	m_NoisePct.GetWindowText(buffer, 100);
	g_pRemember->setNoisePct(etu::StringToInt(buffer, g_pRemember->getNoisePct()));

	g_pRemember->setUseDim( m_UseDim.GetCheck() == BST_CHECKED);

	m_DimTimeout.GetWindowText(buffer, 100);
	g_pRemember->setDimTimeout(etu::StringToInt(buffer, g_pRemember->getDimTimeout()));

	m_DimOnsetMin.GetWindowText(buffer, 100);
	g_pRemember->setDimOnsetMin(etu::StringToInt(buffer, g_pRemember->getDimOnsetMin()));

	m_DimOnsetMax.GetWindowText(buffer, 100);
	g_pRemember->setDimOnsetMax(etu::StringToInt(buffer, g_pRemember->getDimOnsetMax()));
	
	m_AnswerPeriod.GetWindowText(buffer, 100);
	g_pRemember->setAnswerPeriod(etu::StringToInt(buffer, g_pRemember->getAnswerPeriod()));

	m_DimBrightness.GetWindowText(buffer, 100);
	g_pRemember->setDimBrightness(etu::StringToInt(buffer, g_pRemember->getDimBrightness()));

	g_pRemember->setHideCenter( m_HideCenter.GetCheck() == BST_CHECKED);
	g_pRemember->setInterleaveNoise( m_InterleaveNoise.GetCheck() == BST_CHECKED);
	g_pRemember->setCenterPoints( m_CenterPoints.GetCheck() == BST_CHECKED);

	g_pRemember->setOffAxis( m_OffAxis.GetCheck() == BST_CHECKED);
	g_pRemember->setResponse( m_Response.GetCheck() == BST_CHECKED);

	m_PeriphXPosL.GetWindowText(buffer, 100);
	g_pRemember->setPeriphXPosL(etu::StringToFloat(buffer, g_pRemember->getPeriphXPosL()));

	m_PeriphYPosL.GetWindowText(buffer, 100);
	g_pRemember->setPeriphYPosL(etu::StringToFloat(buffer, g_pRemember->getPeriphYPosL()));

	m_ResponseTimeout.GetWindowText(buffer, 100);
	g_pRemember->setResponseTimeout(etu::StringToInt(buffer, g_pRemember->getResponseTimeout()));

	g_pRemember->setRampUp( m_RampUp.GetCheck() == BST_CHECKED);

	m_RampDuration.GetWindowText(buffer, 100);
	g_pRemember->setRampDuration(etu::StringToInt(buffer, g_pRemember->getRampDuration()));

	m_PctTimeAtCenter.GetWindowText(buffer, 100);
	g_pRemember->setPctTimeAtCenter(etu::StringToInt(buffer, g_pRemember->getPctTimeAtCenter()));

	m_NumCatchTrials2.GetWindowText(buffer, 100);
	g_pRemember->setNumCatchTrials2(etu::StringToInt(buffer, g_pRemember->getNumCatchTrials2()));

	m_CenterXRadius.GetWindowText(buffer, 100);
	g_pRemember->setCenterXRadius(etu::StringToFloat(buffer, g_pRemember->getCenterXRadius()));

	m_CenterYRadius.GetWindowText(buffer, 100);
	g_pRemember->setCenterYRadius(etu::StringToFloat(buffer, g_pRemember->getCenterYRadius()));

	g_pRemember->setAutoPosition( m_AutoPosition.GetCheck() == BST_CHECKED);

	//send text mark
	g_pRemember->sendSettings(g_pSettings->theSerialComm);
}

void CRememberDlg::OnPaint()
{
	CDialog::OnPaint();

	CRect rect, r;

	CPaintDC dc_cp(&m_CenterPrimaryStatic);
	m_CenterPrimaryStatic.GetClientRect(rect);
	dc_cp.FillSolidRect(rect, RGB(g_pRemember->getCenterPrimaryRed(),g_pRemember->getCenterPrimaryGreen(),g_pRemember->getCenterPrimaryBlue()));

	CPaintDC dc_cs(&m_CenterSecondaryStatic);
	m_CenterSecondaryStatic.GetClientRect(rect);
	dc_cs.FillSolidRect(rect, RGB(g_pRemember->getCenterSecondaryRed(),g_pRemember->getCenterSecondaryGreen(),g_pRemember->getCenterSecondaryBlue()));

	CPaintDC dc_pp(&m_PeriphPrimaryStatic);
	m_PeriphPrimaryStatic.GetClientRect(rect);
	dc_pp.FillSolidRect(rect, RGB(g_pRemember->getPeriphPrimaryRed(),g_pRemember->getPeriphPrimaryGreen(),g_pRemember->getPeriphPrimaryBlue()));

	CPaintDC dc_ps(&m_PeriphSecondaryStatic);
	m_PeriphSecondaryStatic.GetClientRect(rect);
	dc_ps.FillSolidRect(rect, RGB(g_pRemember->getPeriphSecondaryRed(),g_pRemember->getPeriphSecondaryGreen(),g_pRemember->getPeriphSecondaryBlue()));

	CPaintDC dc_pt(&m_PeriphTertiaryStatic);
	m_PeriphTertiaryStatic.GetClientRect(rect);
	dc_pt.FillSolidRect(rect, RGB(g_pRemember->getPeriphTertiaryRed(),g_pRemember->getPeriphTertiaryGreen(),g_pRemember->getPeriphTertiaryBlue()));

	CPaintDC dc_n(&m_NoiseStatic);
	m_NoiseStatic.GetClientRect(rect);
	dc_n.FillSolidRect(rect, RGB(g_pRemember->getNoiseRed(),g_pRemember->getNoiseGreen(),g_pRemember->getNoiseBlue()));
	
	if (le_r != NULL && le_b != NULL && le_o != NULL &&
		le_red_r != NULL && le_red_b != NULL && le_red_o != NULL &&
		le_blue_r != NULL && le_blue_b != NULL && le_blue_o != NULL) {
		CPaintDC dc_le(&m_LeftEyeStatic);
		m_LeftEyeStatic.GetClientRect(rect);
		int draww = rect.right - rect.left;
		int drawh = rect.top - rect.bottom;
		int stephsize = 1 + 2*g_pRemember->getNumStepsH();
		int stepvsize = 1 + 2*g_pRemember->getNumStepsV();
		for (int i=0; i<stephsize; i++) {
			for (int j=0; j<stepvsize; j++) {
				r.top = rect.bottom + drawh * j / stepvsize;
				r.bottom = rect.bottom + drawh * (j+1) / stepvsize;
				r.left = rect.left + draww * i / stephsize;
				r.right = rect.left+ draww * (i+1) / stephsize;
				int total;
				if (g_pRememberDlg->SendDlgItemMessage(IDC_ViewPurple, BM_GETCHECK,0,0)==BST_CHECKED) {
					total = g_pRememberDlg->le_r[i*stepvsize+j] + g_pRememberDlg->le_b[i*stepvsize+j] + g_pRememberDlg->le_o[i*stepvsize+j];
				} 
				else if (g_pRememberDlg->SendDlgItemMessage(IDC_ViewRed, BM_GETCHECK,0,0)==BST_CHECKED) {
					total = g_pRememberDlg->le_red_r[i*stepvsize+j] + g_pRememberDlg->le_red_b[i*stepvsize+j] + g_pRememberDlg->le_red_o[i*stepvsize+j];
				} 
				else {
					total = g_pRememberDlg->le_blue_r[i*stepvsize+j] + g_pRememberDlg->le_blue_b[i*stepvsize+j] + g_pRememberDlg->le_blue_o[i*stepvsize+j];
				}

				if (total == 0) {
					dc_le.FillSolidRect(r, RGB(0, 0, 0));
				} 
				else {
					if (g_pRememberDlg->SendDlgItemMessage(IDC_ViewPurple, BM_GETCHECK,0,0)==BST_CHECKED) {
						dc_le.FillSolidRect(r, RGB(
							255 * g_pRememberDlg->le_r[i*stepvsize+j] / total,
							255 * g_pRememberDlg->le_o[i*stepvsize+j] / total,
							255 * g_pRememberDlg->le_b[i*stepvsize+j] / total));
					} 
					else if (g_pRememberDlg->SendDlgItemMessage(IDC_ViewRed, BM_GETCHECK,0,0)==BST_CHECKED) {
						dc_le.FillSolidRect(r, RGB(
							255 * g_pRememberDlg->le_red_r[i*stepvsize+j] / total,
							255 * g_pRememberDlg->le_red_o[i*stepvsize+j] / total,
							255 * g_pRememberDlg->le_red_b[i*stepvsize+j] / total));
					} 
					else {
						dc_le.FillSolidRect(r, RGB(
							255 * g_pRememberDlg->le_blue_r[i*stepvsize+j] / total,
							255 * g_pRememberDlg->le_blue_o[i*stepvsize+j] / total,
							255 * g_pRememberDlg->le_blue_b[i*stepvsize+j] / total));
					}
				}
			}
		}
		dc_le.FillSolidRect(rect.left + draww/2-1, rect.bottom + drawh/2-1, 2, 2, RGB(255,255,255));
	}

	if (re_r != NULL && re_b != NULL && re_o != NULL &&
		re_red_r != NULL && re_red_b != NULL && re_red_o != NULL &&
		re_blue_r != NULL && re_blue_b != NULL && re_blue_o != NULL) {
		CPaintDC dc_re(&m_RightEyeStatic);
		m_RightEyeStatic.GetClientRect(rect);
		int draww = rect.right - rect.left;
		int drawh = rect.top - rect.bottom;
		int stephsize = 1 + 2*g_pRemember->getNumStepsH();
		int stepvsize = 1 + 2*g_pRemember->getNumStepsV();
		for (int i=0; i<stephsize; i++) {
			for (int j=0; j<stepvsize; j++) {
				r.top = rect.bottom + drawh * j / stepvsize;
				r.bottom = rect.bottom + drawh * (j+1) / stepvsize;
				r.left = rect.left + draww * i / stephsize;
				r.right = rect.left+ draww * (i+1) / stephsize;
				int total;
				if (g_pRememberDlg->SendDlgItemMessage(IDC_ViewPurple, BM_GETCHECK,0,0)==BST_CHECKED) {
					total = g_pRememberDlg->re_r[i*stepvsize+j] + g_pRememberDlg->re_b[i*stepvsize+j] + g_pRememberDlg->re_o[i*stepvsize+j];
				} 
				else if (g_pRememberDlg->SendDlgItemMessage(IDC_ViewRed, BM_GETCHECK,0,0)==BST_CHECKED) {
					total = g_pRememberDlg->re_red_r[i*stepvsize+j] + g_pRememberDlg->re_red_b[i*stepvsize+j] + g_pRememberDlg->re_red_o[i*stepvsize+j];
				} 
				else {
					total = g_pRememberDlg->re_blue_r[i*stepvsize+j] + g_pRememberDlg->re_blue_b[i*stepvsize+j] + g_pRememberDlg->re_blue_o[i*stepvsize+j];
				}

				if (total == 0) {
					dc_re.FillSolidRect(r, RGB(0, 0, 0));
				} 
				else {
					if (g_pRememberDlg->SendDlgItemMessage(IDC_ViewPurple, BM_GETCHECK,0,0)==BST_CHECKED) {
						dc_re.FillSolidRect(r, RGB(
							255 * g_pRememberDlg->re_r[i*stepvsize+j] / total,
							255 * g_pRememberDlg->re_o[i*stepvsize+j] / total,
							255 * g_pRememberDlg->re_b[i*stepvsize+j] / total));
					} 
					else if (g_pRememberDlg->SendDlgItemMessage(IDC_ViewRed, BM_GETCHECK,0,0)==BST_CHECKED) {
						dc_re.FillSolidRect(r, RGB(
							255 * g_pRememberDlg->re_red_r[i*stepvsize+j] / total,
							255 * g_pRememberDlg->re_red_o[i*stepvsize+j] / total,
							255 * g_pRememberDlg->re_red_b[i*stepvsize+j] / total));
					} 
					else {
						dc_re.FillSolidRect(r, RGB(
							255 * g_pRememberDlg->re_blue_r[i*stepvsize+j] / total,
							255 * g_pRememberDlg->re_blue_o[i*stepvsize+j] / total,
							255 * g_pRememberDlg->re_blue_b[i*stepvsize+j] / total));
					}
				}
			}
		}
		dc_re.FillSolidRect(rect.left + draww/2-1, rect.bottom + drawh/2-1, 2, 2, RGB(255,255,255));
	}
}

void CRememberDlg::OnBnClickedButtonapply()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "Human Suppression Scotomas") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
	g_pRememberDlg->Invalidate();
	
}

void CRememberDlg::OnBnClickedOk()
{	
	OnBnClickedButtonapply();
}

void CRememberDlg::OnBnClickedCancel()
{}

void CRememberDlg::OnBnClickedBehavior()
{
	//behavior contingency check box
	if (m_BehaviorCheck.GetCheck()==BST_CHECKED) {
		//grey out flash interval period box
		m_RandomizeFlashInterval.EnableWindow(false);
		m_FlashIntervalRandMin.EnableWindow(false);
		m_FlashIntervalRandMax.EnableWindow(false);
		m_FlashInterval.EnableWindow(false);

		//enable max duration box
		m_MaxDelay.EnableWindow(true);
	} 
	else {
		//enable flash interval period
		if (m_MemoryGuided.GetCheck()==BST_CHECKED) {
			m_RandomizeFlashInterval.EnableWindow(true);
			m_FlashIntervalRandMin.EnableWindow(true);
			m_FlashIntervalRandMax.EnableWindow(true);
			m_FlashInterval.EnableWindow(true);
			OnBnClickedRandomizeflashinterval();
		} 
		else {
			m_RandomizeFlashInterval.EnableWindow(false);
			m_FlashIntervalRandMin.EnableWindow(false);
			m_FlashIntervalRandMax.EnableWindow(false);
			m_FlashInterval.EnableWindow(false);
		}

		//disable max duration box
		m_MaxDelay.EnableWindow(false);
	}

	//(dis/en)able post flash randomize thing
	if (m_MemoryGuided.GetCheck()==BST_CHECKED) {
		m_RandomizePostFlash.EnableWindow(true);
		m_PostFlashRandMin.EnableWindow(true);
		m_PostFlashRandMax.EnableWindow(true);
		m_PostFlashDelay.EnableWindow(true);
		OnBnClickedRandomizepostflash();
	} 
	else {
		m_RandomizePostFlash.EnableWindow(false);
		m_PostFlashRandMin.EnableWindow(false);
		m_PostFlashRandMax.EnableWindow(false);
		m_PostFlashDelay.EnableWindow(false);
	}
}

void CRememberDlg::OnBnClickedCenterradioprimary()
{
	m_CenterRadio = CRemember::COLOR_PRIMARY;
	g_pRemember->setCenterColorRadio(CRemember::COLOR_PRIMARY);

	m_CenterRadioPrimary.SetCheck(BST_CHECKED);
	m_CenterRadioSecondary.SetCheck(BST_UNCHECKED);
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
}

void CRememberDlg::OnBnClickedCenterradiosecondary()
{
	m_CenterRadio = CRemember::COLOR_SECONDARY;
	g_pRemember->setCenterColorRadio(CRemember::COLOR_SECONDARY);

	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_CHECKED);
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
}

void CRememberDlg::OnBnClickedCenterradioalternate()
{
	m_CenterRadio = CRemember::COLOR_ALTERNATE;
	g_pRemember->setCenterColorRadio(CRemember::COLOR_ALTERNATE);

	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_UNCHECKED);
	m_CenterRadioAlternate.SetCheck(BST_CHECKED);
	m_CenterRadioRandomize.SetCheck(BST_UNCHECKED);
}

void CRememberDlg::OnBnClickedCenterradiorandomize()
{
	m_CenterRadio = CRemember::COLOR_RANDOM;
	g_pRemember->setCenterColorRadio(CRemember::COLOR_RANDOM);

	m_CenterRadioPrimary.SetCheck(BST_UNCHECKED);
	m_CenterRadioSecondary.SetCheck(BST_UNCHECKED);
	m_CenterRadioAlternate.SetCheck(BST_UNCHECKED);
	m_CenterRadioRandomize.SetCheck(BST_CHECKED);
}

void CRememberDlg::OnBnClickedPeriphradioprimary()
{
	m_PeriphRadio = CRemember::COLOR_PRIMARY;
	g_pRemember->setPeriphColorRadio(CRemember::COLOR_PRIMARY);

	m_PeriphRadioPrimary.SetCheck(BST_CHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_UNCHECKED);
}

void CRememberDlg::OnBnClickedPeriphradiosecondary()
{
	m_PeriphRadio = CRemember::COLOR_SECONDARY;
	g_pRemember->setPeriphColorRadio(CRemember::COLOR_SECONDARY);

	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_CHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_UNCHECKED);
}

void CRememberDlg::OnBnClickedPeriphradioalternate()
{
	m_PeriphRadio = CRemember::COLOR_ALTERNATE;
	g_pRemember->setPeriphColorRadio(CRemember::COLOR_ALTERNATE);

	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_CHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_UNCHECKED);
}

void CRememberDlg::OnBnClickedPeriphradiorandomize()
{
	m_PeriphRadio = CRemember::COLOR_RANDOM;
	g_pRemember->setPeriphColorRadio(CRemember::COLOR_RANDOM);

	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_CHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_UNCHECKED);
}

void CRememberDlg::OnBnClickedPeriphradiotertiary()
{
	m_PeriphRadio = CRemember::COLOR_TERTIARY;
	g_pRemember->setPeriphColorRadio(CRemember::COLOR_TERTIARY);

	m_PeriphRadioPrimary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioSecondary.SetCheck(BST_UNCHECKED);
	m_PeriphRadioAlternate.SetCheck(BST_UNCHECKED);
	m_PeriphRadioRandomize.SetCheck(BST_UNCHECKED);
	m_PeriphRadioTertiary.SetCheck(BST_CHECKED);
}

void CRememberDlg::OnBnClickedButton1()
{
	//check all step orientation boxes
	m_StepVert.SetCheck(BST_CHECKED);
	m_StepHoriz.SetCheck(BST_CHECKED);
	m_Step45.SetCheck(BST_CHECKED);
	m_Step135.SetCheck(BST_CHECKED);
}

void CRememberDlg::OnBnClickedDistradial()
{
	m_DistXY.SetCheck(BST_UNCHECKED);
	m_DistRadial.SetCheck(BST_CHECKED);
	m_DistType = CRemember::DIST_TYPE_RADIAL;
}

void CRememberDlg::OnBnClickedDistxy()
{
	m_DistXY.SetCheck(BST_CHECKED);
	m_DistRadial.SetCheck(BST_UNCHECKED);
	m_DistType = CRemember::DIST_TYPE_XY;
}

void CRememberDlg::OnBnClickedMemoryGuided()
{
	m_MemoryGuided.SetCheck(BST_CHECKED);
	m_PeripheralFlash.SetCheck(BST_UNCHECKED);

	OnBnClickedBehavior();
}

void CRememberDlg::OnBnClickedPeripheralFlash()
{
	m_MemoryGuided.SetCheck(BST_UNCHECKED);
	m_PeripheralFlash.SetCheck(BST_CHECKED);

	OnBnClickedBehavior();
}



void CRememberDlg::OnBnClickedRedblue()
{
	if (m_RedBlue.GetCheck()==BST_CHECKED) {
		m_CenterPrimaryRed.EnableWindow(true);
		m_CenterPrimaryRed.SetWindowText(etu::IntToString(255));
		m_CenterPrimaryGreen.EnableWindow(false);
		m_CenterPrimaryGreen.SetWindowText(etu::IntToString(0));
		m_CenterPrimaryBlue.EnableWindow(false);
		m_CenterPrimaryBlue.SetWindowText(etu::IntToString(0));
		m_CenterSecondaryRed.EnableWindow(false);
		m_CenterSecondaryRed.SetWindowText(etu::IntToString(0));
		m_CenterSecondaryGreen.EnableWindow(false);
		m_CenterSecondaryGreen.SetWindowText(etu::IntToString(0));
		m_CenterSecondaryBlue.EnableWindow(true);
		m_CenterSecondaryBlue.SetWindowText(etu::IntToString(255));
		m_PeriphPrimaryRed.EnableWindow(true);
		m_PeriphPrimaryRed.SetWindowText(etu::IntToString(255));
		m_PeriphPrimaryGreen.EnableWindow(false);
		m_PeriphPrimaryGreen.SetWindowText(etu::IntToString(0));
		m_PeriphPrimaryBlue.EnableWindow(false);
		m_PeriphPrimaryBlue.SetWindowText(etu::IntToString(0));
		m_PeriphSecondaryRed.EnableWindow(false);
		m_PeriphSecondaryRed.SetWindowText(etu::IntToString(0));
		m_PeriphSecondaryGreen.EnableWindow(false);
		m_PeriphSecondaryGreen.SetWindowText(etu::IntToString(0));
		m_PeriphSecondaryBlue.EnableWindow(true);
		m_PeriphSecondaryBlue.SetWindowText(etu::IntToString(255));
	} 
	else {
		m_CenterPrimaryRed.EnableWindow(true);
		m_CenterPrimaryGreen.EnableWindow(true);
		m_CenterPrimaryBlue.EnableWindow(true);
		m_CenterSecondaryRed.EnableWindow(true);
		m_CenterSecondaryGreen.EnableWindow(true);
		m_CenterSecondaryBlue.EnableWindow(true);
		m_PeriphPrimaryRed.EnableWindow(true);
		m_PeriphPrimaryGreen.EnableWindow(true);
		m_PeriphPrimaryBlue.EnableWindow(true);
		m_PeriphSecondaryRed.EnableWindow(true);
		m_PeriphSecondaryGreen.EnableWindow(true);
		m_PeriphSecondaryBlue.EnableWindow(true);
	}
}

void CRememberDlg::OnBnClickedRandomizepostflash()
{
	m_RandomizePostFlash.EnableWindow(true);
	if (m_RandomizePostFlash.GetCheck() == BST_CHECKED) {
		m_PostFlashRandMin.EnableWindow(true);
		m_PostFlashRandMax.EnableWindow(true);
		m_PostFlashDelay.EnableWindow(false);
	} 
	else {
		m_PostFlashRandMin.EnableWindow(false);
		m_PostFlashRandMax.EnableWindow(false);
		m_PostFlashDelay.EnableWindow(true);
	}
}

void CRememberDlg::OnBnClickedRandomizeflashinterval()
{
	m_RandomizeFlashInterval.EnableWindow(true);
	if (m_RandomizeFlashInterval.GetCheck() == BST_CHECKED) {
		m_FlashIntervalRandMin.EnableWindow(true);
		m_FlashIntervalRandMax.EnableWindow(true);
		m_FlashInterval.EnableWindow(false);
	} 
	else {
		m_FlashIntervalRandMin.EnableWindow(false);
		m_FlashIntervalRandMax.EnableWindow(false);
		m_FlashInterval.EnableWindow(true);
	}
}

void CRememberDlg::OnBnClickedCheckSeqRandom()
{
	m_NumRandomRepeats.EnableWindow(m_CheckSeqRandom.GetCheck()==BST_CHECKED);
}

void CRememberDlg::OnBnClickedUsenoise()
{
	m_NoiseSize.EnableWindow(m_UseNoise.GetCheck()==BST_CHECKED);
	m_NoisePct.EnableWindow(m_UseNoise.GetCheck()==BST_CHECKED);
	m_NoiseRed.EnableWindow(m_UseNoise.GetCheck()==BST_CHECKED);
	m_NoiseGreen.EnableWindow(m_UseNoise.GetCheck()==BST_CHECKED);
	m_NoiseBlue.EnableWindow(m_UseNoise.GetCheck()==BST_CHECKED);
	m_InterleaveNoise.EnableWindow(m_UseNoise.GetCheck()==BST_CHECKED);
}

void CRememberDlg::OnBnClickedUsedim()
{
	m_DimTimeout.EnableWindow(m_UseDim.GetCheck()==BST_CHECKED);
	m_DimBrightness.EnableWindow(m_UseDim.GetCheck()==BST_CHECKED);
	m_AnswerPeriod.EnableWindow(m_UseDim.GetCheck()==BST_CHECKED);
	m_DimOnsetMin.EnableWindow(m_UseDim.GetCheck()==BST_CHECKED);
	m_DimOnsetMax.EnableWindow(m_UseDim.GetCheck()==BST_CHECKED);
}


void CRememberDlg::OnBnClickedButton2()
{
	g_pRemember->GenerateLocations2();
}

void CRememberDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	g_pRemember->GenerateCatchTrials();
}

void CRememberDlg::OnBnClickedViewpurple()
{
	g_pRememberDlg->Invalidate();
}

void CRememberDlg::OnBnClickedViewred()
{
	g_pRememberDlg->Invalidate();
}

void CRememberDlg::OnBnClickedViewblue()
{
	g_pRememberDlg->Invalidate();
}

#endif