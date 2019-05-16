/**
 * @file GratingsDlg.cpp
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
#include "GratingsDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CGratingsDlg, CDialog)
CGratingsDlg::CGratingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGratingsDlg::IDD, pParent)
{}

CGratingsDlg::~CGratingsDlg()
{}

void CGratingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_xLocL, m_XLocL);
	DDX_Control(pDX, IDC_yLocL, m_YLocL);
	DDX_Control(pDX, IDC_xLocR, m_XLocR);
	DDX_Control(pDX, IDC_yLocR, m_YLocR);
	DDX_Control(pDX, IDC_DotSize, m_DotSize);
	DDX_Control(pDX, IDC_GratingWidth, m_GratingWidth);
	DDX_Control(pDX, IDC_PhaseStatic, m_PhaseStatic);
	DDX_Control(pDX, IDC_PhaseRandom, m_PhaseRandom);
	DDX_Control(pDX, IDC_PhaseRotate, m_PhaseRotate);
	DDX_Control(pDX, IDC_AngleStatic, m_AngleStatic);
	DDX_Control(pDX, IDC_AngleRandom, m_AngleRandom);
	DDX_Control(pDX, IDC_AngleRotate, m_AngleRotate);
	DDX_Control(pDX, IDC_TypeSquare, m_TypeSquare);
	DDX_Control(pDX, IDC_TypeSine, m_TypeSine);
	DDX_Control(pDX, IDC_TypeGabor, m_TypeGabor);
	DDX_Control(pDX, IDC_NumAngles, m_NumAngles);
	DDX_Control(pDX, IDC_GaborSD, m_GaborSD);
	DDX_Control(pDX, IDC_StaticPhase, m_StaticPhase);
	DDX_Control(pDX, IDC_StaticAngle, m_StaticAngle);
	DDX_Control(pDX, IDC_Delay, m_Delay);
	DDX_Control(pDX, IDC_Blank, m_Blank);
	DDX_Control(pDX, IDC_NumPhases, m_NumPhases);
	DDX_Control(pDX, IDC_NumRepeats, m_NumRepeats);
	DDX_Control(pDX, IDC_PresentationCounter, m_PresentationCounter);
	DDX_Control(pDX, IDC_UnitTag, m_UnitTag);
	DDX_Control(pDX, IDC_BEHAVIOR, m_Behavior);
	DDX_Control(pDX, IDC_Initiation, m_Initiation);
	DDX_Control(pDX, IDC_Reward, m_Reward);
	DDX_Control(pDX, IDC_CHECK1, m_UseSaccades);
	DDX_Control(pDX, IDC_EDIT5, m_HSteps);
	DDX_Control(pDX, IDC_EDIT6, m_VSteps);
	DDX_Control(pDX, IDC_EDIT7, m_StepSize);
	DDX_Control(pDX, IDC_GratingsPerSaccade, m_GratingsPerSaccade);
	DDX_Control(pDX, IDC_LightPic, m_LightPic);
	DDX_Control(pDX, IDC_DarkPic, m_DarkPic);
	DDX_Control(pDX, IDC_LightText, m_LightText);
	DDX_Control(pDX, IDC_DarkText, m_DarkText);
	DDX_Control(pDX, IDC_ShutterManual, m_ShutterManual);
	DDX_Control(pDX, IDC_ShutterInterleaved, m_ShutterInterleaved);
	DDX_Control(pDX, IDC_ShutterRandom, m_ShutterRandom);
	DDX_Control(pDX, IDC_ShutterInterleavedN, m_ShutterInterleavedN);
	DDX_Control(pDX, IDC_ShutterDelay, m_ShutterDelay);
	DDX_Control(pDX, IDC_RFX, m_RFX);
	DDX_Control(pDX, IDC_RFY, m_RFY);
	DDX_Control(pDX, IDC_grLeftEyeAutoRF, m_LeftEyeAutoRF);
	DDX_Control(pDX, IDC_grRightEyeAutoRF, m_RightEyeAutoRF);
	DDX_Control(pDX, IDC_DevX, m_DevX);
	DDX_Control(pDX, IDC_DevY, m_DevY);
	DDX_Control(pDX, IDC_Speed, m_Speed);
	DDX_Control(pDX, IDC_FinalSize, m_FinalSize);
	DDX_Control(pDX, IDC_InitialSize, m_InitialSize);
	DDX_Control(pDX, IDC_RFL, m_RFL);
	DDX_Control(pDX, IDC_RFR, m_RFR);
	DDX_Control(pDX, IDC_TypeAutoRFMap, m_TypeAutoRFMap);
	DDX_Control(pDX, IDC_RFMapR, m_RFMapR);
	DDX_Control(pDX, IDC_RFMapL, m_RFMapL);

	DDX_Control(pDX, IDC_grEyePosnDisplayLX, m_EyePosnDisplayLX);
	DDX_Control(pDX, IDC_grEyePosnDisplayLY, m_EyePosnDisplayLY);
	DDX_Control(pDX, IDC_grEyePosnDisplayRX, m_EyePosnDisplayRX);
	DDX_Control(pDX, IDC_grEyePosnDisplayRY, m_EyePosnDisplayRY);
}

BEGIN_MESSAGE_MAP(CGratingsDlg, CDialog)
	ON_BN_CLICKED(IDC_ButtonApply, OnBnClickedButtonapply)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_grLeftEyeAutoRF, OnBnClickedLeftEyeAutoRF)
	ON_BN_CLICKED(IDC_grRightEyeAutoRF, OnBnClickedRightEyeAutoRF)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CGratingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGratingsDlg::load()
{
	//m_XLoc.SetWindowText(etu::FloatToString(g_pGratings->getX()));
	//m_YLoc.SetWindowText(etu::FloatToString(g_pGratings->getY()));
	m_XLocL.SetWindowText(etu::FloatToString(g_pGratings->getXLeft()));
	m_YLocL.SetWindowText(etu::FloatToString(g_pGratings->getYLeft()));
	m_XLocR.SetWindowText(etu::FloatToString(g_pGratings->getXRight()));
	m_YLocR.SetWindowText(etu::FloatToString(g_pGratings->getYRight()));
	m_DotSize.SetWindowText(etu::FloatToString(g_pGratings->getDotSize()));
	m_GratingWidth.SetWindowText(etu::FloatToString(g_pGratings->getGratingWidth()));
	m_NumAngles.SetWindowText(etu::IntToString(g_pGratings->getNumAngles()));
	
	if (g_pGratings->getPhase()==CGratings::TYPE_RANDOM) {
		m_PhaseStatic.SetCheck(BST_UNCHECKED);
		m_PhaseRandom.SetCheck(BST_CHECKED);
		m_PhaseRotate.SetCheck(BST_UNCHECKED);
	} 
	else if (g_pGratings->getPhase()==CGratings::TYPE_STATIC) {
		m_PhaseStatic.SetCheck(BST_CHECKED);
		m_PhaseRandom.SetCheck(BST_UNCHECKED);
		m_PhaseRotate.SetCheck(BST_UNCHECKED);
	} 
	else {
		m_PhaseStatic.SetCheck(BST_UNCHECKED);
		m_PhaseRandom.SetCheck(BST_UNCHECKED);
		m_PhaseRotate.SetCheck(BST_CHECKED);
	}
	
	if (g_pGratings->getAngle()==CGratings::TYPE_RANDOM) {
		m_AngleStatic.SetCheck(BST_UNCHECKED);
		m_AngleRandom.SetCheck(BST_CHECKED);
		m_AngleRotate.SetCheck(BST_UNCHECKED);
	} 
	else if (g_pGratings->getAngle()==CGratings::TYPE_STATIC) {
		m_AngleStatic.SetCheck(BST_CHECKED);
		m_AngleRandom.SetCheck(BST_UNCHECKED);
		m_AngleRotate.SetCheck(BST_UNCHECKED);
	} 
	else {
		m_AngleStatic.SetCheck(BST_UNCHECKED);
		m_AngleRandom.SetCheck(BST_UNCHECKED);
		m_AngleRotate.SetCheck(BST_CHECKED);
	}

	if (g_pGratings->getType()==CGratings::STYLE_SQUARE) {
		m_TypeSquare.SetCheck(BST_CHECKED);
		m_TypeSine.SetCheck(BST_UNCHECKED);
		m_TypeGabor.SetCheck(BST_UNCHECKED);
		m_TypeAutoRFMap.SetCheck(BST_UNCHECKED);
	} 
	else if (g_pGratings->getType()==CGratings::STYLE_SINE) {
		m_TypeSquare.SetCheck(BST_UNCHECKED);
		m_TypeSine.SetCheck(BST_CHECKED);
		m_TypeGabor.SetCheck(BST_UNCHECKED);
		m_TypeAutoRFMap.SetCheck(BST_UNCHECKED);
	} 
	else if (g_pGratings->getType()==CGratings::STYLE_GABOR) {
		m_TypeSquare.SetCheck(BST_UNCHECKED);
		m_TypeSine.SetCheck(BST_UNCHECKED);
		m_TypeGabor.SetCheck(BST_CHECKED);
		m_TypeAutoRFMap.SetCheck(BST_UNCHECKED);
	} 
	else {
		m_TypeSquare.SetCheck(BST_UNCHECKED);
		m_TypeSine.SetCheck(BST_UNCHECKED);
		m_TypeGabor.SetCheck(BST_UNCHECKED);
		m_TypeAutoRFMap.SetCheck(BST_CHECKED);
	}

	m_GaborSD.SetWindowText(etu::FloatToString(g_pGratings->getGaborSD()));
	m_StaticPhase.SetWindowText(etu::FloatToString(g_pGratings->getStaticPhase()));
	m_StaticAngle.SetWindowText(etu::FloatToString(g_pGratings->getStaticAngle()));
	m_Delay.SetWindowText(etu::IntToString(g_pGratings->getDelay()));
	m_Blank.SetWindowText(etu::IntToString(g_pGratings->getBlank()));
	m_NumPhases.SetWindowText(etu::IntToString(g_pGratings->getNumPhases()));

	g_pGratingsDlg->lightRGB = RGB(g_pGratings->getGratingRed(), g_pGratings->getGratingGreen(), g_pGratings->getGratingBlue());

	g_pGratingsDlg->darkRGB = RGB(g_pGratings->getGratingRed2(), g_pGratings->getGratingGreen2(), g_pGratings->getGratingBlue2());

	m_NumRepeats.SetWindowText(etu::IntToString(g_pGratings->getNumRepeats()));

	m_UnitTag.SetWindowText(g_pGratings->getUnitTag());
	m_Behavior.SetCheck((g_pGratings->getBehavior())? BST_CHECKED : BST_UNCHECKED);
	m_Initiation.SetWindowText(etu::IntToString(g_pGratings->getInitiationDelay()));
	m_Reward.SetWindowText(etu::IntToString(g_pGratings->getRewardDelay()));
	m_HSteps.SetWindowText(etu::IntToString(g_pGratings->getHSteps()));
	m_VSteps.SetWindowText(etu::IntToString(g_pGratings->getVSteps()));
	m_StepSize.SetWindowText(etu::FloatToString(g_pGratings->getStepSize()));
	m_UseSaccades.SetCheck((g_pGratings->getUseSaccades())? BST_CHECKED : BST_UNCHECKED);
	m_GratingsPerSaccade.SetWindowText(etu::IntToString(g_pGratings->getGratingsPerSaccade()));
	
	if (g_pGratings->getShutters()==CGratings::SHUTTER_MANUAL) {
		m_ShutterManual.SetCheck(BST_CHECKED);
		m_ShutterInterleaved.SetCheck(BST_UNCHECKED);
		m_ShutterRandom.SetCheck(BST_UNCHECKED);
	} 
	else if (g_pGratings->getShutters()==CGratings::SHUTTER_INTERLEAVED) {
		m_ShutterManual.SetCheck(BST_UNCHECKED);
		m_ShutterInterleaved.SetCheck(BST_CHECKED);
		m_ShutterRandom.SetCheck(BST_UNCHECKED);
	} 
	else {
		m_ShutterManual.SetCheck(BST_UNCHECKED);
		m_ShutterInterleaved.SetCheck(BST_UNCHECKED);
		m_ShutterRandom.SetCheck(BST_CHECKED);
	}
	m_ShutterInterleavedN.SetWindowText(etu::IntToString(g_pGratings->getShutterInterleavedN()));

	m_ShutterDelay.SetWindowText(etu::IntToString(g_pGratings->getShutterDelay()));
	m_RFX.SetWindowText(etu::FloatToString(g_pGratings->getRFX()));
	m_RFY.SetWindowText(etu::FloatToString(g_pGratings->getRFY()));
	m_DevX.SetWindowText(etu::FloatToString(g_pGratings->getDevX()));
	m_DevY.SetWindowText(etu::FloatToString(g_pGratings->getDevY()));
	m_Speed.SetWindowText(etu::FloatToString(g_pGratings->getSpeed()));
	m_FinalSize.SetWindowText(etu::FloatToString(g_pGratings->getFinalSize()));
	m_InitialSize.SetWindowText(etu::FloatToString(g_pGratings->getInitialSize()));

	if (g_pGratings->getRFEye()==0) {
		m_RFMapL.SetCheck(BST_UNCHECKED);
		m_RFMapR.SetCheck(BST_CHECKED);
	} 
	else {
		m_RFMapL.SetCheck(BST_CHECKED);
		m_RFMapR.SetCheck(BST_UNCHECKED);
	}

	m_RFL.SetWindowText("RF Result");
	m_RFR.SetWindowText("RF Result");
}

void CGratingsDlg::save() 
{
	char buffer[1024];
	int affected = 0;

	m_XLocL.GetWindowText(buffer, 1024);
	g_pGratings->setXLeft(etu::StringToFloat(buffer, g_pGratings->getXLeft()));
	m_YLocL.GetWindowText(buffer, 1024);
	g_pGratings->setYLeft(etu::StringToFloat(buffer, g_pGratings->getYLeft()));
	m_XLocR.GetWindowText(buffer, 1024);
	g_pGratings->setXRight(etu::StringToFloat(buffer, g_pGratings->getXRight()));
	m_YLocR.GetWindowText(buffer, 1024);
	g_pGratings->setYRight(etu::StringToFloat(buffer, g_pGratings->getYRight()));

	m_DotSize.GetWindowText(buffer, 1024);
	g_pGratings->setDotSize(etu::StringToFloat(buffer, g_pGratings->getDotSize()));

	m_GratingWidth.GetWindowText(buffer, 1024);
	g_pGratings->setGratingWidth(etu::StringToFloat(buffer, g_pGratings->getGratingWidth()));

	m_NumAngles.GetWindowText(buffer, 1024);
	affected += g_pGratings->setNumAngles(etu::StringToInt(buffer, g_pGratings->getNumAngles()));

	if (m_TypeSine.GetCheck()==BST_CHECKED) {
		affected += g_pGratings->setType(CGratings::STYLE_SINE);
	} 
	else if (m_TypeSquare.GetCheck()==BST_CHECKED) {
		affected += g_pGratings->setType(CGratings::STYLE_SQUARE);
	} 
	else if (m_TypeGabor.GetCheck()==BST_CHECKED) {
		affected += g_pGratings->setType(CGratings::STYLE_GABOR);
	} 
	else {
		affected += g_pGratings->setType(CGratings::STYLE_RFMAP);
	}
	
	if (m_AngleStatic.GetCheck()==BST_CHECKED) {
		affected += g_pGratings->setAngle(CGratings::TYPE_STATIC);
	} 
	else if (m_AngleRandom.GetCheck()==BST_CHECKED) {
		affected += g_pGratings->setAngle(CGratings::TYPE_RANDOM);
	} 
	else {
		affected += g_pGratings->setAngle(CGratings::TYPE_ROTATE);
	}
	
	if (m_PhaseStatic.GetCheck()==BST_CHECKED) {
		affected += g_pGratings->setPhase(CGratings::TYPE_STATIC);
	} 
	else if (m_PhaseRandom.GetCheck()==BST_CHECKED) {
		affected += g_pGratings->setPhase(CGratings::TYPE_RANDOM);
	} 
	else {
		affected += g_pGratings->setPhase(CGratings::TYPE_ROTATE);
	}

	m_GaborSD.GetWindowText(buffer, 1024);
	g_pGratings->setGaborSD(etu::StringToFloat(buffer, g_pGratings->getGaborSD()));

	m_StaticPhase.GetWindowText(buffer, 1024);
	affected += g_pGratings->setStaticPhase(etu::StringToFloat(buffer, g_pGratings->getStaticPhase()));

	m_StaticAngle.GetWindowText(buffer, 1024);
	affected += g_pGratings->setStaticAngle(etu::StringToFloat(buffer, g_pGratings->getStaticAngle()));

	m_Delay.GetWindowText(buffer, 1024);
	g_pGratings->setDelay(etu::StringToInt(buffer, g_pGratings->getDelay()));

	m_Blank.GetWindowText(buffer, 1024);
	g_pGratings->setBlank(etu::StringToInt(buffer, g_pGratings->getBlank()));

	m_NumPhases.GetWindowText(buffer, 1024);
	affected += g_pGratings->setNumPhases(etu::StringToInt(buffer, g_pGratings->getNumPhases()));

	g_pGratings->setGratingRed(GetRValue(g_pGratingsDlg->lightRGB));
	g_pGratings->setGratingGreen(GetGValue(g_pGratingsDlg->lightRGB));
	g_pGratings->setGratingBlue(GetBValue(g_pGratingsDlg->lightRGB));

	g_pGratings->setGratingRed2(GetRValue(g_pGratingsDlg->darkRGB));
	g_pGratings->setGratingGreen2(GetGValue(g_pGratingsDlg->darkRGB));
	g_pGratings->setGratingBlue2(GetBValue(g_pGratingsDlg->darkRGB));

	m_NumRepeats.GetWindowText(buffer, 1024);
	affected += g_pGratings->setNumRepeats(etu::StringToInt(buffer, g_pGratings->getNumRepeats()));
	
	m_UnitTag.GetWindowText(buffer, 1024);
	g_pGratings->setUnitTag(buffer);

	g_pGratings->setBehavior(m_Behavior.GetCheck()==BST_CHECKED);

	m_Initiation.GetWindowText(buffer, 1024);
	g_pGratings->setInitiationDelay(etu::StringToInt(buffer, g_pGratings->getInitiationDelay()));

	m_Reward.GetWindowText(buffer, 1024);
	g_pGratings->setRewardDelay(etu::StringToInt(buffer, g_pGratings->getRewardDelay()));

	m_HSteps.GetWindowText(buffer, 1024);
	g_pGratings->setHSteps(etu::StringToInt(buffer, g_pGratings->getHSteps()));

	m_VSteps.GetWindowText(buffer, 1024);
	g_pGratings->setVSteps(etu::StringToInt(buffer, g_pGratings->getVSteps()));

	m_StepSize.GetWindowText(buffer, 1024);
	g_pGratings->setStepSize(etu::StringToFloat(buffer, g_pGratings->getStepSize()));

	g_pGratings->setUseSaccades(m_UseSaccades.GetCheck()==BST_CHECKED);

	m_GratingsPerSaccade.GetWindowText(buffer, 1024);
	g_pGratings->setGratingsPerSaccade(etu::StringToInt(buffer, g_pGratings->getGratingsPerSaccade()));

	if (m_ShutterInterleaved.GetCheck()==BST_CHECKED) {
		affected += g_pGratings->setShutters(CGratings::SHUTTER_INTERLEAVED);
	} else {
		affected += g_pGratings->setShutters(CGratings::SHUTTER_RANDOM);
	}
	m_ShutterInterleavedN.GetWindowText(buffer, 1024);
	affected += g_pGratings->setShutterInterleavedN(etu::StringToInt(buffer, g_pGratings->getShutterInterleavedN()));

	m_ShutterDelay.GetWindowText(buffer, 1024);
	g_pGratings->setShutterDelay(etu::StringToInt(buffer, g_pGratings->getShutterDelay()));

	m_RFX.GetWindowText(buffer, 1024);
	affected += g_pGratings->setRFX(etu::StringToFloat(buffer, g_pGratings->getRFX()));
	m_RFY.GetWindowText(buffer, 1024);
	affected += g_pGratings->setRFY(etu::StringToFloat(buffer, g_pGratings->getRFY()));
	m_DevX.GetWindowText(buffer, 1024);
	affected += g_pGratings->setDevX(etu::StringToFloat(buffer, g_pGratings->getDevX()));
	m_DevY.GetWindowText(buffer, 1024);
	affected += g_pGratings->setDevY(etu::StringToFloat(buffer, g_pGratings->getDevX()));
	m_Speed.GetWindowText(buffer, 1024);
	affected += g_pGratings->setSpeed(etu::StringToFloat(buffer, g_pGratings->getSpeed()));
	m_FinalSize.GetWindowText(buffer, 1024);
	affected += g_pGratings->setFinalSize(etu::StringToFloat(buffer, g_pGratings->getFinalSize()));
	m_InitialSize.GetWindowText(buffer, 1024);
	affected += g_pGratings->setInitialSize(etu::StringToFloat(buffer, g_pGratings->getInitialSize()));

	if (m_RFMapL.GetCheck()==BST_CHECKED) {
		affected += g_pGratings->setRFEye(0);
	} 
	else {
		affected += g_pGratings->setRFEye(1);
	}

	//todo regenerate trial list and reset counter
	if (affected != 0) {
		if (g_pGratings->getType() == CGratings::STYLE_RFMAP) {
			g_pGratings->GenerateRFMappingTrials();
		} else {
			g_pGratings->GenerateTrials();//new trial class paradigm instead of old arrays
		}
	}

	//send textmark
	g_pGratings->sendSettings(g_pSettings->theSerialComm);
}

/**
 * @brief Updates dialog's display of eye positions.
 * @param lxDeg Left eye X position in degrees.
 * @param lyDeg Left eye Y position in degrees.
 * @param rxDeg Right eye X position in degrees.
 * @param ryDeg Right eye Y position in degrees.
 */
void CGratingsDlg::UpdateEyePositions(double lxDeg, double lyDeg,
									  double rxDeg, double ryDeg)
{
	char buffer[16];
	
	sprintf(buffer, "%.2f", lxDeg);
	m_EyePosnDisplayLX.SetWindowText(buffer);
	
	sprintf(buffer, "%.2f", lyDeg);
	m_EyePosnDisplayLY.SetWindowText(buffer);
	
	sprintf(buffer, "%.2f", rxDeg);
	m_EyePosnDisplayRX.SetWindowText(buffer);
	
	sprintf(buffer, "%.2f", ryDeg);
	m_EyePosnDisplayRY.SetWindowText(buffer);
}

void CGratingsDlg::OnBnClickedButtonapply()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "Gratings Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
}

void CGratingsDlg::OnBnClickedOk() 
{	
	OnBnClickedButtonapply();
}

void CGratingsDlg::OnBnClickedButton1()
{
	// Light color selector
	CHOOSECOLOR cdb;
	ZeroMemory(&cdb, sizeof(cdb));
	cdb.hwndOwner = g_pEyeTrackingDlg->GetSafeHwnd();//HWND()
	cdb.lStructSize = sizeof(cdb);
	cdb.lpCustColors = (LPDWORD) g_pEyeTrackingDlg->acrCustClr;
	cdb.rgbResult = g_pGratingsDlg->lightRGB;
	cdb.Flags = CC_SOLIDCOLOR | CC_RGBINIT ;//| CC_FULLOPEN;
	if (ChooseColor(&cdb)) {
		g_pGratingsDlg->lightRGB = cdb.rgbResult;
		g_pEyeTrackingDlg->Invalidate();
	}
}

void CGratingsDlg::OnBnClickedButton2()
{
	// Dark color selector
	CHOOSECOLOR cdb;
	ZeroMemory(&cdb, sizeof(cdb));
	cdb.hwndOwner = g_pEyeTrackingDlg->GetSafeHwnd();//HWND()
	cdb.lStructSize = sizeof(cdb);
	cdb.lpCustColors = (LPDWORD) g_pEyeTrackingDlg->acrCustClr;
	cdb.rgbResult = g_pGratingsDlg->darkRGB;
	cdb.Flags = CC_SOLIDCOLOR | CC_RGBINIT ;//| CC_FULLOPEN;
	if (ChooseColor(&cdb)) {
		g_pGratingsDlg->darkRGB = cdb.rgbResult;
		g_pEyeTrackingDlg->Invalidate();
	}
}

void CGratingsDlg::OnPaint()
{
	CDialog::OnPaint();
	CRect rect;
	char rgbtext[1024];

	CPaintDC dc1(&m_LightPic);
	m_LightPic.GetClientRect(rect);
	dc1.FillSolidRect(rect, g_pGratingsDlg->lightRGB);
	sprintf(rgbtext, "RGB %d, %d, %d\0", GetRValue(g_pGratingsDlg->lightRGB), GetGValue(g_pGratingsDlg->lightRGB), GetBValue(g_pGratingsDlg->lightRGB));
	m_LightText.SetWindowText(CString(rgbtext));

	CPaintDC dc2(&m_DarkPic);
	m_DarkPic.GetClientRect(rect);
	dc2.FillSolidRect(rect, g_pGratingsDlg->darkRGB);
	sprintf(rgbtext, "RGB %d, %d, %d\0", GetRValue(g_pGratingsDlg->darkRGB), GetGValue(g_pGratingsDlg->darkRGB), GetBValue(g_pGratingsDlg->darkRGB));
	m_DarkText.SetWindowText(CString(rgbtext));
}

void CGratingsDlg::OnBnClickedLeftEyeAutoRF()
{
	m_RFX.SetWindowText(etu::FloatToString(g_receptiveFieldLeft.xPos));
	m_RFY.SetWindowText(etu::FloatToString(g_receptiveFieldLeft.yPos));

	g_pGratings->setRFX(g_receptiveFieldLeft.xPos);
	g_pGratings->setRFY(g_receptiveFieldLeft.yPos);
}

void CGratingsDlg::OnBnClickedRightEyeAutoRF()
{
	m_RFX.SetWindowText(etu::FloatToString(g_receptiveFieldRight.xPos));
	m_RFY.SetWindowText(etu::FloatToString(g_receptiveFieldRight.yPos));

	g_pGratings->setRFX(g_receptiveFieldRight.xPos);
	g_pGratings->setRFY(g_receptiveFieldRight.yPos);
}


#endif