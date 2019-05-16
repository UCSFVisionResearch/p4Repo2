/**
 * @file IsoluminantPatchDlg.h
 * @brief 
 */

#ifndef ISOLUMINANTPATCHDLG_H
#define ISOLUMINANTPATCHDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CIsoluminantPatchDlg : public CDialog
{
	DECLARE_DYNAMIC(CIsoluminantPatchDlg)

public:
	CIsoluminantPatchDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIsoluminantPatchDlg();

	/** Dialog Data */
	enum { IDD = IDD_ISOLUMINANT_PATCH };
	void load();
	void save();
	void disableControls();
	void enableControls();
	void UpdateStatus(int numRpts, int numUniques);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_FileName;
	CButton m_Apply;
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedBrowse();
	CEdit m_VoltageRange;
	CEdit m_VoltageOffset;
	CButton m_Browse;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CButton m_HighestPriority;
	CButton m_LeftHemiField;
	afx_msg void OnBnClickedRadioleft();
	CButton m_RightHemiField;
	afx_msg void OnBnClickedRadioright();
	CEdit m_FixationSize;
	CStatic m_StimData;
	CEdit m_FixationX;
	CEdit m_FixationY;
	CEdit m_MeanLuminance;
	CEdit m_NumRepeats;
	CButton m_RadioFNoise;
	CButton m_RadioOther;
	afx_msg void OnBnClickedRadioFnoise();
	afx_msg void OnBnClickedRadioNonFnoise();
	CEdit m_FixationX2;
	CEdit m_FixationY2;
	CButton m_RepeatsOnly;
	CEdit m_MinLuminance;
	CEdit m_MaxLuminance;
};


#endif // ISOLUMINANTPATCHDLG_H
