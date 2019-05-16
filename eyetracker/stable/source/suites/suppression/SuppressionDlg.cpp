/**
 * @file SuppressionDlg.cpp
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
#include "SuppressionDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CSuppressionDlg, CDialog)
CSuppressionDlg::CSuppressionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSuppressionDlg::IDD, pParent)
{}

CSuppressionDlg::~CSuppressionDlg()
{}

void CSuppressionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GridSpace, m_GridSpace);
	DDX_Control(pDX, IDC_GridSize, m_GridSize);
	DDX_Control(pDX, IDC_Delay, m_Delay);
	DDX_Control(pDX, IDC_RedScroll, m_RedScroll);
	DDX_Control(pDX, IDC_BlueScroll, m_BlueScroll);
	DDX_Control(pDX, IDC_RedStatic, m_RedStatic);
	DDX_Control(pDX, IDC_BlueStatic, m_BlueStatic);
	DDX_Control(pDX, IDC_MMTest, m_MMTest);
	DDX_Control(pDX, IDC_FFTest, m_FFTest);
	DDX_Control(pDX, IDC_bgStripeLightRed, m_bgStripeLightRed);
	DDX_Control(pDX, IDC_bgStripeLightGreen, m_bgStripeLightGreen);
	DDX_Control(pDX, IDC_bgStripeLightBlue, m_bgStripeLightBlue);
	DDX_Control(pDX, IDC_bgStripeDarkRed, m_bgStripeDarkRed);
	DDX_Control(pDX, IDC_bgStripeDarkGreen, m_bgStripeDarkGreen);
	DDX_Control(pDX, IDC_bgStripeDarkBlue, m_bgStripeDarkBlue);
	DDX_Control(pDX, IDC_Delayms, m_Delayms);
	DDX_Control(pDX, IDC_RedBlock, m_RedBlock);
	DDX_Control(pDX, IDC_BlueBlock, m_BlueBlock);
	DDX_Control(pDX, IDC_TextValues, m_ShowText);
	DDX_Control(pDX, IDC_StripeHeight, m_StripeHeight);
	DDX_Control(pDX, IDC_AdjustBG, m_AdjustBG);
	DDX_Control(pDX, IDC_MMSineGratings, m_MMSineGratings);
	DDX_Control(pDX, IDC_DotTime, m_DotTime);
	DDX_Control(pDX, IDC_DisplayTime, m_DisplayTime);
	DDX_Control(pDX, IDC_OffTime, m_OffTime);
	DDX_Control(pDX, IDC_SpatialFreq, m_SpatialFreq);
	DDX_Control(pDX, IDC_RBUpDown, m_RBUpDown);
	DDX_Control(pDX, IDC_RBRandom, m_RBRandom);
	DDX_Control(pDX, IDC_RBSlider, m_RBSlider);
	DDX_Control(pDX, IDC_MMMG, m_MMMG);
	DDX_Control(pDX, IDC_NumGratings, m_NumGratings);
	DDX_Control(pDX, IDC_MultipleFullBlue, m_MultipleFullBlue);
}

BEGIN_MESSAGE_MAP(CSuppressionDlg, CDialog)
	ON_BN_CLICKED(IDC_ButtonApply, OnBnClickedButtonapply)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CSuppressionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	load();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSuppressionDlg::load() 
{
	m_GridSpace.SetWindowText(etu::IntToString(g_pSuppression->getGridSpace()));
	m_GridSize.SetWindowText(etu::IntToString(g_pSuppression->getGridSize()));

	m_RedScroll.SetRange(0,255);
	m_RedScroll.SetPos(g_pSuppression->getRedValue());
	m_RedStatic.SetWindowText(etu::IntToString(g_pSuppression->getRedValue()));

	m_BlueScroll.SetRange(0,255);
	m_BlueScroll.SetPos(g_pSuppression->getBlueValue());
	m_BlueStatic.SetWindowText(etu::IntToString(g_pSuppression->getBlueValue()));

	if (g_pSuppression->getTest()==1) {
		m_FFTest.SetCheck(BST_CHECKED);
		m_MMTest.SetCheck(BST_UNCHECKED);
		m_MMSineGratings.SetCheck(BST_UNCHECKED);
		m_MMMG.SetCheck(BST_UNCHECKED);
	} 
	else if (g_pSuppression->getTest()==0) {
		m_FFTest.SetCheck(BST_UNCHECKED);
		m_MMTest.SetCheck(BST_CHECKED);
		m_MMSineGratings.SetCheck(BST_UNCHECKED);
		m_MMMG.SetCheck(BST_UNCHECKED);
	} 
	else if (g_pSuppression->getTest()==2) {
		m_FFTest.SetCheck(BST_UNCHECKED);
		m_MMTest.SetCheck(BST_UNCHECKED);
		m_MMSineGratings.SetCheck(BST_CHECKED);
		m_MMMG.SetCheck(BST_UNCHECKED);
	} 
	else {
		m_FFTest.SetCheck(BST_UNCHECKED);
		m_MMTest.SetCheck(BST_UNCHECKED);
		m_MMSineGratings.SetCheck(BST_UNCHECKED);
		m_MMMG.SetCheck(BST_CHECKED);
	}

	m_bgStripeDarkRed.SetWindowText(etu::IntToString(g_pSuppression->getBgDarkRedValue()));
	m_bgStripeDarkGreen.SetWindowText(etu::IntToString(g_pSuppression->getBgDarkGreenValue()));
	m_bgStripeDarkBlue.SetWindowText(etu::IntToString(g_pSuppression->getBgDarkBlueValue()));

	m_bgStripeLightRed.SetWindowText(etu::IntToString(g_pSuppression->getBgLightRedValue()));
	m_bgStripeLightGreen.SetWindowText(etu::IntToString(g_pSuppression->getBgLightGreenValue()));
	m_bgStripeLightBlue.SetWindowText(etu::IntToString(g_pSuppression->getBgLightBlueValue()));
	
	m_Delayms.SetWindowText(etu::IntToString(g_pSuppression->getDelayms()));

	m_RedBlock.SetCheck(g_pSuppression->getRedBlock()? BST_CHECKED: BST_UNCHECKED);
	m_BlueBlock.SetCheck(g_pSuppression->getBlueBlock()? BST_CHECKED: BST_UNCHECKED);
	m_ShowText.SetCheck(g_pSuppression->getTextBlock()? BST_CHECKED: BST_UNCHECKED);

	m_StripeHeight.SetWindowText(etu::FloatToString((float)g_pSuppression->getStripeHeight()));

	m_AdjustBG.SetCheck(g_pSuppression->getAdjustBG()? BST_CHECKED: BST_UNCHECKED);
	m_DotTime.SetWindowText(etu::IntToString(g_pSuppression->getDotTime()));
	m_DisplayTime.SetWindowText(etu::IntToString(g_pSuppression->getDisplayTime()));
	m_OffTime.SetWindowText(etu::IntToString(g_pSuppression->getOffTime()));
	m_SpatialFreq.SetWindowText(etu::FloatToString((float)g_pSuppression->getSpatialFreq()));

	if (g_pSuppression->getSineColor()==0) {
		m_RBUpDown.SetCheck(BST_CHECKED);
		m_RBRandom.SetCheck(BST_UNCHECKED);
		m_RBSlider.SetCheck(BST_UNCHECKED);
	} 
	else if (g_pSuppression->getSineColor()==1) {
		m_RBUpDown.SetCheck(BST_UNCHECKED);
		m_RBRandom.SetCheck(BST_CHECKED);
		m_RBSlider.SetCheck(BST_UNCHECKED);
	} 
	else {
		m_RBUpDown.SetCheck(BST_UNCHECKED);
		m_RBRandom.SetCheck(BST_UNCHECKED);
		m_RBSlider.SetCheck(BST_CHECKED);
	}
	m_NumGratings.SetWindowText(etu::IntToString(g_pSuppression->getNumGratings()));
	m_MultipleFullBlue.SetCheck(g_pSuppression->getMultipleFullBlue()? BST_CHECKED: BST_UNCHECKED);
}

void CSuppressionDlg::save() 
{
	char buffer[1024];

	m_GridSpace.GetWindowText(buffer, 1024);
	g_pSuppression->setGridSpace(etu::StringToInt(buffer, g_pSuppression->getGridSpace()));

	m_GridSize.GetWindowText(buffer, 1024);
	g_pSuppression->setGridSize(etu::StringToInt(buffer, g_pSuppression->getGridSize()));

	g_pSuppression->setRedValue(m_RedScroll.GetPos());
	g_pSuppression->setBlueValue(m_BlueScroll.GetPos());

	if (m_FFTest.GetCheck()==BST_CHECKED) {
		g_pSuppression->setTest(1);
	} 
	else if (m_MMTest.GetCheck()==BST_CHECKED) {
		g_pSuppression->setTest(0);
	} 

	if (m_MMSineGratings.GetCheck()==BST_CHECKED) {
		g_pSuppression->setTest(2);
	} 
	else {
		g_pSuppression->setTest(3);
	}

	m_bgStripeDarkRed.GetWindowText(buffer, 1024);
	g_pSuppression->setBgDarkRedValue(etu::StringToInt(buffer, g_pSuppression->getBgDarkRedValue()));
	m_bgStripeDarkGreen.GetWindowText(buffer, 1024);
	g_pSuppression->setBgDarkGreenValue(etu::StringToInt(buffer, g_pSuppression->getBgDarkGreenValue()));
	m_bgStripeDarkBlue.GetWindowText(buffer, 1024);
	g_pSuppression->setBgDarkBlueValue(etu::StringToInt(buffer, g_pSuppression->getBgDarkBlueValue()));

	m_bgStripeLightRed.GetWindowText(buffer, 1024);
	g_pSuppression->setBgLightRedValue(etu::StringToInt(buffer, g_pSuppression->getBgLightRedValue()));
	m_bgStripeLightGreen.GetWindowText(buffer, 1024);
	g_pSuppression->setBgLightGreenValue(etu::StringToInt(buffer, g_pSuppression->getBgLightGreenValue()));
	m_bgStripeLightBlue.GetWindowText(buffer, 1024);
	g_pSuppression->setBgLightBlueValue(etu::StringToInt(buffer, g_pSuppression->getBgLightBlueValue()));

	m_Delayms.GetWindowText(buffer, 1024);
	g_pSuppression->setDelayms(etu::StringToInt(buffer, g_pSuppression->getDelayms()));

	g_pSuppression->setRedBlock(m_RedBlock.GetCheck()==BST_CHECKED);
	g_pSuppression->setBlueBlock(m_BlueBlock.GetCheck()==BST_CHECKED);
	g_pSuppression->setTextBlock(m_ShowText.GetCheck()==BST_CHECKED);

	m_StripeHeight.GetWindowText(buffer, 1024);
	g_pSuppression->setStripeHeight((double)etu::StringToFloat(buffer, (float)g_pSuppression->getStripeHeight()));

	g_pSuppression->setAdjustBG(m_AdjustBG.GetCheck()==BST_CHECKED);

	m_DotTime.GetWindowText(buffer, 1024);
	g_pSuppression->setDotTime(etu::StringToInt(buffer, g_pSuppression->getDotTime()));

	m_DisplayTime.GetWindowText(buffer, 1024);
	g_pSuppression->setDisplayTime(etu::StringToInt(buffer, g_pSuppression->getDisplayTime()));

	m_OffTime.GetWindowText(buffer, 1024);
	g_pSuppression->setOffTime(etu::StringToInt(buffer, g_pSuppression->getOffTime()));

	m_SpatialFreq.GetWindowText(buffer, 1024);
	g_pSuppression->setSpatialFreq((double)etu::StringToFloat(buffer, (float)g_pSuppression->getSpatialFreq()));
	if (m_RBUpDown.GetCheck()==BST_CHECKED) {
		g_pSuppression->setSineColor(0);
	} 
	else if (m_RBRandom.GetCheck()==BST_CHECKED) {
		g_pSuppression->setSineColor(1);
	} 
	else {
		g_pSuppression->setSineColor(2);
	}

	m_NumGratings.GetWindowText(buffer, 1024);
	g_pSuppression->setNumGratings(etu::StringToInt(buffer, g_pSuppression->getNumGratings()));
	g_pSuppression->setMultipleFullBlue(m_MultipleFullBlue.GetCheck()==BST_CHECKED);
}

void CSuppressionDlg::OnBnClickedButtonapply()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "Minimum Motion Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
	
	g_pSuppression->setDelay(1);
}

void CSuppressionDlg::OnBnClickedOk()
{	
	OnBnClickedButtonapply();
}

void CSuppressionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	pScrollBar->SetScrollPos(nPos);
	OnRedScroll();
	OnBlueScroll();
}

void CSuppressionDlg::OnRedScroll()
{	
	g_pSuppression->setRedValue(m_RedScroll.GetPos());
	g_pSuppression->setDelay(2);//2
	m_RedStatic.SetWindowText(etu::IntToString(g_pSuppression->getRedValue()));
}

void CSuppressionDlg::OnBlueScroll()
{	
	g_pSuppression->setBlueValue(m_BlueScroll.GetPos());
	g_pSuppression->setDelay(2);//2
	m_BlueStatic.SetWindowText(etu::IntToString(g_pSuppression->getBlueValue()));
}

void CSuppressionDlg::OnBnClickedCancel()
{}


#endif


