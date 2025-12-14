/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/MutexBoostImpl.h
*     Last Update: 2010/09/30 13:55:14
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2009 - 2010.
*     All rights reserved.
*
*     This software is part of Licensed material, which is the property of RCM-X ("Company"),
*     and constitutes Confidential Information of the Company.
*     Unauthorized use, modification, duplication or distribution is strictly prohibited by Federal law.
*     No title to or ownership of this software is hereby transferred.
*
*     The software is provided "as is", and in no event shall the Company or any of its affiliates or successors be liable for any
*     damages, including any lost profits or other incidental or consequential damages relating to the use of this software.
*     The Company makes no representations or warranties, express or implied, with regards to this software.
==================================================================================*/

#pragma once

#ifndef _STRATEGY_STUDIO_UTILITIES_MUTEX_BOOST_IMPL_H_
#define _STRATEGY_STUDIO_UTILITIES_MUTEX_BOOST_IMPL_H_

// disable assignment operator not generated warning
#pragma warning( disable : 4512 )

#include <boost/thread.hpp>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

/**
 * This class is a wrapper around the pthread mutex, essentially providing
 * RAII paradigm for initializing the mutex
 */
class Mutex {
public:
    Mutex() { }
    ~Mutex() { }

    void Lock();
    void Unlock();

    boost::mutex& GetMutex() { return m_mutex; }
    const boost::mutex& GetMutex() const { return m_mutex; }

private:
    mutable boost::mutex m_mutex;
};

inline void Mutex::Lock()
{
    m_mutex.lock();
}

inline void Mutex::Unlock()
{
    m_mutex.unlock();
}

typedef boost::mutex::scoped_lock scoped_lock;

/*----------------------------------------------------------------------------*/

/**
 * This class is provides the scoped locking paradigm for the mutex
 */
class ScopedLock {
public:
    ScopedLock(Mutex& mutex): m_lock(mutex.GetMutex()) {}
    ~ScopedLock() {}

    scoped_lock& GetLock() { return m_lock; }
    const scoped_lock& GetLock() const { return m_lock; }

private:
    mutable scoped_lock m_lock;
};

/*----------------------------------------------------------------------------*/

template<bool>
class ConditionalScopedLock
{
};

/**
 * Provides conditional scoped locking functionality, only if needed
 */
template<>
class ConditionalScopedLock<true> {
public:

    explicit ConditionalScopedLock(Mutex& mutex): m_mutex(mutex)
    {
        m_mutex.Lock();
    }

    ~ConditionalScopedLock()
    {
        m_mutex.Unlock();
    }

private:
    Mutex& m_mutex;
};

/**
 * Provides conditional scoped locking functionality, only if needed
 */
template<>
class ConditionalScopedLock<false> {
public:

    explicit ConditionalScopedLock(Mutex&)
    {
    }

    ~ConditionalScopedLock()
    {
    }
};

/*----------------------------------------------------------------------------*/

class RecursiveMutex {
public:
    RecursiveMutex() { }
    ~RecursiveMutex() { }

    void Lock() { m_mutex.lock(); }
    void Unlock() { m_mutex.unlock(); }

    boost::recursive_mutex& GetMutex() { return m_mutex; }
    const boost::recursive_mutex& GetMutex() const { return m_mutex; }

private:
    mutable boost::recursive_mutex m_mutex;
};

typedef boost::recursive_mutex::scoped_lock recursive_scoped_lock;

/*----------------------------------------------------------------------------*/

/**
 * This class is provides the scoped locking paradigm for the recursive mutex
 */
class RecursiveScopedLock {
public:

    RecursiveScopedLock(RecursiveMutex& mutex): m_lock(mutex.GetMutex()) {}

    ~RecursiveScopedLock() {}

    recursive_scoped_lock& GetLock() { return m_lock; }

    const recursive_scoped_lock& GetLock() const { return m_lock; }

private:
    mutable recursive_scoped_lock m_lock;
};

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif
