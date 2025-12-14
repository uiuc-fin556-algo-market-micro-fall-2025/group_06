/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/SafeQueue.h
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

#ifndef _STRATEGY_STUDIO_UTILITIES_SAFE_QUEUE_H_
#define _STRATEGY_STUDIO_UTILITIES_SAFE_QUEUE_H_

#include "Mutex.h"
#include "Condition.h"
#include "StrategyStudioException.h"
#include "TimeType.h"

#include <boost/atomic.hpp>

#include <queue>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

template<typename T>
class SafeQueue {
public:
    SafeQueue();
    ~SafeQueue();

    /**
     * Test whether container is empty
     */
    bool empty() const { ScopedLock lock(mutex_); return queue_.empty(); }

    /**
     * Inserts item, notifies waiter if successful
     * @returns true if item pushed, false if not (for instance after shutdown)
     */
    template<typename... Args>
    bool Emplace(Args&&... args);

    /**
     * Inserts item, notifies waiter if successful
     * @returns true if item pushed, false if not (for instance after shutdown)
     */
    bool Push(const T& item);

    /**
     * Pop the next element if present, copying to item. If queue empty, returns immediately
     * @returns true if item popped and copied, false if queue was empty
     */
    bool Pop(T& item) { ScopedLock lock(mutex_); return PopImpl(item); }

    /**
     * Pop the next element, waiting unless shutting down
     * @returns true if item popped and copied, false if request timed with empty queue on shutdown
     */
    bool WaitPop(T& item);

    /**
     * Pop the next element, waiting unless shutting down
     * @param timeoutInMilliseconds Indicates how long we want to block for in milliseconds
     *        if set to a negative number, then will block until something
     *        is pushed onto the queue
     * @returns true if item popped and copied, false if request timed out or empty on shutdown
     */
    bool WaitPop(T& item, int timeoutInMilliseconds);

    /**
     * Pop the next element, waiting unless shutting down
     * @param absTime Indicates clock time threshold for timeout
     *        if set to a negative number, then will block until something
     *        is pushed onto the queue
     * @returns true if item popped and copied, false if request timed out or empty on shutdown
     */
    bool WaitPop(T& item, boost::posix_time::ptime absTime);

    /**
     * Informs the queue and any watiers that queue is going to shut down
     */
    void Shutdown();

    /**
     * Start up usage of safe queue if shutdown
     */
    void Start();

private:
    bool PopImpl(T& item);

private:
    std::queue<T> queue_;
    mutable Mutex mutex_;
    Condition condition_;
    bool shutdown_;
};

template<typename T>
SafeQueue<T>::SafeQueue():
    queue_(),
    mutex_(),
    condition_(),
    shutdown_(false)
{
}

template<typename T>
SafeQueue<T>::~SafeQueue()
{
    // trigger any threads waiting on pop to shutdown
    Shutdown();
}

template<typename T>
template<typename... Args>
bool SafeQueue<T>::Emplace(Args&&... args)
{
    ScopedLock lock(mutex_); 
    if (!shutdown_) {
        queue_.emplace(std::forward<Args>(args)...);
        condition_.Notify();
        return true;
    } else {
        return false;
    }
}

template<typename T>
bool SafeQueue<T>::Push(const T& item)
{
    ScopedLock lock(mutex_); 
    if (!shutdown_) {
        queue_.push(item);
        condition_.Notify();
        return true;
    } else {
        return false;
    }
}

template<typename T>
bool SafeQueue<T>::PopImpl(T& item)
{
    if (!queue_.empty()) {
        item = queue_.front();
        queue_.pop();
        return true;
    } else {
        return false;
    }
}

template<typename T>
bool SafeQueue<T>::WaitPop(T& item)
{
    ScopedLock lock(mutex_);
    while (!shutdown_ && queue_.empty()) {
        condition_.Wait(lock);
    }
    return PopImpl(item);
}

template<typename T>
bool SafeQueue<T>::WaitPop(T& item, int timeout)
{
    ScopedLock lock(mutex_);
    bool timedOut = false;
    int waitTime = timeout;
    boost::posix_time::ptime waitStartTime(Now());
    while (!shutdown_ && !timedOut && queue_.empty()) {
        // TimedWait false if we waited the full time, true if the condition variable was signaled (by Push or Shutdown being called, or spurious wakeup)
        timedOut = !condition_.TimedWait(lock, waitTime);
        if (!timedOut) { 
            // Code to deal with spurious wakeups
            boost::posix_time::ptime wakeupTime(Now());
            waitTime = (int)(waitTime - (wakeupTime - waitStartTime).total_milliseconds());
            waitStartTime = wakeupTime;
        }
    }
    return PopImpl(item);
}

template<typename T>
bool SafeQueue<T>::WaitPop(T& item, boost::posix_time::ptime absTime)
{
    ScopedLock lock(mutex_);
    bool timedOut = false;
    while (!shutdown_ && !timedOut && queue_.empty()) {
        // TimedWait false if we waited the full time, true if the condition variable was signaled (by Push or Shutdown being called, or spurious wakeup)
        timedOut = !condition_.TimedWait(lock, absTime);
    }
    return PopImpl(item);
}

template<typename T>
void SafeQueue<T>::Shutdown()
{
    ScopedLock lock(mutex_);
    if (shutdown_) return;
    shutdown_ = true;
    condition_.NotifyAll();
}

template<typename T>
void SafeQueue<T>::Start()
{
    ScopedLock lock(mutex_);
    if (!shutdown_) return;
    shutdown_ = false;
}

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif
