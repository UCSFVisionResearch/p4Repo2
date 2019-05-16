/**
 * @file SceneCalibrationDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "SceneCalibrationDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CSceneCalibrationDlg, CDialog)
CSceneCalibrationDlg::CSceneCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSceneCalibrationDlg::IDD, pParent)
{}

CSceneCalibrationDlg::~CSceneCalibrationDlg()
{}

void CSceneCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ImagePath, m_ImagePath);
	DDX_Control(pDX, IDC_XPosition, m_TargetXPosition);
	DDX_Control(pDX, IDC_YPosition, m_TargetYPosition);
	DDX_Control(pDX, IDOK, m_Apply);
	DDX_Control(pDX, IDC_SwapRedBlue, m_SwapRedBlue);
}

BEGIN_MESSAGE_MAP(CSceneCalibrationDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnBnClickedBrowse)
END_MESSAGE_MAP()

BOOL CSceneCalibrationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSceneCalibrationDlg::load() 
{
	m_ImagePath.SetWindowText(g_pSceneCalibration->getImagePath());
	m_TargetXPosition.SetWindowText(etu::FloatToString(g_pSceneCalibration->getXPosition()));
	m_TargetYPosition.SetWindowText(etu::FloatToString(g_pSceneCalibration->getYPosition()));

}

void CSceneCalibrationDlg::save() 
{
	char buffer[256];

	m_ImagePath.GetWindowText(buffer, 256);
	g_pSceneCalibration->setImagePath(CString(buffer));

	m_TargetXPosition.GetWindowText(buffer, 256);
	g_pSceneCalibration->setXPosition(etu::StringToFloat(buffer, 
		g_pSceneCalibration->getXPosition()));

	m_TargetYPosition.GetWindowText(buffer, 256);
	g_pSceneCalibration->setYPosition(etu::StringToFloat(buffer, 
		g_pSceneCalibration->getYPosition()));
}

void CSceneCalibrationDlg::OnBnClickedOk()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() == "Scene Calibration Stimulus") {
		((CSceneCalibrationStimulus*)g_pActiveStimulus)->setDrawingState(false);
	}
	save();
}

afx_msg void CSceneCalibrationDlg::OnBnClickedBrowse() {
	char buffer[1024];
	m_ImagePath.GetWindowText(buffer, 1024);
	CString filename = CString(buffer);
	filename = etu::BrowseForFile(true, "*.bmp", 
		"256 Color Bitmap Images (*.bmp)|*.bmp", filename);
	
	sprintf(buffer, "%s", filename);
	m_ImagePath.SetWindowText(buffer);
}


#endif