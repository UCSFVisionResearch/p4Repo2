/**
 * @file SettingsDlg.h
 * @brief 
 */

#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>
#include <afxcmn.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

/** Settings dialog. */
class CSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSettingsDlg();
	void load();
	void save();

    /** Dialog data. */
	enum { IDD = IDD_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CEdit m_Distance;
	CEdit m_XOffset;
	CEdit m_YOffset;
	CEdit m_Red;
	CEdit m_Green;
	CEdit m_Blue;
	CEdit m_RedBG;
	CEdit m_GreenBG;
	CEdit m_BlueBG;
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_DegPerVolt;
	CEdit m_ScreenXOffset;
	CEdit m_ScreenYOffset;
	CEdit m_EyePosition;
	CEdit m_ScreenPosition;
	CStatic m_DotStatic;
	CStatic m_BackgroundStatic;
	void OnPaint();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CEdit m_PixelSize;
	CSpinButtonCtrl m_XOffsetSpin;
	CSpinButtonCtrl m_YOffsetSpin;
	CSpinButtonCtrl m_ScreenXOffsetSpin;
	CSpinButtonCtrl m_ScreenYOffsetSpin;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	DWORD dotRGB;
	DWORD bgRGB;
	CEdit m_NSamples;
};


#endif // SETTINGSDLG_H
