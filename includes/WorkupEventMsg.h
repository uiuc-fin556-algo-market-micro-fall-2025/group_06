/*================================================================================
*     Copyright (c) RCM-X, 2016.
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

#ifndef _STRATEGY_STUDIO_LIB_WORKUP_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_WORKUP_EVENT_MSG_H_

#include "MarketDataBaseEventMsg.h"
#include <MarketModels/Instrument.h>
#include <MarketModels/WorkupDataTypes.h>
#include <boost/cstdint.hpp>

namespace RCM {
namespace StrategyStudio {

/**
 * WorkupEventMsg delivers information regarding a particular instrument's
 * workup state and transitions
 */
class WorkupEventMsg : public MarketDataBaseEventMsg {
public:
    WorkupEventMsg(Event* event,
                   MarketModels::Instrument* instrument,
                   const MarketModels::Workup& workup,
                   Utilities::TimeType eventTime) : 
        MarketDataBaseEventMsg(EVENT_MSG_TYPE_WORKUP_DATA, event, eventTime),
        instrument_(instrument),
        workup_(workup)
    {
    }

public:
    std::string name() const { return "WORKUP_EVENT_" + instrument_->symbol(); }

    /**
     * Returns the instrument for the current workup
     */
    const MarketModels::Instrument& instrument() const { return *instrument_; }

    /**
     * Returns the data for the current workup
     */
    const MarketModels::Workup& workup() const { return workup_; }

public: /* MarketDataBaseEventMsg overrides */
    /**
     * Gets the market data source's timestamp for the market data event (eg the exchange feed's timestamp)
     */
    virtual Utilities::TimeType source_time() const { return workup_.source_time(); }

    /**
     * Gets the time the feedhandler processed the market data event, if applicable and available
     */
    virtual Utilities::TimeType feed_handler_time() const { return workup_.feed_handler_time(); }

    /**
     * Gets the time when the Strategy Server's data adapter received the market data event
     */
    virtual Utilities::TimeType adapter_time() const { return workup_.adapter_time(); }

private:
    MarketModels::Instrument* instrument_;
    MarketModels::Workup workup_;
};

} // namespace StrategyStudio
} // namespace RCM

#endif