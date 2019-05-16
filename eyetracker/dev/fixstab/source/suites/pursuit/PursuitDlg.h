/**
 * @file PursuitDlg.h
 * @brief 
 */

#ifndef PURSUITDLG_H
#define PURSUITDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CPursuitDlg : public CDialog
{
	DECLARE_DYNAMIC(CPursuitDlg)

public:
	CPursuitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPursuitDlg();
	void load();
	void save();

	/** Dialog Data */
	enum { IDD = IDD_PURSUIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CEdit m_Amplitude;
	CEdit m_Frequency;
	CEdit m_Size;
	CButton m_RadioHorizontal;
	CButton m_RadioVertical;
	CButton m_Radio45Deg;
	CButton m_Radio135Deg;
	CButton m_RadioSine;
	CButton m_RadioTriangle;
	afx_msg void OnBnClickedRadiohorizontal();
	afx_msg void OnBnClickedRadio45deg();
	afx_msg void OnBnClickedRadiovertical();
	afx_msg void OnBnClickedRadio135deg();
	afx_msg void OnBnClickedRadiosinewave();
	afx_msg void OnBnClickedRadiotriangle();
	afx_msg void OnBnClickedButtonapply();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	CButton m_EnableHighFreq;
	CEdit m_HighFreqAmplitude;
	
	CEdit m_HighFreq;
	afx_msg void OnBnClickedEnablehighfreq();
	CButton m_useString;
	CEdit m_TextString;
	CEdit m_Eccentricity;
	CEdit m_ChangeR;
	CEdit m_ChangeG;
	CEdit m_ChangeB;
	CButton m_Alternating;
};


#endif // PURSUITDLG_H
