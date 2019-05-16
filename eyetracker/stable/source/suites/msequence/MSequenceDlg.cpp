/**
 * @file MSequenceDlg.cpp
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
#include "MSequenceDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CMSequenceDlg, CDialog)
CMSequenceDlg::CMSequenceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMSequenceDlg::IDD, pParent)
{}

CMSequenceDlg::~CMSequenceDlg()
{}

void CMSequenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_Width, m_Width);
	DDX_Control(pDX, IDC_Order, m_Order);
	DDX_Control(pDX, IDC_Delay, m_Delay);
	DDX_Control(pDX, IDC_Tap, m_Tap);
	DDX_Control(pDX, IDC_NumBars, m_NumBars);
	DDX_Control(pDX, IDC_Height, m_Height);
	DDX_Control(pDX, IDC_HCenter, m_HCenter);
	DDX_Control(pDX, IDC_VCenter, m_VCenter);
	DDX_Control(pDX, IDC_CurrentOffset, m_CurrentOffset);
	DDX_Control(pDX, IDC_Textmarks, m_Textmarks);
	DDX_Control(pDX, IDC_Order2, m_Order2);
	DDX_Control(pDX, IDC_Tap2, m_Tap2);
	DDX_Control(pDX, IDC_Order3, m_Order3);
	DDX_Control(pDX, IDC_Tap3, m_Tap3);
	DDX_Control(pDX, IDC_RedR, m_RedR);
	DDX_Control(pDX, IDC_RedG, m_RedG);
	DDX_Control(pDX, IDC_RedB, m_RedB);
	DDX_Control(pDX, IDC_GreenR, m_GreenR);
	DDX_Control(pDX, IDC_GreenG, m_GreenG);
	DDX_Control(pDX, IDC_GreenB, m_GreenB);
	DDX_Control(pDX, IDC_BlueR, m_BlueR);
	DDX_Control(pDX, IDC_BlueG, m_BlueG);
	DDX_Control(pDX, IDC_BlueB, m_BlueB);
}

BEGIN_MESSAGE_MAP(CMSequenceDlg, CDialog)
	ON_BN_CLICKED(IDC_ButtonApply, OnBnClickedButtonapply)
END_MESSAGE_MAP()

BOOL CMSequenceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMSequenceDlg::load() 
{
	m_Width.SetWindowText(etu::IntToString(g_pMSequence->getWidth()));
	m_Order.SetWindowText(etu::IntToString(g_pMSequence->getOrder()));
	m_Tap.SetWindowText(etu::IntToString(g_pMSequence->getTap()));
	m_Delay.SetWindowText(etu::IntToString(g_pMSequence->getDelay()));
	m_NumBars.SetWindowText(etu::IntToString(g_pMSequence->getNumBars()));

	m_Height.SetWindowText(etu::IntToString(g_pMSequence->getHeight()));
	m_HCenter.SetWindowText(etu::FloatToString((float)g_pMSequence->getHCenter()));
	m_VCenter.SetWindowText(etu::FloatToString((float)g_pMSequence->getVCenter()));
	
	if (g_pMSequence->getOrientation()==0) {
		g_pMSequenceDlg->SendDlgItemMessage(IDC_Horiz, BM_SETCHECK, BST_CHECKED, 0);
		g_pMSequenceDlg->SendDlgItemMessage(IDC_Vert, BM_SETCHECK, BST_UNCHECKED, 0);
		g_pMSequenceDlg->SendDlgItemMessage(IDC_ALTERNATE, BM_SETCHECK, BST_UNCHECKED, 0);
	} 
	else if (g_pMSequence->getOrientation()==1) {
		g_pMSequenceDlg->SendDlgItemMessage(IDC_Horiz, BM_SETCHECK, BST_UNCHECKED, 0);
		g_pMSequenceDlg->SendDlgItemMessage(IDC_Vert, BM_SETCHECK, BST_CHECKED, 0);
		g_pMSequenceDlg->SendDlgItemMessage(IDC_ALTERNATE, BM_SETCHECK, BST_UNCHECKED, 0);
	} 
	else {
		g_pMSequenceDlg->SendDlgItemMessage(IDC_Horiz, BM_SETCHECK, BST_UNCHECKED, 0);
		g_pMSequenceDlg->SendDlgItemMessage(IDC_Vert, BM_SETCHECK, BST_UNCHECKED, 0);
		g_pMSequenceDlg->SendDlgItemMessage(IDC_ALTERNATE, BM_SETCHECK, BST_CHECKED, 0);
	}
	m_Textmarks.SetCheck(g_pMSequence->getTextmarks()? BST_CHECKED : BST_UNCHECKED);
	
	m_CurrentOffset.SetWindowText("Current offset: ");
	m_Order2.SetWindowText(etu::IntToString(g_pMSequence->getOrder2()));
	m_Tap2.SetWindowText(etu::IntToString(g_pMSequence->getTap2()));
	m_Order3.SetWindowText(etu::IntToString(g_pMSequence->getOrder3()));
	m_Tap3.SetWindowText(etu::IntToString(g_pMSequence->getTap3()));

	m_RedR.SetWindowText(etu::IntToString(g_pMSequence->getRedR()));
	m_RedG.SetWindowText(etu::IntToString(g_pMSequence->getRedG()));
	m_RedB.SetWindowText(etu::IntToString(g_pMSequence->getRedB()));
	
	m_GreenR.SetWindowText(etu::IntToString(g_pMSequence->getGreenR()));
	m_GreenG.SetWindowText(etu::IntToString(g_pMSequence->getGreenG()));
	m_GreenB.SetWindowText(etu::IntToString(g_pMSequence->getGreenB()));

	m_BlueR.SetWindowText(etu::IntToString(g_pMSequence->getBlueR()));
	m_BlueG.SetWindowText(etu::IntToString(g_pMSequence->getBlueG()));
	m_BlueB.SetWindowText(etu::IntToString(g_pMSequence->getBlueB()));
}

void CMSequenceDlg::save() 
{
	char buffer[100];

	m_Width.GetWindowText(buffer, 100);
	g_pMSequence->setWidth(etu::StringToInt(buffer, g_pMSequence->getWidth()));

	m_Order.GetWindowText(buffer, 100);
	g_pMSequence->setOrder(etu::StringToInt(buffer, g_pMSequence->getOrder()));

	m_Tap.GetWindowText(buffer, 100);
	g_pMSequence->setTap(etu::StringToInt(buffer, g_pMSequence->getTap()));

	m_Delay.GetWindowText(buffer, 100);
	g_pMSequence->setDelay(etu::StringToInt(buffer, g_pMSequence->getDelay()));

	m_NumBars.GetWindowText(buffer, 100);
	g_pMSequence->setNumBars(etu::StringToInt(buffer, g_pMSequence->getNumBars()));

	m_Height.GetWindowText(buffer, 100);
	g_pMSequence->setHeight(etu::StringToInt(buffer, g_pMSequence->getHeight()));

	m_HCenter.GetWindowText(buffer, 100);
	g_pMSequence->setHCenter((double)etu::StringToFloat(buffer, (float)g_pMSequence->getHCenter()));
	
	m_VCenter.GetWindowText(buffer, 100);
	g_pMSequence->setVCenter((double)etu::StringToFloat(buffer, (float)g_pMSequence->getVCenter()));

	if (g_pMSequenceDlg->SendDlgItemMessage(IDC_Horiz, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		g_pMSequence->setOrientation(0);
	} 
	else if (g_pMSequenceDlg->SendDlgItemMessage(IDC_Vert, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		g_pMSequence->setOrientation(1);
	} 
	else {
		g_pMSequence->setOrientation(2);
	}

	g_pMSequence->setTextmarks(m_Textmarks.GetCheck()==BST_CHECKED);

	m_Order2.GetWindowText(buffer, 100);
	g_pMSequence->setOrder2(etu::StringToInt(buffer, g_pMSequence->getOrder2()));

	m_Tap2.GetWindowText(buffer, 100);
	g_pMSequence->setTap2(etu::StringToInt(buffer, g_pMSequence->getTap2()));

	m_Order3.GetWindowText(buffer, 100);
	g_pMSequence->setOrder3(etu::StringToInt(buffer, g_pMSequence->getOrder3()));

	m_Tap3.GetWindowText(buffer, 100);
	g_pMSequence->setTap3(etu::StringToInt(buffer, g_pMSequence->getTap3()));

	m_RedR.GetWindowText(buffer, 100);
	g_pMSequence->setRedR(etu::StringToInt(buffer, g_pMSequence->getRedR()));
	m_RedG.GetWindowText(buffer, 100);
	g_pMSequence->setRedG(etu::StringToInt(buffer, g_pMSequence->getRedB()));
	m_RedB.GetWindowText(buffer, 100);
	g_pMSequence->setRedB(etu::StringToInt(buffer, g_pMSequence->getRedB()));
	
	m_GreenR.GetWindowText(buffer, 100);
	g_pMSequence->setGreenR(etu::StringToInt(buffer, g_pMSequence->getGreenR()));
	m_GreenG.GetWindowText(buffer, 100);
	g_pMSequence->setGreenG(etu::StringToInt(buffer, g_pMSequence->getGreenG()));
	m_GreenB.GetWindowText(buffer, 100);
	g_pMSequence->setGreenB(etu::StringToInt(buffer, g_pMSequence->getGreenB()));
	
	m_BlueR.GetWindowText(buffer, 100);
	g_pMSequence->setBlueR(etu::StringToInt(buffer, g_pMSequence->getBlueR()));
	m_BlueG.GetWindowText(buffer, 100);
	g_pMSequence->setBlueG(etu::StringToInt(buffer, g_pMSequence->getBlueG()));
	m_BlueB.GetWindowText(buffer, 100);
	g_pMSequence->setBlueB(etu::StringToInt(buffer, g_pMSequence->getBlueB()));
}

void CMSequenceDlg::OnBnClickedButtonapply()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "MSequence Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
	
}

void CMSequenceDlg::OnBnClickedOk()
{	
	OnBnClickedButtonapply();
}

void CMSequenceDlg::OnBnClickedCancel()
{}


#endif
