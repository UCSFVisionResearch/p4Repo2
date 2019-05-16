/**
 * @file AlternationDlg.cpp
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
#include "AlternationDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CAlternationDlg, CDialog)
CAlternationDlg::CAlternationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlternationDlg::IDD, pParent)
{}

CAlternationDlg::~CAlternationDlg()
{}

void CAlternationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XPos1, m_XPos1);
	DDX_Control(pDX, IDC_YPos1, m_YPos1);
	DDX_Control(pDX, IDC_XPos2, m_XPos2);
	DDX_Control(pDX, IDC_YPos2, m_YPos2);
	DDX_Control(pDX, IDC_DotSize1, m_DotSize1);
	DDX_Control(pDX, IDC_DotSize2, m_DotSize2);
	DDX_Control(pDX, IDC_Red1, m_Red1);
	DDX_Control(pDX, IDC_Green1, m_Green1);
	DDX_Control(pDX, IDC_Blue1, m_Blue1);
	DDX_Control(pDX, IDC_Red2, m_Red2);
	DDX_Control(pDX, IDC_Green2, m_Green2);
	DDX_Control(pDX, IDC_Blue2, m_Blue2);
	DDX_Control(pDX, IDC_Diamond, m_Diamond);
}

BEGIN_MESSAGE_MAP(CAlternationDlg, CDialog)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedButtonapply)
END_MESSAGE_MAP()

BOOL CAlternationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	load();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAlternationDlg::load() 
{
	m_DotSize1.SetWindowText(etu::FloatToString((float)g_pAlternation->getDotSize1()));
	m_DotSize2.SetWindowText(etu::FloatToString((float)g_pAlternation->getDotSize2()));
	m_XPos1.SetWindowText(etu::FloatToString((float)g_pAlternation->getXPos1()));
	m_YPos1.SetWindowText(etu::FloatToString((float)g_pAlternation->getYPos1()));
	m_XPos2.SetWindowText(etu::FloatToString((float)g_pAlternation->getXPos2()));
	m_YPos2.SetWindowText(etu::FloatToString((float)g_pAlternation->getYPos2()));
	m_Red1.SetWindowText(etu::IntToString(g_pAlternation->getRed1()));
	m_Green1.SetWindowText(etu::IntToString(g_pAlternation->getGreen1()));
	m_Blue1.SetWindowText(etu::IntToString(g_pAlternation->getBlue1()));
	m_Red2.SetWindowText(etu::IntToString(g_pAlternation->getRed2()));
	m_Green2.SetWindowText(etu::IntToString(g_pAlternation->getGreen2()));
	m_Blue2.SetWindowText(etu::IntToString(g_pAlternation->getBlue2()));
	m_Diamond.SetWindowText(etu::FloatToString(g_pAlternation->getDiamond()));
}

void CAlternationDlg::save() 
{
	char buffer[100];

	m_XPos1.GetWindowText(buffer, 100);
	g_pAlternation->setXPos1((double)etu::StringToFloat(buffer, (float)g_pAlternation->getXPos1()));
	m_YPos1.GetWindowText(buffer, 100);
	g_pAlternation->setYPos1((double)etu::StringToFloat(buffer, (float)g_pAlternation->getYPos1()));
	m_XPos2.GetWindowText(buffer, 100);
	g_pAlternation->setXPos2((double)etu::StringToFloat(buffer, (float)g_pAlternation->getXPos2()));
	m_YPos2.GetWindowText(buffer, 100);
	g_pAlternation->setYPos2((double)etu::StringToFloat(buffer, (float)g_pAlternation->getYPos2()));

	m_DotSize1.GetWindowText(buffer, 100);
	g_pAlternation->setDotSize1((double)etu::StringToFloat(buffer, (float)g_pAlternation->getDotSize1()));
	m_DotSize2.GetWindowText(buffer, 100);
	g_pAlternation->setDotSize2((double)etu::StringToFloat(buffer, (float)g_pAlternation->getDotSize2()));

	m_Red1.GetWindowText(buffer, 100);
	g_pAlternation->setRed1(etu::StringToInt(buffer, g_pAlternation->getRed1()));
	m_Green1.GetWindowText(buffer, 100);
	g_pAlternation->setGreen1(etu::StringToInt(buffer, g_pAlternation->getGreen1()));
	m_Blue1.GetWindowText(buffer, 100);
	g_pAlternation->setBlue1(etu::StringToInt(buffer, g_pAlternation->getBlue1()));

	m_Red2.GetWindowText(buffer, 100);
	g_pAlternation->setRed2(etu::StringToInt(buffer, g_pAlternation->getRed2()));
	m_Green2.GetWindowText(buffer, 100);
	g_pAlternation->setGreen2(etu::StringToInt(buffer, g_pAlternation->getGreen2()));
	m_Blue2.GetWindowText(buffer, 100);
	g_pAlternation->setBlue2(etu::StringToInt(buffer, g_pAlternation->getBlue2()));

	m_Diamond.GetWindowText(buffer, 100);
	g_pAlternation->setDiamond(etu::StringToFloat(buffer, g_pAlternation->getDiamond()));

	g_pAlternation->sendSettings(g_pSettings->theSerialComm);
}

void CAlternationDlg::OnBnClickedButtonapply()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "Alternation Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
	//g_pAlternation->GenerateLocations();	
}

void CAlternationDlg::OnBnClickedOk()
{	
	OnBnClickedButtonapply();
}

void CAlternationDlg::OnBnClickedCancel()
{}


#endif


