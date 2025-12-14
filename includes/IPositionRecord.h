/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IPositionRecord.h
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

#ifndef _STRATEGY_STUDIO_LIB_IPOSITION_RECORD_H_
#define _STRATEGY_STUDIO_LIB_IPOSITION_RECORD_H_

#include "DataTypes.h"
#include "FillInfo.h"
#include <deque>
#include <set>
#include <iostream>

namespace RCM {
namespace StrategyStudio {

class IInstrumentFactory;
class IProfitUpdate;

/**
 * IPositionRecord maintains instrument level position data, including
 * pnls, transaction costs, and entry fill records for open positions.
 */
class IPositionRecord {
public:
    typedef std::deque<FillInfo> FillEntries;
    typedef FillEntries::iterator FillEntriesIter;
    typedef FillEntries::const_iterator FillEntriesConstIter;

    virtual size_t num_entries() const = 0;

    virtual FillEntriesConstIter curr_fill_entries_begin() const = 0;
    virtual FillEntriesConstIter curr_fill_entries_end() const = 0;

    virtual size_t num_history() const = 0;

    virtual FillEntriesConstIter hist_fill_entries_begin() const = 0;
    virtual FillEntriesConstIter hist_fill_entries_end() const = 0;

public:
    IPositionRecord() {}

    virtual ~IPositionRecord() = default;
    IPositionRecord(const IPositionRecord&) = default;
    IPositionRecord& operator=(const IPositionRecord&) = default;
    IPositionRecord(IPositionRecord&&) = default;
    IPositionRecord& operator=(IPositionRecord&&) = default;

    /**
     * Retrieve the size of the initial position
     */
    virtual int init_position() const = 0;

    /**
     * Retrieve any positive cashflows received (i.e. dividends)
     */
    virtual double cash_flow() const = 0;

    /**
     * Retrieve the execution costs of the positions
     */
    virtual double execution_costs() const = 0;

    /**
     * Retrieve the last marking price of the positions
     */
    virtual double last_marking_price() const = 0;

    /**
     * Retrieve execution costs-adjusted average fill price
     */
    virtual double average_fill_price() const = 0;

    /**
     * Retrieve average fill price without execution costs
     */
    virtual double average_price() const = 0;

    /**
     * Retrieve notional value of positions
     */
    virtual double notional() const = 0;

    /**
     * Retrieve notional value of long positions
     */
    virtual double long_notional() const = 0;

    /**
     * Retrieve notional value of short positions
     * Note that ShortNotional return value is negative
     */
    virtual double short_notional() const = 0;

    /**
     * Retrieve signed positions (positive for long/negative for short)
     */
    virtual int position() const = 0;

    /**
     * Retrieve signed currency position (positive for long/negative for short)
     */
    virtual double currency_position() const = 0;

    /**
     * Get portfolio realized pnl
     */
    virtual double realized_pnl() const = 0;

    /**
     * Retrieve realized pnl for an instrument
     * @returns num, or 0 if instrument not presentRetrieve notional value of long positions
     */
    virtual double unrealized_pnl() const = 0;

    /**
     * Get the total pnl of the position
     */
    virtual double total_pnl() const = 0;

    /**
     * Get position's day pnl
     */
    virtual double day_pnl() const = 0;

    /**
     * Fetch position at a given time
     */
    virtual int HistoricalPosition(TimeType time) const = 0;

};
} // namespace StrategyStudio
} // namespace RCM

#endif