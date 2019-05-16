/**
 * @file EyeTrackingDlg.h
 * @brief 
 */

#ifndef EYETRACKINGDLG_H
#define EYETRACKINGDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxcmn.h>
#include <afxwin.h>

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

class CGenericStimulus;
UINT __cdecl CalibrationControllingFunction( LPVOID pParam );
UINT __cdecl PursuitControllingFunction( LPVOID pParam );

#ifdef APP_EYE_TRACKING
UINT __cdecl TrialSaccadesControllingFunction( LPVOID pParam );
UINT __cdecl SaccadeControllingFunction( LPVOID pParam );
UINT __cdecl FullfieldControllingFunction( LPVOID pParam );
UINT __cdecl StepRampControllingFunction( LPVOID pParam );
UINT __cdecl SceneCalibrationControllingFunction(LPVOID pParam);
UINT __cdecl OcularFollowingControllingFunction(LPVOID pParam);
UINT __cdecl IsoluminantPatchControllingFunction(LPVOID pParam);
UINT __cdecl RFMappingControllingFunction(LPVOID pParam);
UINT __cdecl OrientationTuningControllingFunction(LPVOID pParam);
UINT __cdecl CircleAnnulusControllingFunction(LPVOID pParam);
UINT __cdecl APDControllingFunction(LPVOID pParam);
UINT __cdecl RememberControllingFunction(LPVOID pParam);
UINT __cdecl SuppressionControllingFunction(LPVOID pParam);
UINT __cdecl AlternationControllingFunction(LPVOID pParam);
UINT __cdecl MSequenceControllingFunction(LPVOID pParam);
UINT __cdecl RulerControllingFunction(LPVOID pParam);
UINT __cdecl AmblyoscopeControllingFunction(LPVOID pParam);
UINT __cdecl ScotomaMapControllingFunction(LPVOID pParam);
UINT __cdecl RandomDotAnaglyphControllingFunction(LPVOID pParam);
UINT __cdecl GratingsControllingFunction(LPVOID pParam);
UINT __cdecl FixationStabilityControllingFunction(LPVOID pParam);
UINT __cdecl ReverseFixationControllingFunction(LPVOID pParam);
UINT __cdecl SparseNoiseControllingFunction(LPVOID pParam);
#endif

#ifdef APP_ACUITY
UINT __cdecl PrefSelectionControllingFunction(LPVOID pParam);
UINT __cdecl NoiseGratingsControllingFunction(LPVOID pParam);
#endif

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CEyeTrackingDlg : public CDialog
{
public:
	CEyeTrackingDlg(CWnd* pParent = NULL);	// standard constructor
	void AddTabs();
	
	CWinThread* pActiveThread;

    /** Dialog Data */
	enum { IDD = IDD_EYETRACKING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	/** Tab Control Object */
	CTabCtrl m_pTabCtrl;
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingTabs(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedRun();
	CButton m_Run;
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedLoadsettings();
	afx_msg void OnBnClickedSavesettings();
	afx_msg void OnBnClickedMakedefault();
	afx_msg void OnBnClickedLoaddefault();
	CButton m_LoadSettings;
	CButton m_SaveSettings;
	CButton m_MakeDefault;
	CButton m_LoadDefault;
	CButton m_Exit;
	CButton m_DotColor;
	afx_msg void OnBnClickedDotcolor();
	CStatic m_DotPic;
	CStatic m_DotText;
	CButton m_BackgroundColor;
	CStatic m_BackgroundText;
	CStatic m_BackgroundPic;
	afx_msg void OnBnClickedBackgroundcolor();

	COLORREF acrCustClr[16]; // array of custom colors
	CEdit m_StimX;
	CEdit m_StimY;
	CSpinButtonCtrl m_StimXSpin;
	CSpinButtonCtrl m_StimYSpin;
	CStatic m_StimXStatic;
	CStatic m_StimYStatic;
	afx_msg void OnDeltaposStimxspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposStimyspin(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_ApplySettings;
	afx_msg void OnBnClickedApplysettings();
	CStatic m_ScreenXStatic;
	CStatic m_ScreenYStatic;
	CEdit m_ScreenX;
	CEdit m_ScreenY;
	CSpinButtonCtrl m_ScreenXSpin;
	CSpinButtonCtrl m_ScreenYSpin;
	afx_msg void OnDeltaposScreenxspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposScreenyspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedShutterl();
	afx_msg void OnBnClickedShutterr();
	afx_msg void OnBnClickedReverseShutters();
	afx_msg void OpenLeftShutter(bool setopen);
	afx_msg void OpenRightShutter(bool setopen);
	CButton m_ShutterL;
	CButton m_ShutterR;
	CButton m_ReverseShutters;
	CStatic m_ShutterRState;
	CStatic m_ShutterLState;

	double *lxs, *lys, *rxs, *rys;
	int lxn, lyn, rxn, ryn;
	double avglx, avgly, avgrx, avgry;
	afx_msg void EyePosSample();
};


#endif // EYETRACKINGDLG_H
