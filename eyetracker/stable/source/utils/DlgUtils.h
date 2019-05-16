/**
 * @file DlgUtil.h
 * @brief Utility types, objects, and functions for working with dialogs.
 *
 */

#ifndef DLG_UTIL_H
#define DLG_UTIL_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include <afxwin.h>
#include "ModelUtils.h"

/*****************************************************************************
 * Namespace etu
 *****************************************************************************/

namespace etu {

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

		RGBTRIVAL get();
		void set(const RGBTRIVAL& color);

	private:
		RGBTRIVAL m_rgb;

		/* Not supported. */
		CColorControls(const CColorControls&);
		CColorControls& operator=(const CColorControls&);
	};

	/*************************************************************************
	 *************************************************************************/

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
		CEdit cNoiseCoverage;
		CEdit cNoiseCommon;
		CColorControls cColor;

		void apply();
		void update();

		bool useNoise() const;
		bool sameBg() const;
		bool interleaveNoise() const;
		bool occludeBg() const;
		int  noiseSize() const;
		int  noiseCoverage() const;
		int  noiseCommon() const;

		void useNoise(bool use);
		void sameBg(bool same);
		void interleaveNoise(bool inter);
		void occludeBg(bool occlude);
		void noiseSize(int size);
		void noiseCoverage(int pct);
		void noiseCommon(int pct);

		CNoiseParameters* parameters();
		void parameters(const CNoiseParameters& params);

		void setEnabled(bool enabled);

	private:
		int m_noiseSize;
		int m_noiseCoverage;
		int m_noiseCommon;
	};
};

#endif // DLG_UTIL_H
