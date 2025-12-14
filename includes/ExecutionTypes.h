/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/ExecutionTypes.h
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

#ifndef _STRATEGY_STUDIO_LIB_EXECUTION_TYPES_H_
#define _STRATEGY_STUDIO_LIB_EXECUTION_TYPES_H_

#include "DataTypes.h"
#include <MarketModels/MarketCenterTypes.h>
#include <boost/any.hpp>
#include <string>
#include <vector>

namespace RCM {
namespace StrategyStudio {

using MarketModels::OrderID;
using MarketModels::MarketCenterID;

typedef std::string OrderTag;

/**
 * Enumerates the sides for an order
 */
enum OrderSide {
    ORDER_SIDE_UNKNOWN = 0,
    ORDER_SIDE_BUY = '1',
    ORDER_SIDE_SELL = '2',
    ORDER_SIDE_SELL_SHORT = '5',
    ORDER_SIDE_SELL_SHORT_EXEMPT = '6',
    ORDER_SIDE_BUY_TO_OPEN = 'a',
    ORDER_SIDE_BUY_TO_CLOSE = 'b',
    ORDER_SIDE_SELL_TO_OPEN = 'c',
    ORDER_SIDE_SELL_TO_CLOSE = 'd'
};

inline std::string OrderSideToString(OrderSide side)
{
    switch (side) {
        case ORDER_SIDE_BUY:
            return "BUY";
        case ORDER_SIDE_SELL:
            return "SELL";
        case ORDER_SIDE_SELL_SHORT:
            return "SELL_SHORT";
        case ORDER_SIDE_SELL_SHORT_EXEMPT:
            return "SELL_SHORT_EXEMPT";
        case ORDER_SIDE_BUY_TO_OPEN:
            return "BUY_TO_OPEN";
        case ORDER_SIDE_BUY_TO_CLOSE:
            return "BUY_TO_CLOSE";
        case ORDER_SIDE_SELL_TO_OPEN:
            return "SELL_TO_OPEN";
        case ORDER_SIDE_SELL_TO_CLOSE:
            return "SELL_TO_CLOSE";
        case ORDER_SIDE_UNKNOWN:
        default:
            return "UNKNOWN";
    }
}

inline OrderSide StringToOrderSide(const std::string& side)
{
    if (side.compare("BUY") == 0)
        return ORDER_SIDE_BUY;
    else if (side.compare("SELL") == 0)
        return ORDER_SIDE_SELL;
    else if (side.compare("SELL_SHORT") == 0)
        return ORDER_SIDE_SELL_SHORT;
    else if (side.compare("BUY_TO_OPEN") == 0)
        return ORDER_SIDE_BUY_TO_OPEN;
    else if (side.compare("BUY_TO_CLOSE") == 0)
        return ORDER_SIDE_BUY_TO_CLOSE;
    else if (side.compare("SELL_TO_OPEN") == 0)
        return ORDER_SIDE_SELL_TO_OPEN;
    else if (side.compare("SELL_TO_CLOSE") == 0)
        return ORDER_SIDE_SELL_TO_CLOSE;
    else
        return ORDER_SIDE_UNKNOWN;
}

/**
 * Enumerates the possible states for an order
 */
enum OrderState {
    ORDER_STATE_UNKNOWN = 0,
    ORDER_STATE_NEW_ORDER = 1, // order has been constructed but not yet successfully sent to execution provider
    ORDER_STATE_OPEN_PENDING = 2, // order has been sent to execution provider and awaiting acknowledgment
    ORDER_STATE_OPEN = 3, // order has been acknowledged by the execution provider
    ORDER_STATE_REPLACE_PENDING = 4, // order has an outstanding cancel replace request
    ORDER_STATE_REPLACED = 5, // order has been successfully cancel replaced
    ORDER_STATE_CANCEL_PENDING = 6, // order has an outstanding cancel request
    ORDER_STATE_CANCELLED = 7, // order has been cancelled and may or may not have some filled quantity
    ORDER_STATE_PARTIALLY_FILLED = 8, // order is working and has some executed quantity
    ORDER_STATE_FILLED = 9, // order is fully filled and completed
    ORDER_STATE_REJECTED = 10, // order has been rejected
    ORDER_STATE_BUSTED = 11 // order has been busted
};

inline std::string OrderStateToString(OrderState orderState)
{
    switch (orderState) {
        case ORDER_STATE_NEW_ORDER:
            return "NEW_ORDER";
        case ORDER_STATE_OPEN_PENDING:
            return "OPEN_PENDING";
        case ORDER_STATE_OPEN:
            return "OPEN";
        case ORDER_STATE_REPLACE_PENDING:
            return "REPLACE_PENDING";
        case ORDER_STATE_REPLACED:
            return "REPLACED";
        case ORDER_STATE_CANCEL_PENDING:
            return "CANCEL_PENDING";
        case ORDER_STATE_CANCELLED:
            return "CANCELLED";
        case ORDER_STATE_PARTIALLY_FILLED:
            return "PARTIALLY_FILLED";
        case ORDER_STATE_FILLED:
            return "FILLED";
        case ORDER_STATE_REJECTED:
            return "REJECTED";
        case ORDER_STATE_BUSTED:
            return "BUSTED";
        case ORDER_STATE_UNKNOWN: // fall-through
        default:
            return "UNKNOWN";
    }
}

inline OrderState StringToOrderState(const std::string& orderState)
{
    if (orderState.compare("NEW_ORDER") == 0)
        return ORDER_STATE_NEW_ORDER;
    else if (orderState.compare("OPEN_PENDING") == 0)
        return ORDER_STATE_OPEN_PENDING;
    else if (orderState.compare("OPEN") == 0)
        return ORDER_STATE_OPEN;
    else if (orderState.compare("REPLACE_PENDING") == 0)
        return ORDER_STATE_REPLACE_PENDING;
    else if (orderState.compare("REPLACED") == 0)
        return ORDER_STATE_REPLACED;
    else if (orderState.compare("CANCEL_PENDING") == 0)
        return ORDER_STATE_CANCEL_PENDING;
    else if (orderState.compare("CANCELLED") == 0)
        return ORDER_STATE_CANCELLED;
    else if (orderState.compare("PARTIALLY_FILLED") == 0)
        return ORDER_STATE_PARTIALLY_FILLED;
    else if (orderState.compare("FILLED") == 0)
        return ORDER_STATE_FILLED;
    else if (orderState.compare("REJECTED") == 0)
        return ORDER_STATE_REJECTED;
    else if (orderState.compare("BUSTED") == 0)
        return ORDER_STATE_BUSTED;
    else
        return ORDER_STATE_UNKNOWN;
}

enum OrderUpdateType {
    ORDER_UPDATE_TYPE_UNKNOWN = 0,
    ORDER_UPDATE_TYPE_OPEN_PENDING = 1,
    ORDER_UPDATE_TYPE_OPEN = 2,
    ORDER_UPDATE_TYPE_REPLACE_PENDING = 3,
    ORDER_UPDATE_TYPE_REPLACE_REJECT = 4,
    ORDER_UPDATE_TYPE_REPLACE = 5,
    ORDER_UPDATE_TYPE_CANCEL_PENDING = 6,
    ORDER_UPDATE_TYPE_CANCEL_REJECT = 7,
    ORDER_UPDATE_TYPE_CANCEL = 8,
    ORDER_UPDATE_TYPE_PARTIAL_FILL = 9,
    ORDER_UPDATE_TYPE_FILL = 10,  //Used for fills that complete an order
    ORDER_UPDATE_TYPE_REJECT = 11,
    ORDER_UPDATE_TYPE_BUST = 12
};

inline std::string OrderUpdateTypeToString(OrderUpdateType updateType)
{
    switch (updateType)	{
        case ORDER_UPDATE_TYPE_OPEN_PENDING:
            return "OPEN_PENDING";
        case ORDER_UPDATE_TYPE_OPEN:
            return "OPEN";
        case ORDER_UPDATE_TYPE_REPLACE_PENDING:
            return "REPLACE_PENDING";
        case ORDER_UPDATE_TYPE_REPLACE_REJECT:
            return "REPLACE_REJECT";
        case ORDER_UPDATE_TYPE_REPLACE:
            return "REPLACE";
        case ORDER_UPDATE_TYPE_CANCEL_PENDING:
            return "CANCEL_PENDING";
        case ORDER_UPDATE_TYPE_CANCEL_REJECT:
            return "CANCEL_REJECT";
        case ORDER_UPDATE_TYPE_CANCEL:
            return "CANCEL";
        case ORDER_UPDATE_TYPE_PARTIAL_FILL:
            return "PARTIAL_FILL";
        case ORDER_UPDATE_TYPE_FILL:
            return "FILL";
        case ORDER_UPDATE_TYPE_REJECT:
            return "REJECT";
        case ORDER_UPDATE_TYPE_BUST:
            return "BUST";
        case ORDER_UPDATE_TYPE_UNKNOWN:
        default:
            return "UNKNOWN";
    }
}

inline OrderUpdateType StringToOrderUpdateType(const std::string& updateType)
{
    if (updateType.compare("OPEN_PENDING") == 0)
        return ORDER_UPDATE_TYPE_OPEN_PENDING;
    if (updateType.compare("OPEN") == 0)
        return ORDER_UPDATE_TYPE_OPEN;
    if (updateType.compare("REPLACE_PENDING") == 0)
        return ORDER_UPDATE_TYPE_REPLACE_PENDING;
    if (updateType.compare("REPLACE_REJECT") == 0)
        return ORDER_UPDATE_TYPE_REPLACE_REJECT;
    if (updateType.compare("REPLACE") == 0)
        return ORDER_UPDATE_TYPE_REPLACE;
    if (updateType.compare("CANCEL_PENDING") == 0)
        return ORDER_UPDATE_TYPE_CANCEL_PENDING;
    if (updateType.compare("CANCEL_REJECT") == 0)
        return ORDER_UPDATE_TYPE_CANCEL_REJECT;
    if (updateType.compare("CANCEL") == 0)
        return ORDER_UPDATE_TYPE_CANCEL;
    if (updateType.compare("PARTIAL_FILL") == 0)
        return ORDER_UPDATE_TYPE_PARTIAL_FILL;
    if (updateType.compare("FILL") == 0)
        return ORDER_UPDATE_TYPE_FILL;
    if (updateType.compare("REJECT") == 0)
        return ORDER_UPDATE_TYPE_REJECT;
    if (updateType.compare("BUST") == 0)
        return ORDER_UPDATE_TYPE_BUST;
    else
        return ORDER_UPDATE_TYPE_UNKNOWN;
}

inline bool OrderIsNew(OrderState state)
{
    return ( (state == ORDER_STATE_NEW_ORDER) ||
             (state == ORDER_STATE_OPEN_PENDING) );
}

inline bool OrderIsOpen(OrderState state)
{
    return ( (state == ORDER_STATE_OPEN) ||
             (state == ORDER_STATE_REPLACE_PENDING) ||
             (state == ORDER_STATE_REPLACED) ||
             (state == ORDER_STATE_CANCEL_PENDING) ||
             (state == ORDER_STATE_PARTIALLY_FILLED) );
}

inline bool OrderIsComplete(OrderState state)
{
    return ( (state == ORDER_STATE_FILLED) ||
             (state == ORDER_STATE_CANCELLED) ||
             (state == ORDER_STATE_REJECTED) ||
             (state == ORDER_STATE_BUSTED) );
}

inline bool OrderIsCancelled(OrderState state)
{
    return (state == ORDER_STATE_CANCELLED);
}

inline bool OrderIsRejected(OrderState state)
{
     return (state == ORDER_STATE_REJECTED);
}

inline bool OrderIsCancelledOrRejected(OrderState state)
{
     return (OrderIsCancelled(state) || OrderIsRejected(state));
}

inline bool IsFillOrderUpdateType(OrderUpdateType updateType)
{
    return (updateType == ORDER_UPDATE_TYPE_PARTIAL_FILL) ||
            (updateType == ORDER_UPDATE_TYPE_FILL);
}

enum TransactionType {
    TRANSACTION_TYPE_UNKNOWN,
    TRANSACTION_TYPE_FILL,
    TRANSACTION_TYPE_BUST,
    TRANSACTION_TYPE_CORRECTION
};

inline std::string TransactionTypeToString(TransactionType transactionType)
{
    switch (transactionType) {
        case TRANSACTION_TYPE_FILL:
            return "FILL";
        case TRANSACTION_TYPE_BUST:
            return "BUST";
        case TRANSACTION_TYPE_CORRECTION:
            return "CORRECTION";
        default:
            return "UNKNOWN";
    }
}

inline TransactionType StringToTransactionType(const std::string& transactionType)
{
    if (transactionType.compare("FILL") == 0)
        return TRANSACTION_TYPE_FILL;
    else if (transactionType.compare("BUST") == 0)
        return TRANSACTION_TYPE_BUST;
    else if (transactionType.compare("CORRECTION") == 0)
        return TRANSACTION_TYPE_CORRECTION;
    else
        return TRANSACTION_TYPE_UNKNOWN;
}

/**
 * Enumerates the kinds of liquidity action
 */
enum LiquidityAction {
    LIQUIDITY_ACTION_UNKNOWN = 0,
    LIQUIDITY_ACTION_ADDED = 1,
    LIQUIDITY_ACTION_REMOVED = 2,
    LIQUIDITY_ACTION_AUCTION = 3,
    LIQUIDITY_ACTION_ROUTED = 4
};

inline std::string LiquidityActionToString(LiquidityAction liquidity)
{
    switch(liquidity) {
        case LIQUIDITY_ACTION_ADDED:
            return "ADDED";
        case LIQUIDITY_ACTION_REMOVED:
            return "REMOVED";
        case LIQUIDITY_ACTION_AUCTION:
            return "AUCTION";
        case LIQUIDITY_ACTION_ROUTED:
            return "ROUTED";
        case LIQUIDITY_ACTION_UNKNOWN:
        default:
            return "UNKNOWN";
    }
}

inline LiquidityAction StringToLiquidityAction(const std::string& liquidity)
{
    if (liquidity.compare("ADDED") == 0)
        return LIQUIDITY_ACTION_ADDED;
    else if (liquidity.compare("REMOVED") == 0)
        return LIQUIDITY_ACTION_REMOVED;
    else if (liquidity.compare("AUCTION") == 0)
        return LIQUIDITY_ACTION_AUCTION;
    else if (liquidity.compare("ROUTED") == 0)
        return LIQUIDITY_ACTION_ROUTED;
    else
        return LIQUIDITY_ACTION_UNKNOWN;
}

/**
 * Enumerates the kinds of time in force
 */
enum OrderTIF {
    ORDER_TIF_UNKNOWN = 0,
    ORDER_TIF_DAY = '0',
    ORDER_TIF_GTC = '1',
    ORDER_TIF_OPG = '2', // On Open
    ORDER_TIF_IOC = '3',
    ORDER_TIF_FOK = '4',
    ORDER_TIF_GTX = '5',
    ORDER_TIF_GTD = '6',
    ORDER_TIF_AT_THE_CLOSE = '7',
    ORDER_TIF_GOOD_THROUGH_CROSSING = '8',
    ORDER_TIF_AT_CROSSING = '9',
    ORDER_TIF_TIME_IN_MARKET = 'a',
};

inline std::string OrderTIFToString(OrderTIF tif)
{
    switch (tif) {
        case ORDER_TIF_DAY:
            return "DAY";
        case ORDER_TIF_GTC:
            return "GTC";
        case ORDER_TIF_OPG:
            return "OPG";
        case ORDER_TIF_IOC:
            return "IOC";
        case ORDER_TIF_FOK:
            return "FOK";
        case ORDER_TIF_GTX:
            return "GTX";
        case ORDER_TIF_GTD:
            return "GTD";
        case ORDER_TIF_AT_THE_CLOSE:
            return "AT_THE_CLOSE";
        case ORDER_TIF_GOOD_THROUGH_CROSSING:
            return "GOOD_THROUGH_CROSSING";
        case ORDER_TIF_AT_CROSSING:
            return "AT_CROSSING";
        case ORDER_TIF_TIME_IN_MARKET:
            return "TIME_IN_MARKET";
        case ORDER_SIDE_UNKNOWN:
        default:
            return "UNKNOWN";
    }
}

inline OrderTIF StringToOrderTIF(const std::string& tif)
{
    if (tif.compare("DAY") == 0)
        return ORDER_TIF_DAY;
    else if (tif.compare("GTC") == 0)
        return ORDER_TIF_GTC;
    else if (tif.compare("OPG") == 0)
        return ORDER_TIF_OPG;
    else if (tif.compare("IOC") == 0)
        return ORDER_TIF_IOC;
    else if (tif.compare("FOK") == 0)
        return ORDER_TIF_FOK;
    else if (tif.compare("GTX") == 0)
        return ORDER_TIF_GTX;
    else if (tif.compare("GTD") == 0)
        return ORDER_TIF_GTD;
    else if (tif.compare("AT_THE_CLOSE") == 0)
        return ORDER_TIF_AT_THE_CLOSE;
    else if (tif.compare("GOOD_THROUGH_CROSSING") == 0)
        return ORDER_TIF_GOOD_THROUGH_CROSSING;
    else if (tif.compare("AT_CROSSING") == 0)
        return ORDER_TIF_AT_CROSSING;
    else if (tif.compare("TIME_IN_MARKET") == 0)
        return ORDER_TIF_TIME_IN_MARKET;
    else
        return ORDER_TIF_UNKNOWN;
}

/**
 * Enumerates the various types of orders we can have
 */
enum OrderType {
    ORDER_TYPE_UNKNOWN = 0,
    ORDER_TYPE_MARKET = '1',
    ORDER_TYPE_LIMIT = '2',
    ORDER_TYPE_STOP = '3',
    ORDER_TYPE_STOP_LIMIT = '4',
    ORDER_TYPE_FOREX_SWAP = 'G',
    ORDER_TYPE_MARKET_WITH_LEFTOVER_AS_LIMIT = 'K'
};

inline std::string OrderTypeToString(OrderType type)
{
    switch (type) {
        case ORDER_TYPE_MARKET:
            return "MARKET";
        case ORDER_TYPE_LIMIT:
            return "LIMIT";
        case ORDER_TYPE_STOP:
            return "STOP";
        case ORDER_TYPE_STOP_LIMIT:
            return "STOP_LIMIT";
        case ORDER_TYPE_FOREX_SWAP:
            return "FOREX_SWAP";
        case ORDER_TYPE_MARKET_WITH_LEFTOVER_AS_LIMIT:
            return "MARKET_WITH_LEFTOVER_AS_LIMIT";
        case ORDER_TYPE_UNKNOWN:
        default:
            return "UNKNOWN";
    }
}

inline bool IsMarketOrderType(OrderType type)
{
    switch (type) {
        case ORDER_TYPE_MARKET:
        case ORDER_TYPE_STOP:
        case ORDER_TYPE_MARKET_WITH_LEFTOVER_AS_LIMIT:
            return true;
        default:
            return false;
    }
}

inline bool IsLimitOrderType(OrderType type)
{
    return !IsMarketOrderType(type);
}

inline OrderType StringToOrderType(const std::string& type)
{
    if (type.compare("MARKET") == 0)
        return ORDER_TYPE_MARKET;
    else if (type.compare("LIMIT") == 0)
        return ORDER_TYPE_LIMIT;
    else if (type.compare("STOP") == 0)
        return ORDER_TYPE_STOP;
    else if (type.compare("STOP_LIMIT") == 0)
        return ORDER_TYPE_STOP_LIMIT;
    else if (type.compare("FOREX_SWAP") == 0)
        return ORDER_TYPE_FOREX_SWAP;
    else if (type.compare("MARKET_WITH_LEFTOVER_AS_LIMIT") == 0)
        return ORDER_TYPE_MARKET_WITH_LEFTOVER_AS_LIMIT;
    else
        return ORDER_TYPE_UNKNOWN;
}

/**
 * Enumerates the various peg order types we can send
 */
enum PegType {
    PEG_TYPE_NONE = 0,
    PEG_TYPE_PRIMARY = 1, //pegs to the same side of the NBBO as teh order
    PEG_TYPE_MARKET = 2, //pegs to the opposite side of the NBBO from the order
    PEG_TYPE_MIDPOINT = 3,  //pegs to the midpoint of the spread
    PEG_TYPE_ALTERNATE_MIDPOINT = 4, //pegs to the midpoint, or 1 tick inside the primary side of the NBBO if less aggressive
    PEG_TYPE_PRICE_IMPROVED_PRIMARY = 5,
    PEG_TYPE_PRICE_IMPROVED_MARKET = 6,
    PEG_TYPE_PRICE_IMPROVED_MIDPOINT = 7,
    PEG_TYPE_DISCRETIONARY_MIDPOINT = 8,
    PEG_TYPE_PENNY_SPREAD_MIDPOINT = 9
};

inline std::string PegTypeToString(PegType pegType)
{
    switch (pegType) {
        case PEG_TYPE_PRIMARY:
            return "PRIMARY";
        case PEG_TYPE_MARKET:
            return "MARKET";
        case PEG_TYPE_MIDPOINT:
            return "MIDPOINT";
        case PEG_TYPE_ALTERNATE_MIDPOINT:
            return "ALTERNATE_MIDPOINT";
        case PEG_TYPE_PRICE_IMPROVED_PRIMARY:
            return "PRICE_IMPROVED_PRIMARY";
        case PEG_TYPE_PRICE_IMPROVED_MARKET:
            return "PRICE_IMPROVED_MARKET";
        case PEG_TYPE_PRICE_IMPROVED_MIDPOINT:
            return "PRICE_IMPROVED_MIDPOINT";
        case PEG_TYPE_DISCRETIONARY_MIDPOINT:
            return "DISCRETIONARY_MIDPOINT";
        case PEG_TYPE_PENNY_SPREAD_MIDPOINT:
            return "PENNY_SPREAD_MIDPOINT";
        case PEG_TYPE_NONE:
        default:
            return "NONE";
    }
}

inline PegType StringToPegType(const std::string& pegType)
{
    if (pegType.compare("PRIMARY") == 0)
        return PEG_TYPE_PRIMARY;
    else if (pegType.compare("MARKET") == 0)
        return PEG_TYPE_MARKET;
    else if (pegType.compare("MIDPOINT") == 0)
        return PEG_TYPE_MIDPOINT;
    else if (pegType.compare("ALTERNATE_MIDPOINT") == 0)
        return PEG_TYPE_ALTERNATE_MIDPOINT;
    else if (pegType.compare("PRICE_IMPROVED_PRIMARY") == 0)
        return PEG_TYPE_PRICE_IMPROVED_PRIMARY;
    else if (pegType.compare("PRICE_IMPROVED_MARKET") == 0)
        return PEG_TYPE_PRICE_IMPROVED_MARKET;
    else if (pegType.compare("PRICE_IMPROVED_MIDPOINT") == 0)
        return PEG_TYPE_PRICE_IMPROVED_MIDPOINT;
    else if (pegType.compare("DISCRETIONARY_MIDPOINT") == 0)
        return PEG_TYPE_DISCRETIONARY_MIDPOINT;
    else if (pegType.compare("PENNY_SPREAD_MIDPOINT") == 0)
        return PEG_TYPE_PENNY_SPREAD_MIDPOINT;
    else
        return PEG_TYPE_NONE;
}

/**
 * Execution handler mode
 */
enum ExecutionHandlerMode {
    EXECUTION_HANDLER_MODE_UNKNOWN = 0,
    EXECUTION_HANDLER_MODE_SIM = 1,
    EXECUTION_HANDLER_MODE_PROD = 2
};

typedef StrategyStudioUInt64 ExecutionProviderID;

/**
 * For use in order params
 */
typedef std::vector<boost::any> CustomParams;
typedef CustomParams::iterator CustomParamsIter;
typedef CustomParams::const_iterator CustomParamsConstIter;

/**
 * Tri-bool to allow for default
 */
enum AllowRouting {
    ALLOW_ROUTING_DEFAULT,
    ALLOW_ROUTING_FALSE,
    ALLOW_ROUTING_TRUE
};

/**
 * Actions to take for non-routable orders if order crosses/locks market.
 * Char-encodings are taken from Lime's FIX encoding.
 */
enum LockedCrossedAction {
    LOCKED_CROSSED_ACTION_DEFAULT = 0,
    LOCKED_CROSSED_ACTION_REJECT = 'R',
    LOCKED_CROSSED_ACTION_PRICE_ADJUST = 'P',
    LOCKED_CROSSED_ACTION_HIDE_NOT_SLIDE = 'H',
    LOCKED_CROSSED_ACTION_PRICE_ADJUST_REJECT_CROSSED = 'L',
    LOCKED_CROSSED_ACTION_SINGLE_REPRICE = 'S',
    LOCKED_CROSSED_ACTION_BLIND = 'B',
    LOCKED_CROSSED_ACTION_NO_MIDPOINT_MATCH = 'M',
    LOCKED_CROSSED_ACTION_MULTIPLE_DISPLAY_PRICE_SLIDING = 'D',
    LOCKED_CROSSED_ACTION_PROACTIVE_IF_LOCKED = 'A'
};

inline std::string LockedCrossedActionToString(LockedCrossedAction action)
{
    switch (action) {
        // Default indicates that the behavior specified in the route config should be used.
        // If config doesn't specify, the exchange's default will be used.
        case LOCKED_CROSSED_ACTION_DEFAULT: {
            return "Default";
        }
        case LOCKED_CROSSED_ACTION_REJECT: {
            return "Reject";
        }
        case LOCKED_CROSSED_ACTION_PRICE_ADJUST: {
            return "Price Adjust";
        }
        case LOCKED_CROSSED_ACTION_HIDE_NOT_SLIDE: {
            return "Hide Not Slide";
        }
        case LOCKED_CROSSED_ACTION_PRICE_ADJUST_REJECT_CROSSED: {
            return "Price Adjust Reject Crossed";
        }
        case LOCKED_CROSSED_ACTION_SINGLE_REPRICE: {
            return "Single Reprice";
        }
        case LOCKED_CROSSED_ACTION_BLIND: {
            return "Blind";
        }
        case LOCKED_CROSSED_ACTION_NO_MIDPOINT_MATCH: {
            return "No Midpoint Match";
        }
        case LOCKED_CROSSED_ACTION_MULTIPLE_DISPLAY_PRICE_SLIDING: {
            return "Multiple Display Price Sliding";
        }
        case LOCKED_CROSSED_ACTION_PROACTIVE_IF_LOCKED: {
            return "Proactive If Locked";
        }
        default:
            return "UNKNOWN";
    }
}

inline LockedCrossedAction StringToLockedCrossedAction(const std::string& type)
{
    if (type.compare("DEFAULT") == 0)
        return LOCKED_CROSSED_ACTION_DEFAULT;
    else if (type.compare("REJECT") == 0)
        return LOCKED_CROSSED_ACTION_REJECT;
    else if (type.compare("PRICE_ADJUST") == 0)
        return LOCKED_CROSSED_ACTION_PRICE_ADJUST;
    else if (type.compare("HIDE_NOT_SLIDE") == 0)
        return LOCKED_CROSSED_ACTION_HIDE_NOT_SLIDE;
    else if (type.compare("PRICE_ADJUST_REJECT_CROSSED") == 0)
        return LOCKED_CROSSED_ACTION_PRICE_ADJUST_REJECT_CROSSED;
    else if (type.compare("SINGLE_REPRICE") == 0)
        return LOCKED_CROSSED_ACTION_SINGLE_REPRICE;
    else if (type.compare("BLIND") == 0)
        return LOCKED_CROSSED_ACTION_BLIND;
    else if (type.compare("NO_MIDPOINT_MATCH") == 0)
        return LOCKED_CROSSED_ACTION_NO_MIDPOINT_MATCH;
    else if (type.compare("MULTIPLE_DISPLAY_PRICE_SLIDING") == 0)
        return LOCKED_CROSSED_ACTION_MULTIPLE_DISPLAY_PRICE_SLIDING;
    else
        return LOCKED_CROSSED_ACTION_DEFAULT;
}

/**
 * Actions to take when a self-trade is detected.
 */
enum WashTradePrevention {
    WASHTRADE_PREVENTION_NONE,
    WASHTRADE_PREVENTION_CANCEL_NEWEST,
    WASHTRADE_PREVENTION_CANCEL_OLDEST,
    WASHTRADE_PREVENTION_CANCEL_BOTH
};

inline std::string WashTradePreventionToString(WashTradePrevention wash_trade_prevention)
{
    switch (wash_trade_prevention) {
        case WASHTRADE_PREVENTION_NONE: {
            return "None";
        }
        case WASHTRADE_PREVENTION_CANCEL_NEWEST: {
            return "Cancel Newest";
        }
        case WASHTRADE_PREVENTION_CANCEL_OLDEST: {
            return "Cancel Oldest";
        }
        case WASHTRADE_PREVENTION_CANCEL_BOTH: {
            return "Cancel Both";
        }
        default:
            return "Unknown";
    }
}

inline WashTradePrevention StringToWashTradePrevention(const std::string& type)
{
    if (type.compare("NONE") == 0)
        return WASHTRADE_PREVENTION_NONE;
    else if (type.compare("CANCEL_NEWEST") == 0)
        return WASHTRADE_PREVENTION_CANCEL_NEWEST;
    else if (type.compare("CANCEL_OLDEST") == 0)
        return WASHTRADE_PREVENTION_CANCEL_OLDEST;
    else if (type.compare("CANCEL_BOTH") == 0)
        return WASHTRADE_PREVENTION_CANCEL_BOTH;
    else
        return WASHTRADE_PREVENTION_NONE;
}

} // namespace StrategyStudio
} // namespace RCM

#endif