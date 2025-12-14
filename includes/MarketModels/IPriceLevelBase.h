/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/IPriceLevelBase.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_IPRICE_LEVEL_BASE_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_IPRICE_LEVEL_BASE_H_

#include "DepthDataTypes.h"
#include "IBookEntry.h"

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * IPriceLevelBase is is an interface for accessing summary information about the price level of an order book
 */
class IPriceLevelBase : public IBookEntry {
public:
    IPriceLevelBase() {}
    virtual ~IPriceLevelBase() {}

public:
    /**
     * Return the number of orders that make up this price level
     */
    virtual int num_orders() const = 0;

    /**
     * Gets the next best price level, iterating away from the top of book
     *
     * Returns null if this is the last price level in the book
     */
    virtual const IPriceLevelBase* next() const = 0;

    /**
     * Gets the previous price level, iterating towards the top of book
     *
     * Returns null if this is the best price level in the book
     */
    virtual const IPriceLevelBase* previous() const = 0;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif