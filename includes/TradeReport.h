/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/TradeReport.h
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

#ifndef _STRATEGY_STUDIO_LIB_TRADE_H_
#define _STRATEGY_STUDIO_LIB_TRADE_H_

#include "DataTypes.h"
#include <string>
#include <ostream>

namespace RCM {
namespace StrategyStudio {

namespace Utilities {
class SerializedStream;
}

/**
 * TradeReport is a construct for helping us track pnls by trades
 *
 * Entry and exit prices are net of fees and are
 * adjusted for splits and dividends that occur during the life of the TradeReport.
 */
class TradeReport {
public:
    TradeReport();
    explicit TradeReport(TradeID tradeID);
    TradeReport(TradeID tradeID, double entryPrice, int entryQuantity);
    TradeReport(TradeID tradeID, double entryPrice, int entryQuantity, TimeType entryTime);

    TradeID id() const { return trade_id_; }

    /**
     * Returns the realized PnL attributable to the trade report
     */
    double realized_pnl() const { return realized_pnl_; }

    /**
     * Returns average entry price net of fees, adjusted for splits and dividends that occured between open and close time
     */
    double average_entry_price() const { return entry_price_; }

    /**
     * Returns average exit price net of fees, adjusted for splits and dividends that occured between open and close time
     */
    double average_exit_price() const { return exit_price_; }

    /**
     * Returns split adjusted cumulative entry quantity, corresponding the maximum shares/contract position size that
     * had been carried during the life of the trade.
     *
     * This number will be negative for shorts
     */
    int entry_quantity() const { return entry_quantity_; }
    void set_entry_quantity(int qty) { entry_quantity_ = qty; }

    /**
     * Returns split adjusted quantity that remains open for this trade report.
     *
     * This number will be negative for shorts
     */
    int open_position() const { return open_quantity_; }
    void set_open_position(int qty) { open_quantity_ = qty; }

    /**
     * Returns the amount of the entry quantity that has been filled against, representing how much of
     * the trade has been closed out.
     *
     * This number will be negative for shorts
     */
    int filled_quantity() const { return (entry_quantity_ - open_quantity_); }

    /**
     * Returns the timestamp of the first entry fill that created the trade report.
     */
    TimeType entry_time() const { return entry_time_; }
    void set_entry_time(TimeType time) { entry_time_ = time; }

    /**
     * Returns the timestamp of the last exit fill that closed the trade report.
     */
    TimeType exit_time() const { return exit_time_; }
    void set_exit_time(TimeType time) { exit_time_ = time; }

    /**
     * Returns whether the trade is opened or closed.
     */
    TradeState trade_state() const { return trade_state_; }
    void set_trade_state(TradeState state) { trade_state_ = state; }

    /**
     * Apply dividend
     */
    void ApplyDividend(double div);

    /**
     * Apply split
     */
    void ApplySplit(double splitRatio);

    /**
     * Open an additional position in the trade
     */
    void OpenPosition(double price, int quantity, double multiplier = 1);

    /**
     * Closes a position (via a fill)
     * @param price Price at which the trade occurred
     * @param quantity Positive indicates that we were buying, negative means we were selling
     */
    void ClosePosition(double price, int quantity, double rate, double multiplier = 1);

    /**
     * Save to stream
     */
    void SaveToBinaryStream(Utilities::SerializedStream& out) const;

    /**
     * Load from stream
     */
    void LoadFromBinaryStream(Utilities::SerializedStream& in);

    bool operator==(const TradeReport& data) const;

    friend std::ostream& operator<<(std::ostream& out, const TradeReport& trade);

private:
    TimeType entry_time_;
    TimeType exit_time_;
    TradeID trade_id_;
    double entry_price_;
    double exit_price_;
    double realized_pnl_;
    int entry_quantity_;
    int open_quantity_;
    TradeState trade_state_;
};

inline std::ostream& operator<<(std::ostream& out, const TradeReport& trade)
{
    out << "ID " << trade.trade_id_
        << " AvgEntryPrice: " << trade.entry_price_
        << " EntryQty: " << trade.entry_quantity_
        << " AvgExitPrice: " << trade.exit_price_
        << " OpenQty: " << trade.open_quantity_
        << " RPL: " << trade.realized_pnl_
        << " EntryTime: " << Utilities::ToString(trade.entry_time_)
        << " ExitTime: " << Utilities::ToString(trade.exit_time_);

    return out;
}

} // namespace StrategyStudio
} // namespace RCM

#endif
