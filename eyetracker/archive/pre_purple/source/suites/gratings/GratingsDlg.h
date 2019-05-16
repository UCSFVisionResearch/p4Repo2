/**
 * @file GratingsDlg.h
 * @brief 
 */

#ifndef GRATINGSDLG_H
#define GRATINGSDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CGratingsDlg  : public CDialog
{
	DECLARE_DYNAMIC(CGratingsDlg )

public:
	CGratingsDlg (CWnd* pParent = NULL);   // standard constructor
	virtual ~CGratingsDlg ();
	void load();
	void save();
	void UpdateEyePositions(double lxDeg, double lyDeg, double rxDeg, 
		double ryDeg);

	/** Dialog Data */
	enum { IDD = IDD_GRATINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonapply();

	CEdit m_YLocL;
	CEdit m_XLocL;
	CEdit m_YLocR;
	CEdit m_XLocR;
	CEdit m_DotSize;
	CEdit m_GratingWidth;
	CButton m_PhaseRandom;
	CButton m_PhaseStatic;
	CButton m_PhaseRotate;
	CButton m_TypeSquare;
	CButton m_TypeSine;
	CButton m_TypeGabor;
	CButton m_AngleRandom;
	CButton m_AngleStatic;
	CButton m_AngleRotate;
	CEdit m_NumAngles;
	CEdit m_GaborSD;
	CEdit m_StaticPhase;
	CEdit m_StaticAngle;
	CEdit m_Delay;
	CEdit m_Blank;
	CEdit m_NumPhases;
	CEdit m_NumRepeats;
	CButton m_LeftEye;
	CButton m_RightEye;
	CButton m_Interleaved;
	CButton m_BothEyes;
	CStatic m_PresentationCounter;
	CEdit m_UnitTag;
	CButton m_Behavior;
	CEdit m_Initiation;
	CEdit m_Reward;
	CButton m_UseSaccades;
	CEdit m_HSteps;
	CEdit m_VSteps;
	CEdit m_StepSize;
	CEdit m_GratingsPerSaccade;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CStatic m_LightPic;
	CStatic m_DarkPic;
	CStatic m_LightText;
	CStatic m_DarkText;
	DWORD darkRGB;
	DWORD lightRGB;
	afx_msg void OnPaint();
	CButton m_ShutterManual;
	CButton m_ShutterInterleaved;
	CButton m_ShutterRandom;
	CEdit m_ShutterInterleavedN;
	CButton m_AutoLR;
	CButton m_AutoLB;
	CButton m_AutoRB;
	CButton m_AutoLRB;
	CEdit m_ShutterDelay;
	CEdit m_RFX;
	CEdit m_RFY;
	CEdit m_DevX;
	CEdit m_DevY;
	CEdit m_Speed;
	CEdit m_FinalSize;
	CEdit m_InitialSize;
	CStatic m_RFL;
	CStatic m_RFR;
	CButton m_TypeAutoRFMap;
	CButton m_RFMapR;
	CButton m_RFMapL;

	CStatic m_EyePosnDisplayLX;
	CStatic m_EyePosnDisplayLY;
	CStatic m_EyePosnDisplayRX;
	CStatic m_EyePosnDisplayRY;
};


#endif // GRATINGSDLG_H
