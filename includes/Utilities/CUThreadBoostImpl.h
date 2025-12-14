/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/CUThreadBoostImpl.h
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

#ifndef ND_LIB_USE_PTHREAD

#pragma once

#ifndef _STRATEGY_STUDIO_UTILITIES_THREAD_BOOST_IMPL_H_
#define _STRATEGY_STUDIO_UTILITIES_THREAD_BOOST_IMPL_H_

#include "Mutex.h"
#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <string>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

/**
 * This is a wrapper class that a derived class can inherit from
 * in order to obtain threading functionality
 * Note: allows for at most one concurrent thread at any one time
 */
class CUThread {
public:
    CUThread(const std::string& threadName);
    virtual ~CUThread();

    const std::string& GetName() const { return m_threadName; }

    /**
     * Returns whether the thread has been started
     */
    bool IsStarted() const { ScopedLock lock(m_mutex); return m_thread != nullptr; }

    /**
     * Allows for the background thread to be disabled
     * Note: will not stop an already running thread
     */
    bool IsDisabled() const { ScopedLock lock(m_mutex); return m_isDisabled; }
    void SetDisabled(bool flag) { ScopedLock lock(m_mutex); m_isDisabled = flag; }

public:
    /**
     * Starts an internal thread in order to execute the thread loop body
     * @throws if could not be started
     */
    void Start();

    /**
     * Stops the internal thread
     * @throws if could not be stopped
     */
    void Stop();

protected:

    bool Shutdown() const { return m_shutdown; }

private:

    /**
    * This is the main thread body that is called by the created
    * internal thread.
    * Note: Not meant to be called in normal circumstances except by internals
    */
    void ThreadLoop();

    /**
     * Main logic to be executed by the threading loop
     * @returns true if meant to continue execution, false if last call
     */
    virtual bool _ThreadBody() = 0;

    /**
     * Additional hook logic executed at end of Start method
     * Meant to be overridden by derived classes
     */
    virtual void _Start() {}

    /**
     * Additional hook logic executed at beginning of Stop method
     * Meant to be overridden by derived classes
     */
    virtual void _Stop() {}

    void SetShutdown(bool flag) { m_shutdown = flag; }

private:
    // disable copying of CUThreads
    CUThread(const CUThread&);
    CUThread& operator=(const CUThread&);

private:
    const std::string m_threadName;
    mutable Mutex m_mutex;
    boost::thread* m_thread;
    boost::atomic<bool> m_shutdown;
    bool m_isDisabled; // if toggled then thread will not start
};

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif

#endif