/**
 * @file UdpListener.h
 * @brief Declares the CWinAppEx subclass CUdpListenerApp.
 */

#ifndef UDP_LISTENER_H
#define UDP_LISTENER_H

/****************************************************************************
 * Includes
 ****************************************************************************/

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		/* Main symbols */
#include "UdpListenerDlg.h"

/****************************************************************************
 * Types
 ****************************************************************************/

/**
 * @brief The mandatory application class derived from CWinAppEx.
 *
 * Note that it also implements the interface to be a callback for dialog
 * events.
 *
 * There should be a singleton (one and only one) instance of this class,
 * implemented in the corresponding .cpp file and accessed via the extern
 * declaration in this file. 
 */
class CUdpListenerApp : public CWinApp, CUdpListenerDlg::CEventCallback
{
public:
	CUdpListenerApp();

	virtual BOOL InitInstance();

	virtual void startButtonPressed(const CUdpListenerDlg& dlg);
	virtual void stopButtonPressed(const CUdpListenerDlg& dlg);
	virtual void quitButtonPressed(const CUdpListenerDlg& dlg);

	DECLARE_MESSAGE_MAP()

private:
	/* Copy and assignment are not supported */
	CUdpListenerApp(const CUdpListenerApp&);
	CUdpListenerApp& operator=(const CUdpListenerApp&);
};

/****************************************************************************
 * External variables
 ****************************************************************************/

/** The global singleton instance of the app. */
extern CUdpListenerApp g_udpListenerApp;

#endif // UDP_LISTENER_H
