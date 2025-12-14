/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/IAggrPriceLevel.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_IAGGR_PRICE_LEVEL_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_IAGGR_PRICE_LEVEL_H_

#include "IPriceLevel.h"
#include <boost/container/flat_map.hpp>

#include <map>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * IAggrPriceLevel is an interface for accessing information about the liquidity present at a particular price level at at a single market center from
 * a Depth-Of-Book market data source
 */
class IAggrPriceLevel : public IPriceLevelBase {
public:
    typedef boost::container::flat_map<MarketCenterID, const IBookEntry*> ParticipatingMarkets;
    typedef ParticipatingMarkets::const_iterator ParticipatingMarketsConstIter;

public:
    IAggrPriceLevel() {}
    virtual ~IAggrPriceLevel() {}

public: /* Overrides of IPriceLevelBase interface */
    /**
     * Gets the next best price level, iterating away from the top of book
     *
     * Returns null if this is the last price level in the book
     */
    virtual const IAggrPriceLevel* next() const = 0;

    /**
     * Gets the previous price level, iterating towards the top of book
     *
     * Returns null if this is the best price level in the book
     */
    virtual const IAggrPriceLevel* previous() const = 0;

public: /* Additions to interface of IPriceLevel */
    /**
     * Retrieve set of participating markets
     * Optionally can return NULL if participating markets not supported by given price level
     */
    virtual const ParticipatingMarkets& participating_markets() const = 0;

    /**
     * Check if market center is participating
     * @returns true if market center present
     */
    virtual bool IsMarketCenterParticipating(MarketCenterID id) const = 0;

    /**
     * Returns number of participating market centers
     */
    virtual int num_markets() const = 0;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif