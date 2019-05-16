/**
 * @file AmblyoscopeDlg.cpp
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
#include "AmblyoscopeDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CAmblyoscopeDlg, CDialog)
CAmblyoscopeDlg::CAmblyoscopeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAmblyoscopeDlg::IDD, pParent)
{
	flip = false;
	lefton = true;
	righton = true;
}

CAmblyoscopeDlg::~CAmblyoscopeDlg()
{}

void CAmblyoscopeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XOffset, m_XOffset);
	DDX_Control(pDX, IDC_YOffset, m_YOffset);
	DDX_Control(pDX, IDC_LRed, m_LRed);
	DDX_Control(pDX, IDC_LGreen, m_LGreen);
	DDX_Control(pDX, IDC_LBlue, m_LBlue);
	DDX_Control(pDX, IDC_RRed, m_RRed);
	DDX_Control(pDX, IDC_RGreen, m_RGreen);
	DDX_Control(pDX, IDC_RBlue, m_RBlue);
	DDX_Control(pDX, IDC_XOffsetDeg, m_XOffsetDeg);
	DDX_Control(pDX, IDC_YOffsetDeg, m_YOffsetDeg);
	DDX_Control(pDX, IDC_Rlabel, m_Rlabel);
	DDX_Control(pDX, IDC_Llabel, m_Llabel);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_CHECK1, m_FlashTest);
	DDX_Control(pDX, IDC_PctMonocular, m_PctMonocular);
	DDX_Control(pDX, IDC_EDIT2, m_FlashDuration);
	DDX_Control(pDX, IDC_EDIT9, m_BlankDuration);
	DDX_Control(pDX, IDC_DeviatedRightX, m_DeviatedRightX);
	DDX_Control(pDX, IDC_DeviatedRightY, m_DeviatedRightY);
	DDX_Control(pDX, IDC_DeviatedLeftX, m_DeviatedLeftX);
	DDX_Control(pDX, IDC_DeviatedLeftY, m_DeviatedLeftY);
	DDX_Control(pDX, IDC_RADIO1, m_CenterRed);
	DDX_Control(pDX, IDC_RADIO2, m_CenterBlue);
	DDX_Control(pDX, IDC_RADIO4, m_CenterRandom);
	DDX_Control(pDX, IDC_RADIO5, m_CenterBlocks);
	DDX_Control(pDX, IDC_FlashBlocks, m_FlashBlocks);
	DDX_Control(pDX, IDC_ArmLength, m_ArmLength);
	DDX_Control(pDX, IDC_ArmWidth, m_ArmWidth);
	DDX_Control(pDX, IDC_SpaceSize, m_SpaceSize);
	DDX_Control(pDX, IDC_ArmStatic, m_ArmStatic);
	DDX_Control(pDX, IDC_FixationDot, m_FixationDot);
	DDX_Control(pDX, IDC_FixationX, m_FixationX);
	DDX_Control(pDX, IDC_FixationY, m_FixationY);
	DDX_Control(pDX, IDC_FixationSize, m_FixationSize);
	DDX_Control(pDX, IDC_CenterX, m_CenterX);
	DDX_Control(pDX, IDC_CenterY, m_CenterY);
	DDX_Control(pDX, IDC_BGBrightness, m_BGBrightness);
	DDX_Control(pDX, IDC_CHECK2, m_Alternate);
	DDX_Control(pDX, IDC_EyePositions, m_EyePositions);
	DDX_Control(pDX, IDC_Fovea, m_Fovea);
}

BEGIN_MESSAGE_MAP(CAmblyoscopeDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedButtonapply)
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONUP()
END_MESSAGE_MAP()

BOOL CAmblyoscopeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAmblyoscopeDlg::load() 
{
	m_XOffset.SetWindowText(etu::IntToString(g_pAmblyoscope->getXOffset()));
	m_YOffset.SetWindowText(etu::IntToString(g_pAmblyoscope->getYOffset()));
	if (g_pAmblyoscope->getPicture() == 0) {
		g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_Clock, BM_SETCHECK, BST_CHECKED, 0);
	} 
	else if (g_pAmblyoscope->getPicture() == 1) {
		g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_Bird, BM_SETCHECK, BST_CHECKED, 0);
	} 
	else if (g_pAmblyoscope->getPicture() == 2) {
		g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_Cross, BM_SETCHECK, BST_CHECKED, 0);
	} 
	else {
		g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_LeftRight, BM_SETCHECK, BST_CHECKED, 0);
	}
	m_LRed.SetWindowText(etu::IntToString(g_pAmblyoscope->getLRed()));
	m_LGreen.SetWindowText(etu::IntToString(g_pAmblyoscope->getLGreen()));
	m_LBlue.SetWindowText(etu::IntToString(g_pAmblyoscope->getLBlue()));
	m_RRed.SetWindowText(etu::IntToString(g_pAmblyoscope->getRRed()));
	m_RGreen.SetWindowText(etu::IntToString(g_pAmblyoscope->getRGreen()));
	m_RBlue.SetWindowText(etu::IntToString(g_pAmblyoscope->getRBlue()));
	m_Background.SetCheck((g_pAmblyoscope->getBackground())? BST_CHECKED : BST_UNCHECKED);
	m_FlashTest.SetCheck((g_pAmblyoscope->getFlashTest())? BST_CHECKED : BST_UNCHECKED);
	m_PctMonocular.SetWindowText(etu::IntToString(g_pAmblyoscope->getPctMonocular()));
	m_FlashDuration.SetWindowText(etu::IntToString(g_pAmblyoscope->getFlashDuration()));
	m_BlankDuration.SetWindowText(etu::IntToString(g_pAmblyoscope->getBlankDuration()));
	m_DeviatedRightX.SetWindowText(etu::FloatToString(g_pAmblyoscope->getDeviatedRightX()));
	m_DeviatedRightY.SetWindowText(etu::FloatToString(g_pAmblyoscope->getDeviatedRightY()));
	m_DeviatedLeftX.SetWindowText(etu::FloatToString(g_pAmblyoscope->getDeviatedLeftX()));
	m_DeviatedLeftY.SetWindowText(etu::FloatToString(g_pAmblyoscope->getDeviatedLeftY()));
	if (g_pAmblyoscope->getFlashCenter() == 0) {
		g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_RADIO1, BM_SETCHECK, BST_CHECKED, 0);//red
	} 
	else if (g_pAmblyoscope->getFlashCenter() == 1) {
		g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_RADIO2, BM_SETCHECK, BST_CHECKED, 0);//blue
	} 
	else if (g_pAmblyoscope->getFlashCenter() == 2){
		g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_RADIO4, BM_SETCHECK, BST_CHECKED, 0);//random
	} 
	else {
		g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_RADIO5, BM_SETCHECK, BST_CHECKED, 0);//random
	}
	m_FlashBlocks.SetWindowText(etu::IntToString(g_pAmblyoscope->getFlashBlocks()));
	m_ArmLength.SetWindowText(etu::IntToString(g_pAmblyoscope->getArmLength()));
	m_ArmWidth.SetWindowText(etu::IntToString(g_pAmblyoscope->getArmWidth()));
	m_SpaceSize.SetWindowText(etu::IntToString(g_pAmblyoscope->getSpaceSize()));
	m_FixationDot.SetCheck((g_pAmblyoscope->getFixationDot())? BST_CHECKED : BST_UNCHECKED);
	m_FixationX.SetWindowText(etu::FloatToString(g_pAmblyoscope->getFixationX()));
	m_FixationY.SetWindowText(etu::FloatToString(g_pAmblyoscope->getFixationY()));
	m_FixationSize.SetWindowText(etu::FloatToString(g_pAmblyoscope->getFixationSize()));
	m_CenterX.SetWindowText(etu::FloatToString(g_pAmblyoscope->getCenterX()));
	m_CenterY.SetWindowText(etu::FloatToString(g_pAmblyoscope->getCenterY()));
	m_BGBrightness.SetWindowText(etu::IntToString(g_pAmblyoscope->getBGBrightness()));
	m_Alternate.SetCheck((g_pAmblyoscope->getAlternate())? BST_CHECKED : BST_UNCHECKED);
}

void CAmblyoscopeDlg::save() 
{
	//char buffer[100];
	char buffer[1024];

	m_LRed.GetWindowText(buffer, 100);
	g_pAmblyoscope->setLRed(etu::StringToInt(buffer, g_pAmblyoscope->getLRed()));
	m_LGreen.GetWindowText(buffer, 100);
	g_pAmblyoscope->setLGreen(etu::StringToInt(buffer, g_pAmblyoscope->getLGreen()));
	m_LBlue.GetWindowText(buffer, 100);
	g_pAmblyoscope->setLBlue(etu::StringToInt(buffer, g_pAmblyoscope->getLBlue()));
	m_RRed.GetWindowText(buffer, 100);
	g_pAmblyoscope->setRRed(etu::StringToInt(buffer, g_pAmblyoscope->getRRed()));
	m_RGreen.GetWindowText(buffer, 100);
	g_pAmblyoscope->setRGreen(etu::StringToInt(buffer, g_pAmblyoscope->getRGreen()));
	m_RBlue.GetWindowText(buffer, 100);
	g_pAmblyoscope->setRBlue(etu::StringToInt(buffer, g_pAmblyoscope->getRBlue()));

	if (g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_Clock, BM_GETCHECK, 0, 0)==BST_CHECKED) {
		g_pAmblyoscope->setPicture(0);
	} 
	else if (g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_Bird, BM_GETCHECK, 0, 0)==BST_CHECKED) {
		g_pAmblyoscope->setPicture(1);
	} 
	else if (g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_Cross, BM_GETCHECK, 0, 0)==BST_CHECKED) {
		g_pAmblyoscope->setPicture(2);
	} 
	else {
		g_pAmblyoscope->setPicture(3);
	}
	g_pAmblyoscope->setBackground(m_Background.GetCheck()==BST_CHECKED);
	g_pAmblyoscope->setFlashTest(m_FlashTest.GetCheck()==BST_CHECKED);
	m_PctMonocular.GetWindowText(buffer, 100);
	g_pAmblyoscope->setPctMonocular(etu::StringToInt(buffer, g_pAmblyoscope->getPctMonocular()));
	m_FlashDuration.GetWindowText(buffer, 100);
	g_pAmblyoscope->setFlashDuration(etu::StringToInt(buffer, g_pAmblyoscope->getFlashDuration()));
	m_BlankDuration.GetWindowText(buffer, 100);
	g_pAmblyoscope->setBlankDuration(etu::StringToInt(buffer, g_pAmblyoscope->getBlankDuration()));
	m_DeviatedRightX.GetWindowText(buffer, 100);
	g_pAmblyoscope->setDeviatedRightX(etu::StringToFloat(buffer, g_pAmblyoscope->getDeviatedRightX()));
	m_DeviatedRightY.GetWindowText(buffer, 100);
	g_pAmblyoscope->setDeviatedRightY(etu::StringToFloat(buffer, g_pAmblyoscope->getDeviatedRightY()));
	m_DeviatedLeftX.GetWindowText(buffer, 100);
	g_pAmblyoscope->setDeviatedLeftX(etu::StringToFloat(buffer, g_pAmblyoscope->getDeviatedLeftX()));
	m_DeviatedLeftY.GetWindowText(buffer, 100);
	g_pAmblyoscope->setDeviatedLeftY(etu::StringToFloat(buffer, g_pAmblyoscope->getDeviatedLeftY()));

	if (g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_RADIO1, BM_GETCHECK, 0, 0)==BST_CHECKED) {
		g_pAmblyoscope->setFlashCenter(0);
	} 
	else if (g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_RADIO2, BM_GETCHECK, 0, 0)==BST_CHECKED) {
		g_pAmblyoscope->setFlashCenter(1);
	} 
	else if (g_pAmblyoscopeDlg->SendDlgItemMessage(IDC_RADIO4, BM_GETCHECK, 0, 0)==BST_CHECKED) {
		g_pAmblyoscope->setFlashCenter(2);
	} 
	else {
		g_pAmblyoscope->setFlashCenter(3);
	}
	m_FlashBlocks.GetWindowText(buffer, 100);
	g_pAmblyoscope->setFlashBlocks(etu::StringToInt(buffer, g_pAmblyoscope->getFlashBlocks()));
	m_ArmLength.GetWindowText(buffer, 100);
	g_pAmblyoscope->setArmLength(etu::StringToInt(buffer, g_pAmblyoscope->getArmLength()));
	m_ArmWidth.GetWindowText(buffer, 100);
	g_pAmblyoscope->setArmWidth(etu::StringToInt(buffer, g_pAmblyoscope->getArmWidth()));
	m_SpaceSize.GetWindowText(buffer, 100);
	g_pAmblyoscope->setSpaceSize(etu::StringToInt(buffer, g_pAmblyoscope->getSpaceSize()));
	g_pAmblyoscope->setFixationDot(m_FixationDot.GetCheck()==BST_CHECKED);
	m_FixationX.GetWindowText(buffer, 100);
	g_pAmblyoscope->setFixationX(etu::StringToFloat(buffer, g_pAmblyoscope->getFixationX()));
	m_FixationY.GetWindowText(buffer, 100);
	g_pAmblyoscope->setFixationY(etu::StringToFloat(buffer, g_pAmblyoscope->getFixationY()));
	m_FixationSize.GetWindowText(buffer, 100);
	g_pAmblyoscope->setFixationSize(etu::StringToFloat(buffer, g_pAmblyoscope->getFixationSize()));
	m_CenterX.GetWindowText(buffer, 100);
	g_pAmblyoscope->setCenterX(etu::StringToFloat(buffer, g_pAmblyoscope->getCenterX()));
	m_CenterY.GetWindowText(buffer, 100);
	g_pAmblyoscope->setCenterY(etu::StringToFloat(buffer, g_pAmblyoscope->getCenterY()));
	m_BGBrightness.GetWindowText(buffer, 100);
	g_pAmblyoscope->setBGBrightness(etu::StringToInt(buffer, g_pAmblyoscope->getBGBrightness()));
	g_pAmblyoscope->setAlternate(m_Alternate.GetCheck()==BST_CHECKED);

	//send textmark
	g_pAmblyoscope->sendSettings(g_pSettings->theSerialComm);
}

void CAmblyoscopeDlg::OnBnClickedButtonapply()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "Amblyoscope Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
}

void CAmblyoscopeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (g_pActiveStimulus  && g_pActiveStimulus->StimulusType() == "Amblyoscope Stimulus") {
		CRect area;
		this->GetClientRect(&area);

		g_pAmblyoscope->setXOffset( (point.x-area.Width()/2) );
		g_pAmblyoscope->setYOffset( (point.y-area.Height()/2) );

		m_XOffset.SetWindowText(etu::IntToString(g_pAmblyoscope->getXOffset()));
		m_YOffset.SetWindowText(etu::IntToString(-g_pAmblyoscope->getYOffset()));

		double xd, yd;
		vsgUnit2Unit(vsgPIXELUNIT, g_pAmblyoscope->getXOffset(), vsgDEGREEUNIT, &xd);
		vsgUnit2Unit(vsgPIXELUNIT, g_pAmblyoscope->getYOffset(), vsgDEGREEUNIT, &yd);

		m_XOffsetDeg.SetWindowText(etu::FloatToString((float)xd));
		m_YOffsetDeg.SetWindowText(etu::FloatToString((float)-yd));

	}

	CDialog::OnMouseMove(nFlags, point);
}

void CAmblyoscopeDlg::OnMButtonUp(UINT nFlags, CPoint point) 
{
	flip = !flip;
	m_Llabel.SetWindowText((flip)? "Center" : "Periph");
	m_Rlabel.SetWindowText((flip)? "Periph" : "Center");

	CDialog::OnMButtonUp(nFlags, point);
}

#endif