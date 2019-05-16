/**
 * @file IsoluminantPatchDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "IsoluminantPatchDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CIsoluminantPatchDlg, CDialog)
CIsoluminantPatchDlg::CIsoluminantPatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIsoluminantPatchDlg::IDD, pParent)
{}

CIsoluminantPatchDlg::~CIsoluminantPatchDlg()
{}

void CIsoluminantPatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FileName, m_FileName);
	DDX_Control(pDX, IDC_APPLY, m_Apply);
	DDX_Control(pDX, IDC_VoltageRange, m_VoltageRange);
	DDX_Control(pDX, IDC_VoltageOffset, m_VoltageOffset);
	DDX_Control(pDX, IDC_Browse, m_Browse);
	DDX_Control(pDX, IDC_HIGHEST_PRIORITY, m_HighestPriority);
	DDX_Control(pDX, IDC_RadioLeft, m_LeftHemiField);
	DDX_Control(pDX, IDC_RadioRight, m_RightHemiField);
	DDX_Control(pDX, IDC_FixationSize, m_FixationSize);
	DDX_Control(pDX, IDC_STIM_DATA, m_StimData);
	DDX_Control(pDX, IDC_FixationX, m_FixationX);
	DDX_Control(pDX, IDC_FixationY, m_FixationY);
	DDX_Control(pDX, IDC_MEAN_LUMINANCE, m_MeanLuminance);
	DDX_Control(pDX, IDC_NUM_REPEATS, m_NumRepeats);
	DDX_Control(pDX, IDC_RADIO_FNOISE, m_RadioFNoise);
	DDX_Control(pDX, IDC_RADIO_NON_FNOISE, m_RadioOther);
	DDX_Control(pDX, IDC_FixationX2, m_FixationX2);
	DDX_Control(pDX, IDC_FixationY2, m_FixationY2);
	DDX_Control(pDX, IDC_RepeatsOnly, m_RepeatsOnly);
	DDX_Control(pDX, IDC_MinLuminance, m_MinLuminance);
	DDX_Control(pDX, IDC_MaxLuminance, m_MaxLuminance);
}

BEGIN_MESSAGE_MAP(CIsoluminantPatchDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDC_Browse, OnBnClickedBrowse)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_RadioLeft, OnBnClickedRadioleft)
	ON_BN_CLICKED(IDC_RadioRight, OnBnClickedRadioright)
	ON_BN_CLICKED(IDC_RADIO_FNOISE, OnBnClickedRadioFnoise)
	ON_BN_CLICKED(IDC_RADIO_NON_FNOISE, OnBnClickedRadioNonFnoise)
END_MESSAGE_MAP()

BOOL CIsoluminantPatchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CIsoluminantPatchDlg::load() 
{
	m_FileName.SetWindowText(g_pIsoluminantPatch->GetFileName());
	m_VoltageRange.SetWindowText(etu::FloatToString(g_pIsoluminantPatch->GetVoltageRange()));
	m_VoltageOffset.SetWindowText(etu::FloatToString(g_pIsoluminantPatch->GetVoltageOffset()));
	m_FixationSize.SetWindowText(etu::FloatToString(g_pIsoluminantPatch->GetFixationSize()));
	m_FixationX.SetWindowText(etu::FloatToString(g_pIsoluminantPatch->GetFixationPointX()));
	m_FixationY.SetWindowText(etu::FloatToString(g_pIsoluminantPatch->GetFixationPointY()));
	m_MeanLuminance.SetWindowText(etu::IntToString(g_pIsoluminantPatch->GetMeanLuminance()));
	m_NumRepeats.SetWindowText(etu::IntToString(g_pIsoluminantPatch->GetNumRepeats()));
	m_FixationX2.SetWindowText(etu::FloatToString(g_pIsoluminantPatch->GetFixationPointX2()));
	m_FixationY2.SetWindowText(etu::FloatToString(g_pIsoluminantPatch->GetFixationPointY2()));
	m_MinLuminance.SetWindowText(etu::IntToString(g_pIsoluminantPatch->GetMinLuminance()));
	m_MaxLuminance.SetWindowText(etu::IntToString(g_pIsoluminantPatch->GetMaxLuminance()));

	if (g_pIsoluminantPatch->useHighestPriority()) {
		m_HighestPriority.SetCheck(BST_CHECKED);
	}

	if (g_pIsoluminantPatch->useLeftHemifield()) {
		OnBnClickedRadioleft();
	} 
	else {
		OnBnClickedRadioright();
	}

	if (g_pIsoluminantPatch->isFNoise()) {
		OnBnClickedRadioFnoise();
	} 
	else {
		OnBnClickedRadioFnoise();
	}

	if (g_pIsoluminantPatch->RepeatsOnly()) {
		m_RepeatsOnly.SetCheck(true);
	} 
	else {
		m_RepeatsOnly.SetCheck(false);
	}
}

void CIsoluminantPatchDlg::save()
{
	char buffer[100];

	m_FileName.GetWindowText(buffer, 100);
	g_pIsoluminantPatch->SetFileName(buffer);
	
	m_VoltageRange.GetWindowText(buffer, 100);
	g_pIsoluminantPatch->SetVoltageRange(etu::StringToFloat(buffer, g_pIsoluminantPatch->GetVoltageRange()));

	m_VoltageOffset.GetWindowText(buffer, 100);
	g_pIsoluminantPatch->SetVoltageOffset(etu::StringToFloat(buffer, g_pIsoluminantPatch->GetVoltageOffset()));
	
	m_FixationSize.GetWindowText(buffer, 100);
	g_pIsoluminantPatch->SetFixationSize(etu::StringToFloat(buffer, g_pIsoluminantPatch->GetFixationSize()));

	m_FixationX.GetWindowText(buffer, 100);
	g_pIsoluminantPatch->SetFixationPointX(etu::StringToFloat(buffer, g_pIsoluminantPatch->GetFixationPointX()));

	m_FixationY.GetWindowText(buffer, 100);
	g_pIsoluminantPatch->SetFixationPointY(etu::StringToFloat(buffer, g_pIsoluminantPatch->GetFixationPointY()));

	m_FixationX2.GetWindowText(buffer, 100);
	g_pIsoluminantPatch->SetFixationPointX2(etu::StringToFloat(buffer, g_pIsoluminantPatch->GetFixationPointX2()));

	m_FixationY2.GetWindowText(buffer, 100);
	g_pIsoluminantPatch->SetFixationPointY2(etu::StringToFloat(buffer, g_pIsoluminantPatch->GetFixationPointY2()));

	m_MeanLuminance.GetWindowText(buffer, 100);
	g_pIsoluminantPatch->SetMeanLuminance(etu::StringToInt(buffer, g_pIsoluminantPatch->GetMeanLuminance()));

	m_NumRepeats.GetWindowText(buffer, 100);
	g_pIsoluminantPatch->SetNumRepeats(etu::StringToInt(buffer, g_pIsoluminantPatch->GetNumRepeats()));

	m_MinLuminance.GetWindowText(buffer, 100);
	g_pIsoluminantPatch->SetMinLuminance(etu::StringToInt(buffer, g_pIsoluminantPatch->GetMinLuminance()));

	m_MaxLuminance.GetWindowText(buffer, 100);
	g_pIsoluminantPatch->SetMaxLuminance(etu::StringToInt(buffer, g_pIsoluminantPatch->GetMaxLuminance()));

	g_pIsoluminantPatch->setHighestPriority(m_HighestPriority.GetCheck() == BST_CHECKED);	
	g_pIsoluminantPatch->setLeftHemifield(m_LeftHemiField.GetCheck() == BST_CHECKED);
	g_pIsoluminantPatch->SetFNoise(m_RadioFNoise.GetCheck() == BST_CHECKED);
	g_pIsoluminantPatch->SetRepeatsOnly(m_RepeatsOnly.GetCheck() == BST_CHECKED);
	
}

void CIsoluminantPatchDlg::OnBnClickedOk()
{
	
	OnBnClickedApply();
}

void CIsoluminantPatchDlg::OnBnClickedCancel()
{}

void CIsoluminantPatchDlg::OnBnClickedApply()
{
	save();
}

void CIsoluminantPatchDlg::OnBnClickedBrowse()
{
	char buffer[200];
	m_FileName.GetWindowText(buffer, 200);
	CString filename = CString(buffer);
	filename = etu::BrowseForFile(true, "*.txt", "Text Files (*.txt)|*.txt", filename);
	
	sprintf(buffer, "%s", filename);
	m_FileName.SetWindowText(buffer);
}

void CIsoluminantPatchDlg::disableControls()
{
	m_Browse.EnableWindow(false);
	m_Apply.EnableWindow(false);
	m_FileName.EnableWindow(false);
	m_VoltageRange.EnableWindow(false);
	m_VoltageOffset.EnableWindow(false);
	m_HighestPriority.EnableWindow(false);
	m_FixationSize.EnableWindow(false);
	m_FixationX.EnableWindow(false);
	m_FixationY.EnableWindow(false);
	m_FixationX2.EnableWindow(false);
	m_FixationY2.EnableWindow(false);
	m_LeftHemiField.EnableWindow(false);
	m_RightHemiField.EnableWindow(false);
	m_RadioFNoise.EnableWindow(false);
	m_RadioOther.EnableWindow(false);
	m_NumRepeats.EnableWindow(false);
	m_RepeatsOnly.EnableWindow(false);
	m_MeanLuminance.EnableWindow(false);
	m_MinLuminance.EnableWindow(false);
	m_MaxLuminance.EnableWindow(false);

}

void CIsoluminantPatchDlg::enableControls() 
{
	m_Browse.EnableWindow(true);
	m_Apply.EnableWindow(true);
	m_FileName.EnableWindow(true);
	m_VoltageRange.EnableWindow(true);
	m_VoltageOffset.EnableWindow(true);
	m_HighestPriority.EnableWindow(true);
	m_FixationSize.EnableWindow(true);
	m_FixationX.EnableWindow(true);
	m_FixationY.EnableWindow(true);
	m_FixationX2.EnableWindow(true);
	m_FixationY2.EnableWindow(true);
	m_LeftHemiField.EnableWindow(true);
	m_RightHemiField.EnableWindow(true);
	m_RadioFNoise.EnableWindow(true);
	m_RadioOther.EnableWindow(true);
	m_NumRepeats.EnableWindow(true);
	m_RepeatsOnly.EnableWindow(true);
	m_MeanLuminance.EnableWindow(true);
	m_MinLuminance.EnableWindow(true);
	m_MaxLuminance.EnableWindow(true);
}

void CIsoluminantPatchDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);
}

void CIsoluminantPatchDlg::OnBnClickedRadioleft()
{
	m_LeftHemiField.SetCheck(BST_CHECKED);
	m_RightHemiField.SetCheck(BST_UNCHECKED);
}

void CIsoluminantPatchDlg::OnBnClickedRadioright()
{
	m_LeftHemiField.SetCheck(BST_UNCHECKED);
	m_RightHemiField.SetCheck(BST_CHECKED);
}

void CIsoluminantPatchDlg::UpdateStatus(int numRpts, int numUniques) {
	char buffer[100];
	if (g_pIsoluminantPatch->isFNoise()) {
		sprintf(buffer, "Repeated Sequences Completed:\t%d\n\nUnique Sequences Completed:\t%d", numRpts, numUniques);
	} 
	else {
		sprintf(buffer, "Completed %d of %d Repeats", numRpts, g_pIsoluminantPatch->GetNumRepeats());
	}
	m_StimData.SetWindowText(buffer);
}

void CIsoluminantPatchDlg::OnBnClickedRadioFnoise()
{
	m_RadioFNoise.SetCheck(BST_CHECKED);
	m_RadioOther.SetCheck(BST_UNCHECKED);
	m_RepeatsOnly.EnableWindow(true);
	
	m_NumRepeats.EnableWindow(false);
}

void CIsoluminantPatchDlg::OnBnClickedRadioNonFnoise()
{
	m_RadioFNoise.SetCheck(BST_UNCHECKED);
	m_RadioOther.SetCheck(BST_CHECKED);
	m_RepeatsOnly.EnableWindow(false);

	m_NumRepeats.EnableWindow(true);
}


#endif