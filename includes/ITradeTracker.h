/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/ITradeTracker.h
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

#ifndef _STRATEGY_STUDIO_LIB_I_TRADE_TRACKER_H_
#define _STRATEGY_STUDIO_LIB_I_TRADE_TRACKER_H_

#include "DataTypes.h"
#include "TradeRecord.h"
#include <boost/unordered_map.hpp>

namespace RCM {
namespace StrategyStudio {

/**
 * ITradeTracker tracks the performance of each trading exposure a strategy initiates.
 *
 * The class calculates the net realized pnl from the time a position is opened to the time it is closed.
 * By default a new trade ID is generated every time you increase your position size through a new order,
 * whereas partial fills within an order count towards the same trade ID. When you place an order, you can set
 * legging_in to true to group multiple back-to-back buy or sell orders into a single trade ID
 *
 */
class ITradeTracker {
public:
    typedef boost::unordered_map<const Instrument*, TradeRecord, boost::hash<const Instrument*> > TradeMap;
    typedef TradeMap::iterator TradeMapIter;
    typedef TradeMap::const_iterator TradeMapConstIter;

public:
    ITradeTracker() {}

    virtual ~ITradeTracker() {}

    virtual TradeMapConstIter trade_map_begin() const = 0;
    virtual TradeMapConstIter trade_map_end() const = 0;

    virtual const TradeRecord& GetRecord(const Instrument* key) const = 0;

    /**
     * Check number of open trades for all instruments
     */
    virtual size_t num_open_trades() const = 0;

    /**
     * Check number of open trades for specified instrument
     */
    virtual size_t num_open_trades(const Instrument* key) const = 0;

    /**
     * Check number of closed trades for all instruments
     */
    virtual size_t num_closed_trades() const = 0;

    /**
     * Check number of closed trades for specified instrument
     */
    virtual size_t num_closed_trades(const Instrument* key) const = 0;

    /**
     * Returns the ID of the newest trade
     */
    virtual TradeID curr_trade_id() const = 0;

    virtual bool save_history_flag() const = 0;
};

} // namespace StrategyStudio
} // namespace RCM

#endif
