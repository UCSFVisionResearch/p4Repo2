/**
 * @file CircleAnnulusDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "CircleAnnulusDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CCircleAnnulusDlg, CDialog)
CCircleAnnulusDlg::CCircleAnnulusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCircleAnnulusDlg::IDD, pParent)
{}

CCircleAnnulusDlg::~CCircleAnnulusDlg()
{}

void CCircleAnnulusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RightEyeField, m_RightEyeField);
	DDX_Control(pDX, IDC_LeftEyeField, m_LeftEyeField);
	DDX_Control(pDX, IDC_InterleavedFields, m_InterleavedFields);
	DDX_Control(pDX, IDC_ManualFields, m_ManualFields);
	DDX_Control(pDX, IDC_XPosition, m_XPosition);
	DDX_Control(pDX, IDC_YPosition, m_YPosition);
	DDX_Control(pDX, IDC_TrialDuration, m_TrialDuration);
	DDX_Control(pDX, IDC_BlankPeriod, m_BlankPeriod);
	DDX_Control(pDX, IDC_IntertrialDuration, m_IntertrialDuration);
	DDX_Control(pDX, IDC_CirclesOnly, m_CirclesOnly);
	DDX_Control(pDX, IDC_AnnuliOnly, m_AnnuliOnly);
	DDX_Control(pDX, IDC_CirclesAnnuli, m_CirclesAnnuli);
	DDX_Control(pDX, IDC_CheckOnOff, m_CheckOnOff);
	DDX_Control(pDX, IDC_RadiusMin, m_RadiusMin);
	DDX_Control(pDX, IDC_RadiusMax, m_RadiusMax);
	DDX_Control(pDX, IDC_RadiusStep, m_RadiusStep);
	DDX_Control(pDX, IDC_AnnulusWidthMin, m_AnnulusWidthMin);
	DDX_Control(pDX, IDC_AnnulusWidthMax, m_AnnulusWidthMax);
	DDX_Control(pDX, IDC_AnnulusWidthStep, m_AnnulusWidthStep);
	DDX_Control(pDX, IDC_PrimaryRed, m_PrimaryRed);
	DDX_Control(pDX, IDC_PrimaryGreen, m_PrimaryGreen);
	DDX_Control(pDX, IDC_PrimaryBlue, m_PrimaryBlue);
	DDX_Control(pDX, IDC_PrimaryList, m_PrimaryList);
	DDX_Control(pDX, IDC_SecondaryRed, m_SecondaryRed);
	DDX_Control(pDX, IDC_SecondaryGreen, m_SecondaryGreen);
	DDX_Control(pDX, IDC_SecondaryBlue, m_SecondaryBlue);
	DDX_Control(pDX, IDC_SecondaryList, m_SecondaryList);
	DDX_Control(pDX, IDC_Repeats, m_Repeats);
	DDX_Control(pDX, IDC_UnitTag, m_UnitTag);
	DDX_Control(pDX, IDC_StaticRadius, m_StaticRadius);
	DDX_Control(pDX, IDC_StaticAnnulusWidth, m_StaticAnnulusWidth);
	DDX_Control(pDX, IDC_StaticRepeat, m_StaticRepeat);
	DDX_Control(pDX, IDC_StaticTrialsLeft, m_StaticTrialsLeft);
	DDX_Control(pDX, IDC_START_PRESENTATION, m_StartPresentations);
	DDX_Control(pDX, IDC_SecondaryAdd, m_SecondaryAdd);
	DDX_Control(pDX, IDC_SecondaryRemove, m_SecondaryRemove);
	DDX_Control(pDX, IDC_SecondaryClear, m_SecondaryClear);
	DDX_Control(pDX, IDC_PrimaryAdd, m_PrimaryAdd);
	DDX_Control(pDX, IDC_PrimaryRemove, m_PrimaryRemove);
	DDX_Control(pDX, IDC_PrimaryClear, m_PrimaryClear);
	DDX_Control(pDX, IDC_StaticLeftEye, m_StaticLeftEyeField);
	DDX_Control(pDX, IDC_StaticRightEye, m_StaticRightEyeField);
}

BEGIN_MESSAGE_MAP(CCircleAnnulusDlg, CDialog)
	ON_BN_CLICKED(IDC_RightEyeField, OnBnClickedRighteyefield)
	ON_BN_CLICKED(IDC_LeftEyeField, OnBnClickedLefteyefield)
	ON_BN_CLICKED(IDC_InterleavedFields, OnBnClickedInterleavedfields)
	ON_BN_CLICKED(IDC_ManualFields, OnBnClickedManualfields)
	ON_BN_CLICKED(IDC_CirclesOnly, OnBnClickedCirclesonly)
	ON_BN_CLICKED(IDC_AnnuliOnly, OnBnClickedAnnulionly)
	ON_BN_CLICKED(IDC_CirclesAnnuli, OnBnClickedCirclesannuli)
	ON_BN_CLICKED(IDC_CheckOnOff, OnBnClickedCheckonoff)
	ON_BN_CLICKED(IDC_PrimaryAdd, OnBnClickedPrimaryadd)
	ON_BN_CLICKED(IDC_PrimaryRemove, OnBnClickedPrimaryremove)
	ON_BN_CLICKED(IDC_PrimaryClear, OnBnClickedPrimaryclear)
	ON_BN_CLICKED(IDC_SecondaryAdd, OnBnClickedSecondaryadd)
	ON_BN_CLICKED(IDC_SecondaryRemove, OnBnClickedSecondaryremove)
	ON_BN_CLICKED(IDC_SecondaryClear, OnBnClickedSecondaryclear)
	ON_BN_CLICKED(ID_APPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDC_START_PRESENTATION, OnBnClickedStartPresentation)
	ON_WM_SHOWWINDOW()
	ON_STN_CLICKED(IDC_StaticLeftEye, OnStnClickedStaticlefteye)
END_MESSAGE_MAP()

BOOL CCircleAnnulusDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	m_StartPresentations.EnableWindow(false);
	OnBnClickedApply();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCircleAnnulusDlg::load() 
{
	m_XPosition.SetWindowText(etu::FloatToString(g_pCircleAnnulus->getXPosition()));
	m_YPosition.SetWindowText(etu::FloatToString(g_pCircleAnnulus->getYPosition()));
	m_TrialDuration.SetWindowText(etu::IntToString(g_pCircleAnnulus->getTrialDuration()));
	m_BlankPeriod.SetWindowText(etu::IntToString(g_pCircleAnnulus->getBlankPeriod()));
	m_IntertrialDuration.SetWindowText(etu::IntToString(g_pCircleAnnulus->getIntertrialDuration()));
	m_Repeats.SetWindowText(etu::IntToString(g_pCircleAnnulus->getRepeats()));
	m_UnitTag.SetWindowText(g_pCircleAnnulus->getUnitTag());

	m_RadiusMin.SetWindowText(etu::FloatToString(g_pCircleAnnulus->getCircleRadiusMin()));
	m_RadiusMax.SetWindowText(etu::FloatToString(g_pCircleAnnulus->getCircleRadiusMax()));
	m_RadiusStep.SetWindowText(etu::FloatToString(g_pCircleAnnulus->getCircleRadiusStep()));
	m_AnnulusWidthMin.SetWindowText(etu::FloatToString(g_pCircleAnnulus->getAnnulusWidthMin()));
	m_AnnulusWidthMax.SetWindowText(etu::FloatToString(g_pCircleAnnulus->getAnnulusWidthMax()));
	m_AnnulusWidthStep.SetWindowText(etu::FloatToString(g_pCircleAnnulus->getAnnulusWidthStep()));

	m_PrimaryRed.SetWindowText("0.00000");
	m_PrimaryGreen.SetWindowText("0.00000");
	m_PrimaryBlue.SetWindowText("0.00000");
	m_SecondaryRed.SetWindowText("0.00000");
	m_SecondaryGreen.SetWindowText("0.00000");
	m_SecondaryBlue.SetWindowText("0.00000");

	if (g_pCircleAnnulus->getField() == CCircleAnnulus::FIELD_RIGHT_EYE) {
		OnBnClickedRighteyefield();
	} 
	else if (g_pCircleAnnulus->getField() == CCircleAnnulus::FIELD_LEFT_EYE) {
		OnBnClickedLefteyefield();
	} 
	else if (g_pCircleAnnulus->getField() == CCircleAnnulus::FIELD_INTERLEAVED) {
		OnBnClickedInterleavedfields();
	} 
	else  {
		OnBnClickedManualfields();
	}

	if (g_pCircleAnnulus->getShape() == CCircleAnnulus::SHAPES_CIRCLES) {
		OnBnClickedCirclesonly();
	} 
	else if (g_pCircleAnnulus->getShape() == CCircleAnnulus::SHAPES_ANNULI) {
		OnBnClickedAnnulionly();
	} 
	else {
		OnBnClickedCirclesannuli();
	}

	if (g_pCircleAnnulus->getOnOff()) {
		m_CheckOnOff.SetCheck(BST_CHECKED);
	} 
	else {
		m_CheckOnOff.SetCheck(BST_UNCHECKED);

	}
	OnBnClickedCheckonoff();

	refreshPrimaryList();
	refreshSecondaryList();
}


void CCircleAnnulusDlg::save() 
{
	char buffer[100];

	m_XPosition.GetWindowText(buffer, 100);
	g_pCircleAnnulus->setXPosition(etu::StringToFloat(buffer, g_pCircleAnnulus->getXPosition()));

	m_YPosition.GetWindowText(buffer, 100);
	g_pCircleAnnulus->setYPosition(etu::StringToFloat(buffer, g_pCircleAnnulus->getYPosition()));

	m_TrialDuration.GetWindowText(buffer, 100);
	g_pCircleAnnulus->setTrialDuration(etu::StringToInt(buffer, g_pCircleAnnulus->getTrialDuration()));

	m_BlankPeriod.GetWindowText(buffer, 100);
	g_pCircleAnnulus->setBlankPeriod(etu::StringToInt(buffer, g_pCircleAnnulus->getBlankPeriod()));

	m_IntertrialDuration.GetWindowText(buffer, 100);
	g_pCircleAnnulus->setIntertrialDuration(etu::StringToInt(buffer, g_pCircleAnnulus->getIntertrialDuration()));

	m_RadiusMin.GetWindowText(buffer, 100);
	g_pCircleAnnulus->setCircleRadiusMin(etu::StringToFloat(buffer, g_pCircleAnnulus->getCircleRadiusMin()));

	m_RadiusMax.GetWindowText(buffer, 100);
	g_pCircleAnnulus->setCircleRadiusMax(etu::StringToFloat(buffer, g_pCircleAnnulus->getCircleRadiusMax()));

	m_RadiusStep.GetWindowText(buffer, 100);
	g_pCircleAnnulus->setCircleRadiusStep(etu::StringToFloat(buffer, g_pCircleAnnulus->getCircleRadiusStep()));

	m_AnnulusWidthMin.GetWindowText(buffer, 100);
	g_pCircleAnnulus->setAnnulusWidthMin(etu::StringToFloat(buffer, g_pCircleAnnulus->getAnnulusWidthMin()));

	m_AnnulusWidthMax.GetWindowText(buffer, 100);
	g_pCircleAnnulus->setAnnulusWidthMax(etu::StringToFloat(buffer, g_pCircleAnnulus->getAnnulusWidthMax()));

	m_AnnulusWidthStep.GetWindowText(buffer, 100);
	g_pCircleAnnulus->setAnnulusWidthStep(etu::StringToFloat(buffer, g_pCircleAnnulus->getAnnulusWidthStep()));

	m_Repeats.GetWindowText(buffer, 100);
	g_pCircleAnnulus->setRepeats(etu::StringToInt(buffer, g_pCircleAnnulus->getRepeats()));

	m_UnitTag.GetWindowText(buffer, 100);
	g_pCircleAnnulus->setUnitTag(CString(buffer));
	
	if (m_RightEyeField.GetCheck() == BST_CHECKED) {
		g_pCircleAnnulus->setField(CCircleAnnulus::FIELD_RIGHT_EYE);
	} 
	else if (m_LeftEyeField.GetCheck() == BST_CHECKED) {
		g_pCircleAnnulus->setField(CCircleAnnulus::FIELD_LEFT_EYE);
	} 
	else if (m_InterleavedFields.GetCheck() == BST_CHECKED) {
		g_pCircleAnnulus->setField(CCircleAnnulus::FIELD_INTERLEAVED);
	} 
	else {
		g_pCircleAnnulus->setField(CCircleAnnulus::FIELD_MANUAL);
	}

	if (m_CirclesOnly.GetCheck() == BST_CHECKED) {
		g_pCircleAnnulus->setShape(CCircleAnnulus::SHAPES_CIRCLES);
	} 
	else if (m_AnnuliOnly.GetCheck() == BST_CHECKED) {
		g_pCircleAnnulus->setShape(CCircleAnnulus::SHAPES_ANNULI);
    } 
	else {
		g_pCircleAnnulus->setShape(CCircleAnnulus::SHAPES_CIRCLE_ANNULI);
	}
	
	g_pCircleAnnulus->setOnOff(m_CheckOnOff.GetCheck() == BST_CHECKED);
}


void CCircleAnnulusDlg::OnBnClickedRighteyefield()
{
	m_RightEyeField.SetCheck(BST_CHECKED);
	m_LeftEyeField.SetCheck(BST_UNCHECKED);
	m_InterleavedFields.SetCheck(BST_UNCHECKED);
	m_ManualFields.SetCheck(BST_UNCHECKED);

	m_XPosition.EnableWindow(false);
	m_YPosition.EnableWindow(false);
}

void CCircleAnnulusDlg::OnBnClickedLefteyefield()
{
	m_RightEyeField.SetCheck(BST_UNCHECKED);
	m_LeftEyeField.SetCheck(BST_CHECKED);
	m_InterleavedFields.SetCheck(BST_UNCHECKED);
	m_ManualFields.SetCheck(BST_UNCHECKED);

	m_XPosition.EnableWindow(false);
	m_YPosition.EnableWindow(false);
}

void CCircleAnnulusDlg::OnBnClickedInterleavedfields()
{
	m_RightEyeField.SetCheck(BST_UNCHECKED);
	m_LeftEyeField.SetCheck(BST_UNCHECKED);
	m_InterleavedFields.SetCheck(BST_CHECKED);
	m_ManualFields.SetCheck(BST_UNCHECKED);

	m_XPosition.EnableWindow(false);
	m_YPosition.EnableWindow(false);
}

void CCircleAnnulusDlg::OnBnClickedManualfields()
{
	m_RightEyeField.SetCheck(BST_UNCHECKED);
	m_LeftEyeField.SetCheck(BST_UNCHECKED);
	m_InterleavedFields.SetCheck(BST_UNCHECKED);
	m_ManualFields.SetCheck(BST_CHECKED);

	m_XPosition.EnableWindow(true);
	m_YPosition.EnableWindow(true);
}

void CCircleAnnulusDlg::OnBnClickedCirclesonly()
{
	m_CirclesOnly.SetCheck(BST_CHECKED);
	m_AnnuliOnly.SetCheck(BST_UNCHECKED);
	m_CirclesAnnuli.SetCheck(BST_UNCHECKED);

	m_RadiusMin.EnableWindow(true);
	m_RadiusMax.EnableWindow(m_CheckOnOff.GetCheck() == BST_UNCHECKED);
	m_RadiusStep.EnableWindow(m_CheckOnOff.GetCheck() == BST_UNCHECKED);

	m_AnnulusWidthMin.EnableWindow(false);
	m_AnnulusWidthMax.EnableWindow(false);
	m_AnnulusWidthStep.EnableWindow(false);
}

void CCircleAnnulusDlg::OnBnClickedAnnulionly()
{
	m_CirclesOnly.SetCheck(BST_UNCHECKED);
	m_AnnuliOnly.SetCheck(BST_CHECKED);
	m_CirclesAnnuli.SetCheck(BST_UNCHECKED);

	m_RadiusMin.EnableWindow(true);
	m_RadiusMax.EnableWindow(m_CheckOnOff.GetCheck() == BST_UNCHECKED);
	m_RadiusStep.EnableWindow(m_CheckOnOff.GetCheck() == BST_UNCHECKED);

	m_AnnulusWidthMin.EnableWindow(true);
	m_AnnulusWidthMax.EnableWindow(m_CheckOnOff.GetCheck() == BST_UNCHECKED);
	m_AnnulusWidthStep.EnableWindow(m_CheckOnOff.GetCheck() == BST_UNCHECKED);
}

void CCircleAnnulusDlg::OnBnClickedCirclesannuli()
{
	m_CirclesOnly.SetCheck(BST_UNCHECKED);
	m_AnnuliOnly.SetCheck(BST_UNCHECKED);
	m_CirclesAnnuli.SetCheck(BST_CHECKED);

	m_RadiusMin.EnableWindow(true);
	m_RadiusMax.EnableWindow(m_CheckOnOff.GetCheck() == BST_UNCHECKED);
	m_RadiusStep.EnableWindow(m_CheckOnOff.GetCheck() == BST_UNCHECKED);

	m_AnnulusWidthMin.EnableWindow(true);
	m_AnnulusWidthMax.EnableWindow(m_CheckOnOff.GetCheck() == BST_UNCHECKED);
	m_AnnulusWidthStep.EnableWindow(m_CheckOnOff.GetCheck() == BST_UNCHECKED);
}

void CCircleAnnulusDlg::OnBnClickedCheckonoff()
{
	if (m_CheckOnOff.GetCheck() == BST_UNCHECKED) {
		m_RadiusMin.EnableWindow(true);
		m_RadiusMax.EnableWindow(true);
		m_RadiusStep.EnableWindow(true);
		m_AnnulusWidthMax.EnableWindow(true);
		m_AnnulusWidthStep.EnableWindow(true);
		m_AnnulusWidthMin.EnableWindow(true);
		disableSecondaryColor();
	} 
	else {
		m_RadiusMin.EnableWindow(true);
		m_RadiusMax.EnableWindow(false);
		m_RadiusStep.EnableWindow(false);
		m_AnnulusWidthMax.EnableWindow(false);
		m_AnnulusWidthStep.EnableWindow(false);
		m_AnnulusWidthMin.EnableWindow(m_CirclesOnly.GetCheck() == BST_UNCHECKED);
		enableSecondaryColor();
	}
}

void CCircleAnnulusDlg::OnBnClickedPrimaryadd()
{
	float red, green, blue;
	char buffer[100];

	m_PrimaryRed.GetWindowText(buffer, 100);
	red = etu::StringToFloat(buffer, 0);

	m_PrimaryGreen.GetWindowText(buffer, 100);
	green = etu::StringToFloat(buffer, 0);

	m_PrimaryBlue.GetWindowText(buffer, 100);
	blue = etu::StringToFloat(buffer, 0);

	g_pCircleAnnulus->addColor(red, green, blue);
	refreshPrimaryList();
}

void CCircleAnnulusDlg::OnBnClickedPrimaryremove()
{
	g_pCircleAnnulus->removeColor(m_PrimaryList.GetCurSel());
	refreshPrimaryList();
}


void CCircleAnnulusDlg::OnBnClickedPrimaryclear()
{
	g_pCircleAnnulus->clearColors();
	refreshPrimaryList();
}

void CCircleAnnulusDlg::OnBnClickedSecondaryadd()
{
	float red, green, blue;
	char buffer[100];

	m_SecondaryRed.GetWindowText(buffer, 100);
	red = etu::StringToFloat(buffer, 0);

	m_SecondaryGreen.GetWindowText(buffer, 100);
	green = etu::StringToFloat(buffer, 0);

	m_SecondaryBlue.GetWindowText(buffer, 100);
	blue = etu::StringToFloat(buffer, 0);

	g_pCircleAnnulus->addSecondaryColor(red, green, blue);
	refreshSecondaryList();
}

void CCircleAnnulusDlg::OnBnClickedSecondaryremove()
{
	g_pCircleAnnulus->removeSecondaryColor(m_SecondaryList.GetCurSel());
	refreshSecondaryList();
}

void CCircleAnnulusDlg::OnBnClickedSecondaryclear()
{
	g_pCircleAnnulus->clearSecondaryColors();
	refreshSecondaryList();
}

void CCircleAnnulusDlg::OnBnClickedApply()
{
	save();
	int numLeft = -1;
	if (!g_pCircleAnnulus->getOnOff()) {
		float fNumCircles = 1;
		float fNumAnnuli = 1;
		float totalNumCircles = 0;
		float totalNumAnnuli = 0;
		
		if (g_pCircleAnnulus->getShape() != CCircleAnnulus::SHAPES_ANNULI) {
			fNumCircles = ((g_pCircleAnnulus->getCircleRadiusMax() - g_pCircleAnnulus->getCircleRadiusMin()) / g_pCircleAnnulus->getCircleRadiusStep())+1;
			totalNumCircles = fNumCircles * g_pCircleAnnulus->getNumColors();
		}

		if (g_pCircleAnnulus->getShape() != CCircleAnnulus::SHAPES_CIRCLES) {
			fNumAnnuli = ((g_pCircleAnnulus->getAnnulusWidthMax() - g_pCircleAnnulus->getAnnulusWidthMin()) / g_pCircleAnnulus->getAnnulusWidthStep())+1;
			totalNumAnnuli = fNumAnnuli * totalNumCircles;
		} 

		numLeft = (int)totalNumAnnuli + (int)totalNumCircles;

		if (m_InterleavedFields.GetCheck() == BST_CHECKED) {
			numLeft *= 2;
		}
	}

	updateDisplayStrings(-1, -1, -1, numLeft*g_pCircleAnnulus->getRepeats());	
}

void CCircleAnnulusDlg::refreshPrimaryList() {
	m_PrimaryList.ResetContent();
	for (int i = 0; i < g_pCircleAnnulus->getNumColors(); i++) {
		m_PrimaryList.AddString(g_pCircleAnnulus->getColorString(i));
	}
}

void CCircleAnnulusDlg::refreshSecondaryList() {
	m_SecondaryList.ResetContent();
	for (int i = 0; i < g_pCircleAnnulus->getNumSecondaryColors(); i++) {
		m_SecondaryList.AddString(g_pCircleAnnulus->getSecondaryColorString(i));
	}
}

void CCircleAnnulusDlg::updateDisplayStrings(float radius, float annulusWidth, int repeatNum, int trialsLeft) {
	if (radius > -0.5) {
		m_StaticRadius.SetWindowText(etu::FloatToString(radius));
	} 
	else {
		m_StaticRadius.SetWindowText("");
	}

	if (annulusWidth > -0.5) {
		m_StaticAnnulusWidth.SetWindowText(etu::FloatToString(annulusWidth));
	} 
	else {
		m_StaticAnnulusWidth.SetWindowText("");
	}

	if (repeatNum > -0.5) {
		m_StaticRepeat.SetWindowText(etu::IntToString(repeatNum));
	} 
	else {
		m_StaticRepeat.SetWindowText("");
	}

	if (trialsLeft > -0.5) {
		m_StaticTrialsLeft.SetWindowText(etu::IntToString(trialsLeft));
	} 
	else {
		m_StaticTrialsLeft.SetWindowText("Infinite");
	}
}

void CCircleAnnulusDlg::disableControls() 
{}

void CCircleAnnulusDlg::enableControls() 
{}

void CCircleAnnulusDlg::OnBnClickedStartPresentation()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() == "Circle Annulus Stimulus") {
		disableControls();
		((CCircleAnnulusStimulus*)g_pActiveStimulus)->StartTrials(true);
	}
}

void CCircleAnnulusDlg::disableStartPresentations() 
{
	m_StartPresentations.EnableWindow(false);
}

void CCircleAnnulusDlg::enableStartPresentations() 
{
	m_StartPresentations.EnableWindow(true);
}

void CCircleAnnulusDlg::disableSecondaryColor() 
{
	m_SecondaryRed.EnableWindow(false);
	m_SecondaryGreen.EnableWindow(false);
	m_SecondaryBlue.EnableWindow(false);
	m_SecondaryAdd.EnableWindow(false);
	m_SecondaryRemove.EnableWindow(false);
	m_SecondaryClear.EnableWindow(false);
	m_SecondaryList.EnableWindow(false);
}

void CCircleAnnulusDlg::enableSecondaryColor() 
{
	m_SecondaryRed.EnableWindow(true);
	m_SecondaryGreen.EnableWindow(true);
	m_SecondaryBlue.EnableWindow(true);
	m_SecondaryAdd.EnableWindow(true);
	m_SecondaryRemove.EnableWindow(true);
	m_SecondaryClear.EnableWindow(true);
	m_SecondaryList.EnableWindow(true);
}

void CCircleAnnulusDlg::updateFieldStrings() 
{
	char buffer[200];
	sprintf(buffer, "X Position:\t%.3f deg\n\nY Position:\t%.3f deg\n\nWidth:\t\t%.3f deg\n\nHeight:\t\t%.3f deg\n\nBar Width:\t%.3f deg\n\nBar Height:\t%.3f deg\n\nOrientation:\t%.3f deg\n\nCircle Radius:\t%.3f deg", g_receptiveFieldLeft.xPos, g_receptiveFieldLeft.yPos, g_receptiveFieldLeft.width, g_receptiveFieldLeft.height, g_receptiveFieldLeft.barWidth, g_receptiveFieldLeft.barHeight, g_receptiveFieldLeft.orientation, g_receptiveFieldLeft.circleRadius);
	m_StaticLeftEyeField.SetWindowText(buffer);
	
	sprintf(buffer, "X Position:\t%.3f deg\n\nY Position:\t%.3f deg\n\nWidth:\t\t%.3f deg\n\nHeight:\t\t%.3f deg\n\nBar Width:\t%.3f deg\n\nBar Height:\t%.3f deg\n\nOrientation:\t%.3f deg\n\nCircle Radius:\t%.3f deg", g_receptiveFieldRight.xPos, g_receptiveFieldRight.yPos, g_receptiveFieldRight.width, g_receptiveFieldRight.height, g_receptiveFieldRight.barWidth, g_receptiveFieldRight.barHeight, g_receptiveFieldRight.orientation, g_receptiveFieldRight.circleRadius);
	m_StaticRightEyeField.SetWindowText(buffer);
}


void CCircleAnnulusDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	updateFieldStrings();
}

void CCircleAnnulusDlg::OnStnClickedStaticlefteye()
{}

#endif

