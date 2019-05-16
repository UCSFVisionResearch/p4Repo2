/**	
 * @file Asserts.cpp
 * @brief Implementation of debugging asserts, on by default.
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>     /* CString */
#include <afxwin.h>  /* AfxMessageBox */
#include "utils/Asserts.h"

/*****************************************************************************
 * Functions
 *****************************************************************************/

void etu::_et_assert(int cond, const char* condStr, const char* file, 
						 int line)
{
	if (!cond) {
		CString msg;
		msg.AppendFormat("Assert failed: '%s' in %s, line %d", condStr, file, line);
		AfxMessageBox((LPCTSTR)msg);
	}
}