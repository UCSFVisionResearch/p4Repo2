/**
 * @file SuppressionDlg.h
 * @brief 
 */

#ifndef SUPPRESSIONDLG_H
#define SUPPRESSIONDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSuppressionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSuppressionDlg)

public:
	CSuppressionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSuppressionDlg();
	void load();
	void save();

	/** Dialog Data */
	enum { IDD = IDD_SUPPRESSION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonapply();
	afx_msg void OnRedScroll();
	afx_msg void OnBlueScroll();
	void CSuppressionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CEdit m_GridSpace;
	CEdit m_GridSize;
	CEdit m_Delay;
	CSliderCtrl m_RedScroll;
	CSliderCtrl m_BlueScroll;
	CStatic m_RedStatic;
	CStatic m_BlueStatic;
	CButton m_FFTest;
	CButton m_MMTest;
	CEdit m_bgStripeLightRed;
	CEdit m_bgStripeLightGreen;
	CEdit m_bgStripeLightBlue;
	CEdit m_bgStripeDarkRed;
	CEdit m_bgStripeDarkGreen;
	CEdit m_bgStripeDarkBlue;
	CEdit m_Delayms;
	CButton m_RedBlock;
	CButton m_BlueBlock;
	CButton m_ShowText;
	CEdit m_StripeHeight;
	CButton m_AdjustBG;
	CButton m_MMSineGratings;
	CEdit m_DotTime;
	CEdit m_DisplayTime;
	CEdit m_OffTime;
	CEdit m_SpatialFreq;
	CButton m_RBUpDown;
	CButton m_RBRandom;
	CButton m_RBSlider;
	CButton m_MMMG;
	CEdit m_NumGratings;
	CButton m_MultipleFullBlue;
};


#endif // SUPPRESSIONDLG_H
