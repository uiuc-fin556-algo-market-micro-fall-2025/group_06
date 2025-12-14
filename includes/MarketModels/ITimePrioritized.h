/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/ITimePrioritized.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_ITIME_PRIORITIZED_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_ITIME_PRIORITIZED_H_

#include <Utilities/TimeType.h>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

using RCM::StrategyStudio::Utilities::TimeType;
using RCM::StrategyStudio::Utilities::TimeDuration;

struct TimePrioritizedGreaterPtrCmp;

/**
 * Abstract interface for implementing events that can be prioritzed by time
 */
class ITimePrioritized {
public:
    static const int DEFAULT_PRIORITY = 5;

public:
    ITimePrioritized(int priority = DEFAULT_PRIORITY);

    virtual ~ITimePrioritized() = default;
    ITimePrioritized(const ITimePrioritized&) = default;
    ITimePrioritized& operator=(const ITimePrioritized&) = default;
    ITimePrioritized(ITimePrioritized&&) = default;
    ITimePrioritized& operator=(ITimePrioritized&&) = default;

    TimeType NextFireTime() const { return next_fire_time_; }
    void SetNextFireTime(TimeType fireTime) { next_fire_time_ = fireTime; }

    TimeType LastFireTime() const { return last_fire_time_; }

    bool IsRecurring() const { return is_recurring_; }
    void SetIsRecurring(bool isRecurring) { is_recurring_ = isRecurring; }

    int priority() const { return priority_; }
    void set_priority(int flag) { priority_ = flag; }

    boost::uint64_t sequence() const { return sequence_; }
    void set_sequence(boost::uint64_t sequence) { sequence_ = sequence; }

    /**
     * Add time duration to next fire time
     */
    void IncrNextFireTime(TimeDuration td);

    /**
     * Reset fire times
     */
    void ResetFireTimes();

    /**
     * Triggers any logic related to firing
     * Note: This should appropriately set the next fire time
     * @returns true if further processing should occur, false otherwise
     */
    virtual bool Trigger(TimeType currTime) = 0;

    /**
     * Triggers any logic after the firing
     * Note: Empty by default
     */
    virtual void PostTrigger(TimeType) {}

    bool operator<(const ITimePrioritized& rhs) const;

    friend struct TimePrioritizedGreaterPtrCmp;

protected:
    TimeType next_fire_time_;
    TimeType last_fire_time_;
    boost::uint64_t sequence_;
    int priority_;
    bool is_recurring_;
};

inline void ITimePrioritized::IncrNextFireTime(TimeDuration td)
{
    last_fire_time_ = next_fire_time_;
    next_fire_time_ += td;
}

inline bool ITimePrioritized::operator<(const ITimePrioritized& rhs) const
{
    return (next_fire_time_ < rhs.next_fire_time_);
}

struct TimePrioritizedGreaterPtrCmp {
public:
    bool operator()(const ITimePrioritized* lhs, const ITimePrioritized* rhs) const
    {
        if (lhs->next_fire_time_ == rhs->next_fire_time_) {
            if (lhs->priority_ == rhs->priority_) {
                return lhs->sequence_ > rhs->sequence_;
            } else {
                return lhs->priority_ > rhs->priority_;
            }
        } else {
            return lhs->next_fire_time_ > rhs->next_fire_time_;
        }
    }
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif
