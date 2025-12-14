/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/PriceBandEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_PRICE_BAND_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_PRICE_BAND_EVENT_MSG_H_

#include "DataTypes.h"
#include "DataTypesUtils.h"
#include "Event.h"
#include "MarketDataBaseEventMsg.h"

namespace RCM {
namespace StrategyStudio {

/**
 * PriceBandEventMsg contains updates on the Limit Up/Limit Down price limits disseminated by
 * the US Consolidated equity feeds (CTA/UTP), which were introduced in Spring 2013 as a replacement for the
 * Single Stock Circuit Breakers
 */
class PriceBandEventMsg : public MarketDataBaseEventMsg {
public:
    PriceBandEventMsg(Event* event,
                      Instrument* instrument,
                      double limit_down_price,
                      double limit_up_price,
                      TimeType effective_time,
                      TimeType source_time,
                      TimeType feed_handler_time,
                      TimeType adapter_time,
                      const std::string& reason = "");

    std::string name() const;

    /**
     * Returns a const reference to the instrument which traded
     */
    const Instrument& instrument() const { return *instrument_; }

    /**
     * Returns the lower price band for the instrument
     */
    double limit_down_price() const { return limit_down_price_; }

    /**
     * Returns the upper price band for the instrument
     */
    double limit_up_price() const { return limit_up_price_; }

    /**
     * Timestamp the price band became effective, NULL_TIME_TYPE if not available
     */
    TimeType effective_time() const { return effective_time_; }

    /**
     * String contains additional information about the source and/or ramifications
     * of the price band update, when available
     */
    const std::string& reason() const { return reason_; }

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

    double limit_down_price_;
    double limit_up_price_;

    TimeType source_time_;
    TimeType feed_handler_time_;
    TimeType adapter_time_;
    TimeType effective_time_;
};

inline std::string PriceBandEventMsg::name() const
{
    return "PRICE_BAND_EVENT_" + instrument_->symbol();
}

} // namespace StrategyStudio
} // namespace RCM

#endif