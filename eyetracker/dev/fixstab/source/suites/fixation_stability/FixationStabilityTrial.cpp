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

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

namespace fstab {

	/*************************************************************************/

	/** Initialize */
	CFixationStabilityTrial::CFixationStabilityTrial(EYE eye, int xDeg, 
													 int yDeg, bool occLeft, 
													 bool occRight)
		: m_eye(eye),
		  m_xDeg(xDeg),
		  m_yDeg(yDeg),
		  m_occludeLeft(occLeft),
		  m_occludeRight(occRight)
	{}

	/** Destroy; nothing to do. */
	CFixationStabilityTrial::~CFixationStabilityTrial() {}

	EYE CFixationStabilityTrial::eye() const { return m_eye; }
	int CFixationStabilityTrial::xDeg() const { return m_xDeg; }
	int CFixationStabilityTrial::yDeg() const { return m_yDeg; }
	bool CFixationStabilityTrial::occludeLeft() const { return m_occludeLeft; }
	bool CFixationStabilityTrial::occludeRight() const { return m_occludeRight; }

	/*************************************************************************/

} // namespace fstab