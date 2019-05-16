/**
 * @file SaccadeDlg.cpp
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
#include "SaccadeDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CSaccadeDlg, CDialog)
CSaccadeDlg::CSaccadeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaccadeDlg::IDD, pParent)
{}

CSaccadeDlg::~CSaccadeDlg()
{}

void CSaccadeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EditDotSize, m_DotSize);
	DDX_Control(pDX, IDC_EditStepSize, m_StepSize);
	DDX_Control(pDX, IDC_EditNumSteps, m_NumSteps);
	DDX_Control(pDX, IDC_EditDuration, m_EditDuration);
	DDX_Control(pDX, IDC_CheckReturnToPt, m_ReturnToPt);
	DDX_Control(pDX, IDC_CheckUseAllDir, m_UseAllDir);
	DDX_Control(pDX, IDC_RadioRadial, m_RadioRadial);
	DDX_Control(pDX, IDC_RadioXY, m_RadioXY);
	DDX_Control(pDX, IDC_EditPointX, m_PointX);
	DDX_Control(pDX, IDC_EditPointY, m_PointY);
	DDX_Control(pDX, IDC_CheckHorizontal, m_Horizontal);
	DDX_Control(pDX, IDC_CheckVertical, m_Vertical);
	DDX_Control(pDX, IDC_Check45Deg, m_45Deg);
	DDX_Control(pDX, IDC_Check135Deg, m_135Deg);
	DDX_Control(pDX, IDC_RANDOM_DURATIONS, m_RandomDurations);
	DDX_Control(pDX, IDC_MIN_DURATION, m_MinDuration);
	DDX_Control(pDX, IDC_MAX_DURATION, m_MaxDuration);
	DDX_Control(pDX, IDC_EnableOscillations, m_EnableOscillations);
	DDX_Control(pDX, IDC_OscillationAmplitude, m_OscillationAmplitude);
	DDX_Control(pDX, IDC_OscillationFreq, m_OscillationFreq);
	DDX_Control(pDX, IDC_RadioHorizontal, m_RadioHorizontal);
	DDX_Control(pDX, IDC_RadioVertical, m_RadioVertical);
	DDX_Control(pDX, IDC_Radio45Deg, m_Radio45Deg);
	DDX_Control(pDX, IDC_Radio135Deg, m_Radio135Deg);
	DDX_Control(pDX, IDC_Check_Behvaior, m_Behavior);
	DDX_Control(pDX, IDC_MaxDisplayDuration, m_MaxDisplayDuration);
	DDX_Control(pDX, IDC_BlankTime, m_BlankTime);
	DDX_Control(pDX, IDC_JITTER, m_JitterX);
	DDX_Control(pDX, IDC_JITTERY, m_JitterY);
	DDX_Control(pDX, IDC_AVOID_ZERO, m_AvoidZero);
	DDX_Control(pDX, IDC_RadioPrimary, m_RadioPrimary);
	DDX_Control(pDX, IDC_RadioSecondary, m_RadioSecondary);
	DDX_Control(pDX, IDC_RadioAlternate, m_RadioAlternate);
	DDX_Control(pDX, IDC_EditRed, m_RedPrimary);
	DDX_Control(pDX, IDC_EditGreen, m_GreenPrimary);
	DDX_Control(pDX, IDC_EditBlue, m_BluePrimary);
	DDX_Control(pDX, IDC_EditRedSec, m_RedSecondary);
	DDX_Control(pDX, IDC_EditGreenSec, m_GreenSecondary);
	DDX_Control(pDX, IDC_EditBlueSec, m_BlueSecondary);
	DDX_Control(pDX, IDC_RadioRandom, m_RadioRandom);
	DDX_Control(pDX, IDC_EditNumStepsY, m_NumStepsY);
	DDX_Control(pDX, IDC_PrimaryStatic, m_PrimaryStatic);
	DDX_Control(pDX, IDC_SecondaryStatic, m_SecondaryStatic);
	DDX_Control(pDX, IDC_GridSteps, m_GridSteps);
	DDX_Control(pDX, IDC_PointDot, m_PointDot);
	DDX_Control(pDX, IDC_PointLandoltC, m_PointLandoltC);
	DDX_Control(pDX, IDC_PointE, m_PointE);
	DDX_Control(pDX, IDC_SequentialOrder, m_SequentialOrder);

	/* Noise handling. */
	DDX_Control(pDX, IDC_saccadeDisplayNoise, m_displayNoiseBg);
	DDX_Control(pDX, IDC_saccadeSameBgTrial, m_sameBgTrial);
	DDX_Control(pDX, IDC_saccadeAlternateNoise, m_alternateNoise);
	DDX_Control(pDX, IDC_saccadeOccludeBG, m_occludeBg);
	DDX_Control(pDX, IDC_saccadeNoiseRed, m_noiseRed);
	DDX_Control(pDX, IDC_saccadeNoiseGreen, m_noiseGreen);
	DDX_Control(pDX, IDC_saccadeNoiseBlue, m_noiseBlue);
	DDX_Control(pDX, IDC_saccadeNoisePercent, m_noisePercent);
	DDX_Control(pDX, IDC_saccadeNoiseSize, m_noiseSize);
	DDX_Control(pDX, IDC_saccadeNoiseStatic, m_noiseStatic);
}

BEGIN_MESSAGE_MAP(CSaccadeDlg, CDialog)
	ON_BN_CLICKED(IDC_CheckReturnToPt, OnBnClickedCheckreturntopt)
	ON_BN_CLICKED(IDC_RadioRadial, OnBnClickedRadioradial)
	ON_BN_CLICKED(IDC_RadioXY, OnBnClickedRadioxy)
	ON_BN_CLICKED(IDC_CheckUseAllDir, OnBnClickedCheckusealldir)
	ON_BN_CLICKED(IDC_ButtonApply, OnBnClickedButtonapply)
	ON_BN_CLICKED(IDC_CheckHorizontal, OnBnClickedCheckhorizontal)
	ON_BN_CLICKED(IDC_CheckVertical, OnBnClickedCheckvertical)
	ON_BN_CLICKED(IDC_Check45Deg, OnBnClickedCheck45deg)
	ON_BN_CLICKED(IDC_Check135Deg, OnBnClickedCheck135deg)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RANDOM_DURATIONS, OnBnClickedRandomDurations)
	ON_BN_CLICKED(IDC_RadioHorizontal, OnBnClickedRadiohorizontal)
	ON_BN_CLICKED(IDC_RadioVertical, OnBnClickedRadiovertical)
	ON_BN_CLICKED(IDC_Radio45Deg, OnBnClickedRadio45deg)
	ON_BN_CLICKED(IDC_Radio135Deg, OnBnClickedRadio135deg)
	ON_BN_CLICKED(IDC_EnableOscillations, OnBnClickedEnableoscillations)
	ON_BN_CLICKED(IDC_Check_Behvaior, OnBnClickedCheckBehvaior)
	ON_BN_CLICKED(IDC_RadioPrimary, OnBnClickedRadioprimary)
	ON_BN_CLICKED(IDC_RadioSecondary, OnBnClickedRadiosecondary)
	ON_BN_CLICKED(IDC_RadioAlternate, OnBnClickedRadioalternate)
	ON_BN_CLICKED(IDC_RadioRandom, OnBnClickedRadiorandom)
	ON_EN_CHANGE(IDC_EditRedSec, OnEnChangeEditredsec)
	ON_BN_CLICKED(IDC_saccadeDisplayNoise, OnBnClickedDisplayNoiseBg)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CSaccadeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	load();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSaccadeDlg::load() 
{
	m_EditDuration.SetWindowText(etu::IntToString(g_pSaccade->getDisplayDuration()));
	m_DotSize.SetWindowText(etu::FloatToString(g_pSaccade->getDotSize()));
	m_StepSize.SetWindowText(etu::FloatToString(g_pSaccade->getStepSize()));
	m_NumSteps.SetWindowText(etu::IntToString(g_pSaccade->getNumSteps()));
	m_NumStepsY.SetWindowText(etu::IntToString(g_pSaccade->getNumStepsY()));
	m_MinDuration.SetWindowText(etu::IntToString(g_pSaccade->getMinDuration()));
	m_MaxDuration.SetWindowText(etu::IntToString(g_pSaccade->getMaxDuration()));
	m_PointX.SetWindowText(etu::FloatToString(g_pSaccade->getPointX()));
	m_PointY.SetWindowText(etu::FloatToString(g_pSaccade->getPointY()));
	m_OscillationAmplitude.SetWindowText(etu::FloatToString(g_pSaccade->getOscillationAmplitude()));
	m_OscillationFreq.SetWindowText(etu::FloatToString(g_pSaccade->getOscillationFreq()));
	m_MaxDisplayDuration.SetWindowText(etu::IntToString(g_pSaccade->getMaxDisplayDuration()));
	m_BlankTime.SetWindowText(etu::IntToString(g_pSaccade->getBlankTime()));
	m_JitterX.SetWindowText(etu::IntToString(g_pSaccade->getJitterX()));
	m_JitterY.SetWindowText(etu::IntToString(g_pSaccade->getJitterY()));
	m_RedPrimary.SetWindowText(etu::IntToString(g_pSaccade->getRedPrimary()));
	m_GreenPrimary.SetWindowText(etu::IntToString(g_pSaccade->getGreenPrimary()));
	m_BluePrimary.SetWindowText(etu::IntToString(g_pSaccade->getBluePrimary()));
	m_RedSecondary.SetWindowText(etu::IntToString(g_pSaccade->getRedSecondary()));
	m_GreenSecondary.SetWindowText(etu::IntToString(g_pSaccade->getGreenSecondary()));
	m_BlueSecondary.SetWindowText(etu::IntToString(g_pSaccade->getBlueSecondary()));
	
	if (g_pSaccade->useBehavior()) {
		m_Behavior.SetCheck(BST_CHECKED);
	} 
	else {
		m_Behavior.SetCheck(BST_UNCHECKED);
	}
	
	if (g_pSaccade->isOscillationEnabled()) {
		m_EnableOscillations.SetCheck(BST_CHECKED);
	} 
	else {
		m_EnableOscillations.SetCheck(BST_UNCHECKED);
	}

	switch (g_pSaccade->getOscillationDirection()) {
		case CSaccade::ORIENT_HORIZ:
			OnBnClickedRadiohorizontal();
			break;
		case CSaccade::ORIENT_VERT:
			OnBnClickedRadiovertical();
			break;
		case CSaccade::ORIENT_45DEG:
			OnBnClickedRadio45deg();
			break;
		case CSaccade::ORIENT_135DEG:
			OnBnClickedRadio135deg();
			break;
	}

	OnBnClickedEnableoscillations();

	if (g_pSaccade->getRandomDurations()) {
		m_RandomDurations.SetCheck(BST_CHECKED);
	} 
	else {
		m_RandomDurations.SetCheck(BST_UNCHECKED);
		m_MinDuration.EnableWindow(false);
		m_MaxDuration.EnableWindow(false);
	}

	if (g_pSaccade->getStepHorizontal()) 
		m_Horizontal.SetCheck(BST_CHECKED);
	else
		m_Horizontal.SetCheck(BST_UNCHECKED);

	if (g_pSaccade->getStepVertical()) 
		m_Vertical.SetCheck(BST_CHECKED);
	else
		m_Vertical.SetCheck(BST_UNCHECKED);

	if (g_pSaccade->getStep45Deg()) 
		m_45Deg.SetCheck(BST_CHECKED);
	else
		m_45Deg.SetCheck(BST_UNCHECKED);

	if (g_pSaccade->getStep135Deg()) 
		m_135Deg.SetCheck(BST_CHECKED);
	else
		m_135Deg.SetCheck(BST_UNCHECKED);

	switch (g_pSaccade->getDistType()) {
		case CSaccade::DIST_TYPE_RADIAL: OnBnClickedRadioradial(); break;
		case CSaccade::DIST_TYPE_XY: OnBnClickedRadioxy(); break;
	}

	if (g_pSaccade->isReturnToPt()) {
		m_ReturnToPt.SetCheck(BST_CHECKED);
		m_PointX.EnableWindow(true);
		m_PointY.EnableWindow(true);
	} 
	else {
		m_ReturnToPt.SetCheck(BST_UNCHECKED);
		m_PointX.EnableWindow(false);
		m_PointY.EnableWindow(false);
	}

	if (g_pSaccade->useAllDirs()) {
		m_UseAllDir.SetCheck(BST_CHECKED);
		m_Horizontal.EnableWindow(false);
		m_Vertical.EnableWindow(false);
		m_45Deg.EnableWindow(false);
		m_135Deg.EnableWindow(false);
		m_RadioRadial.EnableWindow(true);
		m_RadioXY.EnableWindow(true);
	} 
	else {
		m_UseAllDir.SetCheck(BST_UNCHECKED);
		m_Horizontal.EnableWindow(true);
		m_Vertical.EnableWindow(true);
		m_45Deg.EnableWindow(true);
		m_135Deg.EnableWindow(true);

		if (g_pSaccade->getStep135Deg() || g_pSaccade->getStep45Deg()) {
			m_RadioRadial.EnableWindow(true);
			m_RadioXY.EnableWindow(true);
		} 
		else {
			m_RadioRadial.EnableWindow(false);
			m_RadioXY.EnableWindow(false);
		}
	}

	if (g_pSaccade->avoidZero()) {
		m_AvoidZero.SetCheck(BST_CHECKED);
	} 
	else {
		m_AvoidZero.SetCheck(BST_UNCHECKED);
	}

	if (g_pSaccade->getColorSetting() == CSaccade::COLOR_PRIMARY) {
		OnBnClickedRadioprimary();
	} 
	else if (g_pSaccade->getColorSetting() == CSaccade::COLOR_SECONDARY) {
		OnBnClickedRadiosecondary();
	} 
	else if (g_pSaccade->getColorSetting() == CSaccade::COLOR_ALTERNATE) {
		OnBnClickedRadioalternate();
	} 
	else {
		OnBnClickedRadiorandom();
	}

	OnBnClickedRandomDurations();
	OnBnClickedCheckBehvaior();

	m_GridSteps.SetCheck( (g_pSaccade->getGridSteps())? BST_CHECKED : BST_UNCHECKED);

	switch (g_pSaccade->getPointType()) {
		case 0: m_PointDot.SetCheck(BST_CHECKED);	break;
		case 1: m_PointLandoltC.SetCheck(BST_CHECKED);	break;
		case 2: m_PointE.SetCheck(BST_CHECKED);	break;
	}

	/* Noise handling */
	m_displayNoiseBg.SetCheck(g_pSaccade->getDisplayNoiseBg() ? BST_CHECKED : BST_UNCHECKED);
	m_sameBgTrial.SetCheck(g_pSaccade->getSameBgTrial() ? BST_CHECKED : BST_UNCHECKED);
	m_alternateNoise.SetCheck(g_pSaccade->getAlternateNoise() ? BST_CHECKED : BST_UNCHECKED);
	m_occludeBg.SetCheck(g_pSaccade->getOccludeBg() ? BST_CHECKED : BST_UNCHECKED);
	m_noiseRed.SetWindowText(etu::IntToString(g_pSaccade->getRedNoise()));
	m_noiseGreen.SetWindowText(etu::IntToString(g_pSaccade->getGreenNoise()));
	m_noiseBlue.SetWindowText(etu::IntToString(g_pSaccade->getBlueNoise()));
	m_noisePercent.SetWindowText(etu::IntToString(g_pSaccade->getNoisePercent()));
	m_noiseSize.SetWindowText(etu::IntToString(g_pSaccade->getNoiseSize()));

	m_SequentialOrder.SetCheck((g_pSaccade->getSequentialOrder())? BST_CHECKED : BST_UNCHECKED);
}

void CSaccadeDlg::save() 
{
	char buffer[100];

	m_EditDuration.GetWindowText(buffer, 100);
	g_pSaccade->setDisplayDuration(etu::StringToInt(buffer, g_pSaccade->getDisplayDuration()));

	m_DotSize.GetWindowText(buffer, 100);
	g_pSaccade->setDotSize(etu::StringToFloat(buffer, g_pSaccade->getDotSize()));

	m_NumSteps.GetWindowText(buffer, 100);
	g_pSaccade->setNumSteps(etu::StringToInt(buffer, g_pSaccade->getNumSteps()));

	m_NumStepsY.GetWindowText(buffer, 100);
	g_pSaccade->setNumStepsY(etu::StringToInt(buffer, g_pSaccade->getNumStepsY()));

	m_PointX.GetWindowText(buffer, 100);
	g_pSaccade->setPointX(etu::StringToFloat(buffer, g_pSaccade->getPointX()));
	
	m_PointY.GetWindowText(buffer, 100);
	g_pSaccade->setPointY(etu::StringToFloat(buffer, g_pSaccade->getPointY()));

	m_StepSize.GetWindowText(buffer, 100);
	g_pSaccade->setStepSize(etu::StringToFloat(buffer, g_pSaccade->getStepSize()));

	m_MinDuration.GetWindowText(buffer, 100);
	g_pSaccade->setMinDuration(etu::StringToInt(buffer, g_pSaccade->getMinDuration()));
	
	m_MaxDuration.GetWindowText(buffer, 100);
	g_pSaccade->setMaxDuration(etu::StringToInt(buffer, g_pSaccade->getMaxDuration()));

	m_OscillationAmplitude.GetWindowText(buffer, 100);
	g_pSaccade->setOscillationAmplitude(etu::StringToFloat(buffer, g_pSaccade->getOscillationAmplitude()));

	m_OscillationFreq.GetWindowText(buffer, 100);
	g_pSaccade->setOscillationFreq(etu::StringToFloat(buffer, g_pSaccade->getOscillationFreq()));

	m_MaxDisplayDuration.GetWindowText(buffer, 100);
	g_pSaccade->setMaxDisplayDuration(etu::StringToInt(buffer, g_pSaccade->getMaxDisplayDuration()));

	m_BlankTime.GetWindowText(buffer, 100);
	g_pSaccade->setBlankTime(etu::StringToInt(buffer, g_pSaccade->getBlankTime()));

	m_JitterX.GetWindowText(buffer, 100);
	g_pSaccade->setJitterX(etu::StringToInt(buffer, g_pSaccade->getJitterX()));

	m_JitterY.GetWindowText(buffer, 100);
	g_pSaccade->setJitterY(etu::StringToInt(buffer, g_pSaccade->getJitterY()));

	m_RedPrimary.GetWindowText(buffer, 100);
	g_pSaccade->setRedPrimary(etu::StringToInt(buffer, g_pSaccade->getRedPrimary()));

	m_GreenPrimary.GetWindowText(buffer, 100);
	g_pSaccade->setGreenPrimary(etu::StringToInt(buffer, g_pSaccade->getGreenPrimary()));

	m_BluePrimary.GetWindowText(buffer, 100);
	g_pSaccade->setBluePrimary(etu::StringToInt(buffer, g_pSaccade->getBluePrimary()));

	m_RedSecondary.GetWindowText(buffer, 100);
	g_pSaccade->setRedSecondary(etu::StringToInt(buffer, g_pSaccade->getRedSecondary()));

	m_GreenSecondary.GetWindowText(buffer, 100);
	g_pSaccade->setGreenSecondary(etu::StringToInt(buffer, g_pSaccade->getGreenSecondary()));

	m_BlueSecondary.GetWindowText(buffer, 100);
	g_pSaccade->setBlueSecondary(etu::StringToInt(buffer, g_pSaccade->getBlueSecondary()));
	
	// check the checkboxes
	g_pSaccade->setReturnToPt(m_ReturnToPt.GetCheck() == BST_CHECKED);
	g_pSaccade->setUseAllDirs(m_UseAllDir.GetCheck() == BST_CHECKED);
	g_pSaccade->setRandomDurations(m_RandomDurations.GetCheck() == BST_CHECKED);
	g_pSaccade->setOsciallationEnabled(m_EnableOscillations.GetCheck() == BST_CHECKED);
	g_pSaccade->setAvoidZero(m_AvoidZero.GetCheck() == BST_CHECKED);

	// Directions
	g_pSaccade->setStepHorizontal(m_Horizontal.GetCheck() == BST_CHECKED);
	g_pSaccade->setStepVertical(m_Vertical.GetCheck() == BST_CHECKED);
	g_pSaccade->setStep45Deg(m_45Deg.GetCheck() == BST_CHECKED);
	g_pSaccade->setStep135Deg(m_135Deg.GetCheck() == BST_CHECKED);

	if (m_RadioHorizontal.GetCheck() == BST_CHECKED) {
		g_pSaccade->setOscillationDirection(CSaccade::ORIENT_HORIZ);
	} 
	else if (m_RadioVertical.GetCheck() == BST_CHECKED) {
		g_pSaccade->setOscillationDirection(CSaccade::ORIENT_VERT);
	} 
	else if (m_Radio45Deg.GetCheck() == BST_CHECKED) {
		g_pSaccade->setOscillationDirection(CSaccade::ORIENT_45DEG);
	} 
	else if (m_Radio135Deg.GetCheck() == BST_CHECKED) {
		g_pSaccade->setOscillationDirection(CSaccade::ORIENT_135DEG);
	} 
	
	g_pSaccade->setGridSteps(m_GridSteps.GetCheck() == BST_CHECKED);

	// Distance Type
	if (m_RadioRadial.GetCheck() == BST_CHECKED)
		g_pSaccade->setDistType(CSaccade::DIST_TYPE_RADIAL);
	else
		g_pSaccade->setDistType(CSaccade::DIST_TYPE_XY);

	g_pSaccade->setBehavior(m_Behavior.GetCheck() == BST_CHECKED);

	if (m_RadioPrimary.GetCheck() == BST_CHECKED) {
		g_pSaccade->setColorSetting(CSaccade::COLOR_PRIMARY);
	} 
	else if (m_RadioSecondary.GetCheck() == BST_CHECKED) {
		g_pSaccade->setColorSetting(CSaccade::COLOR_SECONDARY);
	} 
	else if (m_RadioAlternate.GetCheck() == BST_CHECKED) {
		g_pSaccade->setColorSetting(CSaccade::COLOR_ALTERNATE);
	} 
	else {
		g_pSaccade->setColorSetting(CSaccade::COLOR_RANDOM);
	}
	
	if (m_PointDot.GetCheck() == BST_CHECKED) {
		g_pSaccade->setPointType(0);
	} 
	else if (m_PointLandoltC.GetCheck() == BST_CHECKED) {
		g_pSaccade->setPointType(1);
	} 
	else {
		g_pSaccade->setPointType(2);
	} 

	/* Noise handling */
	g_pSaccade->setDisplayNoiseBg(m_displayNoiseBg.GetCheck() == BST_CHECKED);
	g_pSaccade->setSameBgTrial(m_sameBgTrial.GetCheck() == BST_CHECKED);
	g_pSaccade->setAlternateNoise(m_alternateNoise.GetCheck() == BST_CHECKED);
	g_pSaccade->setOccludeBg(m_occludeBg.GetCheck() == BST_CHECKED);
	m_noiseRed.GetWindowText(buffer, 100);
	g_pSaccade->setRedNoise(etu::StringToInt(buffer, g_pSaccade->getRedNoise()));
	m_noiseGreen.GetWindowText(buffer, 100);
	g_pSaccade->setGreenNoise(etu::StringToInt(buffer, g_pSaccade->getGreenNoise()));
	m_noiseBlue.GetWindowText(buffer, 100);
	g_pSaccade->setBlueNoise(etu::StringToInt(buffer, g_pSaccade->getBlueNoise()));
	m_noisePercent.GetWindowText(buffer, 100);
	g_pSaccade->setNoisePercent(etu::StringToInt(buffer, g_pSaccade->getNoisePercent()));
	m_noiseSize.GetWindowText(buffer, 100);
	g_pSaccade->setNoiseSize(etu::StringToInt(buffer, g_pSaccade->getNoiseSize()));

	g_pSaccade->sendSettings(g_pSettings->theSerialComm);

	g_pSaccade->setSequentialOrder(m_SequentialOrder.GetCheck() == BST_CHECKED);
}

void CSaccadeDlg::OnBnClickedCheckreturntopt()
{
	if (m_ReturnToPt.GetCheck()== BST_CHECKED) {
		m_PointX.EnableWindow(true);
		m_PointY.EnableWindow(true);
	} 
	else {
		m_PointX.EnableWindow(false);
		m_PointY.EnableWindow(false);
	}
}

void CSaccadeDlg::OnPaint()
{
	CDialog::OnPaint();

	CRect rect;

	CPaintDC dc_p(&m_PrimaryStatic);
	m_PrimaryStatic.GetClientRect(rect);
	dc_p.FillSolidRect(rect, RGB(g_pSaccade->getRedPrimary(),g_pSaccade->getGreenPrimary(),g_pSaccade->getBluePrimary()));

	CPaintDC dc_s(&m_SecondaryStatic);
	m_SecondaryStatic.GetClientRect(rect);
	dc_s.FillSolidRect(rect, RGB(g_pSaccade->getRedSecondary(),g_pSaccade->getGreenSecondary(),g_pSaccade->getBlueSecondary()));

	CPaintDC dc_n(&m_noiseStatic);
	m_noiseStatic.GetClientRect(rect);
	dc_n.FillSolidRect(rect, RGB(g_pSaccade->getRedNoise(),g_pSaccade->getGreenNoise(),g_pSaccade->getBlueNoise()));
}

void CSaccadeDlg::OnBnClickedRadioradial()
{
	m_RadioRadial.SetCheck(BST_CHECKED);
	m_RadioXY.SetCheck(BST_UNCHECKED);
}

void CSaccadeDlg::OnBnClickedRadioxy()
{
	m_RadioRadial.SetCheck(BST_UNCHECKED);
	m_RadioXY.SetCheck(BST_CHECKED);
}

void CSaccadeDlg::OnBnClickedCheckusealldir()
{
	if (m_UseAllDir.GetCheck()==BST_CHECKED) {
		m_RadioRadial.EnableWindow(true);
		m_RadioXY.EnableWindow(true);
		m_Horizontal.EnableWindow(false);
		m_Vertical.EnableWindow(false);
		m_45Deg.EnableWindow(false);
		m_135Deg.EnableWindow(false);
	} 
	else {
		m_Horizontal.EnableWindow(true);
		m_Vertical.EnableWindow(true);
		m_45Deg.EnableWindow(true);
		m_135Deg.EnableWindow(true);
		if (m_45Deg.GetCheck() == BST_CHECKED || m_135Deg.GetCheck() == BST_CHECKED) {// obliques{
			m_RadioRadial.EnableWindow(true);
			m_RadioXY.EnableWindow(true);
		} 
		else { // non obliques
			m_RadioRadial.EnableWindow(false);
			m_RadioXY.EnableWindow(false);
		}
	}
}

void CSaccadeDlg::OnBnClickedButtonapply()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "Saccade Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}

	// Pushes all dialog info into the CSaccade instance.
	save();
	
	g_pSaccadeDlg->Invalidate();
	g_pSaccade->GenerateLocations();
}

void CSaccadeDlg::OnBnClickedCheckhorizontal()
{
	m_RadioRadial.EnableWindow(m_45Deg.GetCheck() == BST_CHECKED || m_135Deg.GetCheck() == BST_CHECKED);
	m_RadioXY.EnableWindow(m_45Deg.GetCheck() == BST_CHECKED || m_135Deg.GetCheck() == BST_CHECKED);
}

void CSaccadeDlg::OnBnClickedCheckvertical()
{
	m_RadioRadial.EnableWindow(m_45Deg.GetCheck() == BST_CHECKED || m_135Deg.GetCheck() == BST_CHECKED);
	m_RadioXY.EnableWindow(m_45Deg.GetCheck() == BST_CHECKED || m_135Deg.GetCheck() == BST_CHECKED);
}

void CSaccadeDlg::OnBnClickedCheck45deg()
{
	m_RadioRadial.EnableWindow(m_45Deg.GetCheck() == BST_CHECKED || m_135Deg.GetCheck() == BST_CHECKED);
	m_RadioXY.EnableWindow(m_45Deg.GetCheck() == BST_CHECKED || m_135Deg.GetCheck() == BST_CHECKED);
}

void CSaccadeDlg::OnBnClickedCheck135deg()
{
	m_RadioRadial.EnableWindow(m_45Deg.GetCheck() == BST_CHECKED || m_135Deg.GetCheck() == BST_CHECKED);
	m_RadioXY.EnableWindow(m_45Deg.GetCheck() == BST_CHECKED || m_135Deg.GetCheck() == BST_CHECKED);
}

void CSaccadeDlg::OnBnClickedOk()
{	
	OnBnClickedButtonapply();
}

void CSaccadeDlg::OnBnClickedCancel()
{}

void CSaccadeDlg::OnBnClickedRandomDurations()
{
	m_MaxDuration.EnableWindow(m_RandomDurations.GetCheck() == BST_CHECKED);
	m_MinDuration.EnableWindow(m_RandomDurations.GetCheck() == BST_CHECKED);
	m_EditDuration.EnableWindow(m_RandomDurations.GetCheck() != BST_CHECKED);

}

void CSaccadeDlg::OnBnClickedRadiohorizontal()
{
	m_RadioHorizontal.SetCheck(BST_CHECKED);
	m_RadioVertical.SetCheck(BST_UNCHECKED);
	m_Radio45Deg.SetCheck(BST_UNCHECKED);
	m_Radio135Deg.SetCheck(BST_UNCHECKED);
}

void CSaccadeDlg::OnBnClickedRadiovertical()
{
	m_RadioHorizontal.SetCheck(BST_UNCHECKED);
	m_RadioVertical.SetCheck(BST_CHECKED);
	m_Radio45Deg.SetCheck(BST_UNCHECKED);
	m_Radio135Deg.SetCheck(BST_UNCHECKED);
}

void CSaccadeDlg::OnBnClickedRadio45deg()
{
	m_RadioHorizontal.SetCheck(BST_UNCHECKED);
	m_RadioVertical.SetCheck(BST_UNCHECKED);
	m_Radio45Deg.SetCheck(BST_CHECKED);
	m_Radio135Deg.SetCheck(BST_UNCHECKED);
}

void CSaccadeDlg::OnBnClickedRadio135deg()
{
	m_RadioHorizontal.SetCheck(BST_UNCHECKED);
	m_RadioVertical.SetCheck(BST_UNCHECKED);
	m_Radio45Deg.SetCheck(BST_UNCHECKED);
	m_Radio135Deg.SetCheck(BST_CHECKED);
}

void CSaccadeDlg::OnBnClickedEnableoscillations()
{
	if (m_EnableOscillations.GetCheck() == BST_CHECKED) {
		m_RadioHorizontal.EnableWindow(true);
		m_RadioVertical.EnableWindow(true);
		m_Radio45Deg.EnableWindow(true);
		m_Radio135Deg.EnableWindow(true);
		m_OscillationAmplitude.EnableWindow(true);
		m_OscillationFreq.EnableWindow(true);

	} 
	else {
		m_RadioHorizontal.EnableWindow(false);
		m_RadioVertical.EnableWindow(false);
		m_Radio45Deg.EnableWindow(false);
		m_Radio135Deg.EnableWindow(false);
		m_OscillationAmplitude.EnableWindow(false);
		m_OscillationFreq.EnableWindow(false);
	}
}

void CSaccadeDlg::OnBnClickedCheckBehvaior()
{
	m_EditDuration.EnableWindow(m_Behavior.GetCheck() != BST_CHECKED);
	m_MaxDuration.EnableWindow(m_Behavior.GetCheck() != BST_CHECKED);
	m_MinDuration.EnableWindow(m_Behavior.GetCheck() != BST_CHECKED);
	m_RandomDurations.EnableWindow(m_Behavior.GetCheck() != BST_CHECKED);
	m_MaxDisplayDuration.EnableWindow(m_Behavior.GetCheck() == BST_CHECKED);
	
	if (m_Behavior.GetCheck() == BST_UNCHECKED) {
		OnBnClickedRandomDurations();
	} 
}

void CSaccadeDlg::OnBnClickedRadioprimary()
{
	m_RadioPrimary.SetCheck(BST_CHECKED);
	m_RadioSecondary.SetCheck(BST_UNCHECKED);
	m_RadioAlternate.SetCheck(BST_UNCHECKED);
	m_RadioRandom.SetCheck(BST_UNCHECKED);

	m_RedPrimary.EnableWindow(true);
	m_GreenPrimary.EnableWindow(true);
	m_BluePrimary.EnableWindow(true);

	m_RedSecondary.EnableWindow(false);
	m_GreenSecondary.EnableWindow(false);
	m_BlueSecondary.EnableWindow(false);
}

void CSaccadeDlg::OnBnClickedRadiosecondary()
{
	m_RadioPrimary.SetCheck(BST_UNCHECKED);
	m_RadioSecondary.SetCheck(BST_CHECKED);
	m_RadioAlternate.SetCheck(BST_UNCHECKED);
	m_RadioRandom.SetCheck(BST_UNCHECKED);

	m_RedPrimary.EnableWindow(false);
	m_GreenPrimary.EnableWindow(false);
	m_BluePrimary.EnableWindow(false);

	m_RedSecondary.EnableWindow(true);
	m_GreenSecondary.EnableWindow(true);
	m_BlueSecondary.EnableWindow(true);
}

void CSaccadeDlg::OnBnClickedRadioalternate()
{
	m_RadioPrimary.SetCheck(BST_UNCHECKED);
	m_RadioSecondary.SetCheck(BST_UNCHECKED);
	m_RadioAlternate.SetCheck(BST_CHECKED);
	m_RadioRandom.SetCheck(BST_UNCHECKED);

	m_RedPrimary.EnableWindow(true);
	m_GreenPrimary.EnableWindow(true);
	m_BluePrimary.EnableWindow(true);

	m_RedSecondary.EnableWindow(true);
	m_GreenSecondary.EnableWindow(true);
	m_BlueSecondary.EnableWindow(true);
}

void CSaccadeDlg::OnBnClickedRadiorandom()
{
	m_RadioPrimary.SetCheck(BST_UNCHECKED);
	m_RadioSecondary.SetCheck(BST_UNCHECKED);
	m_RadioAlternate.SetCheck(BST_UNCHECKED);
	m_RadioRandom.SetCheck(BST_CHECKED);

	m_RedPrimary.EnableWindow(true);
	m_GreenPrimary.EnableWindow(true);
	m_BluePrimary.EnableWindow(true);

	m_RedSecondary.EnableWindow(true);
	m_GreenSecondary.EnableWindow(true);
	m_BlueSecondary.EnableWindow(true);
}

void CSaccadeDlg::OnEnChangeEditredsec()
{}

void CSaccadeDlg::OnBnClickedDisplayNoiseBg()
{
	bool turnOn = (BST_CHECKED == m_displayNoiseBg.GetCheck());

	m_sameBgTrial.EnableWindow(turnOn);
	m_alternateNoise.EnableWindow(turnOn);
	m_occludeBg.EnableWindow(turnOn);
	m_noiseRed.EnableWindow(turnOn);
	m_noiseGreen.EnableWindow(turnOn);
	m_noiseBlue.EnableWindow(turnOn);
	m_noisePercent.EnableWindow(turnOn);
	m_noiseSize.EnableWindow(turnOn);
	m_noiseStatic.EnableWindow(turnOn);
}

#endif // #ifdef EYE_TRACKING

