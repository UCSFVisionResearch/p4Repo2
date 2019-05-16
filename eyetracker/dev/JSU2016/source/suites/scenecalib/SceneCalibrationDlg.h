/**
 * @file SceneCalibrationDlg.h
 * @brief 
 */

#ifndef SCENECALIBRATIONDLG_H
#define SCENECALIBRATIONDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSceneCalibrationDlg : public CDialog
{
	DECLARE_DYNAMIC(CSceneCalibrationDlg)

public:
	CSceneCalibrationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSceneCalibrationDlg();
	void load();
	void save();

    /** Dialog data */
	enum { IDD = IDD_SCENE_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CEdit m_ImagePath;
	CEdit m_TargetXPosition;
	CEdit m_TargetYPosition;
	CButton m_Apply;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBrowse();
	CButton m_SwapRedBlue;
};


#endif // SCENECALIBRATIONDLG_H
