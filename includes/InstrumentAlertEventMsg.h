/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/InstrumentAlertEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_INSTRUMENT_ALERT_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_INSTRUMENT_ALERT_EVENT_MSG_H_

#include "DataTypes.h"
#include "DataTypesUtils.h"
#include "Event.h"
#include "MarketDataBaseEventMsg.h"

namespace RCM {
namespace StrategyStudio {

/**
 * InstrumentAlertEventMsg informs strategies instrument specific alerts from a
 * market data feed. Alerts include updates to trading status (halts), short sale restrictions, etc.
 */
class InstrumentAlertEventMsg : public MarketDataBaseEventMsg {
public:
    InstrumentAlertEventMsg(Event* event,
                            Instrument* instrument,
                            MarketCenterID market_center,
                            FeedType feed_type,
                            InstrumentStatus instrument_status,
                            ShortSaleRestrictionIndicator short_sale_status,
                            TimeType source_time,
                            TimeType feed_handler_time,
                            TimeType adapter_time,
                            const std::string& reason = "");

    std::string name() const;

    const Instrument& instrument() const { return *instrument_; }

    InstrumentStatus instrument_status() const { return instrument_status_; }

    ShortSaleRestrictionIndicator short_sale_status() const { return short_sale_status_; }

    const std::string& reason() const { return reason_; }

    /**
     * Returns which market center the alert pertains to
     */
    MarketCenterID market_center() const { return market_center_; }

    /**
     * Identifies which type of data feed is delivering an alert
     */
    FeedType feed_type() const { return feed_type_; }

    /**
     * Gets the market data source's timestamp for the market data event (eg the exchange feed's timestamp)
     */
    TimeType source_time() const { return source_time_; }

    /**
     * Gets the time the feedhandler processed the market data event, if applicable and available
     */
    TimeType feed_handler_time() const { return feed_handler_time_; }

    /**
     * Gets the time when the Strategy Server's data adapter received the market data event
     */
    TimeType adapter_time() const { return adapter_time_; }

private:
    std::string reason_;

    Instrument* instrument_;

    TimeType source_time_;
    TimeType feed_handler_time_;
    TimeType adapter_time_;

    InstrumentStatus instrument_status_;
    ShortSaleRestrictionIndicator short_sale_status_;
    FeedType feed_type_;
    MarketCenterID market_center_;
};

inline std::string InstrumentAlertEventMsg::name() const
{
    return "INSTRUMENT_ALERT_EVENT_" + instrument_->symbol();
}

} // namespace StrategyStudio
} // namespace RCM

#endif