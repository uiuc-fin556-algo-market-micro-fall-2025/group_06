/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/DailyPnlSnapshot.h
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

#ifndef _STRATEGY_STUDIO_LIB_DAILY_SNAPSHOT_H_
#define _STRATEGY_STUDIO_LIB_DAILY_SNAPSHOT_H_

#include "DataTypes.h"

#include <fstream>

namespace RCM {
namespace StrategyStudio {

class IPortfolioTracker;

namespace Utilities {
class SerializedStream;
}

/**
 * Contains pnl snapshot
 */
struct DailyPnlSnapshot {
public:
    DailyPnlSnapshot();

    explicit DailyPnlSnapshot(const boost::gregorian::date& date);

    DailyPnlSnapshot(const boost::gregorian::date& date,
                     double acctEquity,
                     double totalPnl,
                     double dailyPnl,
                     double longNotional,
                     double shortNotional,
                     double cashBalance,
                     double borrowBalance,
                     double rebateBalance);

    /**
     * Check if this daily pnl snapshot has the same date as another one
     */
    bool EqualsDate(const DailyPnlSnapshot& rhs) const;

    /**
     * Save to stream
     */
    void SaveToBinaryStream(Utilities::SerializedStream& out) const;

    /**
     * Load from stream
     */
    void LoadFromBinaryStream(Utilities::SerializedStream& in);

    /**
     * DailyPnlSnapshot is ordered by date
     */
    bool operator<(const DailyPnlSnapshot& rhs) const;

    /**
     * Check if all members of this struct are equivalent to another DailyPnlSnapshot
     */
    bool operator==(const DailyPnlSnapshot& rhs) const;

    /**
     * Create DailyPnlSnapshot using data from IPortfolioTracker
     */
    static DailyPnlSnapshot Create(boost::gregorian::date date, const IPortfolioTracker& portTracker);

public:
    boost::gregorian::date date;
    double account_equity;
    double total_pnl;
    double daily_pnl;
    double long_notional;
    double short_notional;
    double cash_balance;
    double borrow_balance;
    double rebate_balance;
};

inline std::ostream& operator<<(std::ostream& out, const DailyPnlSnapshot& daily)
{
    out << "Date: " << to_simple_string(daily.date) << " ";
    out << "Account equity: " << daily.account_equity << " ";
    out << "TotalPnl: " << daily.total_pnl << " ";
    out << "DailyPnl: " << daily.daily_pnl << " ";
    out << "Long Notional: " << daily.long_notional << " ";
    out << "Short Notional: " << daily.short_notional << " ";
    out << "Cash Balance: " << daily.cash_balance << " ";
    out << "Borrow Balance: " << daily.borrow_balance << " ";
    out << "Rebate Balance: " << daily.rebate_balance << " ";

    return out;
}

} // namespace StrategyStudio
} // namespace RCM

#endif
