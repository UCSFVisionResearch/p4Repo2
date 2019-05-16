/**
 * @file APDDlg.cpp
 * @brief 
 */


#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "APDDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CAPDDlg, CDialog)
CAPDDlg::CAPDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAPDDlg::IDD, pParent)
{}

CAPDDlg::~CAPDDlg()
{}

void CAPDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOW_FIXATION, m_ShowFixation);
	DDX_Control(pDX, IDC_FIXATION_POSX, m_FixationPosX);
	DDX_Control(pDX, IDC_FIXATION_POSY, m_FixationPosY);
	DDX_Control(pDX, IDC_FIXATION_RED, m_FixationRed);
	DDX_Control(pDX, IDC_FIXATION_GREEN, m_FixationGreen);
	DDX_Control(pDX, IDC_FIXATION_BLUE, m_FixationBlue);
	DDX_Control(pDX, IDC_ON_RED, m_OnRed);
	DDX_Control(pDX, IDC_ON_GREEN, m_OnGreen);
	DDX_Control(pDX, IDC_ON_BLUE, m_OnBlue);
	DDX_Control(pDX, IDC_OFF_RED, m_OffRed);
	DDX_Control(pDX, IDC_OFF_GREEN, m_OffGreen);
	DDX_Control(pDX, IDC_OFF_BLUE, m_OffBlue);
	DDX_Control(pDX, IDC_ON_TIME, m_OnTime);
	DDX_Control(pDX, IDC_OFF_TIME, m_OffTime);
	DDX_Control(pDX, IDOK, m_ApplyButton);
	DDX_Control(pDX, IDC_FIXATION_SIZE, m_FixationSize);
	DDX_Control(pDX, IDC_LEFT_FIELD, m_LeftField);
	DDX_Control(pDX, IDC_RIGHT_FIELD, m_RightField);
	DDX_Control(pDX, IDC_ALTERNATE, m_Alternate);
	DDX_Control(pDX, IDC_FULLSCREEN, m_FullScreen);
	DDX_Control(pDX, IDC_ALTERNATE_DELAY, m_AlternateDelay);
	DDX_Control(pDX, IDC_FIXATION_DELAY, m_FixationDelay);
	DDX_Control(pDX, IDC_BEHAVIOR, m_Behavior);
	DDX_Control(pDX, IDC_FIELD_WIDTH, m_FieldWidth);
	DDX_Control(pDX, IDC_FIELD_HEIGHT, m_FieldHeight);
	DDX_Control(pDX, IDC_CENTER_X_LEFT, m_CenterXLeft);
	DDX_Control(pDX, IDC_CENTER_Y_LEFT, m_CenterYLeft);
	DDX_Control(pDX, IDC_CENTER_X_RIGHT, m_CenterXRight);
	DDX_Control(pDX, IDC_CENTER_Y_RIGHT, m_CenterYRight);
}

BEGIN_MESSAGE_MAP(CAPDDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SHOW_FIXATION, OnBnClickedShowFixation)
	ON_BN_CLICKED(IDC_LEFT_FIELD, OnBnClickedLeftField)
	ON_BN_CLICKED(IDC_RIGHT_FIELD, OnBnClickedRightField)
	ON_BN_CLICKED(IDC_ALTERNATE, OnBnClickedAlternate)
	ON_BN_CLICKED(IDC_FULLSCREEN, OnBnClickedFullscreen)
	ON_BN_CLICKED(IDC_BEHAVIOR, OnBnClickedBehavior)
END_MESSAGE_MAP()

BOOL CAPDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAPDDlg::load()
{
	m_FixationSize.SetWindowText(etu::FloatToString(g_pAPD->getFixationSize()));
	m_FixationPosX.SetWindowText(etu::FloatToString(g_pAPD->getFixationPosX()));
	m_FixationPosY.SetWindowText(etu::FloatToString(g_pAPD->getFixationPosY()));
	
	m_FixationRed.SetWindowText(etu::IntToString(g_pAPD->getFixationRed()));
	m_FixationGreen.SetWindowText(etu::IntToString(g_pAPD->getFixationGreen()));
	m_FixationBlue.SetWindowText(etu::IntToString(g_pAPD->getFixationBlue()));

	m_OnRed.SetWindowText(etu::IntToString(g_pAPD->getOnRed()));
	m_OnGreen.SetWindowText(etu::IntToString(g_pAPD->getOnGreen()));
	m_OnBlue.SetWindowText(etu::IntToString(g_pAPD->getOnBlue()));

	m_OffRed.SetWindowText(etu::IntToString(g_pAPD->getOffRed()));
	m_OffGreen.SetWindowText(etu::IntToString(g_pAPD->getOffGreen()));
	m_OffBlue.SetWindowText(etu::IntToString(g_pAPD->getOffBlue()));

	m_OnTime.SetWindowText(etu::IntToString(g_pAPD->getOnTime()));
	m_OffTime.SetWindowText(etu::IntToString(g_pAPD->getOffTime()));
	m_AlternateDelay.SetWindowText(etu::IntToString(g_pAPD->getIntertrialDelay()));
	m_FixationDelay.SetWindowText(etu::IntToString(g_pAPD->getFixationDelay()));
	m_FieldWidth.SetWindowText(etu::FloatToString((float)g_pAPD->getFieldWidth()));
	m_FieldHeight.SetWindowText(etu::FloatToString((float)g_pAPD->getFieldHeight()));

	m_CenterXLeft.SetWindowText(etu::FloatToString((float)g_pAPD->getCenterXLeft()));
	m_CenterYLeft.SetWindowText(etu::FloatToString((float)g_pAPD->getCenterYLeft()));
	m_CenterXRight.SetWindowText(etu::FloatToString((float)g_pAPD->getCenterXRight()));
	m_CenterYRight.SetWindowText(etu::FloatToString((float)g_pAPD->getCenterYRight()));
	
	if (g_pAPD->getShowFixation()) {
		m_ShowFixation.SetCheck(BST_CHECKED);
	} 
	else {
		m_ShowFixation.SetCheck(BST_UNCHECKED);
	}
	OnBnClickedShowFixation();

	if (g_pAPD->getField() == CAPD::FIELD_LT) {
		OnBnClickedLeftField();
	} 
	else if (g_pAPD->getField() == CAPD::FIELD_RT) {
		OnBnClickedRightField();
	} 
	else if (g_pAPD->getField() == CAPD::FIELD_ALTERNATE) {
		OnBnClickedAlternate();
	} 
	else {
		OnBnClickedFullscreen();
	}

	if (g_pAPD->useBehavior()) {
		m_Behavior.SetCheck(BST_CHECKED);
	} 
	else {
		m_Behavior.SetCheck(BST_UNCHECKED);
	}
	OnBnClickedBehavior();
}

void CAPDDlg::save()
{
	char buffer[100];

	m_FixationSize.GetWindowText(buffer, 100);
	g_pAPD->setFixationSize(etu::StringToFloat(buffer, g_pAPD->getFixationSize()));

	m_FixationPosX.GetWindowText(buffer, 100);
	g_pAPD->setFixationPosX(etu::StringToFloat(buffer, g_pAPD->getFixationPosX()));
	
	m_FixationPosY.GetWindowText(buffer, 100);
	g_pAPD->setFixationPosY(etu::StringToFloat(buffer, g_pAPD->getFixationPosY()));

	m_FixationRed.GetWindowText(buffer, 100);
	g_pAPD->setFixationRed(etu::StringToInt(buffer, g_pAPD->getFixationRed()));

	m_FixationGreen.GetWindowText(buffer, 100);
	g_pAPD->setFixationGreen(etu::StringToInt(buffer, g_pAPD->getFixationGreen()));

	m_FixationBlue.GetWindowText(buffer, 100);
	g_pAPD->setFixationBlue(etu::StringToInt(buffer, g_pAPD->getFixationBlue()));

	m_OnRed.GetWindowText(buffer, 100);
	g_pAPD->setOnRed(etu::StringToInt(buffer, g_pAPD->getOnRed()));

	m_OnGreen.GetWindowText(buffer, 100);
	g_pAPD->setOnGreen(etu::StringToInt(buffer, g_pAPD->getOnGreen()));

	m_OnBlue.GetWindowText(buffer, 100);
	g_pAPD->setOnBlue(etu::StringToInt(buffer, g_pAPD->getOnBlue()));

	m_OffRed.GetWindowText(buffer, 100);
	g_pAPD->setOffRed(etu::StringToInt(buffer, g_pAPD->getOffRed()));

	m_OffGreen.GetWindowText(buffer, 100);
	g_pAPD->setOffGreen(etu::StringToInt(buffer, g_pAPD->getOffGreen()));

	m_OffBlue.GetWindowText(buffer, 100);
	g_pAPD->setOffBlue(etu::StringToInt(buffer, g_pAPD->getOffBlue()));

	m_OnTime.GetWindowText(buffer, 100);
	g_pAPD->setOnTime(etu::StringToInt(buffer, g_pAPD->getOnTime()));

	m_OffTime.GetWindowText(buffer, 100);
	g_pAPD->setOffTime(etu::StringToInt(buffer, g_pAPD->getOffTime()));

	m_AlternateDelay.GetWindowText(buffer, 100);
	g_pAPD->setIntertrialDelay(etu::StringToInt(buffer, g_pAPD->getIntertrialDelay()));

	m_FixationDelay.GetWindowText(buffer, 100);
	g_pAPD->setFixationDelay(etu::StringToInt(buffer, g_pAPD->getFixationDelay()));

	m_FieldWidth.GetWindowText(buffer, 100);
	g_pAPD->setFieldWidth((double)etu::StringToFloat(buffer, (float)g_pAPD->getFieldWidth()));

	m_FieldHeight.GetWindowText(buffer, 100);
	g_pAPD->setFieldHeight((double)etu::StringToFloat(buffer, (float)g_pAPD->getFieldHeight()));

	m_CenterXLeft.GetWindowText(buffer, 100);
	g_pAPD->setCenterXLeft((double)etu::StringToFloat(buffer, (float)g_pAPD->getCenterXLeft()));

	m_CenterXRight.GetWindowText(buffer, 100);
	g_pAPD->setCenterXRight((double)etu::StringToFloat(buffer, (float)g_pAPD->getCenterXRight()));

	m_CenterYLeft.GetWindowText(buffer, 100);
	g_pAPD->setCenterYLeft((double)etu::StringToFloat(buffer, (float)g_pAPD->getCenterYLeft()));

	m_CenterYRight.GetWindowText(buffer, 100);
	g_pAPD->setCenterYRight((double)etu::StringToFloat(buffer, (float)g_pAPD->getCenterYRight()));
	
	g_pAPD->setShowFixation(m_ShowFixation.GetCheck() == BST_CHECKED);
	g_pAPD->setBehavior(m_Behavior.GetCheck() == BST_CHECKED);

	if (m_LeftField.GetCheck() == BST_CHECKED) {
		g_pAPD->setField(CAPD::FIELD_LT);
	} 
	else if (m_RightField.GetCheck() == BST_CHECKED) {
		g_pAPD->setField(CAPD::FIELD_RT);
	} 
	else if (m_Alternate.GetCheck() == BST_CHECKED) {
		g_pAPD->setField(CAPD::FIELD_ALTERNATE);
	} 
	else {
		g_pAPD->setField(CAPD::FIELD_FULLSCREEN);
	}

	g_pAPD->sendSettings(g_pSettings->theSerialComm);
}

void CAPDDlg::OnBnClickedOk()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "Afferent Pupil Defect Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
}

void CAPDDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CAPDDlg::OnBnClickedShowFixation()
{
	if (m_ShowFixation.GetCheck() == BST_CHECKED) {
		m_FixationSize.EnableWindow(true);
		m_FixationPosX.EnableWindow(true);
		m_FixationPosY.EnableWindow(true);
		m_FixationRed.EnableWindow(true);
		m_FixationGreen.EnableWindow(true);
		m_FixationBlue.EnableWindow(true);

	} 
	else {
		m_FixationSize.EnableWindow(false);
		m_FixationPosX.EnableWindow(false);
		m_FixationPosY.EnableWindow(false);
		m_FixationRed.EnableWindow(false);
		m_FixationGreen.EnableWindow(false);
		m_FixationBlue.EnableWindow(false);
	}
}

void CAPDDlg::OnBnClickedLeftField()
{
	m_LeftField.SetCheck(BST_CHECKED);
	m_RightField.SetCheck(BST_UNCHECKED);
	m_Alternate.SetCheck(BST_UNCHECKED);
	m_FullScreen.SetCheck(BST_UNCHECKED);
}

void CAPDDlg::OnBnClickedRightField()
{
	m_LeftField.SetCheck(BST_UNCHECKED);
	m_RightField.SetCheck(BST_CHECKED);
	m_Alternate.SetCheck(BST_UNCHECKED);
	m_FullScreen.SetCheck(BST_UNCHECKED);
}

void CAPDDlg::OnBnClickedAlternate()
{
	m_LeftField.SetCheck(BST_UNCHECKED);
	m_RightField.SetCheck(BST_UNCHECKED);
	m_Alternate.SetCheck(BST_CHECKED);
	m_FullScreen.SetCheck(BST_UNCHECKED);
}

void CAPDDlg::OnBnClickedFullscreen()
{
	m_LeftField.SetCheck(BST_UNCHECKED);
	m_RightField.SetCheck(BST_UNCHECKED);
	m_Alternate.SetCheck(BST_UNCHECKED);
	m_FullScreen.SetCheck(BST_CHECKED);
}


void CAPDDlg::OnBnClickedBehavior()
{
	if (m_Behavior.GetCheck() == BST_CHECKED) {
		m_FixationDelay.EnableWindow(false);
	} 
	else {
		m_FixationDelay.EnableWindow(true);
	}
}


#endif
