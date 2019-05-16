/**
 * @file TestApp.cpp
 * @brief Defines the class behaviors for the application.
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <cstdlib>
#include <cstdio>
#include "stdafx.h"
#include "utils/Utils.h"
#include "utils/Math.h"
#include "TestApp.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief Console wrapper.
 */
class EtConsole {
public:
	~EtConsole();
	bool WriteLine(LPCTSTR format, ...);

	static void Init();
	static void DeInit();
	static EtConsole& GetInst();

private:
	EtConsole();

	HANDLE m_consoleOut;
	static EtConsole* m_instance;

	/* Not supported. */
	EtConsole(const EtConsole&);
	EtConsole& operator=(const EtConsole&);
};

/* Null-initialize. */
EtConsole* EtConsole::m_instance = NULL;

/*****************************************************************************
 * Defines
 *****************************************************************************/

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*****************************************************************************
 * Message Map
 *****************************************************************************/

BEGIN_MESSAGE_MAP(CTestAppApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/*****************************************************************************
 * Statics and Globals
 *****************************************************************************/

/* The one and only CEyeTrackingApp object */
CTestAppApp theApp;

/** Pointer for second thread that will write to the log file. */
static CWinThread* s_pThread;

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/** Allocate console via Windows API. */
EtConsole::EtConsole()
{
	if (AllocConsole()) {
		m_consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);

		if ((m_consoleOut == NULL) || (m_consoleOut == INVALID_HANDLE_VALUE)) {
			AfxMessageBox("Could not open console.");
			m_consoleOut = NULL;
		}
	}
}

/** Free console via Windows API. */
EtConsole::~EtConsole()
{
	FreeConsole();
	m_consoleOut = NULL;
}

/** Make sure console has been initialized. */
void EtConsole::Init()
{
	if (m_instance == NULL) {
		m_instance = new EtConsole();
	}
}

/** Free class-static data stored on heap. */
void EtConsole::DeInit()
{
	if (m_instance != NULL) {
		delete m_instance;
	}
	m_instance = NULL;
}

/** Get allocated instance (allocating first if needed). */
EtConsole& EtConsole::GetInst()
{
	Init();
	return (*m_instance);
}

/** Write line to console, returning FALSE for failure. */
bool EtConsole::WriteLine(LPCTSTR format, ...)
{
	CString message;
	DWORD numChars;
	bool retVal = FALSE;

	if (m_consoleOut != NULL) {
		va_list vaArgs;
		va_start(vaArgs, format);
		message.FormatV(format, vaArgs);	
		va_end(vaArgs);

		WriteConsole(m_consoleOut, (LPCTSTR)message, message.GetLength(), 
			&numChars, NULL);
		
		retVal = (message.GetLength() == numChars);
	}

	return retVal;
}

/*****************************************************************************/

CTestAppApp::CTestAppApp()
{
	/* Place all significant initialization in InitInstance */
}

BOOL CTestAppApp::InitInstance()
{
	/* InitCommonControls() is required on Windows XP if an application
	 * manifest specifies use of ComCtl32.dll version 6 or later to enable
	 * visual styles.  Otherwise, any window creation will fail. */
	InitCommonControls();

	CWinApp::InitInstance();

	
	
	/* Since the dialog has been closed, return FALSE so that we exit the
	 * application, rather than start the application's message pump. */
	return TRUE;
}
