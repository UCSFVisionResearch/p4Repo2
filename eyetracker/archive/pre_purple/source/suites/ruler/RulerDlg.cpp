/**
 * @file RulerDlg.cpp
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
#include "RulerDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CRulerDlg, CDialog)
CRulerDlg::CRulerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRulerDlg::IDD, pParent)
{}

CRulerDlg::~CRulerDlg()
{}

void CRulerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NumRings, m_NumRings);
	DDX_Control(pDX, IDC_Box, m_Box);
}

BEGIN_MESSAGE_MAP(CRulerDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedButtonapply)
END_MESSAGE_MAP()

BOOL CRulerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	load();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRulerDlg::load() 
{
	m_NumRings.SetWindowText(etu::IntToString(g_pRuler->getNumRings()));
	
	m_Box.SetCheck(g_pRuler->getBox());
}

void CRulerDlg::save() 
{
	char buffer[100];

	m_NumRings.GetWindowText(buffer, 100);
	g_pRuler->setNumRings(etu::StringToInt(buffer, g_pRuler->getNumRings()));

	g_pRuler->setBox(m_Box.GetCheck()==BST_CHECKED);
}

void CRulerDlg::OnBnClickedButtonapply()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "Ruler Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
}


#endif


