// CalibrationDlg.cpp : implementation file
//

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "FlashingDlg.h"



// CFlashingDlg dialog

IMPLEMENT_DYNAMIC(CFlashingDlg, CDialog)
CFlashingDlg::CFlashingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlashingDlg::IDD, pParent)
{
}

CFlashingDlg::~CFlashingDlg()
{
}

void CFlashingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EditSize, m_Size);
	DDX_Control(pDX, IDC_EditSeparation, m_Separation);

	DDX_Control(pDX, IDC_Check_ShowAll, m_ShowAll);
	DDX_Control(pDX, IDC_EYE_LIST, m_EyeList);
	DDX_Control(pDX, IDC_STATIC_DEBUG, m_Debug);
	DDX_Control(pDX, IDC_AcceptDelay, m_AcceptDelay);
	DDX_Control(pDX, IDC_CHECK_CYCLE, m_CheckCycle);
}


BEGIN_MESSAGE_MAP(CFlashingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_CYCLE, OnBnClickedCheckCycle)
	ON_BN_CLICKED(IDC_Check_ShowAll, OnBnClickedCheckShowall)
END_MESSAGE_MAP()


// CFlashingDlg message handlers
BOOL CFlashingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_EyeList.ResetContent();
	m_EyeList.AddString("Left");
	m_EyeList.AddString("Right");
	m_EyeList.AddString("Both");

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFlashingDlg::load() {
	m_Separation.SetWindowText(ETUtils::FloatToString(m_pCalibration->getSeparation()));
	m_Size.SetWindowText(ETUtils::FloatToString(m_pCalibration->getSize()));
	m_AcceptDelay.SetWindowText(ETUtils::IntToString(m_pCalibration->getAcceptDelay()));
	m_EyeList.SetCurSel(m_pCalibration->getEye());

	if (m_pCalibration->showAll())
		m_ShowAll.SetCheck(BST_CHECKED);
	else 
		m_ShowAll.SetCheck(BST_UNCHECKED);

	if (m_pCalibration->getCycle()) {
		m_CheckCycle.SetCheck(BST_CHECKED);
	} else {
		m_CheckCycle.SetCheck(BST_UNCHECKED);
	}
	
	
}

void CFlashingDlg::save() {
	char buffer[100];

	m_Separation.GetWindowText(buffer, 100);
	m_pCalibration->setSeparation(ETUtils::StringToFloat(buffer, m_pCalibration->getSeparation()));

	m_Size.GetWindowText(buffer, 100);
	m_pCalibration->setSize(ETUtils::StringToFloat(buffer, m_pCalibration->getSize()));

	m_AcceptDelay.GetWindowText(buffer, 100);
	m_pCalibration->setAcceptDelay(ETUtils::StringToInt(buffer, m_pCalibration->getAcceptDelay()));
	m_pCalibration->setEye(m_EyeList.GetCurSel());
	m_pCalibration->setShowAll(m_ShowAll.GetCheck() == BST_CHECKED);
	m_pCalibration->setCycle(m_CheckCycle.GetCheck() == BST_CHECKED);


	


}

void CFlashingDlg::OnBnClickedButton1()
{
	if (pActiveStimulus && pActiveStimulus->StimulusType() != "Calibration Stimulus") {
		pEyeTrackingDlg->OnBnClickedRun();
	}

	save();
}


void CFlashingDlg::setDebugString(CString str) {
	char buffer[256];
	sprintf(buffer, "%s", str);
	m_Debug.SetWindowText(buffer);
}

void CFlashingDlg::OnBnClickedOk()
{	
	OnBnClickedButton1();
}

void CFlashingDlg::OnBnClickedCancel()
{	
}


void CFlashingDlg::OnBnClickedCheckCycle()
{	
}

void CFlashingDlg::OnBnClickedCheckShowall()
{
	// TODO: Add your control notification handler code here
}
