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

int Round(double);
int StringToInt(char*, int);
CString IntToString(int);
float StringToFloat(char*, float);
CString FloatToString(float);
CString BrowseForFile(bool, CString, CString, CString, bool = true);
void AnalogOut(int, float, bool = true);
float DegToRad(float degrees);
int ChooseRandom(int min, int max);
bool RandomChoice(double prob);

}; // namespace etu

#endif
