/**
 * @file ModelUtil.h
 * @brief Utility types, objects, and functions for working with models.
 *
 */

#ifndef MODEL_UTIL_H
#define MODEL_UTIL_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include <cstdio>
#include "VSGV8.H"

/*****************************************************************************
 * Namespace etu
 *****************************************************************************/

namespace etu {

	/** Holds three ints, 0 - 255. */
	struct RGBTRIVAL {
		int rr;
		int gg;
		int bb;
	};

	/*************************************************************************
	 *************************************************************************/

	/** Color utility object. */
	class CColorSpec {
	public:
		CColorSpec(int rr, int gg, int bb);
		CColorSpec(double rr, double gg, double bb);
		CColorSpec(const RGBTRIVAL& rgb);
		CColorSpec(const VSGTRIVAL& vsg);
		CColorSpec(COLORREF rgbRef);
		CColorSpec(const CColorSpec& spec);
		~CColorSpec();

		VSGTRIVAL toVsg() const;
		RGBTRIVAL toRgb() const;
		COLORREF  toRef() const;

		double getVsgR() const;
		double getVsgG() const;
		double getVsgB() const;

		int getRgbR() const;
		int getRgbG() const;
		int getRgbB() const;

		static CColorSpec* getFrom(FILE* infile);
		void putTo(FILE* outfile);

	private:
		void rgbToVsg();
		void vsgToRgb();

		RGBTRIVAL m_rgb;
		VSGTRIVAL m_vsg;
	};

	/*************************************************************************
	 *************************************************************************/

	/** Color, size, percent, and options for backgrounds. */
	class CNoiseParameters {
	public:
		CNoiseParameters(bool useNoise, bool sameBg, bool interleave, 
			bool occlude, int size, int noiseCov, int noiseCom, 
			const CColorSpec& color);
		~CNoiseParameters();

		bool useNoise() const;
		bool sameBg() const;
		bool interleaveNoise() const;
		bool occludeBg() const;
		int  noiseSize() const;
		int  noiseCoverage() const;
		int  noiseCommon() const;
		const CColorSpec& color() const;

		static CNoiseParameters* getFrom(FILE* infile);
		void putTo(FILE* outfile);

	private:
		bool m_useNoise;
		bool m_sameBG;
		bool m_interleaveNoise;
		bool m_occludeBG;
		int m_noiseSize;
		int m_noiseCoverage;
		int m_noiseCommon;
		CColorSpec m_color;

		/* Not supported. */
		CNoiseParameters(const CNoiseParameters&);
		CNoiseParameters& operator=(const CNoiseParameters&);
	};
};

#endif // MODEL_UTIL_H
