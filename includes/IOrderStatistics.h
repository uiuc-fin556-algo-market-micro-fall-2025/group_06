/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IOrderStatistics.h
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

#ifndef _STRATEGY_STUDIO_LIB_IORDER_STATISTICS_H_
#define _STRATEGY_STUDIO_LIB_IORDER_STATISTICS_H_

namespace RCM {
namespace StrategyStudio {

/**
 * Class for holding statistics for the orders tracked by an IOrderTracker
 */
class IOrderStatistics {
public:
    IOrderStatistics() {}

    virtual ~IOrderStatistics() {}

public:

    /**
     * Returns number of orders succesfully sent today
     */
    virtual int num_orders_sent() const = 0;

    /**
     * Returns number of orders filled to completion today
     */
    virtual int num_orders_filled() const = 0;

    /**
     * Returns number of orders rejected today
     */
    virtual int num_orders_rejected() const = 0;

    /**
     * Returns number of orders cancelled today
     */
    virtual int num_orders_cancelled() const = 0;

    /**
     * Returns number of pending orders
     */
    virtual int num_orders_open_pending() const = 0;

    /**
     * Returns number of orders cancel pending
     */
    virtual int num_orders_cancel_pending() const = 0;

    /**
     * Returns number of orders replace pending
     */
    virtual int num_orders_replace_pending() const = 0;

    /**
     * Returns the total size of all working long orders
     */
    virtual int long_working_order_size() const = 0;

    /**
     * Returns the total (signed) size of all working short orders
     */
    virtual int short_working_order_size() const = 0;

    /**
     * Returns the net (signed) size of all working orders
     */
    virtual int net_working_order_size() const = 0;

    /**
     * Total notional of all working long orders
     *
     * Returns sum of order price * size_remaining over all working long orders.
     * To have this calculation work with market orders send a meaningful order price.
     */
    virtual double long_working_notional() const = 0;

    /**
     * Total (signed) notional of all working short orders
     *
     * Returns sum of order price * size_remaining over all working short orders.
     * To have this calculation work with market orders send a meaningful order price.
     */
    virtual double short_working_notional() const = 0;

    /**
     * Net (signed) notional of all working short orders
     *
     * Returns sum of order price * size_remaining over all working orders.
     * To have this calculation work with market orders send a meaningful order price.
     */
    virtual double net_working_notional() const = 0;
};

} // namespace StrategyStudio
} // namespace RCM

#endif
