/**
 * @file CalibrationDlg.h
 * @brief 
 */

#ifndef REVERSEFIXATIONDLG_H
#define REVERSEFIXATIONDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>
#include "app/res/resource.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CReverseFixationDlg : public CDialog
{
	DECLARE_DYNAMIC(CReverseFixationDlg)

public:
	CReverseFixationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReverseFixationDlg();
	void load();
	void save();
	void printDebug(LPCTSTR text);
	void UpdateEyePositions(double lxDeg, double lyDeg, double rxDeg, double ryDeg);

	/** Dialog Data */
	enum { IDD = IDD_REVERSEFIX_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void OnPaint();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();

	afx_msg void OnBnClickedInitLeft();
	afx_msg void OnBnClickedInitRight();
	afx_msg void OnBnClickedModeInitLoop();
	afx_msg void OnBnClickedModeFoveaLoop();
	afx_msg void OnBnClickedModeTest();


	void PaintColorStatics();

	CEdit m_PrimaryRed;
	CEdit m_PrimaryGreen;
	CEdit m_PrimaryBlue;
	CStatic m_PrimaryStatic;

	CEdit m_SecondaryRed;
	CEdit m_SecondaryGreen;
	CEdit m_SecondaryBlue;
	CStatic m_SecondaryStatic;

	CButton m_initLeft;
	CButton m_initRight;
	CEdit m_initLeftX;
	CEdit m_initLeftY;
	CEdit m_initRightX;
	CEdit m_initRightY;

	CEdit m_dotSize;
	CEdit m_displayTime;
	CButton m_modeFoveaLoop;
	CButton m_modeInitLoop;
	CButton m_modeTest;

	CStatic m_EyePosnDisplayLX;
	CStatic m_EyePosnDisplayLY;
	CStatic m_EyePosnDisplayRX;
	CStatic m_EyePosnDisplayRY;

	CEdit m_WindowAllowancePix;
	CButton m_Wait;

	CButton m_Apply;
	CStatic m_debug;

private:
	CButton m_modeFovea;
};

#endif // REVERSEFIXATIONDLG_H
