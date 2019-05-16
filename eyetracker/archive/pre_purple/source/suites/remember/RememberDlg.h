/**
 * @file RememberDlg.h
 * @brief 
 */

#ifndef REMEMBERDLG_H
#define REMEMBERDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CRememberDlg : public CDialog
{
	DECLARE_DYNAMIC(CRememberDlg)
public:
	CRememberDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRememberDlg();
	void load();
	void save();
	int response;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	/** Dialog Data */
	enum { IDD = IDD_REMEMBER_DLG };

	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonapply();

	CEdit m_DotSize;
	CEdit m_CenterXPos;
	CEdit m_CenterYPos;
	afx_msg void OnBnClickedBehavior();
	CEdit m_StepSize;
	CEdit m_NumStepsH;
	CEdit m_NumStepsV;
	CEdit m_PeriphDotSize;
	CEdit m_FlashInterval;
	CEdit m_FlashDuration;
	CButton m_BehaviorCheck;
	CButton m_AvoidZero;
	CButton m_CenterRadioPrimary;
	CButton m_CenterRadioSecondary;
	CButton m_CenterRadioAlternate;
	CButton m_CenterRadioRandomize;
	CEdit m_CenterPrimaryRed;
	CEdit m_CenterPrimaryGreen;
	CEdit m_CenterPrimaryBlue;
	CEdit m_CenterSecondaryRed;
	CEdit m_CenterSecondaryGreen;
	CEdit m_CenterSecondaryBlue;
	CButton m_PeriphRadioPrimary;
	CButton m_PeriphRadioSecondary;
	CButton m_PeriphRadioAlternate;
	CButton m_PeriphRadioTertiary;
	CButton m_PeriphRadioRandomize;
	CEdit m_PeriphPrimaryRed;
	CEdit m_PeriphPrimaryGreen;
	CEdit m_PeriphPrimaryBlue;
	CEdit m_PeriphSecondaryRed;
	CEdit m_PeriphSecondaryGreen;
	CEdit m_PeriphSecondaryBlue;
	afx_msg void OnBnClickedCenterradioprimary();
	afx_msg void OnBnClickedCenterradiosecondary();
	afx_msg void OnBnClickedCenterradioalternate();
	afx_msg void OnBnClickedCenterradiorandomize();
	afx_msg void OnBnClickedPeriphradioprimary();
	afx_msg void OnBnClickedPeriphradiosecondary();
	afx_msg void OnBnClickedPeriphradioalternate();
	afx_msg void OnBnClickedPeriphradiorandomize();
	int m_CenterRadio;
	int m_PeriphRadio;
	afx_msg void OnBnClickedButton1();
	CButton m_StepHoriz;
	CButton m_StepVert;
	CButton m_Step45;
	CButton m_Step135;
	int m_DistType;
	CButton m_DistRadial;
	CButton m_DistXY;
	afx_msg void OnBnClickedDistradial();
	afx_msg void OnBnClickedDistxy();
	CEdit m_AlternateBlockSize;
	CEdit m_MaxDelay;
	
	CButton m_MemoryGuided;
	CButton m_PeripheralFlash;
	CEdit m_PostFlashDelay;
	CEdit m_IntertrialDelay;
	afx_msg void OnBnClickedMemoryGuided();
	afx_msg void OnBnClickedPeripheralFlash();
	CButton m_RedBlue;
	afx_msg void OnBnClickedRedblue();
	CButton m_RandomizePostFlash;
	CEdit m_PostFlashRandMin;
	CEdit m_PostFlashRandMax;
	CButton m_RandomizeFlashInterval;
	CEdit m_FlashIntervalRandMin;
	CEdit m_FlashIntervalRandMax;
	afx_msg void OnBnClickedRandomizepostflash();
	afx_msg void OnBnClickedRandomizeflashinterval();
	CStatic m_CenterPrimaryStatic;
	CStatic m_CenterSecondaryStatic;
	CStatic m_PeriphPrimaryStatic;
	CStatic m_PeriphSecondaryStatic;
	void OnPaint();
	CEdit m_PeriphXPos;
	CEdit m_PeriphYPos;
	CEdit m_PeriphXRadius;
	CEdit m_PeriphYRadius;
	CEdit m_PctTimeAtPeriph;
	CButton m_CheckSeqRandom;
	CEdit m_NumRandomRepeats;
	CStatic m_Countdown;
	CButton m_UseNoise;
	CEdit m_NoiseRed;
	CEdit m_NoiseGreen;
	CEdit m_NoiseBlue;
	CStatic m_NoiseStatic;
	CEdit m_PeriphTertiaryRed;
	CEdit m_PeriphTertiaryGreen;
	CEdit m_PeriphTertiaryBlue;
	CStatic m_PeriphTertiaryStatic;
	afx_msg void OnBnClickedPeriphradiotertiary();
	CEdit m_NumCatchTrials;
	CButton m_GridSteps;
	CEdit m_NoiseSize;
	CEdit m_CrossPix;
	CEdit m_NumBlankTrials;
	CEdit m_NoisePct;
	afx_msg void OnBnClickedCheckSeqRandom();
	afx_msg void OnBnClickedUsenoise();
	CButton m_UseDim;
	CEdit m_DimOnsetMin;
	CEdit m_DimOnsetMax;
	CEdit m_DimTimeout;
	CEdit m_AnswerPeriod;
	afx_msg void OnBnClickedUsedim();
	CEdit m_DimBrightness;
	CButton m_HideCenter;
	CButton m_InterleaveNoise;
	CButton m_CenterPoints;
	CButton m_Pause;
	afx_msg void OnBnClickedButton2();
	CButton m_OffAxis;
	CButton m_Response;
	CEdit m_ResponseTimeout;
	CEdit m_PeriphXPosL;
	CEdit m_PeriphYPosL;
	CButton m_RampUp;
	CEdit m_RampDuration;
	int *le_r;
	int *le_b;
	int *le_o;
	CStatic m_LeftEyeStatic;
	int *re_r;
	int *re_b;
	int *re_o;
	CStatic m_RightEyeStatic;
	afx_msg void OnBnClickedButton6();
	CStatic m_CatchPctR;
	CStatic m_CatchPctB;
	CStatic m_CatchPctN;
	int catch_r_total;
	int catch_b_total;
	int catch_n_total;
	int catch_r;
	int catch_b;
	int catch_n;
	CEdit m_PctTimeAtCenter;
	CEdit m_NumCatchTrials2;
	CEdit m_CenterXRadius;
	CEdit m_CenterYRadius;
	CStatic m_Countdown2;
	int *re_red_r;
	int *re_red_b;
	int *re_red_o;
	int *le_red_r;
	int *le_red_b;
	int *le_red_o;
	int *re_blue_r;
	int *re_blue_b;
	int *re_blue_o;
	int *le_blue_r;
	int *le_blue_b;
	int *le_blue_o;
	afx_msg void OnBnClickedViewpurple();
	afx_msg void OnBnClickedViewred();
	afx_msg void OnBnClickedViewblue();
	CButton m_AutoPosition;
};

#endif // REMEMBERDLG_H
