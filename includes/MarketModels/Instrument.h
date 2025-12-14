/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/Instrument.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_INSTRUMENT_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_INSTRUMENT_H_

#include <Utilities/TimeType.h>
#include "QuoteSide.h"
#include "Quote.h"
#include "Markets.h"
#include "MarketCenter.h"
#include "MarketCenterInfoFactory.h"
#include "IAggrOrderBook.h"
#include "NBBOQuote.h"
#include "IOrderBookBuilder.h"
#include "CurrencyTypes.h"

#include <boost/math/special_functions/round.hpp>

#include <time.h>
#include <map>
#include <string>
#include <set>
#include <ostream>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

class ConversionRate;

typedef std::string SymbolTag;
typedef unsigned InstrumentID;
typedef std::map<StrategyStudioInt64, double> TickSizes;  // Price-TickSize pairs - this must be an ordered container

/**
 * Lists the supported instrument types
 */
enum InstrumentType {
    INSTRUMENT_TYPE_UNKNOWN = 0,
    INSTRUMENT_TYPE_EQUITY = 1,
    INSTRUMENT_TYPE_FUTURE = 2,
    INSTRUMENT_TYPE_OPTION = 3,
    INSTRUMENT_TYPE_FX = 4,
    INSTRUMENT_TYPE_BOND = 5,
    INSTRUMENT_TYPE_INDEX = 6,
    INSTRUMENT_TYPE_FUND = 7
};

/**
 * Converts InstrumentType into its string representation
 */
inline std::string InstrumentTypeToString(InstrumentType secType) {
    switch (secType) {
        case INSTRUMENT_TYPE_EQUITY:
            return "INSTRUMENT_TYPE_EQUITY";
        case INSTRUMENT_TYPE_FUTURE:
            return "INSTRUMENT_TYPE_FUTURE";
        case INSTRUMENT_TYPE_OPTION:
            return "INSTRUMENT_TYPE_OPTION";
        case INSTRUMENT_TYPE_FX:
            return "INSTRUMENT_TYPE_FX";
        case INSTRUMENT_TYPE_BOND:
            return "INSTRUMENT_TYPE_BOND";
        default:
            return "INSTRUMENT_TYPE_UNKNOWN";
    }
}

/**
 * Provides the base functionality for all instrument types.
 *
 * It contains the instrument's basic descriptive information and the current state of its
 * market activity.
 */
class Instrument {
public:
    typedef std::set<Instrument*> Children;
    typedef Children::iterator ChildrenIter;
    typedef Children::const_iterator ChildrenConstIter;

    typedef Markets::QuoteUpdateResult QuoteUpdateResult;

public:
    Instrument(InstrumentID instrID,
               const SymbolTag& symbol,
               const InstrumentType secType,
               MarketType marketType,
               const Markets::MarketCenterIDCollection& marketCenterIDs,
               IOrderBookBuilderPtr pOrderBookBuilder,
               MarketCenterInfoFactory* marketCenterInfoFactory);

    virtual ~Instrument();

    IAggrOrderBook& aggregate_order_book() { return markets_.aggregate_order_book(); }
    const IAggrOrderBook& aggregate_order_book() const { return markets_.aggregate_order_book(); }

    /**
     * Gets the instrument's symbol
     */
    const SymbolTag& symbol() const { return symbol_; }

    /**
     * Sets the instrument's symbol
     */
    void set_symbol(const SymbolTag& symbol) { symbol_ = symbol; }

    /**
     * Gets the instrument's id
     */
    InstrumentID id() const { return instrument_id_; }

    /**
     * Retrieves the instrument's type
     *
     * This can be checked to cast to the appropriate Instrument specialization to look up
     * more specialized instrument information
     */
    InstrumentType type() const { return instrument_type_; }

    /**
     * Retrieves the instrument's underlier
     *
     * @return null if the instrument is an underlier, or if the instrument's underlier is not registered
     */
    const Instrument* underlier() const  { return underlier_; }

    void set_underlier(Instrument* instrument) { underlier_ = instrument; }

    ///
    /// Non-const accessor to the markets collection
    ///
    Markets& markets() { return markets_; }

    /**
     * Retrieve the markets collection
     */
    const Markets& markets() const { return markets_; }

    MarketType market_type() const { return market_type_; }

    /**
     * Retrieves the current date's last source time of any trade, quote, or depth event from any data source or feed type
     */
    TimeType source_time() const { return source_update_time_; }
    void set_source_time(TimeType time) { source_update_time_ = time; }

    /**
     * Returns Strategy Studio's calculated TopQuote for the instrument.
     *
     * This is a synthetic quote Strategy Studio maintains; it provides a summary of total quote liquidity at the best bid and best offered prices, summed
     * across all market centers quoting at those prices. Strategy Studio bases this calculation on the
     * best available quote source for each market center.
     *
     * If the quote datasources only provide ticks that change the NBBO, top quote will be set to NBBO prices and sizes
     */
    const Quote& top_quote() const { return markets_.top_quote(); }

    NBBOQuote& nbbo() { return nbbo_quote_; }

    /**
     * Retrieves the official NBBO quote for the instrument, if available. If not it returns an empty quote.
     *
     * This is the National Best Bid/Best Offer as calculated and disseminated by the SIP (for example, CTA, UTP, OPRA)
     */
    const NBBOQuote& nbbo() const { return nbbo_quote_; }

    /**
     * Retrieves the most recent trade received from the preferred source of a market center's trade tick data
     *
     * By default, filters out trade.IsTradePriceNonCurrent; server config variable USE_OFFICIAL_LAST=true can be enabled to have
     * API filter to trade.DoesTradeUpdateConsolidatedLast instead, which for US Equities excludes odd lot and extended hours trade 
     * conditions when present.
     */
    const Trade& last_trade() const { return markets_.last_trade(); }

    /**
     * Retrieves daily trading statistics
     */
    const TradeStats& trade_stats() const { return markets_.trade_stats(); }

    bool is_first_trade_received() const { return first_trade_received_; }

    /**
     * Retrieves the instrument's current tick size
     */
    double min_tick_size() const;

    /**
     * Get/Set tick sizes based on price
     */
    double min_tick_size(double price) const;
    void set_tick_sizes(const TickSizes* tickSizes);

    /**
     * Retrieves the instrument's contract value multiplier
     */
    double multiplier() const { return multiplier_; }
    void set_multiplier(double mult) { multiplier_ = mult; }

    /**
     * Controls whether crossed order books contribute to the aggregate
     */
    bool withdraw_crossed_books() const { return markets_.withdraw_crossed_books(); }
    void set_withdraw_crossed_books(bool flag);

    /**
     * Controls whether last trade is filtered by DoesTradeUpdateConsolidated/MarketCenterLast vs !IsTradePriceNonCurrent
     */
    bool use_official_last() const { return markets_.use_official_last(); }
    void set_use_official_last(bool use_official_last) { markets_.set_use_official_last(use_official_last); }

    /**
     * Retrieves the instrument's quoting currency
     */
    CurrencyType currency() const;

    /**
     * Retrieves conversion rate to translate from instrument's valuation currency to server's home accounting currency
     */
    double conversion_rate() const;
    void set_conversion_rate(const ConversionRate* conversionRate) { quote_conversion_rate_ = conversionRate; }

    ChildrenIter children_begin() { return children_.begin(); }
    ChildrenIter children_end() { return children_.end(); }
    ChildrenConstIter children_begin() const { return children_.begin(); }
    ChildrenConstIter children_end() const { return children_.end(); }

    FeedType preferred_quote_feed(MarketCenterID marketCenter) const { return markets_.preferred_quote_feed(marketCenter); }
    FeedType preferred_trade_feed(MarketCenterID marketCenter) const { return markets_.preferred_quote_feed(marketCenter); }

    void set_preferred_quote_feed(MarketCenterID marketCenter, FeedType quoteFeedType);
    void set_preferred_trade_feed(MarketCenterID marketCenter, FeedType tradeFeedType);

    /**
     * Add a instrument as a child to this instrument
     *
     * Does not register the child instrument if instrument already present in children list
     * Child instruments are used for quick access to an underlier's derivatives
     *
     * @returns true if instrument registered, false otherwise
     */
    bool AddChildInstrument(Instrument* instrument);

    /**
     * Check if a particular instrument is already registered as a child
     *
     * @returns true if instrument present in children list, false otherwise
     */
    bool HasChildInstrument(Instrument* instrument) const;

    /**
     * Check if our calculated top quote is better than the consolidated NBBO
     */
    bool IsCalcTopQuoteBetter();

    /**
     * Print price data for this instrument
     */
    void PrintMarketPriceData(std::ostream& out) const;

    /**
     * Resets prices on instrument
     */
    void ResetPrices();

    /**
     * Simple equality comparison that checks that the securities
     * symbols are equivalent
     * @return true if passes equality check, false otherwise
     */
    bool SymbolEquals(const Instrument& instrument) const;

    /**
     * Updates instrument state from a trade
     */
    void UpdateFromTrade(const Trade& trade, FeedType feedType, TimeType eventTime);

    /**
     * Updates only the bid-ask prices and sizes from a new quote for a market center's data
     * @returns QuoteUpdateResult (see typedef)
     */
    QuoteUpdateResult UpdateInstrumentQuote(MarketCenterID exch, const Quote& quote, FeedType feedType);

    // Used to set top_quote to NBBO in the case where NBBO ticks are the only ticks in the consolidated feed sources
    // @returns whether NBBO was used to update the top_quote
    bool UpdateTopQuoteFromNBBO(const NBBOQuote& quote, FeedType feedType);

    /**
     * Updates only the bid-ask prices and sizes from a new quote
     */
    void UpdateNBBO(const NBBOQuote& quote);

    /**
     * Equality operator, does additional dynamic comparison via Compare()
     */
    bool operator==(const Instrument& instrument) const;

    friend std::ostream& operator<<(std::ostream& out, const Instrument& sec);

private:
    /**
     * Does additional setting of min tick size depending on instrument
     * Default doesn't do anything
     */
    virtual void CalcMinTickSize();

    Instrument(const Instrument& instrument); // not implemented
    Instrument& operator=(const Instrument& instrument); // not implemented

protected: /* Information common for all securities */
    InstrumentID instrument_id_;
    SymbolTag symbol_;
    InstrumentType instrument_type_;
    NBBOQuote nbbo_quote_;
    TimeType source_update_time_;
    Instrument* underlier_; // Used for derivative instrument
    Children children_; // Lists children securities
    bool first_trade_received_; // set to true when first trade received
    double min_tick_size_; // minimum tick size
    double multiplier_; // multiplier for instrument
    IOrderBookBuilderPtr order_book_builder_; // used for constructing orderbooks in derived objects
    Markets markets_;
    MarketType market_type_;
    const ConversionRate* quote_conversion_rate_; // to convert from quote to home currency
    const TickSizes* tick_sizes_;
};

inline std::ostream& operator<<(std::ostream& out, const Instrument& sec)
{
    out << "SymbolTag: " << sec.symbol() << " ";
    out << "Type: " << InstrumentTypeToString(sec.type()) << std::endl;
    out << "Top quote: " << sec.top_quote() << std::endl;
    out << "NBBO Consolidated: " << sec.nbbo() << std::endl;
    out << "Last Trade: " << sec.last_trade().price() << std::endl;
    out << "Last Size: " << sec.last_trade().size() << std::endl;
    out << sec.markets_ << std::endl;
    return out;
}

/**
 * Checks if a price is valid in terms of the instrument's minimum tick size
 */
inline bool IsValidPriceIncrement(const Instrument* instrument, double price)
{
    double min_tick_size = instrument->min_tick_size(price);
    return fabs(boost::math::round(price / min_tick_size) * min_tick_size - price) <= Utilities::PRICE_EPSILON * min_tick_size;
}

/**
 * Returns the price rounded to the nearest minimum tick size increment
 */
inline double RoundPriceToNearestTick(const Instrument* instrument, double price) {
    double min_tick_size = instrument->min_tick_size(price);
    double epsilon = price < 0 ? -min_tick_size / 2 : min_tick_size / 2;
    StrategyStudioInt64 ticks = static_cast<StrategyStudioInt64>((price + epsilon) / min_tick_size);
    return (ticks * min_tick_size);
}

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif