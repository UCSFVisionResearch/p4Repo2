/**
 * @file sys_concurrency.h
 * @brief Facilities for functions, types, and objects used across multiple 
 * threads.
 */

#ifndef SYS__CONCURRENCY_H
#define SYS__CONCURRENCY_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

/* Forward declaration. */
class Mixin_ThreadAware;

/*****************************************************************************/

/**
 * @brief Class interface to native Windows mutex behavior.
 *
 * Declare one and initialize, either statically or via the new operator.
 * Acquire the lock when needed; release when no longer needed.  Supports
 * reentry from the same thread just like the underlying mutex HANDLE.
 */
class SysMutex
{
public:
	SysMutex();
	~SysMutex();

	void lock();
	void unlock();

private:
	HANDLE m_mutex;

	/* Not supported. */
	SysMutex(const SysMutex&);
	SysMutex& operator=(const SysMutex&);
};

/*****************************************************************************/

/**
 * @brief RAII type.
 *
 * RAII -- Resource Acquisition is Initialization.  SmartLock acquires a lock
 * on the given mutex in constructor and releases the lock in the destructor.
 * Thus, to acquire a mutex lock safely, declare a SmartLock as a local
 * stack variable and pass in the desired mutex.  Done.
 */
class SmartLock
{
public:
	explicit SmartLock(SysMutex& mutex);
	explicit SmartLock(Mixin_ThreadAware& threadAware);
	~SmartLock();

private:
	SysMutex& m_mutex;

	/* Not supported. */
	SmartLock(const SmartLock&);
	SmartLock operator=(const SmartLock&);
};

/*****************************************************************************/

/**
 * @brief Mixin to render a class thread-aware, i.e. by having a mutex and an
 * accessor method.
 *
 * A mixin is a class intended for composition by multiple inheritence.  Any
 * class may inherit from a mixin to gain functionality without fundamentally
 * altering its other behavior.
 */
class Mixin_ThreadAware
{
public:
	Mixin_ThreadAware();
	virtual ~Mixin_ThreadAware();

	/* Must be a const function to allow const objects to use the mixin. */
	SysMutex& getMutex() const;

private:
	/* Must be mutable so getMutex() may be a const function. */
	mutable SysMutex m_mixinThreadAware_Mutex;

	/* Not supported. */
	Mixin_ThreadAware(const Mixin_ThreadAware&);
	Mixin_ThreadAware& operator=(const Mixin_ThreadAware&);
};

#endif // SYS__CONCURRENCY_H
