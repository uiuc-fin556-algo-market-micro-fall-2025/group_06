/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IOrderTracker.h
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

#ifndef _STRATEGY_STUDIO_LIB_IORDERTRACKER_H_
#define _STRATEGY_STUDIO_LIB_IORDERTRACKER_H_

#include "ExecutionTypes.h"
#include "Order.h"
#include "IOrderStatistics.h"
#include <boost/unordered_map.hpp>

namespace RCM {
namespace StrategyStudio {

class IInstrumentFactory;

/**
 * Interface for tracking orders placed by a strategy
 */
class IOrderTracker {
public:
    typedef std::list<Order*> WorkingOrders;
    typedef WorkingOrders::iterator WorkingOrdersIter;
    typedef WorkingOrders::const_iterator WorkingOrdersConstIter;

    typedef std::pair<Order*, WorkingOrdersIter> OrderTrackingPair;

    typedef boost::unordered_map<OrderID, OrderTrackingPair> TrackedOrders;
    typedef TrackedOrders::iterator TrackedOrdersIter;
    typedef TrackedOrders::const_iterator TrackedOrdersConstIter;

public:

    IOrderTracker();

    virtual ~IOrderTracker();

    /**
     * Provides access to the working orders
     */
    virtual WorkingOrdersConstIter working_orders_begin() const = 0;
    virtual WorkingOrdersConstIter working_orders_end() const = 0;

    /**
     * Provides access to the working orders for a particular instrument
     */
    virtual WorkingOrdersConstIter working_orders_begin(const Instrument* instrument) const = 0;
    virtual WorkingOrdersConstIter working_orders_end(const Instrument* instrument) const = 0;

    /**
     * Provides access to all of today's orders if recovery is enabled
     *
     * @returns an iterator to TrackedOrders, where member second is an OrderTrackingPair
     */
    virtual TrackedOrdersConstIter tracked_orders_begin() const = 0;
    virtual TrackedOrdersConstIter tracked_orders_end() const = 0;

    /**
     * Finds an order by its order ID
     *
     * @returns a pointer to the order if found, null otherwise
     */
    virtual const Order* find(OrderID orderID) const = 0;

    /**
     * Finds a working order by its order ID
     *
     * @returns a pointer to the order if found, null otherwise
     */
    virtual const Order* find_working(OrderID orderID) const = 0;

    /**
     * Checks if the OrderTracker contains an order for the supplied ID
     */
    virtual bool contains(OrderID orderID) const = 0;

    /**
     * Checks whether the order corresponding to the supplied ID is present in the working orders list
     */
    virtual bool is_working(OrderID orderID) const = 0;

    /**
     * Returns number of working orders present in order tracker
     */
    virtual size_t num_working_orders() const = 0;

    /**
     * Returns number of working orders present in order tracker for a particular instrument
     */
    virtual size_t num_working_orders(const Instrument* instr) const = 0;

    /**
     * Returns number of orders present in order tracker
     */
    virtual size_t num_tracked_orders() const = 0;

    /**
     * Retrieve order tracking statistics
     */
    virtual const IOrderStatistics& stats() const = 0;

    /**
     * Retrieve order tracking statistics for a particular instrument
     */
    virtual const IOrderStatistics& stats(const Instrument* instr) const = 0;
};

} // namespace StrategyStudio
} // namespace RCM

#endif
