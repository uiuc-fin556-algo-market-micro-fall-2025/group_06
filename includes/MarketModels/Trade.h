/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/Trade.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_TRADE_DATA_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_TRADE_DATA_H_

#include "MarketCenterTypes.h"
#include "ConditionCodes.h"
#include "TradeSide.h"
#include <Utilities/TimeType.h>
#include "MDBaseObject.h"
#include <string>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

#define NUM_TRADE_CONDITION_FIELDS 4

/**
 * Contains information on trades ticks
 */
class Trade : public MDBaseObject {
public:
    Trade();

    Trade(double lastPrice,
          int quantity,
          MarketCenterID market_center,
          TradeSide side = TRADE_SIDE_UNKNOWN,
          TradeCondition condition = TRADE_COND_REGULAR);

    Trade(double lastPrice,
          int quantity,
          MarketCenterID market_center,
          TimeType sourceTime,
          TimeType feedHandlerTime,
          TimeType adapterTime,
          TradeSide side = TRADE_SIDE_UNKNOWN,
          TradeCondition condition = TRADE_COND_REGULAR);

    /**
     * Constructs a trade with multiple trade conditions
     *
     * condition_converter is a function pointer which converts each char in conditions to a 
     * normalized TradeCondition. TRADE_COND_REGULAR will be populated if all conditions 
     * are REGULAR or UNKNOWN, or if a null function pointer is supplied
     */
    Trade(double lastPrice,
          int quantity,
          MarketCenterID market_center,
          TimeType sourceTime,
          TimeType feedHandlerTime,
          TimeType adapterTime,
          const std::string& conditions,
          TradeConditionConverter condition_converter,
          TradeSide side = TRADE_SIDE_UNKNOWN);

    /**
     * Constructs a trade with multiple trade conditions
     *
     * condition_converter is a function pointer which converts each char in conditions to a 
     * normalized TradeCondition. TRADE_COND_REGULAR will be populated if all conditions 
     * are REGULAR or UNKNOWN, or if a null function pointer is supplied
     */
    Trade(double lastPrice,
          int quantity,
          MarketCenterID market_center,
          TimeType sourceTime,
          TimeType feedHandlerTime,
          TimeType adapterTime,
          const char conditions[],
          size_t conditions_length, // not null padded
          TradeConditionConverter condition_converter,
          TradeSide side = TRADE_SIDE_UNKNOWN);

    /**
     * Constructs a trade with multiple trade conditions. TRADE_COND_REGULAR will be populated if all conditions 
     * are REGULAR or UNKNOWN
     */
    Trade(double lastPrice,
          int quantity,
          MarketCenterID market_center,
          TimeType sourceTime,
          TimeType feedHandlerTime,
          TimeType adapterTime,
          const std::vector<TradeCondition>& conditions,
          TradeSide side = TRADE_SIDE_UNKNOWN);

    /**
     * Constructs a trade with multiple trade conditions. TRADE_COND_REGULAR will be populated if all conditions 
     * are REGULAR or UNKNOWN
     */
    Trade(double lastPrice,
          int quantity,
          MarketCenterID market_center,
          TimeType sourceTime,
          TimeType feedHandlerTime,
          TimeType adapterTime,
          TradeCondition trade_condition,
          TradeCondition trade_condition2,
          TradeCondition trade_condition3,
          TradeCondition trade_condition4,
          TradeSide side = TRADE_SIDE_UNKNOWN);

    /**
     * Gets the price of the trade
     */
    double price() const { return price_; }
    void set_price(double price) { price_ = price; }

    /**
     * Gets the quantity of the trade
     */
    int size() const { return quantity_; }
    void set_size(int quantity) { quantity_ = quantity; }

    /**
     * Gets the side of the trade
     */
    TradeSide side() const { return side_; }
    void set_side(TradeSide side) { side_ = side; }

    /**
     * Gets the market center on which the trade occurred
     */
    MarketCenterID market_id() const { return market_center_; }
    void set_market_id(MarketCenterID market_center) { market_center_ = market_center; }

    /**
     * Verify that price and quantity are non-zero
     */
    bool IsValid() const;

    /**
     * Checks whether the trade tick carries with it an associated trade condition
     *
     * Since some datafeeds have multiple condition fields, this allows you to check
     * for the existence of a trade condition without having to worry about which field
     * should contain which subset of the trade conditions.
     *
     * @param condition The trade condition to check on
     * @returns whether the supplied condition is associated with the trade tick
     */
    bool HasSpecifiedTradeCondition(TradeCondition condition) const {return conditions_[kTradeConditionAttributes[condition] & 0x03] == static_cast<StrategyStudioUInt8>(condition);}

    /**
     * Accessors for the trade conditions
     */
    TradeCondition trade_condition() const { return static_cast<TradeCondition>(conditions_[0]); }
    TradeCondition trade_condition2() const { return static_cast<TradeCondition>(conditions_[1]); }
    TradeCondition trade_condition3() const { return static_cast<TradeCondition>(conditions_[2]); }
    TradeCondition trade_condition4() const { return static_cast<TradeCondition>(conditions_[3]); }

    /**
     * Checks whether trade's conditions indicate trade  was reported late, at a delay
     */
    bool IsTradeReportedLate() const;

    /**
     * Checks whether trade's conditions indicate a cancellation print
     */
    bool IsTradeACancel() const;

    /**
     * Checks whether trade's conditions indicate trade was reported out of sequence
     */
    bool IsTradeReportedOutOfSequence() const;

    /**
     * Checks whether trade's conditions indicate trade was executed as a block
     */
    bool IsTradeABlockTrade() const;

    /**
     * Checks whether trade's conditions indicate trade was executed during a crossing session (not inclusive of price improvement micro auctions/crosses)
     */
    bool IsTradeFromCrossingSession() const;

    /**
     * Is trade official open print, based on trade conditions.
     */
    bool IsTradeOfficialOpen() const;

    /**
     * Is trade official close print, based on trade conditions.
     */
    bool IsTradeOfficialClose() const;

    /**
     * Checks whether trade's conditions indicate trade may not be straightforwardly comparable to the current state of the market
     */
    bool IsTradePriceNonCurrent() const;

    /**
     * Checks whether a trade's conditions indicate trade has non-standard settlement terms
     */
    bool IsTradeSettlementNonStandard() const;

    /**
     * Checks whether trade's conditions indicate trade was exempt from RegNMS checks
     */
    bool IsTradeThroughExempt() const;

    /**
     * Checks whether trade's conditions indicate trade should be used to update daily volume stats
     */
    bool DoesTradeUpdateVolume() const;

    /**
     * Checks whether trade's conditions indicate trade should be used to update the consolidated Highs and Lows for the day
     *
     * Note when trade conditions are present, this will return false for extended hour and odd lot trades, per SIAC/UTP guidelines
     */
    bool DoesTradeUpdateConsolidatedHighLows() const;

    /**
     * Checks whether trade's conditions indicate trade should be used to update the consolidated last price print
     *
     * Note when trade conditions are present, this will return false for extended hour and odd lot trades, per SIAC/UTP guidelines
     */
    bool DoesTradeUpdateConsolidatedLast(bool isFirstTradeOfDay) const;

    /**
     * Checks whether trade's conditions indicate trade should be used to update the highs and lows for the MarketCenter on which the trade occurred
     *
     * Note when trade conditions are present, this will return false for extended hour and odd lot trades, per SIAC/UTP guidelines
     */
    bool DoesTradeUpdateMarketCenterHighLows() const;

    /**
     * Checks whether trade's conditions indicate trade should be used to update the last price print for the MarketCenter on which the trade occurred
     *
     * Note when trade conditions are present, this will return false for extended hour and odd lot trades, per SIAC/UTP guidelines
     */
    bool DoesTradeUpdateMarketCenterLast(bool isFirstTradeOfDay) const;

    /**
     * Update trade data with contents of another trade data
     */
    bool UpdateTradeData(const Trade& entry);

    bool operator==(const Trade& rhs) const;

    void Reset();

private:
    void InsertTradeCondition(TradeCondition condition);

private:
    double price_;
    int quantity_;
    MarketCenterID market_center_;
    TradeSide side_;
    StrategyStudioUInt8 conditions_[NUM_TRADE_CONDITION_FIELDS];
    StrategyStudioUInt16 flags_;
    static const StrategyStudioUInt16 kTradeConditionAttributes[];
};

inline bool Trade::IsValid() const
{
    return (quantity_ != 0);
}

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif