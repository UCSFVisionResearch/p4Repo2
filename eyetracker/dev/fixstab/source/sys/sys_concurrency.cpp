/**
 * @file sys_concurrency.cpp
 * @brief Implementation of sys_concurrency.h
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>
#include <strsafe.h>
#include "sys_concurrency.h"

/*****************************************************************************
 * Functions
 *****************************************************************************/

/**
 * @brief Display a message box with an error.
 *
 * Taken directly from Microsoft.
 *
 * @param lpszFunction A string literal in _T().
 */
static void ErrorMessage(LPTSTR lpszFunction) 
{ 
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    AfxMessageBox((LPCTSTR)lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/**
 * @brief Initialize SysMutex object and underlying mutex HANDLE.
 */
SysMutex::SysMutex()  { m_mutex = CreateMutex(NULL, false, NULL); }

/**
 * @brief Cleanup underlying mutex HANDLE.
 */
SysMutex::~SysMutex() { CloseHandle(m_mutex); }

/**
 * @brief Get lock on this mutex, possibly waiting for it.
 */
void SysMutex::lock()
{
	DWORD result = WaitForSingleObject(m_mutex, INFINITE);
	if (result == WAIT_FAILED) {
		ErrorMessage(_T("SysMutex::lock"));
	}
}

/**
 * @brief Release previously acquired lock on this mutex.
 */
void SysMutex::unlock()
{
	DWORD result = ReleaseMutex(m_mutex);
	if (result == 0) {
		ErrorMessage(_T("SysMutex::unlock"));
	}
}

/*****************************************************************************/

/**
 * @brief Create critical section object that locks the given mutex.
 * @param mutex The mutex object to lock at initialization, unlock at tear-
 * down.
 */
SmartLock::SmartLock(SysMutex &mutex)
: m_mutex(mutex)
{
	m_mutex.lock();
}

/**
 * @brief Create critical section object that locks the given object's mutex.
 * @param mutex The mutex object to lock at initialization, unlock at tear-
 * down.
 */
SmartLock::SmartLock(Mixin_ThreadAware& threadAware)
: m_mutex(threadAware.getMutex())
{
	m_mutex.lock();
}

/**
 * @brief Release the underlying mutex lock.
 */
SmartLock::~SmartLock()
{
	m_mutex.unlock();
}

/*****************************************************************************/

/** Init; nothing to do; called automatically. */
Mixin_ThreadAware::Mixin_ThreadAware() {}

/** Tear down; nothing to do; called automatically. */
Mixin_ThreadAware::~Mixin_ThreadAware() {}

/**
 * @brief Get mutex.
 * @return SysMutex object for locking.
 */
SysMutex& Mixin_ThreadAware::getMutex() const { return m_mixinThreadAware_Mutex; }
