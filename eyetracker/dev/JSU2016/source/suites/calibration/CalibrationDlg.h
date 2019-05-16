/**
 * @file CalibrationDlg.h
 * @brief 
 */

#ifndef CALIBRATIONDLG_H
#define CALIBRATIONDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>
#include "app/res/resource.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CCalibrationDlg : public CDialog
{
	DECLARE_DYNAMIC(CCalibrationDlg)

public:
	CCalibrationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCalibrationDlg();
	void load();
	void save();
	void setDebugString(CString str);

	/** Dialog Data */
	enum { IDD = IDD_CALIBRATION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CEdit m_Size;
	CEdit m_HSeparation;
	CEdit m_VSeparation;
	CButton m_DefaultSeparation;
	afx_msg void OnBnClickedDefaultSeparation();
	afx_msg void OnBnClickedButton1();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_Velocity;
	CButton m_ShowAll;
	CComboBox m_EyeList;
	CStatic m_Debug;
	CEdit m_AcceptDelay;
	CButton m_CheckCycle;
	afx_msg void OnBnClickedCheckCycle();
	afx_msg void OnBnClickedCheckShowall();
	CButton m_Dot1;
	CButton m_Dot2;
	CButton m_Dot3;
	CButton m_Dot4;
	CButton m_Dot5;
	CButton m_Dot6;
	CButton m_Dot7;
	CButton m_Dot8;
	CButton m_Dot9;

private:
	double m_oldHSeparation;
	double m_oldVSeparation;
};

#endif // CALIBRATIONDLG_H
