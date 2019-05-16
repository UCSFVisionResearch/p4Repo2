/**
 * @file ScotomaTrial.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <ostream>
#include <algorithm>
#include "app/AppCore.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/**
 * @brief Create new trial for given code, class, and X,Y coordinates.
 * @param stimCode Code for the stimulus.  See STIM_CODE.
 * @param stimClass Class for the stimulus.  See STIM_CLASS.
 * @param xDeg X coordinate, in degrees.
 * @param yDeg Y coordinate, in degrees.
 */
CScotomaTrial::CScotomaTrial(STIM_CODE stimCode, STIM_CLASS stimClass, 
							 double xDeg, double yDeg)
: m_stimCode(stimCode), m_stimClass(stimClass), m_xDeg(xDeg), m_yDeg(yDeg)
{}

/**
 * @brief Clean up.
 */
CScotomaTrial::~CScotomaTrial() { /* Empty. */}

/**
 * @brief Create a duplicate of this trial with same state.
 *
 * Trials are deliberately immutable once created.  This method offers a 
 * specific way of creating duplicates.
 *
 * @return A trial with state duplicated from this one.
 */
CScotomaTrial* CScotomaTrial::clone() 
{
	return (new CScotomaTrial(m_stimCode, m_stimClass, m_xDeg, m_yDeg));
}

/*****************************************************************************
 * Functions
 *****************************************************************************/

/**
 * @brief Put a trial to an out stream in a readable format.
 *
 * The trial's central and peripheral colors are written, followed by the 
 * dimensions.  The codes are finally printed as a logic check.
 *
 * @param out Output stream.
 * @param trial The trial.
 */
std::ostream& operator<< (std::ostream& out, CScotomaTrial& trial)
{
	out << (trial.isCenterRed() ? "Red" : "Blue") << "\t"
		<< (trial.isPeriphRed() ? "Red" : 
			(trial.isPeriphBlue() ? "Blue" : 
				(trial.isPeriphBlank() ? "Blank" : "Purple"))) << "\t"
		<< trial.getXDeg() << "\t" << trial.getYDeg() << "\t"
		<< trial.getStimCode() << "\t" << trial.getStimClass() << "\t"
		<< std::endl;
	return out;
}
