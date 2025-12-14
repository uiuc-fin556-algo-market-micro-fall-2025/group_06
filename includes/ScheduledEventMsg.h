/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/ScheduledEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_TIMED_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_TIMED_EVENT_MSG_H_

#include "TimePrioritizedEventMsg.h"
#include "Event.h"
#include "DataTypes.h"
#include <ostream>

namespace RCM {
namespace StrategyStudio {

/**
 * ScheduledEventMsg delivers timed events to strategies
 */
class ScheduledEventMsg : public TimePrioritizedEventMsg {
public:
    static const int TIMED_EVENT_PRIORITY = 7;

public:
    ScheduledEventMsg(const std::string& name,
                      ScheduledEventType type,
                      TimeType fireTime,
                      TimeDuration interval,
                      Event* event);

    ScheduledEventMsg(const std::string& name,
                      ScheduledEventType type,
                      TimeType fireTime,
                      TimeDuration interval,
                      bool fireIfPast,
                      Event* event);

    ScheduledEventMsg(const std::string& name,
                      ScheduledEventType type,
                      TimeType fireTime,
                      TimeType endFireTime,
                      TimeDuration interval,
                      bool fireIfPast,
                      Event* event);

    /**
     * Returns the name of the scheduled event passed in when registering for the event
     */
    const std::string& scheduled_event_name() const { return scheduled_event_name_; }

    /**
     * Identifies whether this is a single or recurring scheduled event
     */
    ScheduledEventType type() const { return scheduled_event_type_; }

    /**
     * Time that this occurence of the event was scheduled to fire
     */
    TimeType fire_time() const { return NextFireTime(); }

    /**
     * For recurring scheduled events, specifies the time when the events will stop. NULL_TIME_TYPE implies end of day.
     */
    TimeType end_fire_time() const { return end_fire_time_; }

    /**
     * Returns the time between firings for recurring events
     */
    TimeDuration interval() const { return interval_; }

    bool fire_if_past() const { return fire_if_past_; }
    void set_fire_if_past(bool flag) { fire_if_past_ = flag; }

public: /* Override from ITimePrioritized */
    /**
     * Triggers any logic related to firing
     * @returns true if further processing should occur, false otherwise
     */
    virtual bool Trigger(TimeType currTime);

    /**
     * Triggers any logic after the firing
     */
    void PostTrigger(TimeType currTime);

private:
    std::string scheduled_event_name_;
    TimeType end_fire_time_;
    TimeDuration interval_;
    ScheduledEventType scheduled_event_type_;
    bool fire_if_past_;
};

inline std::ostream& operator<<(std::ostream& out, const ScheduledEventMsg& msg)
{
    out << "Scheduled event " << msg.name() << " " << EventTypeUtils::ToString(msg.type()) << " "
        << Utilities::TimeTypeToString(msg.NextFireTime()) << " " << Utilities::ToString(msg.interval());

    return out;
}

} // namespace StrategyStudio
} // namespace RCM

#endif