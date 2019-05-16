#ifdef APP_ACUITY

// NoiseGratingsDlg.cpp : implementation file
//

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "NoiseGratingsDlg.h"


// CNoiseGratingsDlg dialog

IMPLEMENT_DYNAMIC(CNoiseGratingsDlg, CDialog)
CNoiseGratingsDlg::CNoiseGratingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoiseGratingsDlg::IDD, pParent)
{
}

CNoiseGratingsDlg::~CNoiseGratingsDlg()
{
}

void CNoiseGratingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MeanLevel, m_NoiseMean);
	DDX_Control(pDX, IDC_NoiseAmplitude, m_NoiseAmplitude);
	DDX_Control(pDX, IDC_NoiseType, m_NoiseType);
	DDX_Control(pDX, IDC_UseMask, m_UseMask);
	DDX_Control(pDX, IDC_MaskRadius, m_MaskRadius);
	DDX_Control(pDX, IDC_MaskBlending, m_MaskBlending);
	DDX_Control(pDX, IDC_UseMouseControl, m_UseMouseControl);
	DDX_Control(pDX, IDC_XPos, m_XPosition);
	DDX_Control(pDX, IDC_YPos, m_YPosition);
	DDX_Control(pDX, IDC_Check_AutoFreq, m_AutoFreq);
	DDX_Control(pDX, IDC_Check_AutoContrast, m_AutoContrast);
	DDX_Control(pDX, IDC_Check_AutoOrientation, m_AutoOrientation);
	DDX_Control(pDX, IDC_Check_AutoWidth, m_AutoWidth);
	DDX_Control(pDX, IDC_Check_AutoHeight, m_AutoHeight);
	DDX_Control(pDX, IDC_Check_AutoPhase, m_AutoPhase);
	DDX_Control(pDX, IDC_FreqMin, m_FreqMin);
	DDX_Control(pDX, IDC_FreqMax, m_FreqMax);
	DDX_Control(pDX, IDC_FreqStep, m_FreqStep);
	DDX_Control(pDX, IDC_FreqSetPt, m_FreqSetPt);
	DDX_Control(pDX, IDC_OrientationMin, m_OrientationMin);
	DDX_Control(pDX, IDC_OrientationMax, m_OrientationMax);
	DDX_Control(pDX, IDC_OrientationStep, m_OrientationStep);
	DDX_Control(pDX, IDC_OrientationSetPt, m_OrientationSetPt);
	DDX_Control(pDX, IDC_ContrastMin, m_ContrastMin);
	DDX_Control(pDX, IDC_ContrastMax, m_ContrastMax);
	DDX_Control(pDX, IDC_ContrastStep, m_ContrastStep);
	DDX_Control(pDX, IDC_ContrastSetPt, m_ContrastSetPt);
	DDX_Control(pDX, IDC_WidthMin, m_WidthMin);
	DDX_Control(pDX, IDC_WidthMax, m_WidthMax);
	DDX_Control(pDX, IDC_WidthStep, m_WidthStep);
	DDX_Control(pDX, IDC_WidthSetPt, m_WidthSetPt);
	DDX_Control(pDX, IDC_HeightMin, m_HeightMin);
	DDX_Control(pDX, IDC_HeightMax, m_HeightMax);
	DDX_Control(pDX, IDC_HeightStep, m_HeightStep);
	DDX_Control(pDX, IDC_HeightSetPt, m_HeightSetPt);
	DDX_Control(pDX, IDC_PhaseMin, m_PhaseMin);
	DDX_Control(pDX, IDC_PhaseMax, m_PhaseMax);
	DDX_Control(pDX, IDC_PhaseStep, m_PhaseStep);
	DDX_Control(pDX, IDC_PhaseSetPt, m_PhaseSetPt);


	DDX_Control(pDX, IDC_AutomationSpeed, m_AutomationSpeed);
	DDX_Control(pDX, IDC_Radio_GratingSinewave, m_Radio_SineWave);
	DDX_Control(pDX, IDC_Radio_GratingSquareWave, m_Radio_SquareWave);
}


BEGIN_MESSAGE_MAP(CNoiseGratingsDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDC_UseMask, OnBnClickedUsemask)
	ON_BN_CLICKED(IDC_UseMouseControl, OnBnClickedUsemousecontrol)
	ON_BN_CLICKED(IDC_Radio_GratingSinewave, OnBnClickedRadioGratingsinewave)
	ON_BN_CLICKED(IDC_Radio_GratingSquareWave, OnBnClickedRadioGratingsquarewave)
	ON_BN_CLICKED(IDC_Check_AutoFreq, OnBnClickedCheckAutofreq)
	ON_BN_CLICKED(IDC_Check_AutoContrast, OnBnClickedCheckAutocontrast)
	ON_BN_CLICKED(IDC_Check_AutoOrientation, OnBnClickedCheckAutoorientation)
	ON_BN_CLICKED(IDC_Check_AutoWidth, OnBnClickedCheckAutowidth)
	ON_BN_CLICKED(IDC_Check_AutoHeight, OnBnClickedCheckAutoheight)
	ON_BN_CLICKED(IDC_Check_AutoPhase, OnBnClickedCheckAutophase)
END_MESSAGE_MAP()


// CNoiseGratingsDlg message handlers
BOOL CNoiseGratingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_NoiseType.ResetContent();
	m_NoiseType.AddString("White");
	m_NoiseType.AddString("Gaussian");
	m_NoiseType.SetCurSel(0);
	m_MaskBlending.ResetContent();
	m_MaskBlending.AddString("Linear");
	m_MaskBlending.AddString("Quadratic");
	m_MaskBlending.AddString("Exponential");
	m_MaskBlending.SetCurSel(0);




	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CNoiseGratingsDlg::OnBnClickedOk()
{
	OnBnClickedApply();
}

void CNoiseGratingsDlg::OnBnClickedCancel()
{
	
}

void CNoiseGratingsDlg::OnBnClickedApply()
{
	save();
}

void CNoiseGratingsDlg::OnBnClickedUsemask()
{
	if (m_UseMask.GetCheck() == BST_CHECKED) {
		m_MaskRadius.EnableWindow(true);
		m_MaskBlending.EnableWindow(true);
	} else {
		m_MaskRadius.EnableWindow(false);
		m_MaskBlending.EnableWindow(false);
	}
	
}


void CNoiseGratingsDlg::load() {
	// Noise, Mask, Misc Parameters
	m_NoiseMean.SetWindowText(etu::IntToString(g_pNoiseGratings->getMeanLevel()));
	m_NoiseAmplitude.SetWindowText(etu::IntToString(g_pNoiseGratings->getAmplitude()));
	m_MaskRadius.SetWindowText(etu::FloatToString(g_pNoiseGratings->getMaskRadius()));
	m_XPosition.SetWindowText(etu::FloatToString(g_pNoiseGratings->getXDeg()));
	m_YPosition.SetWindowText(etu::FloatToString(g_pNoiseGratings->getYDeg()));
	m_AutomationSpeed.SetWindowText(etu::FloatToString(g_pNoiseGratings->getAutomationSpeed()));

	// FREQ
	m_FreqMin.SetWindowText(etu::FloatToString(g_pNoiseGratings->getMinFreq()));
	m_FreqMax.SetWindowText(etu::FloatToString(g_pNoiseGratings->getMaxFreq()));
	m_FreqStep.SetWindowText(etu::FloatToString(g_pNoiseGratings->getStepFreq()));
	m_FreqSetPt.SetWindowText(etu::FloatToString(g_pNoiseGratings->getSetPtFreq()));

	if (g_pNoiseGratings->getAutoFreq()) {
    	m_AutoFreq.SetCheck(BST_CHECKED);
		m_FreqMin.EnableWindow(true);
		m_FreqMax.EnableWindow(true);
		m_FreqStep.EnableWindow(true);
		m_FreqSetPt.EnableWindow(false);
	} else {
		m_AutoFreq.SetCheck(BST_UNCHECKED);
		m_FreqMin.EnableWindow(false);
		m_FreqMax.EnableWindow(false);
		m_FreqStep.EnableWindow(false);
		m_FreqSetPt.EnableWindow(true);
	}



	
	// Contrast
	m_ContrastMin.SetWindowText(etu::FloatToString(g_pNoiseGratings->getMinContrast()));
	m_ContrastMax.SetWindowText(etu::FloatToString(g_pNoiseGratings->getMaxContrast()));
	m_ContrastStep.SetWindowText(etu::FloatToString(g_pNoiseGratings->getStepContrast()));
	m_ContrastSetPt.SetWindowText(etu::FloatToString(g_pNoiseGratings->getSetPtContrast()));

	if (g_pNoiseGratings->getAutoContrast()) {
    	m_AutoContrast.SetCheck(BST_CHECKED);
		m_ContrastMin.EnableWindow(true);
		m_ContrastMax.EnableWindow(true);
		m_ContrastStep.EnableWindow(true);
		m_ContrastSetPt.EnableWindow(false);
	} else {
		m_AutoContrast.SetCheck(BST_UNCHECKED);
		m_ContrastMin.EnableWindow(false);
		m_ContrastMax.EnableWindow(false);
		m_ContrastStep.EnableWindow(false);
		m_ContrastSetPt.EnableWindow(true);
	}
	
	// Orientation
	m_OrientationMin.SetWindowText(etu::FloatToString(g_pNoiseGratings->getMinOrientation()));
	m_OrientationMax.SetWindowText(etu::FloatToString(g_pNoiseGratings->getMaxOrientation()));
	m_OrientationStep.SetWindowText(etu::FloatToString(g_pNoiseGratings->getStepOrientation()));
	m_OrientationSetPt.SetWindowText(etu::FloatToString(g_pNoiseGratings->getSetPtOrientation()));

	if (g_pNoiseGratings->getAutoOrientation()) {
    	m_AutoOrientation.SetCheck(BST_CHECKED);
		m_OrientationMin.EnableWindow(true);
		m_OrientationMax.EnableWindow(true);
		m_OrientationStep.EnableWindow(true);
		m_OrientationSetPt.EnableWindow(false);
	} else {
		m_AutoOrientation.SetCheck(BST_UNCHECKED);
		m_OrientationMin.EnableWindow(false);
		m_OrientationMax.EnableWindow(false);
		m_OrientationStep.EnableWindow(false);
		m_OrientationSetPt.EnableWindow(true);
	}
	
	// Width
	m_WidthMin.SetWindowText(etu::FloatToString(g_pNoiseGratings->getMinWidth()));
	m_WidthMax.SetWindowText(etu::FloatToString(g_pNoiseGratings->getMaxWidth()));
	m_WidthStep.SetWindowText(etu::FloatToString(g_pNoiseGratings->getStepWidth()));
	m_WidthSetPt.SetWindowText(etu::FloatToString(g_pNoiseGratings->getSetPtWidth()));

	if (g_pNoiseGratings->getAutoWidth()) {
    	m_AutoWidth.SetCheck(BST_CHECKED);
		m_WidthMin.EnableWindow(true);
		m_WidthMax.EnableWindow(true);
		m_WidthStep.EnableWindow(true);
		m_WidthSetPt.EnableWindow(false);
	} else {
		m_AutoWidth.SetCheck(BST_UNCHECKED);
		m_WidthMin.EnableWindow(false);
		m_WidthMax.EnableWindow(false);
		m_WidthStep.EnableWindow(false);
		m_WidthSetPt.EnableWindow(true);
	}
	
	// Height
	m_HeightMin.SetWindowText(etu::FloatToString(g_pNoiseGratings->getMinHeight()));
	m_HeightMax.SetWindowText(etu::FloatToString(g_pNoiseGratings->getMaxHeight()));
	m_HeightStep.SetWindowText(etu::FloatToString(g_pNoiseGratings->getStepHeight()));
	m_HeightSetPt.SetWindowText(etu::FloatToString(g_pNoiseGratings->getSetPtHeight()));

	if (g_pNoiseGratings->getAutoHeight()) {
    	m_AutoHeight.SetCheck(BST_CHECKED);
		m_HeightMin.EnableWindow(true);
		m_HeightMax.EnableWindow(true);
		m_HeightStep.EnableWindow(true);
		m_HeightSetPt.EnableWindow(false);
	} else {
		m_AutoHeight.SetCheck(BST_UNCHECKED);
		m_HeightMin.EnableWindow(false);
		m_HeightMax.EnableWindow(false);
		m_HeightStep.EnableWindow(false);
		m_HeightSetPt.EnableWindow(true);
	}
	
	// Phase
	m_PhaseMin.SetWindowText(etu::FloatToString(g_pNoiseGratings->getMinPhase()));
	m_PhaseMax.SetWindowText(etu::FloatToString(g_pNoiseGratings->getMaxPhase()));
	m_PhaseStep.SetWindowText(etu::FloatToString(g_pNoiseGratings->getStepPhase()));
	m_PhaseSetPt.SetWindowText(etu::FloatToString(g_pNoiseGratings->getSetPtPhase()));

	if (g_pNoiseGratings->getAutoPhase()) {
    	m_AutoPhase.SetCheck(BST_CHECKED);
		m_PhaseMin.EnableWindow(true);
		m_PhaseMax.EnableWindow(true);
		m_PhaseStep.EnableWindow(true);
		m_PhaseSetPt.EnableWindow(false);
	} else {
		m_AutoPhase.SetCheck(BST_UNCHECKED);
		m_PhaseMin.EnableWindow(false);
		m_PhaseMax.EnableWindow(false);
		m_PhaseStep.EnableWindow(false);
		m_PhaseSetPt.EnableWindow(true);
	}
	

	// USE MASK?
	if (g_pNoiseGratings->useMask()) {
		m_UseMask.SetCheck(BST_CHECKED);
		m_MaskRadius.EnableWindow(true);
		m_MaskBlending.EnableWindow(true);
	} else {
		m_UseMask.SetCheck(BST_UNCHECKED);
		m_MaskRadius.EnableWindow(false);
		m_MaskBlending.EnableWindow(false);
	}

	// USE MOUSE CONTROL?
	if (g_pNoiseGratings->useMouseControl()) {
		m_UseMouseControl.SetCheck(BST_CHECKED);
		m_XPosition.EnableWindow(false);
		m_YPosition.EnableWindow(false);
	} else {
		m_UseMouseControl.SetCheck(BST_UNCHECKED);
		m_XPosition.EnableWindow(true);
		m_YPosition.EnableWindow(true);
	}

	// GRATING TYPE
	if (g_pNoiseGratings->getGratingType() == CNoiseGratings::GRATING_TYPE_SINE) {
		m_Radio_SineWave.SetCheck(BST_CHECKED);
		m_Radio_SquareWave.SetCheck(BST_UNCHECKED);
	} else {
		m_Radio_SineWave.SetCheck(BST_UNCHECKED);
		m_Radio_SquareWave.SetCheck(BST_CHECKED);
	}

	// Noise Type, Blending type
	m_NoiseType.SetCurSel(g_pNoiseGratings->getNoiseType());
	m_MaskBlending.SetCurSel(g_pNoiseGratings->getBlendingType());


}

void CNoiseGratingsDlg::save() {
	char buffer[100];

	m_NoiseMean.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMeanLevel(etu::StringToInt(buffer, g_pNoiseGratings->getMeanLevel()));

	m_NoiseAmplitude.GetWindowText(buffer, 100);
	g_pNoiseGratings->setAmplitude(etu::StringToInt(buffer, g_pNoiseGratings->getAmplitude()));

	m_MaskRadius.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMaskRadius(etu::StringToFloat(buffer, g_pNoiseGratings->getMaskRadius()));

	m_XPosition.GetWindowText(buffer, 100);
	g_pNoiseGratings->setXDeg(etu::StringToFloat(buffer, g_pNoiseGratings->getXDeg()));

	m_YPosition.GetWindowText(buffer, 100);
	g_pNoiseGratings->setYDeg(etu::StringToFloat(buffer, g_pNoiseGratings->getYDeg()));

	m_AutomationSpeed.GetWindowText(buffer, 100);
	g_pNoiseGratings->setAutomationSpeed(etu::StringToFloat(buffer, g_pNoiseGratings->getAutomationSpeed()));

	// freq
	g_pNoiseGratings->setAutoFreq(m_AutoFreq.GetCheck() == BST_CHECKED);
	
	m_FreqMin.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMinFreq(etu::StringToFloat(buffer, g_pNoiseGratings->getMinFreq()));

	m_FreqMax.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMaxFreq(etu::StringToFloat(buffer, g_pNoiseGratings->getMaxFreq()));

	m_FreqStep.GetWindowText(buffer, 100);
	g_pNoiseGratings->setStepFreq(etu::StringToFloat(buffer, g_pNoiseGratings->getStepFreq()));

	m_FreqSetPt.GetWindowText(buffer, 100);
	g_pNoiseGratings->setSetPtFreq(etu::StringToFloat(buffer, g_pNoiseGratings->getSetPtFreq()));


	// Contrast
	g_pNoiseGratings->setAutoContrast(m_AutoContrast.GetCheck() == BST_CHECKED);
	
	m_ContrastMin.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMinContrast(etu::StringToFloat(buffer, g_pNoiseGratings->getMinContrast()));

	m_ContrastMax.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMaxContrast(etu::StringToFloat(buffer, g_pNoiseGratings->getMaxContrast()));

	m_ContrastStep.GetWindowText(buffer, 100);
	g_pNoiseGratings->setStepContrast(etu::StringToFloat(buffer, g_pNoiseGratings->getStepContrast()));

	m_ContrastSetPt.GetWindowText(buffer, 100);
	g_pNoiseGratings->setSetPtContrast(etu::StringToFloat(buffer, g_pNoiseGratings->getSetPtContrast()));

	// Orientation
	g_pNoiseGratings->setAutoOrientation(m_AutoOrientation.GetCheck() == BST_CHECKED);
	
	m_OrientationMin.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMinOrientation(etu::StringToFloat(buffer, g_pNoiseGratings->getMinOrientation()));

	m_OrientationMax.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMaxOrientation(etu::StringToFloat(buffer, g_pNoiseGratings->getMaxOrientation()));

	m_OrientationStep.GetWindowText(buffer, 100);
	g_pNoiseGratings->setStepOrientation(etu::StringToFloat(buffer, g_pNoiseGratings->getStepOrientation()));

	m_OrientationSetPt.GetWindowText(buffer, 100);
	g_pNoiseGratings->setSetPtOrientation(etu::StringToFloat(buffer, g_pNoiseGratings->getSetPtOrientation()));

	// Width
	g_pNoiseGratings->setAutoWidth(m_AutoWidth.GetCheck() == BST_CHECKED);
	
	m_WidthMin.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMinWidth(etu::StringToFloat(buffer, g_pNoiseGratings->getMinWidth()));

	m_WidthMax.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMaxWidth(etu::StringToFloat(buffer, g_pNoiseGratings->getMaxWidth()));

	m_WidthStep.GetWindowText(buffer, 100);
	g_pNoiseGratings->setStepWidth(etu::StringToFloat(buffer, g_pNoiseGratings->getStepWidth()));

	m_WidthSetPt.GetWindowText(buffer, 100);
	g_pNoiseGratings->setSetPtWidth(etu::StringToFloat(buffer, g_pNoiseGratings->getSetPtWidth()));

	// Height
	g_pNoiseGratings->setAutoHeight(m_AutoHeight.GetCheck() == BST_CHECKED);
	
	m_HeightMin.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMinHeight(etu::StringToFloat(buffer, g_pNoiseGratings->getMinHeight()));

	m_HeightMax.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMaxHeight(etu::StringToFloat(buffer, g_pNoiseGratings->getMaxHeight()));

	m_HeightStep.GetWindowText(buffer, 100);
	g_pNoiseGratings->setStepHeight(etu::StringToFloat(buffer, g_pNoiseGratings->getStepHeight()));

	m_HeightSetPt.GetWindowText(buffer, 100);
	g_pNoiseGratings->setSetPtHeight(etu::StringToFloat(buffer, g_pNoiseGratings->getSetPtHeight()));

	// Phase
	g_pNoiseGratings->setAutoPhase(m_AutoPhase.GetCheck() == BST_CHECKED);
	
	m_PhaseMin.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMinPhase(etu::StringToFloat(buffer, g_pNoiseGratings->getMinPhase()));

	m_PhaseMax.GetWindowText(buffer, 100);
	g_pNoiseGratings->setMaxPhase(etu::StringToFloat(buffer, g_pNoiseGratings->getMaxPhase()));

	m_PhaseStep.GetWindowText(buffer, 100);
	g_pNoiseGratings->setStepPhase(etu::StringToFloat(buffer, g_pNoiseGratings->getStepPhase()));

	m_PhaseSetPt.GetWindowText(buffer, 100);
	g_pNoiseGratings->setSetPtPhase(etu::StringToFloat(buffer, g_pNoiseGratings->getSetPtPhase()));


	g_pNoiseGratings->setNoiseType(m_NoiseType.GetCurSel());
	g_pNoiseGratings->setBlendingType(m_MaskBlending.GetCurSel());
	g_pNoiseGratings->setUseMask(m_UseMask.GetCheck() == BST_CHECKED);
	g_pNoiseGratings->setUseMouseControl(m_UseMouseControl.GetCheck() == BST_CHECKED);
	if (m_Radio_SineWave.GetCheck() == BST_CHECKED) {
		g_pNoiseGratings->setGratingType(CNoiseGratings::GRATING_TYPE_SINE);
	} else {
		g_pNoiseGratings->setGratingType(CNoiseGratings::GRATING_TYPE_SQUARE);
	}

}


void CNoiseGratingsDlg::OnBnClickedUsemousecontrol()
{
	if (m_UseMouseControl.GetCheck() == BST_CHECKED) {
		m_XPosition.EnableWindow(false);
		m_YPosition.EnableWindow(false);
	} else {
		m_XPosition.EnableWindow(false);
		m_YPosition.EnableWindow(false);
	}
}

void CNoiseGratingsDlg::OnBnClickedRadioGratingsinewave()
{
	m_Radio_SineWave.SetCheck(BST_CHECKED);
	m_Radio_SquareWave.SetCheck(BST_UNCHECKED);
}

void CNoiseGratingsDlg::OnBnClickedRadioGratingsquarewave()
{
	m_Radio_SineWave.SetCheck(BST_UNCHECKED);
	m_Radio_SquareWave.SetCheck(BST_CHECKED);
}

void CNoiseGratingsDlg::OnBnClickedCheckAutofreq()
{
	if (m_AutoFreq.GetCheck() == BST_CHECKED) {
		m_FreqMin.EnableWindow(true);
		m_FreqMax.EnableWindow(true);
		m_FreqStep.EnableWindow(true);
		m_FreqSetPt.EnableWindow(false);
	} else {
		m_FreqMin.EnableWindow(false);
		m_FreqMax.EnableWindow(false);
		m_FreqStep.EnableWindow(false);
		m_FreqSetPt.EnableWindow(true);
	}
}

void CNoiseGratingsDlg::OnBnClickedCheckAutocontrast()
{
	if (m_AutoContrast.GetCheck() == BST_CHECKED) {
		m_ContrastMin.EnableWindow(true);
		m_ContrastMax.EnableWindow(true);
		m_ContrastStep.EnableWindow(true);
		m_ContrastSetPt.EnableWindow(false);
	} else {
		m_ContrastMin.EnableWindow(false);
		m_ContrastMax.EnableWindow(false);
		m_ContrastStep.EnableWindow(false);
		m_ContrastSetPt.EnableWindow(true);
	}
}

void CNoiseGratingsDlg::OnBnClickedCheckAutoorientation()
{
	if (m_AutoOrientation.GetCheck() == BST_CHECKED) {
		m_OrientationMin.EnableWindow(true);
		m_OrientationMax.EnableWindow(true);
		m_OrientationStep.EnableWindow(true);
		m_OrientationSetPt.EnableWindow(false);
	} else {
		m_OrientationMin.EnableWindow(false);
		m_OrientationMax.EnableWindow(false);
		m_OrientationStep.EnableWindow(false);
		m_OrientationSetPt.EnableWindow(true);
	}
}

void CNoiseGratingsDlg::OnBnClickedCheckAutowidth()
{
	if (m_AutoWidth.GetCheck() == BST_CHECKED) {
		m_WidthMin.EnableWindow(true);
		m_WidthMax.EnableWindow(true);
		m_WidthStep.EnableWindow(true);
		m_WidthSetPt.EnableWindow(false);
	} else {
		m_WidthMin.EnableWindow(false);
		m_WidthMax.EnableWindow(false);
		m_WidthStep.EnableWindow(false);
		m_WidthSetPt.EnableWindow(true);
	}
}

void CNoiseGratingsDlg::OnBnClickedCheckAutoheight()
{
	if (m_AutoHeight.GetCheck() == BST_CHECKED) {
		m_HeightMin.EnableWindow(true);
		m_HeightMax.EnableWindow(true);
		m_HeightStep.EnableWindow(true);
		m_HeightSetPt.EnableWindow(false);
	} else {
		m_HeightMin.EnableWindow(false);
		m_HeightMax.EnableWindow(false);
		m_HeightStep.EnableWindow(false);
		m_HeightSetPt.EnableWindow(true);
	}
}

void CNoiseGratingsDlg::OnBnClickedCheckAutophase()
{
	if (m_AutoPhase.GetCheck() == BST_CHECKED) {
		m_PhaseMin.EnableWindow(true);
		m_PhaseMax.EnableWindow(true);
		m_PhaseStep.EnableWindow(true);
		m_PhaseSetPt.EnableWindow(false);
	} else {
		m_PhaseMin.EnableWindow(false);
		m_PhaseMax.EnableWindow(false);
		m_PhaseStep.EnableWindow(false);
		m_PhaseSetPt.EnableWindow(true);
	}
}


#endif