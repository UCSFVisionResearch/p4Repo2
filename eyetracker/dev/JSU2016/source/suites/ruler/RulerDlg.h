/**
 * @file RulerDlg.h
 * @brief 
 */

#ifndef RULERDLG_H
#define RULERDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CRulerDlg : public CDialog
{
	DECLARE_DYNAMIC(CRulerDlg)

public:
	CRulerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRulerDlg();
	void load();
	void save();

    /** Dialog Data */
	enum { IDD = IDD_RULER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonapply();
	CEdit m_NumRings;
	CButton m_Box;
};


#endif // RULERDLG_H
