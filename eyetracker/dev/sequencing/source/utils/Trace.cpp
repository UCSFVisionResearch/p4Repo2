/**
 * @file Trace.h
 * @brief Debugging trace.
 *
 * Adapted from XY Trace by Xiangyang Liu, 
 * http://www.codeproject.com/script/Articles/ViewDownloads.aspx?aid=1710
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h> /* CString class */
#include <cstdio>
#include <ctime>
#include "sys/sys_concurrency.h"
#include "utils/Trace.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief Private helper class.
 */
class EtTraceHelper : public Mixin_ThreadAware
{
	/* Friend functions of this class */
	friend void etu::SetFilePrefix(LPCTSTR strFilePrefix);
	friend void etu::SetLevel(const etu::ET_TRACE_LEVEL nLevel);
	friend void etu::Write(const etu::ET_TRACE_LEVEL nLevel, LPCTSTR strFormat, ...);
	
public:
	EtTraceHelper();
	~EtTraceHelper();

private:
	tm    m_timeStart;
	FILE* m_pFile;
	long  m_nThreadId;
	CString  m_pTraceFilePrefix;
	SysMutex m_mutex;
	etu::ET_TRACE_LEVEL m_nLevel;

	void CloseTraceFile();
	FILE* OpenTraceFile();
	void SetTraceLevel(const etu::ET_TRACE_LEVEL nLevel);
	void SetTraceFilePrefix(LPCTSTR strFilePrefix);
	
	/* Not supported */
	EtTraceHelper(const EtTraceHelper&);
	EtTraceHelper& operator=(const EtTraceHelper&);
};

/*****************************************************************************
 * Static and Global Variables
 *****************************************************************************/

/** The one and only instance of EtTraceHelper */
static EtTraceHelper s_theHelper;

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/** 
 * @brief  Constructor. 
 */
EtTraceHelper::EtTraceHelper() 
: m_pFile(NULL), m_nLevel(etu::ET_TRACE_NONE), m_nThreadId(0)
{}

/**
 * @brief Destructor. 
 */
EtTraceHelper::~EtTraceHelper() { CloseTraceFile(); }

/** 
 * @brief Close the current trace file 
 */
void EtTraceHelper::CloseTraceFile() 
{
	if (m_pFile) {
		fclose(m_pFile);
	}
	m_pFile = NULL;
}

/** 
 * @brief Open a new trace file 
 */
FILE* EtTraceHelper::OpenTraceFile() 
{
	/* Construct the new trace file path */
	CString strFilePath;
	time_t sysTime;
	tm* timeInfo;

	time(&sysTime);
	timeInfo = localtime(&sysTime);

	strFilePath.AppendFormat("%s_%4d-%02d-%02d_%02d-%02d-%02d_0x%X.txt",
		m_pTraceFilePrefix.IsEmpty() ? "Trace" : (LPCTSTR)m_pTraceFilePrefix,
		timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday,
		timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec,
		::GetCurrentProcessId());

	/* Create the new trace file */
	m_pFile = fopen((LPCTSTR)strFilePath, "w");

	/* If successful, copy the time info into our struct */
	if (m_pFile) {
		m_timeStart = (*timeInfo);
	}

	return m_pFile;
}

/** 
 * @brief Set the current trace level 
 */
void EtTraceHelper::SetTraceLevel(const etu::ET_TRACE_LEVEL nLevel) 
{ 
	m_nLevel = nLevel > 0 ? nLevel : etu::ET_TRACE_NONE; 
}

/** 
 * @brief Set the trace file name prefix 
 * @param strFilePrefix The new prefix, optionally a directory path or file 
 *        name.  Examples:
 *        -- "C:\\temp\\foo"  A path and name fragment
 *        -- "foo"            A name fragment only
 */
void EtTraceHelper::SetTraceFilePrefix(LPCTSTR strFilePrefix) 
{
	/* Close existing trace file first */
	CloseTraceFile();
	/* Copy the file name prefix */
	m_pTraceFilePrefix = strFilePrefix;
}

/*****************************************************************************
 * Functions
 *****************************************************************************/

namespace etu {

/**
 * @brief Set the file prefix for the trace file.
 * @param strFilePrefix The new prefix, optionally a directory path or file 
 *        name.  Examples:
 *        -- "C:\\temp\\foo"  A path and name fragment
 *        -- "foo"            A name fragment only
 */
void SetFilePrefix(LPCTSTR strFilePrefix)
{
	SmartLock helperLock(s_theHelper);
	s_theHelper.SetTraceFilePrefix(strFilePrefix);
}

/**
 * @brief Set the new logging level.
 * @param nLevel The new level.
 */
void SetLevel(const ET_TRACE_LEVEL nLevel)
{
	SmartLock helperLock(s_theHelper);
	s_theHelper.SetTraceLevel(nLevel);
}

/**
 * @brief Write trace output at the given level.
 * @param nLevel The level for the trace; not printed if > current level.
 * @param strFormat Format string for message; 0 or more var args follow.
 */
void Write(const ET_TRACE_LEVEL nLevel, LPCTSTR strFormat, ...)
{
	SmartLock helperLock(s_theHelper);
	time_t sysTime;
	tm* timeInfo;
	FILE* fileHandle;

	/* Log only if nLevel <= current logging level, provided not 0. */
	if ((s_theHelper.m_nLevel == ET_TRACE_NONE) || 
		(nLevel > s_theHelper.m_nLevel)) 
	{
		return;
	}

	time(&sysTime);
	timeInfo = localtime(&sysTime);
	fileHandle = s_theHelper.m_pFile;
	
	if (fileHandle == NULL) {
		fileHandle = s_theHelper.OpenTraceFile();
	}
	else if (
		(timeInfo->tm_year != s_theHelper.m_timeStart.tm_year) ||
		(timeInfo->tm_mon  != s_theHelper.m_timeStart.tm_mon)  ||
		(timeInfo->tm_mday != s_theHelper.m_timeStart.tm_mday))
	{
		s_theHelper.CloseTraceFile();
		s_theHelper.OpenTraceFile();
	}

	if (fileHandle) {
		CString traceString;
		traceString.Format("%02d:%02d:%02d_%X %s ", timeInfo->tm_hour, 
			timeInfo->tm_min, timeInfo->tm_sec, s_theHelper.m_nThreadId,
			LevelToString(nLevel));
		
		va_list args;
		va_start(args, strFormat);
		traceString.AppendFormatV(strFormat, args);
		va_end(args);

		traceString.AppendFormat("\r\n");

		fprintf(fileHandle, (LPCTSTR)traceString);
	}
}

/**
 * @brief Get string for the logging level.
 * @brief nLevel The level to convert to a human-readable string.
 * @return Human-readable string, e.g. "ERROR".
 */
LPCTSTR LevelToString(ET_TRACE_LEVEL nLevel)
{

	switch (nLevel) {
		case ET_TRACE_NONE:
			return ("(none) ");
			break;
		case ET_TRACE_ERROR:
			return ("ERROR  ");
			break;
		case ET_TRACE_INFO:
			return ("Info   ");
			break;
		case ET_TRACE_DEBUG:
			return ("Dbg    ");
			break;
		default:
			return ("??     ");
			break;
	}
}

}; // namespace etu
