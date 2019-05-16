/**
 * @file CircleAnnulusDlg.h
 * @brief 
 */

#ifndef CIRCLEANNULUSDLG_H
#define CIRCLEANNULUSDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CCircleAnnulusDlg : public CDialog
{
	DECLARE_DYNAMIC(CCircleAnnulusDlg)

public:
	CCircleAnnulusDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCircleAnnulusDlg();

	void load();
	void save();
	void disableControls();
	void enableControls();
	void disableSecondaryColor();
	void enableSecondaryColor();
	void disableStartPresentations();
	void enableStartPresentations();
	void refreshPrimaryList();
	void refreshSecondaryList();
	void updateDisplayStrings(float radius, float annulusWidth, int repeatNum, int trialsLeft);
	void updateFieldStrings();

	/** Dialog Data */
	enum { IDD = IDD_CIRCLE_ANNULUS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CButton m_RightEyeField;
	CButton m_LeftEyeField;
	CButton m_InterleavedFields;
	CButton m_ManualFields;
	CEdit m_XPosition;
	CEdit m_YPosition;
	CEdit m_TrialDuration;
	CEdit m_BlankPeriod;
	CEdit m_IntertrialDuration;
	CButton m_CirclesOnly;
	CButton m_AnnuliOnly;
	CButton m_CirclesAnnuli;
	CButton m_CheckOnOff;
	CEdit m_RadiusMin;
	CEdit m_RadiusMax;
	CEdit m_RadiusStep;
	CEdit m_AnnulusWidthMin;
	CEdit m_AnnulusWidthMax;
	CEdit m_AnnulusWidthStep;
	CEdit m_PrimaryRed;
	CEdit m_PrimaryGreen;
	CEdit m_PrimaryBlue;
	CListBox m_PrimaryList;
	CEdit m_SecondaryRed;
	CEdit m_SecondaryGreen;
	CEdit m_SecondaryBlue;
	CListBox m_SecondaryList;
	afx_msg void OnBnClickedRighteyefield();
	afx_msg void OnBnClickedLefteyefield();
	afx_msg void OnBnClickedInterleavedfields();
	afx_msg void OnBnClickedManualfields();
	afx_msg void OnBnClickedCirclesonly();
	afx_msg void OnBnClickedAnnulionly();
	afx_msg void OnBnClickedCirclesannuli();
	afx_msg void OnBnClickedCheckonoff();
	afx_msg void OnBnClickedPrimaryadd();
	afx_msg void OnBnClickedPrimaryremove();
	afx_msg void OnBnClickedPrimaryclear();
	afx_msg void OnBnClickedSecondaryadd();
	afx_msg void OnBnClickedSecondaryremove();
	afx_msg void OnBnClickedSecondaryclear();
	afx_msg void OnBnClickedApply();
	CEdit m_Repeats;
	CEdit m_UnitTag;
	CStatic m_StaticRadius;
	CStatic m_StaticAnnulusWidth;
	CStatic m_StaticRepeat;
	CStatic m_StaticTrialsLeft;
	afx_msg void OnBnClickedStartPresentation();
	CButton m_StartPresentations;
	CButton m_SecondaryAdd;
	CButton m_SecondaryRemove;
	CButton m_SecondaryClear;
	CButton m_PrimaryAdd;
	CButton m_PrimaryRemove;
	CButton m_PrimaryClear;
	CStatic m_StaticLeftEyeField;
	CStatic m_StaticRightEyeField;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnStnClickedStaticlefteye();
};


#endif // CIRCLEANNULUSDLG_H
