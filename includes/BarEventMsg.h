/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/BarEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_BAR_EVENTMSG_H_
#define _STRATEGY_STUDIO_LIB_BAR_EVENTMSG_H_

#include "TimePrioritizedEventMsg.h"
#include "Event.h"
#include "DataTypes.h"
#include "BarDataTypes.h"
#include "IEventCallback.h"

#include <iostream>

namespace RCM {
namespace StrategyStudio {

class QuoteEventMsg;
class TradeDataEventMsg;

/**
 * BarEventMsg is an event message that gets triggered when a Bar is complete
 */
class BarEventMsg : public TimePrioritizedEventMsg {
public:
    const static int TIMED_EVENT_PRIORITY = 5;

public:
    BarEventMsg(const BarDefinition& barDefinition,
                const Bar& bar,
                const Instrument* instrument,
                Event* event);

    std::string name() const;

    /**
     * Returns a const reference to the Instrument which the current Bar summarizes
     */
    const Instrument& instrument() const { return *instrument_; }

    /**
     * Returns a reference to the current Bar
     */
    Bar& bar() { return bar_data_; }

    /**
     * Returns a const reference to the current Bar
     */
    const Bar& bar() const { return bar_data_; }

    /**
     * Identifies the type of Bar that has completed
     */
    BarType type() const { return bar_definition_.type; }

    /**
     * Describes the size of the Bar that has completed
     */
    double interval() const { return bar_definition_.interval; }

    /**
     * The p_value of the bar definition, relavent only for realized variance bars
     */
    double p_value() const { return bar_definition_.p_value; }

    /**
     * The alignment time of the bar definition, relevant only for time bars
     */
    TimeType alignment_time() const { return bar_definition_.alignment_time; }

    /**
     * Returns the time the current Bar completed
     */
    TimeType bar_time() const { return (bar_definition_.type == BAR_TYPE_TIME) ? NextFireTime() : this->event_time(); }

    /**
     * Used only for time bars. Doesn't do anything for other bars.
     */
    bool Trigger(TimeType /*currTime*/) { return true; }

protected:
    BarDefinition bar_definition_;
    Bar bar_data_;
    const Instrument* instrument_;
};

inline std::string BarEventMsg::name() const
{
    return "Bar_Type_" + BarTypeToString(bar_definition_.type) + "_Instr_" + instrument_->symbol();
}

inline std::ostream& operator<<(std::ostream& out, const BarEventMsg& msg)
{
    out << Utilities::TimeTypeToString(msg.bar_time()) << " - "
        << msg.instrument().symbol()
        << msg.bar();
    return out;
}

} // namespace StrategyStudio
} // namespace RCM

#endif
