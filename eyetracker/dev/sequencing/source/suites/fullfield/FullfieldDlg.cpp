/**
 * @file FullfieldDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "FullfieldDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CFullfieldDlg, CDialog)
CFullfieldDlg::CFullfieldDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFullfieldDlg::IDD, pParent)
{}

CFullfieldDlg::~CFullfieldDlg()
{}

void CFullfieldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EditWidth, m_BarWidth);
	DDX_Control(pDX, IDC_EditVelocity, m_DriftVelocity);
	DDX_Control(pDX, IDC_RadioUp, m_Up);
	DDX_Control(pDX, IDC_RadioDown, m_Down);
	DDX_Control(pDX, IDC_RadioLeft, m_Left);
	DDX_Control(pDX, IDC_RadioRight, m_Right);
	DDX_Control(pDX, IDC_CheckFixation, m_CheckFixation);
	DDX_Control(pDX, IDC_EditFixationSize, m_FixationSize);
	DDX_Control(pDX, IDC_EditFixationRed, m_FixationRed);
	DDX_Control(pDX, IDC_EditFixationGreen, m_FixationGreen);
	DDX_Control(pDX, IDC_EditFixationBlue, m_FixationBlue);
	DDX_Control(pDX, IDC_EditFixtionXPos, m_FixationXPos);
	DDX_Control(pDX, IDC_EditFixationYPos, m_FixationYPos);	
	DDX_Control(pDX, IDC_DURATION, m_Duration);
	DDX_Control(pDX, IDC_ButtonStartMotion, m_Motion);
	DDX_Control(pDX, IDC_ButtonApply, m_Apply);
	DDX_Control(pDX, IDC_Noise, m_Noise);
	DDX_Control(pDX, IDC_useGlobalColors, m_UseGlobalColors);
	DDX_Control(pDX, IDC_EditContrast, m_Contrast);
}

BEGIN_MESSAGE_MAP(CFullfieldDlg, CDialog)
	ON_BN_CLICKED(IDC_RadioUp, OnBnClickedRadioup)
	ON_BN_CLICKED(IDC_RadioLeft, OnBnClickedRadioleft)
	ON_BN_CLICKED(IDC_RadioDown, OnBnClickedRadiodown)
	ON_BN_CLICKED(IDC_RadioRight, OnBnClickedRadioright)
	ON_BN_CLICKED(IDC_ButtonApply, OnBnClickedButtonapply)
	ON_BN_CLICKED(IDC_CheckFixation, OnBnClickedCheckfixation)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ButtonStartMotion, OnBnClickedButtonstartmotion)
END_MESSAGE_MAP()

BOOL CFullfieldDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Motion.EnableWindow(false);
	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFullfieldDlg::load() 
{
	m_BarWidth.SetWindowText(etu::FloatToString(g_pFullfield->getBarWidth()));
	m_DriftVelocity.SetWindowText(etu::FloatToString(g_pFullfield->getDriftVelocity()));
	m_FixationSize.SetWindowText(etu::FloatToString(g_pFullfield->getFixationSize()));
	m_FixationRed.SetWindowText(etu::IntToString(g_pFullfield->getFixationRed()));
	m_FixationGreen.SetWindowText(etu::IntToString(g_pFullfield->getFixationGreen()));
	m_FixationBlue.SetWindowText(etu::IntToString(g_pFullfield->getFixationBlue()));
	m_FixationXPos.SetWindowText(etu::FloatToString(g_pFullfield->getFixationXPos()));
	m_FixationYPos.SetWindowText(etu::FloatToString(g_pFullfield->getFixationYPos()));
	m_Duration.SetWindowText(etu::FloatToString(g_pFullfield->getDuration()));
	m_Contrast.SetWindowText(etu::FloatToString(g_pFullfield->getContrast()));
	
	switch (g_pFullfield->getDriftDirection()) {
		case CFullfield::DIR_UP: OnBnClickedRadioup(); break;
		case CFullfield::DIR_DN: OnBnClickedRadiodown(); break;
		case CFullfield::DIR_LT: OnBnClickedRadioleft(); break;
		case CFullfield::DIR_RT: OnBnClickedRadioright(); break;
	}

	m_CheckFixation.SetCheck(g_pFullfield->useFixation() ? 
		BST_CHECKED : BST_UNCHECKED);
	
	m_Noise.SetCheck(g_pFullfield->getNoise() ? 
		BST_CHECKED : BST_UNCHECKED);

	m_UseGlobalColors.SetCheck(g_pFullfield->getUseGlobalColors() ? 
		BST_CHECKED : BST_UNCHECKED);
	
	OnBnClickedCheckfixation();
}


void CFullfieldDlg::save() 
{
	char buffer[100];

	m_BarWidth.GetWindowText(buffer, 100);
	g_pFullfield->setBarWidth(etu::StringToFloat(buffer, g_pFullfield->getBarWidth()));

	m_FixationSize.GetWindowText(buffer, 100);
	g_pFullfield->setFixationSize(etu::StringToFloat(buffer, g_pFullfield->getFixationSize()));

	m_DriftVelocity.GetWindowText(buffer, 100);
	g_pFullfield->setDriftVelocity(etu::StringToFloat(buffer, g_pFullfield->getDriftVelocity()));

	m_FixationRed.GetWindowText(buffer, 100);
	g_pFullfield->setFixationRed(etu::StringToInt(buffer, g_pFullfield->getFixationRed()));

	m_FixationGreen.GetWindowText(buffer, 100);
	g_pFullfield->setFixationGreen(etu::StringToInt(buffer, g_pFullfield->getFixationGreen()));

	m_FixationBlue.GetWindowText(buffer, 100);
	g_pFullfield->setFixationBlue(etu::StringToInt(buffer, g_pFullfield->getFixationBlue()));

	m_FixationXPos.GetWindowText(buffer, 100);
	g_pFullfield->setFixationXPos(etu::StringToFloat(buffer, g_pFullfield->getFixationXPos()));

	m_FixationYPos.GetWindowText(buffer, 100);
	g_pFullfield->setFixationYPos(etu::StringToFloat(buffer, g_pFullfield->getFixationYPos()));

    m_Duration.GetWindowText(buffer, 100);
	g_pFullfield->setDuration(etu::StringToFloat(buffer, g_pFullfield->getDuration()));

	m_Contrast.GetWindowText(buffer, 100);
	g_pFullfield->setContrast(etu::StringToFloat(buffer, g_pFullfield->getContrast()));


	if (m_Up.GetCheck() == BST_CHECKED) {
		g_pFullfield->setDriftDirection(CFullfield::DIR_UP);
	} 
	else if (m_Down.GetCheck() == BST_CHECKED) {
		g_pFullfield->setDriftDirection(CFullfield::DIR_DN);
	} 
	else if (m_Left.GetCheck() == BST_CHECKED) {
		g_pFullfield->setDriftDirection(CFullfield::DIR_LT);
	} 
	else if (m_Right.GetCheck() == BST_CHECKED) {
		g_pFullfield->setDriftDirection(CFullfield::DIR_RT);
	}

	g_pFullfield->setUseFixation(m_CheckFixation.GetCheck() == BST_CHECKED);

	g_pFullfield->setNoise(m_Noise.GetCheck() == BST_CHECKED);

	g_pFullfield->setUseGlobalColors(m_UseGlobalColors.GetCheck() == BST_CHECKED);

	g_pFullfield->sendSettings(g_pSettings->theSerialComm);
}

void CFullfieldDlg::OnBnClickedRadioup()
{
	m_Up.SetCheck(BST_CHECKED);
	m_Down.SetCheck(BST_UNCHECKED);
	m_Left.SetCheck(BST_UNCHECKED);
	m_Right.SetCheck(BST_UNCHECKED);
}

void CFullfieldDlg::OnBnClickedRadioleft()
{
	m_Up.SetCheck(BST_UNCHECKED);
	m_Down.SetCheck(BST_UNCHECKED);
	m_Left.SetCheck(BST_CHECKED);
	m_Right.SetCheck(BST_UNCHECKED);
}

void CFullfieldDlg::OnBnClickedRadiodown()
{
	m_Up.SetCheck(BST_UNCHECKED);
	m_Down.SetCheck(BST_CHECKED);
	m_Left.SetCheck(BST_UNCHECKED);
	m_Right.SetCheck(BST_UNCHECKED);
}

void CFullfieldDlg::OnBnClickedRadioright()
{
	m_Up.SetCheck(BST_UNCHECKED);
	m_Down.SetCheck(BST_UNCHECKED);
	m_Left.SetCheck(BST_UNCHECKED);
	m_Right.SetCheck(BST_CHECKED);
}

void CFullfieldDlg::OnBnClickedButtonapply()
{
	bool bWasActive = false;
	if (g_pActiveStimulus) {
		bWasActive = (g_pActiveStimulus->StimulusType() == "Full Field Stimulus");
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
	if (bWasActive) {
		while (g_pActiveStimulus)
			Sleep(100);
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
}

void CFullfieldDlg::OnBnClickedCheckfixation()
{
	if (m_CheckFixation.GetCheck() == BST_CHECKED) {
		m_FixationSize.EnableWindow(true);
		m_FixationRed.EnableWindow(true);
		m_FixationGreen.EnableWindow(true);
		m_FixationBlue.EnableWindow(true);
		m_FixationXPos.EnableWindow(true);
		m_FixationYPos.EnableWindow(true);
	} 
	else {
		m_FixationSize.EnableWindow(false);
		m_FixationRed.EnableWindow(false);
		m_FixationGreen.EnableWindow(false);
		m_FixationBlue.EnableWindow(false);
		m_FixationXPos.EnableWindow(false);
		m_FixationYPos.EnableWindow(false);
	}
}

void CFullfieldDlg::OnBnClickedOk()
{	
	OnBnClickedButtonapply();
}

void CFullfieldDlg::OnBnClickedCancel()
{	
	
}

void CFullfieldDlg::OnBnClickedButtonstartmotion()
{
	if (g_pActiveStimulus->StimulusType() == "Full Field Stimulus") {
		m_Motion.EnableWindow(false);
		((CFullfieldStimulus*)g_pActiveStimulus)->ActivateMotion();
	}
}

void CFullfieldDlg::ActivateMotionButton() 
{
	m_Motion.EnableWindow(true);
}

void CFullfieldDlg::ActivateApplyButton(bool bActivate) 
{
	m_Apply.EnableWindow(bActivate);
}


#endif