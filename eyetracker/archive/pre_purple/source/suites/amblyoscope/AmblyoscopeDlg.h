/**
 * @file AmblyoscopeDlg.h
 * @brief 
 */

#ifndef AMBLYOSCOPEDLG_H
#define AMBLYOSCOPEDLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CAmblyoscopeDlg: public CDialog
{
	DECLARE_DYNAMIC(CAmblyoscopeDlg)

public:
	CAmblyoscopeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAmblyoscopeDlg();
	void load();
	void save();

	/** Dialog Data */
	enum { IDD = IDD_AMBLYOSCOPE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonapply();
	CStatic m_XOffset;
	CStatic m_YOffset;
	CEdit m_LRed;
	CEdit m_LGreen;
	CEdit m_LBlue;
	CEdit m_RRed;
	CEdit m_RGreen;
	CEdit m_RBlue;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CStatic m_XOffsetDeg;
	CStatic m_YOffsetDeg;
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	bool flip;
	bool lefton;
	bool righton;
	CStatic m_Rlabel;
	CStatic m_Llabel;
	CButton m_Background;
	CButton m_FlashTest;
	CEdit m_PctMonocular;
	CEdit m_FlashDuration;
	CEdit m_BlankDuration;
	CEdit m_DeviatedRightX;
	CEdit m_DeviatedRightY;
	CEdit m_DeviatedLeftX;
	CEdit m_DeviatedLeftY;
	CButton m_CenterRed;
	CButton m_CenterBlue;
	CButton m_CenterRandom;
	CButton m_CenterBlocks;
	CEdit m_FlashBlocks;
	CEdit m_ArmLength;
	CEdit m_ArmWidth;
	CEdit m_SpaceSize;
	CStatic m_ArmStatic;
	CButton m_FixationDot;
	CEdit m_FixationX;
	CEdit m_FixationY;
	CEdit m_FixationSize;
	CEdit m_CenterX;
	CEdit m_CenterY;
	CEdit m_BGBrightness;
	CButton m_Alternate;
	CStatic m_EyePositions;
	CButton m_Fovea;
};


#endif // AMBLYOSCOPEDLG_H
