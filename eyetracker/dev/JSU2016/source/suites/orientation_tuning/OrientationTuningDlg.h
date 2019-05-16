/**
 * @file OrientationTuningDlg.h
 * @brief 
 */

#ifndef ORIENTATIONTUNINGDLG_H
#define ORIENTATIONTUNINGDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class COrientationTuningDlg : public CDialog
{
	DECLARE_DYNAMIC(COrientationTuningDlg)

public:
	COrientationTuningDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COrientationTuningDlg();
	void load();
	void save();
	void updateDisplayStrings(float orientation, int repeat, int numLeft, float circleRadius, float annulusWidth);
	void disableControls(); 
	void enableControls();
	void refreshColorList();
	void refreshSecondaryColorList();
	void enableOrientationTuning();
	void disableOrientationTuning();
	void enableCircleAnnulus();
	void disableCircleAnnulus();
	void disableApply();
	void enableApply();
	void disableStartPresentations();
	void enableStartPresentations();
	void updateFieldStrings();

	/** Dialog Data */
	enum { IDD = IDD_ORIENTATION_TUNING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedStartPresentation();
	CEdit m_BarWidth;
	CEdit m_BarHeight;
	CEdit m_AngularStep;
	CEdit m_MotionSpeed;
	CEdit m_IntertrialTime;
	CEdit m_TrialDuration;
	CEdit m_nRepeats;
	CEdit m_UnitTag;
	CStatic m_StaticOrientation;
	CStatic m_StaticRepeatNumber;
	CStatic m_StaticTrialsLeft;
	
	CButton m_StartPresentations;
	CEdit m_BlankPeriod;
	CButton m_Apply;
	CStatic m_StaticLeftEyeField;
	CStatic m_StaticRightEyeField;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CButton m_RightEyeField;
	CButton m_LeftEyeField;
	CButton m_InterleavedFields;
	CButton m_ManualFields;
	CEdit m_XPosition;
	CEdit m_YPosition;
	CEdit m_StartAngle;
	CEdit m_EndAngle;
	afx_msg void OnBnClickedRighteyefield();
	afx_msg void OnBnClickedLefteyefield();
	afx_msg void OnBnClickedInterleavedfields();
	afx_msg void OnBnClickedManualfields();
	CEdit m_FixX;
	CEdit m_FixY;
	CEdit m_FixD;
	CEdit m_FixColorR;
	CEdit m_FixColorG;
	CEdit m_FixColorB;
	CButton m_UseFixation;
};


#endif // ORIENTATIONTUNINGDLG_H
