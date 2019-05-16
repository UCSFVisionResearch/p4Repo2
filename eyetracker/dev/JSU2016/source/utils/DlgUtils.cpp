/**	
 * @file DlgUtil.cpp
 * @brief Utility types, objects, and functions for working with dialogs.
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>     /* CString */
#include "Utils.h"
#include "ModelUtils.h"
#include "DlgUtils.h"

/*****************************************************************************
 * Namespace etu
 *****************************************************************************/

namespace etu {

	CColorControls::CColorControls()
	{
		m_rgb.rr = 0;
		m_rgb.gg = 0;
		m_rgb.bb = 0;
	}

	CColorControls::~CColorControls() { /* empty */ }

	/** Push contents of CEdits into numerical data. Does not repaint. */
	void CColorControls::apply()
	{
		char buffer[16];

		cRed.GetWindowText(buffer, sizeof buffer);
		m_rgb.rr = StringToInt(buffer, 0);
		cGreen.GetWindowText(buffer, sizeof buffer);
		m_rgb.gg = StringToInt(buffer, 0);
		cBlue.GetWindowText(buffer, sizeof buffer);
		m_rgb.bb = StringToInt(buffer, 0);
	}

	/** Push numerical data into contents of dlg components. Does not repaint. */
	void CColorControls::update()
	{
		cRed.SetWindowText(IntToString(m_rgb.rr));
		cGreen.SetWindowText(IntToString(m_rgb.gg));
		cBlue.SetWindowText(IntToString(m_rgb.bb));
	}

	/** Update the color display square. */
	void CColorControls::rePaint() 
	{
		CRect rect;
		CPaintDC painter(&cDisplay);
		CColorSpec colorSpec(m_rgb);
		cDisplay.GetClientRect(rect);
		painter.FillSolidRect(rect, colorSpec.toRef());
	}

	/** Returns a memberwise copy. */
	RGBTRIVAL CColorControls::get() { return m_rgb; }

	/** Memberwise copy. */
	void CColorControls::set(const RGBTRIVAL& rgb) { m_rgb = rgb; }

	/****************************************************************************
	 ****************************************************************************/

	CNoiseControls::CNoiseControls()
	: m_noiseSize(0),
	  m_noiseCoverage(0),
	  m_noiseCommon(0)
	{}

	CNoiseControls::~CNoiseControls() { /* empty */ }

	void CNoiseControls::apply()
	{
		char buffer[16];
		cNoiseSize.GetWindowText(buffer, sizeof buffer);
		m_noiseSize = StringToInt(buffer, 0);
		cNoiseCoverage.GetWindowText(buffer, sizeof buffer);
		m_noiseCoverage = StringToInt(buffer, 0);
		cNoiseCommon.GetWindowText(buffer, sizeof buffer);
		m_noiseCommon = StringToInt(buffer, 0);
		cColor.apply();
	}

	void CNoiseControls::update()
	{
		cNoiseSize.SetWindowText(IntToString(m_noiseSize));
		cNoiseCoverage.SetWindowText(IntToString(m_noiseCoverage));
		cNoiseCommon.SetWindowText(IntToString(m_noiseCommon));
		cColor.update();
	}

	bool CNoiseControls::useNoise() const { return (cUseNoise.GetCheck() == BST_CHECKED); }
	bool CNoiseControls::sameBg() const { return (cSameBG.GetCheck() == BST_CHECKED); }
	bool CNoiseControls::interleaveNoise() const { return (cInterleaveNoise.GetCheck() == BST_CHECKED); }
	bool CNoiseControls::occludeBg() const { return (cOccludeBG.GetCheck() == BST_CHECKED); }

	void CNoiseControls::useNoise(bool use) { cUseNoise.SetCheck(use ? BST_CHECKED : BST_UNCHECKED); }
	void CNoiseControls::sameBg(bool same) {cSameBG.SetCheck(same ? BST_CHECKED : BST_UNCHECKED); }
	void CNoiseControls::interleaveNoise(bool inter) {cInterleaveNoise.SetCheck(inter  ? BST_CHECKED : BST_UNCHECKED); }
	void CNoiseControls::occludeBg(bool occlude) { cOccludeBG.SetCheck(occlude ? BST_CHECKED : BST_UNCHECKED); }

	int  CNoiseControls::noiseSize() const { return m_noiseSize; }
	int  CNoiseControls::noiseCoverage() const { return m_noiseCoverage; }
	int  CNoiseControls::noiseCommon() const { return m_noiseCommon; }

	void CNoiseControls::noiseSize(int size) 
	{
		m_noiseSize = size;
		cNoiseSize.SetWindowText(IntToString(m_noiseSize));
	}

	void CNoiseControls::noiseCoverage(int pct) 
	{
		m_noiseCoverage = pct;
		cNoiseCoverage.SetWindowText(IntToString(m_noiseCoverage));
	}

	void CNoiseControls::noiseCommon(int pct) 
	{
		m_noiseCommon = pct;
		cNoiseCommon.SetWindowText(IntToString(m_noiseCommon));
	}

	CNoiseParameters* CNoiseControls::parameters()
	{
		CColorSpec color(cColor.get());
		return (new CNoiseParameters(useNoise(), sameBg(), interleaveNoise(), 
			occludeBg(), m_noiseSize, m_noiseCoverage, m_noiseCommon, color));
	}

	void CNoiseControls::parameters(const CNoiseParameters& params)
	{
		useNoise(params.useNoise());
		sameBg(params.sameBg());
		interleaveNoise(params.interleaveNoise());
		occludeBg(params.occludeBg());
		noiseSize(params.noiseSize());
		noiseCoverage(params.noiseCoverage());
		noiseCommon(params.noiseCommon());
		cColor.set(params.color().toRgb());
		update();
	}

	/** Whether "enable" box is checked. 
	 * @param enabled True if box is check to enable controls. 
	 */
	void CNoiseControls::setEnabled(bool enabled)
	{
		cUseNoise.SetCheck(enabled ? BST_CHECKED : BST_UNCHECKED);
		cInterleaveNoise.EnableWindow(enabled);
		cSameBG.EnableWindow(enabled);
		cOccludeBG.EnableWindow(enabled);
		cNoiseSize.EnableWindow(enabled);
		cNoiseCoverage.EnableWindow(enabled);
		cNoiseCommon.EnableWindow(enabled);
		cColor.cRed.EnableWindow(enabled);
		cColor.cGreen.EnableWindow(enabled);
		cColor.cBlue.EnableWindow(enabled);
	}
};