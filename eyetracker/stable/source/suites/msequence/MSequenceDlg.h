/**
 * @file MSequenceDlg.h
 * @brief 
 */

#ifndef MSEQUENCEDLG_H
#define MSEQUENCEDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CMSequenceDlg : public CDialog
{
	DECLARE_DYNAMIC(CMSequenceDlg)

public:
	CMSequenceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMSequenceDlg();
	void load();
	void save();

	/** Dialog Data */
	enum { IDD = IDD_MSEQUENCE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonapply();
	
	CEdit m_Width;
	CEdit m_Order;
	CEdit m_Tap;
	CEdit m_Delay;
	CEdit m_NumBars;
	CEdit m_Height;
	CEdit m_HCenter;
	CEdit m_VCenter;
	CStatic m_CurrentOffset;
	CButton m_Textmarks;
	CEdit m_Order2;
	CEdit m_Tap2;
	CEdit m_Order3;
	CEdit m_Tap3;
	CEdit m_RedR;
	CEdit m_RedG;
	CEdit m_RedB;
	CEdit m_GreenR;
	CEdit m_GreenG;
	CEdit m_GreenB;
	CEdit m_BlueR;
	CEdit m_BlueG;
	CEdit m_BlueB;
};


#endif // MSEQUENCEDLG_H
