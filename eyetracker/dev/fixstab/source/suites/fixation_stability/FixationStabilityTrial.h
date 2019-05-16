/**
 * @file FixationStabilityTrial.h
 * @brief 
 */

#ifndef FIXATION_STABILITY_TRIAL_H
#define FIXATION_STABILITY_TRIAL_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include <deque>
#include "VSGV8.H"
#include "comm/SerialComm.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

namespace fstab 
{
	class CFixationStabilityTrial {
	public:
		CFixationStabilityTrial(EYE eye, int xDeg, int yDeg, bool occLeft, 
			bool occRight);
		~CFixationStabilityTrial();

		EYE eye() const;
		int xDeg() const;
		int yDeg() const;
		bool occludeLeft() const;
		bool occludeRight() const;

	private:
		EYE m_eye;
		int m_xDeg, m_yDeg;
		bool m_occludeLeft, m_occludeRight;
	};

	typedef std::deque<CFixationStabilityTrial> TrialList_t;
	typedef TrialList_t::const_iterator const_TrialIter_t;

}; // namespace

#endif // FIXATION_STABILITY_TRIAL_H
