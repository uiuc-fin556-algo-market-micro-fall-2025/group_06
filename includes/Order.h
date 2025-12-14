/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Order.h
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

#ifndef _STRATEGY_STUDIO_LIB_ORDER_H_
#define _STRATEGY_STUDIO_LIB_ORDER_H_

#include "DataTypes.h"
#include "ExecutionTypes.h"
#include "OrderParams.h"
#include <Utilities/TimeType.h>
#include <cmath>

namespace RCM {
namespace StrategyStudio {

namespace Utilities {
class SerializedStream;
}

class IInstrumentFactory;

/**
 * Contains information related to orders
 */
class Order {
public:
    Order();

    Order(const OrderParams& params,
                 StrategyID strategyID,
                 StrategyRunMode runMode,
                 const std::string& trader,
                 const std::string& account,
                 const std::string& brokerName,
                 ExecutionProviderID execProviderID,
                 TimeType entryTime,
                 TimeType lastUpdateTime = NULL_TIME_TYPE);

    Order(const OrderParams& params,
                 StrategyID strategyID,
                 StrategyRunMode runMode,
                 const std::string& trader,
                 const std::string& account,
                 const std::string& brokerName,
                 ExecutionProviderID execProviderID,
                 int sizeCompleted,
                 double avgFillPrice,
                 double execCosts,
                 OrderState orderState,
                 OrderUpdateType updateType,
                 const std::string& reason,
                 TimeType entryTime,
                 TimeType lastUpdateTime = NULL_TIME_TYPE);

    Order(const Order& order);

    ~Order();

    Order& operator=(const Order& order);

public:
    const OrderParams& params() const { return m_params; }
    OrderParams& params() { return m_params; }
    void set_params(const OrderParams& params) { m_params = params; }

    StrategyID strategy_id() const { return m_strategyId; }

    StrategyRunMode run_mode() const { return m_runMode; }

    const std::string& trader() const { return m_trader; }

    const std::string& account() const { return m_account; }
    void set_account(const std::string& account) { m_account = account; }

    const std::string& broker_name() const { return m_brokerName; }

    ExecutionProviderID execution_provider_id() const { return m_execProviderID; }

    /**
     * The number of shares or contracts that have been executed. This number will be signed, negative for sales.
     */
    int size_completed() const { return m_sizeCompleted; }

    /**
     * The number of shares or contracts that remain to be executed. This number will be signed, negative for sales.
     */
    int size_remaining() const { return (m_params.quantity * (IsBuySide(m_params.order_side) ? 1 : -1)) - m_sizeCompleted; }

    double avg_fill_price() const { return m_avgFillPrice; }

    double exec_costs() const { return m_execCosts; }

    TimeType last_update_time() const { return m_lastUpdateTime; }

    TimeType entry_time() const { return m_entryTime; }

    OrderState order_state() const { return m_orderState; }

    OrderUpdateType last_update_type() const{ return m_lastUpdateType; }

    const std::string& reason() const { return m_reason; }

public: // convenience accessors for params fields
    const Instrument* instrument() const { return m_params.instrument; }

    /**
     * Gets the quantity of the order. Like in the OrderParams, this will be a positive number
     */
    int order_quantity() const { return m_params.quantity; }
    int signed_order_quantity() const { return (m_params.quantity) * (IsBuySide(m_params.order_side) ? 1 : -1); }

    /**
     * The display quantity of the order. This will be a non-negative number.
     */
    int display_quantity() const { return m_params.display_quantity; }
    int minimum_quantity() const { return m_params.minimum_quantity; }
    Price price() const { return m_params.price; }
    Price stop_price() const { return m_params.stop_price; }
    MarketCenterID market_center() const { return m_params.market_center; }
    OrderSide order_side() const { return m_params.order_side; }
    OrderTIF order_tif() const { return m_params.order_tif; }
    OrderType order_type() const { return m_params.order_type; }
    bool is_iso() const { return m_params.is_iso; }
    StrategyStudioUInt32 iso_group() const { return m_params.iso_group; }
    PegType peg_type() const { return m_params.peg_type; }
    PriceOffset peg_offset() const { return m_params.peg_offset; }
    bool is_hidden() const { return m_params.is_hidden; }
    bool post_only() const { return m_params.post_only; }
    AllowRouting allow_routing() const { return m_params.allow_routing; }
    LockedCrossedAction locked_crossed_action() const { return m_params.locked_crossed_action; }
    bool imbalance_only() const { return m_params.imbalance_only; }
    WashTradePrevention wash_trade_prevention() const { return m_params.wash_trade_prevention; }
    const std::string& routing_instructions() const { return m_params.routing_instructions; }
    bool legging_in() const { return m_params.legging_in; }
    OrderID order_id() const { return m_params.order_id; }
    const OrderTag& order_tag() const { return m_params.order_tag; }
    const boost::any& closure() const { return m_params.closure; }

public:
    /**
     * Load from binary stream and return the symbol; instrument will be nullptr
     * Enables loading without knowing the specific InstrumentFactory
     * @return string of the symbol loaded
     */
    std::string LoadFromBinaryStream(Utilities::SerializedStream& in);

    /**
     * Load from binary stream
     */
    void LoadFromBinaryStream(Utilities::SerializedStream& in, IInstrumentFactory& factory);

    /**
     * Save to stream
     */
    void SaveToBinaryStream(Utilities::SerializedStream& out) const;

    /**
     * Updates the order details with the new state and sets the previous
     * state member
     * @returns the previous state
     */
    OrderState UpdateOrderState(TimeType lastUpdateTime,
                                OrderState state,
                                OrderUpdateType updateType,
                                const std::string& reason = "");

    /**
     * Updates the order details with the new state and sets the previous (with exec costs)
     * state member
     * @returns the previous state
     */
    OrderState ProcessFill(TimeType lastUpdateTime,
                                OrderState state,
                                OrderUpdateType updateType,
                                double fillPrice,
                                int fillSize,
                                double execCosts);

    bool operator==(const Order& order) const;

private:
    OrderParams m_params;
    StrategyID m_strategyId;
    StrategyRunMode m_runMode;
    std::string m_trader;
    std::string m_account;
    std::string m_brokerName;
    ExecutionProviderID m_execProviderID;

    /**
     * The number of shares or contracts that have been executed. This number will be signed, negative for sales.
     */
    int m_sizeCompleted;
    double m_avgFillPrice;
    double m_execCosts;
    TimeType m_lastUpdateTime;
    TimeType m_entryTime;
    OrderState m_orderState;
    OrderUpdateType m_lastUpdateType;
    std::string m_reason;
};

} // namespace StrategyStudio
} // namespace RCM

#endif