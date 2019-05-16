/**
 * @file RFMappingDlg.h
 * @brief 
 */

#ifndef RFMAPPINGDLG_H
#define RFMAPPINGDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>
#include "app/ReceptiveField.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CRFMappingDlg : public CDialog
{
	DECLARE_DYNAMIC(CRFMappingDlg)

public:
	CRFMappingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRFMappingDlg();

	void load();
	void save();
	void disableControls();
	void enableControls();

	void updateFieldString(CReceptiveField rf, bool bLeft);

	/** Dialog Data */
	enum { IDD = IDD_RF_MAPPING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	CEdit m_XPosition;
	CEdit m_YPosition;
	CEdit m_Width;
	CEdit m_Height;
	CButton m_RadioCircle;
	CButton m_RadioBar;
	CButton m_EnableMouse;
	CButton m_EnablePulse;
	afx_msg void OnBnClickedSaveasleft();
	afx_msg void OnBnClickedSaveasright();
	CStatic m_LeftEyeField;
	CStatic m_RightEyeField;
	afx_msg void OnBnClickedCircle();
	afx_msg void OnBnClickedBar();
	afx_msg void OnBnClickedMousecontrol();
	afx_msg void OnBnClickedPulse();
	afx_msg void OnBnClickedApply();
	CEdit m_BarWidth;
	CEdit m_BarHeight;
	CEdit m_Orientation;
	CEdit m_CircleRadius;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	CPoint lastPoint;
	CPoint currPoint;
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	CButton m_Apply;
	CButton m_SaveAsLeft;
	CButton m_SaveAsRight;
};


#endif // RFMAPPINGDLG_H
