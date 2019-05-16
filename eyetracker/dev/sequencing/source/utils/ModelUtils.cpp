/**	
 * @file ModelUtil.cpp
 * @brief Utility types, objects, and functions for working with models.
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "ModelUtils.h"

/*****************************************************************************
 * Namespace etu
 *****************************************************************************/

namespace etu {

	/** Initialize with RGB values.
	 * @param rr Red value, range [0:255].
	 * @param gg Green value, range [0:255].
	 * @param bb Blue value, range [0:255].
	 */
	CColorSpec::CColorSpec(int rr, int gg, int bb)
	{
		m_rgb.rr = rr; m_rgb.gg = gg; m_rgb.bb = bb;
		rgbToVsg();
	}

	/** Initialize with percentage values.
	 * @param rr Red value, range [0:1].
	 * @param gg Green value, range [0:1].
	 * @param bb Blue value, range [0:1].
	 */
	CColorSpec::CColorSpec(double rr, double gg, double bb)
	{
		m_vsg.a = rr; m_vsg.b = gg; m_vsg.c = bb;
		vsgToRgb();
	}

	/** Initialize with RGB values in struct.
	 * @param rgb An RGB struct.
	 */
	CColorSpec::CColorSpec(const RGBTRIVAL& rgb)
		: m_rgb(rgb)
	{
		rgbToVsg();
	}

	/** Initialize with a VSGTRIVAL struct.
	 * @param rgb An RGB struct.
	 */
	CColorSpec::CColorSpec(const VSGTRIVAL& vsg)
		: m_vsg(vsg)
	{
		vsgToRgb();
	}

	/** Initialize with a windows COLORREF object.
	 * @param rgbRef Windows COLORREF type.
	 */
	CColorSpec::CColorSpec(COLORREF rgbRef)
	{
		m_rgb.rr = GetRValue(rgbRef); 
		m_rgb.gg = GetGValue(rgbRef); 
		m_rgb.bb = GetBValue(rgbRef);
		rgbToVsg();
	}

	/** Copy constructor. 
	 * @brief spec An initialized color spec.
	 */
	CColorSpec::CColorSpec(const CColorSpec& spec)
	{
		m_rgb.rr = spec.m_rgb.rr;
		m_rgb.gg = spec.m_rgb.gg;
		m_rgb.bb = spec.m_rgb.bb;
		m_vsg.a = spec.m_vsg.a;
		m_vsg.b = spec.m_vsg.b;
		m_vsg.c = spec.m_vsg.c;
	}

	CColorSpec::~CColorSpec() { /* Nothing to do */ }

	/*
	 * Convert interal RGB state into VSGTRIVAL state and store to internal 
	 * VSGTRIVAL state.
	 */
	void CColorSpec::rgbToVsg()
	{
		m_vsg.a = m_rgb.rr / 256.0;
		m_vsg.b = m_rgb.gg / 256.0;
		m_vsg.c = m_rgb.bb / 256.0;
	}

	/*
	 * Convert interal VSGTRIVAL state into RGB state and store to internal
	 * RGB state.
	 */
	void CColorSpec::vsgToRgb()
	{
		m_rgb.rr = (int)(m_vsg.a * 256);
		m_rgb.gg = (int)(m_vsg.b * 256);
		m_rgb.bb = (int)(m_vsg.c * 256);
	}

	VSGTRIVAL CColorSpec::toVsg() const { return m_vsg; }
	RGBTRIVAL CColorSpec::toRgb() const { return m_rgb; }
	COLORREF  CColorSpec::toRef() const { return RGB(m_rgb.rr, m_rgb.gg, m_rgb.bb); }

	double CColorSpec::getVsgR() const { return m_vsg.a; }
	double CColorSpec::getVsgG() const { return m_vsg.b; }
	double CColorSpec::getVsgB() const { return m_vsg.c; }

	int CColorSpec::getRgbR() const { return m_rgb.rr; }
	int CColorSpec::getRgbG() const { return m_rgb.gg; }
	int CColorSpec::getRgbB() const { return m_rgb.bb; }

	CColorSpec* CColorSpec::getFrom(FILE* infile)
	{
		int rr, gg, bb;
		CColorSpec* retval = NULL;
		if (fscanf(infile, "%d\t%d\t%d\n", &rr, &gg, &bb) == 3) {
			retval = new CColorSpec(rr, gg, bb);
		}
		return retval;
	}

	void CColorSpec::putTo(FILE* outfile)
	{
		fprintf(outfile, "%d\t%d\t%d\n", m_rgb.rr, m_rgb.gg, m_rgb.bb);
	}

	/*************************************************************************
	 *************************************************************************/

	CNoiseParameters::CNoiseParameters(bool useNoise, bool sameBg, bool interleave,
								   bool occlude, int size, int noiseCov, int noiseCom,
								   const CColorSpec& color)
		: m_useNoise(useNoise),
		  m_sameBG(sameBg),
		  m_interleaveNoise(interleave),
		  m_occludeBG(occlude),
		  m_noiseSize(size),
		  m_noiseCoverage(noiseCov),
		  m_noiseCommon(noiseCom),
		  m_color(color)
	{ /* empty */ }

	CNoiseParameters::~CNoiseParameters() { /* empty */ }

	bool CNoiseParameters::useNoise() const { return m_useNoise; }
	bool CNoiseParameters::sameBg() const { return m_sameBG; }
	bool CNoiseParameters::interleaveNoise() const { return m_interleaveNoise; }
	bool CNoiseParameters::occludeBg() const { return m_occludeBG; }
	int  CNoiseParameters::noiseSize() const { return m_noiseSize; }
	int  CNoiseParameters::noiseCoverage() const { return m_noiseCoverage; }
	int  CNoiseParameters::noiseCommon() const { return m_noiseCommon; }
	const CColorSpec& CNoiseParameters::color() const { return m_color; }

	CNoiseParameters* CNoiseParameters::getFrom(FILE* infile)
	{
		CNoiseParameters* retval = NULL;
		int useNoise, sameBG, interleaveNoise;
		int occludeBG, noiseSize, noiseCommon, noiseCoverage;

		if (fscanf(infile, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n", &useNoise, &sameBG,
			&interleaveNoise, &occludeBG, &noiseSize, &noiseCoverage, &noiseCommon) == 7) 
		{
			CColorSpec* color = CColorSpec::getFrom(infile);
			if (color != NULL) {
				retval = new CNoiseParameters(useNoise, sameBG, 
					interleaveNoise, occludeBG, noiseSize, noiseCoverage,
					noiseCommon, (*color));
				delete color;
			}
		}

		return retval;
	}

	void CNoiseParameters::putTo(FILE* outfile)
	{
		fprintf(outfile, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n", m_useNoise, m_sameBG, 
			m_interleaveNoise, m_occludeBG, m_noiseSize, m_noiseCoverage, m_noiseCommon); 
		m_color.putTo(outfile);
	}
};