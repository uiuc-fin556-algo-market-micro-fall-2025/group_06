/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IntradayPnlSnapshot.h
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

#ifndef _STRATEGY_STUDIO_LIB_INTRADAY_PNL_SNAPSHOT_H_
#define _STRATEGY_STUDIO_LIB_INTRADAY_PNL_SNAPSHOT_H_

#include "DataTypes.h"
#include "FillInfo.h"
#include <boost/unordered_map.hpp>
#include <fstream>
#include <deque>

namespace RCM {
namespace StrategyStudio {

namespace Utilities {
class SerializedStream;
}

class IInstrumentFactory;

/**
 * Contains pnl info for a given instrument
 */
 struct PnlPoint {
 public:
     PnlPoint();

     PnlPoint(double totalPnl, double rpl, double notional, double bid, double ask);

    /**
     * Save to stream
     */
    void SaveToBinaryStream(Utilities::SerializedStream& out) const;

    /**
     * Load from stream
     */
    void LoadFromBinaryStream(Utilities::SerializedStream& in);

    bool operator==(const PnlPoint& rhs) const;

 public:
    double total_pnl;
    double realized_pnl;
    double notional;
    double bid;
    double ask;
 };

/**
 * Contains pnl snapshot
 */
struct IntradayPnlSnapshot {
public:
    typedef std::deque<FillInfo> FillEntries;
    typedef FillEntries::iterator FillEntriesIter;
    typedef FillEntries::const_iterator FillEntriesConstIter;

    typedef boost::unordered_map<const Instrument*, PnlPoint, boost::hash<const Instrument*> > InstrumentPnls;
    typedef InstrumentPnls::iterator InstrumentPnlsIter;
    typedef InstrumentPnls::const_iterator InstrumentPnlsConstIter;

public:
    IntradayPnlSnapshot();

    explicit IntradayPnlSnapshot(TimeType datetime);

    IntradayPnlSnapshot(TimeType datetime,
                        double fTotalPnl,
                        double fRealizedPnl);

    FillEntriesIter fill_entries_begin() { return fill_queue.begin(); }
    FillEntriesConstIter fill_entries_begin() const { return fill_queue.begin(); }

    FillEntriesIter fill_entries_end() { return fill_queue.end(); }
    FillEntriesConstIter fill_entries_end() const { return fill_queue.end(); }

    InstrumentPnlsIter instrument_pnls_begin() { return instrument_pnls.begin(); }
    InstrumentPnlsConstIter instrument_pnls_begin() const { return instrument_pnls.begin(); }

    InstrumentPnlsIter instrument_pnls_end() { return instrument_pnls.end(); }
    InstrumentPnlsConstIter instrument_pnls_end() const { return instrument_pnls.end(); }

    /**
     * Adds a fill entry for tracking within the snapshot
     */
    void AddFillInfo(const FillInfo& fillInfo);

    /**
     * Get the total pnl associated with an instrument
     * @returns pnl as double, 0 if instrument not present
     */
    double GetInstrumentTotalPnl(const Instrument* instrument) const;

    /**
     * Get the realized pnl associated with an instrument
     * @returns pnl as double, 0 if instrument not present
     */
    double GetInstrumentRPnl(const Instrument* instrument) const;

    /**
     * Adds an instrument pnl for tracking within the snapshot
     */
    void SetInstrumentPnl(const Instrument* instrument, double totalPnl, double rpl, double notional, double bid, double ask);

    /**
     * Get the instrument pnl point
     */
    PnlPoint GetInstrumentPnls(const Instrument* instrument);

    /**
     * Get the instrument pnl point as const
     */
    const PnlPoint& GetInstrumentPnls(const Instrument* instrument) const;

    /**
     * Save to binary stream
     */
    virtual void SaveToBinaryStream(Utilities::SerializedStream& out) const;

    /**
     * Load from binary stream
     * Note: Leaves instrument set to nullptr
     */
    virtual void LoadFromBinaryStream(Utilities::SerializedStream& in, IInstrumentFactory& instrumentFactory);

    /**
     * IntradayPnlSnapshot is ordered by datetime
     */
    bool operator<(const IntradayPnlSnapshot& rhs) const;

    bool operator==(const IntradayPnlSnapshot& rhs) const;

public:
    mutable InstrumentPnls instrument_pnls;
    FillEntries fill_queue;
    TimeType datetime;
    double total_pnl;
    double realized_pnl;
};

inline std::ostream& operator<<(std::ostream& out, const IntradayPnlSnapshot& fillInfo)
{
    out << "DateTime: " << boost::posix_time::to_simple_string(fillInfo.datetime) << " ";
    out << "TotalPnl: " << fillInfo.total_pnl << " ";
    out << "RealizedPnl: " << fillInfo.realized_pnl << std::endl;

    for (IntradayPnlSnapshot::FillEntriesConstIter it = fillInfo.fill_queue.begin(); it != fillInfo.fill_queue.end(); ++it) {
        out << "FillEntry: " << *it << std::endl;
    }

    return out;
}

} // namespace StrategyStudio
} // namespace RCM

#endif