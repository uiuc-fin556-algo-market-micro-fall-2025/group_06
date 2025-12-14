/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketDataBaseEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_MARKET_DATA_BASE_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_MARKET_DATA_BASE_EVENT_MSG_H_

#include "EventMsg.h"
#include <MarketModels/DepthUpdate.h>

namespace RCM {
namespace StrategyStudio {

/**
 * MarketDataBaseEventMsg adds additional interface functions for dealing specifically with market data
 * event types.
 */
class MarketDataBaseEventMsg : public EventMsg {
public:
    MarketDataBaseEventMsg(EventMsgType eventMsgType, Event* event);
    MarketDataBaseEventMsg(EventMsgType eventMsgType, Event* event, Utilities::TimeType eventTime);

    /**
     * Gets the market data source's timestamp for the market data event (eg the exchange feed's timestamp)
     */
    virtual Utilities::TimeType source_time() const = 0;

    /**
     * Gets the time the feedhandler processed the market data event, if applicable and available
     */
    virtual Utilities::TimeType feed_handler_time() const = 0;

    /**
     * Gets the time when the Strategy Server's data adapter received the market data event
     */
    virtual Utilities::TimeType adapter_time() const = 0;
};

} // namespace StrategyStudio
} // namespace RCM

#endif