/**
 * @file ETMath.h
 * @brief Standardized mathematical operations and constants.
 */

#ifndef ETMATH_H
#define ETMATH_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <cstdarg>

/*****************************************************************************
 * Defines
 *****************************************************************************/

/** sqrt(2) reference value, float */
#define SQRT2_FLOAT		(1.41421356F)

/** sqrt(2) reference value, double. */
#define SQRT2_DOUBLE	(1.4142135623730950)

/** sqrt(3) reference value, float */
#define SQRT3_FLOAT		(1.73205081F)

/** sqrt(3) reference value, double. */
#define SQRT3_DOUBLE	(1.7320508075688773)

/** pi reference value, float. */
#define PI_FLOAT		(3.14159265F)

/** pi reference value, double. */
#define PI_DOUBLE		(3.1415926535897932)

/*****************************************************************************
 * Function
 *****************************************************************************/

namespace etu {

int gcf(int aa, int bb);
int gcf(int argc, const int* argv);

void reduceRatio(int argc, const int* ratio, int* reduced);
void reduceRatioV(int argc, ...);

}; // namespace etu

#endif // ETMATH_H
