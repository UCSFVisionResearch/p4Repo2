/**
 * @file ETUtils.h
 * @brief 
 */

#ifndef ETUTILS_H
#define ETUTILS_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>	/* CString */
#include "vsgext/vsgext.h"

/*****************************************************************************
 * Defines
 *****************************************************************************/

/**
 * @brief Return the maximum of comparable objects aa and bb.
 * Objects aa and bb must support comparison by >.
 * @param aa Object that supports operator >.
 * @param bb Object for which aa.operator >(bb) is supported.
 * @return aa if aa > bb; bb otherwise.
 */
#define ETU_MAX(aa, bb) (((aa) > (bb)) ? (aa) : (bb))

/**
 * @brief Return the minimum of comparable objects aa and bb.
 * Objects aa and bb must support comparison by <.
 * @param aa Object that supports operator <.
 * @param bb Object for which aa.operator <(bb) is supported.
 * @return aa if aa < bb; bb otherwise.
 */
#define ETU_MIN(aa, bb) (((aa) < (bb)) ? (aa) : (bb))

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

namespace etu {

// Numeric functions +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int Round(double);

// Conversion functions ++++++++++++++++++++++++++++++++++++++++++++++++++++++
int    StringToInt(char*, int);
double StringToFloat(char*, double);
CString IntToString(int);
CString FloatToString(double);

double DegToRad(double degrees);

VSGTRIVAL RgbToTrival(int rr, int gg, int bb);
VSGTRIVAL ColorrefToTrival(COLORREF cc);
void TrivalToRGB(VSGTRIVAL& vt, int& rr, int& gg, int& bb);

// I/O facilities ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CString BrowseForFile(bool, CString, CString, CString, bool = true);
void AnalogOut(int, float, bool = true);

// Randomness facilities +++++++++++++++++++++++++++++++++++++++++++++++++++++
int RandChoice(int min, int max);
bool RandCoinFlip(double prob = 0.5);

    // LEGACY NAMES
    inline int ChooseRandom(int min, int max) { return RandChoice(min, max); }
    inline bool RandomChoice(double prob = 0.5) { return RandCoinFlip(prob); }

}; // namespace etu

#endif
