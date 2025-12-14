/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/IPriceLevel.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_IPRICE_LEVEL_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_IPRICE_LEVEL_H_

#include "IPriceLevelBase.h"

#include <set>
#include <list>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

class IBookOrder;

/**
 * IPriceLevel is an interface for accessing information about the liquidity present at a particular price level at at a single market center from
 * a Depth-Of-Book market data source
 */
class IPriceLevel : public IPriceLevelBase {
public:
    IPriceLevel() {}
    virtual ~IPriceLevel() {}

public: /* Overrides of IPriceLevelBase interface */
    /**
     * Return the number of orders that make up this price level
     *
     * Note depth-by-price-level feeds may deliver an order count without actually delivering individual orders,
     * so this count will not always match the number of orders iterable from front() to back()
     */
    virtual int num_orders() const = 0;

    /**
     * Gets the next best price level, iterating away from the top of book
     *
     * Returns null if this is the last price level in the book
     */
    virtual const IPriceLevel* next() const = 0;

    /**
     * Gets the previous price level, iterating towards the top of book
     *
     * Returns null if this is the best price level in the book
     */
    virtual const IPriceLevel* previous() const = 0;

public: /* Additions to interface of IPriceLevelBase */
    /**
     * Gets the portion of the price level's size attributable to direct liquidity
     */
    virtual int direct_size() const = 0;

    /**
     * Gets the portion of the price level's size attributable to implied liquidity
     */
    virtual int implied_size() const = 0;

    /**
     * Gets the order at the front of the price level, or null if there are no orders at the price level
     */
    virtual const IBookOrder* front() const = 0;

    /**
     * Gets the order at the back of the price level, or null if there are no orders at the price level
     */
    virtual const IBookOrder* back() const = 0;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif
