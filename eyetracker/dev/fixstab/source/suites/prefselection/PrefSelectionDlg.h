/**
 * @file PrefSelectionDlg.h
 * @brief 
 */

#ifndef PREFSELECTIONDLG_H
#define PREFSELECTIONDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CPrefSelectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CPrefSelectionDlg)

public:
	CPrefSelectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPrefSelectionDlg();
	void load();
	void save();

	void setCurrParameters(float spatialFreq, float contrast, int iSide, int numReversals, float currStep);

	/** Dialog Data */
	enum { IDD = IDD_PREF_SEL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_TimeOut;
	CEdit m_SpatialFreq;
	afx_msg void OnBnClickedButtonapply();
	CStatic m_CurrFreqText;
	CStatic m_CurrContrastText;
	CEdit m_CoarseStep;
	CEdit m_FineStep;
	CEdit m_NumReversals;
	CEdit m_NUp;
	CEdit m_NDown;
	CEdit m_GratingHeight;
	CEdit m_GratingWidth;
	CEdit m_DotPosition;
	CEdit m_GratingPosition;
	CEdit m_MinContrast;
	CEdit m_MaxContrast;
	CStatic m_CurrPosition;
	CEdit m_MedStep;
	CEdit m_InitialContrastList;
	CEdit m_CorrectTimeOut;
	afx_msg void OnBnClickedIncreasestep();
	CStatic m_ReversalsCompleted;
	afx_msg void OnBnClickedDecreasestep();
	CEdit m_UberFineStep;
	CStatic m_CurStepSize;
	CEdit m_DotSize;
	CEdit m_TargetDelay;

	CButton m_TrainingMode;
	CEdit m_PctTrialsRight;
};


#endif // PREFSELECTIONDLG_H
