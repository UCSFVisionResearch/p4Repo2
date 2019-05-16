/**
 * @file FixationStability.h
 * @brief 
 */

#ifndef FIXATION_STABILITY_H
#define FIXATION_STABILITY_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include <deque>
#include <cstdio>
#include "VSGV8.H"
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

namespace fstab
{
	/** Coefficient for left or right. */
	enum EYE {
		LEFT_EYE = -1,
		RIGHT_EYE = 1
	};

	/*************************************************************************
	 *************************************************************************/

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

	/** 
	 * @brief Locations at which to display the cross. 
     * Display it at (EYE)whichEye * coefficient * deviation.
	 */
	typedef std::deque<double> CrossLocations_t;
	typedef CrossLocations_t::const_iterator const_LocationIter_t;

	/*************************************************************************
	 *************************************************************************/

	/** Aggregation of parameters for a fixating eye. */
	class CEyeParameters {
	public:
		CEyeParameters(EYE eye, int dev, const CColorSpec& color,
			const CrossLocations_t& unoccluded, 
			const CrossLocations_t& occluded);
		~CEyeParameters();

		EYE eye() const;
		int deviation() const;
		const CColorSpec& color() const;
		const CrossLocations_t& unoccluded() const;
		const CrossLocations_t& occluded() const;

		static CEyeParameters* getFrom(FILE* infile);
		void putTo(FILE* outfile);

	private:
		EYE m_eye;
		int m_deviation;
		CColorSpec m_color;
		CrossLocations_t m_unoccluded;
		CrossLocations_t m_occluded;

		/* Not supported. */
		CEyeParameters(const CEyeParameters&);
		CEyeParameters& operator=(const CEyeParameters&);
	};

	/*************************************************************************
	 *************************************************************************/

	/** Color, size, percent, and options for backgrounds. */
	class CNoiseParameters {
	public:
		CNoiseParameters(bool useNoise, bool sameBg, bool interleave, 
			bool occlude, int size, int pct, const CColorSpec& color);
		~CNoiseParameters();

		bool useNoise() const;
		bool sameBg() const;
		bool interleaveNoise() const;
		bool occludeBg() const;
		int  noiseSize() const;
		int  noisePct() const;
		const CColorSpec& color() const;

		static CNoiseParameters* getFrom(FILE* infile);
		void putTo(FILE* outfile);

	private:
		bool m_useNoise;
		bool m_sameBG;
		bool m_interleaveNoise;
		bool m_occludeBG;
		int m_noiseSize;
		int m_noisePct;
		CColorSpec m_color;

		/* Not supported. */
		CNoiseParameters(const CNoiseParameters&);
		CNoiseParameters& operator=(const CNoiseParameters&);
	};

}; // namespace

#endif // FIXATION_STABILITY_H
