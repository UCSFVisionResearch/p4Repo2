/**
 * @file RFMappingDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "RFMappingDlg.h"
#include "app/EyeTrackingDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CRFMappingDlg, CDialog)
CRFMappingDlg::CRFMappingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRFMappingDlg::IDD, pParent)
{
	lastPoint.x = 0;
	lastPoint.y = 0;
	currPoint.x = 0;
	currPoint.y = 0;
}

CRFMappingDlg::~CRFMappingDlg()
{}

void CRFMappingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_rfmCrossX, m_CrossX);
	DDX_Control(pDX, IDC_rfmCrossY, m_CrossY);
	DDX_Control(pDX, IDC_rfmCrossDiameter, m_CrossDiameter);
	DDX_Control(pDX, IDC_rfmCrossThickness, m_CrossThickness);

	DDX_Control(pDX, IDC_rfmBinocular, m_Binocular);
	DDX_Control(pDX, IDC_rfmCrossX2, m_CrossX2);
	DDX_Control(pDX, IDC_rfmCrossY2, m_CrossY2);
	DDX_Control(pDX, IDC_rfmSwapInt, m_SwapInt);

	DDX_Control(pDX, IDC_rfmPrimaryRed, m_PrimaryRed);
	DDX_Control(pDX, IDC_rfmPrimaryGreen, m_PrimaryGreen);
	DDX_Control(pDX, IDC_rfmPrimaryBlue, m_PrimaryBlue);
	DDX_Control(pDX, IDC_rfmPrimaryStatic, m_PrimaryStatic);

	DDX_Control(pDX, IDC_rfmSecondaryRed, m_SecondaryRed);
	DDX_Control(pDX, IDC_rfmSecondaryGreen, m_SecondaryGreen);
	DDX_Control(pDX, IDC_rfmSecondaryBlue, m_SecondaryBlue);
	DDX_Control(pDX, IDC_rfmSecondaryStatic, m_SecondaryStatic);

	DDX_Control(pDX, IDC_rfmTertiaryRed, m_TertiaryRed);
	DDX_Control(pDX, IDC_rfmTertiaryGreen, m_TertiaryGreen);
	DDX_Control(pDX, IDC_rfmTertiaryBlue, m_TertiaryBlue);
	DDX_Control(pDX, IDC_rfmTertiaryStatic, m_TertiaryStatic);

	DDX_Control(pDX, IDC_rfmCheckPrimaryL, m_usePrimaryL);
	DDX_Control(pDX, IDC_rfmCheckPrimaryR, m_usePrimaryR);
	DDX_Control(pDX, IDC_rfmCheckSecondaryL, m_useSecondaryL);
	DDX_Control(pDX, IDC_rfmCheckSecondaryR, m_useSecondaryR);
	DDX_Control(pDX, IDC_rfmCheckTertiaryL, m_useTertiaryL);
	DDX_Control(pDX, IDC_rfmCheckTertiaryR, m_useTertiaryR);

	DDX_Control(pDX, IDC_Circle, m_RadioCircle);
	DDX_Control(pDX, IDC_Bar, m_RadioBar);
	DDX_Control(pDX, IDC_CircleRadius, m_CircleRadius);
	DDX_Control(pDX, IDC_BarWidth, m_BarWidth);
	DDX_Control(pDX, IDC_BarHeight, m_BarHeight);
	DDX_Control(pDX, IDC_Orientation, m_Orientation);

	DDX_Control(pDX, IDC_rfmUseMouse, m_RadioUseMouse);
	DDX_Control(pDX, IDC_rfmUseGrid, m_RadioUseGrid);
	DDX_Control(pDX, IDC_rfmStepSizeDeg, m_StepSizeDeg);
	DDX_Control(pDX, IDC_rfmStepsX, m_StepsX);
	DDX_Control(pDX, IDC_rfmStepsY, m_StepsY);
	DDX_Control(pDX, IDC_rfmDisplayTimeMs, m_DisplayTimeMs);

	DDX_Control(pDX, IDC_rfmBlanks, m_Blanks);
	DDX_Control(pDX, IDC_rfmBlankTimeMs, m_BlankTimeMs);

	DDX_Control(pDX, IDC_Pulse, m_EnablePulse);

	DDX_Control(pDX, IDC_XPosition, m_XPosition);
	DDX_Control(pDX, IDC_YPosition, m_YPosition);
	DDX_Control(pDX, IDC_Width, m_Width);
	DDX_Control(pDX, IDC_Height, m_Height);
		
	DDX_Control(pDX, IDC_StaticLeftEye, m_LeftEyeField);
	DDX_Control(pDX, IDC_StaticRightEye, m_RightEyeField);
	
	DDX_Control(pDX, IDC_SaveAsLeft, m_SaveAsLeft);
	DDX_Control(pDX, IDC_SaveAsRight, m_SaveAsRight);
	DDX_Control(pDX, IDC_APPLY, m_Apply);

	/* Noise controls */
	DDX_Control(pDX, IDC_rfmUseNoise, m_UseNoise);
	DDX_Control(pDX, IDC_rfmSameBG, m_SameBG);
	DDX_Control(pDX, IDC_rfmInterleaveNoise, m_InterleaveNoise);
	DDX_Control(pDX, IDC_rfmOccludeBG, m_OccludeBG);
	DDX_Control(pDX, IDC_rfmNoiseSize, m_NoiseSize);
	DDX_Control(pDX, IDC_rfmNoisePct, m_NoisePct);
	DDX_Control(pDX, IDC_rfmNoiseRed, m_NoiseRed);
	DDX_Control(pDX, IDC_rfmNoiseGreen, m_NoiseGreen);
	DDX_Control(pDX, IDC_rfmNoiseBlue, m_NoiseBlue);
	DDX_Control(pDX, IDC_rfmNoiseStatic, m_NoiseStatic);

	DDX_Control(pDX, IDC_rfmDebug, m_debug);

	DDX_Control(pDX, IDC_rfmEyePosnDisplayLX, m_EyePosnDisplayLX);
	DDX_Control(pDX, IDC_rfmEyePosnDisplayLY, m_EyePosnDisplayLY);
	DDX_Control(pDX, IDC_rfmEyePosnDisplayRX, m_EyePosnDisplayRX);
	DDX_Control(pDX, IDC_rfmEyePosnDisplayRY, m_EyePosnDisplayRY);

	DDX_Control(pDX, IDC_rfmWindowAllowancePix, m_WindowAllowancePix);
	DDX_Control(pDX, IDC_rfmWait, m_Wait);

}

BEGIN_MESSAGE_MAP(CRFMappingDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_rfmBinocular, OnBnClickedBinocular)
	ON_BN_CLICKED(IDC_SaveAsLeft, OnBnClickedSaveasleft)
	ON_BN_CLICKED(IDC_SaveAsRight, OnBnClickedSaveasright)
	ON_BN_CLICKED(IDC_Circle, OnBnClickedCircle)
	ON_BN_CLICKED(IDC_Bar, OnBnClickedBar)
	ON_BN_CLICKED(IDC_rfmUseMouse, OnBnClickedUseMouse)
	ON_BN_CLICKED(IDC_rfmUseGrid, OnBnClickedUseGrid)
	ON_BN_CLICKED(IDC_rfmBlanks, OnBnClickedBlanks)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDC_rfmCheckPrimaryL, OnBnClickedPrimaryL)
	ON_BN_CLICKED(IDC_rfmCheckPrimaryR, OnBnClickedPrimaryR)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_rfmUseNoise, OnBnClickedUseNoise)
END_MESSAGE_MAP()

BOOL CRFMappingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();
	OnBnClickedSaveasright();
	OnBnClickedSaveasleft();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRFMappingDlg::load() 
{
	m_CrossX.SetWindowText(etu::FloatToString(g_pRFMapping->getCrossX()));
	m_CrossY.SetWindowText(etu::FloatToString(g_pRFMapping->getCrossY()));
	m_CrossDiameter.SetWindowText(etu::FloatToString(g_pRFMapping->getCrossDiameter()));
	m_CrossThickness.SetWindowText(etu::FloatToString(g_pRFMapping->getCrossThickness()));

	m_Binocular.SetCheck(g_pRFMapping->getUseBinocular() ? BST_CHECKED : BST_UNCHECKED);
	m_CrossX2.SetWindowText(etu::FloatToString(g_pRFMapping->getCrossX2()));
	m_CrossY2.SetWindowText(etu::FloatToString(g_pRFMapping->getCrossY2()));
	m_SwapInt.SetWindowText(etu::FloatToString(g_pRFMapping->getSwapInt()));

	m_PrimaryRed.SetWindowText(etu::IntToString(g_pRFMapping->getPrimaryRed()));
	m_PrimaryGreen.SetWindowText(etu::IntToString(g_pRFMapping->getPrimaryGreen()));
	m_PrimaryBlue.SetWindowText(etu::IntToString(g_pRFMapping->getPrimaryBlue()));
	m_SecondaryRed.SetWindowText(etu::IntToString(g_pRFMapping->getSecondaryRed()));
	m_SecondaryGreen.SetWindowText(etu::IntToString(g_pRFMapping->getSecondaryGreen()));
	m_SecondaryBlue.SetWindowText(etu::IntToString(g_pRFMapping->getSecondaryBlue()));
	m_TertiaryRed.SetWindowText(etu::IntToString(g_pRFMapping->getTertiaryRed()));
	m_TertiaryGreen.SetWindowText(etu::IntToString(g_pRFMapping->getTertiaryGreen()));
	m_TertiaryBlue.SetWindowText(etu::IntToString(g_pRFMapping->getTertiaryBlue()));
	m_usePrimaryR.SetCheck((g_pRFMapping->usePrimaryR())? BST_CHECKED : BST_UNCHECKED);
	m_usePrimaryL.SetCheck((g_pRFMapping->usePrimaryL())? BST_CHECKED : BST_UNCHECKED);
	m_useSecondaryR.SetCheck((g_pRFMapping->useSecondaryR())? BST_CHECKED : BST_UNCHECKED);
	m_useSecondaryL.SetCheck((g_pRFMapping->useSecondaryL())? BST_CHECKED : BST_UNCHECKED);
	m_useTertiaryR.SetCheck((g_pRFMapping->useTertiaryR())? BST_CHECKED : BST_UNCHECKED);
	m_useTertiaryL.SetCheck((g_pRFMapping->useTertiaryL())? BST_CHECKED : BST_UNCHECKED);
	m_NoiseRed.SetWindowText(etu::IntToString(g_pRFMapping->getNoiseRed()));
	m_NoiseGreen.SetWindowText(etu::IntToString(g_pRFMapping->getNoiseGreen()));
	m_NoiseBlue.SetWindowText(etu::IntToString(g_pRFMapping->getNoiseBlue()));
	m_NoisePct.SetWindowText(etu::IntToString(g_pRFMapping->getNoisePct()));
	m_NoiseSize.SetWindowText(etu::IntToString(g_pRFMapping->getNoiseSize()));
	m_UseNoise.SetCheck((g_pRFMapping->getUseNoise())? BST_CHECKED : BST_UNCHECKED);
	m_InterleaveNoise.SetCheck((g_pRFMapping->getInterleaveNoise())? BST_CHECKED : BST_UNCHECKED);
	m_SameBG.SetCheck((g_pRFMapping->getSameBG())? BST_CHECKED : BST_UNCHECKED);
	m_OccludeBG.SetCheck((g_pRFMapping->getOccludeBG())? BST_CHECKED : BST_UNCHECKED);


	m_CircleRadius.SetWindowText(etu::FloatToString(g_pRFMapping->getCircleRadius()));
	m_BarWidth.SetWindowText(etu::FloatToString(g_pRFMapping->getBarWidth()));
	m_BarHeight.SetWindowText(etu::FloatToString(g_pRFMapping->getBarHeight()));
	m_Orientation.SetWindowText(etu::FloatToString(g_pRFMapping->getOrientation()));	

	m_StepSizeDeg.SetWindowText(etu::FloatToString(g_pRFMapping->getStepSizeDeg()));
	m_StepsX.SetWindowText(etu::IntToString(g_pRFMapping->getStepsX()));
	m_StepsY.SetWindowText(etu::IntToString(g_pRFMapping->getStepsY()));
	m_DisplayTimeMs.SetWindowText(etu::IntToString(g_pRFMapping->getDisplayTimeMs()));
	m_Blanks.SetCheck(g_pRFMapping->getUseBlanks() ? BST_CHECKED : BST_UNCHECKED);
	m_BlankTimeMs.SetWindowTextA(etu::IntToString(g_pRFMapping->getBlankTimeMs()));
	OnBnClickedBlanks();

	m_XPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getXPosition()));
	m_YPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getYPosition()));
	m_Width.SetWindowText(etu::FloatToString(g_pRFMapping->getWidth()));
	m_Height.SetWindowText(etu::FloatToString(g_pRFMapping->getHeight()));

	m_WindowAllowancePix.SetWindowTextA(etu::FloatToString(g_pRFMapping->getWindowAllowancePix()));
	m_Wait.SetCheck(g_pRFMapping->getUseWait()?BST_CHECKED:BST_UNCHECKED);

	if(g_pRFMapping->getUseBinocular()){
		OnBnClickedBinocular();
	}
	
	if (g_pRFMapping->useCircle()) {
		OnBnClickedCircle();
	} 
	else {
		OnBnClickedBar();
	}

	if (g_pRFMapping->getUseMouse()) {
		OnBnClickedUseMouse();
	}
	else {
		OnBnClickedUseGrid();
	}

	m_EnablePulse.SetCheck(g_pRFMapping->usePulse());

	PaintColorStatics();
}

void CRFMappingDlg::save()
{
	char buffer[100];

	m_CrossX.GetWindowText(buffer, 100);
	g_pRFMapping->setCrossX(etu::StringToFloat(buffer, g_pRFMapping->getCrossX()));

	m_CrossY.GetWindowText(buffer, 100);
	g_pRFMapping->setCrossY(etu::StringToFloat(buffer, g_pRFMapping->getCrossY()));

	m_CrossDiameter.GetWindowText(buffer, 100);
	g_pRFMapping->setCrossDiameter(etu::StringToFloat(buffer, g_pRFMapping->getCrossDiameter()));

	m_CrossThickness.GetWindowText(buffer, 100);
	g_pRFMapping->setCrossThickness(etu::StringToFloat(buffer, g_pRFMapping->getCrossThickness()));

	g_pRFMapping->setUseBinocular(m_Binocular.GetCheck() == BST_CHECKED);
	
	m_CrossX2.GetWindowText(buffer, 100);
	g_pRFMapping->setCrossX2(etu::StringToFloat(buffer, g_pRFMapping->getCrossX2()));

	m_CrossY2.GetWindowText(buffer, 100);
	g_pRFMapping->setCrossY2(etu::StringToFloat(buffer, g_pRFMapping->getCrossY2()));

	m_SwapInt.GetWindowText(buffer, 100);
	g_pRFMapping->setSwapInt(etu::StringToFloat(buffer, g_pRFMapping->getSwapInt()));

	m_PrimaryRed.GetWindowTextA(buffer, 100);
	g_pRFMapping->setPrimaryRed(etu::StringToInt(buffer, g_pRFMapping->getPrimaryRed()));
	m_PrimaryGreen.GetWindowTextA(buffer, 100);
	g_pRFMapping->setPrimaryGreen(etu::StringToInt(buffer, g_pRFMapping->getPrimaryGreen()));
	m_PrimaryBlue.GetWindowTextA(buffer, 100);
	g_pRFMapping->setPrimaryBlue(etu::StringToInt(buffer, g_pRFMapping->getPrimaryBlue()));

	m_SecondaryRed.GetWindowTextA(buffer, 100);
	g_pRFMapping->setSecondaryRed(etu::StringToInt(buffer, g_pRFMapping->getSecondaryRed()));
	m_SecondaryGreen.GetWindowTextA(buffer, 100);
	g_pRFMapping->setSecondaryGreen(etu::StringToInt(buffer, g_pRFMapping->getSecondaryGreen()));
	m_SecondaryBlue.GetWindowTextA(buffer, 100);
	g_pRFMapping->setSecondaryBlue(etu::StringToInt(buffer, g_pRFMapping->getSecondaryBlue()));

	m_TertiaryRed.GetWindowTextA(buffer, 100);
	g_pRFMapping->setTertiaryRed(etu::StringToInt(buffer, g_pRFMapping->getTertiaryRed()));
	m_TertiaryGreen.GetWindowTextA(buffer, 100);
	g_pRFMapping->setTertiaryGreen(etu::StringToInt(buffer, g_pRFMapping->getTertiaryGreen()));
	m_TertiaryBlue.GetWindowTextA(buffer, 100);
	g_pRFMapping->setTertiaryBlue(etu::StringToInt(buffer, g_pRFMapping->getTertiaryBlue()));

	g_pRFMapping->setPrimaryR(m_usePrimaryR.GetCheck() == BST_CHECKED);
	g_pRFMapping->setPrimaryL(m_usePrimaryL.GetCheck() == BST_CHECKED);
	g_pRFMapping->setSecondaryR(m_useSecondaryR.GetCheck() == BST_CHECKED);
	g_pRFMapping->setSecondaryL(m_useSecondaryL.GetCheck() == BST_CHECKED);
	g_pRFMapping->setTertiaryR(m_useTertiaryR.GetCheck() == BST_CHECKED);
	g_pRFMapping->setTertiaryL(m_useTertiaryL.GetCheck() == BST_CHECKED);

	m_NoiseRed.GetWindowTextA(buffer, 100);
	g_pRFMapping->setNoiseRed(etu::StringToInt(buffer, g_pRFMapping->getNoiseRed()));
	m_NoiseGreen.GetWindowTextA(buffer, 100);
	g_pRFMapping->setNoiseGreen(etu::StringToInt(buffer, g_pRFMapping->getNoiseGreen()));
	m_NoiseBlue.GetWindowTextA(buffer, 100);
	g_pRFMapping->setNoiseBlue(etu::StringToInt(buffer, g_pRFMapping->getNoiseBlue()));
	
	g_pRFMapping->setUseNoise(m_UseNoise.GetCheck() == BST_CHECKED);
	g_pRFMapping->setInterleaveNoise(m_InterleaveNoise.GetCheck() == BST_CHECKED);
	g_pRFMapping->setSameBG(m_SameBG.GetCheck() == BST_CHECKED);
	g_pRFMapping->setOccludeBG(m_OccludeBG.GetCheck() == BST_CHECKED);

	//TODo: Get Rid of Hack. Drawing without noise is disabled. Find out cause.
	if(m_UseNoise.GetCheck() == BST_CHECKED){
		m_NoisePct.GetWindowText(buffer, 100);
		g_pRFMapping->setNoisePct(etu::StringToInt(buffer, g_pRFMapping->getNoisePct()));
		m_NoiseSize.GetWindowText(buffer, 100);
		g_pRFMapping->setNoiseSize(etu::StringToInt(buffer, g_pRFMapping->getNoiseSize()));
	}else{
		g_pRFMapping->setNoisePct(0);
		g_pRFMapping->setNoiseSize(0);
	}

	m_CircleRadius.GetWindowText(buffer, 100);
	g_pRFMapping->setCircleRadius(etu::StringToFloat(buffer, g_pRFMapping->getCircleRadius()));

	m_BarWidth.GetWindowText(buffer, 100);
	g_pRFMapping->setBarWidth(etu::StringToFloat(buffer, g_pRFMapping->getBarWidth()));

	m_BarHeight.GetWindowText(buffer, 100);
	g_pRFMapping->setBarHeight(etu::StringToFloat(buffer, g_pRFMapping->getBarHeight()));

	m_Orientation.GetWindowText(buffer, 100);
	g_pRFMapping->setOrientation(etu::StringToFloat(buffer, g_pRFMapping->getOrientation()));

	m_StepSizeDeg.GetWindowText(buffer, 100);
	g_pRFMapping->setStepSizeDeg(etu::StringToFloat(buffer, g_pRFMapping->getStepSizeDeg()));

	m_StepsX.GetWindowText(buffer, 100);
	g_pRFMapping->setStepsX(etu::StringToInt(buffer, g_pRFMapping->getStepsX()));

	m_StepsY.GetWindowText(buffer, 100);
	g_pRFMapping->setStepsY(etu::StringToInt(buffer, g_pRFMapping->getStepsY()));

	m_DisplayTimeMs.GetWindowText(buffer, 100);
	g_pRFMapping->setDisplayTimeMs(etu::StringToInt(buffer, g_pRFMapping->getDisplayTimeMs()));

	g_pRFMapping->setUseBlanks(m_Blanks.GetCheck() == BST_CHECKED);

	m_BlankTimeMs.GetWindowTextA(buffer, 100);
	g_pRFMapping->setBlankTimeMs(etu::StringToInt(buffer, g_pRFMapping->getBlankTimeMs()));

	m_CircleRadius.GetWindowText(buffer, 100);
	g_pRFMapping->setCircleRadius(etu::StringToFloat(buffer, g_pRFMapping->getCircleRadius()));

	m_XPosition.GetWindowText(buffer, 100);
	g_pRFMapping->setXPosition(etu::StringToFloat(buffer, g_pRFMapping->getXPosition()));

	m_YPosition.GetWindowText(buffer, 100);
	g_pRFMapping->setYPosition(etu::StringToFloat(buffer, g_pRFMapping->getYPosition()));

	m_Width.GetWindowText(buffer, 100);
	g_pRFMapping->setWidth(etu::StringToFloat(buffer, g_pRFMapping->getWidth()));

	m_Height.GetWindowText(buffer, 100);
	g_pRFMapping->setHeight(etu::StringToFloat(buffer, g_pRFMapping->getHeight()));

	m_WindowAllowancePix.GetWindowTextA(buffer,100);
	g_pRFMapping->setWindowAllowancePix(etu::StringToFloat(buffer,g_pRFMapping->getWindowAllowancePix()));

	if (m_RadioCircle.GetCheck() == BST_CHECKED) {
		g_pRFMapping->setCircle();
	} 
	else {
		g_pRFMapping->setBar();
	}

	if (m_RadioUseMouse.GetCheck() == BST_CHECKED) {
		g_pRFMapping->setUseMouse();
	}
	else {
		g_pRFMapping->setUseGrid();
	}

	g_pRFMapping->setPulse(m_EnablePulse.GetCheck() == BST_CHECKED);
	g_pRFMapping->setUseWait(m_Wait.GetCheck()==BST_CHECKED);

	PaintColorStatics();
}

void CRFMappingDlg::OnPaint() 
{
	CDialog::OnPaint();
	PaintColorStatics();
}

void CRFMappingDlg::PaintColorStatics()
{
	CRect rect;
	DWORD primaryColor, secondaryColor, tertiaryColor;
	DWORD noiseColor;

	primaryColor = RGB(g_pRFMapping->getPrimaryRed(), g_pRFMapping->getPrimaryGreen(), g_pRFMapping->getPrimaryBlue());
	secondaryColor = RGB(g_pRFMapping->getSecondaryRed(), g_pRFMapping->getSecondaryGreen(), g_pRFMapping->getSecondaryBlue());
	tertiaryColor = RGB(g_pRFMapping->getTertiaryRed(), g_pRFMapping->getTertiaryGreen(), g_pRFMapping->getTertiaryBlue());
	noiseColor = RGB(g_pRFMapping->getNoiseRed(), g_pRFMapping->getNoiseGreen(), g_pRFMapping->getNoiseBlue());

	CPaintDC dc_p(&m_PrimaryStatic);
	m_PrimaryStatic.GetClientRect(rect);
	dc_p.FillSolidRect(rect, primaryColor);

	CPaintDC dc_s(&m_SecondaryStatic);
	m_SecondaryStatic.GetClientRect(rect);
	dc_s.FillSolidRect(rect, secondaryColor);

	CPaintDC dc_t(&m_TertiaryStatic);
	m_TertiaryStatic.GetClientRect(rect);
	dc_t.FillSolidRect(rect, tertiaryColor);

	CPaintDC dc_n(&m_NoiseStatic);
	m_NoiseStatic.GetClientRect(rect);
	dc_n.FillSolidRect(rect, noiseColor);
}

void CRFMappingDlg::OnBnClickedOk()
{
	OnBnClickedApply();
}

void CRFMappingDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CRFMappingDlg::OnBnClickedBinocular()
{
	if (m_Binocular.GetCheck()==BST_CHECKED){
		m_CrossX2.EnableWindow(true);
		m_CrossY2.EnableWindow(true);
		m_SwapInt.EnableWindow(true);
		m_usePrimaryL.EnableWindow(true);
		m_usePrimaryR.EnableWindow(true);
		m_useSecondaryL.EnableWindow(true);
		m_useSecondaryR.EnableWindow(true);
		m_useTertiaryL.EnableWindow(true);
		m_useTertiaryR.EnableWindow(true);
	}else{
		m_CrossX2.EnableWindow(false);
		m_CrossY2.EnableWindow(false);
		m_SwapInt.EnableWindow(false);
		m_usePrimaryR.SetCheck(true);
		m_usePrimaryR.EnableWindow(true);
		m_usePrimaryL.SetCheck(false);
		m_usePrimaryL.EnableWindow(true);
		m_useSecondaryR.SetCheck(false);
		m_useSecondaryR.EnableWindow(false);
		m_useSecondaryL.SetCheck(false);
		m_useSecondaryL.EnableWindow(false);
		m_useTertiaryR.SetCheck(false);
		m_useTertiaryR.EnableWindow(false);
		m_useTertiaryL.SetCheck(false);
		m_useTertiaryL.EnableWindow(false);


	}
	

}

void CRFMappingDlg::OnBnClickedSaveasleft()
{
	save();
	g_receptiveFieldLeft.xPos = g_pRFMapping->getXPosition();
	g_receptiveFieldLeft.yPos = g_pRFMapping->getYPosition();
	g_receptiveFieldLeft.width = g_pRFMapping->getWidth();
	g_receptiveFieldLeft.height = g_pRFMapping->getHeight();
	g_receptiveFieldLeft.barWidth = g_pRFMapping->getBarWidth();
	g_receptiveFieldLeft.barHeight = g_pRFMapping->getBarHeight();
	g_receptiveFieldLeft.orientation = g_pRFMapping->getOrientation();
	g_receptiveFieldLeft.circleRadius = g_pRFMapping->getCircleRadius();
	updateFieldString(g_receptiveFieldLeft, true);
}

void CRFMappingDlg::OnBnClickedSaveasright()
{
	save();
	g_receptiveFieldRight.xPos = g_pRFMapping->getXPosition();
	g_receptiveFieldRight.yPos = g_pRFMapping->getYPosition();
	g_receptiveFieldRight.width = g_pRFMapping->getWidth();
	g_receptiveFieldRight.height = g_pRFMapping->getHeight();
	g_receptiveFieldRight.barWidth = g_pRFMapping->getBarWidth();
	g_receptiveFieldRight.barHeight = g_pRFMapping->getBarHeight();
	g_receptiveFieldRight.orientation = g_pRFMapping->getOrientation();
	g_receptiveFieldRight.circleRadius = g_pRFMapping->getCircleRadius();
	updateFieldString(g_receptiveFieldRight, false);
}

void CRFMappingDlg::OnBnClickedCircle()
{
	m_RadioCircle.SetCheck(BST_CHECKED);
	m_RadioBar.SetCheck(BST_UNCHECKED);

	if (m_RadioUseMouse.GetCheck() == BST_UNCHECKED) {
		m_CircleRadius.EnableWindow(true);
		m_BarWidth.EnableWindow(false);
		m_BarHeight.EnableWindow(false);
		m_Orientation.EnableWindow(false);
	}
}

void CRFMappingDlg::OnBnClickedBar()
{
	m_RadioCircle.SetCheck(BST_UNCHECKED);
	m_RadioBar.SetCheck(BST_CHECKED);

	if (m_RadioUseMouse.GetCheck() == BST_UNCHECKED) {
		m_CircleRadius.EnableWindow(false);
		m_BarWidth.EnableWindow(true);
		m_BarHeight.EnableWindow(true);
		m_Orientation.EnableWindow(true);
	}
}

void CRFMappingDlg::OnBnClickedUseMouse()
{
	m_RadioUseMouse.SetCheck(BST_CHECKED);
	m_RadioUseGrid.SetCheck(BST_UNCHECKED);

	m_CircleRadius.EnableWindow(false);
	m_BarWidth.EnableWindow(false);
	m_BarHeight.EnableWindow(false);
	m_Orientation.EnableWindow(false);

	m_StepSizeDeg.EnableWindow(false);
	m_StepsX.EnableWindow(false);
	m_StepsY.EnableWindow(false);
	m_DisplayTimeMs.EnableWindow(false);
	m_Blanks.EnableWindow(false);
	m_BlankTimeMs.EnableWindow(false);
	m_EnablePulse.EnableWindow(true);
	m_Wait.SetCheck(false);
	m_Wait.EnableWindow(false);
//Disable Use of noise
	m_UseNoise.SetCheck(BST_UNCHECKED);
	m_SameBG.SetCheck(BST_UNCHECKED);
	m_InterleaveNoise.SetCheck(BST_UNCHECKED);
	m_OccludeBG.SetCheck(BST_UNCHECKED);
	m_UseNoise.EnableWindow(false);
	m_SameBG.EnableWindow(false);
	m_InterleaveNoise.EnableWindow(false);
	m_OccludeBG.EnableWindow(false);
	m_NoisePct.EnableWindow(false);
	m_NoiseSize.EnableWindow(false);
	m_NoiseRed.EnableWindow(false);
	m_NoiseGreen.EnableWindow(false);
	m_NoiseBlue.EnableWindow(false);
	
}

void CRFMappingDlg::OnBnClickedUseGrid()
{
	m_RadioUseMouse.SetCheck(BST_UNCHECKED);
	m_RadioUseGrid.SetCheck(BST_CHECKED);

	if (m_RadioCircle.GetCheck() == BST_CHECKED) {
		m_CircleRadius.EnableWindow(true);
	}
	else {
		m_BarWidth.EnableWindow(true);
		m_BarHeight.EnableWindow(true);
		m_Orientation.EnableWindow(true);
	}
	m_StepSizeDeg.EnableWindow(true);
	m_StepsX.EnableWindow(true);
	m_StepsY.EnableWindow(true);
	m_DisplayTimeMs.EnableWindow(true);
	m_Blanks.SetCheck(false);
	m_Blanks.EnableWindow(false);
	m_EnablePulse.SetCheck(false);
	m_EnablePulse.EnableWindow(false);
	OnBnClickedBlanks();
	
	m_UseNoise.EnableWindow(true);
	m_SameBG.EnableWindow(true);
	m_InterleaveNoise.EnableWindow(true);
	m_OccludeBG.EnableWindow(true);
	m_NoisePct.EnableWindow(true);
	m_NoiseSize.EnableWindow(true);
	m_NoiseRed.EnableWindow(true);
	m_NoiseGreen.EnableWindow(true);
	m_NoiseBlue.EnableWindow(true);
	//Previous request that all boxes be check automatically
	//m_UseNoise.SetCheck(true);
	//m_SameBG.SetCheck(true);

	//m_usePrimaryL.SetCheck(true);
	//m_usePrimaryR.SetCheck(true);
	//m_useSecondaryL.SetCheck(true);
	//m_useSecondaryR.SetCheck(true);
	//m_useTertiaryL.SetCheck(true);
	//m_useTertiaryR.SetCheck(true);

	m_Wait.EnableWindow(true);

}

void CRFMappingDlg::OnBnClickedBlanks()
{
	m_BlankTimeMs.EnableWindow(m_Blanks.GetCheck() == BST_CHECKED);
}

void CRFMappingDlg::OnBnClickedPrimaryL()
{
	if(m_Binocular.GetCheck() == BST_UNCHECKED){
		if(m_usePrimaryL.GetCheck()==BST_CHECKED){
			m_usePrimaryR.SetCheck(false);
		}else{
			m_usePrimaryR.SetCheck(true);
		}
	}

}

void CRFMappingDlg::OnBnClickedPrimaryR()
{
	if(m_Binocular.GetCheck() == BST_UNCHECKED){
		if(m_usePrimaryR.GetCheck()==BST_CHECKED){
			m_usePrimaryL.SetCheck(false);
		}else{
			m_usePrimaryL.SetCheck(true);
		}
	}
}

void CRFMappingDlg::OnBnClickedApply()
{
	if (g_pActiveStimulus  && g_pActiveStimulus->StimulusType() == "RF Mapping Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
	this->Invalidate(false);
}

void CRFMappingDlg::updateFieldString(CReceptiveField rf, bool bLeft) 
{
	char buffer[200];
	sprintf(buffer, \
		"X Position:\t%.3f deg\n\n" \
		"Y Position:\t%.3f deg\n\n" \
		"Width:\t\t%.3f deg\n\n" \
		"Height:\t\t%.3f deg\n\n" \
		"Bar Width:\t%.3f deg\n\n" \
		"Bar Height:\t%.3f deg\n\n" \
		"Orientation:\t%.3f deg\n\n" \
		"Circle Radius:\t%.3f deg", 
		rf.xPos, rf.yPos, rf.width, rf.height, 
		rf.barWidth, rf.barHeight, rf.orientation, rf.circleRadius);
	if (bLeft) {
		m_LeftEyeField.SetWindowText(buffer);
	} 
	else {
		m_RightEyeField.SetWindowText(buffer);
	}	
}

void CRFMappingDlg::updateFieldResults()
{
	m_XPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getXPosition()-g_pRFMapping->getCrossX()));
	m_YPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getYPosition()-g_pRFMapping->getCrossY()));
	m_Width.SetWindowText(etu::FloatToString(g_pRFMapping->getWidth()));
	m_Height.SetWindowText(etu::FloatToString(g_pRFMapping->getHeight()));
}

/* Disable controls that should be disabled when actively running stimulus. */
void CRFMappingDlg::disableControls() 
{
	m_XPosition.EnableWindow(false);
	m_YPosition.EnableWindow(false);
	m_Width.EnableWindow(false);
	m_Height.EnableWindow(false);

	m_RadioUseMouse.EnableWindow(false);
	m_RadioUseGrid.EnableWindow(false);
	m_StepSizeDeg.EnableWindow(false);
	m_StepsX.EnableWindow(false);
	m_StepsY.EnableWindow(false);
	m_DisplayTimeMs.EnableWindow(false);

	m_EnablePulse.EnableWindow(false);

	m_Apply.EnableWindow(false);
	
	m_SaveAsLeft.EnableWindow(false);
	m_SaveAsRight.EnableWindow(false);
	
	m_RadioCircle.EnableWindow(false);
	m_RadioBar.EnableWindow(false);
	m_BarWidth.EnableWindow(false);
	m_BarHeight.EnableWindow(false);
	m_Orientation.EnableWindow(false);
	m_CircleRadius.EnableWindow(false);
}

/* Re-enable controls now that the stimulus is no longer running. */
void CRFMappingDlg::enableControls() 
{
	m_XPosition.EnableWindow(true);
	m_YPosition.EnableWindow(true);
	m_Width.EnableWindow(true);
	m_Height.EnableWindow(true);

	m_RadioUseMouse.EnableWindow(true);
	m_RadioUseGrid.EnableWindow(true);

	m_EnablePulse.EnableWindow(true);

	m_Apply.EnableWindow(true);
	m_SaveAsLeft.EnableWindow(true);
	m_SaveAsRight.EnableWindow(true);
	m_RadioCircle.EnableWindow(true);
	m_RadioBar.EnableWindow(true);
	
	if (m_RadioBar.GetCheck() == BST_CHECKED) {
		OnBnClickedBar();
	} 
	else {
		OnBnClickedCircle();
	}

	if (m_RadioUseMouse.GetCheck() == BST_CHECKED) {
		OnBnClickedUseMouse();
	}
	else {
		OnBnClickedUseGrid();
	}
}

void CRFMappingDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (g_pRFMapping->getUseMouse() && g_pActiveStimulus  && g_pActiveStimulus->StimulusType() == "RF Mapping Stimulus") {
		if (MK_LBUTTON & nFlags) {
			float pixelsX = (float)(point.x - lastPoint.x);
			float pixelsY = (float)(point.y - lastPoint.y);
			((CRFMappingStimulus*)g_pActiveStimulus)->updateStimulus((int)pixelsX, (int)pixelsY, true);
			Sleep(1);
			updateFieldResults();
		} 
		else if (MK_RBUTTON & nFlags) {
			float pixelsX = (float)(point.x - lastPoint.x);
			float pixelsY = (float)(point.y - lastPoint.y);
			((CRFMappingStimulus*)g_pActiveStimulus)->updateStimulus((int)pixelsX, (int)pixelsY, false);
			Sleep(1);
			updateFieldResults();

		} 
		else {
			CRect area;
			this->GetClientRect(&area);
			int pixW = area.Width();
			int pixH = area.Height();

			currPoint.x = point.x;
			currPoint.y = point.y;
			
			((CRFMappingStimulus*)g_pActiveStimulus)->updatePosition((float)currPoint.x / (float)pixW, 
				(float)currPoint.y / (float)pixH); 
			
			updateFieldResults();
			
		}

		lastPoint.x = point.x;
		lastPoint.y = point.y;
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CRFMappingDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// locks target
	if (g_pActiveStimulus  && g_pActiveStimulus->StimulusType() == "RF Mapping Stimulus") {
		((CRFMappingStimulus*)g_pActiveStimulus)->StopExecution();
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CRFMappingDlg::OnBnClickedUseNoise()
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

void CRFMappingDlg::printDebug(LPCTSTR text){
	m_debug.SetWindowText(text);
}

void CRFMappingDlg::UpdateEyePositions(double lxDeg, double lyDeg, 
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
#endif
