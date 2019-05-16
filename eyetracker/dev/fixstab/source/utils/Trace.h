/**
 * @file Trace.h
 * @brief Debugging trace.
 *
 * Adapted from XY Trace by Xiangyang Liu, 
 * http://www.codeproject.com/script/Articles/ViewDownloads.aspx?aid=1710
 */

#ifndef TRACE_H
#define TRACE_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include <cstdio>

namespace etu {

/*****************************************************************************
 * Types
 *****************************************************************************/

/**
 * @brief Numerical trace level.
 * The various constants allow filtering of trace messages.
 */
enum ET_TRACE_LEVEL {
	ET_TRACE_NONE  = 0,  /**< All calls to write a trace are ignored. */
	ET_TRACE_ERROR = 10, /**< Run-time error that user should know about. */
	ET_TRACE_INFO  = 20, /**< Info on important program events. */
	ET_TRACE_DEBUG = 30  /**< Detailed info on program events. */
};

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

extern void SetFilePrefix(LPCTSTR strFilePrefix);
extern void SetLevel(ET_TRACE_LEVEL nLevel);
extern void Write(ET_TRACE_LEVEL nLevel, LPCTSTR strFormat, ...);
extern LPCTSTR LevelToString(ET_TRACE_LEVEL nLevel);

} // namespace etu

#endif // TRACE_H

