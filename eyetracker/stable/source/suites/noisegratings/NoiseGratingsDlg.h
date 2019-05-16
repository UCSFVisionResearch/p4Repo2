/**
 * @file NoiseGratingsDlg.h
 * @brief 
 */

#ifndef NOISEGRATINGSDLG_H
#define NOISEGRATINGSDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/


class CNoiseGratingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CNoiseGratingsDlg)

public:
	CNoiseGratingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNoiseGratingsDlg();

	void load();
	void save();

	/** Dialog Data */
	enum { IDD = IDD_NOISE_GRATING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();
	CEdit m_NoiseMean;
	CEdit m_NoiseAmplitude;
	CComboBox m_NoiseType;
	CButton m_UseMask;
	CEdit m_MaskRadius;
	CComboBox m_MaskBlending;
	CButton m_UseMouseControl;	
	CEdit m_XPosition;
	CEdit m_YPosition;
	CButton m_AutoFreq;
	CButton m_AutoContrast;
	CButton m_AutoOrientation;
	CButton m_AutoWidth;
	CButton m_AutoHeight;
	CButton m_AutoPhase;
	CEdit m_FreqMin;
	CEdit m_FreqMax;
	CEdit m_FreqStep;
	CEdit m_FreqSetPt;
	CEdit m_ContrastMin;
	CEdit m_ContrastMax;
	CEdit m_ContrastStep;
	CEdit m_ContrastSetPt;
	CEdit m_OrientationMin;
	CEdit m_OrientationMax;
	CEdit m_OrientationStep;
	CEdit m_OrientationSetPt;
	CEdit m_WidthMin;
	CEdit m_WidthMax;
	CEdit m_WidthStep;
	CEdit m_WidthSetPt;
	CEdit m_HeightMin;
	CEdit m_HeightMax;
	CEdit m_HeightStep;
	CEdit m_HeightSetPt;
	CEdit m_PhaseMin;
	CEdit m_PhaseMax;
	CEdit m_PhaseStep;
	CEdit m_PhaseSetPt;
	afx_msg void OnBnClickedUsemask();
	CEdit m_AutomationSpeed;
	CButton m_Radio_SineWave;
	CButton m_Radio_SquareWave;
	
	afx_msg void OnBnClickedUsemousecontrol();
	afx_msg void OnBnClickedRadioGratingsinewave();
	afx_msg void OnBnClickedRadioGratingsquarewave();
	afx_msg void OnBnClickedCheckAutofreq();
	afx_msg void OnBnClickedCheckAutocontrast();
	afx_msg void OnBnClickedCheckAutoorientation();
	afx_msg void OnBnClickedCheckAutowidth();
	afx_msg void OnBnClickedCheckAutoheight();
	afx_msg void OnBnClickedCheckAutophase();
};


#endif // NOISEGRATINGSDLG_H
