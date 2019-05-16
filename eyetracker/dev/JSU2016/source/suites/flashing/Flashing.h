#pragma once
#include <afxwin.h>


// CFlashingDlg dialog

class CFlashingDlg : public CDialog
{
	DECLARE_DYNAMIC(CCalibrationDlg)

public:
	CFlashingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFlashingDlg();
	void load();
	void save();
	void setDebugString(CString str);

// Dialog Data
	enum { IDD = IDD_FLASHING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_Size;
	CEdit m_Separation;
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
};