/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/IOrderBookBase.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_IORDERBOOK_BASE_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_IORDERBOOK_BASE_H_

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

class IPriceLevelBase;

/**
 * IOrderBook is an interface to be implemented by market data order books
 */
class IOrderBookBase {
public:
    IOrderBookBase() {}
    virtual ~IOrderBookBase() {}

public:
    /**
     * Returns whether the order book is currently in an initialization state
     */
    virtual bool is_initializing() const = 0;

    /**
     * Retrieve the best bid price level
     * @returns order book price level pointer, or null if not found
     */
    virtual const IPriceLevelBase* BestBidLevel() const = 0;

    /**
     * Retrieve the best ask price level
     * @returns order book price level pointer, or null if not found
     */
    virtual const IPriceLevelBase* BestAskLevel() const = 0;

    /**
     * Retrieve the worst bid price level
     * @returns order book price level  pointer, or null if not found
     */
    virtual const IPriceLevelBase* WorstBidLevel() const = 0;

    /**
     * Retrieve the worst ask price level
     * @returns order book price level  pointer, or null if not found
     */
    virtual const IPriceLevelBase* WorstAskLevel() const = 0;

    /**
     * Get bid price level at a given price
     * @returns NULL if price level doesn't exist
     */
    virtual const IPriceLevelBase* FindBidLevel(double price) const = 0;

    /**
     * Get ask price level at a given price
     * @returns NULL if price level doesn't exist
     */
    virtual const IPriceLevelBase* FindAskLevel(double price) const = 0;

    /**
     * Get bid price level at a given level
     * @returns NULL if price level doesn't exist
     */
    virtual const IPriceLevelBase* BidPriceLevelAtLevel(int level) const = 0;

    /**
     * Get ask price level at a given level
     * @returns NULL if price level doesn't exist
     */
    virtual const IPriceLevelBase* AskPriceLevelAtLevel(int level) const = 0;

    /**
     * Get total bid liquidity in shares/contracts accross all bid levels of the book
     */
    virtual int TotalBidSize() const = 0;

    /**
     * Get total ask liquidity in shares/contracts accross all asl levels of the book
     */
    virtual int TotalAskSize() const = 0;

    /**
     * Return the amount of bid liquidity at a given price
     */
    virtual int BidSizeAtPrice(double price) const = 0;

    /**
     * Return the amount of ask liquidity at a given price
     */
    virtual int AskSizeAtPrice(double price) const = 0;

    /**
     * Return the amount of bid liquidity at a given price level
     * Level is zero-indexed (first level starts at zero)
     */
    virtual int BidSizeAtLevel(int level) const = 0;

    /**
     * Return the amount of ask liquidity at a given price level
     * Level is zero-indexed (first level starts at zero)
     */
    virtual int AskSizeAtLevel(int level) const = 0;

    /**
     * Check if the given buy price level exists
     */
    virtual bool CheckBuyPriceLevelExists(double price) const = 0;

    /**
     * Check if the given sell price level exists
     */
    virtual bool CheckSellPriceLevelExists(double price) const = 0;

    /**
     * Get number of bid levels
     */
    virtual int NumBidLevels() const = 0;

    /**
     * Get number of ask levels
     */
    virtual int NumAskLevels() const = 0;

    /**
     * Get number of bid orders
     */
    virtual int NumBidOrders() const = 0;

    /**
     * Get number of ask orders
     */
    virtual int NumAskOrders() const = 0;

    /**
     * Get number of total orders
     */
    virtual int NumTotalOrders() const = 0;

    /**
     * Return the amount of number of bid orders at a given price
     */
    virtual int NumBidOrdersAtPrice(double price) const = 0;

    /**
     * Return the amount of number of ask orders at a given price
     */
    virtual int NumAskOrdersAtPrice(double price) const = 0;

    /**
     * Return the amount of number of bid orders at a given price level
     * Level is zero-indexed (first level starts at zero)
     */
    virtual int NumBidOrdersAtLevel(int level) const = 0;

    /**
     * Return the amount of number of ask orders at a given price level
     * Level is zero-indexed (first level starts at zero)
     */
    virtual int NumAskOrdersAtLevel(int level) const = 0;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif