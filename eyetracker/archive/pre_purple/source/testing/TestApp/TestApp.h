/**
 * @file TestApp.h
 * @brief Main header file for the TestApp application
 */

#ifndef TESTAPP_H
#define TESTAPP_H

#ifndef __AFXWIN_H__
	#error Include 'stdafx.h' before including this file for PCH
#endif

/*****************************************************************************
 * Includes
 *****************************************************************************/

/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief CWinApp-derived application container.
 * See EyeTracking.cpp for the implementation of this class.
 */
class CTestAppApp : public CWinApp
{
public:
	CTestAppApp();

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CTestAppApp theApp;

#endif // TESTAPP_H
