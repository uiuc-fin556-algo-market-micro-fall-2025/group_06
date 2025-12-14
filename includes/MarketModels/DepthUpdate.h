/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/DepthUpdate.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_DEPTH_UPDATE_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_DEPTH_UPDATE_H_

#include <Utilities/TimeType.h>
#include "DepthDataTypes.h"
#include "MarketCenterTypes.h"
#include "MDBaseObject.h"

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * This class describes an update to an order book
 */
class DepthUpdate : public MDBaseObject {
public:
    DepthUpdate();

    // depth-by-price constructor, implicitly sets update_type to DEPTH_UPDATE_TYPE_BY_PRICE
    DepthUpdate(DepthSide side, 
                double price, 
                int size, 
                int numOrders, 
                MarketCenterID mktCenter, 
                TimeType sourceTime = NULL_TIME_TYPE, 
                TimeType feedTime = NULL_TIME_TYPE, 
                TimeType adapterTime = NULL_TIME_TYPE, 
                DepthReasonForChange reason = DEPTH_REASON_FOR_CHANGE_UNKNOWN, 
                bool isImplied = false,
                bool isPartial = false);

    // depth-by-order constructor, implicitly sets update_type to DEPTH_UPDATE_TYPE_BY_ORDER
    DepthUpdate(DepthOrderID orderID,
                DepthSide side,
                double price,
                int size,
                MarketCenterID mktCenter,
                TimeType sourceTime = NULL_TIME_TYPE,
                TimeType feedTime = NULL_TIME_TYPE,
                TimeType adapterTime = NULL_TIME_TYPE,
                DepthReasonForChange reason = DEPTH_REASON_FOR_CHANGE_UNKNOWN,
                const std::string& mpid = "",
                StrategyStudioUInt64 priorityIndicator = 0,
                bool isPartial = false);

    // depth-by-order constructor for DEPTH_REASON_FOR_CHANGE_CANCEL_REPLACE special case, implicitly sets update_type to DEPTH_UPDATE_TYPE_BY_ORDER
    DepthUpdate(DepthOrderID orderID, 
                DepthSide side, 
                double price, 
                int size, 
                MarketCenterID mktCenter, 
                TimeType sourceTime, 
                TimeType feedTime, 
                TimeType adapterTime, 
                DepthOrderID oldOrderId, 
                const std::string& mpid = "",
                StrategyStudioUInt64 priorityIndicator = 0,
                bool isPartial = false);

    // constructor for administrative updates, ie DepthUpdateType DEPTH_UPDATE_TYPE_RESET_ORDER_BOOK and higher
    DepthUpdate(DepthUpdateType type,
                MarketCenterID mktCenter, 
                TimeType sourceTime = NULL_TIME_TYPE, 
                TimeType feedTime = NULL_TIME_TYPE, 
                TimeType adapterTime = NULL_TIME_TYPE);

    /**
     * Returns whether the depth update is by price level or by individual order, or whether this update resets the order book
     */
    DepthUpdateType update_type() const { return update_type_; }
    void set_update_type(DepthUpdateType updateType) { update_type_ = updateType; }

    /**
     * Returns the market center whose orer book is updating
     */
    MarketCenterID market_center() const { return market_center_id_; }
    void set_market_center(MarketCenterID mktCenter) { market_center_id_ = mktCenter; }

    /**
     * Returns the current price of the order book entry
     */
    double price() const { return price_; }
    void set_price(double price) { price_ = price; }

    /**
     * Retuns the previous price of the order book entry if this update modifies an existing entry
     */
    double old_price() const { return old_price_; }
    void set_old_price(double price) { old_price_ = price; }

    /**
     * Returns the current size of the order book entry
     */
    int size() const { return size_; }
    void set_size(int size) { size_ = size; }

    /**
     * Retuns the previous size of the order book entry if this update modifies an existing entry
     */
    int old_size() const { return old_size_; }
    void set_old_size(int size) { old_size_ = size; }

    /**
     * Returns the side of the order book that is being updated
     */
    DepthSide side() const { return side_; }
    void set_side(DepthSide side) { side_ = side; }

    /**
     * Returns the order id of the order in the order book that has updated, zero if not applicable
     */
    DepthOrderID order_id() const { return order_id_; }
    void set_order_id(DepthOrderID orderID) { order_id_ = orderID; }

    /**
     * Returns the previous order id of the order, applicable only if reason() == DEPTH_REASON_FOR_CHANGE_CANCEL_REPLACE, zero otherwise
     */
    DepthOrderID old_order_id() const { return old_order_id_; }
    void set_old_order_id(DepthOrderID orderID) { old_order_id_ = orderID; }

    /**
     * Returns a queue placement priority indicator if supplied by the data source. Within price level, Lower values indicate higher priority in the book.
     *
     * std::numeric_limits<StrategyStudioUInt64>::max() can indicate a priority reset on order modification without regard to relative values.
     */
    StrategyStudioUInt64 priority_indicator() const { return priority_indicator_; }
    void set_priority_indicator(StrategyStudioUInt64 priorityIndicator) { priority_indicator_ = priorityIndicator; }

    /**
     * Original value of priority indicator if this update modifies an existing order
     */
    StrategyStudioUInt64 old_priority_indicator() const { return old_priority_indicator_; }
    void set_old_priority_indicator(StrategyStudioUInt64 priorityIndicator) { old_priority_indicator_ = priorityIndicator; }

    /**
     * Returns the number of orders when update type is depth-by-price-level; returns 0 for depth-by-order
     */
    int num_orders() const { return num_orders_; }
    void set_num_orders(int numOrders) { num_orders_ = numOrders; }

    /**
     * Returns an enumerator describing the type of change that triggered the depth-of-book tick
     */
    DepthReasonForChange reason() const { return reason_; }
    void set_reason(DepthReasonForChange reason) { reason_ = reason; }

    /**
     * When a new order is being added, returns the MPID associated with the order, if applicable, an empty string otherwise
     */
    const std::string& market_participant_id() const { return mpid_; }
    void set_market_participant_id(const std::string& mpid) { mpid_ = mpid; }

    /**
     * Returns whether or not this is an implied-originated update
     */
    bool is_implied() const { return is_implied_; }
    void set_is_implied(bool isImplied) { is_implied_ = isImplied; }

    /**
     * Returns whether this update is part of a batch of updates and is not the last of the batch
     */
    bool is_partial() const { return is_partial_; }
    void set_is_partial(bool isPartial) { is_partial_ = isPartial; }

    void Reset();

private:
    std::string mpid_;
    double price_;
    double old_price_;
    DepthOrderID order_id_;
    DepthOrderID old_order_id_;
    StrategyStudioUInt64 priority_indicator_;
    StrategyStudioUInt64 old_priority_indicator_;
    int size_;
    int old_size_;
    int num_orders_;
    MarketCenterID market_center_id_;
    DepthUpdateType update_type_;
    DepthSide side_;
    DepthReasonForChange reason_;
    bool is_implied_;
    bool is_partial_;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif
