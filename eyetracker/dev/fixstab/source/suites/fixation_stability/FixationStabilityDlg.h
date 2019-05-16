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
#include "FixationStability.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

/** RGB color controls and display. */
class CColorControls {
public:
	CColorControls();
	~CColorControls();

	CEdit   cRed;
	CEdit   cGreen;
	CEdit   cBlue;
	CStatic cDisplay;

	void apply();
	void update();
	void rePaint();

	fstab::RGBTRIVAL get();
	void set(const fstab::RGBTRIVAL& color);

private:
	fstab::RGBTRIVAL m_rgb;

	/* Not supported. */
	CColorControls(const CColorControls&);
	CColorControls& operator=(const CColorControls&);
};

/*****************************************************************************
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
	CColorControls cColor;
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

/** Color, size, percent, and options for backgrounds. */
class CNoiseControls {
public:
	CNoiseControls();
	~CNoiseControls();

	CButton cUseNoise;
	CButton cSameBG;
	CButton cInterleaveNoise;
	CButton cOccludeBG;
	CEdit cNoiseSize;
	CEdit cNoisePct;
	CColorControls cColor;

	void apply();
	void update();

	bool useNoise() const;
	bool sameBg() const;
	bool interleaveNoise() const;
	bool occludeBg() const;
	int  noiseSize() const;
	int  noisePct() const;

	void useNoise(bool use);
	void sameBg(bool same);
	void interleaveNoise(bool inter);
	void occludeBg(bool occlude);
	void noiseSize(int size);
	void noisePct(int pct);

	fstab::CNoiseParameters* parameters();
	void parameters(const fstab::CNoiseParameters& params);

private:
	int m_noiseSize;
	int m_noisePct;
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
	CNoiseControls NoiseControls;
};

#endif // FIXATION_STABILITY_DLG_H
