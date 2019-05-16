/**
 * @file EyeTrackingDlg.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "stdafx.h"
#include "AppCore.h"
#include "eyetrackingdlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CEyeTrackingDlg::CEyeTrackingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEyeTrackingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	//color picker custom colors
	acrCustClr[0] = RGB(0,0,0);
	acrCustClr[1] = RGB(127,127,127);
	acrCustClr[2] = RGB(255,255,255);
	acrCustClr[3] = RGB(255,0,0);
	acrCustClr[4] = RGB(0,255,0);
	acrCustClr[5] = RGB(0,0,255);
	acrCustClr[6] = RGB(255,0,255);

	//analog eye position filter
	lxs=NULL;
	lys=NULL;
	rxs=NULL;
	rys=NULL;
	lxn=0;
	lyn=0;
	rxn=0;
	ryn=0;
	avglx=0;
	avgly=0;
	avgrx=0;
	avgry=0;
}

void CEyeTrackingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABS, m_pTabCtrl);
	DDX_Control(pDX, ID_RUN, m_Run);
	DDX_Control(pDX, IDC_LoadSettings, m_LoadSettings);
	DDX_Control(pDX, IDC_SaveSettings, m_SaveSettings);
	DDX_Control(pDX, IDC_MakeDefault, m_MakeDefault);
	DDX_Control(pDX, IDC_LoadDefault, m_LoadDefault);
	DDX_Control(pDX, ID_EXIT, m_Exit);
	DDX_Control(pDX, IDC_DotColor, m_DotColor);
	DDX_Control(pDX, IDC_DotPic, m_DotPic);
	DDX_Control(pDX, IDC_DotText, m_DotText);
	DDX_Control(pDX, IDC_BackgroundColor, m_BackgroundColor);
	DDX_Control(pDX, IDC_BackgroundText, m_BackgroundText);
	DDX_Control(pDX, IDC_BackgroundPic, m_BackgroundPic);
	DDX_Control(pDX, IDC_StimX, m_StimX);
	DDX_Control(pDX, IDC_StimY, m_StimY);
	DDX_Control(pDX, IDC_StimXSpin, m_StimXSpin);
	DDX_Control(pDX, IDC_StimYSpin, m_StimYSpin);
	DDX_Control(pDX, IDC_StimXStatic, m_StimXStatic);
	DDX_Control(pDX, IDC_StimYStatic, m_StimYStatic);
	DDX_Control(pDX, IDC_ApplySettings, m_ApplySettings);
	DDX_Control(pDX, IDC_ScreenXStatic, m_ScreenXStatic);
	DDX_Control(pDX, IDC_ScreenYStatic, m_ScreenYStatic);
	DDX_Control(pDX, IDC_ScreenX, m_ScreenX);
	DDX_Control(pDX, IDC_ScreenY, m_ScreenY);
	DDX_Control(pDX, IDC_ScreenXSpin, m_ScreenXSpin);
	DDX_Control(pDX, IDC_ScreenYSpin, m_ScreenYSpin);
	DDX_Control(pDX, IDC_ShutterL, m_ShutterL);
	DDX_Control(pDX, IDC_ShutterR, m_ShutterR);
	DDX_Control(pDX, IDC_ShutterLState, m_ShutterLState);
	DDX_Control(pDX, IDC_ShutterRState, m_ShutterRState);
}

BEGIN_MESSAGE_MAP(CEyeTrackingDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABS, OnTcnSelchangeTabs)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TABS, OnTcnSelchangingTabs)
	ON_BN_CLICKED(ID_EXIT, OnBnClickedExit)
	ON_BN_CLICKED(ID_RUN, OnBnClickedRun)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_LoadSettings, OnBnClickedLoadsettings)
	ON_BN_CLICKED(IDC_SaveSettings, OnBnClickedSavesettings)
	ON_BN_CLICKED(IDC_MakeDefault, OnBnClickedMakedefault)
	ON_BN_CLICKED(IDC_LoadDefault, OnBnClickedLoaddefault)
    ON_BN_CLICKED(IDC_DotColor, OnBnClickedDotcolor)
    ON_BN_CLICKED(IDC_BackgroundColor, OnBnClickedBackgroundcolor)
    ON_NOTIFY(UDN_DELTAPOS, IDC_StimXSpin, OnDeltaposStimxspin)
    ON_NOTIFY(UDN_DELTAPOS, IDC_StimYSpin, OnDeltaposStimyspin)
    ON_BN_CLICKED(IDC_ApplySettings, OnBnClickedApplysettings)
    ON_NOTIFY(UDN_DELTAPOS, IDC_ScreenXSpin, OnDeltaposScreenxspin)
    ON_NOTIFY(UDN_DELTAPOS, IDC_ScreenYSpin, OnDeltaposScreenyspin)
    ON_BN_CLICKED(IDC_ShutterL, OnBnClickedShutterl)
    ON_BN_CLICKED(IDC_ShutterR, OnBnClickedShutterr)
END_MESSAGE_MAP()

BOOL CEyeTrackingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	pActiveThread=NULL;
	g_pActiveStimulus=NULL;

	#ifdef APP_EYE_TRACKING
		SetWindowText("Eye Tracking Application");
	#endif

	#ifdef APP_ACUITY
		SetWindowText("Acuity Testing Application");
	#endif

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	AddTabs();

	return FALSE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEyeTrackingDlg::OnPaint() 
{
	if (IsIconic()) {
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialog::OnPaint();
		CRect rect;
		char rgbtext[1024];

		CPaintDC dc_d(&m_DotPic);
		m_DotPic.GetClientRect(rect);
		dc_d.FillSolidRect(rect, g_pSettingsDlg->dotRGB);
		sprintf(rgbtext, "RGB %d, %d, %d\0", GetRValue(g_pSettingsDlg->dotRGB), GetGValue(g_pSettingsDlg->dotRGB), GetBValue(g_pSettingsDlg->dotRGB));
		m_DotText.SetWindowText(CString(rgbtext));

		CPaintDC dc_b(&m_BackgroundPic);
		m_BackgroundPic.GetClientRect(rect);
		dc_b.FillSolidRect(rect, g_pSettingsDlg->bgRGB);
		sprintf(rgbtext, "RGB %d, %d, %d\0", GetRValue(g_pSettingsDlg->bgRGB), GetGValue(g_pSettingsDlg->bgRGB), GetBValue(g_pSettingsDlg->bgRGB));
		m_BackgroundText.SetWindowText(CString(rgbtext));
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEyeTrackingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEyeTrackingDlg::AddTabs() 
{
	//automatically rearrange all of the controls on the main dialog window
	ShowWindow(SW_MAXIMIZE);
	RECT thisWindow;
	this->GetWindowRect(&thisWindow);
	int tabLeft = 10;
	int tabTop = 10;
	int tabWidth = thisWindow.right - thisWindow.left-50;
	int tabHeight = thisWindow.bottom - thisWindow.top - 175;
	int buttonTop = tabTop + tabHeight + 20;
	int buttonLeft = tabLeft+5;
	int buttonWidth = 200;
	int buttonHeight = 75;
	m_pTabCtrl.SetWindowPos(0, tabLeft, tabTop, tabWidth, tabHeight, SWP_SHOWWINDOW);

	m_LoadSettings.SetWindowPos(0, buttonLeft, buttonTop+40, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_SaveSettings.SetWindowPos(0, buttonLeft, buttonTop+75, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_MakeDefault.SetWindowPos(0, buttonLeft+110, buttonTop+40, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_LoadDefault.SetWindowPos(0, buttonLeft+110, buttonTop+75, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_Run.SetWindowPos(0, buttonLeft+tabWidth - 105, buttonTop, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_Exit.SetWindowPos(0, buttonLeft+tabWidth - 105, buttonTop+75, buttonWidth, buttonHeight, SWP_NOSIZE);

	//these controls were previously on the global settings panel
	m_DotColor.SetWindowPos(0, buttonLeft+400, buttonTop+5, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_DotText.SetWindowPos(0, buttonLeft+400, buttonTop+35, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_DotPic.SetWindowPos(0, buttonLeft+510, buttonTop, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_BackgroundColor.SetWindowPos(0, buttonLeft+400, buttonTop+65, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_BackgroundText.SetWindowPos(0, buttonLeft+400, buttonTop+95, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_BackgroundPic.SetWindowPos(0, buttonLeft+510, buttonTop+60, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_StimXStatic.SetWindowPos(0, buttonLeft+560, buttonTop+4, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_StimX.SetWindowPos(0, buttonLeft+680, buttonTop, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_StimXSpin.SetWindowPos(0, buttonLeft+740, buttonTop, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_StimYStatic.SetWindowPos(0, buttonLeft+560, buttonTop+4+35, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_StimY.SetWindowPos(0, buttonLeft+680, buttonTop+35, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_StimYSpin.SetWindowPos(0, buttonLeft+740, buttonTop+35, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_ScreenXStatic.SetWindowPos(0, buttonLeft+780, buttonTop+4, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_ScreenX.SetWindowPos(0, buttonLeft+900, buttonTop, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_ScreenXSpin.SetWindowPos(0, buttonLeft+960, buttonTop, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_ScreenYStatic.SetWindowPos(0, buttonLeft+780, buttonTop+4+35, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_ScreenY.SetWindowPos(0, buttonLeft+900, buttonTop+35, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_ScreenYSpin.SetWindowPos(0, buttonLeft+960, buttonTop+35, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_ApplySettings.SetWindowPos(0, buttonLeft+710, buttonTop+75, buttonWidth, buttonHeight, SWP_NOSIZE);

	//shutter controls
	m_ShutterLState.SetWindowPos(0, buttonLeft+1130, buttonTop, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_ShutterL.SetWindowPos(0, buttonLeft+1130, buttonTop+35, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_ShutterRState.SetWindowPos(0, buttonLeft+1250, buttonTop, buttonWidth, buttonHeight, SWP_NOSIZE);
	m_ShutterR.SetWindowPos(0, buttonLeft+1250, buttonTop+35, buttonWidth, buttonHeight, SWP_NOSIZE);
	OpenLeftShutter(true);
	OpenRightShutter(true);
	
	TCITEM tcItem;

	// setup tabs that are common to all apps
	char* tabZero ="         Global Settings         ";
	char* tabOne = "Calibration Stimulus";
	char* tabTwo = "Pursuit Stimulus";

	tcItem.mask=TCIF_TEXT;
	tcItem.cchTextMax=50;

	tcItem.pszText=tabZero;
	m_pTabCtrl.InsertItem(0, &tcItem);

	tcItem.pszText=tabOne;
	m_pTabCtrl.InsertItem(1, &tcItem);

	tcItem.pszText=tabTwo;
	m_pTabCtrl.InsertItem(2, &tcItem);

	g_pCalibrationDlg = new CCalibrationDlg();
	g_pCalibrationDlg->Create(IDD_CALIBRATION_DLG, this);

	g_pSettingsDlg = new CSettingsDlg();
	g_pSettingsDlg->Create(IDD_SETTINGS_DLG, this);

	g_pPursuitDlg = new CPursuitDlg();
	g_pPursuitDlg->Create(IDD_PURSUIT_DLG, this);
	
#ifdef APP_EYE_TRACKING
	char* tabThree = "Saccade";
	char* tabFour = "Full Field Motion";
	char* tabFive = "Step Ramp";
	char* tabSix = "Scene Calibration";
	char* tabSeven = "Ocular Following";
	char* tabEight = "Isoluminant Patch";
	char* tabNine = "RF Mapping";
	char* tabTen = "Orientation Tuning";
	char* tabEleven = "Circle Annulus";
	char* tabTwelve = "APD";
	char* tabThirteen = "Human Suppression Scotomas";
	char* tabFourteen = "Minimum Motion";
	char* tabFifteen = "Alternation";
	char* tabSixteen= "m-Sequence";
	char* tabSeventeen= "Ruler";
	char* tabEighteen= "Amblyoscope";
	char* tabNineteen= "Scotoma Mapping";
	char* tabTwenty= "Random Dot Anaglyph";
	char* tabTwentyOne= "Gratings";
	char* tabTwentyTwo= "Trial-based Saccades";
	char* tabTwentyThree = "Fixation Stability";

	tcItem.pszText=tabThree;
	m_pTabCtrl.InsertItem(3, &tcItem);

	tcItem.pszText=tabFour;
	m_pTabCtrl.InsertItem(4, &tcItem);

	tcItem.pszText=tabFive;
	m_pTabCtrl.InsertItem(5, &tcItem);

	tcItem.pszText = tabSix;
	m_pTabCtrl.InsertItem(6, &tcItem);

	tcItem.pszText = tabSeven;
	m_pTabCtrl.InsertItem(7, &tcItem);

	tcItem.pszText = tabEight;
	m_pTabCtrl.InsertItem(8, &tcItem);
	
	tcItem.pszText = tabNine;
	m_pTabCtrl.InsertItem(9, &tcItem);

	tcItem.pszText = tabTen;
	m_pTabCtrl.InsertItem(10, &tcItem);

	tcItem.pszText = tabEleven;
	m_pTabCtrl.InsertItem(11, &tcItem);

	tcItem.pszText = tabTwelve;
	m_pTabCtrl.InsertItem(12, &tcItem);

	tcItem.pszText = tabThirteen;
	m_pTabCtrl.InsertItem(13, &tcItem);

	tcItem.pszText = tabFourteen;
	m_pTabCtrl.InsertItem(14, &tcItem);

	tcItem.pszText = tabFifteen;
	m_pTabCtrl.InsertItem(15, &tcItem);

	tcItem.pszText = tabSixteen;
	m_pTabCtrl.InsertItem(16, &tcItem);

	tcItem.pszText = tabSeventeen;
	m_pTabCtrl.InsertItem(17, &tcItem);

	tcItem.pszText = tabEighteen;
	m_pTabCtrl.InsertItem(18, &tcItem);
	
	tcItem.pszText = tabNineteen;
	m_pTabCtrl.InsertItem(19, &tcItem);

	tcItem.pszText = tabTwenty;
	m_pTabCtrl.InsertItem(20, &tcItem);

	tcItem.pszText = tabTwentyOne;
	m_pTabCtrl.InsertItem(21, &tcItem);

	tcItem.pszText = tabTwentyTwo;
	m_pTabCtrl.InsertItem(22, &tcItem);

	tcItem.pszText = tabTwentyThree;
	m_pTabCtrl.InsertItem(23, &tcItem);

	g_pSaccadeDlg=new CSaccadeDlg();
	g_pSaccadeDlg->Create(IDD_SACCADE_DLG, this);
	
	g_pFullfieldDlg = new CFullfieldDlg();
	g_pFullfieldDlg->Create(IDD_FULLFIELD_DLG, this);

	g_pStepRampDlg= new CStepRampDlg();
	g_pStepRampDlg->Create(IDD_STEPRAMP_DLG, this);

	g_pSceneCalibrationDlg= new CSceneCalibrationDlg();
	g_pSceneCalibrationDlg->Create(IDD_SCENE_CALIBRATION, this);

	g_pOcularFollowingDlg= new COcularFollowingDlg();
	g_pOcularFollowingDlg->Create(IDD_OCULAR_FOLLOWING, this);

	g_pIsoluminantPatchDlg = new CIsoluminantPatchDlg();
	g_pIsoluminantPatchDlg->Create(IDD_ISOLUMINANT_PATCH, this);

	g_pRFMappingDlg = new CRFMappingDlg();
	g_pRFMappingDlg->Create(IDD_RF_MAPPING_DLG, this);
	g_pRFMappingDlg->SetWindowPos(0, tabLeft+20, tabTop+50, tabWidth-100, tabHeight-100, SWP_HIDEWINDOW);//+10 +25 -10 -25

	g_pOrientationTuningDlg = new COrientationTuningDlg();
	g_pOrientationTuningDlg->Create(IDD_ORIENTATION_TUNING_DLG, this);
	g_pOrientationTuningDlg->SetWindowPos(0, tabLeft+20, tabTop+50, tabWidth-100, tabHeight-100, SWP_HIDEWINDOW);

	g_pCircleAnnulusDlg = new CCircleAnnulusDlg();
	g_pCircleAnnulusDlg->Create(IDD_CIRCLE_ANNULUS_DLG, this);
	g_pCircleAnnulusDlg->SetWindowPos(0, tabLeft+20, tabTop+50, tabWidth-100, tabHeight-100, SWP_HIDEWINDOW);

	g_pAPDDlg = new CAPDDlg();
	g_pAPDDlg->Create(IDD_APD_DLG, this);

	g_pRememberDlg = new CRememberDlg();
	g_pRememberDlg->Create(IDD_REMEMBER_DLG, this);
	
	g_pSuppressionDlg = new CSuppressionDlg();
	g_pSuppressionDlg->Create(IDD_SUPPRESSION_DLG, this);

	g_pAlternationDlg = new CAlternationDlg();
	g_pAlternationDlg->Create(IDD_ALTERNATION_DLG, this);

	g_pMSequenceDlg = new CMSequenceDlg();
	g_pMSequenceDlg->Create(IDD_MSEQUENCE_DLG, this);

	g_pRulerDlg = new CRulerDlg();
	g_pRulerDlg->Create(IDD_RULER_DLG, this);
	
	g_pAmblyoscopeDlg = new CAmblyoscopeDlg();
	g_pAmblyoscopeDlg->Create(IDD_AMBLYOSCOPE_DLG, this);

	g_pScotomaMapDlg = new CScotomaMapDlg();
	g_pScotomaMapDlg->Create(IDD_SCOTOMA_MAP_DLG, this);

	g_pTrialSaccadesDlg = new CTrialSaccadesDlg();
	g_pTrialSaccadesDlg->Create(IDD_TRIAL_SACCADES_DLG, this);
	
	g_pRandomDotAnaglyphDlg = new CRandomDotAnaglyphDlg();
	g_pRandomDotAnaglyphDlg->Create(IDD_ANAGLYPH_DLG, this);

	g_pGratingsDlg = new CGratingsDlg();
	g_pGratingsDlg->Create(IDD_GRATINGS_DLG, this);

	g_pFixationStabilityDlg = new CFixationStabilityDlg();
	g_pFixationStabilityDlg->Create(IDD_FIXATIONSTAB_DLG, this);
#endif

#ifdef APP_ACUITY
	char* tabThree = "Preferrential Selection";
	char* tabFour = "Noise Grating Stimulus";
	
	tcItem.pszText=tabThree;
	m_pTabCtrl.InsertItem(3, &tcItem);

	tcItem.pszText=tabFour;
	m_pTabCtrl.InsertItem(4, &tcItem);
	
	// create dialogs;
	g_pPrefSelectionDlg= new CPrefSelectionDlg();
	g_pPrefSelectionDlg->Create(IDD_PREF_SEL_DLG, this);

	g_pNoiseGratingsDlg =new CNoiseGratingsDlg();
	g_pNoiseGratingsDlg->Create(IDD_NOISE_GRATING_DLG, this);
#endif

	g_pSettingsDlg->ShowWindow(SW_SHOW);
	m_Run.EnableWindow(false);
}

void CEyeTrackingDlg::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iCurrSel=m_pTabCtrl.GetCurSel();

	switch (iCurrSel) {
		case 0:  g_pSettingsDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(false); break;		
		case 1:  g_pCalibrationDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true);  break;
		case 2:  g_pPursuitDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
	#ifdef APP_EYE_TRACKING
		case 3:  g_pSaccadeDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 4:  g_pFullfieldDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 5:  g_pStepRampDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 6:  g_pSceneCalibrationDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 7:  g_pOcularFollowingDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 8:  g_pIsoluminantPatchDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 9:  g_pRFMappingDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 10: g_pOrientationTuningDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 11: g_pCircleAnnulusDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 12: g_pAPDDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 13: g_pRememberDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 14: g_pSuppressionDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 15: g_pAlternationDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 16: g_pMSequenceDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 17: g_pRulerDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 18: g_pAmblyoscopeDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 19: g_pScotomaMapDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 20: g_pRandomDotAnaglyphDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 21: g_pGratingsDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 22: g_pTrialSaccadesDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 23: g_pFixationStabilityDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
	#endif
	#ifdef APP_ACUITY
		case 3: g_pPrefSelectionDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
		case 4: g_pNoiseGratingsDlg->ShowWindow(SW_SHOW); m_Run.EnableWindow(true); break;
	#endif		
	} // switch

	*pResult = 0;
}

void CEyeTrackingDlg::OnTcnSelchangingTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int iCurrSel=m_pTabCtrl.GetCurSel();
	switch (iCurrSel) {
		case 0:  g_pSettingsDlg->ShowWindow(SW_HIDE); break;
		case 1:  g_pCalibrationDlg->ShowWindow(SW_HIDE); break;
		case 2:  g_pPursuitDlg->ShowWindow(SW_HIDE); break;
	#ifdef APP_EYE_TRACKING
		case 3:  g_pSaccadeDlg->ShowWindow(SW_HIDE); break;
		case 4:  g_pFullfieldDlg->ShowWindow(SW_HIDE); break;
		case 5:  g_pStepRampDlg->ShowWindow(SW_HIDE); break;
		case 6:  g_pSceneCalibrationDlg->ShowWindow(SW_HIDE); break;
		case 7:  g_pOcularFollowingDlg->ShowWindow(SW_HIDE); break;
		case 8:  g_pIsoluminantPatchDlg->ShowWindow(SW_HIDE); break;
		case 9:  g_pRFMappingDlg->ShowWindow(SW_HIDE); break;
		case 10: g_pOrientationTuningDlg->ShowWindow(SW_HIDE); break;
		case 11: g_pCircleAnnulusDlg->ShowWindow(SW_HIDE); break;
		case 12: g_pAPDDlg->ShowWindow(SW_HIDE); break;
		case 13: g_pRememberDlg->ShowWindow(SW_HIDE); break;
		case 14: g_pSuppressionDlg->ShowWindow(SW_HIDE); break;
		case 15: g_pAlternationDlg->ShowWindow(SW_HIDE); break;
		case 16: g_pMSequenceDlg->ShowWindow(SW_HIDE); break;
		case 17: g_pRulerDlg->ShowWindow(SW_HIDE); break;
		case 18: g_pAmblyoscopeDlg->ShowWindow(SW_HIDE); break;
		case 19: g_pScotomaMapDlg->ShowWindow(SW_HIDE); break;
		case 20: g_pRandomDotAnaglyphDlg->ShowWindow(SW_HIDE); break;
		case 21: g_pGratingsDlg->ShowWindow(SW_HIDE); break;
		case 22: g_pTrialSaccadesDlg->ShowWindow(SW_HIDE); break;
		case 23: g_pFixationStabilityDlg->ShowWindow(SW_HIDE); break;
	#endif
	#ifdef APP_ACUITY
		case 3:  g_pPrefSelectionDlg->ShowWindow(SW_HIDE); break;
		case 4:  g_pNoiseGratingsDlg->ShowWindow(SW_HIDE); break;
	#endif	
	} // switch

	*pResult = 0;
}

void CEyeTrackingDlg::OnBnClickedExit()
{
	// if there is an active stimulus make sure its stopped first.
	if (g_pActiveStimulus) OnBnClickedRun();

	ExitApp();
}

void CEyeTrackingDlg::OnBnClickedRun()
{
	// if there is not an active stimulus, create a new thread and start running the appropriate stim
	if (!g_pActiveStimulus) {
		int iCurrSel=m_pTabCtrl.GetCurSel();
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
		switch (iCurrSel) {
			case 1:  pActiveThread = AfxBeginThread(CalibrationControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;			
		#ifdef APP_EYE_TRACKING
			case 3:  pActiveThread = AfxBeginThread(SaccadeControllingFunction, &m_Run, THREAD_PRIORITY_TIME_CRITICAL, 0, 0, NULL); break;
			case 4:  pActiveThread = AfxBeginThread(FullfieldControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;
			case 5:  pActiveThread = AfxBeginThread(StepRampControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;			
			case 6:  pActiveThread = AfxBeginThread(SceneCalibrationControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;
			case 7:  pActiveThread = AfxBeginThread(OcularFollowingControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;
			case 8:  pActiveThread = AfxBeginThread(IsoluminantPatchControllingFunction, &m_Run, THREAD_PRIORITY_TIME_CRITICAL, 0, 0, NULL); break;
			case 9:  pActiveThread = AfxBeginThread(RFMappingControllingFunction, &m_Run, THREAD_PRIORITY_TIME_CRITICAL, 0, 0, NULL); break;
			case 10: pActiveThread = AfxBeginThread(OrientationTuningControllingFunction, &m_Run, THREAD_PRIORITY_TIME_CRITICAL, 0, 0, NULL); break;
			case 11: pActiveThread = AfxBeginThread(CircleAnnulusControllingFunction, &m_Run, THREAD_PRIORITY_TIME_CRITICAL, 0, 0, NULL); break;
			case 12: pActiveThread = AfxBeginThread(APDControllingFunction, &m_Run, THREAD_PRIORITY_TIME_CRITICAL, 0, 0, NULL); break;
			case 13: pActiveThread = AfxBeginThread(RememberControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;
			case 14: pActiveThread = AfxBeginThread(SuppressionControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;
			case 15: pActiveThread = AfxBeginThread(AlternationControllingFunction, &m_Run, THREAD_PRIORITY_BELOW_NORMAL, 0, 0, NULL); break;
			case 16: pActiveThread = AfxBeginThread(MSequenceControllingFunction, &m_Run, THREAD_PRIORITY_TIME_CRITICAL, 0, 0, NULL); break;
			case 17: pActiveThread = AfxBeginThread(RulerControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;
			case 18: pActiveThread = AfxBeginThread(AmblyoscopeControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;
			case 19: pActiveThread = AfxBeginThread(ScotomaMapControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;
			case 20: pActiveThread = AfxBeginThread(RandomDotAnaglyphControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;
			case 21: pActiveThread = AfxBeginThread(GratingsControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;
			case 22: pActiveThread = AfxBeginThread(TrialSaccadesControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;
			case 23: pActiveThread = AfxBeginThread(FixationStabilityControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;
		#endif
		#ifdef APP_ACUITY
			case 3:  pActiveThread = AfxBeginThread(PrefSelectionControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;			
			case 4:  pActiveThread = AfxBeginThread(NoiseGratingsControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;
		#endif
			case 2: 
			default:
				pActiveThread = AfxBeginThread(PursuitControllingFunction, &m_Run, THREAD_PRIORITY_NORMAL, 0, 0, NULL); break;	
		} // switch
	} 
	else {
		// Stop button pressed
		m_Run.EnableWindow(false);
		g_pActiveStimulus->StopExecution();
		SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
		
	}
}

// Runs the CalibrationStimulus by spawning a new thread
UINT __cdecl CalibrationControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}

	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new CCalibrationStimulus();

	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);
	return 0;
}


// Runs the PursuitStimulus by spawning a new thread
UINT __cdecl PursuitControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}

	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new CPursuitStimulus();

	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	return 0;
}

#ifdef APP_EYE_TRACKING
// Runs the SaccadeStimulus by spawning a new thread
UINT __cdecl SaccadeControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}
	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new CSaccadeStimulus();

	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	return 0;
}

UINT __cdecl FullfieldControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}

	g_pFullfieldDlg->ActivateApplyButton(false);

	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new CFullfieldStimulus();

	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	g_pFullfieldDlg->ActivateApplyButton(true);

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);
	
	return 0;
}

UINT __cdecl StepRampControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}

	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new CStepRampStimulus();

	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);
	
	return 0;
}

// Runs the SaccadeStimulus by spawning a new thread
UINT __cdecl SceneCalibrationControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}
	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new CSceneCalibrationStimulus();

	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;
	
	return 0;
}

// Runs the OcularTrainingStimulus by spawning a new thread
UINT __cdecl OcularFollowingControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}
	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new COcularFollowingStimulus();

	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	return 0;
}

// Runs the IsoluminantPatchStimulus by spawning a new thread
UINT __cdecl IsoluminantPatchControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}
	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new CIsoluminantPatchStimulus();

	g_pIsoluminantPatchDlg->disableControls();

	g_pIsoluminantPatchDlg->SetCapture();

	if (g_pIsoluminantPatch->useHighestPriority()) {
		g_pIsoluminantPatch->SetLoop(false);
		DWORD pID = 0;
		DWORD h = GetWindowThreadProcessId(AfxGetMainWnd()->GetSafeHwnd(), &pID);
		HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, false, pID);
		SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);
		CloseHandle(hProcess);

		g_pActiveStimulus->Run();
				
		h = GetWindowThreadProcessId(AfxGetMainWnd()->GetSafeHwnd(), &pID);
		hProcess = OpenProcess(PROCESS_SET_INFORMATION, false, pID);
		SetPriorityClass(hProcess, NORMAL_PRIORITY_CLASS);
		CloseHandle(hProcess);
	
	} 
	else {
		g_pActiveStimulus->Run();
	}

	ReleaseCapture();

	g_pIsoluminantPatchDlg->enableControls();

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	return 0;
}

UINT __cdecl RFMappingControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}
	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new CRFMappingStimulus();

	DWORD pID = 0;
	DWORD h = GetWindowThreadProcessId(AfxGetMainWnd()->GetSafeHwnd(), &pID);
	HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, false, pID);
	SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
	CloseHandle(hProcess);

	g_pRFMappingDlg->SetCapture();
	g_pEyeTrackingDlg->SetFocus();

	g_pRFMappingDlg->disableControls();
	g_pActiveStimulus->Run();
	ReleaseCapture();
	g_pRFMappingDlg->enableControls();

	pID = 0;
	h = GetWindowThreadProcessId(AfxGetMainWnd()->GetSafeHwnd(), &pID);
	hProcess = OpenProcess(PROCESS_SET_INFORMATION, false, pID);
	SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
	CloseHandle(hProcess);

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	return 0;
}

UINT __cdecl OrientationTuningControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}
	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new COrientationTuningStimulus();

	DWORD pID = 0;
	DWORD h = GetWindowThreadProcessId(AfxGetMainWnd()->GetSafeHwnd(), &pID);
	HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, false, pID);
	SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
	CloseHandle(hProcess);

	g_pOrientationTuningDlg->disableControls();
	g_pOrientationTuningDlg->disableApply();
	g_pOrientationTuningDlg->enableStartPresentations();
		
	g_pActiveStimulus->Run();

	g_pOrientationTuningDlg->disableStartPresentations();
	g_pOrientationTuningDlg->enableControls();

	pID = 0;
	h = GetWindowThreadProcessId(AfxGetMainWnd()->GetSafeHwnd(), &pID);
	hProcess = OpenProcess(PROCESS_SET_INFORMATION, false, pID);
	SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
	CloseHandle(hProcess);

	g_pOrientationTuningDlg->enableApply();

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	return 0;
}

UINT __cdecl CircleAnnulusControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}
	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new CCircleAnnulusStimulus();

	DWORD pID = 0;
	DWORD h = GetWindowThreadProcessId(AfxGetMainWnd()->GetSafeHwnd(), &pID);
	HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, false, pID);
	SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);
	CloseHandle(hProcess);

	g_pCircleAnnulusDlg->disableControls();
	g_pCircleAnnulusDlg->enableStartPresentations();
		
	g_pActiveStimulus->Run();

	g_pCircleAnnulusDlg->enableControls();
	g_pCircleAnnulusDlg->disableStartPresentations();
	
	pID = 0;
	h = GetWindowThreadProcessId(AfxGetMainWnd()->GetSafeHwnd(), &pID);
	hProcess = OpenProcess(PROCESS_SET_INFORMATION, false, pID);
	SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
	CloseHandle(hProcess);

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	// thread is done
	delete ((CCircleAnnulusStimulus*)g_pActiveStimulus);
    g_pActiveStimulus = NULL;
	
	return 0;
}

UINT __cdecl APDControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}

	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new CAPDStimulus();

	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	return 0;
}

UINT __cdecl RememberControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}
	
	((CButton*)pParam)->SetWindowText("Stop");
	
	// Create the saccade stimulus object
	g_pActiveStimulus = new CRememberStimulus();
	
	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();
	
	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;
	
	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	return 0;
}

UINT __cdecl SuppressionControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}

	((CButton*)pParam)->SetWindowText("Stop");

	// Create the suppression stimulus object
	g_pActiveStimulus = new CSuppressionStimulus();

	// Run the suppression stimulus object
	g_pActiveStimulus->Run();

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	return 0;
}

UINT __cdecl AlternationControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}

	((CButton*)pParam)->SetWindowText("Stop");

	// Create the alternation stimulus object
	g_pActiveStimulus = new CAlternationStimulus();

	// Run the alternation stimulus object
	g_pActiveStimulus->Run();

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	return 0;
}

UINT __cdecl MSequenceControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}

	((CButton*)pParam)->SetWindowText("Stop");

	// Create the alternation stimulus object
	g_pActiveStimulus = new CMSequenceStimulus();

	// Run the alternation stimulus object
	g_pActiveStimulus->Run();

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	return 0;
}

UINT __cdecl RulerControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}

	((CButton*)pParam)->SetWindowText("Stop");

	// Create the alternation stimulus object
	g_pActiveStimulus = new CRulerStimulus();

	// Run the alternation stimulus object
	g_pActiveStimulus->Run();

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	return 0;
}

UINT __cdecl AmblyoscopeControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}

	((CButton*)pParam)->SetWindowText("Stop");

	// Create the alternation stimulus object
	g_pActiveStimulus = new CAmblyoscopeStimulus();

	// Run the alternation stimulus object
	g_pActiveStimulus->Run();

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	return 0;
}

UINT __cdecl ScotomaMapControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}
	
	((CButton*)pParam)->SetWindowText("Stop");
	
	// Create the saccade stimulus object
	g_pActiveStimulus = new CScotomaMapStimulus();
	
	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();
	
	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;
	
	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	return 0;
}

UINT __cdecl RandomDotAnaglyphControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}
	
	((CButton*)pParam)->SetWindowText("Stop");
	
	// Create the saccade stimulus object
	g_pActiveStimulus = new CRandomDotAnaglyphStimulus();
	
	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();
	
	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;
	
	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	return 0;
}


UINT __cdecl TrialSaccadesControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}
	
	((CButton*)pParam)->SetWindowText("Stop");
	
	// Create the saccade stimulus object
	g_pActiveStimulus = new CTrialSaccadesStimulus();
	
	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();
	
	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;
	
	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	return 0;
}

UINT __cdecl GratingsControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}
	
	((CButton*)pParam)->SetWindowText("Stop");
	
	// Create the saccade stimulus object
	g_pActiveStimulus = new CGratingsStimulus();
	
	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();
	
	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;
	
	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);
	
	return 0;
}

UINT __cdecl FixationStabilityControllingFunction(LPVOID pParam) 
{
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}
	
	((CButton*)pParam)->SetWindowText("Stop");
	
	// Create the saccade stimulus object
	g_pActiveStimulus = new CFixationStabilityStimulus();
	
	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();
	
	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;
	
	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);
	
	return 0;
}

#endif // #ifdef APP_EYE_TRACKING

#ifdef APP_ACUITY
UINT __cdecl PrefSelectionControllingFunction(LPVOID pParam) {
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}

	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new CPrefSelectionStimulus();

	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	
	return 0;
}


UINT __cdecl NoiseGratingsControllingFunction(LPVOID pParam) {
	if (g_pActiveStimulus) {
		AfxMessageBox("Error -- Old Stimulus still active");
		return 1;
	}

	((CButton*)pParam)->SetWindowText("Stop");

	// Create the saccade stimulus object
	g_pActiveStimulus = new CNoiseGratingsStimulus();

	// Run the sacadde stimulus object
	g_pActiveStimulus->Run();

	// thread is done
	delete g_pActiveStimulus;
    g_pActiveStimulus = NULL;

	// reactivate the run / stop button and set the label to run
	((CButton*)pParam)->SetWindowText("Run");
	((CButton*)pParam)->EnableWindow(true);

	
	return 0;
}


#endif

void CEyeTrackingDlg::OnClose()
{
	ExitApp();

	CDialog::OnClose();
}

void CEyeTrackingDlg::OnBnClickedOk()
{
	
}

void CEyeTrackingDlg::OnBnClickedCancel()
{
	
}

void CEyeTrackingDlg::OnBnClickedLoadsettings()
{
	int iCurrSel=m_pTabCtrl.GetCurSel();
	switch (iCurrSel) {
		case 0:g_pSettings->LoadFile(false); g_pSettingsDlg->load(); break;
		case 1:g_pCalibration->LoadFile(false); g_pCalibrationDlg->load(); break;
		case 2:g_pPursuit->LoadFile(false); g_pPursuitDlg->load(); break;
#ifdef APP_EYE_TRACKING
		case 3:g_pSaccade->LoadFile(false); g_pSaccadeDlg->load(); break;
		case 4:g_pFullfield->LoadFile(false); g_pFullfieldDlg->load(); break;
		case 5:g_pStepRamp->LoadFile(false); g_pStepRampDlg->load(); break;
		case 6:g_pSceneCalibration->LoadFile(false); g_pSceneCalibrationDlg->load(); break;
		case 7:g_pOcularFollowing->LoadFile(false); g_pOcularFollowingDlg->load(); break;
		case 8:g_pIsoluminantPatch->LoadFile(false); g_pIsoluminantPatchDlg->load(); break;
		case 9:g_pRFMapping->LoadFile(false); g_pRFMappingDlg->load(); break;
		case 10:g_pOrientationTuning->LoadFile(false); g_pOrientationTuningDlg->load(); break;
		case 11:g_pCircleAnnulus->LoadFile(false); g_pCircleAnnulusDlg->load(); break;
		case 12:g_pAPD->LoadFile(false); g_pAPDDlg->load(); break;
		case 13:g_pRemember->LoadFile(false); g_pRememberDlg->load(); break;
		case 14:g_pSuppression->LoadFile(false); g_pSuppressionDlg->load(); break;
		case 15:g_pAlternation->LoadFile(false); g_pAlternationDlg->load(); break;
		case 16:g_pMSequence->LoadFile(false); g_pMSequenceDlg->load(); break;
		case 17:g_pRuler->LoadFile(false); g_pRulerDlg->load(); break;
		case 18:g_pAmblyoscope->LoadFile(false); g_pAmblyoscopeDlg->load(); break;
		case 19:g_pScotomaMap->LoadFile(false); g_pScotomaMapDlg->load(); break;
		case 20:g_pRandomDotAnaglyph->LoadFile(false); g_pRandomDotAnaglyphDlg->load(); break;
		case 21:g_pGratings->LoadFile(false); g_pGratingsDlg->load(); break;
		case 22:g_pTrialSaccades->LoadFile(false); g_pTrialSaccadesDlg->load(); break;
		case 23:g_pFixationStabilityModel->LoadFile(false); g_pFixationStabilityDlg->load(); break;
#endif
#ifdef APP_ACUITY
		case 3:g_pPrefSelection->LoadFile(false); g_pPrefSelectionDlg->load(); break;
		case 4:g_pNoiseGratings->LoadFile(false); g_pNoiseGratingsDlg->load(); break;
#endif
	}
	
}

void CEyeTrackingDlg::OnBnClickedSavesettings()
{
	int iCurrSel=m_pTabCtrl.GetCurSel();
	switch (iCurrSel) {
		case 0:g_pSettings->SaveFile(false); break;
		case 1:g_pCalibration->SaveFile(false); break;
		case 2:g_pPursuit->SaveFile(false); break;
#ifdef APP_EYE_TRACKING
		case 3:g_pSaccade->SaveFile(false); break;
		case 4:g_pFullfield->SaveFile(false); break;
		case 5:g_pStepRamp->SaveFile(false); break;
		case 6:g_pSceneCalibration->SaveFile(false); break;
		case 7:g_pOcularFollowing->SaveFile(false); break;
		case 8:g_pIsoluminantPatch->SaveFile(false); break;
		case 9:g_pRFMapping->SaveFile(false); break;
		case 10:g_pOrientationTuning->SaveFile(false); break;
		case 11:g_pCircleAnnulus->SaveFile(false); break;
		case 12:g_pAPD->SaveFile(false); break;
		case 13:g_pRemember->SaveFile(false); break;
		case 14:g_pSuppression->SaveFile(false); break;
		case 15:g_pAlternation->SaveFile(false); break;
		case 16:g_pMSequence->SaveFile(false); break;
		case 17:g_pRuler->SaveFile(false); break;
		case 18:g_pAmblyoscope->SaveFile(false); break;
		case 19:g_pScotomaMap->SaveFile(false); break;
		case 20:g_pRandomDotAnaglyph->SaveFile(false); break;
		case 21:g_pGratings->SaveFile(false); break;
		case 22:g_pTrialSaccades->SaveFile(false); break;
		case 23:g_pFixationStabilityModel->SaveFile(false); break;
#endif
#ifdef APP_ACUITY
		case 3:g_pPrefSelection->SaveFile(false); break;
		case 4:g_pNoiseGratings->SaveFile(false); break;
#endif
	}
}

void CEyeTrackingDlg::OnBnClickedMakedefault()
{
	int iCurrSel=m_pTabCtrl.GetCurSel();
	switch (iCurrSel) {
		case 0:g_pSettings->SaveFile(true); break;
		case 1:g_pCalibration->SaveFile(true); break;
		case 2:g_pPursuit->SaveFile(true); break;
#ifdef APP_EYE_TRACKING
		case 3:g_pSaccade->SaveFile(true); break;
		case 4:g_pFullfield->SaveFile(true); break;
		case 5:g_pStepRamp->SaveFile(true); break;
		case 6:g_pSceneCalibration->SaveFile(true); break;
		case 7:g_pOcularFollowing->SaveFile(true); break;
		case 8:g_pIsoluminantPatch->SaveFile(true); break;
		case 9:g_pRFMapping->SaveFile(true); break;
		case 10:g_pOrientationTuning->SaveFile(true); break;
		case 11:g_pCircleAnnulus->SaveFile(true); break;
		case 12:g_pAPD->SaveFile(true); break;
		case 13:g_pRemember->SaveFile(true); break;
		case 14:g_pSuppression->SaveFile(true); break;
		case 15:g_pAlternation->SaveFile(true); break;
		case 16:g_pMSequence->SaveFile(true); break;
		case 17:g_pRuler->SaveFile(true); break;
		case 18:g_pAmblyoscope->SaveFile(true); break;
		case 19:g_pScotomaMap->SaveFile(true); break;
		case 20:g_pRandomDotAnaglyph->SaveFile(true); break;
		case 21:g_pGratings->SaveFile(true); break;
		case 22:g_pTrialSaccades->SaveFile(true); break;
		case 23:g_pFixationStabilityModel->SaveFile(true); break;
#endif
#ifdef APP_ACUITY
		case 3:g_pPrefSelection->SaveFile(true); break;
		case 4:g_pNoiseGratings->SaveFile(true); break;
#endif
	}
}

void CEyeTrackingDlg::OnBnClickedLoaddefault()
{
	int iCurrSel=m_pTabCtrl.GetCurSel();
	switch (iCurrSel) {
		case 0:g_pSettings->LoadFile(true); g_pSettingsDlg->load();  break;
		case 1:g_pCalibration->LoadFile(true); g_pCalibrationDlg->load();  break;
		case 2:g_pPursuit->LoadFile(true); g_pPursuitDlg->load(); break;
#ifdef APP_EYE_TRACKING
		
		case 3:g_pSaccade->LoadFile(true); g_pSaccadeDlg->load(); break;
		case 4:g_pFullfield->LoadFile(true); g_pFullfieldDlg->load(); break;
		case 5:g_pStepRamp->LoadFile(true); g_pStepRampDlg->load(); break;
		case 6:g_pSceneCalibration->LoadFile(true); g_pSceneCalibrationDlg->load(); break;
		case 7:g_pOcularFollowing->LoadFile(true); g_pOcularFollowingDlg->load(); break;
		case 8:g_pIsoluminantPatch->LoadFile(true); g_pIsoluminantPatchDlg->load(); break;
		case 9:g_pRFMapping->LoadFile(true); g_pRFMappingDlg->load(); break;
		case 10:g_pOrientationTuning->LoadFile(true); g_pOrientationTuningDlg->load(); break;
		case 11:g_pCircleAnnulus->LoadFile(true); g_pCircleAnnulusDlg->load(); break;
		case 12:g_pAPD->LoadFile(true); g_pAPDDlg->load(); break;
		case 13:g_pRemember->LoadFile(true); g_pRememberDlg->load(); break;
		case 14:g_pSuppression->LoadFile(true); g_pSuppressionDlg->load(); break;
		case 15:g_pAlternation->LoadFile(true); g_pAlternationDlg->load(); break;
		case 16:g_pMSequence->LoadFile(true); g_pMSequenceDlg->load(); break;
		case 17:g_pRuler->LoadFile(true); g_pRulerDlg->load(); break;
		case 18:g_pAmblyoscope->LoadFile(true); g_pAmblyoscopeDlg->load(); break;
		case 19:g_pScotomaMap->LoadFile(true); g_pScotomaMapDlg->load(); break;
		case 20:g_pRandomDotAnaglyph->LoadFile(true); g_pRandomDotAnaglyphDlg->load(); break;
		case 21:g_pGratings->LoadFile(true); g_pGratingsDlg->load(); break;
		case 22:g_pTrialSaccades->LoadFile(true); g_pTrialSaccadesDlg->load(); break;
		case 23:g_pFixationStabilityModel->LoadFile(true); g_pFixationStabilityDlg->load(); break;
#endif
#ifdef APP_ACUITY
		case 3:g_pPrefSelection->LoadFile(true); g_pPrefSelectionDlg->load(); break;
		case 4:g_pNoiseGratings->LoadFile(true); g_pNoiseGratingsDlg->load(); break;
#endif
	}

}

/*
* 10/18/2007
* MKF
* Note. Moved some global settings controls to the eyetrackingdlg controls to avoid switching back to tab
* Changed dot and bg colors, stim and screen offsets, and apply button
*/

void CEyeTrackingDlg::OnBnClickedDotcolor()
{
	CHOOSECOLOR cdb;
	ZeroMemory(&cdb, sizeof(cdb));
	cdb.hwndOwner = g_pEyeTrackingDlg->GetSafeHwnd();//HWND()
	cdb.lStructSize = sizeof(cdb);
	cdb.lpCustColors = (LPDWORD) acrCustClr;
	//cdb.rgbResult = RGB(g_pSettings->getRed(), g_pSettings->getGreen(), g_pSettings->getBlue());
	cdb.rgbResult = g_pSettingsDlg->dotRGB;
	cdb.Flags = CC_SOLIDCOLOR | CC_RGBINIT ;//| CC_FULLOPEN;
	if (ChooseColor(&cdb)) {
		g_pSettingsDlg->dotRGB = cdb.rgbResult;
		g_pEyeTrackingDlg->Invalidate();
	}
}

void CEyeTrackingDlg::OnBnClickedBackgroundcolor()
{
	CHOOSECOLOR cdb;
	ZeroMemory(&cdb, sizeof(cdb));
	cdb.hwndOwner = g_pEyeTrackingDlg->GetSafeHwnd();//HWND()
	cdb.lStructSize = sizeof(cdb);
	cdb.lpCustColors = (LPDWORD) acrCustClr;
	//cdb.rgbResult = RGB(g_pSettings->getRedBG(), g_pSettings->getGreenBG(), g_pSettings->getBlueBG());
	cdb.rgbResult = g_pSettingsDlg->bgRGB;
	cdb.Flags = CC_SOLIDCOLOR | CC_RGBINIT ;//| CC_FULLOPEN;
	if (ChooseColor(&cdb)) {
		g_pSettingsDlg->bgRGB = cdb.rgbResult;
		g_pEyeTrackingDlg->Invalidate();
	}
}

void CEyeTrackingDlg::OnDeltaposStimxspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	char buffer[100];
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_StimX.GetWindowText(buffer, 100);
	float val = etu::StringToFloat(buffer, g_pSettings->getXOffset());
	
	//pNMUpDown->iDelta		-1=up	1=down
	if (pNMUpDown->iDelta > 0) {
		m_StimX.SetWindowText(etu::FloatToString(1.0*(float)ceil(1.0*val-1.0)));
	} 
	else {
		m_StimX.SetWindowText(etu::FloatToString(1.0*(float)floor(1.0*val+1.0)));
	}

	*pResult = 0;
}

void CEyeTrackingDlg::OnDeltaposStimyspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	char buffer[100];
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_StimY.GetWindowText(buffer, 100);
	float val = etu::StringToFloat(buffer, g_pSettings->getYOffset());
	
	//pNMUpDown->iDelta		-1=up	1=down
	if (pNMUpDown->iDelta > 0) {
		m_StimY.SetWindowText(etu::FloatToString(1.0*(float)ceil(1.0*val-1.0)));
	} else {
		m_StimY.SetWindowText(etu::FloatToString(1.0*(float)floor(1.0*val+1.0)));
	}

	*pResult = 0;
}

void CEyeTrackingDlg::OnBnClickedApplysettings()
{
	bool bWasActive = false;
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() == "Pursuit Stimulus") {
		bWasActive = true;
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	g_pSettingsDlg->save();
	g_pSettingsDlg->load();
	if (bWasActive) {
		while (g_pActiveStimulus)
			Sleep(100);
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	g_pEyeTrackingDlg->Invalidate();
}

void CEyeTrackingDlg::OnDeltaposScreenxspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	char buffer[100];
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_ScreenX.GetWindowText(buffer, 100);
	float val = etu::StringToFloat(buffer, g_pSettings->getScreenXOffset());
	
	//pNMUpDown->iDelta		-1=up	1=down
	if (pNMUpDown->iDelta > 0) {
		m_ScreenX.SetWindowText(etu::FloatToString(1.0*(float)ceil(1.0*val-1.0)));
	} else {
		m_ScreenX.SetWindowText(etu::FloatToString(1.0*(float)floor(1.0*val+1.0)));
	}

	*pResult = 0;
}

void CEyeTrackingDlg::OnDeltaposScreenyspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	char buffer[100];
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_ScreenY.GetWindowText(buffer, 100);
	float val = etu::StringToFloat(buffer, g_pSettings->getScreenYOffset());
	
	//pNMUpDown->iDelta		-1=up	1=down
	if (pNMUpDown->iDelta > 0) {
		m_ScreenY.SetWindowText(etu::FloatToString(1.0*(float)ceil(1.0*val-1.0)));
	} else {
		m_ScreenY.SetWindowText(etu::FloatToString(1.0*(float)floor(1.0*val+1.0)));
	}

	*pResult = 0;
}

/*
* 10/19/2007
* MKF
* Note. Added shutter controls to the bottom panel.  These should be disabled when controlled
* automatically by the stimulus, but the state should still be updated (just use the functions)
*/

void CEyeTrackingDlg::OnBnClickedShutterl()
{
	// Left Shutter
	//raise or lower
	long state = vsgIOReadDigitalOut() & vsgDIG6;
	if (state > 0) {
		//l shutter is on. turn it off
		OpenLeftShutter(true);
	} else if (state == 0) {
		//l shutter is off. turn it on
		OpenLeftShutter(false);
	} else {
		m_ShutterLState.SetWindowText("Left Shutter: error");
	}
}

void CEyeTrackingDlg::OnBnClickedShutterr()
{
	// Right Shutter
	//raise or lower
	long state = vsgIOReadDigitalOut() & vsgDIG5;
	if (state > 0) {
		//r shutter is on. turn it off
		OpenRightShutter(true);
	} else if (state == 0) {
		//r shutter is off. turn it on
		OpenRightShutter(false);
	} else {
		m_ShutterRState.SetWindowText("Right Shutter: error");
	}
}

void CEyeTrackingDlg::OpenLeftShutter(bool setopen) {
	if (setopen) {
		vsgIOWriteDigitalOut(0x0000, vsgDIG6);
		m_ShutterL.SetWindowText("Lower");
		m_ShutterLState.SetWindowText("Left Shutter: open");
	} else {
		vsgIOWriteDigitalOut(0xFFFF, vsgDIG6);
		m_ShutterL.SetWindowText("Raise");
		m_ShutterLState.SetWindowText("Left Shutter: closed");
	}
}

void CEyeTrackingDlg::OpenRightShutter(bool setopen) {
	if (setopen) {
		vsgIOWriteDigitalOut(0x0000, vsgDIG5);
		m_ShutterR.SetWindowText("Lower");
		m_ShutterRState.SetWindowText("Right Shutter: open");
	} else {
		vsgIOWriteDigitalOut(0xFFFF, vsgDIG5);
		m_ShutterR.SetWindowText("Raise");
		m_ShutterRState.SetWindowText("Right Shutter: closed");
	}
}

void CEyeTrackingDlg::EyePosSample() 
{
	int numSamples = g_pSettings->getNSamples();
	numSamples = (numSamples < 1)? 1 : numSamples;

	if (numSamples > 1) {
		if (lxs == NULL) {
			lxs = (double*)malloc(sizeof(double) * numSamples);
			for (int i = 0; i < numSamples; i++) lxs[i] = 0.0;
			avglx = 0;
			lxn = 0;
		}
		if (lys == NULL) {
			lys = (double*)malloc(sizeof(double) * numSamples);
			for (int i = 0; i < numSamples; i++) lys[i] = 0.0;
			avgly = 0;
			lyn = 0;
		}
		if (rxs == NULL) {
			rxs = (double*)malloc(sizeof(double) * numSamples);
			for (int i = 0; i < numSamples; i++) rxs[i] = 0.0;
			avgrx = 0;
			rxn = 0;
		}
		if (rys == NULL) {
			rys = (double*)malloc(sizeof(double) * numSamples);
			for (int i = 0; i < numSamples; i++) rys[i] = 0.0;
			avgry = 0;
			ryn = 0;
		}
	}

	double aiscale = 0.5;
	double lxdeg = -aiscale * (vsgIOReadADC(0) - 2048.0) / g_pSettings->getDegPerVolt();//this is not in the range +/- 5.12 volts...
	double lydeg = -aiscale * (vsgIOReadADC(1) - 2048.0) / g_pSettings->getDegPerVolt();
	double rxdeg = -aiscale * (vsgIOReadADC(3) - 2048.0) / g_pSettings->getDegPerVolt();
	double rydeg = -aiscale * (vsgIOReadADC(4) - 2048.0) / g_pSettings->getDegPerVolt();

	if ((-90.0 < lxdeg) && (lxdeg < 90.0) && (-90.0 < lydeg) && (lydeg < 90.0)) {
		//left eye sample is ok. update
		if (numSamples == 1) {
			avglx = lxdeg;
			avgly = lydeg;
		}
		else {
			avglx += (lxdeg - lxs[lxn]) / ((double)numSamples);
			avgly += (lydeg - lys[lyn]) / ((double)numSamples);
			lxs[lxn] = lxdeg;
			lys[lyn] = lydeg;
			lxn = (lxn + 1) % numSamples;
			lyn = (lyn + 1) % numSamples;
		}
	}

	
	if ((-90.0 < rxdeg) && (rxdeg < 90.0) && (-90.0 < rydeg) && (rydeg < 90.0)) {
		//right eye sample is ok. update
		if (numSamples == 1) {
			avgrx = rxdeg;
			avgry = rydeg;
		}
		else {
			avgrx += (rxdeg - rxs[rxn]) / ((double)numSamples);
			avgry += (rydeg - rys[ryn]) / ((double)numSamples);
			rxs[rxn] = rxdeg;
			rys[ryn] = rydeg;
			rxn = (rxn + 1) % numSamples;
			ryn = (ryn + 1) % numSamples;
		}
	}
}
