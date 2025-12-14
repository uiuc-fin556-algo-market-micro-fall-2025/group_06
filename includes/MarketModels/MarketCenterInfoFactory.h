/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/MarketCenterInfoFactory.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_MARKET_CENTER_INFO_FACTORY_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_MARKET_CENTER_INFO_FACTORY_H_

#include "MarketCenterFees.h"
#include <boost/unordered_map.hpp>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

class MarketCenterInfo;

/**
 * MarketCenterInfo factory manages the storage and retrieval of MarketCenterInfo for different market centers
 */
class MarketCenterInfoFactory {
public:
    typedef boost::unordered_map<MarketCenterID, MarketCenterInfo*> MarketCenterInfoMap;
    typedef MarketCenterInfoMap::iterator MarketCenterInfoMapIter;
    typedef MarketCenterInfoMap::const_iterator MarketCenterInfoMapConstIter;

    typedef boost::unordered_map<MarketModels::MarketCenterID, MarketModels::MarketCenterFees, boost::hash<MarketModels::MarketCenterID> > MarketCenterFeesMap;
    typedef MarketCenterFeesMap::iterator MarketCenterFeesMapIter;
    typedef MarketCenterFeesMap::const_iterator MarketCenterFeesMapConstIter;

public:
    MarketCenterInfoFactory();
    explicit MarketCenterInfoFactory(const MarketCenterFees& defaultMarketCenterFees, const MarketCenterFeesMap& marketFees);
    ~MarketCenterInfoFactory();

    MarketCenterInfoMapIter begin() { return market_center_info_map_.begin(); }
    MarketCenterInfoMapConstIter begin() const { return market_center_info_map_.begin(); }

    MarketCenterInfoMapIter end() { return market_center_info_map_.end(); }
    MarketCenterInfoMapConstIter end() const { return market_center_info_map_.end(); }

public:
    MarketCenterFees default_market_center_fees() { return default_fees_; }
    void set_default_market_center_fees(const MarketCenterFees& fees) { default_fees_ = fees; }

    /**
     * Check if key is already present in the factory
     */
    bool Contains(MarketCenterID key) const;

    /**
     * Builds a new MarketCenterInfo
     */
    MarketCenterInfo* Get(MarketCenterID marketCenterID);

private:
    MarketCenterInfoMap market_center_info_map_;
    MarketCenterFees default_fees_;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif 