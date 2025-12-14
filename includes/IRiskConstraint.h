/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IRiskConstraint.h
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

#ifndef _STRATEGY_STUDIO_LIB_IRISK_CONSTRAINT_H_
#define _STRATEGY_STUDIO_LIB_IRISK_CONSTRAINT_H_

#include "DataTypes.h"
#include "IPortfolioTracker.h"
#include "IOrderTracker.h"
#include "OrderParams.h"
#include <Utilities/Testable.h>

namespace RCM {
namespace StrategyStudio {

class Order;
class IRiskConstraint;

/**
 * Return value from risk check
 * boolean test returns true if risk check is ok
 * Otherwise false and constraint() returns the failing constraint and error_msg() returns a failure message
 */
class RiskCheckRetVal : public Utilities::Testable <RiskCheckRetVal> { // CRTP
public:
    RiskCheckRetVal(const IRiskConstraint* constraint, std::string errorMsg) : m_constraint(constraint), m_errorMsg(errorMsg) {}
    const IRiskConstraint* constraint() const { return m_constraint; }
    const std::string& error_msg() const { return m_errorMsg; }

    bool test() const { return (m_constraint == 0); } // must be public because of CRTP

private:
    RiskCheckRetVal& operator=(const RiskCheckRetVal&); // not implemented due to const members

private:
    const IRiskConstraint* m_constraint;
    std::string m_errorMsg;
};

/**
 * Interface for a risk constraint that risk profile will check
 * Concrete derived classes should define a label string called 'TOKEN'
 */
class IRiskConstraint {
protected:
    IRiskConstraint() {}

public:
    virtual ~IRiskConstraint() {}

    /**
     * Get the name of this constraint
     * Note: Value returned as const ref
     */
    virtual const std::string& name() const = 0;

    /**
     * Runs a check to see if risk constraint is within allowed limits
     * @returns true if risk constraint is within allowed limits, false otherwise
     */
    virtual RiskCheckRetVal CheckRiskConstraint(const IPortfolioTracker& portTracker,
                                     const IOrderTracker& orderTracker,
                                     const OrderParams& order,
                                     Utilities::TimeType curTime,
                                     const Order* oldOrder) const = 0;

    /**
     * Runs a check to verify portfolio risk is within allowed limit
     * @returns true if risk constraint is within allowed limits, false otherwise
     */
    virtual RiskCheckRetVal CheckRiskConstraint(const IPortfolioTracker& portTracker,
                                     const IOrderTracker& orderTracker,
                                     const Instrument* instrument) const { return success; }


    /**
     * Gets limit for specified symbol, or default limit if empty string passed in
     * @returns limit as a double, or quiet_NaN() if no limit applies
     */
    virtual double limit(const std::string& symbol = "") const = 0;
    virtual void set_limit(double limit, const std::string& symbol) = 0;

    static const RiskCheckRetVal success;

protected:
    static double MarkingPrice(const OrderParams& order);
    static const double INVALID_DOUBLE;
};

} // namespace StrategyStudio
} // namespace RCM

#endif