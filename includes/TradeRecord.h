/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/TradeRecord.h
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

#ifndef _STRATEGY_STUDIO_LIB_TRADERECORD_H_
#define _STRATEGY_STUDIO_LIB_TRADERECORD_H_

#include "TradeReport.h"
#include <list>
#include <vector>
#include <utility>

namespace RCM {
namespace StrategyStudio {

namespace Utilities {
class SerializedStream;
}

class IInstrumentFactory;

/**
 * TradeRecord holds the list of trade reports associated with a particular instrument
 */
class TradeRecord {
public:
    typedef std::list<TradeReport> TradeList;
    typedef TradeList::iterator TradeListIter;
    typedef TradeList::const_iterator TradeListConstIter;

    typedef std::vector<TradeReport> HistoricalTrades;
    typedef HistoricalTrades::iterator HistoricalTradesIter;
    typedef HistoricalTrades::const_iterator HistoricalTradesConstIter;

public:
    explicit TradeRecord(bool saveHistory = false);
    explicit TradeRecord(const Instrument* instrument, bool saveHistory = false);

    const Instrument* instrument() const { return instrument_; }
    void set_instrument(const Instrument* instrument) { instrument_ = instrument; }

    TradeListIter trade_open_begin() { return open_trades_.begin(); }
    TradeListConstIter trade_open_begin() const { return open_trades_.begin(); }

    TradeListIter trade_open_end() { return open_trades_.end(); }
    TradeListConstIter trade_open_end() const { return open_trades_.end(); }

    TradeListIter trade_notify_begin() { return notify_trades_.begin(); }
    TradeListConstIter trade_notify_begin() const { return notify_trades_.begin(); }

    TradeListIter trade_notify_end() { return notify_trades_.end(); }
    TradeListConstIter trade_notify_end() const { return notify_trades_.end(); }

    HistoricalTradesIter trade_closed_begin() { return closed_trades_.begin(); }
    HistoricalTradesConstIter trade_closed_begin() const { return closed_trades_.begin(); }

    HistoricalTradesIter trade_closed_end() { return closed_trades_.end(); }
    HistoricalTradesConstIter trade_closed_end() const { return closed_trades_.end(); }

    size_t num_open_trades() const { return open_trades_.size(); }
    size_t num_notify_trades() const { return notify_trades_.size(); }
    size_t num_closed_trades() const { return closed_trades_.size(); }

    bool save_history_flag() const { return save_history_; }
    void set_save_history_flag(bool flag) { save_history_ = flag; }

    /**
     * Returns the trade at the front of the record
     */
    TradeReport& FrontTrade() { assert(open_trades_.size() > 0); return open_trades_.front(); }
    const TradeReport& FrontTrade() const { assert(open_trades_.size() > 0); return open_trades_.front(); }

    /**
     * Returns the trade at the back of the record
     */
    TradeReport& BackTrade() { assert(open_trades_.size() > 0); return open_trades_.back(); }
    const TradeReport& BackTrade() const { assert(open_trades_.size() > 0); return open_trades_.back(); }

    /**
     * Get most current closed trade
     */
    TradeReport& LastTrade() { assert(closed_trades_.size() > 0); return closed_trades_.back(); }
    const TradeReport& LastTrade() const { assert(closed_trades_.size() > 0); return closed_trades_.back(); }

    /**
     * Pops all contents of notify trades to an out iterator
     */
    template<typename Iter>
    void PopNotifyTrades(Iter outIter);

    std::pair<bool, bool> AddTrade(TradeID tradeID, double price, int quantity, bool leggingIn);

    /**
     * Adds a new trade for record keeping with given trade id
     * @returns pair <true if trade id was used, false otherwise : true if close occurred, false otherwise>
     */
    std::pair<bool, bool> AddTrade(TradeID tradeID, double price, int quantity, TimeType updateTime, bool leggingIn);

    /**
     * Apply dividend to each trade
     */
    void ApplyDividend(double div);

    /**
     * Apply split to each trade if instruments match
     */
    void ApplySplit(double splitRatio);

    void ClearOpenTrades();

    void ClearNotifyTrades();

    void ClearClosedTrades();

    /**
     * Reset all associated state
     */
    void ResetState();

    /**
     * Save to stream
     */
    void SaveToBinaryStream(Utilities::SerializedStream& out, bool saveOnlyOpen = false) const;

    /**
     * Load from stream
     */
    void LoadFromBinaryStream(Utilities::SerializedStream& in, const IInstrumentFactory& factory);

    bool operator==(const TradeRecord& record) const;

private:
    TradeList open_trades_; // list of open trades
    TradeList notify_trades_; // list of trades that were recently closed for notifying clients
    HistoricalTrades closed_trades_; // historical list of trades that were closed
    const Instrument* instrument_;
    bool save_history_; // flag for letting us know if we should save history
};

 template<typename Iter>
 void TradeRecord::PopNotifyTrades(Iter outIter)
 {
    while (!notify_trades_.empty()) {
        *outIter = notify_trades_.front();
        notify_trades_.pop_front();
    }
 }

} // namespace StrategyStudio
} // namespace RCM

#endif
