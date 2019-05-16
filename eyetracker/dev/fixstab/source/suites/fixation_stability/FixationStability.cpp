/**
 * @file FixationStability.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <cstdio>
#include "FixationStability.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

namespace fstab {

	CColorSpec::CColorSpec(int rr, int gg, int bb)
	{
		m_rgb.rr = rr; m_rgb.gg = gg; m_rgb.bb = bb;
		rgbToVsg();
	}

	CColorSpec::CColorSpec(double rr, double gg, double bb)
	{
		m_vsg.a = rr; m_vsg.b = gg; m_vsg.c = bb;
		vsgToRgb();
	}

	CColorSpec::CColorSpec(const RGBTRIVAL& rgb)
		: m_rgb(rgb)
	{
		rgbToVsg();
	}

	CColorSpec::CColorSpec(const VSGTRIVAL& vsg)
		: m_vsg(vsg)
	{
		vsgToRgb();
	}

	CColorSpec::CColorSpec(COLORREF rgbRef)
	{
		m_rgb.rr = GetRValue(rgbRef); 
		m_rgb.gg = GetGValue(rgbRef); 
		m_rgb.bb = GetBValue(rgbRef);
		rgbToVsg();
	}

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

	void CColorSpec::rgbToVsg()
	{
		m_vsg.a = m_rgb.rr / 256.0;
		m_vsg.b = m_rgb.gg / 256.0;
		m_vsg.c = m_rgb.bb / 256.0;
	}

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

	CEyeParameters::CEyeParameters(EYE eye, int dev, const CColorSpec& color,
								   const CrossLocations_t& unoccluded,
								   const CrossLocations_t& occluded)
		: m_eye(eye),
		  m_deviation(dev),
		  m_color(color),
		  m_unoccluded(unoccluded.begin(), unoccluded.end()),
		  m_occluded(occluded.begin(), occluded.end())
	{}

	CEyeParameters::~CEyeParameters() { /* Nothing to do */ }

	EYE CEyeParameters::eye() const { return m_eye; }
	int CEyeParameters::deviation() const { return m_deviation; }
	const CColorSpec& CEyeParameters::color() const { return m_color; }
	const CrossLocations_t& CEyeParameters::unoccluded() const { return m_unoccluded; }
	const CrossLocations_t& CEyeParameters::occluded() const { return m_occluded; }

	CEyeParameters* CEyeParameters::getFrom(FILE* infile)
	{
		int eye, dev;
		int size = 0;
		int temp;
		CColorSpec* color = NULL;
		CrossLocations_t occluded, unoccluded;
		CEyeParameters* retval = NULL;

		if (fscanf(infile, "%d\n%d\n", &eye, &dev) == 2) {
			if ((color = CColorSpec::getFrom(infile)) != NULL) {

				fscanf(infile, "%d\t", &size);
				for (int ix = 0; ix < size; ix++) {
					fscanf(infile, "%d\t", &temp);
					unoccluded.push_back(temp / 2.0);
				}

				fscanf(infile, "\n");

				fscanf(infile, "%d\t", &size);
				for (int ix = 0; ix < size; ix++) {
					fscanf(infile, "%d\t", &temp);
					occluded.push_back(temp / 2.0);
				}
				
				retval = new CEyeParameters((EYE)eye, dev, (*color), 
					unoccluded, occluded);

				delete color;
			}
		}

		return retval;
	}

	void CEyeParameters::putTo(FILE* outfile) 
	{
		fprintf(outfile, "%d\n%d\n", m_eye, m_deviation);

		m_color.putTo(outfile);
		
		fprintf(outfile, "%d\t", m_unoccluded.size());
		for (const_LocationIter_t curr = m_unoccluded.begin(); 
			 curr < m_unoccluded.end(); ++curr) 
		{
			fprintf(outfile, "%d\t", (int)((*curr) * 2));
		}
		fprintf(outfile, "\n");

		fprintf(outfile, "%d\t", m_occluded.size());
		for (const_LocationIter_t curr = m_occluded.begin(); 
			 curr < m_occluded.end(); ++curr) 
		{
			fprintf(outfile, "%d\t", (int)((*curr) * 2));
		}
		fprintf(outfile, "\n");
	}

	/*************************************************************************
	 *************************************************************************/

	CNoiseParameters::CNoiseParameters(bool useNoise, bool sameBg, bool interleave,
								   bool occlude, int size, int pct, 
								   const CColorSpec& color)
		: m_useNoise(useNoise),
		  m_sameBG(sameBg),
		  m_interleaveNoise(interleave),
		  m_occludeBG(occlude),
		  m_noiseSize(size),
		  m_noisePct(pct),
		  m_color(color)
	{ /* empty */ }

	CNoiseParameters::~CNoiseParameters() { /* empty */ }

	bool CNoiseParameters::useNoise() const { return m_useNoise; }
	bool CNoiseParameters::sameBg() const { return m_sameBG; }
	bool CNoiseParameters::interleaveNoise() const { return m_interleaveNoise; }
	bool CNoiseParameters::occludeBg() const { return m_occludeBG; }
	int  CNoiseParameters::noiseSize() const { return m_noiseSize; }
	int  CNoiseParameters::noisePct() const { return m_noisePct; }
	const CColorSpec& CNoiseParameters::color() const { return m_color; }

	CNoiseParameters* CNoiseParameters::getFrom(FILE* infile)
	{
		CNoiseParameters* retval = NULL;
		int useNoise, sameBG, interleaveNoise;
		int occludeBG, noiseSize, noisePct;

		if (fscanf(infile, "%d\n%d\n%d\n%d\n%d\n%d\n", &useNoise, &sameBG,
			&interleaveNoise, &occludeBG, &noiseSize, &noisePct) == 6) 
		{
			CColorSpec* color = CColorSpec::getFrom(infile);
			if (color != NULL) {
				retval = new CNoiseParameters(useNoise, sameBG, 
					interleaveNoise, occludeBG, noiseSize, noisePct, 
					(*color));
				delete color;
			}
		}

		return retval;
	}

	void CNoiseParameters::putTo(FILE* outfile)
	{
		fprintf(outfile, "%d\n%d\n%d\n%d\n%d\n%d\n", m_useNoise, m_sameBG, 
			m_interleaveNoise, m_occludeBG, m_noiseSize, m_noisePct); 
		m_color.putTo(outfile);
	}

} // namespace fstab
