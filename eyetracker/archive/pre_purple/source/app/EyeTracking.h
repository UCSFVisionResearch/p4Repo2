/**
 * @file EyeTracking.h
 * @brief Main header file for the EyeTracking application
 */

#ifndef EYETRACKING_H
#define EYETRACKING_H

#ifndef __AFXWIN_H__
	#error Include 'stdafx.h' before including this file for PCH
#endif

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "res/resource.h"		/* main symbols */

/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief CWinApp-derived application container.
 * See EyeTracking.cpp for the implementation of this class.
 */
class CEyeTrackingApp : public CWinApp
{
public:
	CEyeTrackingApp();

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CEyeTrackingApp theApp;

#endif // EYETRACKING_H
