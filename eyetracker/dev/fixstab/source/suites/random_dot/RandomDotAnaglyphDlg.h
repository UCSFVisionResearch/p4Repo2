/**
 * @file RandomDotAnaglyphDlg.h
 * @brief 
 */

#ifndef RANDOMDOTANAGLYPHDLG_H
#define RANDOMDOTANAGLYPHDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CRandomDotAnaglyphDlg : public CDialog
{
	DECLARE_DYNAMIC(CRandomDotAnaglyphDlg)

public:
	CRandomDotAnaglyphDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRandomDotAnaglyphDlg();
	void load();
	void save();

	/** Dialog Data */
	enum { IDD = IDD_ANAGLYPH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonapply();
	CEdit m_Red;
	CEdit m_Blue;
	CEdit m_Text;
	CEdit m_DepthImage;
	CEdit m_BGDisparity;
	CEdit m_Noise;
	CEdit m_TextDisparity;
	afx_msg void OnBnClickedButton1();
	CEdit m_MaxDisparity;
	CButton m_ShowDisparity;
};


#endif // RANDOMDOTANAGLYPHDLG_H
