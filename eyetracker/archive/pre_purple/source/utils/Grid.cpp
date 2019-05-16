/**
 * @file Grid.cpp
 * @brief Grid types.
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <math.h>
#include "utils/Math.h"
#include "utils/Grid.h"

namespace etu {

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

SPoint::SPoint(double xD, double yD, double aD, double rD)
: xDeg(xD), yDeg(yD), aDeg(aD), rDeg(rD)
{}

/*****************************************************************************
 * Functions
 *****************************************************************************/

/**
 * @brief Create grid for defined by parameters.
 * @param stepsX Number of steps in positive and negative X directions.
 * @param stepsY Number of steps in positive and negative Y directions.
 * @param spaceDeg Step size in degrees.
 * @param zeroPt Use the zero point on the grid; default false.
 */
pGrid_t createCartesianGrid(int stepsX, int stepsY, double spaceDeg, 
							bool zeroPt)
{
	double aDeg, rDeg, xDeg, yDeg;
	pGrid_t pGrid(new Grid_t());

	for (int xx = -stepsX; xx <= stepsX; xx++) {
		for (int yy = -stepsY; yy <= stepsY; yy++) {
			/* Not both 0 unless using zero point */
			if (zeroPt || xx || yy) {
				xDeg = xx * spaceDeg;
				yDeg = yy * spaceDeg;

				cartesianToCompass(xDeg, yDeg, aDeg, rDeg);
				pGrid->push_back(SPoint(xDeg, yDeg, aDeg, rDeg));
			}
		}
	}

	return pGrid;
}

/**
 * @brief Construct grid according to given radial parameters.
 *
 * The radial grid draws points along rays from the origin.  The rays are given 
 * by polar angles (0 degrees is north, 90 degrees east, etc).  An optional 
 * user-specified offset is added to the angle.  Along each ray, points are 
 * drawn at radii given by angles of eccentricity, again with an optional radius 
 * offset.
 *
 * @param angleOffset The angle to offset the polar angles, between 0 and 45 
 *        degrees east of north.
 * @param angles Deque of polar angles in compass-reference 
 *        (0 degrees = north), which are offset by angleOffset.  Copied.
 * @param radiusOffset The offset to the angles of eccentricity used for 
 *        radial displacement from 0; between 0 and 5 degrees.
 * @param radii Deque of radii represented as angles of eccentricity.  Copied.
 * @param radiusOrigin True if the origin point is to be included regardless 
 *        of offset; false otherwise.
 */
pGrid_t createRadialGrid(int angleOffset, const int_deque_t& angles,
						 int radiusOffset, const int_deque_t& radii, 
						 bool radiusOrigin)
{
	double aDeg, rDeg, xDeg, yDeg;
	bool doneZero = false;
	int_deque_t::const_iterator anglesEnd = angles.end();
	int_deque_t::const_iterator radiiEnd = radii.end();

	pGrid_t pGrid(new Grid_t());

	for (int_deque_t::const_iterator aI = angles.begin(); aI != anglesEnd; ++aI)
	{
		aDeg = (*aI) + angleOffset;

		for (int_deque_t::const_iterator rI = radii.begin(); rI != radiiEnd; ++rI) 
		{
			rDeg = (*rI) + radiusOffset;

			if (rDeg == 0) {
				if (doneZero) continue;
				else doneZero = true;
			}

			compassToCartesian(aDeg, rDeg, xDeg, yDeg);
			pGrid->push_back(SPoint(xDeg, yDeg, aDeg, rDeg));
		}
	}

	if (radiusOrigin && !doneZero) {
		pGrid->push_back(SPoint(0.0, 0.0, 0.0, 0.0));
	}

	return pGrid;
}

/**
 * @brief Convert cartesian coordinates to compass coordinates.
 * @param xDeg X coordinate, in degrees.
 * @param yDeg Y coordinate, in degrees.
 * @param[out] aDeg Output angle, in compass coordinates (not polar).
 * @param[out] rDeg Output radius, in degrees of eccentricity.
 */
void cartesianToCompass(double xDeg, double yDeg, double& aDeg, double& rDeg)
{
	double aRad;
	rDeg = sqrt(xDeg * xDeg + yDeg * yDeg);
	aRad = asin(xDeg / rDeg);
	aDeg = aRad * 180.0 / PI_DOUBLE;
}

/**
 * @brief Convert compass coordinates to cartesian coordinates.
 * @param aDeg Output angle, in compass coordinates (not polar).
 * @param rDeg Output radius, in degrees of eccentricity.
 * @param[out] xDeg X coordinate, in degrees.
 * @param[out] yDeg Y coordinate, in degrees.
 */
void compassToCartesian(double aDeg, double rDeg, double& xDeg, double& yDeg)
{
	double aRad = aDeg * PI_DOUBLE / 180.0;
	xDeg = rDeg * sin(aRad);
	yDeg = rDeg * cos(aRad);
}

}; // namespace etu