/**
 * @file FixationStabilityDlg.h
 * @brief 
 */

#ifndef FIXATION_STABILITY_DLG_H
#define FIXATION_STABILITY_DLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>
#include "app/res/resource.h"
#include "utils/DlgUtils.h"
#include "utils/ModelUtils.h"
#include "FixationStability.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

/** Whether to locate the cross at each of the three positions. */
class CLocationCheckboxes {
public:
	CLocationCheckboxes();
	~CLocationCheckboxes();

	CButton cCenter;  /**< Checkbox to use (0,0). */
	CButton cHalf;    /**< Checkbox to use (+/- 0.5 * dev, 0). */
	CButton cPeriph;  /**< Checkbox to use (+/- dev, 0). */

	void apply();
	void update();

	const fstab::CrossLocations_t& locations() const;
	void locations(const fstab::CrossLocations_t& locations);

private:
	fstab::CrossLocations_t m_locations;

	/* Not supported. */
	CLocationCheckboxes(const CLocationCheckboxes&);
	CLocationCheckboxes& operator=(const CLocationCheckboxes&);
};

/*****************************************************************************
 *****************************************************************************/

/** Color, deviation, and positions to use for one eye's fixation. */
class CEyeParameterControls {
public:
	CEyeParameterControls();
	~CEyeParameterControls();

	CEdit cDeviation;
	etu::CColorControls cColor;
	CLocationCheckboxes cUnOccludedPositions;
	CLocationCheckboxes cOccludedPositions;

	void apply();
	void update();

	int deviation() const;
	void deviation(int dev);

	fstab::CEyeParameters* parameters(fstab::EYE eye);
	void parameters(const fstab::CEyeParameters& params);

private:
	int m_deviation;

	/* Not supported. */
	CEyeParameterControls(const CEyeParameterControls&);
	CEyeParameterControls& operator=(const CEyeParameterControls&);
};

/*****************************************************************************
 *****************************************************************************/

/** Actual dialog. */
class CFixationStabilityDlg : public CDialog
{
	DECLARE_DYNAMIC(CFixationStabilityDlg)

public:
	CFixationStabilityDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFixationStabilityDlg();
	void load();
	void save();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	/** Dialog Data */
	enum { IDD = IDD_FIXATIONSTAB_DLG };

	virtual BOOL OnInitDialog();
	void OnPaint();

	afx_msg void OnBnClickedUseNoise();
	afx_msg void OnBnClickedApply();
	
	CButton ApplyButton;

	CEdit CrosshairDiameter;
	CEdit CrosshairThickness;
	CEdit FixationTime;

	CEyeParameterControls LeftEyeControls;
	CEyeParameterControls RightEyeControls;
	etu::CNoiseControls NoiseControls;
};

#endif // FIXATION_STABILITY_DLG_H
