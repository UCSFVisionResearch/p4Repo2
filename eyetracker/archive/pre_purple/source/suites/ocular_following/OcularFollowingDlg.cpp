/**
 * @file OcularFollowingDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "OcularFollowingDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(COcularFollowingDlg, CDialog)
COcularFollowingDlg::COcularFollowingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COcularFollowingDlg::IDD, pParent)
{}

COcularFollowingDlg::~COcularFollowingDlg()
{}

void COcularFollowingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BKG_SPEED, m_BkgSpeed);
	DDX_Control(pDX, IDC_POST_SACCADE_LAT, m_Latency);
	DDX_Control(pDX, IDC_BKG_STEP_DURATION, m_Bkg_StepDuration);
	DDX_Control(pDX, IDC_TIMEOUT, m_TimeOut);
	DDX_Control(pDX, IDC_FX_X_POS, m_FixationXPos);
	DDX_Control(pDX, IDC_FX_Y_POS, m_FixationYPos);
	DDX_Control(pDX, IDC_FX_SIZE, m_FixationSize);
	DDX_Control(pDX, IDC_CheckUp, m_CheckUp);
	DDX_Control(pDX, IDC_CheckDown, m_CheckDown);
	DDX_Control(pDX, IDC_CheckLeft, m_CheckLeft);
	DDX_Control(pDX, IDC_CheckRight, m_CheckRight);
	DDX_Control(pDX, IDC_BEHAVIOR, m_Behavior);
	DDX_Control(pDX, IDC_MinRadius, m_MinRadius);
	DDX_Control(pDX, IDC_MaxRadius, m_MaxRadius);
	DDX_Control(pDX, IDC_Random, m_Random);
	DDX_Control(pDX, IDC_NoiseSize, m_NoiseSize);
	DDX_Control(pDX, IDC_NoisePct, m_NoisePct);
	DDX_Control(pDX, IDC_NoiseContrast, m_NoiseContrast);
}

BEGIN_MESSAGE_MAP(COcularFollowingDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDC_ButtonAll, OnBnClickedButtonall)
	ON_BN_CLICKED(IDC_ButtonNone, OnBnClickedButtonnone)
END_MESSAGE_MAP()

BOOL COcularFollowingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COcularFollowingDlg::load() 
{
	m_FixationSize.SetWindowText(etu::FloatToString(g_pOcularFollowing->getSize()));
	m_FixationXPos.SetWindowText(etu::FloatToString(g_pOcularFollowing->getXPosition()));
	m_FixationYPos.SetWindowText(etu::FloatToString(g_pOcularFollowing->getYPosition()));
	m_TimeOut.SetWindowText(etu::IntToString(g_pOcularFollowing->getTimeOut()));
	m_Bkg_StepDuration.SetWindowText(etu::IntToString(g_pOcularFollowing->getBackgroundDuration()));
	m_BkgSpeed.SetWindowText(etu::IntToString(g_pOcularFollowing->getBackgroundSpeed()));
	m_Latency.SetWindowText(etu::IntToString(g_pOcularFollowing->getLatency()));
	
	if (g_pOcularFollowing->getDirectionUp()) {
		m_CheckUp.SetCheck(BST_CHECKED);
	} 
	else {
		m_CheckUp.SetCheck(BST_UNCHECKED);
	}

	if (g_pOcularFollowing->getDirectionDown()) {
		m_CheckDown.SetCheck(BST_CHECKED);
	} 
	else {
		m_CheckDown.SetCheck(BST_UNCHECKED);
	}

	if (g_pOcularFollowing->getDirectionLeft()) {
		m_CheckLeft.SetCheck(BST_CHECKED);
	} 
	else {
		m_CheckLeft.SetCheck(BST_UNCHECKED);
	}

	if (g_pOcularFollowing->getDirectionRight()) {
		m_CheckRight.SetCheck(BST_CHECKED);
	} 
	else {
		m_CheckRight.SetCheck(BST_UNCHECKED);
	}

	m_Behavior.SetCheck((g_pOcularFollowing->getBehavior())? BST_CHECKED : BST_UNCHECKED);
	m_MinRadius.SetWindowText(etu::FloatToString(g_pOcularFollowing->getMinRadius()));
	m_MaxRadius.SetWindowText(etu::FloatToString(g_pOcularFollowing->getMaxRadius()));

	m_Random.SetCheck((g_pOcularFollowing->getRandom())? BST_CHECKED : BST_UNCHECKED);

	m_NoiseSize.SetWindowText(etu::IntToString(g_pOcularFollowing->getNoiseSize()));
	m_NoisePct.SetWindowText(etu::IntToString(g_pOcularFollowing->getNoisePct()));
	m_NoiseContrast.SetWindowText(etu::IntToString(g_pOcularFollowing->getNoiseContrast()));
}

void COcularFollowingDlg::save() 
{
	char buffer[100];

	m_FixationSize.GetWindowText(buffer, 100);
	g_pOcularFollowing->setSize(etu::StringToFloat(buffer, g_pOcularFollowing->getSize()));

	m_FixationXPos.GetWindowText(buffer, 100);
	g_pOcularFollowing->setXPosition(etu::StringToFloat(buffer, g_pOcularFollowing->getXPosition()));

	m_FixationYPos.GetWindowText(buffer, 100);
	g_pOcularFollowing->setYPosition(etu::StringToFloat(buffer, g_pOcularFollowing->getYPosition()));

	m_TimeOut.GetWindowText(buffer, 100);
	g_pOcularFollowing->setTimeOut(etu::StringToInt(buffer, g_pOcularFollowing->getTimeOut()));

	m_Bkg_StepDuration.GetWindowText(buffer, 100);
	g_pOcularFollowing->setBackgroundDuration(etu::StringToInt(buffer, g_pOcularFollowing->getBackgroundDuration()));

	m_BkgSpeed.GetWindowText(buffer, 100);
	g_pOcularFollowing->setBackgroundSpeed(etu::StringToInt(buffer, g_pOcularFollowing->getBackgroundSpeed()));

	m_Latency.GetWindowText(buffer, 100);
	g_pOcularFollowing->setLatency(etu::StringToInt(buffer, g_pOcularFollowing->getLatency()));

	g_pOcularFollowing->setDirectionUp(m_CheckUp.GetCheck() ==BST_CHECKED);
	g_pOcularFollowing->setDirectionDown(m_CheckDown.GetCheck() ==BST_CHECKED);
	g_pOcularFollowing->setDirectionLeft(m_CheckLeft.GetCheck() ==BST_CHECKED);
	g_pOcularFollowing->setDirectionRight(m_CheckRight.GetCheck() ==BST_CHECKED);
	
	g_pOcularFollowing->setBehavior(m_Behavior.GetCheck() ==BST_CHECKED);
	m_MinRadius.GetWindowText(buffer, 100);
	g_pOcularFollowing->setMinRadius(etu::StringToFloat(buffer, g_pOcularFollowing->getMinRadius()));
	m_MaxRadius.GetWindowText(buffer, 100);
	g_pOcularFollowing->setMaxRadius(etu::StringToFloat(buffer, g_pOcularFollowing->getMaxRadius()));

	g_pOcularFollowing->setRandom(m_Random.GetCheck() ==BST_CHECKED);

	m_NoiseSize.GetWindowText(buffer, 100);
	g_pOcularFollowing->setNoiseSize(etu::StringToInt(buffer, g_pOcularFollowing->getNoiseSize()));

	m_NoisePct.GetWindowText(buffer, 100);
	g_pOcularFollowing->setNoisePct(etu::StringToInt(buffer, g_pOcularFollowing->getNoisePct()));

	m_NoiseContrast.GetWindowText(buffer, 100);
	g_pOcularFollowing->setNoiseContrast(etu::StringToInt(buffer, g_pOcularFollowing->getNoiseContrast()));

	g_pOcularFollowing->sendSettings(g_pSettings->theSerialComm);
}

void COcularFollowingDlg::OnBnClickedOk()
{
	OnBnClickedApply();
}

void COcularFollowingDlg::OnBnClickedCancel()
{}

void COcularFollowingDlg::OnBnClickedApply()
{
	bool bWasActive = false;
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() == "Ocular Following Stimulus") {
		bWasActive = true;
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
}

void COcularFollowingDlg::OnBnClickedButtonall()
{
	m_CheckUp.SetCheck(BST_CHECKED);
	m_CheckDown.SetCheck(BST_CHECKED);
	m_CheckLeft.SetCheck(BST_CHECKED);
	m_CheckRight.SetCheck(BST_CHECKED);
}

void COcularFollowingDlg::OnBnClickedButtonnone()
{
	m_CheckUp.SetCheck(BST_UNCHECKED);
	m_CheckDown.SetCheck(BST_UNCHECKED);
	m_CheckLeft.SetCheck(BST_UNCHECKED);
	m_CheckRight.SetCheck(BST_UNCHECKED);
}


#endif