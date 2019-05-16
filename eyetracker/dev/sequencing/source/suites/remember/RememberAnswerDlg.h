#pragma once
#include <afxwin.h>

// CRememberAnswerDlg dialog used for coding responses online to Remember peripheral flash stim

class CRememberAnswerDlg : public CDialog {
public:
	CRememberAnswerDlg();
	afx_msg void CRememberAnswerDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnOkButton();
	void OnCloseButton();
	BOOL OnInitDialog();
	// Dialog Data
	enum { IDD = IDD_REM_ANS_DLG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// Implementation
	DECLARE_MESSAGE_MAP()
};
