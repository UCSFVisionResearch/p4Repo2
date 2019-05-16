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
	void updateFieldResults();
	void printDebug(LPCTSTR text);
	void UpdateEyePositions(double lxDeg, double lyDeg, double rxDeg, 
	double ryDeg);

	/** Dialog Data */
	enum { IDD = IDD_RF_MAPPING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void OnPaint();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	

	afx_msg void OnBnClickedBinocular();
	afx_msg void OnBnClickedSaveasleft();
	afx_msg void OnBnClickedSaveasright();
	afx_msg void OnBnClickedCircle();
	afx_msg void OnBnClickedBar();
	afx_msg void OnBnClickedUseMouse();
	afx_msg void OnBnClickedUseGrid();
	afx_msg void OnBnClickedBlanks();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedUseNoise();
	afx_msg void OnBnClickedPrimaryL();
	afx_msg void OnBnClickedPrimaryR();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	void PaintColorStatics();
	
	CEdit m_CrossX;
	CEdit m_CrossY;
	CEdit m_CrossDiameter;
	CEdit m_CrossThickness;

	CButton m_Binocular;
	CEdit m_CrossX2;
	CEdit m_CrossY2;
	CEdit m_SwapInt;

	CEdit m_PrimaryRed;
	CEdit m_PrimaryGreen;
	CEdit m_PrimaryBlue;
	CStatic m_PrimaryStatic;

	CEdit m_SecondaryRed;
	CEdit m_SecondaryGreen;
	CEdit m_SecondaryBlue;
	CStatic m_SecondaryStatic;

	CEdit m_TertiaryRed;
	CEdit m_TertiaryGreen;
	CEdit m_TertiaryBlue;
	CStatic m_TertiaryStatic;

	CButton m_usePrimaryL;
	CButton m_usePrimaryR;
	CButton m_useSecondaryL;
	CButton m_useSecondaryR;
	CButton m_useTertiaryL;
	CButton m_useTertiaryR;

	CButton m_RadioCircle;
	CButton m_RadioBar;
	CEdit m_CircleRadius;
	CEdit m_BarWidth;
	CEdit m_BarHeight;
	CEdit m_Orientation;
	CButton m_RadioUseMouse;
	CButton m_RadioUseGrid;
	CEdit m_StepSizeDeg;
	CEdit m_StepsX;
	CEdit m_StepsY;
	CEdit m_DisplayTimeMs;
	CButton m_Blanks;
	CEdit m_BlankTimeMs;
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

	CStatic m_debug;

	CStatic m_EyePosnDisplayLX;
	CStatic m_EyePosnDisplayLY;
	CStatic m_EyePosnDisplayRX;
	CStatic m_EyePosnDisplayRY;

	CEdit m_WindowAllowancePix;
	CButton m_Wait;
	

};


#endif // RFMAPPINGDLG_H
