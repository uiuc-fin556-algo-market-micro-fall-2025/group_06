/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/MutexPthreadImpl.h
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

#ifndef _STRATEGY_STUDIO_UTILITIES_MUTEX_PTHREAD_IMPL_H_
#define _STRATEGY_STUDIO_UTILITIES_MUTEX_PTHREAD_IMPL_H_

#ifdef ND_LIB_USE_PTHREAD

#include <pthread.h>
#include <string>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

/**
 * This class is a wrapper around the pthread mutex, essentially providing
 * RAII paradigm for initializing the mutex
 */
class Mutex {
public:
    Mutex();
    ~Mutex();

    void Lock();
    void Unlock();

    pthread_mutex_t& GetMutex();
    const pthread_mutex_t& GetMutex() const;

private:
    pthread_mutex_t mutex_;
};

inline pthread_mutex_t& Mutex::GetMutex()
{
    return mutex_;
}

inline const pthread_mutex_t& Mutex::GetMutex() const
{
    return mutex_;
}

inline void Mutex::Lock()
{
    pthread_mutex_lock (&mutex_);
}

inline void Mutex::Unlock()
{
    pthread_mutex_unlock (&mutex_);
}

/*----------------------------------------------------------------------------*/

/**
 * This class is provides the scoped locking paradigm for the mutex
 */
class ScopedLock {
public:

    ScopedLock(Mutex& mutex): mutex_(mutex)
    {
        mutex_.Lock();
    }

    ~ScopedLock()
    {
        mutex_.Unlock();
    }

    Mutex& GetLock() { return mutex_; }
    const Mutex& GetLock() const { return mutex_; }

private:

    Mutex& mutex_;
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

    ConditionalScopedLock(Mutex& mutex): mutex_(mutex)
    {
        mutex_.Lock();
    }

    ~ConditionalScopedLock()
    {
        mutex_.Unlock();
    }

    Mutex& GetLock() { return mutex_; }
    const Mutex& GetLock() const { return mutex_; }

private:
    Mutex& mutex_;
};

/**
 * Provides conditional scoped locking functionality, only if needed
 */
template<>
class ConditionalScopedLock<false> {
public:

    ConditionalScopedLock(Mutex&)
    {
    }

    ~ConditionalScopedLock()
    {
    }
};

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif

#endif
