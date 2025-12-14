/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IndexEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_INDEX_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_INDEX_EVENT_MSG_H_

#include "DataTypes.h"
#include "DataTypesUtils.h"
#include "Event.h"
#include "MarketDataBaseEventMsg.h"

namespace RCM {
namespace StrategyStudio {

/**
 * IndexEventMsg is an event message containing estimated fair
 * value pricing for an Index or ETP IIPV/IIV
 */
class IndexEventMsg : public MarketDataBaseEventMsg {
public:
    IndexEventMsg(Event* event,
                  Instrument* instrument,
                  FeedType feed_type,
                  double value,
                  TimeType source_time,
                  TimeType feed_handler_time,
                  TimeType adapter_time);

    std::string name() const;

    /**
     * Returns a const reference to the relevant instrument
     */
    const Instrument& instrument() const { return *instrument_; }

    /**
     * Identifies which type of data feed is delivering the message
     */
    FeedType feed_type() const { return feed_type_; }

    /**
    * Gets the estimated or indicative value
    */
    double value() const { return value_; }

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
    Instrument* instrument_;
    double value_;
    TimeType source_time_;
    TimeType feed_handler_time_;
    TimeType adapter_time_;
    FeedType feed_type_;
};

inline std::string IndexEventMsg::name() const
{
    return "INDEX_EVENT_" + instrument_->symbol();
}

} // namespace StrategyStudio
} // namespace RCM

#endif