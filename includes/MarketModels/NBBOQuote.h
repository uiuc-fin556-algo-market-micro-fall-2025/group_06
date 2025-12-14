/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/NBBOQuote.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_NBBO_QUOTE_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_NBBO_QUOTE_H_

#include "Quote.h"
#include "MarketCenterTypes.h"

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * Specialization of the Quote class for NBBO data, where the bid exchange and ask exchange may be different
 *
 * NBBO is the National Best Bid/Best Offer as calculated and disseminated by the SIP (for example, CTA, UTP, OPRA)
 */
class NBBOQuote : public Quote {
public:
    NBBOQuote();
    NBBOQuote(const QuoteSide& bidEntry, MarketCenterID bidExchange, const QuoteSide& askEntry, MarketCenterID askExchange);
    NBBOQuote(double bidPrice, int bidSize, MarketCenterID bidExchange, double askPrice, int askSize, MarketCenterID askExchange);
    NBBOQuote(double bidPrice, int bidSize, MarketCenterID bidExchange, double askPrice, int askSize, MarketCenterID askExchange, QuoteCondition cond);

    NBBOQuote(double bidPrice,
              int bidSize,
              MarketCenterID bidExchange,
              double askPrice,
              int askSize,
              MarketCenterID askExchange,
              TimeType bidSourceTime,
              TimeType bidFeedTime,
              TimeType askSourceTime,
              TimeType askFeedTime,
              TimeType adapterTime);

    NBBOQuote(double bidPrice,
              int bidSize,
              MarketCenterID bidExchange,
              double askPrice,
              int askSize,
              MarketCenterID askExchange,
              QuoteCondition cond,
              TimeType bidSourceTime,
              TimeType bidFeedTime,
              TimeType askSourceTime,
              TimeType askFeedTime,
              TimeType adapterTime);

    MarketCenterID bid_exchange() const { return bid_exchange_; }
    void set_bid_exchange(MarketCenterID exch) { bid_exchange_ = exch; }

    MarketCenterID ask_exchange() const { return ask_exchange_; }
    void set_ask_exchange(MarketCenterID exch) { ask_exchange_ = exch; }

    /**
     * Update the relevant fields for the NBBO quote
     */
    void UpdateNBBOQuote(const NBBOQuote& quote);

    /**
     * Update the relevant fields for the quote
     */
    void UpdateNBBOQuote(double bidPrice,
                         int bidSize,
                         double askPrice,
                         int askSize,
                         MarketCenterID bidExchange,
                         MarketCenterID askExchange,
                         TimeType bidSourceTime,
                         TimeType bidFeedHandlerTime,
                         TimeType askSourceTime,
                         TimeType askFeedHandlerTime,
                         TimeType adapterTime);

private:
    MarketCenterID bid_exchange_;
    MarketCenterID ask_exchange_;
};

inline std::ostream& operator<<(std::ostream& out, const NBBOQuote& quote)
{
    out << "BID: " << quote.bid_side() << " EXCH: " << MarketCenterToString(quote.bid_exchange())
        << " ASK: " << quote.ask_side() << " EXCH: " << MarketCenterToString(quote.ask_exchange())
        << " cond: " << QuoteConditionUtil::QuoteConditionToString(quote.condition());

    return out;
}

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif