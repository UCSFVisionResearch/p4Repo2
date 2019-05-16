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

	afx_msg void OnBnClickedSaveasleft();
	afx_msg void OnBnClickedSaveasright();
	afx_msg void OnBnClickedCircle();
	afx_msg void OnBnClickedBar();
	afx_msg void OnBnClickedMousecontrol();
	afx_msg void OnBnClickedPulse();
	afx_msg void OnBnClickedApply();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	
	CEdit m_rfmCrossX;
	CEdit m_rfmCrossY;
	CEdit m_rfmCrossDiameter;
	CEdit m_rfmCrossThickness;

	CButton m_RadioCircle;
	CButton m_RadioBar;
	CEdit m_CircleRadius;
	CEdit m_BarWidth;
	CEdit m_BarHeight;
	CEdit m_Orientation;
	CButton m_EnableMouse;
	CButton m_EnablePulse;

	CEdit m_XPosition;
	CEdit m_YPosition;
	CEdit m_Width;
	CEdit m_Height;
	
	CStatic m_LeftEyeField;
	CStatic m_RightEyeField;

	CButton m_SaveAsLeft;
	CButton m_SaveAsRight;
	CButton m_Apply;
	
	CPoint lastPoint;
	CPoint currPoint;
};


#endif // RFMAPPINGDLG_H
