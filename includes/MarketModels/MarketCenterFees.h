/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/MarketCenterFees.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_MARKET_CENTER_FEES_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_MARKET_CENTER_FEES_H_

#include "MarketCenterTypes.h"

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * Enumerates different standards for unit of measurement in market center fees
 */
enum MarketCenterFeeType {
    MARKET_CENTER_FEE_TYPE_CURRENCY,
    MARKET_CENTER_FEE_TYPE_PERCENTAGE
};

/**
 * Contains exchange specific params for customizing behavior by different exchanges
 */
struct MarketCenterFees {
public:
    MarketCenterFees();

    MarketCenterFees(MarketCenterID market,
                     double taking_fee,
                     double posting_fee,
                     double posting_midpoint_fee,
                     double posting_hidden_fee,
                     double routing_fee,
                     double opening_auction_fee,
                     double closing_auction_fee,
                     MarketCenterFeeType fee_type);

public:
    /**
     * The fee for removing/taking liquidity on the market center
     *
     * Note: negative numbers indicate a rebate
     */
    double taking_fee;

    /**
     * The fee for adding/posting liquidity on the market center
     *
     * Note: negative numbers indicate a rebate
     */
    double posting_fee;

    /**
    * The fee for posting midpoint liquidity on the market center, applied if
    * liquidity action is ADDED and the order's peg_type is midpoint
    *
    * Note: negative numbers indicate a rebate
    */
    double posting_midpoint_fee;

    /**
    * The fee for posting midpoint liquidity on the market center, applied if
    * liquidity action is ADDED and the order params has is_hidden set to true
    * or peg_type is set to peg other than midpoint
    *
    * Note: negative numbers indicate a rebate
    */
    double posting_hidden_fee;

    /**
    * The fee for fills that route out to another market center
    *
    * Note: negative numbers indicate a rebate
    */
    double routing_fee;

    /**
     * The fee for participating in the opening auction on the market center
     *
     * Note: negative numbers indicate a rebate
     */
    double opening_auction_fee;

    /**
     * The fee for participating in the closing auction on the market center
     *
     * Note: negative numbers indicate a rebate
     */
    double closing_auction_fee;

    MarketCenterID market_id;

    /**
     * Indicates whether the fees are represented in currency per share/contract or percentage of notional
     */
    MarketCenterFeeType fee_type;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif
