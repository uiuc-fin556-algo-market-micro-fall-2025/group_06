/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/AccountFees.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_ACCOUNT_FEES_H
#define _STRATEGY_STUDIO_MARKET_MODELS_ACCOUNT_FEES_H

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * This struct contains the fees that are associated with a trading account.
 */
struct AccountFees {
public:
    AccountFees();

    AccountFees(double fBrkClrFeesPerOrder,
                double fBrkClrFeesPerContract,
                double fFeesPercentageOfNotional,
                double fSalesFeesPercentageOfNotional,
                double fSalesFeesPerShare,
                bool bPassesThroughExchangeFees);

public:
    /**
     * Represents the costs per order.
     *
     * Should be the sum of the applicable brokerage and clearing fees, and any other applicable per order fee
     */
    double fees_per_order;

    /**
     * Represents the costs per contract.
     *
     * Should be the sum of the applicable brokerage and clearing fees, and any other applicable per contract fee
     */
    double fees_per_contract;

    /**
     * Represents fees as a percentage of notional.
     *
     * This number is represented in raw decimal format, so .01 represents a 1% transaction fee
     */
    double fees_percentage_of_notional;

    /**
     * Represents sales fees as a percentage of notional; these fees are only applied on the sale of an instrument.
     *
     * An example of this fee type are the SEC Section 31 Transaction fees
     * This number is represented in raw decimal format, so .01 represents a 1% transaction fee
     */
    double sales_fees_percentage_of_notional;

    /**
     * Represents sales fees per share; these fees are only applied on the sale of an instrument.
     *
     * An example of this fee type are the FINRA TAF fees
     * This number is represented in raw decimal format, so .01 represents a 1% transaction fee
     */
    double sales_fees_per_share;

    /**
     * Indicates whether the account's broker passes through exchange fees
     *
     * If this flag is set to true, Strategy Studio will try to listen to whether a fill has added or removed liquidity, and will apply
     * the exchange's posting/taking fees/rebates as appropriate
     */
    bool passes_through_exchange_fees;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif
