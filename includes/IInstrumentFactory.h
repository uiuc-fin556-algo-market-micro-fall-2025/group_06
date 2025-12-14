/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IInstrumentFactory.h
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

#ifndef _STRATEGY_STUDIO_LIB_I_INSTRUMENT_FACTORY_H_
#define _STRATEGY_STUDIO_LIB_I_INSTRUMENT_FACTORY_H_

#include "DataTypes.h"
#include "SymbologyTypes.h"

#include <boost/unordered_set.hpp>

namespace RCM {
namespace StrategyStudio {

class InstrumentRefUpdateEventMsg;
class PreferredFeedTypes;

namespace MarketModels {
class CurrencyRatesMgr;
typedef class ConversionRate CurrencyRate;
} // namespace MarketModels

/**
 * Interface for  managing the storage and retrieval of instruments
 */
class IInstrumentFactory {
public:
    typedef boost::unordered_map<SymbolTag, Instrument*, boost::hash<SymbolTag> > InstrumentMap;
    typedef InstrumentMap::iterator InstrumentMapIter;
    typedef InstrumentMap::const_iterator InstrumentMapConstIter;

    /**
     * Multimap for linking underliers to child instruments infos
     */
    typedef boost::unordered_multimap<SymbolTag, Instrument*> SymbolChildMap;
    typedef SymbolChildMap::iterator SymbolChildMapIter;
    typedef SymbolChildMap::const_iterator SymbolChildMapConstIter;

    typedef boost::unordered_set<MarketModels::CurrencyRate*> CurrencyRateSet;

public:
    IInstrumentFactory() {}

    virtual ~IInstrumentFactory() {}

    virtual InstrumentMapIter begin() = 0;
    virtual InstrumentMapConstIter begin() const = 0;

    virtual InstrumentMapIter end() = 0;
    virtual InstrumentMapConstIter end() const = 0;

    virtual PreferredFeedTypes& preferred_feeds() = 0;
    virtual const PreferredFeedTypes& preferred_feeds() const = 0;

    virtual MarketModels::MarketCenterInfoFactory* market_center_info_factory() = 0;
    virtual const MarketModels::MarketCenterInfoFactory* market_center_info_factory() const = 0;

    /**
     * Builds a new instrument
     */
    virtual Instrument* Build(const SymbolTag& key,
                      SymbologyType symbology)=0;

    /**
     * Removes contained instruments
     */
    virtual void Clear() = 0;

    /**
     * Check if key is already present in the factory
     */
    virtual bool Contains(const SymbolTag& key) const = 0;

    /**
     * Resets the prices for all contained instruments
     */
    virtual void ResetPrices() = 0;

    /**
     * Retrieve the value associated with a key
     * @returns instrument if present, null otherwise
     */
    virtual Instrument* Retrieve(const SymbolTag& key)=0;
    virtual const Instrument* Retrieve(const SymbolTag& key) const = 0;

    /**
     * Processes a symbol change
     */
    virtual void ProcessSymbolChange(const SymbolTag& oldSymbol, const SymbolTag& newSymbol)=0;

    /**
     * Applies a preferred feed type change to created instruments and local copy
     */
    virtual void ProcessPreferredFeedTypeChange(MarketCenterID mc, FeedType quoteFeedType, FeedType tradeFeedType)=0;

    /**
     * Updates affected instruments' reference data
     */
    virtual void ProcessInstrumentRefUpdate(const InstrumentRefUpdateEventMsg& refUpdate) = 0;

    /**
     * Reset static currency rates for specific date
     */
    virtual CurrencyRateSet ResetStaticRatesForDate(Utilities::DateType date) = 0;

    virtual MarketModels::CurrencyRatesMgr& currency_rates_mgr() = 0;
};

} // namespace StrategyStudio
} // namespace RCM

#endif