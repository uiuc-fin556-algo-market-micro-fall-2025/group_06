/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/TradeStats.h
*     Last Update: 2014/04/09 13:55:14
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2014.
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_TRADE_STATS_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_TRADE_STATS_H_

#include "MarketCenterTypes.h"
#include <Utilities/TimeType.h>
#include <string>
#include <vector>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

using RCM::StrategyStudio::Utilities::DateType;

/**
 * Contains daily high/low/open/close data for an Instrument when available
 *
 * The server's abiltiy to initialize these values correctly on late day connections may
 * vary for different data adapters.
 */
class TradeStats {
public:
    TradeStats();

    /**
     * Highest trade price of the day
     */
    double high() const { return high_; }

    /**
     * Lowest trade price of the day
     */
    double low() const { return low_; }

    /**
     * Today's official opening print
     *
     * Will only be set based upon initialization or when the trade conditions of a
     * trade message affirmatively identify the opening print. May not be available from
     * all feeds. Will be zero pre-open or if not available.
     */
    double open() const { return open_; }

    /**
     * Official closing print
     *
     * Will only be set based upon initialization or when the trade conditions of a
     * trade message affirmatively identify the closing print. May not be available from
     * all feeds
     */
    double prior_close() const { return prior_close_; }

    /**
     * Official closing print date
     *
     * Contains the date associated with the prior_close, or NULL_DATE_TYPE if not set.
     */
    DateType prior_close_date() const { return prior_close_date_; }

    /**
     * Day's volume for an instrument
     *
     * When the server subscribes to multiple overlapping feed types, after any initialization
     * this will increment from the trade ticks on the market center's preferred_trade_feed.
     */
    StrategyStudioUInt32 volume() const { return volume_; }

    void set_high(double high) { high_ = high; }
    void set_low(double low) { low_ = low; }
    void set_open(double open) { open_ = open; }
    void set_prior_close(double priorClose) { prior_close_ =  priorClose; }
    void set_prior_close_date(DateType priorCloseDate) { prior_close_date_ = priorCloseDate; }
    void init_volume(StrategyStudioUInt32 volume, MarketCenterID market);
    void increment_volume(int size, MarketCenterID market);
    void ResetForNewDay();

private:
    std::vector<StrategyStudioUInt32> volume_init_tracker_;
    double high_;
    double low_;
    double open_;
    double prior_close_;
    StrategyStudioUInt32 volume_;
    DateType prior_close_date_;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif