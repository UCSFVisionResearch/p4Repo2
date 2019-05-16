/**
 * @file RandomDotAnaglyphDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "RandomDotAnaglyphDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CRandomDotAnaglyphDlg, CDialog)
CRandomDotAnaglyphDlg::CRandomDotAnaglyphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRandomDotAnaglyphDlg::IDD, pParent)
{}

CRandomDotAnaglyphDlg::~CRandomDotAnaglyphDlg()
{}

void CRandomDotAnaglyphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Red, m_Red);
	DDX_Control(pDX, IDC_Blue, m_Blue);
	DDX_Control(pDX, IDC_Text, m_Text);
	DDX_Control(pDX, IDC_Image, m_DepthImage);
	DDX_Control(pDX, IDC_BGDisparity, m_BGDisparity);
	DDX_Control(pDX, IDC_Noise, m_Noise);
	DDX_Control(pDX, IDC_TextDisparity, m_TextDisparity);
	DDX_Control(pDX, IDC_MaxDisparity, m_MaxDisparity);
	DDX_Control(pDX, IDC_ShowDisparity, m_ShowDisparity);
}

BEGIN_MESSAGE_MAP(CRandomDotAnaglyphDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CRandomDotAnaglyphDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRandomDotAnaglyphDlg::load() {
	m_Text.SetWindowText(g_pRandomDotAnaglyph->getText());
	m_DepthImage.SetWindowText(g_pRandomDotAnaglyph->getImagePath());
	m_Red.SetWindowText(etu::IntToString(g_pRandomDotAnaglyph->getRed()));
	m_Blue.SetWindowText(etu::IntToString(g_pRandomDotAnaglyph->getBlue()));

	if (g_pRandomDotAnaglyph->getRadio()==0) {
		g_pRandomDotAnaglyphDlg->SendDlgItemMessage(IDC_RadioText, BM_SETCHECK, BST_CHECKED, 0);
	} 
	else if (g_pRandomDotAnaglyph->getRadio()==1) {
		g_pRandomDotAnaglyphDlg->SendDlgItemMessage(IDC_RadioImage, BM_SETCHECK, BST_CHECKED, 0);
	} 
	else {
		g_pRandomDotAnaglyphDlg->SendDlgItemMessage(IDC_RadioShapes, BM_SETCHECK, BST_CHECKED, 0);
	}
	m_BGDisparity.SetWindowText(etu::IntToString(g_pRandomDotAnaglyph->getBGDisparity()));
	m_Noise.SetWindowText(etu::IntToString(g_pRandomDotAnaglyph->getNoise()));
	m_TextDisparity.SetWindowText(etu::IntToString(g_pRandomDotAnaglyph->getTextDisparity()));
	m_MaxDisparity.SetWindowText(etu::IntToString(g_pRandomDotAnaglyph->getMaxDisparity()));
}

void CRandomDotAnaglyphDlg::save() 
{
	char buffer[1024];

	m_Text.GetWindowText(buffer, 1024);
	g_pRandomDotAnaglyph->setText(CString(buffer));

	m_DepthImage.GetWindowText(buffer, 1024);
	g_pRandomDotAnaglyph->setImagePath(CString(buffer));

	m_Red.GetWindowText(buffer, 1024);
	g_pRandomDotAnaglyph->setRed(etu::StringToInt(buffer, g_pRandomDotAnaglyph->getRed()));

	m_Blue.GetWindowText(buffer, 1024);
	g_pRandomDotAnaglyph->setBlue(etu::StringToInt(buffer, g_pRandomDotAnaglyph->getBlue()));

	if (g_pRandomDotAnaglyphDlg->SendDlgItemMessage(IDC_RadioText, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		g_pRandomDotAnaglyph->setRadio(0);
	} 
	else if (g_pRandomDotAnaglyphDlg->SendDlgItemMessage(IDC_RadioImage, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		g_pRandomDotAnaglyph->setRadio(1);
	} 
	else {
		g_pRandomDotAnaglyph->setRadio(2);
	}

	m_BGDisparity.GetWindowText(buffer, 1024);
	g_pRandomDotAnaglyph->setBGDisparity(etu::StringToInt(buffer, g_pRandomDotAnaglyph->getBGDisparity()));

	m_Noise.GetWindowText(buffer, 1024);
	g_pRandomDotAnaglyph->setNoise(etu::StringToInt(buffer, g_pRandomDotAnaglyph->getNoise()));

	m_TextDisparity.GetWindowText(buffer, 1024);
	g_pRandomDotAnaglyph->setTextDisparity(etu::StringToInt(buffer, g_pRandomDotAnaglyph->getTextDisparity()));

	m_MaxDisparity.GetWindowText(buffer, 1024);
	g_pRandomDotAnaglyph->setMaxDisparity(etu::StringToInt(buffer, g_pRandomDotAnaglyph->getMaxDisparity()));

	g_pRandomDotAnaglyph->sendSettings(g_pSettings->theSerialComm);
}

void CRandomDotAnaglyphDlg::OnBnClickedOk()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "Random Dot Anaglyph Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
	g_pRandomDotAnaglyph->bStarted = false;
}

void CRandomDotAnaglyphDlg::OnBnClickedButtonapply() 
{
	OnBnClickedOk();
}
void CRandomDotAnaglyphDlg::OnBnClickedButton1() 
{
	char buffer[1024];
	ZeroMemory(buffer, 1024);
	m_DepthImage.GetWindowText(buffer, 1024);
	m_DepthImage.SetWindowText( etu::BrowseForFile(true, CString(".bmp"), CString("256 Color Bitmap"), CString(buffer)));
}


#endif
