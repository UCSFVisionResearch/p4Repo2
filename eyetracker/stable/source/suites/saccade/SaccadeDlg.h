/**
 * @file SaccadeDlg.h
 * @brief 
 */

#ifndef SACCADEDLG_H
#define SACCADEDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSaccadeDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaccadeDlg)
	void load();
	void save();

public:
	CSaccadeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSaccadeDlg();

    /** Dialog Data */
	enum { IDD = IDD_SACCADE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	
	// Dot Size Edit Box
	CEdit   m_DotSize;
	CEdit   m_StepSize;
	CEdit   m_NumSteps;
	CEdit   m_NumStepsY;
	CEdit   m_EditDuration;
	CButton m_ReturnToPt;
	CButton m_UseAllDir;
	CButton m_RadioRadial;
	CButton m_RadioXY;
	CEdit   m_PointX;
	CEdit   m_PointY;
	afx_msg void OnBnClickedCheckreturntopt();
	afx_msg void OnBnClickedRadioradial();
	afx_msg void OnBnClickedRadioxy();
	afx_msg void OnBnClickedCheckusealldir();
	afx_msg void OnBnClickedButtonapply();

	CButton m_Horizontal;
	CButton m_Vertical;
	CButton m_45Deg;
	CButton m_135Deg;
	afx_msg void OnBnClickedCheckhorizontal();
	afx_msg void OnBnClickedCheckvertical();
	afx_msg void OnBnClickedCheck45deg();
	afx_msg void OnBnClickedCheck135deg();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	CButton m_RandomDurations;
	CEdit   m_MinDuration;
	CEdit   m_MaxDuration;
	afx_msg void OnBnClickedRandomDurations();

	CButton m_EnableOscillations;
	CEdit   m_OscillationAmplitude;
	CEdit   m_OscillationFreq;
	CButton m_RadioHorizontal;
	CButton m_RadioVertical;
	CButton m_Radio45Deg;
	CButton m_Radio135Deg;
	afx_msg void OnBnClickedRadiohorizontal();
	afx_msg void OnBnClickedRadiovertical();
	afx_msg void OnBnClickedRadio45deg();
	afx_msg void OnBnClickedRadio135deg();
	afx_msg void OnBnClickedEnableoscillations();

	CButton m_Behavior;
	afx_msg void OnBnClickedCheckBehvaior();

	CEdit   m_MaxDisplayDuration;
	CEdit   m_BlankTime;
	CEdit   m_JitterX;
	CEdit   m_JitterY;
	CButton m_AvoidZero;
	afx_msg void OnBnClickedRadioprimary();
	afx_msg void OnBnClickedRadiosecondary();
	afx_msg void OnBnClickedRadioalternate();

	CButton m_RadioPrimary;
	CButton m_RadioSecondary;
	CButton m_RadioAlternate;
	CEdit   m_RedPrimary;
	CEdit   m_GreenPrimary;
	CEdit   m_BluePrimary;
	CEdit   m_RedSecondary;
	CEdit   m_GreenSecondary;
	CEdit   m_BlueSecondary;
	CButton m_RadioRandom;
	afx_msg void OnBnClickedRadiorandom();
	afx_msg void OnEnChangeEditredsec();
	
	void OnPaint();
	CStatic m_PrimaryStatic;
	CStatic m_SecondaryStatic;
	CButton m_GridSteps;

	CButton m_PointDot;
	CButton m_PointLandoltC;
	CButton m_PointE;
	CButton m_SequentialOrder;

	/* Noise handling */
	CButton m_displayNoiseBg;
	CButton m_sameBgTrial;
	CButton m_alternateNoise;
	CButton m_occludeBg;
	CEdit m_noiseRed;
	CEdit m_noiseGreen;
	CEdit m_noiseBlue;
	CEdit m_noisePercent;
	CEdit m_noiseSize;
	CStatic m_noiseStatic;

	afx_msg void OnBnClickedDisplayNoiseBg();
};


#endif // SACCADEDLG_H
