/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/DataSubscriptionDataEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_DATA_SUBSCRIPTION_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_DATA_SUBSCRIPTION_EVENT_MSG_H_

#include "DataTypes.h"
#include "DataSourceTypes.h"
#include "EventMsg.h"
#include "Event.h"

namespace RCM {
namespace StrategyStudio {

/**
 * DataSubscriptionEventMsg informs strategies of changes in state for a particular
 * data subscription from a data adapter.
 */
class DataSubscriptionEventMsg : public EventMsg {
public:
    DataSubscriptionEventMsg(DataSubscriptionState state,
                             DataSubscriptionAlert alert,
                             FeedType feed_type,
                             MarketCenterID market_center,
                             Event* event,
                             TimeType event_time);

    DataSubscriptionEventMsg(DataSubscriptionState state,
                             DataSubscriptionAlert alert,
                             FeedType feed_type,
                             MarketCenterID market_center,
                             const std::string& description,
                             StrategyStudioInt32 severity,
                             Instrument* instrument,
                             Event* event,
                             TimeType event_time);

    std::string name() const;

    /**
     * Returns the current state of the data subscription
     */
    DataSubscriptionState state() const { return state_; }

    /**
     * This classifies the type of alert coming from the data source
     */
    DataSubscriptionAlert alert() const { return alert_; }

    /**
     * Identifies which type of data feed is delivering an alert
     */
    FeedType feed_type() const { return feed_type_; }

    /**
     * Returns which market center's feed is delivering an alert
     *
     * Returns MARKET_CENTER_ID_UNKNOWN for consoloidated data feeds
     */
    MarketCenterID market_center() const { return market_center_; }

    /**
     * Provides additional information about the alert when applicable
     */
    const std::string& description() const { return description_; }

    /**
     * Returns a number used to indicate the severity of the data alert.
     *
     * Typically this field is used for the number of messages lost when
     * the alert is DATA_SUBSCRIPTION_ALERT_DATA_GAP_DETECTED
     */
    const StrategyStudioInt32 severity() const { return severity_; }

    /**
     * Returns the instrument affected by the data alert if attributable, null otherwise
     */
    const Instrument* instrument() const { return instrument_; }

private:
    std::string description_;
    Instrument* instrument_;
    DataSubscriptionState state_;
    DataSubscriptionAlert alert_;
    FeedType feed_type_;
    MarketCenterID market_center_;
    StrategyStudioInt32 severity_;
};

inline std::string DataSubscriptionEventMsg::name() const
{
    return "DATA_SUBSCRIPTION_EVENT_" +
        DataSubscriptionStateToString(state_) + "_" +
        DataSubscriptionAlertToString(alert_) +
        ((instrument_ != nullptr) ? ("_" + instrument_->symbol()) : "");
}

} // namespace StrategyStudio
} // namespace RCM

#endif