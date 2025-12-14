/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IPortfolioTracker.h
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

#ifndef _STRATEGY_STUDIO_LIB_IPNLTRACKER_H_
#define _STRATEGY_STUDIO_LIB_IPNLTRACKER_H_

#include "IProfitUpdate.h"
#include "DailyPnlSnapshot.h"
#include "IntradayPnlSnapshot.h"
#include "IPositionRecord.h"
#include <MarketModels/Instrument.h>
#include <deque>
#include <set>
#include <boost/unordered_map.hpp>
#include <list>

namespace RCM {
namespace StrategyStudio {

namespace Utilities {
class QuickLogger;
} // namespace Utilities

struct FillInfo;

/**
 * Base interface for tracking profit and loss
 * This class exists to allow customization of what kinds of
 * tracking the engine can do
 */
class IPortfolioTracker : public IProfitUpdate {
public:
    typedef boost::unordered_map<const Instrument*, IPositionRecord*, boost::hash<const Instrument*> > IPositionRecords;
    typedef IPositionRecords::iterator IPositionRecordsIter;
    typedef IPositionRecords::const_iterator IPositionRecordsConstIter;

    typedef std::set<DailyPnlSnapshot> DailyPnlSnapshots;
    typedef DailyPnlSnapshots::iterator DailyPnlSnapshotsIter;
    typedef DailyPnlSnapshots::const_iterator DailyPnlSnapshotsConstIter;

    typedef std::list<IntradayPnlSnapshot*> IntradayPnlSnapshots;
    typedef IntradayPnlSnapshots::iterator IntradayPnlSnapshotsIter;
    typedef IntradayPnlSnapshots::const_iterator IntradayPnlSnapshotsConstIter;

public:
    IPortfolioTracker() {}

    virtual ~IPortfolioTracker() {}

    virtual IPositionRecordsConstIter position_records_begin() const = 0;
    virtual IPositionRecordsConstIter position_records_end() const = 0;
    virtual DailyPnlSnapshotsConstIter daily_snapshot_begin() const = 0;
    virtual DailyPnlSnapshotsConstIter daily_snapshot_end() const = 0;

    /**
     * Get portfolio total pnl
     */
    virtual double total_pnl() const = 0;

    /**
     * Retrieve total pnl for an instrument
     * @returns num, or 0 if instrument not present
     */
    virtual double total_pnl(const MarketModels::Instrument* instrument) const = 0;

    /**
     * Get portfolio unrealized pnl
     */
    virtual double unrealized_pnl() const = 0;

    /**
     * Retrieve unrealized pnl for an instrument
     * @returns num, or 0 if instrument not present
     */
    virtual double unrealized_pnl(const MarketModels::Instrument* instrument) const = 0;

    /**
     * Get portfolio realized pnl
     */
    virtual double realized_pnl() const = 0;

    /**
     * Retrieve realized pnl for an instrument
     * @returns num, or 0 if instrument not present
     */
    virtual double realized_pnl(const MarketModels::Instrument* instrument) const = 0;

    /**
     * Get portfolio day pnl
     */
    virtual double day_pnl() const = 0;

    /**
     * Get the day's opening cummulative pnl
     */
    virtual double opening_cumulative_pnl() const = 0;

    /**
     * Get the cummulative execution costs for the portfolio
     */
    virtual double execution_costs() const = 0;

    /**
     * Get the initial equity
     */
    virtual double initial_equity() const = 0;

    /**
     * Get initial cash
     */
    virtual double initial_cash() const = 0;

    /**
     * Get the account equity
     */
    virtual double account_equity() const = 0;

    /**
     * Get current cash balance
     */
    virtual double cash_balance() const = 0;

    /**
     * Get borrow balance
     */
    virtual double borrow_balance() const = 0;

    /**
     * Get rebate balance
     */
    virtual double rebate_balance() const = 0;

    /**
     * Returns the portfolio's net notional exposure
     */
    virtual double notional() const = 0;

    /**
     * Retrieve notional for an instrument
     * @returns num, or 0 if instrument not present
     */
    virtual double notional(const Instrument* instrument) const = 0;

    /**
     * Returns the portfolio's long notional exposure
     */
    virtual double long_notional() const = 0;

    /**
     * Retrieve LongNotional for an instrument
     * @returns num, or 0 if instrument not present
     */
    virtual double long_notional(const Instrument* instrument) const = 0;

    /**
     * Returns the portfolio's short notional exposure as a negative number
     */
    virtual double short_notional() const = 0;

    /**
     * Retrieve ShortNotional for an instrument
     * Note that ShortNotional return value is negative
     * @returns num, or 0 if instrument not present
     */
    virtual double short_notional(const Instrument* instrument) const = 0;

    /**
     * Retrieve leverage
     * Some types of pnl trackers don't use this
     */
    virtual double leverage() const = 0;

    /**
     * Calculate the numerator portion of leverage
     * Needed to handle multi-asset cases
     */
    virtual double leverage_numerator() const = 0;

    /**
     * Retrieve Position for an instrument
     * @returns num, or 0 if instrument not present
     */
    virtual int position(const Instrument* instrument) const = 0;

    /**
     * Get record via instrument
     * Note: Not all derived will implement this
     * in such cases, those just return nullptr
     */
    virtual const IPositionRecord* GetRecord(const MarketModels::Instrument* instrument) const = 0;

    /**
     * Retrieve number of records within the pnl tracker
     */
    virtual size_t num_records() const = 0;

    /**
     * Retrieve number of daily snapshots
     */
    virtual size_t num_daily_snapshots() const = 0;

    /**
     * Check if instrument is contained in portfolio
     * Note: Not all derived instruments will implement this
     * in such cases, it should return false
     */
    virtual bool Contains(const MarketModels::Instrument* instrument) const = 0;

    /**
     * Retrieve the pnl information for an instrument if it is present.
     *
     * Output parameters retrieve the total pnl and the realized pnl, the unrealized pnl can be found as their difference
     *
     * @returns true if instrument present and prices set, false otherwise
     */
    virtual bool GetPnLInfo(const Instrument* instrument, double* outTotalPnl, double* outRealizedPnl) const = 0;

    /**
     * Retrieve the pnl information for an instrument if it is present (including notional)
     *
     * Output parameters retrieve the total pnl and the realized pnl, the unrealized pnl can be found as their difference
     *
     * @returns true if instrument present and prices set, false otherwise
     */
    virtual bool GetPnLInfo(const Instrument* instrument, double* outTotalPnl, double* outRealizedPnl, double* outNotional) const = 0;

    /**
     * Retrieve the portfolio weight of the given instruments
     * @returns port weight, 0 if instrument not present
     */
    virtual double PortfolioWeight(const Instrument* instrument) const = 0;

    /**
     * Logs the total p&l for each instrument in the portfolio to logger
     */
    virtual void ShowResults(Utilities::QuickLogger& logger) const = 0;

    virtual double proceeds() const = 0;

    /**
     * Retrieve currency exposure (in terms of the currency) for the given currency
     */
    virtual double currency_exposure(MarketModels::CurrencyType currency) const = 0;
};

} // namespace StrategyStudio
} // namespace RCM

#endif
