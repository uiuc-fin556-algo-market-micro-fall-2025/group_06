/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/EventMsg.h
*     Last Update: 2010/09/30 13:55:14
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2009 - 2021.
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

#ifndef _STRATEGY_STUDIO_LIB_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_EVENT_MSG_H_

#include "Event.h"
#include <Utilities/TimeType.h>

namespace RCM {
namespace StrategyStudio {

enum EventMsgType {
    EVENT_MSG_TYPE_UNKNOWN = 0,
    EVENT_MSG_TYPE_DEPTH_DATA,
    EVENT_MSG_TYPE_QUOTE_DATA,
    EVENT_MSG_TYPE_TRADE_DATA,
    EVENT_MSG_TYPE_OPTIONS_ANALYTICS,
    EVENT_MSG_TYPE_WORKUP_DATA,
    EVENT_MSG_TYPE_IMBALANCE_DATA,
    EVENT_MSG_TYPE_INSTRUMENT_ALERT,
    EVENT_MSG_TYPE_PRICE_BAND,
    EVENT_MSG_TYPE_INDEX,
    EVENT_MSG_TYPE_ORDER_UPDATE,
    EVENT_MSG_TYPE_BAR_DATA,
    EVENT_MSG_TYPE_SERVER_HEARTBEAT,
    EVENT_MSG_TYPE_NEWS,
    EVENT_MSG_TYPE_SENTIMENT,
    EVENT_MSG_TYPE_SENTIMENT_ACTIVITY,
    EVENT_MSG_TYPE_SCHEDULED,
    EVENT_MSG_TYPE_STRATEGY_NOTIFY,
    EVENT_MSG_TYPE_CONTROL,
    EVENT_MSG_TYPE_TRADE_STATS_INIT,
    EVENT_MSG_TYPE_APP_STATE,
    EVENT_MSG_TYPE_STRATEGY_STATE_CONTROL,
    EVENT_MSG_TYPE_STRATEGY_CREATE,
    EVENT_MSG_TYPE_MARKET_STATE,
    EVENT_MSG_TYPE_CORP_ACTION,
    EVENT_MSG_TYPE_MARKET_CENTER_FEES,
    EVENT_MSG_TYPE_FILL_SIMULATOR_PARAMS,
    EVENT_MSG_TYPE_REPLAY,
    EVENT_MSG_TYPE_DATA_SUBSCRIPTION,
    EVENT_MSG_TYPE_STRATEGY_EXTERNAL_FILL_NOTIFY,
    EVENT_MSG_TYPE_EXTERNAL_ORDER,
    EVENT_MSG_TYPE_EXTERNAL_BASKET_ORDER,
    EVENT_MSG_TYPE_ACCOUNT_FEES_CHANGE,
    EVENT_MSG_TYPE_ACCOUNT_CHANGE,
    EVENT_MSG_TYPE_PREFERRED_FEED_TYPE_CHANGE,
    EVENT_MSG_TYPE_TOPIC_SUBSCRIPTION,
    EVENT_MSG_TYPE_STRATEGY_COMMAND,
    EVENT_MSG_TYPE_INSTRUMENT_REF_UPDATE
};

class EventMsgTypeUtils {
public:
    static const std::string& ToString(EventMsgType type);
};

/**
 * EventMsg is the base class for event messages which hold the information for a given
 * event.
 */
class EventMsg {
public:
    EventMsg(EventMsgType eventMsgType, Event* event, Utilities::TimeType eventTime = NULL_TIME_TYPE);

    virtual ~EventMsg() = default;
    EventMsg(const EventMsg&) = default;
    EventMsg& operator=(const EventMsg&) = default;
    EventMsg(EventMsg&&) = default;
    EventMsg& operator=(EventMsg&&) = default;

    Event* event() { return event_; }
    const Event* event() const { return event_; }

    void set_event(Event* event) { event_ = event; }

    EventMsgType event_message_type() const { return event_msg_type_; }

    Utilities::TimeType event_time() const { return event_time_; }
    void set_event_time(Utilities::TimeType timeType) { event_time_ = timeType; }

    virtual std::string name() const { return EventMsgTypeUtils::ToString(event_msg_type_); }

    /**
     * Check if this is a market data type of event
     */
    bool IsMarketDataTypeEvent() const;

    /**
     * Publish this message to all registered listeners
     */
    void Process();

private: /* Hook functions */
    /**
     * This is an optional hook that derived messages can implement.
     * Allows child message to customize some behavior before message is sent to listeners
     * Optionally this function returns a bool signifying if this message should be propagated to listeners
     * This allows custom filtering to be done based on event logic
     * @returns true if message should be propagated to event listeners, false otherwise
     */
    virtual bool PreProcess();

    /**
     * This is an optional hook that derived messages can implement.
     * Allows child message to customize some behavior after message is sent to listeners
     */
    virtual void PostProcess();

private:
    Utilities::TimeType event_time_; // signifies time that the event message was created
    Event* event_;
    EventMsgType event_msg_type_;
};

inline bool EventMsg::IsMarketDataTypeEvent() const
{
    return ((event_msg_type_ >= EVENT_MSG_TYPE_DEPTH_DATA) &&
            (event_msg_type_ <= EVENT_MSG_TYPE_INDEX));
}

inline void EventMsg::Process()
{
    // allow customized child behavior before msg is processed
    if (PreProcess()) {
        event_->TriggerEvent(this);
        // allow customized child behavior after msg is processed
        PostProcess();
    }
}

} // namespace StrategyStudio
} // namespace RCM

#endif