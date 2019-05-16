/**
 * @file Timer.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>
#include "Timer.h"

namespace etu {

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/**
 * @brief Initialize the timer and calculate API overhead.
 */
CTimer::CTimer() 
{
	LARGE_INTEGER longInt, longInt2;
	
	QueryPerformanceFrequency(&longInt);
	m_iFreq = longInt.QuadPart;

	QueryPerformanceCounter(&longInt);
	QueryPerformanceCounter(&longInt2);
	overhead = CountsToMs(longInt2.QuadPart - longInt.QuadPart);
}

/** Teardown */
CTimer::~CTimer() { /* empty */}

/**
 * @brief Start the stopwatch.
 */
void CTimer::StartTimer() 
{
	LARGE_INTEGER longInt;
	QueryPerformanceCounter(&longInt);
	m_iStartTime = longInt.QuadPart;
}

/**
 * @brief Get number of milliseconds since stopwatch was started.
 * @return Number of milliseconds since watch was started.
 */
double CTimer::CheckTimer() 
{
	LARGE_INTEGER longInt;
	QueryPerformanceCounter(&longInt);
	return (CountsToMs(longInt.QuadPart - m_iStartTime) - overhead);
}

/**
 * @brief Get number of milliseconds since stopwatch was started and reset
 * timer to time 0.
 * @return Number of milliseconds since watch was started.
 */
double CTimer::CheckTimerWithReset() 
{
	LARGE_INTEGER longInt;
	double duration;

	QueryPerformanceCounter(&longInt);	
	duration = CountsToMs(longInt.QuadPart - m_iStartTime) - overhead;
	m_iStartTime = longInt.QuadPart;

	return duration;
}

/**
 * @brief Convert the given number of counts to the number of ms it 
 * represents.
 *
 * Depends on the correct m_iFreq being set in the contructor.
 *
 * @param counts The number of ticks of the performance timer.
 * @return The number of ms represented by the given time.
 */
double CTimer::CountsToMs(LONGLONG counts)
{
	return ((double)counts / m_iFreq * 1000);
}

}; // namespace etu