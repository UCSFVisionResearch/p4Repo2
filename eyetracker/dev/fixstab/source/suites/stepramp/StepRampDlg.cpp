/**
 * @file StepRampDlg.cpp
 * @brief StepRampDlg.cpp : implementation file
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "steprampdlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CStepRampDlg, CDialog)
CStepRampDlg::CStepRampDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStepRampDlg::IDD, pParent)
{}

CStepRampDlg::~CStepRampDlg() {}

void CStepRampDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EditDotSize, m_DotSize);
	DDX_Control(pDX, IDC_EditStepSize, m_StepSize);
	DDX_Control(pDX, IDC_EditVelocity, m_Velocity);
	DDX_Control(pDX, IDC_EditExcursion, m_Excursion);
	DDX_Control(pDX, IDC_EditDelay, m_Delay);
	DDX_Control(pDX, IDC_EditPlateau, m_Plateau);
	DDX_Control(pDX, IDC_EditOffDelay, m_OffDelay);
	DDX_Control(pDX, IDC_BUTTON1, m_Apply);
	DDX_Control(pDX, IDC_CheckUp, m_CheckUp);
	DDX_Control(pDX, IDC_CheckDown, m_CheckDown);
	DDX_Control(pDX, IDC_CheckLeft, m_CheckLeft);
	DDX_Control(pDX, IDC_CheckRight, m_CheckRight);
	DDX_Control(pDX, IDC_Check_Behavior, m_Behavior);
	DDX_Control(pDX, IDC_StartXUp, m_StartXUp);
	DDX_Control(pDX, IDC_StartYUp, m_StartYUp);
	DDX_Control(pDX, IDC_StartXDown, m_StartXDown);
	DDX_Control(pDX, IDC_StartYDown, m_StartYDown);
	DDX_Control(pDX, IDC_StartXLeft, m_StartXLeft);
	DDX_Control(pDX, IDC_StartYLeft, m_StartYLeft);
	DDX_Control(pDX, IDC_StartXRight, m_StartXRight);
	DDX_Control(pDX, IDC_StartYRight, m_StartYRight);
}

BEGIN_MESSAGE_MAP(CStepRampDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CheckUp, OnBnClickedCheckup)
	ON_BN_CLICKED(IDC_CheckLeft, OnBnClickedCheckleft)
	ON_BN_CLICKED(IDC_CheckDown, OnBnClickedCheckdown)
	ON_BN_CLICKED(IDC_CheckRight, OnBnClickedCheckright)
	ON_BN_CLICKED(IDC_ButtonAll, OnBnClickedButtonall)
	ON_BN_CLICKED(IDC_ButtonNone, OnBnClickedButtonnone)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_Check_Behavior, OnBnClickedCheckBehavior)
END_MESSAGE_MAP()

// CStepRampDlg message handlers
BOOL CStepRampDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStepRampDlg::load() 
{
	m_DotSize.SetWindowText(etu::FloatToString(g_pStepRamp->getDotSize()));
	m_StepSize.SetWindowText(etu::FloatToString(g_pStepRamp->getStepSize()));
	m_Velocity.SetWindowText(etu::FloatToString(g_pStepRamp->getVelocity()));
	m_Excursion.SetWindowText(etu::FloatToString(g_pStepRamp->getExcursion()));
	m_Delay.SetWindowText(etu::IntToString(g_pStepRamp->getOnDelay()));
	m_OffDelay.SetWindowText(etu::IntToString(g_pStepRamp->getOffDelay()));
	m_Plateau.SetWindowText(etu::IntToString(g_pStepRamp->getPlateau()));

	m_CheckUp.SetCheck((g_pStepRamp->getStepUp())? BST_CHECKED : BST_UNCHECKED);
	m_CheckDown.SetCheck((g_pStepRamp->getStepDown())? BST_CHECKED : BST_UNCHECKED);
	m_CheckLeft.SetCheck((g_pStepRamp->getStepLeft())? BST_CHECKED : BST_UNCHECKED);
	m_CheckRight.SetCheck((g_pStepRamp->getStepRight())? BST_CHECKED : BST_UNCHECKED);
	
	m_Behavior.SetCheck((g_pStepRamp->useBehavior())? BST_CHECKED : BST_UNCHECKED);
	OnBnClickedCheckBehavior();

	m_StartXUp.SetWindowText(etu::FloatToString(g_pStepRamp->getStartXUp()));
	m_StartYUp.SetWindowText(etu::FloatToString(g_pStepRamp->getStartYUp()));
	m_StartXDown.SetWindowText(etu::FloatToString(g_pStepRamp->getStartXDown()));
	m_StartYDown.SetWindowText(etu::FloatToString(g_pStepRamp->getStartYDown()));
	m_StartXLeft.SetWindowText(etu::FloatToString(g_pStepRamp->getStartXLeft()));
	m_StartYLeft.SetWindowText(etu::FloatToString(g_pStepRamp->getStartYLeft()));
	m_StartXRight.SetWindowText(etu::FloatToString(g_pStepRamp->getStartXRight()));
	m_StartYRight.SetWindowText(etu::FloatToString(g_pStepRamp->getStartYRight()));

}

void CStepRampDlg::save() {
	char buffer[100];

	m_DotSize.GetWindowText(buffer, 100);
	g_pStepRamp->setDotSize(etu::StringToFloat(buffer, g_pStepRamp->getDotSize()));

	m_StepSize.GetWindowText(buffer, 100);
	g_pStepRamp->setStepSize(etu::StringToFloat(buffer, g_pStepRamp->getStepSize()));

	m_Velocity.GetWindowText(buffer, 100);
	g_pStepRamp->setVelocity(etu::StringToFloat(buffer, g_pStepRamp->getVelocity()));

	m_Excursion.GetWindowText(buffer, 100);
	g_pStepRamp->setExcursion(etu::StringToFloat(buffer, g_pStepRamp->getExcursion()));

	m_Delay.GetWindowText(buffer, 100);
	g_pStepRamp->setOnDelay(etu::StringToInt(buffer, g_pStepRamp->getOnDelay()));

	m_OffDelay.GetWindowText(buffer, 100);
	g_pStepRamp->setOffDelay(etu::StringToInt(buffer, g_pStepRamp->getOffDelay()));

	m_Plateau.GetWindowText(buffer, 100);
	g_pStepRamp->setPlateau(etu::StringToInt(buffer, g_pStepRamp->getPlateau()));

	g_pStepRamp->setStepUp(m_CheckUp.GetCheck() == BST_CHECKED);
	g_pStepRamp->setStepDown(m_CheckDown.GetCheck() == BST_CHECKED);
	g_pStepRamp->setStepLeft(m_CheckLeft.GetCheck() == BST_CHECKED);
	g_pStepRamp->setStepRight(m_CheckRight.GetCheck() == BST_CHECKED);
	g_pStepRamp->setBehavior(m_Behavior.GetCheck() == BST_CHECKED);

	g_pStepRamp->sendSettings(g_pSettings->theSerialComm);

	m_StartXUp.GetWindowText(buffer, 100);
	g_pStepRamp->setStartXUp(etu::StringToFloat(buffer, g_pStepRamp->getStartXUp()));
	m_StartYUp.GetWindowText(buffer, 100);
	g_pStepRamp->setStartYUp(etu::StringToFloat(buffer, g_pStepRamp->getStartYUp()));
	m_StartXDown.GetWindowText(buffer, 100);
	g_pStepRamp->setStartXDown(etu::StringToFloat(buffer, g_pStepRamp->getStartXDown()));
	m_StartYDown.GetWindowText(buffer, 100);
	g_pStepRamp->setStartYDown(etu::StringToFloat(buffer, g_pStepRamp->getStartYDown()));
	m_StartXLeft.GetWindowText(buffer, 100);
	g_pStepRamp->setStartXLeft(etu::StringToFloat(buffer, g_pStepRamp->getStartXLeft()));
	m_StartYLeft.GetWindowText(buffer, 100);
	g_pStepRamp->setStartYLeft(etu::StringToFloat(buffer, g_pStepRamp->getStartYLeft()));
	m_StartXRight.GetWindowText(buffer, 100);
	g_pStepRamp->setStartXRight(etu::StringToFloat(buffer, g_pStepRamp->getStartXRight()));
	m_StartYRight.GetWindowText(buffer, 100);
	g_pStepRamp->setStartYRight(etu::StringToFloat(buffer, g_pStepRamp->getStartYRight()));
}

void CStepRampDlg::OnBnClickedButton1()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "Step Ramp Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
}

void CStepRampDlg::OnBnClickedCheckup()
{
	if (m_CheckUp.GetCheck() == BST_CHECKED || m_CheckDown.GetCheck() == BST_CHECKED || \
		m_CheckLeft.GetCheck() == BST_CHECKED || m_CheckRight.GetCheck() == BST_CHECKED) {
		m_Apply.EnableWindow(true);
	} 
	else {
		m_Apply.EnableWindow(false);
	}

}

void CStepRampDlg::OnBnClickedCheckleft()
{
	if (m_CheckUp.GetCheck() == BST_CHECKED || m_CheckDown.GetCheck() == BST_CHECKED || \
		m_CheckLeft.GetCheck() == BST_CHECKED || m_CheckRight.GetCheck() == BST_CHECKED) {
		m_Apply.EnableWindow(true);
	} 
	else {
		m_Apply.EnableWindow(false);
	}
}

void CStepRampDlg::OnBnClickedCheckdown()
{
	if (m_CheckUp.GetCheck() == BST_CHECKED || m_CheckDown.GetCheck() == BST_CHECKED || \
		m_CheckLeft.GetCheck() == BST_CHECKED || m_CheckRight.GetCheck() == BST_CHECKED) {
		m_Apply.EnableWindow(true);
	} 
	else {
		m_Apply.EnableWindow(false);
	}
}

void CStepRampDlg::OnBnClickedCheckright()
{
	if (m_CheckUp.GetCheck() == BST_CHECKED || m_CheckDown.GetCheck() == BST_CHECKED || \
		m_CheckLeft.GetCheck() == BST_CHECKED || m_CheckRight.GetCheck() == BST_CHECKED) {
		m_Apply.EnableWindow(true);
	} 
	else {
		m_Apply.EnableWindow(false);
	}
}


void CStepRampDlg::OnBnClickedButtonall()
{
	m_CheckUp.SetCheck(BST_CHECKED);
	m_CheckDown.SetCheck(BST_CHECKED);
	m_CheckLeft.SetCheck(BST_CHECKED);
	m_CheckRight.SetCheck(BST_CHECKED);
	m_Apply.EnableWindow(true);
}

void CStepRampDlg::OnBnClickedButtonnone()
{
	m_CheckUp.SetCheck(BST_UNCHECKED);
	m_CheckDown.SetCheck(BST_UNCHECKED);
	m_CheckLeft.SetCheck(BST_UNCHECKED);
	m_CheckRight.SetCheck(BST_UNCHECKED);
	m_Apply.EnableWindow(false);
}


void CStepRampDlg::OnBnClickedOk()
{	
	OnBnClickedButton1();
}

void CStepRampDlg::OnBnClickedCancel() {}

void CStepRampDlg::OnBnClickedCheckBehavior()
{
	m_Delay.EnableWindow(m_Behavior.GetCheck() != BST_CHECKED);
}

#endif