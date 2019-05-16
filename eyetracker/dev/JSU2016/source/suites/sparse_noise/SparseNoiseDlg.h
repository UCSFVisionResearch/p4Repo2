/**
 * @file SparseNoiseDlg.h
 * @brief 
 */

#ifndef SPARSENOISEDLG_H
#define SPARSENOISEDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>


/*****************************************************************************
 * Classes
 *****************************************************************************/

class CSparseNoiseDlg : public CDialog
{
	DECLARE_DYNAMIC(CSparseNoiseDlg)

public:
	CSparseNoiseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSparseNoiseDlg();

	void load();
	void save();
	
	void UpdateEyePositions(double lxDeg, double lyDeg, double rxDeg, 
	double ryDeg);
	void printDebug(LPCTSTR text);


	/** Dialog Data */
	enum { IDD = IDD_SPARSE_NOISE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void OnPaint();
	void PaintColorStatics();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	

	afx_msg void OnBnClickedBinocular();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedCreateStimFolder();
	afx_msg void OnBnClickedLoadStimFolder();
	afx_msg void OnBnClickedUseGrid();


	CEdit m_CrossX;
	CEdit m_CrossY;
	CEdit m_CrossDiameter;
	CEdit m_CrossThickness;

	CButton m_Binocular;
	CButton m_Shutters;
	CEdit m_CrossX2;
	CEdit m_CrossY2;
	CEdit m_SwapInt;

	CEdit m_CircleRadius;
	CEdit m_DisplayTimeMs;
	
	
	CButton m_Create;

	
	CStatic m_EyePosnDisplayLX;
	CStatic m_EyePosnDisplayLY;
	CStatic m_EyePosnDisplayRX;
	CStatic m_EyePosnDisplayRY;

	CEdit m_WindowAllowancePix;
	CButton m_Wait;

	CEdit m_FolderName;
	CEdit m_NumFrames;
	CEdit m_StimWidth;
	CEdit m_MinDist;
	CEdit m_PrimaryCount;
	CEdit m_SecondaryCount;
	CEdit m_TertiaryCount;
	CEdit m_Seed;
	CStatic m_debug;

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

	CEdit m_BGRed;
	CEdit m_BGGreen;
	CEdit m_BGBlue;
	CStatic m_BGStatic;

	CButton m_UseGrid;
	CEdit m_StepSizeDeg;
	CEdit m_NumStepsX;
	CEdit m_NumStepsY;

};


#endif // SparseNoiseDLG_H
