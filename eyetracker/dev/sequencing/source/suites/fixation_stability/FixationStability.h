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
#include "utils/ModelUtils.h"

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
		CEyeParameters(EYE eye, int dev, const etu::CColorSpec& color,
			const CrossLocations_t& unoccluded, 
			const CrossLocations_t& occluded);
		~CEyeParameters();

		EYE eye() const;
		int deviation() const;
		const etu::CColorSpec& color() const;
		const CrossLocations_t& unoccluded() const;
		const CrossLocations_t& occluded() const;

		static CEyeParameters* getFrom(FILE* infile);
		void putTo(FILE* outfile);

	private:
		EYE m_eye;
		int m_deviation;
		etu::CColorSpec m_color;
		CrossLocations_t m_unoccluded;
		CrossLocations_t m_occluded;

		/* Not supported. */
		CEyeParameters(const CEyeParameters&);
		CEyeParameters& operator=(const CEyeParameters&);
	};

}; // namespace

#endif // FIXATION_STABILITY_H
