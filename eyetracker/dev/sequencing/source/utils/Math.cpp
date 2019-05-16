/**
 * @file Math.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <cstdarg>
#include <vector>
#include "utils/Utils.h"
#include "utils/Math.h"

/*****************************************************************************
 * Functions
 *****************************************************************************/

namespace etu {

/**
 * @brief Calculate the greatest common factor of two integers.
 * Calculate the greatest common factor (or greatest common divisor) of
 * two integers by the Euclidean algorithm.  All integers must be positive 
 * or 0.  As a mild optimization, it checks for arguments of 1, 2, or 3, 5,
 * or 7, and short-circuits.
 * @param aa First integer > 0.
 * @param bb Second integer > 0.
 * @return The greatest common factor of the integers.
 */
int gcf(int aa, int bb)
{
	if ((aa == 1) || (bb == 1)) {
		return 1;
	}
	else {
		int primes[] = { 2, 3, 5, 7 };
		for (int ix = 0; ix < (sizeof primes)/sizeof(int); ix++) {
			if ((aa == primes[ix]) || (bb == primes[ix])) {
				int notPrime = (aa == primes[ix]) ? bb : aa;
				return (((notPrime % primes[ix]) == 0) ? primes[ix] : 1);
			}
		}

		/* Euclidean alg starts here in new scope. */
		{
			int minInt;
			while (aa != bb) {
				minInt = ETU_MIN(aa, bb);
				aa = ETU_MAX(aa, bb) - minInt;
				bb = minInt;
			}

			return aa;
		}
	}
}

/**
 * @brief Calculate the greatest common factor of some integers.
 * Calculate the greatest common factor (or greatest common divisor) by the 
 * Euclidean algorithm.  All integers must be positive or 0.  The GCF of
 * one integer is that integer.  The GCF of 0 integers is undefined.
 * @param argc Number of integers, one or more.
 * @param argv Array of the integers, all > 0.
 * @return The greatest common factor of all integers.
 */
int gcf(int argc, const int* argv)
{
	if (argc <= 0) return 0;
	else if (argc == 1) return argv[0];
	else {
		int retval = gcf(argv[0], argv[1]);
		for (int ix = 2; ix < argc; ix++) {
			retval = gcf(retval, argv[ix]);
		}
		return retval;
	}
}

/**
 * @brief Reduce a ratio into its smallest integer representation.
 * Convert a ratio of an arbitrary number of integers into the ratio with 
 * smallest integers that preserves the original ratio.  One particularly 
 * useful application is to reduce percentages (which sum to 100) to a
 * ratio.
 * If any of the integers in ratio is 0, it remains 0 in the reduced ratio.
 * @param argc The number of integers in the ratio.  
 * @param[in] ratio The ratio to reduce.
 * @param[out] reduced The reduced ratio.  This array may be the same as
 *     ratio.  
 */
void reduceRatio(int argc, const int* ratio, int* reduced)
{
	std::vector<int> ratioVector;
	int gcfArgs = 1;
	
	for (int ix = 0; ix < argc; ix++) {
		if (ratio[ix] > 0) {
			ratioVector.push_back(ratio[ix]);
		}
	}

	if (ratioVector.size() > 0) {
		gcfArgs = gcf(ratioVector.size(), (int*)&ratioVector[0]);
	}

	for (int ix = 0; ix < argc; ix++) {
		reduced[ix] = ratio[ix] / gcfArgs;
	}
}

/**
 * @brief Reduce a ratio into its smallest integer representation.
 * Convert a ratio of an arbitrary number of integers into the ratio with 
 * smallest integers that preserves the original ratio.  One particularly 
 * useful application is to reduce percentages (which sum to 100) to a
 * ratio.
 * If any of the integers in the varargs is 0, it remains 0 in the reduced 
 * ratio.
 * @param argc The number of integers in the ratio.  There must be argc 
 * integers in the variable argument list, followed by argc integer pointers
 * to receive the output.
 */
void reduceRatioV(int argc, ...)
{
	/* Making this function variadic results in some mess, but I originally
	 * wrote it to avoid declaring the array needed by gcf(int, int*). */
	std::vector<int> ratio;
	std::vector<int> editedRatio;
	va_list varargs;
	int  cArg;
	int  gcfArgs = 1;
	int* current;

	va_start(varargs, argc);

	for (int ix = 0; ix < argc; ix++) {
		cArg = va_arg(varargs, int);
		ratio.push_back(cArg);
		if (cArg > 0) {
			editedRatio.push_back(cArg);
		}
	}
	
	/* Standard guarantees that std::vector stores elements contiguously,
	 * thus &ratio[0] is "getting an int[] from vector". */
	if (editedRatio.size() > 0) {
		gcfArgs = gcf(editedRatio.size(), &editedRatio[0]);
	}

	for (int ix = 0; ix < argc; ix++) {
		current = va_arg(varargs, int*);
		(*current) = ratio[ix] / gcfArgs;
	}

	va_end(varargs);
}

}; // namespace etu
