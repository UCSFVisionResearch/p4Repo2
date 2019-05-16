/**
 * @file Asserts.h
 * @brief Debugging asserts and other macros for the EyeTracking code.
 *
 * To use these asserts, define the preprocessor symbol ET_DEBUG to be a true
 * value, e.g.
 *     -DET_DEBUG=1
 */

#ifndef ASSERTS_H
#define ASSERTS_H

/*****************************************************************************
 * Defines
 *****************************************************************************/

/* Debugging asserts are turned on by default. */
#ifndef ET_DEBUG
#define ET_DEBUG 1
#endif

#define ETU_DEBUG_BLOCK            \
	if (ET_DEBUG)

#define ETU_ASSERT(cond)           \
	if (ET_DEBUG) etu::_et_assert((cond), #cond, __FILE__, __LINE__)


/*****************************************************************************
 * Prototypes
 *****************************************************************************/

namespace etu {

/* Private; assertion functions. */
void _et_assert(int, const char*, const char*, int);

};

#endif // ASSERTS_H
