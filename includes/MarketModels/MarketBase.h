/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/MarketBase.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_MARKET_BASE_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_MARKET_BASE_H_

#include "MarketCenterTypes.h"
#include "NBBOQuote.h"
#include "Trade.h"
#include <vector>
#include <utility>
#include <ostream>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * Contains base market price info on quotes/trades used for types of markets
 */
class MarketBase {
public:
    typedef std::vector<Quote> Quotes;
    typedef Quotes::iterator QuotesIter;
    typedef Quotes::const_iterator QuotesConstIter;

    typedef std::vector<Trade> Trades;
    typedef Trades::iterator TradesIter;
    typedef Trades::const_iterator TradesConstIter;

public:
    MarketBase();
    virtual ~MarketBase();

    QuotesIter quotes_begin() { return quotes_.begin(); }
    QuotesIter quotes_end() { return quotes_.end(); }

    QuotesConstIter quotes_begin() const { return quotes_.begin(); }
    QuotesConstIter quotes_end() const { return quotes_.end(); }

    FeedType preferred_quote_source() const { return preferred_quote_feed_type_; }
    virtual void set_preferred_quote_source(FeedType type) { preferred_quote_feed_type_ = type; }

    FeedType preferred_trade_source() const { return preferred_trade_feed_type_; }
    virtual void set_preferred_trade_source(FeedType type) { preferred_trade_feed_type_ = type; }

    bool withdraw_crossed_book() const { return withdraw_crossed_book_; }
    virtual void set_withdraw_crossed_book(bool flag) = 0;

    /**
     * Gets the market center's quote from the best available data source
     */
    const Quote& quote() const { return quotes_[preferred_quote_feed_type_]; }

    /**
     * Retrieve the quote based on the type we want
     */
    const Quote& quote(FeedType type) const { return quotes_[type]; }

    /**
     * Retrieves the most recent trade received from the preferred source of trade tick data for this market.
     *
     * By default, filters out trade.IsTradePriceNonCurrent; server config variable USE_OFFICIAL_LAST=true can be enabled to have
     * API filter to trade.DoesTradeUpdateMarketCenterLast instead, which for US Equities excludes odd lot and extended hours trade 
     * conditions when present.
     */
    const Trade& last_trade() const { return trades_[preferred_trade_feed_type_]; }

    /**
     * Retrieves the most recent trade received from the specified source of trade tick data.
     *
     * By default, filters out trade.IsTradePriceNonCurrent; server config variable USE_OFFICIAL_LAST=true can be enabled to have
     * API filter to trade.DoesTradeUpdateMarketCenterLast instead, which for US Equities excludes odd lot and extended hours trade 
     * conditions when present.
     */
    const Trade& last_trade(FeedType type) const { return trades_[type]; }

    /**
     * Check if there has been activity on this market center
     */
    bool IsActive() const { return is_active_; }

    /**
     * Reset state of market objects
     */
    void Reset();

    /**
     * Update the quote type
     */
    virtual void UpdateQuote(FeedType type, const Quote& quote);

    /**
     * Update last trade data
     * @returns true if update occurred, false otherwise
     */
    virtual bool UpdateLastTrade(const Trade& trade, FeedType type);

private:
    virtual void InitQuotes();
    virtual void InitTrades();

protected:
    Quotes quotes_;
    Trades trades_;
    FeedType preferred_quote_feed_type_; // set the preferred quote type
    FeedType preferred_trade_feed_type_; // set the preferred trade type
    bool is_active_;
    bool withdraw_crossed_book_;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif