/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyTradeActions.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGY_TRADE_ACTIONS_H_
#define _STRATEGY_STUDIO_LIB_STRATEGY_TRADE_ACTIONS_H_

#include "DataTypes.h"
#include "ExecutionTypes.h"

namespace RCM {
namespace StrategyStudio {

class IStrategy;
class ITimeKeeper;
class IRiskConstraint;
class OrderIDMgr;
struct OrderParams;
class TradeActions;
class StrategyLogger;
class Order;

/**
 * This class functions as a wrapper for a TradeActions object.
 * It's purpose is to simplify the TradeActions interface for
 * strategies by passing any additional objects that the ExecutionHandler needs.
 * Also risk checks are performed at this level.
 * Note: This class must be modified whenever interface functions
 *       are added or changed in the TradeActions class.
 */
class StrategyTradeActions {
public:
    StrategyTradeActions(IStrategy* strategy,
                         ITimeKeeper* timeKeeper,
                         OrderIDMgr* orderIDMgr,
                         StrategyLogger* logger);

    ~StrategyTradeActions();

    ITimeKeeper* time_keeper() { return time_keeper_; }
    void set_time_keeper(ITimeKeeper* timeKeeper) { time_keeper_ = timeKeeper; }

public: /* Execution actions for use by strategies */

    /**
     * Send a new order
     * Note that derived children set the OrderTag portion of the Order Params
     */
    virtual TradeActionResult SendNewOrder(OrderParams& params);
    virtual TradeActionResult SendNewOrder(OrderParams& params, const std::string& account);

    /**
     * Cancel an existing order
     */
    virtual TradeActionResult SendCancelOrder(OrderID orderID);

    /**
     * We assume that the newParams has the same order id as a previously sent order,
     * if not we replace the newParams id with the old params ID as Strategy Studio's internal
     * id's do not change during a cancel replace.
     */
    virtual TradeActionResult SendCancelReplaceOrder(OrderID oldOrderID, OrderParams& newParams);

    /**
     * Sends cancels for all of the strategy's orders
     *
     * @return Returns TRADE_ACTION_RESULT_SUCCESFUL if all cancels can be sent to the execution provider, else TRADE_ACTION_RESULT_OMS_REJECT if any fail to send
     */
    virtual TradeActionResult SendCancelAll();

    /**
     * Sends cancels for all of the strategy's orders for the specified instrument
     *
     * @return Returns TRADE_ACTION_RESULT_SUCCESFUL if all cancels can be sent to the execution provider, else TRADE_ACTION_RESULT_OMS_REJECT if any fail to send
     */
    virtual TradeActionResult SendCancelAll(const Instrument* instrument);

private:
    /**
     * Performs a risk check using the new order information
     * @returns true if risk check passed, false otherwise
     */
    bool CheckRiskProfile(OrderParams& params, Order* oldOrder = 0) const;

    /**
     * Triggered when a new order is sent successfully. Creates an order update event that will be sent to strategy.
     */
    void OnNewOrder(OrderID orderID) const;

    /**
     * Triggered when a cancel request is sent successfully. Creates an order update event that will be sent to strategy.
     */
    void OnCancelOrder(OrderID orderID) const;

    /**
     * Triggered when a cancel replace request is sent successfully. Creates an order update event that will be sent to strategy.
     */
    void OnCancelReplaceOrder(OrderID orderID, const OrderParams& newParams) const;

    /**
     * Triggered when a risk check fails. Creates an order update event that will be sent to strategy.
     */
    void OnOrderReject(OrderID orderID) const;

    /**
     * Triggered when a cancel fails. Creates an order update event that will be sent to strategy.
     */
    void OnCancelReject(OrderID orderID) const;

    /**
     * Triggered when a replace fails. Creates an order update event that will be sent to strategy.
     */
    void OnReplaceReject(OrderID orderID) const;

    /**
     * Triggered when a risk check fails. Creates an order update event that will be sent to strategy.
     */
    void OnRiskConstraintViolation(const IRiskConstraint* riskConstraint, const std::string& failureMsg, OrderParams& params) const;

    /**
     * Creates an order update event that will be sent to strategy with given order state
     */
    void ReportOrderState(OrderID orderID, OrderUpdateType updateType) const;

private:
    IStrategy* strategy_;
    ITimeKeeper* time_keeper_;
    OrderIDMgr* order_id_mgr_;
    StrategyLogger* logger_;
};

} // namespace StrategyStudio
} // namespace RCM

#endif