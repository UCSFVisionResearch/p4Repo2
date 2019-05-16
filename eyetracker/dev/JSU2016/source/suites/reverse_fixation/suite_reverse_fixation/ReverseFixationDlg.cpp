/**
 * @file ReverseFixationDlg.cpp
 * @brief 
 */
#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include "app/stdafx.h"
#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "ReverseFixationDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Defines
 *****************************************************************************/
#define MODE_FOVEA (0)
#define MODE_INIT (1)
#define MODE_TEST (2)

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CReverseFixationDlg, CDialog)
CReverseFixationDlg::CReverseFixationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReverseFixationDlg::IDD, pParent)
{}

CReverseFixationDlg::~CReverseFixationDlg()
{}

void CReverseFixationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_revPrimaryRed, m_PrimaryRed);
	DDX_Control(pDX, IDC_revPrimaryGreen, m_PrimaryGreen);
	DDX_Control(pDX, IDC_revPrimaryBlue, m_PrimaryBlue);
	DDX_Control(pDX, IDC_revPrimaryStatic, m_PrimaryStatic);

	DDX_Control(pDX, IDC_revSecondaryRed, m_SecondaryRed);
	DDX_Control(pDX, IDC_revSecondaryGreen, m_SecondaryGreen);
	DDX_Control(pDX, IDC_revSecondaryBlue, m_SecondaryBlue);
	DDX_Control(pDX, IDC_revSecondaryStatic, m_SecondaryStatic);

	DDX_Control(pDX, IDC_revInitLeft, m_initLeft);
	DDX_Control(pDX, IDC_revInitRight, m_initRight);
	DDX_Control(pDX, IDC_revInitLeftX, m_initLeftX);
	DDX_Control(pDX, IDC_revInitLeftY, m_initLeftY);
	DDX_Control(pDX, IDC_revInitRightX, m_initRightX);
	DDX_Control(pDX, IDC_revInitRightY, m_initRightY);

	DDX_Control(pDX, IDC_revDotSize, m_dotSize);
	DDX_Control(pDX, IDC_revDisplayTime, m_displayTime);
	DDX_Control(pDX, IDC_revModeFoveaLoop, m_modeFoveaLoop);
	DDX_Control(pDX, IDC_revModeInitLoop, m_modeInitLoop);
	DDX_Control(pDX, IDC_revModeTest, m_modeTest);

	DDX_Control(pDX, IDC_revEyePosnDisplayLX, m_EyePosnDisplayLX);
	DDX_Control(pDX, IDC_revEyePosnDisplayLY, m_EyePosnDisplayLY);
	DDX_Control(pDX, IDC_revEyePosnDisplayRX, m_EyePosnDisplayRX);
	DDX_Control(pDX, IDC_revEyePosnDisplayRY, m_EyePosnDisplayRY);

	DDX_Control(pDX, IDC_revWindowAllowancePix, m_WindowAllowancePix);
	DDX_Control(pDX, IDC_revWait, m_Wait);

	DDX_Control(pDX, IDC_APPLY, m_Apply);
	DDX_Control(pDX, IDC_revDebug, m_debug);
}

BEGIN_MESSAGE_MAP(CReverseFixationDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_WM_PAINT()

	ON_BN_CLICKED(IDC_revInitLeft, OnBnClickedInitLeft)
	ON_BN_CLICKED(IDC_revInitRight, OnBnClickedInitRight)
	ON_BN_CLICKED(IDC_revModeInitLoop, OnBnClickedModeInitLoop)
	ON_BN_CLICKED(IDC_revModeFoveaLoop, OnBnClickedModeFoveaLoop)
	ON_BN_CLICKED(IDC_revModeTest, OnBnClickedModeTest)
END_MESSAGE_MAP()

BOOL CReverseFixationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CReverseFixationDlg::load() 
{
	m_PrimaryRed.SetWindowText(etu::IntToString(g_pReverseFixation->getPrimaryRed()));
	m_PrimaryGreen.SetWindowText(etu::IntToString(g_pReverseFixation->getPrimaryGreen()));
	m_PrimaryBlue.SetWindowText(etu::IntToString(g_pReverseFixation->getPrimaryBlue()));
	m_SecondaryRed.SetWindowText(etu::IntToString(g_pReverseFixation->getSecondaryRed()));
	m_SecondaryGreen.SetWindowText(etu::IntToString(g_pReverseFixation->getSecondaryGreen()));
	m_SecondaryBlue.SetWindowText(etu::IntToString(g_pReverseFixation->getSecondaryBlue()));

	if (g_pReverseFixation->getInitLeft()){
		m_initLeft.SetCheck(BST_CHECKED);
		m_initRight.SetCheck(BST_UNCHECKED);
	}else{
		m_initLeft.SetCheck(BST_UNCHECKED);
		m_initRight.SetCheck(BST_CHECKED);
	}
	m_initLeftX.SetWindowText(etu::IntToString(g_pReverseFixation->getInitLeftX()));
	m_initLeftY.SetWindowText(etu::IntToString(g_pReverseFixation->getInitLeftY()));
	m_initRightX.SetWindowText(etu::IntToString(g_pReverseFixation->getInitRightX()));
	m_initRightY.SetWindowText(etu::IntToString(g_pReverseFixation->getInitRightY()));
	
	m_dotSize.SetWindowText(etu::IntToString(g_pReverseFixation->getDotSize()));
	m_displayTime.SetWindowText(etu::IntToString(g_pReverseFixation->getDisplayTime()));

	switch (g_pReverseFixation->getMode()){
		case MODE_FOVEA:
			OnBnClickedModeFoveaLoop();
			break;
		case MODE_INIT:
			OnBnClickedModeInitLoop();
			break;
		case MODE_TEST:
			OnBnClickedModeTest();
			break;
		default:
			OnBnClickedModeTest();
			break;
	}
	m_WindowAllowancePix.SetWindowTextA(etu::FloatToString(g_pReverseFixation->getWindowAllowancePix()));
	m_Wait.SetCheck(g_pReverseFixation->getUseWait()?BST_CHECKED:BST_UNCHECKED);
	m_Wait.EnableWindow(false);
	PaintColorStatics();
}

void CReverseFixationDlg::save() 
{
	char buffer[100];
	
	m_PrimaryRed.GetWindowTextA(buffer, 100);
	g_pReverseFixation->setPrimaryRed(etu::StringToInt(buffer, g_pReverseFixation->getPrimaryRed()));
	m_PrimaryGreen.GetWindowTextA(buffer, 100);
	g_pReverseFixation->setPrimaryGreen(etu::StringToInt(buffer, g_pReverseFixation->getPrimaryGreen()));
	m_PrimaryBlue.GetWindowTextA(buffer, 100);
	g_pReverseFixation->setPrimaryBlue(etu::StringToInt(buffer, g_pReverseFixation->getPrimaryBlue()));

	m_SecondaryRed.GetWindowTextA(buffer, 100);
	g_pReverseFixation->setSecondaryRed(etu::StringToInt(buffer, g_pReverseFixation->getSecondaryRed()));
	m_SecondaryGreen.GetWindowTextA(buffer, 100);
	g_pReverseFixation->setSecondaryGreen(etu::StringToInt(buffer, g_pReverseFixation->getSecondaryGreen()));
	m_SecondaryBlue.GetWindowTextA(buffer, 100);
	g_pReverseFixation->setSecondaryBlue(etu::StringToInt(buffer, g_pReverseFixation->getSecondaryBlue()));

	if(m_initLeft.GetCheck()==BST_CHECKED){
		g_pReverseFixation->setInitLeft(true);
	}else{
		g_pReverseFixation->setInitLeft(false);
	}

	m_initLeftX.GetWindowTextA(buffer, 100);
	g_pReverseFixation->setInitLeftX(etu::StringToInt(buffer, g_pReverseFixation->getInitLeftX()));
	m_initLeftY.GetWindowTextA(buffer, 100);
	g_pReverseFixation->setInitLeftY(etu::StringToInt(buffer, g_pReverseFixation->getInitLeftY()));

	m_initRightX.GetWindowTextA(buffer, 100);
	g_pReverseFixation->setInitRightX(etu::StringToInt(buffer, g_pReverseFixation->getInitRightX()));
	m_initRightY.GetWindowTextA(buffer, 100);
	g_pReverseFixation->setInitRightY(etu::StringToInt(buffer, g_pReverseFixation->getInitRightY()));

	m_dotSize.GetWindowTextA(buffer, 100);
	g_pReverseFixation->setDotSize(etu::StringToInt(buffer, g_pReverseFixation->getDotSize()));
	m_displayTime.GetWindowTextA(buffer, 100);
	g_pReverseFixation->setDisplayTime(etu::StringToInt(buffer, g_pReverseFixation->getDisplayTime()));

	if (m_modeFoveaLoop.GetCheck() == BST_CHECKED) {
		g_pReverseFixation->setMode(MODE_FOVEA);
	}else if(m_modeInitLoop.GetCheck() == BST_CHECKED) {
		g_pReverseFixation->setMode(MODE_INIT);
	}else{
		g_pReverseFixation->setMode(MODE_TEST);
	}
	
	m_WindowAllowancePix.GetWindowTextA(buffer,100);
	g_pReverseFixation->setWindowAllowancePix(etu::StringToFloat(buffer,g_pReverseFixation->getWindowAllowancePix()));

	g_pReverseFixation->setUseWait(m_Wait.GetCheck()==BST_CHECKED);
	//g_pReverseFixation->sendSettings(g_pSettings->theSerialComm);
}

void CReverseFixationDlg::OnPaint() 
{
	CDialog::OnPaint();
	PaintColorStatics();
}

void CReverseFixationDlg::PaintColorStatics()
{
	CRect rect;
	DWORD primaryColor, secondaryColor;

	primaryColor = RGB(g_pReverseFixation->getPrimaryRed(), g_pReverseFixation->getPrimaryGreen(), g_pReverseFixation->getPrimaryBlue());
	secondaryColor = RGB(g_pReverseFixation->getSecondaryRed(), g_pReverseFixation->getSecondaryGreen(), g_pReverseFixation->getSecondaryBlue());

	CPaintDC dc_p(&m_PrimaryStatic);
	m_PrimaryStatic.GetClientRect(rect);
	dc_p.FillSolidRect(rect, primaryColor);

	CPaintDC dc_s(&m_SecondaryStatic);
	m_SecondaryStatic.GetClientRect(rect);
	dc_s.FillSolidRect(rect, secondaryColor);

}

void CReverseFixationDlg::OnBnClickedOk()
{	
	OnBnClickedApply();
}

void CReverseFixationDlg::OnBnClickedApply()
{	
	if (g_pActiveStimulus  && g_pActiveStimulus->StimulusType() == "Reverse Fixation Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
	this->Invalidate(false);
}

void CReverseFixationDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CReverseFixationDlg::OnBnClickedInitLeft()
{
	m_initLeft.SetCheck(true);
	m_initRight.SetCheck(false);
}

void CReverseFixationDlg::OnBnClickedInitRight()
{
	m_initLeft.SetCheck(false);
	m_initRight.SetCheck(true);
}

void CReverseFixationDlg::OnBnClickedModeInitLoop()
{
	m_modeInitLoop.SetCheck(true);
	m_modeFoveaLoop.SetCheck(false);
	m_modeTest.SetCheck(false);
}

void CReverseFixationDlg::OnBnClickedModeFoveaLoop()
{
	m_modeInitLoop.SetCheck(false);
	m_modeFoveaLoop.SetCheck(true);
	m_modeTest.SetCheck(false);
}
void CReverseFixationDlg::OnBnClickedModeTest()
{
	m_modeInitLoop.SetCheck(false);
	m_modeFoveaLoop.SetCheck(false);
	m_modeTest.SetCheck(true);
}

void CReverseFixationDlg::UpdateEyePositions(double lxDeg, double lyDeg, 
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

void CReverseFixationDlg::printDebug(LPCTSTR text){
	m_debug.SetWindowText(text);
}

#endif