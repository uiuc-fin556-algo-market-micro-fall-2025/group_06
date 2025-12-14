/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/FillInfo.h
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

#ifndef _STRATEGY_STUDIO_LIB_FILL_INFO_H_
#define _STRATEGY_STUDIO_LIB_FILL_INFO_H_

#include "DataTypes.h"
#include "ExecutionTypes.h"
#include <Utilities/TimeType.h>
#include <MarketModels/MarketCenterTypes.h>
#include <ostream>

namespace RCM {
namespace StrategyStudio {

namespace Utilities {
class SerializedStream;
}

class IInstrumentFactory;

/**
 * Contains information related to fills
 */
struct FillInfo {
public:
    FillInfo();

    FillInfo(MarketModels::OrderID id, const Instrument* instrument, int size, double price, LiquidityAction la);

    FillInfo(MarketModels::OrderID id,
             const std::string& execID,
             const Instrument* instrument,
             int size,
             double price,
             TransactionType transactionType,
             LiquidityAction la,
             int liquidityCode,
             const std::string& rawLiquidity,
             double execCosts,
             TimeType time,
             bool isPartial,
             MarketModels::MarketCenterID exchange,
             MarketModels::MarketCenterID lastMarket,
             bool leggingIn);

    const Instrument* instrument() const { return instrument_; }
    void set_instrument(const Instrument* instrument) { instrument_ = instrument; }

    /**
     * Price with split adjustment
     */
    double price() const { return adjusted_price_; }

    /**
     * Size with split adjustment
     */
    int size() const { return adjusted_size_; }

    /**
     * Original price without corporate actions applied
     */
    double fill_price() const { return price_; }

    /**
     * Original size without corporate actions applied
     */
    int fill_size() const { return size_; }

    /**
     * Indicates whether this FillInfo represents a new fill or a bust or trade correction
     */
    TransactionType transaction_type() const { return transaction_type_; }

    /**
     * Normalized liquidity code if execution provider liquidity is available
     */
    LiquidityAction liquidity_action() const { return liquidity_action_; }

    /**
     * Liquidity code as provided by the execution provider, if available
     */
    int liquidity_code() const { return liquidity_code_; }

    /**
     * Raw liquidity values as reported by the market, if available
     */
    std::string raw_liquidity() const { return raw_liquidity_; }

    double exec_costs() const { return exec_costs_; }
    void set_exec_costs(double cost) { exec_costs_ = cost; }

    /**
     * The market center where the order was sent
     */
    MarketCenterID fill_market_id() const { return fill_market_id_; }
    void set_fill_market_id(MarketCenterID exch) { fill_market_id_ = exch; }

    /**
     * The market center where this fill occurred if known
     */
    MarketCenterID last_market() const { return last_market_; }

    TimeType fill_time() const { return time_; }
    void set_fill_time(TimeType time) { time_ = time; }

    bool is_partial() const { return is_partial_; }
    void set_is_partial(bool flag) { is_partial_ = flag; }

    const std::string& order_tag() const { return order_tag_; }
    void set_order_tag(const std::string& tag) { order_tag_ = tag; }

    bool is_legging_in() const { return legging_in_; }
    void set_is_legging_in(bool flag) { legging_in_ = flag; }

    double split_adjustment_factor() const { return split_adjustement_factor_; }

    MarketModels::OrderID order_id() const { return order_id_; }
    void order_id(MarketModels::OrderID id) { order_id_ = id; }

    /**
     * The ExecID as reported by the execution provider; empty if not available
     */
    const std::string& exec_id() const { return exec_id_; }
    void set_exec_id(const std::string& id) { exec_id_ = id; }

public:
    /**
     * Add to execution costs
     */
    void AccumExecCosts(double execCost) { exec_costs_ += execCost; }

    /**
     * Initializes size of the fill, assuming there are no split adjustments applicable when called
     */
    void initialize_fill_size(int size);

    /**
     * Add to current size
     */
    void AccumSize(int size);

    /**
     * Sets the dividend adjustment field
     */
    void ApplyDividend(double div);

    /**
     * Sets the split adjustment field
     */
    void ApplySplit(double split);

    /**
     * Save to binary stream
     */
    void SaveToBinaryStream(Utilities::SerializedStream& out) const;

    /**
     * Load from binary stream
     * Note: InstrumentFactory needed to retrieve instrument
     */
    void LoadFromBinaryStream(Utilities::SerializedStream& in, IInstrumentFactory& instrumentFactory);

    /**
     * Load from binary stream
     * Note: Leaves instrument set to nullptr
     */
    void LoadFromBinaryStream(Utilities::SerializedStream& in);

    bool operator==(const FillInfo& fillInfo) const;

    /**
     * FillInfo objects are comparable by timestamp
     */
    bool operator<(const FillInfo& fillInfo) const;

private:
    void LoadFromBinaryStreamImpl(Utilities::SerializedStream& in);

private:
    std::string order_tag_;
    std::string exec_id_;
    std::string raw_liquidity_;
    TimeType time_;
    const Instrument* instrument_;
    MarketModels::OrderID order_id_;
    double price_; // raw fill price
    double adjusted_price_; // price adjusted for corporate actions
    double split_adjustement_factor_; // multiplicative adjustment used for when a split occurs
    double exec_costs_;
    int size_; // raw fill size
    int adjusted_size_; // size adjusted for corporate actions
    int liquidity_code_;
    TransactionType transaction_type_;
    LiquidityAction liquidity_action_;
    MarketCenterID fill_market_id_;
    MarketCenterID last_market_;
    bool is_partial_;
    bool legging_in_;
};

inline void FillInfo::AccumSize(int size)
{
    size_ += static_cast<int>(size*split_adjustement_factor_);
    adjusted_size_ += size;
}

inline void FillInfo::initialize_fill_size(int size)
{
    size_ = size;
    adjusted_size_ = size;
}

inline std::ostream& operator<<(std::ostream& out, const FillInfo& fillInfo)
{
    out << fillInfo.instrument()->symbol() << " at qty " << fillInfo.size() << " at price " << fillInfo.price();

    if (fillInfo.is_partial()) {
        out << " partial";
    }

    return out;
}

} // namespace StrategyStudio
} // namespace RCM

#endif