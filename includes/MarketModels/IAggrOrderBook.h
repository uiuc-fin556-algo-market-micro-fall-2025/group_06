/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/IAggrOrderBook.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_I_AGGR_ORDERBOOK_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_I_AGGR_ORDERBOOK_H_

#include "IOrderBookBase.h"
#include "IAggrPriceLevel.h"

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * IAggrOrderBook is an interface representing an order book aggregated from the Depth-Of-Book feeds of multiple market centers
 */
class IAggrOrderBook : public IOrderBookBase {
public:
    IAggrOrderBook() {}
    virtual ~IAggrOrderBook() {}

public: /* Additions to IOrderBook interface */
    /**
     * Retrieve the best bid price level
     * @returns order book price level pointer, or null if not found
     */
    virtual const IAggrPriceLevel* BestBidLevel() const = 0;

    /**
     * Retrieve the best ask price level
     * @returns order book price level pointer, or null if not found
     */
    virtual const IAggrPriceLevel* BestAskLevel() const = 0;

    /**
     * Retrieve the worst bid price level
     * @returns order book price level  pointer, or null if not found
     */
    virtual const IAggrPriceLevel* WorstBidLevel() const = 0;

    /**
     * Retrieve the worst ask price level
     * @returns order book price level  pointer, or null if not found
     */
    virtual const IAggrPriceLevel* WorstAskLevel() const = 0;

    /**
     * Get bid price level at a given price
     * @returns NULL if price level doesn't exist
     */
    virtual const IAggrPriceLevel* FindBidLevel(double price) const = 0;

    /**
     * Get ask price level at a given price
     * @returns NULL if price level doesn't exist
     */
    virtual const IAggrPriceLevel* FindAskLevel(double price) const = 0;

    /**
     * Get bid price level at a given level
     * @returns NULL if price level doesn't exist
     */
    virtual const IAggrPriceLevel* BidPriceLevelAtLevel(int level) const = 0;

    /**
     * Get ask price level at a given level
     * @returns NULL if price level doesn't exist
     */
    virtual const IAggrPriceLevel* AskPriceLevelAtLevel(int level) const = 0;

    /**
     * Return the amount of number of market centers participating at bid at a given price level
     */
    virtual int NumBidMarketCentersAtPrice(double price) const = 0;

    /**
     * Return the amount of number of market centers participating at ask at a given price level
     */
    virtual int NumAskMarketCentersAtPrice(double price) const = 0;

    /**
     * Return the amount of number of market centers participating at bid at a given price level
     * Level is zero-indexed (first level starts at zero)
     */
    virtual int NumBidMarketCentersAtLevel(int level) const = 0;

    /**
     * Return the amount of number of market centers participating at ask at a given price level
     * Level is zero-indexed (first level starts at zero)
     */
    virtual int NumAskMarketCentersAtLevel(int level) const = 0;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif
