/**
 * @file StepRampDlg.h
 * @brief 
 */

#ifndef STEPRAMPDLG_H
#define STEPRAMPDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

/** Step Ramp dialog */
class CStepRampDlg : public CDialog
{
	DECLARE_DYNAMIC(CStepRampDlg)

public:
	CStepRampDlg(CWnd* pParent = NULL);   /* default constructor */
	virtual ~CStepRampDlg();
	void load();
	void save();

    /** Dialog data. */
	enum { IDD = IDD_STEPRAMP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CEdit m_DotSize;
	CEdit m_StepSize;
	CEdit m_Velocity;
	CEdit m_Excursion;
	CEdit m_Delay;
	afx_msg void OnBnClickedButton1();
	CEdit m_Plateau;
	CEdit m_OffDelay;
	CButton m_Apply;
	CButton m_CheckUp;
	CButton m_CheckDown;
	CButton m_CheckLeft;
	CButton m_CheckRight;
	afx_msg void OnBnClickedCheckup();
	afx_msg void OnBnClickedCheckleft();
	afx_msg void OnBnClickedCheckdown();
	afx_msg void OnBnClickedCheckright();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonall();
	afx_msg void OnBnClickedButtonnone();
	CButton m_Behavior;
	afx_msg void OnBnClickedCheckBehavior();
	CEdit m_StartXUp;
	CEdit m_StartYUp;
	CEdit m_StartXDown;
	CEdit m_StartYDown;
	CEdit m_StartXLeft;
	CEdit m_StartYLeft;
	CEdit m_StartXRight;
	CEdit m_StartYRight;
};

#endif // STEPRAMPDLG_H
