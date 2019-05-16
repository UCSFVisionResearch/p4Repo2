/**
 * @file Grid.h
 * @brief Grid types.
 */

#ifndef GRID_H
#define GRID_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <deque>
#include <utility>
#include <memory>

namespace etu {

/*****************************************************************************
 * Types
 *****************************************************************************/

/** 
 * @brief Typedef for container of integers (convenience).
 **/
typedef std::deque<int> int_deque_t;

/**
 * @brief Structure to hold four doubles: X, Y, angle, radius.
 */
struct SPoint {
	SPoint(double xD, double yD, double aD, double rD);
	double xDeg;
	double yDeg;
	double aDeg;
	double rDeg;
};

/** 
 * @brief Typedef for container of points in cartesian and polar.
 */
typedef std::deque<const SPoint> Grid_t;

/**
 * @brief Typedef for auto pointer to grid.
 */
typedef std::auto_ptr<Grid_t> pGrid_t;

/**
 * @brief Typedef for iterator of points in grid (convenience).
 */
typedef Grid_t::const_iterator GridIter_t;

/*****************************************************************************
 * Function Prototypes
 *****************************************************************************/

pGrid_t createCartesianGrid(int stepsX, int stepsY, double spaceDeg, 
							bool zeroPt, double xOffsetDeg = 0.0, 
							double yOffsetDeg = 0.0);
pGrid_t createRadialGrid(int angleOffset, const int_deque_t& angles, 
						 int radiusOffset, const int_deque_t& radii, 
						 bool radiusOrigin);

void cartesianToCompass(double xDeg, double yDeg, double& aDeg, double& rDeg);
void compassToCartesian(double aDeg, double rDeg, double& xDeg, double& yDeg);

}; // namespace etu

#endif // GRID_H
