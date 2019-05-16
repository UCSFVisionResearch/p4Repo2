/**
 * @file Timer.h
 * @brief 
 */

#ifndef TIMER_H
#define TIMER_H

namespace etu {

/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief Stopwatch-like timer class.
 */
class CTimer 
{
public:
	CTimer();
	~CTimer();

	void StartTimer();
	double CheckTimer();
	double CheckTimerWithReset();

private:
	double m_iFreq;
	double overhead;
	LONGLONG m_iStartTime;	

	double CountsToMs(LONGLONG counts);
};

}; // namespace etu

#endif // TIMER_H
