/**
 * @file OrientationTuningDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "OrientationTuningDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(COrientationTuningDlg, CDialog)
COrientationTuningDlg::COrientationTuningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrientationTuningDlg::IDD, pParent)
{}

COrientationTuningDlg::~COrientationTuningDlg()
{}

void COrientationTuningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BarWidth, m_BarWidth);
	DDX_Control(pDX, IDC_BarHeight, m_BarHeight);
	DDX_Control(pDX, IDC_AngularStep, m_AngularStep);
	DDX_Control(pDX, IDC_MotionStep, m_MotionSpeed);
	DDX_Control(pDX, IDC_IntertrialTime, m_IntertrialTime);
	DDX_Control(pDX, IDC_TrialDuration, m_TrialDuration);
	DDX_Control(pDX, IDC_nRepeats, m_nRepeats);

	DDX_Control(pDX, IDC_UnitTag, m_UnitTag);
	DDX_Control(pDX, IDC_STATIC_ORIENTATION, m_StaticOrientation);
	DDX_Control(pDX, IDC_STATIC_REPEAT, m_StaticRepeatNumber);
	DDX_Control(pDX, IDC_STATIC_TRIALS_LEFT, m_StaticTrialsLeft);
	DDX_Control(pDX, IDC_START_PRESENTATION, m_StartPresentations);
	DDX_Control(pDX, IDC_BlankPeriod, m_BlankPeriod);
	DDX_Control(pDX, IDC_APPLY, m_Apply);
	DDX_Control(pDX, IDC_StaticLeftEye, m_StaticLeftEyeField);
	DDX_Control(pDX, IDC_StaticRightEye, m_StaticRightEyeField);
	DDX_Control(pDX, IDC_RightEyeField, m_RightEyeField);
	DDX_Control(pDX, IDC_LeftEyeField, m_LeftEyeField);
	DDX_Control(pDX, IDC_InterleavedFields, m_InterleavedFields);
	DDX_Control(pDX, IDC_ManualFields, m_ManualFields);
	DDX_Control(pDX, IDC_XPosition, m_XPosition);
	DDX_Control(pDX, IDC_YPosition, m_YPosition);
	DDX_Control(pDX, IDC_StartAngle, m_StartAngle);
	DDX_Control(pDX, IDC_EndAngle, m_EndAngle);
	DDX_Control(pDX, IDC_FixX, m_FixX);
	DDX_Control(pDX, IDC_FixY, m_FixY);
	DDX_Control(pDX, IDC_FixD, m_FixD);
	DDX_Control(pDX, IDC_EDIT3, m_FixColorR);
	DDX_Control(pDX, IDC_EDIT4, m_FixColorG);
	DDX_Control(pDX, IDC_EDIT5, m_FixColorB);
	DDX_Control(pDX, IDC_CHECK1, m_UseFixation);
}

BEGIN_MESSAGE_MAP(COrientationTuningDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDC_START_PRESENTATION, OnBnClickedStartPresentation)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RightEyeField, OnBnClickedRighteyefield)
	ON_BN_CLICKED(IDC_LeftEyeField, OnBnClickedLefteyefield)
	ON_BN_CLICKED(IDC_InterleavedFields, OnBnClickedInterleavedfields)
	ON_BN_CLICKED(IDC_ManualFields, OnBnClickedManualfields)
END_MESSAGE_MAP()

BOOL COrientationTuningDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	m_StartPresentations.EnableWindow(false);
	OnBnClickedApply();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COrientationTuningDlg::load() 
{	
	m_BarWidth.SetWindowText(etu::FloatToString(g_pOrientationTuning->getBarWidth()));
	m_BarHeight.SetWindowText(etu::FloatToString(g_pOrientationTuning->getBarHeight()));
	m_AngularStep.SetWindowText(etu::FloatToString(g_pOrientationTuning->getAngularStep()));
	m_MotionSpeed.SetWindowText(etu::FloatToString(g_pOrientationTuning->getMotionSpeed()));
	m_IntertrialTime.SetWindowText(etu::IntToString(g_pOrientationTuning->getIntertrialTime()));
	m_TrialDuration.SetWindowText(etu::IntToString(g_pOrientationTuning->getTrialDuration()));
	m_nRepeats.SetWindowText(etu::IntToString(g_pOrientationTuning->getRepeats()));
	m_XPosition.SetWindowText(etu::FloatToString(g_pOrientationTuning->getXPosition()));
	m_YPosition.SetWindowText(etu::FloatToString(g_pOrientationTuning->getYPosition()));
	m_UnitTag.SetWindowText(g_pOrientationTuning->getUnitTag());
	m_StartAngle.SetWindowText(etu::FloatToString(g_pOrientationTuning->getStartAngle()));
	m_EndAngle.SetWindowText(etu::FloatToString(g_pOrientationTuning->getEndAngle()));
	
	m_BlankPeriod.SetWindowText(etu::IntToString(g_pOrientationTuning->getBlankPeriod()));

	if (g_pOrientationTuning->getField() == COrientationTuning::FIELD_RIGHT_EYE) {
		OnBnClickedRighteyefield();
	} 
	else if (g_pOrientationTuning->getField() == COrientationTuning::FIELD_LEFT_EYE) {
		OnBnClickedLefteyefield();
	} 
	else if (g_pOrientationTuning->getField() == COrientationTuning::FIELD_INTERLEAVED) {
		OnBnClickedInterleavedfields();
	} 
	else  {
		OnBnClickedManualfields();
	}

	m_FixX.SetWindowText(etu::FloatToString(g_pOrientationTuning->getFixX()));
	m_FixY.SetWindowText(etu::FloatToString(g_pOrientationTuning->getFixY()));
	m_FixD.SetWindowText(etu::FloatToString(g_pOrientationTuning->getFixD()));
	
	m_FixColorR.SetWindowText(etu::IntToString(g_pOrientationTuning->getFixColorR()));
	m_FixColorG.SetWindowText(etu::IntToString(g_pOrientationTuning->getFixColorG()));
	m_FixColorB.SetWindowText(etu::IntToString(g_pOrientationTuning->getFixColorB()));
	m_UseFixation.SetCheck(g_pOrientationTuning->getUseFixation()? BST_CHECKED : BST_UNCHECKED);
}

void COrientationTuningDlg::save() 
{
	char buffer[100];
	m_BarWidth.GetWindowText(buffer, 100);
	g_pOrientationTuning->setBarWidth(etu::StringToFloat(buffer, g_pOrientationTuning->getBarWidth()));

	m_BarHeight.GetWindowText(buffer, 100);
	g_pOrientationTuning->setBarHeight(etu::StringToFloat(buffer, g_pOrientationTuning->getBarHeight()));

	m_AngularStep.GetWindowText(buffer, 100);
	g_pOrientationTuning->setAngularStep(etu::StringToFloat(buffer, g_pOrientationTuning->getAngularStep()));

	m_MotionSpeed.GetWindowText(buffer, 100);
	g_pOrientationTuning->setMotionSpeed(etu::StringToFloat(buffer, g_pOrientationTuning->getMotionSpeed()));

	m_IntertrialTime.GetWindowText(buffer, 100);
	g_pOrientationTuning->setIntertrialTime(etu::StringToInt(buffer, g_pOrientationTuning->getIntertrialTime()));

	m_TrialDuration.GetWindowText(buffer, 100);
	g_pOrientationTuning->setTrialDuration(etu::StringToInt(buffer, g_pOrientationTuning->getTrialDuration()));

	m_nRepeats.GetWindowText(buffer, 100);
	g_pOrientationTuning->setRepeats(etu::StringToInt(buffer, g_pOrientationTuning->getRepeats()));

	m_XPosition.GetWindowText(buffer, 100);
	g_pOrientationTuning->setXPosition(etu::StringToFloat(buffer, g_pOrientationTuning->getXPosition()));

	m_YPosition.GetWindowText(buffer, 100);
	g_pOrientationTuning->setYPosition(etu::StringToFloat(buffer, g_pOrientationTuning->getYPosition()));

	m_UnitTag.GetWindowText(buffer, 100);
	g_pOrientationTuning->setUnitTag(buffer);

	m_BlankPeriod.GetWindowText(buffer, 100);
	g_pOrientationTuning->setBlankPeriod(etu::StringToInt(buffer, g_pOrientationTuning->getBlankPeriod()));

	m_StartAngle.GetWindowText(buffer, 100);
	g_pOrientationTuning->setStartAngle(etu::StringToFloat(buffer, g_pOrientationTuning->getStartAngle()));

	m_EndAngle.GetWindowText(buffer, 100);
	g_pOrientationTuning->setEndAngle(etu::StringToFloat(buffer, g_pOrientationTuning->getEndAngle()));

	if (m_RightEyeField.GetCheck() == BST_CHECKED) {
		g_pOrientationTuning->setField(COrientationTuning::FIELD_RIGHT_EYE);
	} 
	else if (m_LeftEyeField.GetCheck() == BST_CHECKED) {
		g_pOrientationTuning->setField(COrientationTuning::FIELD_LEFT_EYE);
	} 
	else if (m_InterleavedFields.GetCheck() == BST_CHECKED) {
		g_pOrientationTuning->setField(COrientationTuning::FIELD_INTERLEAVED);
	} 
	else {
		g_pOrientationTuning->setField(COrientationTuning::FIELD_MANUAL);
	}

	m_FixX.GetWindowText(buffer, 100);
	g_pOrientationTuning->setFixX(etu::StringToFloat(buffer, g_pOrientationTuning->getFixX()));

	m_FixY.GetWindowText(buffer, 100);
	g_pOrientationTuning->setFixY(etu::StringToFloat(buffer, g_pOrientationTuning->getFixY()));

	m_FixD.GetWindowText(buffer, 100);
	g_pOrientationTuning->setFixD(etu::StringToFloat(buffer, g_pOrientationTuning->getFixD()));

	m_FixColorR.GetWindowText(buffer, 100);
	g_pOrientationTuning->setFixColorR(etu::StringToInt(buffer, g_pOrientationTuning->getFixColorR()));

	m_FixColorG.GetWindowText(buffer, 100);
	g_pOrientationTuning->setFixColorG(etu::StringToInt(buffer, g_pOrientationTuning->getFixColorG()));

	m_FixColorB.GetWindowText(buffer, 100);
	g_pOrientationTuning->setFixColorB(etu::StringToInt(buffer, g_pOrientationTuning->getFixColorB()));

	g_pOrientationTuning->setUseFixation(m_UseFixation.GetCheck() == BST_CHECKED);
}

void COrientationTuningDlg::OnBnClickedOk()
{
	OnBnClickedApply();
	OnOK();
}

void COrientationTuningDlg::OnBnClickedCancel()
{
	OnCancel();
}

void COrientationTuningDlg::OnBnClickedApply()
{
	save();
	int numTrials = (int)((g_pOrientationTuning->getEndAngle() - g_pOrientationTuning->getStartAngle()) / g_pOrientationTuning->getAngularStep()) +1;
	if (g_pOrientationTuning->getField() == COrientationTuning::FIELD_INTERLEAVED)
		numTrials *= 2;
	updateDisplayStrings(-1, -1, numTrials * g_pOrientationTuning->getRepeats(), -1, -1);	
}

void COrientationTuningDlg::OnBnClickedStartPresentation()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() == "Orientation Tuning Stimulus") {
		disableControls();
		
		((COrientationTuningStimulus*)g_pActiveStimulus)->StartTrials(true);
	}
}

void COrientationTuningDlg::updateDisplayStrings(float orientation, int repeat, int numLeft, float circleRadius, float annulusWidth) 
{
	if (orientation >= -0.5) 
		m_StaticOrientation.SetWindowText(etu::FloatToString(orientation));
	else 
		m_StaticOrientation.SetWindowText("");

	if (repeat >= 0) 
		m_StaticRepeatNumber.SetWindowText(etu::IntToString(repeat));
	else
		m_StaticRepeatNumber.SetWindowText("");
	
	m_StaticTrialsLeft.SetWindowText(etu::IntToString(numLeft));
}

void COrientationTuningDlg::enableControls() 
{
	m_XPosition.EnableWindow(true);
	m_YPosition.EnableWindow(true);
	m_TrialDuration.EnableWindow(true);
	m_IntertrialTime.EnableWindow(true);
	m_nRepeats.EnableWindow(true);
	m_UnitTag.EnableWindow(true);
	m_BlankPeriod.EnableWindow(true);
	m_BarWidth.EnableWindow(true);
	m_BarHeight.EnableWindow(true);
	m_StartAngle.EnableWindow(true);
	m_EndAngle.EnableWindow(true);
	m_AngularStep.EnableWindow(true);
	m_LeftEyeField.EnableWindow(true);
	m_RightEyeField.EnableWindow(true);
	m_InterleavedFields.EnableWindow(true);
	m_ManualFields.EnableWindow(true);

	if (m_ManualFields.GetCheck()== BST_CHECKED) {
        OnBnClickedManualfields();
	} 
	else if (m_InterleavedFields.GetCheck() == BST_CHECKED) {
		OnBnClickedInterleavedfields();
	} 
	else if (m_RightEyeField.GetCheck() == BST_CHECKED) {
		OnBnClickedRighteyefield();
	} 
	else {
		OnBnClickedLefteyefield();
	}
}

void COrientationTuningDlg::disableControls() 
{
	m_StartPresentations.EnableWindow(false);
	m_XPosition.EnableWindow(false);
	m_YPosition.EnableWindow(false);
	m_TrialDuration.EnableWindow(false);
	m_IntertrialTime.EnableWindow(false);
	m_nRepeats.EnableWindow(false);
	m_UnitTag.EnableWindow(false);
	m_BlankPeriod.EnableWindow(false);
	m_BarWidth.EnableWindow(false);
	m_BarHeight.EnableWindow(false);
	m_StartAngle.EnableWindow(false);
	m_EndAngle.EnableWindow(false);
	m_AngularStep.EnableWindow(false);
	m_LeftEyeField.EnableWindow(false);
	m_RightEyeField.EnableWindow(false);
	m_InterleavedFields.EnableWindow(false);
	m_ManualFields.EnableWindow(false);
}

void COrientationTuningDlg::enableApply() 
{
	m_Apply.EnableWindow(true);
}

void COrientationTuningDlg::disableApply() 
{
	m_Apply.EnableWindow(false);
}

void COrientationTuningDlg::disableStartPresentations() 
{
	m_StartPresentations.EnableWindow(false);

}

void COrientationTuningDlg::enableStartPresentations() 
{
	m_StartPresentations.EnableWindow(true);
}

void COrientationTuningDlg::updateFieldStrings() 
{
	char buffer[200];
	sprintf(buffer, "X Position:\t%.3f deg\n\nY Position:\t%.3f deg\n\nWidth:\t\t%.3f deg\n\nHeight:\t\t%.3f deg\n\nBar Width:\t%.3f deg\n\nBar Height:\t%.3f deg\n\nOrientation:\t%.3f deg\n\nCircle Radius:\t%.3f deg", g_receptiveFieldLeft.xPos, g_receptiveFieldLeft.yPos, g_receptiveFieldLeft.width, g_receptiveFieldLeft.height, g_receptiveFieldLeft.barWidth, g_receptiveFieldLeft.barHeight, g_receptiveFieldLeft.orientation, g_receptiveFieldLeft.circleRadius);
	m_StaticLeftEyeField.SetWindowText(buffer);
	
	sprintf(buffer, "X Position:\t%.3f deg\n\nY Position:\t%.3f deg\n\nWidth:\t\t%.3f deg\n\nHeight:\t\t%.3f deg\n\nBar Width:\t%.3f deg\n\nBar Height:\t%.3f deg\n\nOrientation:\t%.3f deg\n\nCircle Radius:\t%.3f deg", g_receptiveFieldRight.xPos, g_receptiveFieldRight.yPos, g_receptiveFieldRight.width, g_receptiveFieldRight.height, g_receptiveFieldRight.barWidth, g_receptiveFieldRight.barHeight, g_receptiveFieldRight.orientation, g_receptiveFieldRight.circleRadius);
	m_StaticRightEyeField.SetWindowText(buffer);

}

void COrientationTuningDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	updateFieldStrings();
}

void COrientationTuningDlg::OnBnClickedRighteyefield()
{
	m_RightEyeField.SetCheck(BST_CHECKED);
	m_LeftEyeField.SetCheck(BST_UNCHECKED);
	m_InterleavedFields.SetCheck(BST_UNCHECKED);
	m_ManualFields.SetCheck(BST_UNCHECKED);

	m_XPosition.EnableWindow(false);
	m_YPosition.EnableWindow(false);
	m_BarWidth.EnableWindow(true);
	m_BarHeight.EnableWindow(true);
}

void COrientationTuningDlg::OnBnClickedLefteyefield()
{
	m_RightEyeField.SetCheck(BST_UNCHECKED);
	m_LeftEyeField.SetCheck(BST_CHECKED);
	m_InterleavedFields.SetCheck(BST_UNCHECKED);
	m_ManualFields.SetCheck(BST_UNCHECKED);

	m_XPosition.EnableWindow(false);
	m_YPosition.EnableWindow(false);
	m_BarWidth.EnableWindow(true);
	m_BarHeight.EnableWindow(true);
}

void COrientationTuningDlg::OnBnClickedInterleavedfields()
{
	m_RightEyeField.SetCheck(BST_UNCHECKED);
	m_LeftEyeField.SetCheck(BST_UNCHECKED);
	m_InterleavedFields.SetCheck(BST_CHECKED);
	m_ManualFields.SetCheck(BST_UNCHECKED);

	m_XPosition.EnableWindow(false);
	m_YPosition.EnableWindow(false);
	m_BarWidth.EnableWindow(false);
	m_BarHeight.EnableWindow(false);
}

void COrientationTuningDlg::OnBnClickedManualfields()
{
	m_RightEyeField.SetCheck(BST_UNCHECKED);
	m_LeftEyeField.SetCheck(BST_UNCHECKED);
	m_InterleavedFields.SetCheck(BST_UNCHECKED);
	m_ManualFields.SetCheck(BST_CHECKED);

	m_XPosition.EnableWindow(true);
	m_YPosition.EnableWindow(true);
	m_BarWidth.EnableWindow(true);
	m_BarHeight.EnableWindow(true);
}


#endif
