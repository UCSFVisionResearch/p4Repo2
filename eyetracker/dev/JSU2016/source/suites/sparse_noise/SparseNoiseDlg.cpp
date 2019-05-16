/**
 * @file SparseNoiseDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "SparseNoiseDlg.h"
#include "app/EyeTrackingDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CSparseNoiseDlg, CDialog)
CSparseNoiseDlg::CSparseNoiseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSparseNoiseDlg::IDD, pParent)
{

}

CSparseNoiseDlg::~CSparseNoiseDlg()
{}

void CSparseNoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_snPrimaryRed, m_PrimaryRed);
	DDX_Control(pDX, IDC_snPrimaryGreen, m_PrimaryGreen);
	DDX_Control(pDX, IDC_snPrimaryBlue, m_PrimaryBlue);
	DDX_Control(pDX, IDC_sparsePrimaryStatic, m_PrimaryStatic);

	DDX_Control(pDX, IDC_snSecondaryRed, m_SecondaryRed);
	DDX_Control(pDX, IDC_snSecondaryGreen, m_SecondaryGreen);
	DDX_Control(pDX, IDC_snSecondaryBlue, m_SecondaryBlue);
	DDX_Control(pDX, IDC_sparseSecondaryStatic, m_SecondaryStatic);

	DDX_Control(pDX, IDC_snTertiaryRed, m_TertiaryRed);
	DDX_Control(pDX, IDC_snTertiaryGreen, m_TertiaryGreen);
	DDX_Control(pDX, IDC_snTertiaryBlue, m_TertiaryBlue);
	DDX_Control(pDX, IDC_sparseTertiaryStatic, m_TertiaryStatic);

	DDX_Control(pDX, IDC_snBGRed, m_BGRed);
	DDX_Control(pDX, IDC_snBGGreen, m_BGGreen);
	DDX_Control(pDX, IDC_snBGBlue, m_BGBlue);
	DDX_Control(pDX, IDC_sparseBGStatic, m_BGStatic);

	DDX_Control(pDX, IDC_snCrossX, m_CrossX);
	DDX_Control(pDX, IDC_snCrossY, m_CrossY);
	DDX_Control(pDX, IDC_snCrossDiameter, m_CrossDiameter);
	DDX_Control(pDX, IDC_snCrossThickness, m_CrossThickness);

	DDX_Control(pDX, IDC_snBinocular, m_Binocular);
	DDX_Control(pDX, IDC_snShutters, m_Shutters);
	DDX_Control(pDX, IDC_snCrossX2, m_CrossX2);
	DDX_Control(pDX, IDC_snCrossY2, m_CrossY2);
	DDX_Control(pDX, IDC_snSwapInt, m_SwapInt);

	
	DDX_Control(pDX, IDC_snDisplayTimeMs, m_DisplayTimeMs);

	DDX_Control(pDX, IDC_snEyePosnDisplayLX, m_EyePosnDisplayLX);
	DDX_Control(pDX, IDC_snEyePosnDisplayLY, m_EyePosnDisplayLY);
	DDX_Control(pDX, IDC_snEyePosnDisplayRX, m_EyePosnDisplayRX);
	DDX_Control(pDX, IDC_snEyePosnDisplayRY, m_EyePosnDisplayRY);

	DDX_Control(pDX, IDC_snWindowAllowancePix, m_WindowAllowancePix);
	DDX_Control(pDX, IDC_snWait, m_Wait);

	DDX_Control(pDX, IDC_snFolderName, m_FolderName);
	DDX_Control(pDX, IDC_snNumFrames, m_NumFrames);
	DDX_Control(pDX, IDC_snStimWidth, m_StimWidth);
	DDX_Control(pDX, IDC_snMinDist, m_MinDist);
	DDX_Control(pDX, IDC_snPrimaryCount, m_PrimaryCount);
	DDX_Control(pDX, IDC_snSecondaryCount, m_SecondaryCount);
	DDX_Control(pDX, IDC_snTertiaryCount, m_TertiaryCount);
	DDX_Control(pDX, IDC_snSeed, m_Seed);

	DDX_Control(pDX, IDC_snUseGrid, m_UseGrid);
	DDX_Control(pDX, IDC_snStepSizeDeg, m_StepSizeDeg);
	DDX_Control(pDX, IDC_snNumStepsX, m_NumStepsX);
	DDX_Control(pDX, IDC_snNumStepsY, m_NumStepsY);

	DDX_Control(pDX, IDC_snDebug, m_debug);

	
}

BEGIN_MESSAGE_MAP(CSparseNoiseDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_snBinocular, OnBnClickedBinocular)
	ON_BN_CLICKED(IDC_snCreateStimFolder, OnBnClickedCreateStimFolder)
	ON_BN_CLICKED(IDC_snLoadStimFolder, OnBnClickedLoadStimFolder)
	ON_BN_CLICKED(IDC_snUseGrid, OnBnClickedUseGrid)
	ON_WM_PAINT()
	
END_MESSAGE_MAP()

BOOL CSparseNoiseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSparseNoiseDlg::load() 
{
	m_CrossX.SetWindowText(etu::FloatToString(g_pSparseNoise->getCrossX()));
	m_CrossY.SetWindowText(etu::FloatToString(g_pSparseNoise->getCrossY()));
	m_CrossDiameter.SetWindowText(etu::FloatToString(g_pSparseNoise->getCrossDiameter()));
	m_CrossThickness.SetWindowText(etu::FloatToString(g_pSparseNoise->getCrossThickness()));

	m_Binocular.SetCheck(g_pSparseNoise->getUseBinocular() ? BST_CHECKED : BST_UNCHECKED);
	m_Shutters.SetCheck(g_pSparseNoise->getUseShutters() ? BST_CHECKED : BST_UNCHECKED);
	m_CrossX2.SetWindowText(etu::FloatToString(g_pSparseNoise->getCrossX2()));
	m_CrossY2.SetWindowText(etu::FloatToString(g_pSparseNoise->getCrossY2()));
	m_SwapInt.SetWindowText(etu::FloatToString(g_pSparseNoise->getSwapInt()));

	m_DisplayTimeMs.SetWindowText(etu::IntToString(g_pSparseNoise->getDisplayTimeMs()));
	
	m_WindowAllowancePix.SetWindowTextA(etu::FloatToString(g_pSparseNoise->getWindowAllowancePix()));
	m_Wait.SetCheck(g_pSparseNoise->getUseWait()?BST_CHECKED:BST_UNCHECKED);

	if(g_pSparseNoise->getUseBinocular()){
		OnBnClickedBinocular();
	}
	m_PrimaryRed.SetWindowText(etu::IntToString(g_pSparseNoise->getPrimaryRed()));
	m_PrimaryGreen.SetWindowText(etu::IntToString(g_pSparseNoise->getPrimaryGreen()));
	m_PrimaryBlue.SetWindowText(etu::IntToString(g_pSparseNoise->getPrimaryBlue()));
	m_SecondaryRed.SetWindowText(etu::IntToString(g_pSparseNoise->getSecondaryRed()));
	m_SecondaryGreen.SetWindowText(etu::IntToString(g_pSparseNoise->getSecondaryGreen()));
	m_SecondaryBlue.SetWindowText(etu::IntToString(g_pSparseNoise->getSecondaryBlue()));
	m_TertiaryRed.SetWindowText(etu::IntToString(g_pSparseNoise->getTertiaryRed()));
	m_TertiaryGreen.SetWindowText(etu::IntToString(g_pSparseNoise->getTertiaryGreen()));
	m_TertiaryBlue.SetWindowText(etu::IntToString(g_pSparseNoise->getTertiaryBlue()));
	m_BGRed.SetWindowText(etu::IntToString(g_pSparseNoise->getBGRed()));
	m_BGGreen.SetWindowText(etu::IntToString(g_pSparseNoise->getBGGreen()));
	m_BGBlue.SetWindowText(etu::IntToString(g_pSparseNoise->getBGBlue()));

	m_StepSizeDeg.SetWindowText(etu::FloatToString(g_pSparseNoise->getStepSizeDeg()));
	m_NumStepsX.SetWindowText(etu::IntToString(g_pSparseNoise->getStepsX()));
	m_NumStepsY.SetWindowText(etu::IntToString(g_pSparseNoise->getStepsY()));

	if (g_pSparseNoise->getUseGrid()) {
		OnBnClickedUseGrid();
	}

	m_FolderName.SetWindowText("default");
	OnBnClickedLoadStimFolder();
	PaintColorStatics();


	
}

void CSparseNoiseDlg::save()
{
	char buffer[100];

	m_CrossX.GetWindowText(buffer, 100);
	g_pSparseNoise->setCrossX(etu::StringToFloat(buffer, g_pSparseNoise->getCrossX()));

	m_CrossY.GetWindowText(buffer, 100);
	g_pSparseNoise->setCrossY(etu::StringToFloat(buffer, g_pSparseNoise->getCrossY()));

	m_CrossDiameter.GetWindowText(buffer, 100);
	g_pSparseNoise->setCrossDiameter(etu::StringToFloat(buffer, g_pSparseNoise->getCrossDiameter()));

	m_CrossThickness.GetWindowText(buffer, 100);
	g_pSparseNoise->setCrossThickness(etu::StringToFloat(buffer, g_pSparseNoise->getCrossThickness()));

	g_pSparseNoise->setUseBinocular(m_Binocular.GetCheck() == BST_CHECKED);

	g_pSparseNoise->setUseShutters(m_Shutters.GetCheck() == BST_CHECKED);
	
	m_CrossX2.GetWindowText(buffer, 100);
	g_pSparseNoise->setCrossX2(etu::StringToFloat(buffer, g_pSparseNoise->getCrossX2()));

	m_CrossY2.GetWindowText(buffer, 100);
	g_pSparseNoise->setCrossY2(etu::StringToFloat(buffer, g_pSparseNoise->getCrossY2()));

	m_SwapInt.GetWindowText(buffer, 100);
	g_pSparseNoise->setSwapInt(etu::StringToFloat(buffer, g_pSparseNoise->getSwapInt()));


	m_DisplayTimeMs.GetWindowText(buffer, 100);
	g_pSparseNoise->setDisplayTimeMs(etu::StringToInt(buffer, g_pSparseNoise->getDisplayTimeMs()));

	m_WindowAllowancePix.GetWindowTextA(buffer,100);
	g_pSparseNoise->setWindowAllowancePix(etu::StringToFloat(buffer,g_pSparseNoise->getWindowAllowancePix()));

	g_pSparseNoise->setUseWait(m_Wait.GetCheck()==BST_CHECKED);

	m_FolderName.GetWindowText(buffer, 100);
	g_pSparseNoise->setFolderName(buffer);

	m_NumFrames.GetWindowText(buffer, 100);
	g_pSparseNoise->setNumFrames(etu::StringToFloat(buffer, g_pSparseNoise->getNumFrames()));
	m_StimWidth.GetWindowText(buffer, 100);
	g_pSparseNoise->setStimWidth(etu::StringToFloat(buffer, g_pSparseNoise->getStimWidth()));
	m_MinDist.GetWindowText(buffer, 100);
	g_pSparseNoise->setMinDist(etu::StringToFloat(buffer, g_pSparseNoise->getMinDist()));
	m_PrimaryCount.GetWindowText(buffer, 100);
	g_pSparseNoise->setPrimaryCount(etu::StringToFloat(buffer, g_pSparseNoise->getPrimaryCount()));
	m_SecondaryCount.GetWindowText(buffer, 100);
	g_pSparseNoise->setSecondaryCount(etu::StringToFloat(buffer, g_pSparseNoise->getSecondaryCount()));
	m_TertiaryCount.GetWindowText(buffer, 100);
	g_pSparseNoise->setTertiaryCount(etu::StringToFloat(buffer, g_pSparseNoise->getTertiaryCount()));
	m_Seed.GetWindowText(buffer, 100);
	g_pSparseNoise->setSeed(etu::StringToInt(buffer, g_pSparseNoise->getSeed()));

	m_PrimaryRed.GetWindowTextA(buffer, 100);
	g_pSparseNoise->setPrimaryRed(etu::StringToInt(buffer, g_pSparseNoise->getPrimaryRed()));
	m_PrimaryGreen.GetWindowTextA(buffer, 100);
	g_pSparseNoise->setPrimaryGreen(etu::StringToInt(buffer, g_pSparseNoise->getPrimaryGreen()));
	m_PrimaryBlue.GetWindowTextA(buffer, 100);
	g_pSparseNoise->setPrimaryBlue(etu::StringToInt(buffer, g_pSparseNoise->getPrimaryBlue()));

	m_SecondaryRed.GetWindowTextA(buffer, 100);
	g_pSparseNoise->setSecondaryRed(etu::StringToInt(buffer, g_pSparseNoise->getSecondaryRed()));
	m_SecondaryGreen.GetWindowTextA(buffer, 100);
	g_pSparseNoise->setSecondaryGreen(etu::StringToInt(buffer, g_pSparseNoise->getSecondaryGreen()));
	m_SecondaryBlue.GetWindowTextA(buffer, 100);
	g_pSparseNoise->setSecondaryBlue(etu::StringToInt(buffer, g_pSparseNoise->getSecondaryBlue()));

	m_TertiaryRed.GetWindowTextA(buffer, 100);
	g_pSparseNoise->setTertiaryRed(etu::StringToInt(buffer, g_pSparseNoise->getTertiaryRed()));
	m_TertiaryGreen.GetWindowTextA(buffer, 100);
	g_pSparseNoise->setTertiaryGreen(etu::StringToInt(buffer, g_pSparseNoise->getTertiaryGreen()));
	m_TertiaryBlue.GetWindowTextA(buffer, 100);
	g_pSparseNoise->setTertiaryBlue(etu::StringToInt(buffer, g_pSparseNoise->getTertiaryBlue()));

	m_BGRed.GetWindowTextA(buffer, 100);
	g_pSparseNoise->setBGRed(etu::StringToInt(buffer, g_pSparseNoise->getBGRed()));
	m_BGGreen.GetWindowTextA(buffer, 100);
	g_pSparseNoise->setBGGreen(etu::StringToInt(buffer, g_pSparseNoise->getBGGreen()));
	m_BGBlue.GetWindowTextA(buffer, 100);
	g_pSparseNoise->setBGBlue(etu::StringToInt(buffer, g_pSparseNoise->getBGBlue()));

	m_StepSizeDeg.GetWindowText(buffer, 100);
	g_pSparseNoise->setStepSizeDeg(etu::StringToFloat(buffer, g_pSparseNoise->getStepSizeDeg()));

	m_NumStepsX.GetWindowText(buffer, 100);
	g_pSparseNoise->setStepsX(etu::StringToInt(buffer, g_pSparseNoise->getStepsX()));

	m_NumStepsY.GetWindowText(buffer, 100);
	g_pSparseNoise->setStepsY(etu::StringToInt(buffer, g_pSparseNoise->getStepsY()));

	if (m_UseGrid.GetCheck() == BST_CHECKED) {
		g_pSparseNoise->setUseGrid(true);
	}else{
		g_pSparseNoise->setUseGrid(false);
	}

	PaintColorStatics();


}

void CSparseNoiseDlg::OnPaint() 
{
	CDialog::OnPaint();
	PaintColorStatics();
}
void CSparseNoiseDlg::PaintColorStatics()
{
	CRect rect;
	DWORD primaryColor, secondaryColor, tertiaryColor, bgColor;

	primaryColor = RGB(g_pSparseNoise->getPrimaryRed(), g_pSparseNoise->getPrimaryGreen(), g_pSparseNoise->getPrimaryBlue());
	secondaryColor = RGB(g_pSparseNoise->getSecondaryRed(), g_pSparseNoise->getSecondaryGreen(), g_pSparseNoise->getSecondaryBlue());
	tertiaryColor = RGB(g_pSparseNoise->getTertiaryRed(), g_pSparseNoise->getTertiaryGreen(), g_pSparseNoise->getTertiaryBlue());
	bgColor = RGB(g_pSparseNoise->getBGRed(), g_pSparseNoise->getBGGreen(), g_pSparseNoise->getBGBlue());

	CPaintDC dc_p(&m_PrimaryStatic);
	m_PrimaryStatic.GetClientRect(rect);
	dc_p.FillSolidRect(rect, primaryColor);

	CPaintDC dc_s(&m_SecondaryStatic);
	m_SecondaryStatic.GetClientRect(rect);
	dc_s.FillSolidRect(rect, secondaryColor);

	CPaintDC dc_t(&m_TertiaryStatic);
	m_TertiaryStatic.GetClientRect(rect);
	dc_t.FillSolidRect(rect, tertiaryColor);

	CPaintDC dc_bg(&m_BGStatic);
	m_BGStatic.GetClientRect(rect);
	dc_bg.FillSolidRect(rect, bgColor);
	printDebug("Called Color");

}

void CSparseNoiseDlg::OnBnClickedOk()
{
	OnBnClickedApply();
}

void CSparseNoiseDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CSparseNoiseDlg::OnBnClickedBinocular()
{
	
}


void CSparseNoiseDlg::OnBnClickedApply()
{
	if (g_pActiveStimulus  && g_pActiveStimulus->StimulusType() == "Sparse Noise Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
	this->Invalidate(false);
}
void CSparseNoiseDlg::OnBnClickedUseGrid()
{

	if (m_UseGrid.GetCheck() == BST_CHECKED) {
		m_StepSizeDeg.EnableWindow(true);
		m_NumStepsX.EnableWindow(true);
		m_NumStepsY.EnableWindow(true);
	}
	else {
		m_StepSizeDeg.EnableWindow(false);
		m_NumStepsX.EnableWindow(false);
		m_NumStepsY.EnableWindow(false);
	}
	
}
void CSparseNoiseDlg::OnBnClickedCreateStimFolder()
{	char buffer[100];
	m_FolderName.GetWindowText(buffer, 100);
	g_pSparseNoise->setFolderName(buffer);

	m_NumFrames.GetWindowText(buffer, 100);
	g_pSparseNoise->setNumFrames(etu::StringToInt(buffer, g_pSparseNoise->getNumFrames()));
	m_StimWidth.GetWindowText(buffer, 100);
	g_pSparseNoise->setStimWidth(etu::StringToFloat(buffer, g_pSparseNoise->getStimWidth()));
	m_MinDist.GetWindowText(buffer, 100);
	g_pSparseNoise->setMinDist(etu::StringToFloat(buffer, g_pSparseNoise->getMinDist()));
	m_PrimaryCount.GetWindowText(buffer, 100);
	g_pSparseNoise->setPrimaryCount(etu::StringToFloat(buffer, g_pSparseNoise->getPrimaryCount()));
	m_SecondaryCount.GetWindowText(buffer, 100);
	g_pSparseNoise->setSecondaryCount(etu::StringToFloat(buffer, g_pSparseNoise->getSecondaryCount()));
	m_TertiaryCount.GetWindowText(buffer, 100);
	g_pSparseNoise->setTertiaryCount(etu::StringToFloat(buffer, g_pSparseNoise->getTertiaryCount()));
	m_Seed.GetWindowText(buffer, 100);
	g_pSparseNoise->setSeed(etu::StringToInt(buffer, g_pSparseNoise->getSeed()));

	g_pSparseNoise->CreateStimFolder();
	if (g_pSparseNoise->getCreateFrames()){
		g_pEyeTrackingDlg->OnBnClickedRun();
		save();
		this->Invalidate(false);
	}
	
}


void CSparseNoiseDlg::OnBnClickedLoadStimFolder()
{	char buffer[100];
	m_FolderName.GetWindowText(buffer, 100);
	g_pSparseNoise->setFolderName(buffer);
	g_pSparseNoise->LoadStimFolder();
	m_NumFrames.SetWindowTextA(etu::FloatToString(g_pSparseNoise->getNumFrames()));
	m_StimWidth.SetWindowTextA(etu::FloatToString(g_pSparseNoise->getStimWidth()));
	m_MinDist.SetWindowTextA(etu::FloatToString(g_pSparseNoise->getMinDist()));
	m_PrimaryCount.SetWindowTextA(etu::FloatToString(g_pSparseNoise->getPrimaryCount()));
	m_SecondaryCount.SetWindowTextA(etu::FloatToString(g_pSparseNoise->getSecondaryCount()));
	m_TertiaryCount.SetWindowTextA(etu::FloatToString(g_pSparseNoise->getTertiaryCount()));
	m_Seed.SetWindowTextA(etu::IntToString(g_pSparseNoise->getSeed()));
	this->save();
}

void CSparseNoiseDlg::UpdateEyePositions(double lxDeg, double lyDeg, 
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

void CSparseNoiseDlg::printDebug(LPCTSTR text){
	m_debug.SetWindowText(text);
}
#endif
