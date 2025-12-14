/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/DepthDataTypes.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_DEPTH_DATA_TYPES_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_DEPTH_DATA_TYPES_H_

#include "MarketCenterTypes.h"
#include <Utilities/StdTypes.h>
#include <ostream>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

enum DepthSide {
    DEPTH_SIDE_UNKNOWN = 0,
    DEPTH_SIDE_BUY = 1,
    DEPTH_SIDE_SELL = 2
};

enum DepthUpdateType {
    DEPTH_UPDATE_TYPE_UNKNOWN = 0,
    DEPTH_UPDATE_TYPE_BY_PRICE = 1,                  // depth update by price
    DEPTH_UPDATE_TYPE_BY_ORDER = 2,                  // depth update by order
    DEPTH_UPDATE_TYPE_RESET_ORDER_BOOK = 3,          // command to reset order book
    DEPTH_UPDATE_TYPE_JOIN_IN_AGGREGATE = 5,         // market center's order book added for inclusion in the aggr order book during runtime
    DEPTH_UPDATE_TYPE_WITHDRAW_FROM_AGGREGATE = 6,   // market center's order book removed from the aggr order book during runtime
    DEPTH_UPDATE_TYPE_BOOK_INITIALIZING = 7,         // order book is entering an initialization state
    DEPTH_UPDATE_TYPE_BOOK_INITIALIZED = 8           // order book initialization is complete
};

enum DepthReasonForChange {
    DEPTH_REASON_FOR_CHANGE_UNKNOWN = 0,
    DEPTH_REASON_FOR_CHANGE_UNATTRIBUTED_CHANGE = 1, // for use by adapters only, order book will attempt to classify Reason. DEPTH_UPDATE_TYPE_BY_ORDER modifications other than to reduce size lose queue position unless priority_indicator is input as a non-zero value and indicates otherwise
    DEPTH_REASON_FOR_CHANGE_ADD_ORDER = 2,           // new order added. if DEPTH_UPDATE_TYPE_BY_PRICE, also used to indicate a new price level
    DEPTH_REASON_FOR_CHANGE_PARTIAL_CANCEL = 3,      // modification to size as result of partial cancel or if executions not flagged by data source, remaining quantity maintains priority
    DEPTH_REASON_FOR_CHANGE_FULL_CANCEL = 4,         // deletion of order due to full cancellation or if executions not identified by data source. if DEPTH_UPDATE_TYPE_BY_PRICE, also used for deleted price levels if reason is unattributed by data source
    DEPTH_REASON_FOR_CHANGE_EXECUTED = 5,            // deletion or reduction of order due to execution, remaining quantity maintains priority
    DEPTH_REASON_FOR_CHANGE_EXECUTED_AT_PRICE = 6,   // deletion or reduction of order due to execution at diff price, remaining quantity maintains priority
    DEPTH_REASON_FOR_CHANGE_MODIFY = 7,              // cancel/replace without generating new order id, priority_indicator field specifies whether to reset queue priority if price unchanged. if DEPTH_UPDATE_TYPE_BY_PRICE, also used for modified price levels if reason is unattributed by data source
    DEPTH_REASON_FOR_CHANGE_CANCEL_REPLACE = 8,      // cancel/replace that generates new order id, order loses priority.
};

/**
 * Contains result of process depth update functionality
 */
struct DepthUpdateResult {
public:
    DepthUpdateResult();
    DepthUpdateResult(bool orderBookUpdated, bool marketCenterBestUpdated, bool topQuoteUpdated);
    void Reset();

public:
    bool order_book_was_updated;
    bool market_center_best_was_updated;
    bool top_quote_was_updated;
};

typedef StrategyStudioUInt64 DepthOrderID;

inline std::ostream &operator<<(std::ostream &os, const DepthUpdateResult &result)
{
    os << "Order Book Updated: " << (result.order_book_was_updated ? "TRUE" : "FALSE") << std::endl;
    os << "Market Center Best Updated: " << (result.market_center_best_was_updated ? "TRUE" : "FALSE") << std::endl;
    os << "Top Quote Updated: " << (result.top_quote_was_updated ? "TRUE" : "FALSE") << std::endl;
    return os;
}

inline std::string ReasonForChangeToString(DepthReasonForChange reason)
{
    switch (reason) {
        case DEPTH_REASON_FOR_CHANGE_UNATTRIBUTED_CHANGE:
            return "UNATTRIBUTED_CHANGE";
            break;
        case DEPTH_REASON_FOR_CHANGE_ADD_ORDER:
            return "ADD_ORDER";
            break;
        case DEPTH_REASON_FOR_CHANGE_PARTIAL_CANCEL:
            return "PARTIAL_CANCEL";
            break;
        case DEPTH_REASON_FOR_CHANGE_FULL_CANCEL:
            return "FULL_CANCEL";
            break;
        case DEPTH_REASON_FOR_CHANGE_EXECUTED:
            return "EXECUTED";
            break;
        case DEPTH_REASON_FOR_CHANGE_EXECUTED_AT_PRICE:
            return "EXECUTED_AT_PRICE";
            break;
        case DEPTH_REASON_FOR_CHANGE_MODIFY:
            return "MODIFY";
            break;
        case DEPTH_REASON_FOR_CHANGE_CANCEL_REPLACE:
            return "CANCEL_REPLACE";
            break;
        case DEPTH_REASON_FOR_CHANGE_UNKNOWN: // fall-through
        default:
            return "UNKNOWN";
            break;
    }
}

inline std::string DepthUpdateTypeToString(DepthUpdateType updateType)
{
    switch (updateType) {
        case DEPTH_UPDATE_TYPE_BY_PRICE:
            return "BY_PRICE";
        case DEPTH_UPDATE_TYPE_BY_ORDER:
            return "BY_ORDER";
        case DEPTH_UPDATE_TYPE_RESET_ORDER_BOOK:
            return "RESET_ORDER_BOOK";
        case DEPTH_UPDATE_TYPE_JOIN_IN_AGGREGATE:
            return "JOIN_IN_AGGREGATE";
        case DEPTH_UPDATE_TYPE_WITHDRAW_FROM_AGGREGATE:
            return "WITHDRAW_FROM_AGGREGATE";
        case DEPTH_UPDATE_TYPE_BOOK_INITIALIZING:
            return "BOOK_INITIALIZING";
        case DEPTH_UPDATE_TYPE_BOOK_INITIALIZED:
            return "BOOK_INITIALIZED";
        case DEPTH_UPDATE_TYPE_UNKNOWN:
        default:
            return "Unknown";
    }
}

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif