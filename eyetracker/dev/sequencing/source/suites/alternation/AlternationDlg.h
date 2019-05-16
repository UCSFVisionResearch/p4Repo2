/**
 * @file AlternationDlg.h
 * @brief 
 */

#ifndef ALTERNATIONDLG_H
#define ALTERNATIONDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CAlternationDlg : public CDialog
{
	DECLARE_DYNAMIC(CAlternationDlg)

public:
	CAlternationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAlternationDlg();
	void load();
	void save();

	/** Dialog Data */
	enum { IDD = IDD_ALTERNATION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonapply();

	CEdit m_DotSize1;
	CEdit m_DotSize2;
	CEdit m_XPos1;
	CEdit m_YPos1;
	CEdit m_XPos2;
	CEdit m_YPos2;
	
	CEdit m_Red1;
	CEdit m_Green1;
	CEdit m_Blue1;
	CEdit m_Red2;
	CEdit m_Green2;
	CEdit m_Blue2;
	CEdit m_Diamond;
};


#endif // ALTERNATIONDLG_H
