/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/ConditionBoostImpl.h
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

#ifndef _STRATEGY_STUDIO_UTILITIES_CONDITION_BOOST_IMPL_H_
#define _STRATEGY_STUDIO_UTILITIES_CONDITION_BOOST_IMPL_H_

#include "Mutex.h"
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

class Condition {
public:
    Condition() { }
    ~Condition() { }

    void Wait(ScopedLock& lock);
    bool TimedWait(ScopedLock& lock, int ms);
    bool TimedWait(ScopedLock& lock, boost::system_time absTime);
    void Notify();
    void NotifyAll();

private:
    boost::condition_variable m_cond;
};

inline void Condition::Wait(ScopedLock& lock)
{
    m_cond.wait(lock.GetLock());
}

inline bool Condition::TimedWait(ScopedLock& lock, int ms)
{
    boost::posix_time::time_duration m_timeDuration = boost::posix_time::millisec(ms);
    return m_cond.timed_wait(lock.GetLock(), m_timeDuration);
}

inline bool Condition::TimedWait(ScopedLock& lock, boost::system_time absTime)
{
    return m_cond.timed_wait(lock.GetLock(), absTime);
}

inline void Condition::Notify()
{
    m_cond.notify_one();
}

inline void Condition::NotifyAll()
{
    m_cond.notify_all();
}

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif
