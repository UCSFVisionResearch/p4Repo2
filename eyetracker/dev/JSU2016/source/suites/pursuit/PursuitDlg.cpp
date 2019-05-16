/**
 * @file PursuitDlg.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "PursuitDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Defines
 *****************************************************************************/
#define CALIBRATION_CHECK_AMPLITUDE_HORIZONTAL (30.0)
#define CALIBRATION_CHECK_AMPLITUDE_VERTICAL (15.0)
#define CALIBRATION_CHECK_FREQUENCY_HORIZONTAL (0.12)
#define CALIBRATION_CHECK_FREQUENCY_VERTICAL (0.17)

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CPursuitDlg, CDialog)
CPursuitDlg::CPursuitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPursuitDlg::IDD, pParent), m_oldAmplitude(-1.0), m_oldFrequency(-1.0)
{}

CPursuitDlg::~CPursuitDlg()
{}

void CPursuitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EditAmplitude, m_Amplitude);
	DDX_Control(pDX, IDC_EditFrequency, m_Frequency);
	DDX_Control(pDX, IDC_CalibrationCheckPursuit, m_CalibrationCheckPursuit);
	DDX_Control(pDX, IDC_EditSize, m_Size);
	DDX_Control(pDX, IDC_RadioHorizontal, m_RadioHorizontal);
	DDX_Control(pDX, IDC_RadioVertical, m_RadioVertical);
	DDX_Control(pDX, IDC_Radio45Deg, m_Radio45Deg);
	DDX_Control(pDX, IDC_Radio135Deg, m_Radio135Deg);
	DDX_Control(pDX, IDC_RadioSinewave, m_RadioSine);
	DDX_Control(pDX, IDC_RadioTriangle, m_RadioTriangle);

	DDX_Control(pDX, IDC_EnableHighFreq, m_EnableHighFreq);
	DDX_Control(pDX, IDC_HighFreqAmplitude, m_HighFreqAmplitude);
	DDX_Control(pDX, IDC_HighFreq, m_HighFreq);
	DDX_Control(pDX, IDC_UseString, m_useString);
	DDX_Control(pDX, IDC_TextString, m_TextString);
	DDX_Control(pDX, IDC_Eccentricity, m_Eccentricity);
	DDX_Control(pDX, IDC_ChangeR, m_ChangeR);
	DDX_Control(pDX, IDC_ChangeG, m_ChangeG);
	DDX_Control(pDX, IDC_ChangeB, m_ChangeB);
	DDX_Control(pDX, IDC_CHECK1, m_Alternating);
}

BEGIN_MESSAGE_MAP(CPursuitDlg, CDialog)
	ON_BN_CLICKED(IDC_CalibrationCheckPursuit, OnBnClickedCalibrationCheckPursuit)
	ON_BN_CLICKED(IDC_RadioHorizontal, OnBnClickedRadiohorizontal)
	ON_BN_CLICKED(IDC_Radio45Deg, OnBnClickedRadio45deg)
	ON_BN_CLICKED(IDC_RadioVertical, OnBnClickedRadiovertical)
	ON_BN_CLICKED(IDC_Radio135Deg, OnBnClickedRadio135deg)
	ON_BN_CLICKED(IDC_RadioSinewave, OnBnClickedRadiosinewave)
	ON_BN_CLICKED(IDC_RadioTriangle, OnBnClickedRadiotriangle)
	ON_BN_CLICKED(IDC_ButtonApply, OnBnClickedButtonapply)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_EnableHighFreq, OnBnClickedEnablehighfreq)
END_MESSAGE_MAP()

BOOL CPursuitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPursuitDlg::load() 
{
	m_Amplitude.SetWindowText(etu::FloatToString(g_pPursuit->getAmplitude()));
	m_Frequency.SetWindowText(etu::FloatToString(g_pPursuit->getFrequency()));
	m_Size.SetWindowText(etu::FloatToString(g_pPursuit->getSize()));
	m_HighFreq.SetWindowText(etu::FloatToString(g_pPursuit->getHighFrequency()));
	m_HighFreqAmplitude.SetWindowText(etu::FloatToString(g_pPursuit->getHighAmplitude()));

	switch (g_pPursuit->getOrientation() ) {
		case 1: OnBnClickedRadiohorizontal(); break;
		case 2: OnBnClickedRadiovertical(); break;
		case 3: OnBnClickedRadio45deg(); break;
		case 4: OnBnClickedRadio135deg(); break;
	}

	switch (g_pPursuit->getWaveform() ) {
		case 1: OnBnClickedRadiosinewave(); break;
		case 2: OnBnClickedRadiotriangle(); break;
	}


	if (g_pPursuit->isHighFreqEnabled()) {
		m_HighFreq.EnableWindow(true);
		m_HighFreqAmplitude.EnableWindow(true);
		m_EnableHighFreq.SetCheck(BST_CHECKED);
	} 
	else {
		m_HighFreq.EnableWindow(false);
		m_HighFreqAmplitude.EnableWindow(false);
		m_EnableHighFreq.SetCheck(BST_UNCHECKED);
	}
	
	m_useString.SetCheck( (g_pPursuit->getUseText())? BST_CHECKED : BST_UNCHECKED);

	char buffer[1024];
	strcpy(buffer, g_pPursuit->getTextString());
	m_TextString.SetWindowText(buffer);

	m_Eccentricity.SetWindowText(etu::FloatToString(g_pPursuit->getEccentricity()));
	m_ChangeR.SetWindowText(etu::IntToString(g_pPursuit->getChangeR()));
	m_ChangeG.SetWindowText(etu::IntToString(g_pPursuit->getChangeG()));
	m_ChangeB.SetWindowText(etu::IntToString(g_pPursuit->getChangeB()));

	m_Alternating.SetCheck( (g_pPursuit->getAlternating())? BST_CHECKED : BST_UNCHECKED);
}

void CPursuitDlg::save() 
{
	char buffer[1024];

	m_Amplitude.GetWindowText(buffer, 100);
	g_pPursuit->setAmplitude(etu::StringToFloat(buffer, g_pPursuit->getAmplitude()));

	m_Frequency.GetWindowText(buffer, 100);
	g_pPursuit->setFrequency(etu::StringToFloat(buffer, g_pPursuit->getFrequency()));

	m_Size.GetWindowText(buffer, 100);
	g_pPursuit->setSize(etu::StringToFloat(buffer, g_pPursuit->getSize()));

	m_HighFreqAmplitude.GetWindowText(buffer, 100);
	g_pPursuit->setHighAmplitude(etu::StringToFloat(buffer, g_pPursuit->getHighAmplitude()));

	m_HighFreq.GetWindowText(buffer, 100);
	g_pPursuit->setHighFrequency(etu::StringToFloat(buffer, g_pPursuit->getHighFrequency()));

	// Directions
	if (m_RadioHorizontal.GetCheck() == BST_CHECKED)  
		g_pPursuit->setOrientation(CPursuit::ORIENT_HORIZ);
	else if (m_RadioVertical.GetCheck() == BST_CHECKED)  
		g_pPursuit->setOrientation(CPursuit::ORIENT_VERT);
	else if (m_Radio45Deg.GetCheck() == BST_CHECKED)  
		g_pPursuit->setOrientation(CPursuit::ORIENT_45DEG);
	else 
		g_pPursuit->setOrientation(CPursuit::ORIENT_135DEG);

	if (m_RadioSine.GetCheck() == BST_CHECKED) {
		g_pPursuit->setWaveform(CPursuit::WAVEFORM_SINE);
	} 
	else {
		g_pPursuit->setWaveform(CPursuit::WAVEFORM_TRIANGLE);
	}
	
	g_pPursuit->setHighFreqEnabled(m_EnableHighFreq.GetCheck() == BST_CHECKED);
	
	g_pPursuit->setUseText(m_useString.GetCheck() == BST_CHECKED);
	m_TextString.GetWindowText(buffer, 1024);
	CString s;
	s.SetString(buffer, (int)strlen(buffer));
	g_pPursuit->setTextString(s);

	m_Eccentricity.GetWindowText(buffer, 100);
	g_pPursuit->setEccentricity(etu::StringToFloat(buffer, g_pPursuit->getEccentricity()));
	m_ChangeR.GetWindowText(buffer, 100);
	g_pPursuit->setChangeR(etu::StringToInt(buffer, g_pPursuit->getChangeR()));
	m_ChangeG.GetWindowText(buffer, 100);
	g_pPursuit->setChangeG(etu::StringToInt(buffer, g_pPursuit->getChangeG()));
	m_ChangeB.GetWindowText(buffer, 100);
	g_pPursuit->setChangeB(etu::StringToInt(buffer, g_pPursuit->getChangeB()));

	g_pPursuit->setAlternating(m_Alternating.GetCheck() == BST_CHECKED);
	
	g_pPursuit->sendSettings(g_pSettings->theSerialComm);
}

void CPursuitDlg::OnBnClickedCalibrationCheckPursuit()
{
	bool isOn = (m_CalibrationCheckPursuit.GetCheck() == BST_CHECKED);

	m_Amplitude.EnableWindow(!isOn);
	m_Frequency.EnableWindow(!isOn);
	m_Radio45Deg.EnableWindow(!isOn);
	m_Radio135Deg.EnableWindow(!isOn);

	if (isOn) {
		/* Save old settings if unsaved, and set to default values for 
		 * checking calibration. */
		if ((m_oldAmplitude < 0) && (m_oldFrequency < 0)) {
			/* Without this check, we'll overwrite the true originals with
			 * defaults for one orientation when switching to the other 
			 * orientation. */
			m_oldAmplitude = g_pPursuit->getAmplitude();
			m_oldFrequency = g_pPursuit->getFrequency();
		}

		/* Doesn't work with 45-deg or 135-deg. The invariant is maintained by
		 * the orientation radio buttons. */
		if (m_RadioHorizontal.GetCheck() == BST_CHECKED) {
			m_Amplitude.SetWindowText(etu::FloatToString(CALIBRATION_CHECK_AMPLITUDE_HORIZONTAL));
			m_Frequency.SetWindowText(etu::FloatToString(CALIBRATION_CHECK_FREQUENCY_HORIZONTAL));
		}
		else {
			m_Amplitude.SetWindowText(etu::FloatToString(CALIBRATION_CHECK_AMPLITUDE_VERTICAL));
			m_Frequency.SetWindowText(etu::FloatToString(CALIBRATION_CHECK_FREQUENCY_VERTICAL));
		}
	}
	else {
		/* If we have saved settings, restore them, then discard. */
		if ((m_oldAmplitude >= 0) && (m_oldFrequency >= 0)) {
			m_Amplitude.SetWindowText(etu::FloatToString(m_oldAmplitude));
			m_Frequency.SetWindowText(etu::FloatToString(m_oldFrequency));
			m_oldAmplitude = -1.0;
			m_oldFrequency = -1.0;
		}
	}
}

void CPursuitDlg::OnBnClickedRadiohorizontal()
{
	m_RadioHorizontal.SetCheck(BST_CHECKED);
	m_RadioVertical.SetCheck(BST_UNCHECKED);
	m_Radio45Deg.SetCheck(BST_UNCHECKED);
	m_Radio135Deg.SetCheck(BST_UNCHECKED);
	/* Ensure user can choose calibration check defaults. */
	m_CalibrationCheckPursuit.EnableWindow(true);
	/* If the user has chosen calibration check defaults, dynamically update
	 * them based on orientation. */
	OnBnClickedCalibrationCheckPursuit();
}

void CPursuitDlg::OnBnClickedRadio45deg()
{
	m_RadioHorizontal.SetCheck(BST_UNCHECKED);
	m_RadioVertical.SetCheck(BST_UNCHECKED);
	m_Radio45Deg.SetCheck(BST_CHECKED);
	m_Radio135Deg.SetCheck(BST_UNCHECKED);
	/* Calibration check defaults are not compatible with this orientation */
	m_CalibrationCheckPursuit.EnableWindow(false);
	/* Deselect the box and call its callback to return parameter control to
	 * user. */
	m_CalibrationCheckPursuit.SetCheck(BST_UNCHECKED);
	OnBnClickedCalibrationCheckPursuit();
}

void CPursuitDlg::OnBnClickedRadiovertical()
{
	m_RadioHorizontal.SetCheck(BST_UNCHECKED);
	m_RadioVertical.SetCheck(BST_CHECKED);
	m_Radio45Deg.SetCheck(BST_UNCHECKED);
	m_Radio135Deg.SetCheck(BST_UNCHECKED);
	/* Ensure user can choose calibration check defaults. */
	m_CalibrationCheckPursuit.EnableWindow(true);
	/* If the user has chosen calibration check defaults, dynamically update
	 * them based on orientation. */
	OnBnClickedCalibrationCheckPursuit();
}

void CPursuitDlg::OnBnClickedRadio135deg()
{
	m_RadioHorizontal.SetCheck(BST_UNCHECKED);
	m_RadioVertical.SetCheck(BST_UNCHECKED);
	m_Radio45Deg.SetCheck(BST_UNCHECKED);
	m_Radio135Deg.SetCheck(BST_CHECKED);
	/* Calibration check defaults are not compatible with this orientation */
	m_CalibrationCheckPursuit.EnableWindow(false);
	/* Deselect the box and call its callback to return parameter control to
	 * user. */
	m_CalibrationCheckPursuit.SetCheck(BST_UNCHECKED);
	OnBnClickedCalibrationCheckPursuit();
}

void CPursuitDlg::OnBnClickedRadiosinewave()
{
	m_RadioSine.SetCheck(BST_CHECKED);
	m_RadioTriangle.SetCheck(BST_UNCHECKED);
}

void CPursuitDlg::OnBnClickedRadiotriangle()
{
	m_RadioSine.SetCheck(BST_UNCHECKED);
	m_RadioTriangle.SetCheck(BST_CHECKED);
}

void CPursuitDlg::OnBnClickedButtonapply()
{	
	bool bWasActive = false;
	if (g_pActiveStimulus) {
		bWasActive = (g_pActiveStimulus->StimulusType() == "Pursuit Stimulus");;
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
	if (bWasActive) {
		while (g_pActiveStimulus)
			Sleep(100);
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
}

void CPursuitDlg::OnBnClickedOk()
{	
	OnBnClickedButtonapply();
}

void CPursuitDlg::OnBnClickedCancel()
{	}

void CPursuitDlg::OnBnClickedEnablehighfreq()
{
	if (m_EnableHighFreq.GetCheck() == BST_CHECKED) {
		m_HighFreq.EnableWindow(true);
		m_HighFreqAmplitude.EnableWindow(true);
	} 
	else {
		m_HighFreq.EnableWindow(false);
		m_HighFreqAmplitude.EnableWindow(false);
	}
}
