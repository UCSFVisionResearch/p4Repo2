/**
 * @file UdpListenerDlg.h
 * @brief 
 */

#ifndef UDP_LISTENER_DLG_H
#define UDP_LISTENER_DLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "stdafx.h"
#include "UdpListenerTypes.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief The dialog for interacting graphically with the user.
 */
class CUdpListenerDlg : public CDialog
{
public:
	/* Standard constructor. */
	CUdpListenerDlg(CWnd* pParent = NULL);

	/* Dialog Data (MFC magic) */
	enum { IDD = IDD_UDPLISTENER_DIALOG };

	/** 
	 * @brief Interface for callbacks to receive dialog events.
	 */
	class CEventCallback
	{
	public:
		virtual void startButtonPressed(const CUdpListenerDlg& dlg) =0;
		virtual void stopButtonPressed(const CUdpListenerDlg& dlg) =0;
		virtual void quitButtonPressed(const CUdpListenerDlg& dlg) =0;
	};

	void setEventCallback(CEventCallback* callback);

	int getLocalUdpPort() const;
	CString getTextReceived() const;
	eTdtUnit getTdtUnit() const;

	void addTextReceived(const CString& text);

	void setStatusColor(int color);

	/* Objects corresponding to dialog controls (must be public) */
	CEdit editLocalPort;
	CEdit editTextOutput;
	
	CComboBox comboTdtUnit;

	CButton buttonStartSend;
	CButton buttonStopSend;

	CProgressCtrl progressTrigShape;

	CListCtrl listOutput;

	CButton buttonQuit;

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	/* Generated message map functions. */
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStartSend();
	afx_msg void OnButtonStopSend();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

private:
	CEventCallback* m_callback;
};


#endif // UDP_LISTENER_DLG_H
