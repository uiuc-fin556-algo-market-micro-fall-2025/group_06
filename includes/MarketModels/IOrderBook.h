/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/IOrderBook.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_IORDERBOOK_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_IORDERBOOK_H_

#include "DepthUpdate.h"
#include "IOrderBookBase.h"
#include "IBookOrder.h"
#include "IPriceLevel.h"
#include "Utilities/utils.h"

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * IOrderBook is an interface for accessing the order book information at an individual market center
 */
class IOrderBook : public IOrderBookBase {
public:
    IOrderBook() {}
    virtual ~IOrderBook() {}

public:
    /**
     * Retrieve the best bid price level
     * @returns order book price level  pointer, or null if not found
     */
    virtual const IPriceLevel* BestBidLevel() const = 0;

    /**
     * Retrieve the best ask price level
     * @returns order book price level  pointer, or null if not found
     */
    virtual const IPriceLevel* BestAskLevel() const = 0;

    /**
     * Retrieve the worst bid price level
     * @returns order book price level  pointer, or null if not found
     */
    virtual const IPriceLevel* WorstBidLevel() const = 0;

    /**
     * Retrieve the worst ask price level
     * @returns order book price level  pointer, or null if not found
     */
    virtual const IPriceLevel* WorstAskLevel() const = 0;

    /**
     * Get bid price level at a given price
     * @returns NULL if price level doesn't exist
     */
    virtual const IPriceLevel* FindBidLevel(double price) const = 0;

    /**
     * Get ask price level at a given price
     * @returns NULL if price level doesn't exist
     */
    virtual const IPriceLevel* FindAskLevel(double price) const = 0;

    /**
     * Get bid price level at a given level
     * @returns NULL if price level doesn't exist
     */
    virtual const IPriceLevel* BidPriceLevelAtLevel(int level) const = 0;

    /**
     * Get ask price level at a given level
     * @returns NULL if price level doesn't exist
     */
    virtual const IPriceLevel* AskPriceLevelAtLevel(int level) const = 0;

    /**
     * Check if order book contains the order id
     * @returns true if order book has given order id, false otherwise
     */
    virtual bool ContainsOrderID(DepthOrderID orderID) const = 0;

    /**
     * Find order by order ID
     * @returns pointer to order id if order present, NULL if not present
     */
    virtual const IBookOrder* FindOrderByID(DepthOrderID orderID) const = 0;

    /**
     * Retrieve the market center for the order book
     */
    virtual MarketCenterID market_center() const = 0;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif