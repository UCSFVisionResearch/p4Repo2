/**
 * @file APDDlg.h
 * @brief 
 */

#ifndef APDDLG_H
#define APDDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CAPDDlg : public CDialog
{
	DECLARE_DYNAMIC(CAPDDlg)

public:
	CAPDDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAPDDlg();
	void load();
	void save();

	/** Dialog Data */
	enum { IDD = IDD_APD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CButton m_ShowFixation;
	afx_msg void OnBnClickedShowFixation();
	CEdit m_FixationPosX;
	CEdit m_FixationPosY;
	CEdit m_FixationRed;
	CEdit m_FixationGreen;
	CEdit m_FixationBlue;
	CEdit m_OnRed;
	CEdit m_OnGreen;
	CEdit m_OnBlue;
	CEdit m_OffRed;
	CEdit m_OffGreen;
	CEdit m_OffBlue;
	CEdit m_OnTime;
	CEdit m_OffTime;
	CButton m_ApplyButton;
	CEdit m_FixationSize;
	CButton m_LeftField;
	CButton m_RightField;
	CButton m_Alternate;
	CButton m_FullScreen;
	afx_msg void OnBnClickedLeftField();
	afx_msg void OnBnClickedRightField();
	afx_msg void OnBnClickedAlternate();
	afx_msg void OnBnClickedFullscreen();
	CEdit m_AlternateDelay;
	CEdit m_FixationDelay;
	CButton m_Behavior;
	CEdit m_FieldWidth;
	CEdit m_FieldHeight;
	afx_msg void OnBnClickedBehavior();
	CEdit m_CenterXLeft;
	CEdit m_CenterYLeft;
	CEdit m_CenterXRight;
	CEdit m_CenterYRight;
};


#endif // APDDLG_H
