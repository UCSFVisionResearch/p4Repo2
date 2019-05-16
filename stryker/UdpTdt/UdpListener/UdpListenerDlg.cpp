/**
 * @file UdpListenerDlg.cpp
 * @brief Implementation of the dialog.
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "stdafx.h"
#include "UdpListener.h"
#include "UdpListenerDlg.h"

/*****************************************************************************
 * Defines
 *****************************************************************************/

/* If debugging, replace heap allocation operator with a debug version. */
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*****************************************************************************
 * Implementation
 *****************************************************************************/

// CUdpListenerDlg dialog
CUdpListenerDlg::CUdpListenerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdpListenerDlg::IDD, pParent),
	m_callback(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUdpListenerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_editLocalPort, editLocalPort);
	DDX_Control(pDX, IDC_editTextOutput, editTextOutput);

	DDX_Control(pDX, IDC_comboTdtUnit, comboTdtUnit);

	DDX_Control(pDX, IDC_buttonStartSend, buttonStartSend);
	DDX_Control(pDX, IDC_buttonStopSend, buttonStopSend);

	DDX_Control(pDX, IDC_progressTrigShape, progressTrigShape);

	DDX_Control(pDX, IDC_listOutput, listOutput);
	DDX_Control(pDX, IDC_buttonQuit, buttonQuit);
}

BEGIN_MESSAGE_MAP(CUdpListenerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_buttonStartSend, OnButtonStartSend)
	ON_BN_CLICKED(IDC_buttonStopSend, OnButtonStopSend)
	ON_BN_CLICKED(IDC_buttonQuit, OnClose)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CUdpListenerDlg message handlers

BOOL CUdpListenerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Initialize combo box w/TDT types

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUdpListenerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUdpListenerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUdpListenerDlg::OnButtonStartSend()
{
	if (m_callback != NULL) m_callback->startButtonPressed(*this);
}

void CUdpListenerDlg::OnButtonStopSend()
{
	if (m_callback != NULL) m_callback->stopButtonPressed(*this);
}

void CUdpListenerDlg::OnClose()
{
	if (m_callback != NULL) m_callback->quitButtonPressed(*this);
}

void CUdpListenerDlg::setEventCallback(CEventCallback* callback)
{ 
	if (callback != NULL) m_callback = callback;
}

int CUdpListenerDlg::getLocalUdpPort() const
{
	CString portString;
	LPTSTR endPtr = NULL;
	int output; 

	editLocalPort.GetWindowText(portString);
	portString.Trim();
	output = (int)_tcstol((LPCTSTR)portString, &endPtr, 10);

	if (endPtr != NULL) {
		AfxMessageBox(_T("UDP port must be a positive integer."));
		output = 12345;
	}

	return output;
}

CString CUdpListenerDlg::getTextReceived() const
{
    // TODO: Get input in Text Received box
	return CString("foo");
}

eTdtUnit CUdpListenerDlg::getTdtUnit() const
{
    // TODO: Get TDT unit
	return TDT_UNIT_NUM_UNITS;
}

void CUdpListenerDlg::addTextReceived(const CString& text)
{
    // TODO: Add received text to GUI
}

void CUdpListenerDlg::setStatusColor(int color)
{
	// TODO: Set color of progress indicator
}
