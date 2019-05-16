/**
 * @file EyeTracking.cpp
 * @brief Defines the class behaviors for the application.
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "stdafx.h"
#include "AppCore.h"
#include "EyeTrackingDlg.h"
#include "EyeTracking.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Defines
 *****************************************************************************/

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*****************************************************************************
 * Message Map
 *****************************************************************************/

BEGIN_MESSAGE_MAP(CEyeTrackingApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/*****************************************************************************
 * Statics and Globals
 *****************************************************************************/

/* The one and only CEyeTrackingApp object */
CEyeTrackingApp theApp;

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CEyeTrackingApp::CEyeTrackingApp()
{
	/* Place all significant initialization in InitInstance */
}

BOOL CEyeTrackingApp::InitInstance()
{
	/* InitCommonControls() is required on Windows XP if an application
	 * manifest specifies use of ComCtl32.dll version 6 or later to enable
	 * visual styles.  Otherwise, any window creation will fail. */
	InitCommonControls();

	CWinApp::InitInstance();

	/* Call the InitApp Function in AppCore.cpp */
	if (!InitApp())
		return FALSE;

	if (!RunApp())
		return FALSE;

	m_pMainWnd = g_pEyeTrackingDlg;

	/* Added to catch kb events */
	MSG msg;
    BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
        if (bRet == -1) {
            /* handle the error and possibly exit */
        }
        else {
            TranslateMessage(&msg); 
            DispatchMessage(&msg); 

			#ifdef APP_EYE_TRACKING
				if (msg.message == WM_KEYDOWN) {
					if (g_pEyeTrackingDlg->m_pTabCtrl.GetCurSel()==13 && g_pRemember->getResponse()) {//remember dialog
						if (msg.wParam == VK_NUMPAD0) {
							/* red */
							g_pRememberDlg->response=0;
						} 
						else if (msg.wParam == VK_NUMPAD1) {
							/* both */
							g_pRememberDlg->response=1;
						} 
						else if (msg.wParam == VK_NUMPAD2) {
							/* blue */
							g_pRememberDlg->response=2;
						} 
						else if (msg.wParam == VK_NUMPAD3) {
							/* blank */
							g_pRememberDlg->response=3;
						} 
						else if (msg.wParam == VK_NUMPAD4) {
							/* ignore the last trial */
							g_pRememberDlg->response=4;
						} 
						else if (msg.wParam == 'X') {
							/* send an x textmark */
							char buff[80];
							sprintf(buff, "x|0x%2x/\0", 'x');
							g_pSettings->theSerialComm.transmit(buff, (int)strlen(buff));
						}
					} 
					else if (g_pEyeTrackingDlg->m_pTabCtrl.GetCurSel()==18) {//amblyoscope
						if (msg.wParam == 'P') {
							/* VK_NUMPAD1
							 * toggle left on and off */
							g_pAmblyoscopeDlg->lefton = !(g_pAmblyoscopeDlg->lefton);
						} 
						else if (msg.wParam == 'C') {
							/* VK_NUMPAD2
							 * toggle right on and off */
							g_pAmblyoscopeDlg->righton = !(g_pAmblyoscopeDlg->righton);
						}
					} 
					else if (g_pEyeTrackingDlg->m_pTabCtrl.GetCurSel()==19 && \
						     g_pScotomaMap->getUseKeyboard()) {
						/* monkey scotoma map */
						if (msg.wParam == VK_NUMPAD0) {
							/* red */
							g_pScotomaMapDlg->response=0;
						} 
						else if (msg.wParam == VK_NUMPAD1) {
							/* both */
							g_pScotomaMapDlg->response=1;
						} 
						else if (msg.wParam == VK_NUMPAD2) {
							/* blue */
							g_pScotomaMapDlg->response=2;
						} 
						else if (msg.wParam == VK_NUMPAD3) {
							/* blank */
							g_pScotomaMapDlg->response=3;
						} 
						else if (msg.wParam == VK_NUMPAD4) {
							/* ignore the last trial */
							g_pScotomaMapDlg->response=4;
						} 
						else if (msg.wParam == 'X') {
							/* send an x textmark */
							char buff[80];
							sprintf(buff, "x|0x%2x/\0", 'x');
							g_pSettings->theSerialComm.transmit(buff, (int)strlen(buff));
						}
					}
				} 
				else if (msg.message == WM_MOUSEWHEEL) {
					if (g_pEyeTrackingDlg->m_pTabCtrl.GetCurSel()==9) {
						/* RF mapping dialog */
						if (g_pRFMapping->getUseMouse() && g_pActiveStimulus  && \
							g_pActiveStimulus->StimulusType() == "RF Mapping Stimulus") {
							short zDelta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
							if (zDelta != 0) {
								((CRFMappingStimulus*)g_pActiveStimulus)->updateStimulus(0, zDelta/30, false);
								Sleep(1);
								g_pRFMappingDlg->m_Orientation.SetWindowText(etu::FloatToString(g_pRFMapping->getOrientation()));
							}
						}
					}
				}
			#endif
        }
    } 
	/* end kb events code */
	
	/* Since the dialog has been closed, return FALSE so that we exit the
	 * application, rather than start the application's message pump. */
	return TRUE;
}
