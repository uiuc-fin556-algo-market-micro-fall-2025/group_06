/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/OrderUpdateEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_ORDER_UPDATE_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_ORDER_UPDATE_EVENT_MSG_H_

#include "TimePrioritizedEventMsg.h"
#include "FillInfo.h"
#include "ExecutionTypes.h"
#include "Event.h"
#include "Order.h"
#include <Utilities/Cast.h>
#include <string>

namespace RCM {
namespace StrategyStudio {

/**
 * OrderUpdateEventMsg is an event message for delivering updates about a Strategy's orders, such as fills, rejects, etc
 */
class OrderUpdateEventMsg : public TimePrioritizedEventMsg {
public:
    OrderUpdateEventMsg(OrderID orderID,
                        OrderUpdateType updateType,
                        Event* event,
                        TimeType updateTime);

    std::string name() const;

    /**
     * Returns a reference to the original order
     */
    const Order& order() const { return *order_; }

    /**
     * Returns Strategy Studio's internal OrderID
     */
    OrderID order_id() const { return order_id_; }

    /**
     * Returns the update type for this event
     */
    OrderUpdateType update_type() const { return update_type_; }

    /**
     * Returns the time the order update occurred, according to the Execution Provider's timestamp
     */
    TimeType update_time() const { return order_update_time_; }

    /**
     * Returns the reason text associated with this order update
     */
    const std::string& reason() const { static const std::string blankReason; return order_ ? order_->reason() : blankReason; }

    /**
     * Check if a fill occurred
     *
     * @return true if a fill occurred (full or partial), false otherwise
     */
    virtual bool fill_occurred() const;

    /**
     * Returns a pointer to the fill information, or nullptr if no fill occurred
     */
    virtual const FillInfo* fill() const;

    /**
     * Returns whether this update removes the order from the Working Orders collection
     */
    bool completes_order() const { return completes_order_; }

    // From ITimePrioritized - not really used
    bool Trigger(TimeType) { return true; }

protected:
    TimeType order_update_time_;
    const Order* order_;
    MarketModels::OrderID order_id_;
    OrderUpdateType update_type_;
    bool completes_order_;
};

inline std::string OrderUpdateEventMsg::name() const
{
    return "OrderUpdateEvent_ID_" + Utilities::Cast<std::string>()(order_id_) + "_" + OrderUpdateTypeToString(update_type_);
}

} // namespace StrategyStudio
} // namespace RCM

#endif