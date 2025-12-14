/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/OrderParams.h
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

#ifndef STRATEGY_STUDIO_LIB_ORDER_PARAMS_H
#define STRATEGY_STUDIO_LIB_ORDER_PARAMS_H

#include "DataTypes.h"
#include "ExecutionTypes.h"

#include <boost/any.hpp>

namespace RCM {
namespace StrategyStudio {

namespace Utilities {
class SerializedStream;
}

class IInstrumentFactory;

/**
 * This struct wraps up data fields needed by the execution handler to send orders
 *
 * Note: Values set in this object will override any defaults that may be set via an Execution Handler's
 * route settings file.
 */
struct OrderParams {
public:
    OrderParams();

    /**
     * Explicitly reserves the number of params
     */
    explicit OrderParams(int numParams);

    OrderParams(const Instrument& instrument,
                OrderID orderID);

    /**
     * OrderParams constructor
     *
     * Note order quantities should be positive. Strategy Studio will validate this when you send an order.
     */
    OrderParams(const Instrument& instrument,
                int quantity,
                Price price,
                MarketCenterID marketCenter,
                OrderSide orderSide,
                OrderTIF orderTIF,
                OrderType orderType);

    /**
     * OrderParams constructor
     *
     * Note order quantities should be positive. Strategy Studio will validate this when you send an order.
     */
    OrderParams(const Instrument& instrument,
                int quantity,
                Price price,
                MarketCenterID marketCenter,
                OrderSide orderSide,
                OrderTIF orderTIF,
                OrderType orderType,
                bool leggingIn);

    /**
     * OrderParams constructor
     *
     * Note order quantities should be positive. Strategy Studio will validate this when you send an order.
     */
    OrderParams(const Instrument& instrument,
                int quantity,
                int displayQuantity,
                Price price,
                MarketCenterID marketCenter,
                OrderSide orderSide,
                OrderTIF orderTIF,
                OrderType orderType,
                bool leggingIn);

public:
    size_t num_params() const { return custom_params.size(); }

    bool operator==(const OrderParams& params) const;

    /**
     * Sort on price
     */
    bool operator<(const OrderParams& params) const;

    /**
     * Load from binary stream and return the symbol; instrument will be nullptr
     * Enables loading without knowing the specific InstrumentFactory
     * @return string of the symbol loaded
     */
    std::string LoadFromBinaryStream(Utilities::SerializedStream& in);

    /**
     * Load from binary stream
     */
    void LoadFromBinaryStream(Utilities::SerializedStream& in, IInstrumentFactory& factory);

    /**
     * Save to stream
     */
    void SaveToBinaryStream(Utilities::SerializedStream& out) const;

public:
    /**
     * Instrument to send the order for
     */
    const Instrument* instrument; // closure for use by callback

    /**
     * Quantity for the order. This number should be positive; StrategyStudio will not send an order if it is negative or zero
     */
    int quantity;

    /**
     * Display quantity for the order. This should be zero or positive, otherwise Strategy Studio will reject the order
     */
    int display_quantity;

    /**
     * Minimum quantity for the order. This should be zero or positive.
     *
     * Destinations that support this field will not fill/cross an order unless/until at least the minimum_quantity is available.
     */
    int minimum_quantity;

    /**
     * Order price
     */
    Price price;

    /**
     * Stop price for the order if the order is a stop or stop limit order. Note not all execution providers will
     * accept stop order types
     */
    Price stop_price;

    /**
     * Market center to route the order to
     */
    MarketCenterID market_center;

    /**
     * Order side
     */
    OrderSide order_side;

    /**
     * Order time in force
     */
    OrderTIF order_tif;

    /**
     * Order type
     */
    OrderType order_type;

    /**
     * Specifies whether this order is an intermarket sweep order. Generally speaking, ISO orders must also
     * be limit orders with an IOC time in force. To use an ISO order you most be properly configured with your
     * broker and must comply with Reg NMS.
     */
    bool is_iso;

    /**
     * Specifies the ISO Group to which this ISO order belongs.
     */
    StrategyStudioUInt32 iso_group;

    /**
     * Specifies the pegging strategy to use with the order.
     *
     * Note not all exchanges and brokers support pegged orders, and specific routes generally accept a subset of the peg types.
     */
    PegType peg_type;

    /**
     * Specifies a price offset from the benchmark price associated with the peg_type
     *
     * Note not all exchanges and brokers support pegged orders, and those that do may not
     * accept peg offsets for all peg_types.
     */
    PriceOffset peg_offset;

    /**
     * Specifies whether to send a hidden order. Not valid for all execution providers and routes.
     */
    bool is_hidden;

    /**
     * Specifies whether execution venue should avoid letting your order take liquidity, by rejecting
     * or repricing orders that would cross the venue's current market
     *
     * Not valid for all execution providers and routes.
     */
    bool post_only;

    /**
     * True to allow order to be routed to other exchanges
     */
    AllowRouting allow_routing;

    /**
     * Action to take for non-routable order if crosses/locks market.
     */
    LockedCrossedAction locked_crossed_action;

    /**
     * Indicates an order is during an auction, with the intent of closing the imbalance.
     */
    bool imbalance_only;

    /**
     * Specifies how to handle a detected self-trade.
     */
    WashTradePrevention wash_trade_prevention;

    /**
     * Exchange-specific routing instructions
     */
    std::string routing_instructions;

    /**
     * Allows for user-defined data structures
     *
     * Serialization/deserialization of closure only occurs if it is boost::any_cast-able to std::string
     */
    boost::any closure;

    /**
     * Specifies whether you are legging in to the prior trade ID. If so, Strategy Studio will attribute the fills
     * for this order to the most recent trade ID created for an order on of the same side as this order.
     */
    bool legging_in;

    /**
     * This order's ParentOrderID, or 0 if not applicable
     */
    OrderID parent_order_id;

    /**
     * Set by execution handler in new order, or used in cancel order command
     */
    OrderID order_id;
    OrderTag order_tag; // set by execution handler

    /**
     * Note: A value set in custom_params,
     * if of the form std::pair<int, std::string> where the int is a FIX tag,
     * will take precedence over a value elsewhere in OrderParams corresponding to the same FIX tag.
     *
     * Serialization/deserialization of a custom_param only occurs if the custom_param is boost::any_cast-able to std::pair<int, std::string>
     */
    CustomParams custom_params;
};

/**
 * Check if order side is a buy
 */
inline bool IsBuySide(OrderSide side)
{
    return ((side == ORDER_SIDE_BUY) ||
            (side == ORDER_SIDE_BUY_TO_OPEN) ||
            (side == ORDER_SIDE_BUY_TO_CLOSE) );
}

/**
 * Check if order side is a sell
 */
inline bool IsSellSide(OrderSide side)
{
    return ((side == ORDER_SIDE_SELL) ||
            (side == ORDER_SIDE_SELL_SHORT) ||
            (side == ORDER_SIDE_SELL_SHORT_EXEMPT) ||
            (side == ORDER_SIDE_SELL_TO_OPEN) ||
            (side == ORDER_SIDE_SELL_TO_CLOSE) );
}

/**
 * Check if this is a buy order
 */
inline bool IsBuyOrder(const OrderParams& params)
{
    return IsBuySide(params.order_side);
}

/**
 * Check if this is a sell order
 */
inline bool IsSellOrder(const OrderParams& params)
{
    return IsSellSide(params.order_side);
}

} // namespace StrategyStudio
} // namespace RCM

#endif
