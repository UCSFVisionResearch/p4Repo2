/**
 * @file FullfieldDlg.h
 * @brief 
 */

#ifndef FULLFIELDDLG_H
#define FULLFIELDDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CFullfieldDlg : public CDialog
{
	DECLARE_DYNAMIC(CFullfieldDlg)

public:
	CFullfieldDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFullfieldDlg();
	void load();
	void save();
	void ActivateMotionButton();
	void ActivateApplyButton(bool bActivate);

	/** Dialog Data */
	enum { IDD = IDD_FULLFIELD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CEdit m_BarWidth;
	CEdit m_DriftVelocity;
	CButton m_Up;
	CButton m_Down;
	CButton m_Left;
	CButton m_Right;
	CButton m_CheckFixation;
	CEdit m_FixationSize;
	CEdit m_FixationRed;
	CEdit m_FixationGreen;
	CEdit m_FixationBlue;
	afx_msg void OnBnClickedRadioup();
	afx_msg void OnBnClickedRadioleft();
	afx_msg void OnBnClickedRadiodown();
	afx_msg void OnBnClickedRadioright();
	afx_msg void OnBnClickedButtonapply();
	afx_msg void OnBnClickedCheckfixation();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_FixationXPos;
	CEdit m_FixationYPos;
	CEdit m_Duration;
	CButton m_Motion;
	afx_msg void OnBnClickedButtonstartmotion();
	CButton m_Apply;
	CButton m_Noise;
	CButton m_UseGlobalColors;
	CEdit m_Contrast;
};


#endif // FULLFIELDDLG_H
