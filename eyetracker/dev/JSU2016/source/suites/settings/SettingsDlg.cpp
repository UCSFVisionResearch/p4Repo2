/**
 * @file SettingsDlg.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "settingsdlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialog)
CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
{}

CSettingsDlg::~CSettingsDlg()
{}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EditDistance, m_Distance);
	DDX_Control(pDX, IDC_EditXOffset, m_XOffset);
	DDX_Control(pDX, IDC_EditYOffset, m_YOffset);
	DDX_Control(pDX, IDC_EditRed, m_Red);
	DDX_Control(pDX, IDC_EditGreen, m_Green);
	DDX_Control(pDX, IDC_EditBlue, m_Blue);
	DDX_Control(pDX, IDC_EditRedBG, m_RedBG);
	DDX_Control(pDX, IDC_EditGreenBG, m_GreenBG);
	DDX_Control(pDX, IDC_EditBlueBG, m_BlueBG);
	DDX_Control(pDX, IDC_EditDegPerVolt, m_DegPerVolt);
	DDX_Control(pDX, IDC_EditScreenXOffset, m_ScreenXOffset);
	DDX_Control(pDX, IDC_EditScreenYOffset, m_ScreenYOffset);
	DDX_Control(pDX, IDC_EditEyePosition, m_EyePosition);
	DDX_Control(pDX, IDC_EditScreenPosition, m_ScreenPosition);
	DDX_Control(pDX, IDC_DotStatic, m_DotStatic);
	DDX_Control(pDX, IDC_BackgroundStatic, m_BackgroundStatic);
	DDX_Control(pDX, IDC_PixelSize, m_PixelSize);
	DDX_Control(pDX, IDC_SPIN1, m_XOffsetSpin);
	DDX_Control(pDX, IDC_SPIN2, m_YOffsetSpin);
	DDX_Control(pDX, IDC_SPIN3, m_ScreenXOffsetSpin);
	DDX_Control(pDX, IDC_SPIN4, m_ScreenYOffsetSpin);
	DDX_Control(pDX, IDC_NSamples, m_NSamples);
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, OnDeltaposSpin4)
END_MESSAGE_MAP()

// CSettingsDlg message handlers
// CCalibrationDlg message handlers
BOOL CSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();
	g_pSettings->sendSettings(g_pSettings->theSerialComm);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingsDlg::load() 
{
	m_Distance.SetWindowText(etu::IntToString(g_pSettings->getViewDistance()));
	m_EyePosition.SetWindowText(etu::IntToString(g_pSettings->getEyePosition()));
	m_ScreenPosition.SetWindowText(etu::IntToString(g_pSettings->getScreenPosition()));

	m_Red.SetWindowText(etu::IntToString(g_pSettings->getRed()));
	m_Green.SetWindowText(etu::IntToString(g_pSettings->getGreen()));
	m_Blue.SetWindowText(etu::IntToString(g_pSettings->getBlue()));
	dotRGB = RGB(g_pSettings->getRed(), g_pSettings->getGreen(), g_pSettings->getBlue());

	m_RedBG.SetWindowText(etu::IntToString(g_pSettings->getRedBG()));
	m_GreenBG.SetWindowText(etu::IntToString(g_pSettings->getGreenBG()));
	m_BlueBG.SetWindowText(etu::IntToString(g_pSettings->getBlueBG()));
	bgRGB = RGB(g_pSettings->getRedBG(), g_pSettings->getGreenBG(), g_pSettings->getBlueBG());

	m_XOffset.SetWindowText(etu::FloatToString(g_pSettings->getXOffset()));
	m_YOffset.SetWindowText(etu::FloatToString(g_pSettings->getYOffset()));
	g_pEyeTrackingDlg->m_StimX.SetWindowText(etu::FloatToString(g_pSettings->getXOffset()));
	g_pEyeTrackingDlg->m_StimY.SetWindowText(etu::FloatToString(g_pSettings->getYOffset()));

	m_DegPerVolt.SetWindowText(etu::FloatToString(g_pSettings->getDegPerVolt()));
	m_ScreenXOffset.SetWindowText(etu::FloatToString(g_pSettings->getScreenXOffset()));
	m_ScreenYOffset.SetWindowText(etu::FloatToString(g_pSettings->getScreenYOffset()));
	g_pEyeTrackingDlg->m_ScreenX.SetWindowText(etu::FloatToString(g_pSettings->getScreenXOffset()));
	g_pEyeTrackingDlg->m_ScreenY.SetWindowText(etu::FloatToString(g_pSettings->getScreenYOffset()));
	m_PixelSize.SetWindowText(etu::FloatToString(g_pSettings->getPixelSize()));
	m_NSamples.SetWindowText(etu::IntToString(g_pSettings->getNSamples()));
}

void CSettingsDlg::save() 
{
	char buffer[100];

	m_EyePosition.GetWindowText(buffer, 100);
	g_pSettings->setEyePosition(etu::StringToInt(buffer, g_pSettings->getEyePosition()));

	m_ScreenPosition.GetWindowText(buffer, 100);
	g_pSettings->setScreenPosition(etu::StringToInt(buffer, g_pSettings->getScreenPosition()));

	g_pSettings->setRed(GetRValue(dotRGB));
	g_pSettings->setGreen(GetGValue(dotRGB));
	g_pSettings->setBlue(GetBValue(dotRGB));
	g_pSettings->setRedBG(GetRValue(bgRGB));
	g_pSettings->setGreenBG(GetGValue(bgRGB));
	g_pSettings->setBlueBG(GetBValue(bgRGB));

	g_pEyeTrackingDlg->m_StimX.GetWindowText(buffer, 100);
	g_pSettings->setXOffset(etu::StringToFloat(buffer, g_pSettings->getXOffset()));

	g_pEyeTrackingDlg->m_StimY.GetWindowText(buffer, 100);
	g_pSettings->setYOffset(etu::StringToFloat(buffer, g_pSettings->getYOffset()));
	
	m_DegPerVolt.GetWindowText(buffer, 100);
	g_pSettings->setDegPerVolt(etu::StringToFloat(buffer, g_pSettings->getDegPerVolt()));

	g_pEyeTrackingDlg->m_ScreenX.GetWindowText(buffer, 100);
	g_pSettings->setScreenXOffset(etu::StringToFloat(buffer, g_pSettings->getScreenXOffset()));

	g_pEyeTrackingDlg->m_ScreenY.GetWindowText(buffer, 100);
	g_pSettings->setScreenYOffset(etu::StringToFloat(buffer, g_pSettings->getScreenYOffset()));

	m_PixelSize.GetWindowText(buffer, 100);
	g_pSettings->setPixelSize(etu::StringToFloat(buffer, g_pSettings->getPixelSize()));

	m_NSamples.GetWindowText(buffer, 100);
	g_pSettings->setNSamples(etu::StringToInt(buffer, g_pSettings->getNSamples()));

	//send text mark
	g_pSettings->sendSettings(g_pSettings->theSerialComm);
}

void CSettingsDlg::OnPaint()
{
	CDialog::OnPaint();

	CRect rect;

	CPaintDC dc_d(&m_DotStatic);
	m_DotStatic.GetClientRect(rect);
	dc_d.FillSolidRect(rect, RGB(g_pSettings->getRed(),g_pSettings->getGreen(),g_pSettings->getBlue()));

	CPaintDC dc_b(&m_BackgroundStatic);
	m_BackgroundStatic.GetClientRect(rect);
	dc_b.FillSolidRect(rect, RGB(g_pSettings->getRedBG(),g_pSettings->getGreenBG(),g_pSettings->getBlueBG()));

}

void CSettingsDlg::OnBnClickedApply()
{
	bool bWasActive = false;
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() == "Pursuit Stimulus") {
		bWasActive = true;
		g_pEyeTrackingDlg->OnBnClickedRun();
	}

	int oldNSamples = g_pSettings->getNSamples();
	save();
	load();
	if (oldNSamples != g_pSettings->getNSamples()) {
		//changed number of samples
		g_pEyeTrackingDlg->lxn=0;
		g_pEyeTrackingDlg->lyn=0;
		g_pEyeTrackingDlg->rxn=0;
		g_pEyeTrackingDlg->ryn=0;
		if (g_pEyeTrackingDlg->lxs) { free(g_pEyeTrackingDlg->lxs); g_pEyeTrackingDlg->lxs = NULL; }
		if (g_pEyeTrackingDlg->lys) { free(g_pEyeTrackingDlg->lys); g_pEyeTrackingDlg->lys = NULL; }
		if (g_pEyeTrackingDlg->rxs) { free(g_pEyeTrackingDlg->rxs); g_pEyeTrackingDlg->rxs = NULL; }
		if (g_pEyeTrackingDlg->rys) { free(g_pEyeTrackingDlg->rys); g_pEyeTrackingDlg->rys = NULL; }
	}

	if (bWasActive) {
		while (g_pActiveStimulus)
			Sleep(100);
		g_pEyeTrackingDlg->OnBnClickedRun();
	}

	g_pEyeTrackingDlg->Invalidate();
}

void CSettingsDlg::OnBnClickedOk()
{	
	OnBnClickedApply();
}

void CSettingsDlg::OnBnClickedCancel()
{}

void CSettingsDlg::OnBnClickedButton1()
{
	// red
	m_Red.SetWindowText(etu::IntToString(255));
	m_Green.SetWindowText(etu::IntToString(0));
	m_Blue.SetWindowText(etu::IntToString(0));
}

void CSettingsDlg::OnBnClickedButton3()
{
	// blue
	m_Red.SetWindowText(etu::IntToString(0));
	m_Green.SetWindowText(etu::IntToString(0));
	m_Blue.SetWindowText(etu::IntToString(255));
}

void CSettingsDlg::OnBnClickedButton4()
{
	// purple
	m_Red.SetWindowText(etu::IntToString(255));
	m_Green.SetWindowText(etu::IntToString(0));
	m_Blue.SetWindowText(etu::IntToString(255));
}

void CSettingsDlg::OnBnClickedButton5()
{
	// white
	m_Red.SetWindowText(etu::IntToString(255));
	m_Green.SetWindowText(etu::IntToString(255));
	m_Blue.SetWindowText(etu::IntToString(255));
}

void CSettingsDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	char buffer[100];
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	m_XOffset.GetWindowText(buffer, 100);
	float val = etu::StringToFloat(buffer, g_pSettings->getXOffset());
	
	//pNMUpDown->iDelta		-1=up	1=down
	if (pNMUpDown->iDelta > 0) {
		m_XOffset.SetWindowText(etu::FloatToString(1.0*(float)ceil(1.0*val-1.0)));
	} 
	else {
		m_XOffset.SetWindowText(etu::FloatToString(1.0*(float)floor(1.0*val+1.0)));
	}

	*pResult = 0;
}

void CSettingsDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	char buffer[100];
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	m_YOffset.GetWindowText(buffer, 100);
	float val = etu::StringToFloat(buffer, g_pSettings->getYOffset());
	
	//pNMUpDown->iDelta		-1=up	1=down
	if (pNMUpDown->iDelta > 0) {
		m_YOffset.SetWindowText(etu::FloatToString(1.0*(float)ceil(1.0*val-1.0)));
	} 
	else {
		m_YOffset.SetWindowText(etu::FloatToString(1.0*(float)floor(1.0*val+1.0)));
	}

	*pResult = 0;
}

void CSettingsDlg::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	char buffer[100];
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	m_ScreenXOffset.GetWindowText(buffer, 100);
	float val = etu::StringToFloat(buffer, g_pSettings->getScreenXOffset());
	
	//pNMUpDown->iDelta		-1=up	1=down
	if (pNMUpDown->iDelta > 0) {
		m_ScreenXOffset.SetWindowText(etu::FloatToString(1.0*(float)ceil(1.0*val-1.0)));
	} 
	else {
		m_ScreenXOffset.SetWindowText(etu::FloatToString(1.0*(float)floor(1.0*val+1.0)));
	}

	*pResult = 0;
}

void CSettingsDlg::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	char buffer[100];
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	m_ScreenYOffset.GetWindowText(buffer, 100);
	float val = etu::StringToFloat(buffer, g_pSettings->getScreenYOffset());
	
	//pNMUpDown->iDelta		-1=up	1=down
	if (pNMUpDown->iDelta > 0) {
		m_ScreenYOffset.SetWindowText(etu::FloatToString(1.0*(float)ceil(1.0*val-1.0)));
	} 
	else {
		m_ScreenYOffset.SetWindowText(etu::FloatToString(1.0*(float)floor(1.0*val+1.0)));
	}

	*pResult = 0;
}
