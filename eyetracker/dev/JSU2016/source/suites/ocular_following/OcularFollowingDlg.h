/**
 * @file OcularFollowingDlg.h
 * @brief 
 */

#ifndef OCULARFOLLOWINGDLG_H
#define OCULARFOLLOWINGDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/


class COcularFollowingDlg : public CDialog
{
	DECLARE_DYNAMIC(COcularFollowingDlg)

public:
	COcularFollowingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COcularFollowingDlg();
	void load();
	void save();

	/** Dialog Data */
	enum { IDD = IDD_OCULAR_FOLLOWING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_BkgSpeed;
	CEdit m_Latency;
	CEdit m_Bkg_StepDuration;
	CEdit m_TimeOut;
	CEdit m_FixationXPos;
	CEdit m_FixationYPos;
	CEdit m_FixationSize;
	afx_msg void OnBnClickedApply();
	CButton m_CheckUp;
	CButton m_CheckDown;
	CButton m_CheckLeft;
	CButton m_CheckRight;
	afx_msg void OnBnClickedButtonall();
	afx_msg void OnBnClickedButtonnone();
	CButton m_Behavior;
	CEdit m_MinRadius;
	CEdit m_MaxRadius;
	CButton m_Random;
	CEdit m_NoiseSize;
	CEdit m_NoisePct;
	CEdit m_NoiseContrast;
};


#endif // OCULARFOLLOWINGDLG_H
