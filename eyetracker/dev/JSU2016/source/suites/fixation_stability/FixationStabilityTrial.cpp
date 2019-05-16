/**
 * @file FixationStabilityTrial.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include "FixationStability.h"
#include "FixationStabilityTrial.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/* List of codes of trial types. 
 * First 6 = unoccluded		Second 6 = occluded
 *    First 3 = right eye	    First 3 = right eye
 *    Second 3 = left eye       Second 3 = left eye
 * For each 3, the sequence is center, half, and full.
 */
static int s_trialCodes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

namespace fstab {

	/*************************************************************************/

	/** Initialize */
	CFixationStabilityTrial::CFixationStabilityTrial(EYE eye, int xDeg, 
													 int yDeg, bool occLeft, 
													 bool occRight, double loc)
		: m_eye(eye),
		  m_xDeg(xDeg),
		  m_yDeg(yDeg),
		  m_occludeLeft(occLeft),
		  m_occludeRight(occRight),
		  m_location(loc)
	{}

	/** Destroy; nothing to do. */
	CFixationStabilityTrial::~CFixationStabilityTrial() {}

	EYE CFixationStabilityTrial::eye() const { return m_eye; }
	int CFixationStabilityTrial::xDeg() const { return m_xDeg; }
	int CFixationStabilityTrial::yDeg() const { return m_yDeg; }
	bool CFixationStabilityTrial::occludeLeft() const { return m_occludeLeft; }
	bool CFixationStabilityTrial::occludeRight() const { return m_occludeRight; }

	int CFixationStabilityTrial::getCode() const
	{
		/* List of codes of trial types. 
		 * First 6 = unoccluded		Second 6 = occluded
		 *    First 3 = right eye	    First 3 = right eye
		 *    Second 3 = left eye       Second 3 = left eye
		 * For each 3, the sequence is center, half, and full.
		 */
		int occIx = (int)(m_occludeLeft || m_occludeRight);
		int eyeIx = (int)(m_eye == LEFT_EYE);
		int locIx = (int)(m_location * 2);
		int ix = (6 * occIx) + (3 * eyeIx) + locIx;
		int retval = 255;

		if (ix < (sizeof s_trialCodes)) {
			retval = s_trialCodes[ix];
		}

		return retval;
	}

	/*************************************************************************/

} // namespace fstab