/**
 * @file TrialSaccadesDlg.h
 * @brief 
 */

#ifndef TRIALSACCADESDLG_H
#define TRIALSACCADESDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>
#include <map>
#include "comm/SerialComm.h"
#include "sys/sys_concurrency.h"
#include "SaccadeTrial.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CTrialSaccadesDlg : public CDialog, public Mixin_ThreadAware
{
	DECLARE_DYNAMIC(CTrialSaccadesDlg)

public:
	CTrialSaccadesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTrialSaccadesDlg();
	void load();
	void save();

	void ClearTrialCounts();
	void IncrementCount(CSaccadeTrial::STIM_CODE code, bool correct);
	void UpdateEyePositions(double lxDeg, double lyDeg, double rxDeg, 
		double ryDeg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
    /** Dialog Data */
	enum { IDD = IDD_TRIAL_SACCADES_DLG };

	virtual BOOL OnInitDialog();
	void OnPaint();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();

	afx_msg void OnBnClickedEyeCoverageRadioSpatiotopic();
	afx_msg void OnBnClickedEyeCoverageRadioRetinotopic();
	afx_msg void OnBnClickedUseApprox();

	afx_msg void OnBnClickedGridSettingsRadioRandom();
	afx_msg void OnBnClickedGridSettingsRadioCyclopean();

	afx_msg void OnBnClickedCenterRadioPrimary();
	afx_msg void OnBnClickedCenterRadioSecondary();
	afx_msg void OnBnClickedCenterRadioAlternate();
	afx_msg void OnBnClickedCenterRadioRandomize();

	afx_msg void OnBnClickedPeriphRadioPrimary();
	afx_msg void OnBnClickedPeriphRadioSecondary();
	afx_msg void OnBnClickedPeriphRadioAlternate();
	afx_msg void OnBnClickedPeriphRadioRandomize();

	afx_msg void OnBnClickedGridRadioCartesian();
	afx_msg void OnBnClickedGridRadioRadial();

	afx_msg void OnBnClickedRandomizeDisplayTime();
	afx_msg void OnBnClickedRandomizeFlashInterval();		
	afx_msg void OnBnClickedUseNoise();	

	CEdit   m_PrimaryRed;
	CEdit   m_PrimaryGreen;
	CEdit   m_PrimaryBlue;
	CStatic m_PrimaryStatic;

	CEdit   m_SecondaryRed;
	CEdit   m_SecondaryGreen;
	CEdit   m_SecondaryBlue;
	CStatic m_SecondaryStatic;
	
	CButton m_debugResponderOn;
	CButton m_debugLabelsOn;
	
	CEdit m_FixationFlashDelay;
	CEdit m_MisbehaviorDelay;
	CEdit m_WrongDelay;
	CEdit m_RewardDelay;

	CButton m_PauseDotOff;
	CButton m_PauseDotOn;

	CButton m_EyeCoverageRadioSpatiotopic;
	CButton m_EyeCoverageRadioRetinotopic;
	CButton m_UseApprox;
	CStatic m_EyeCoverageDescription;

	CStatic m_InfoDisplay;
	CStatic m_EyePosnDisplayLX;
	CStatic m_EyePosnDisplayLY;
	CStatic m_EyePosnDisplayRX;
	CStatic m_EyePosnDisplayRY;

	CEdit   m_CenterDotSize;
	CEdit   m_CrossPix;
	CEdit   m_CenterXPos;
	CEdit   m_CenterYPos;
	CEdit   m_cStepSizeDeg;
	CEdit   m_cNumStepsX;
	CEdit   m_cNumStepsY;
	CButton m_ShowCenterFlash;
	CButton m_ShowCenterHold;
	CEdit   m_DisplayTime;
	CButton m_RandomizeDisplayTime;
	CEdit   m_DisplayRandMax;
	CEdit   m_DisplayRandMin;
	CButton m_CenterRadioPrimary;
	CButton m_CenterRadioSecondary;
	CButton m_CenterRadioAlternate;
	CEdit   m_AlternateBlockSize;
	CButton m_CenterRadioRandomize;

	CEdit   m_PeriphDotSize;
	CEdit   m_TargetActivePeriod;
	CEdit   m_FlashInterval;
	CButton m_RandomizeFlashInterval;
	CEdit   m_FlashIntervalRandMax;
	CEdit   m_FlashIntervalRandMin;
	CEdit   m_SaccadeHoldPeriod;
	CButton m_DisplayDuringHoldPeriod;
	CButton m_UseShapes;
	CButton m_DotGratings;
	CEdit   m_DotGratingContrast;
	CEdit   m_DotGratingCycles;

	CButton m_AdvanceEitherEye;
	
	CButton m_GridSettingsRadioRandom;
	CButton m_GridSettingsRadioCyclopean;
	CStatic m_GridSettingsDescription;
	CEdit   m_DeviationAngle;

	CButton m_PeriphRadioPrimary;
	CButton m_PeriphRadioSecondary;
	CButton m_PeriphRadioAlternate;
	CButton m_PeriphRadioRandomize;
	CEdit   m_NumRedTrials;

	/* Cartesian Grid controls */
	CButton m_GridRadioCartesian;
	CEdit   m_NumStepsH;
	CEdit   m_NumStepsV;
	CEdit   m_StepSize;
	CButton m_AvoidZero;

	/* Radial Grid controls */
	CButton m_GridRadioRadial;
	CEdit   m_AngleOffset;
	CButton m_Angle0;
	CButton m_Angle45;
	CButton m_Angle90;
	CButton m_Angle135;
	CButton m_Angle180;
	CButton m_Angle225;
	CButton m_Angle270;
	CButton m_Angle315;
	CEdit   m_RadiusOffset;
	CButton m_RadiusOrigin;
	CButton m_Radius0;
	CButton m_Radius5;
	CButton m_Radius10;
	CButton m_Radius15;
	CButton m_Radius20;
	CButton m_Radius25;
	CButton m_Radius30;

	/* Noise controls */
	CButton m_UseNoise;
	CButton m_SameBG;
	CButton m_InterleaveNoise;
	CButton m_OccludeBG;
	CEdit   m_NoiseSize;
	CEdit   m_NoisePct;
	CEdit   m_NoiseRed;
	CEdit   m_NoiseGreen;
	CEdit   m_NoiseBlue;
	CStatic m_NoiseStatic;

	/* Trial tracking. */
	CStatic m_TrialResults_RR_R;
	CStatic m_TrialResults_RR_W;
	CStatic m_TrialResults_RR_T;

	CStatic m_TrialResults_RB_R;
	CStatic m_TrialResults_RB_W;
	CStatic m_TrialResults_RB_T;

	CStatic m_TrialResults_BR_R;
	CStatic m_TrialResults_BR_W;
	CStatic m_TrialResults_BR_T;

	CStatic m_TrialResults_BB_R;
	CStatic m_TrialResults_BB_W;
	CStatic m_TrialResults_BB_T;

	CStatic m_TrialResults_R_T;
	CStatic m_TrialResults_W_T;
	CStatic m_TrialResults_Total;

	int m_CenterRadio;
	int m_PeriphRadio;

private:
	int m_cRed_pRed_Right;
	int m_cRed_pRed_Wrong;
	int m_cRed_pRed_Total;

	int m_cRed_pBlue_Right;
	int m_cRed_pBlue_Wrong;
	int m_cRed_pBlue_Total;

	int m_cBlue_pRed_Right;
	int m_cBlue_pRed_Wrong;
	int m_cBlue_pRed_Total;

	int m_cBlue_pBlue_Right;
	int m_cBlue_pBlue_Wrong;
	int m_cBlue_pBlue_Total;

	int m_totalRight;
	int m_totalWrong;
	int m_totalTotal;

	void EnableGridCartesianControls(bool enable);
	void EnableGridRadialControls(bool enable);
	void EnableControlsForEyeCoverageSpatiotopic(bool random);
	void HandleDeviationAngleBox();

	void IncrementSingleCount(int& total, int& right, int& wrong, bool correct);
	void RefreshCountsDisplay();
	void RefreshCountDisplay(CStatic& boxTotal, CStatic& boxRight, 
		CStatic& boxWrong, int total, int right, int wrong);
	CString PercentString(int num, int whole, char sep);
};

#endif // TRIALSACCADESDLG_H
