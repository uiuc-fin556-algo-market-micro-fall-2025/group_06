/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/QuoteEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_QUOTE_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_QUOTE_EVENT_MSG_H_

#include "DataTypes.h"
#include "MarketDataBaseEventMsg.h"
#include "Event.h"
#include <Utilities/StrategyStudioException.h>

namespace RCM {
namespace StrategyStudio {

class Event;

enum QuoteUpdateType {
    QUOTE_UPDATE_TYPE_UNKNOWN = 0,
    QUOTE_UPDATE_TYPE_NBBO_AND_MARKET_CENTER = 1, // include both NBBO and market center quote
    QUOTE_UPDATE_TYPE_NBBO_ONLY = 2, // include only NBBO quote
    QUOTE_UPDATE_TYPE_MARKET_CENTER_ONLY = 3, // include only market center quote
    QUOTE_UPDATE_TYPE_MARKET_CENTER_ONLY_BID_SIDE = 4, // include only bid side for a market center
    QUOTE_UPDATE_TYPE_MARKET_CENTER_ONLY_ASK_SIDE = 5, // include only bid side for a market center
    QUOTE_UPDATE_TYPE_TOP_UPDATE = 6 // generated top from other data
};

/**
 * QuoteEventMsg is an event message representing a quote event from a market data feed.
 */
class QuoteEventMsg : public MarketDataBaseEventMsg {
public:
    // Default empty market data msg
    QuoteEventMsg(FeedType quoteType,
                  Event* event,
                  Instrument* instrument,
                  QuoteUpdateType mdType);

    // Constructor for a quote when feed only delivers the side that changed
    QuoteEventMsg(FeedType quoteType,
                  Event* event,
                  Instrument* instrument,
                  double price,
                  int size,
                  MarketCenterID exchange,
                  bool isBid,
                  QuoteCondition cond,
                  TimeType sourceTime,
                  TimeType feedHandlerTime,
                  TimeType adapterTime);

    // Constructor for a calculated quote where new top or direct (calculated from order book) is propagated
    // Note that update type mut be either QUOTE_UPDATE_TYPE_TOP_OB_UPDATE or QUOTE_UPDATE_TYPE_DIRECT_OB_UPDATE
    QuoteEventMsg(FeedType quoteType,
                  Event* event,
                  Instrument* instrument,
                  QuoteUpdateType updateType,
                  double calcBidPrice,
                  double calcAskPrice,
                  int calcBidSize,
                  int calcAskSize,
                  MarketCenterID calcBidExchange,
                  MarketCenterID calcAskExchange,
                  TimeType bidSourceTime,
                  TimeType bidFeedHandlerTime,
                  TimeType askSourceTime,
                  TimeType askFeedHandlerTime,
                  TimeType adapterTime);

    // Constructor for quotes that contain information about a market center but do not contain an NBBO update
    QuoteEventMsg(FeedType quoteType,
                  Event* event,
                  Instrument* instrument,
                  double bidPrice,
                  double askPrice,
                  int bidSize,
                  int askSize,
                  MarketCenterID Exchange,
                  QuoteCondition cond,
                  TimeType bidSourceTime,
                  TimeType bidFeedHandlerTime,
                  TimeType askSourceTime,
                  TimeType askFeedHandlerTime,
                  TimeType adapterTime);


    // Constructor for quote updates that contain separate information about the market center's quote and the NBBO and their times
    QuoteEventMsg(FeedType quoteType,
                  Event* event,
                  Instrument* instrument,
                  double nbboBidPrice,
                  double nbboAskPrice,
                  int nbboBidSize,
                  int nbboAskSize,
                  MarketCenterID nbboBidExchange,
                  MarketCenterID nbboAskExchange,
                  QuoteCondition nbboCond,
                  TimeType nbboBidSourceTime,
                  TimeType nbboAskSourceTime,
                  double bidPrice,
                  double askPrice,
                  int bidSize,
                  int askSize,
                  MarketCenterID exchange,
                  QuoteCondition cond,
                  TimeType bidSourceTime,
                  TimeType askSourceTime,
                  TimeType feedHandlerTime,
                  TimeType adapterTime);

    // Constructor for quote sources that only supply ticks that changed the NBBO
    QuoteEventMsg(FeedType quoteType,
                  Event* event,
                  Instrument* instrument,
                  double nbboBidPrice,
                  double nbboAskPrice,
                  int nbboBidSize,
                  int nbboAskSize,
                  MarketCenterID nbboBidExchange,
                  MarketCenterID nbboAskExchange,
                  TimeType sourceTime,
                  TimeType feedHandlerTime,
                  TimeType adapterTime);

    std::string name() const;

    /**
     * Get the quote type for this quote event
     */
    FeedType quote_feed_type() const { return quote_feed_type_; }

    // modify the quote feed type
    void set_quote_feed_type(FeedType feedType) { quote_feed_type_ = feedType; }

     /**
      * Gets the earlier source time among either bid or ask
      */
    TimeType source_time() const;

    /**
     * Gets the earlier feed handler time among either bid or ask
     */
    TimeType feed_handler_time() const;

    /**
     * Gets the time when the Strategy Server's data adapter received the market data event
     */
    TimeType adapter_time() const;

    /**
     * Returns a const reference to the instrument for which the quote was updated
     */
    const Instrument& instrument() const { return *instrument_; }

    /**
     * Returns the quote which was updated
     */
    const Quote& quote() const;

    /**
     * Check if the current quote updates the NBBO, ony applicable when feeds deliver the official NBBO
     */
    bool updates_nbbo() const;

    /**
     * Returns the NBBO, ony populated if the feed delivers the official NBBO and this tick contains an NBBO update.
     */
    const NBBOQuote& nbbo() const { return nbbo_quote_update_; }

    // Returns the type of quote update
    QuoteUpdateType update_type() const { return quote_update_type_; }

    // Check if an update occurred for this current market tick
    bool updates_top() const;

    /**
     * Check if this message is for an updated top quote
     * Note: This is mutually exclusive with IsConsolidated()
     */
    bool IsTopQuote() const;

    /**
     * Check if this message for a consolidated quote
     * Note: This is mutually exclusive with IsTopQuote()
     */
    bool IsConsolidated() const;

    /**
     * Check if this message is for a direct quote
     * Note: This is mutually exclusive with the above
     */
    bool IsDirect() const;

    /**
     * Check if this message is for a depth quote
     * Note: This is mutually exclusive with the above
     */
    bool IsFromDepth() const;

    /**
     * Returns true if data source is only sending quote ticks that affect the NBBO
     *
     * This is true of some Consolidated (L1) datasets. In this mode Strategy Studio copies the NBBO
     * to the top_quote() rather than maintaining a top_quote() that represents the sum of liquidity from
     * all market centers at top prices. This is because with only the NBBO datapoints we can't accurately
     * maintain the state of each market center's quotes.
     */
    bool FeedOnlyHasNBBOQuotes() const;

    MarketCenterID market_center_id() const { return market_center_id_; }

private: /* Event msg hooks */
    /**
     * Handles updating of the instrument before handing instrument to strategy
     */
    virtual bool PreProcess();

private:
    Quote market_center_quote_; // if market center was updated, contents here
    NBBOQuote nbbo_quote_update_; // if NBBO was updated, contents here
    Instrument* instrument_; // instrument in question
    MarketCenterID market_center_id_; // if market center was updated, exchange here
    FeedType quote_feed_type_; // lets us know if quote is consolidated, depth, or top
    QuoteUpdateType quote_update_type_; // type of update that just occurred
    bool top_update_occurred_; // set to true if update just occurred
};

inline std::string QuoteEventMsg::name() const
{
    return "Quote_Instr_" + instrument_->symbol();
}

inline const Quote& QuoteEventMsg::quote() const
{
    return (IsTopQuote() ? instrument_->markets().top_quote() : market_center_quote_);
}

inline bool QuoteEventMsg::IsTopQuote() const
{
    return (quote_feed_type_ == FEED_TYPE_TOP);
}

inline bool QuoteEventMsg::IsConsolidated() const
{
    return (quote_feed_type_ == FEED_TYPE_CONSOLIDATED);
}

inline bool QuoteEventMsg::IsDirect() const
{
    return (quote_feed_type_ == FEED_TYPE_DIRECT);
}

inline bool QuoteEventMsg::IsFromDepth() const
{
    return (quote_feed_type_ == FEED_TYPE_DEPTH);
}

inline bool QuoteEventMsg::FeedOnlyHasNBBOQuotes() const
{
    return (quote_update_type_ == QUOTE_UPDATE_TYPE_NBBO_ONLY);
}

inline bool QuoteEventMsg::updates_top() const
{
    return top_update_occurred_;
}

inline bool QuoteEventMsg::updates_nbbo() const
{
    return (quote_update_type_ == QUOTE_UPDATE_TYPE_NBBO_AND_MARKET_CENTER) || (quote_update_type_ == QUOTE_UPDATE_TYPE_NBBO_ONLY);
}

} // namespace StrategyStudio
} // namespace RCM

#endif