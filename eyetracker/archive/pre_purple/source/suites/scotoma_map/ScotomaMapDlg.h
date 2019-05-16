/**
 * @file ScotomaMapDlg.h
 * @brief 
 */

#ifndef SCOTOMAMAPDLG_H
#define SCOTOMAMAPDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>
#include "comm/SerialComm.h"
#include "sys/sys_concurrency.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CScotomaMapDlg : public CDialog, public Mixin_ThreadAware
{
	DECLARE_DYNAMIC(CScotomaMapDlg)

public:
	CScotomaMapDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScotomaMapDlg();
	void load();
	void save();
	int response;

	void resetGraphs();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
    /** Dialog Data */
	enum { IDD = IDD_SCOTOMA_MAP_DLG };

	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonapply();

	CEdit m_PrimaryRed;
	CEdit m_PrimaryGreen;
	CEdit m_PrimaryBlue;
	CEdit m_SecondaryRed;
	CEdit m_SecondaryGreen;
	CEdit m_SecondaryBlue;
	CEdit m_TertiaryRed;
	CEdit m_TertiaryGreen;
	CEdit m_TertiaryBlue;
	CEdit m_NoiseRed;
	CEdit m_NoiseGreen;
	CEdit m_NoiseBlue;
	CEdit m_ResponseDelay;
	CEdit m_WrongDelay;
	CEdit m_NumRandomRepeats;
	CEdit m_NoisePct;
	CEdit m_NoiseSize;
	CEdit m_RampDuration;
	CEdit m_DisplayTime;
	CEdit m_DisplayRandMax;
	CEdit m_DisplayRandMin;
	CEdit m_FlashInterval;
	CEdit m_FlashIntervalRandMax;
	CEdit m_FlashIntervalRandMin;
	CEdit m_CrossPix;
	CEdit m_AlternateBlockSize;
	CEdit m_NumCatchTrials;
	CEdit m_NumCatchTrials2;
	CEdit m_NumBlankTrials;
	CEdit m_NumStepsH;
	CEdit m_NumStepsV;
	CEdit m_PctTimeAtCenter;
	CEdit m_PctTimeAtPeriph;
	CButton m_UseNoise;
	CButton m_InterleaveNoise;
	CButton m_RampUp;
	CButton m_HideCenter;
	CButton m_RandomizeFlashInterval;
	CButton m_RandomizeDisplayTime;
	CButton m_AvoidZero;
	CButton m_CenterPoints;
	CButton m_OffAxis;
	CEdit m_CenterXPos;
	CEdit m_CenterYPos;
	CEdit m_CenterDotSize;
	CEdit m_PeriphDotSize;
	CEdit m_StepSize;
	CEdit m_CenterXRadius;
	CEdit m_CenterYRadius;
	CEdit m_PeriphXRadius;
	CEdit m_PeriphYRadius;
	CButton m_CenterRadioPrimary;
	CButton m_CenterRadioSecondary;
	CButton m_CenterRadioAlternate;
	CButton m_CenterRadioRandomize;
	CButton m_CenterRadioGlobalDot;
	CButton m_PeriphRadioPrimary;
	CButton m_PeriphRadioSecondary;
	CButton m_PeriphRadioAlternate;
	CButton m_PeriphRadioTertiary;
	CButton m_PeriphRadioRandomize;
	afx_msg void OnBnClickedCenterradioprimary();
	afx_msg void OnBnClickedCenterradiosecondary();
	afx_msg void OnBnClickedCenterradioalternate();
	afx_msg void OnBnClickedCenterradiorandomize();
	afx_msg void OnBnClickedPeriphradioprimary();
	afx_msg void OnBnClickedPeriphradiosecondary();
	afx_msg void OnBnClickedPeriphradioalternate();
	afx_msg void OnBnClickedPeriphradiorandomize();
	afx_msg void OnBnClickedPeriphradiotertiary();
	int m_CenterRadio;
	int m_PeriphRadio;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedRandomizedisplaytime();
	afx_msg void OnBnClickedRandomizeflashinterval();
	void OnPaint();
	CStatic m_Countdown;
	CStatic m_PrimaryStatic;
	CStatic m_SecondaryStatic;
	CStatic m_NoiseStatic;
	CStatic m_TertiaryStatic;
	afx_msg void OnBnClickedUsenoise();
	CButton m_Pause;
	afx_msg void OnBnClickedButton2();
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

	CEdit m_MisbehaviorDelay;
	CEdit m_TertiaryRewardPct;
	CButton m_RewardCorrect;
	CButton m_RewardAny;
	CButton m_RewardTertiary;
	CButton m_RewardTrainingMode;
	afx_msg void OnBnClickedRewardcorrect();
	afx_msg void OnBnClickedRewardany();
	afx_msg void OnBnClickedRewardtertiary();
	CEdit m_RewardDelay;
	afx_msg void OnBnClickedCenterradioglobaldot();
	afx_msg void OnBnClickedRewardtrainingmode();
	CButton m_DisplayDuringChoice;
	CButton m_UseShapes;
	CEdit m_cStepSizeDeg;
	CEdit m_cNumStepsX;
	CEdit m_cNumStepsY;
	CButton m_SameBG;
	CButton m_OccludeBG;
	CEdit m_PctTimeAtEnriched;
	CEdit m_EnrichedX;
	CEdit m_EnrichedY;
	CEdit m_EnrichedXRadius;
	CEdit m_EnrichedYRadius;
	CButton m_DotGratings;
	CEdit m_DotGratingContrast;
	CEdit m_DotGratingCycles;
	
	CStatic m_LeftEyeStaticR;
	CStatic m_RightEyeStaticR;
	CStatic m_LeftEyeStaticB;
	CStatic m_RightEyeStaticB;
	CEdit m_SeparationV;
	CButton m_UseKeyboard;
	CStatic m_PurpleCount;
	int reptotal;
	int leptotal;
	CStatic m_TotalTrials;
	CButton m_Pause2;

	CButton m_debugResponserOn;
	CButton m_debugLabelsOn;
};

#endif // SCOTOMAMAPDLG_H
