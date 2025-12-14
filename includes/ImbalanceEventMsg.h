/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/ImbalanceEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_IMBALANCE_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_IMBALANCE_EVENT_MSG_H_

#include "DataTypes.h"
#include "DataTypesUtils.h"
#include "Event.h"
#include "MarketDataBaseEventMsg.h"

namespace RCM {
namespace StrategyStudio {

/**
 * ImbalanceEventMsg is an event message containing information about the imbalance
 * quantities related to an exchange's price setting auctions.
 */
class ImbalanceEventMsg : public MarketDataBaseEventMsg {
public:
    ImbalanceEventMsg(Event* event,
                      Instrument* instrument,
                      MarketCenterID market_center,
                      FeedType feed_type,
                      AuctionType auction_type,
                      int matched_quantity,
                      int imbalance_size,
                      int imbalance_size2,
                      double clearing_price,
                      double clearing_price2,
                      double reference_price,
                      TimeType source_time,
                      TimeType feed_handler_time,
                      TimeType adapter_time);

    ImbalanceEventMsg(Event* event,
                      Instrument* instrument,
                      MarketCenterID market_center,
                      FeedType feed_type,
                      TimeType source_time,
                      TimeType feed_handler_time,
                      TimeType adapter_time);

    std::string name() const;

    /**
     * Returns a const reference to the instrument for which the quote was updated
     */
    const Instrument& instrument() const { return *instrument_; }

    /**
     * Returns which market center's auction the imbalance data pertains to
     */
    MarketCenterID market_center() const { return market_center_; }

    /**
     * Identifies which type of data feed is delivering an alert
     */
    FeedType feed_type() const { return feed_type_; }

    /**
     * Identifies the type of auction associated with the imbalance update
     */
    AuctionType auction_type() const { return auction_type_; }

    /**
     * Quantity of shares that are matched/paired off at the refrence price
     *
     * For BATS Auction feed, this is normalized from min(BuyShares, SellShares)
     */
    int matched_quantity() const { return matched_quantity_; }

    /**
     * Number of shares that are out of balance
     *
     * Normalized so all feeds have a signed value, positive for more buy orders than sell orders
     *
     * For BATS Auction feed, this is normalized from BuyShares - SellShares
     */
    int imbalance_size() const { return imbalance_size_; }

    /**
     * Number of shares that are out of balance based off of just market orders
     *
     * Applicable only on ARCA; a signed value, positive for more buy orders than sell orders
     */
    int imbalance_size2() const { return imbalance_size2_; }

    /**
     * Closest indicative price where imbalance would be zero
     *
     * Contains 'near price' for NASDAQ
     * Contains 'clearing price' for NYSE Open imbalance and 'continuous book clearing price' for closing imbalance
     * Contains 'indicative match price' for ARCA Book Multicast and XDP 2.1, was for a time called 'reference price' in ARCA Book XDP 1 spec
     * Contains 'indicative price' for BATS Auction feed
     * Contains 'indicative clearing price' for IEX
     */
    double clearing_price() const { return clearing_price_; }

    /**
     * Provides additional indicative measure of clearing price where applicable
     *
     * Contains 'far price' for NASDAQ
     * Contains 'closing only clearing price' for NYSE
     * Contains 'auction only price' for BATS Auction feed
     * Contains 'auction book clearing price' for IEX
     */
    double clearing_price2() const { return clearing_price2_; }

    /**
     * Reference price corresponding to the imbalance quantities
     *
     * Not provided in Arca's imbalance feed prior to XDP 2.1
     */
    double reference_price() const { return reference_price_; }

    /**
     * Gets the market data source's timestamp for the market data event (eg the exchange feed's timestamp)
     */
    TimeType source_time() const { return source_time_; }

    /**
     * Gets the time the feedhandler processed the market data event, if applicable and available
     */
    TimeType feed_handler_time() const { return feed_handler_time_; }

    /**
     * Gets the time when the Strategy Server's data adapter received the market data event
     */
    TimeType adapter_time() const { return m_adapter_time; }

    void set_auction_type(AuctionType auction_type) { auction_type_ = auction_type; }
    void set_matched_quantity(int matched_quantity) { matched_quantity_ = matched_quantity; }
    void set_imbalance_size(int imbalance_size) { imbalance_size_ = imbalance_size; }
    void set_imbalance_size2(int imbalance_size2) { imbalance_size2_ = imbalance_size2; }
    void set_clearing_price(double clearing_price) { clearing_price_ = clearing_price; }
    void set_clearing_price2(double clearing_price2) { clearing_price2_ = clearing_price2; }
    void set_reference_price(double reference_price) { reference_price_ = reference_price; }

private:
    Instrument* instrument_;

    double clearing_price_;
    double clearing_price2_;
    double reference_price_;

    TimeType source_time_;
    TimeType feed_handler_time_;
    TimeType m_adapter_time;

    int matched_quantity_;
    int imbalance_size_;
    int imbalance_size2_;

    AuctionType auction_type_;
    FeedType feed_type_;
    MarketCenterID market_center_;
};

inline std::string ImbalanceEventMsg::name() const
{
    return "IMBALANCE_EVENT_" + instrument_->symbol();
}

} // namespace StrategyStudio
} // namespace RCM

#endif