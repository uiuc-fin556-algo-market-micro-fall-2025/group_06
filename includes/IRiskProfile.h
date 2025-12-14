/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IRiskProfile.h
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

#ifndef _STRATEGY_STUDIO_LIB_I_RISK_PROFILE_H_
#define _STRATEGY_STUDIO_LIB_I_RISK_PROFILE_H_

#include "DataTypes.h"
#include "Utilities/TimeType.h"
#include <utility>

namespace RCM {
namespace StrategyStudio {

class IOrderTracker;
class IPortfolioTracker;
class IRiskConstraint;
struct OrderParams;
class Order;
class RiskCheckRetVal;

/**
 * Interface for risk profile
 */
class IRiskProfile {
public:
    IRiskProfile() {}
    virtual ~IRiskProfile() {}

public: /* Risk Profile interface */
    /**
     * Add a risk constraint to the profile
     */
    virtual void AddRiskConstraint(IRiskConstraint* constraint) = 0;

    /**
     * Runs a check to see risk is within allowed limits including information from new order
     * @returns RiskCheckRetVal
     *   if failed, returns first violated risk constraint and failure message
     */
    virtual RiskCheckRetVal CheckRiskConstraints(const IPortfolioTracker& portTracker,
                                                 const IOrderTracker& orderTracker,
                                                 const OrderParams& order,
                                                 Utilities::TimeType curTime,
                                                 const Order* oldOrder) const = 0;

    /**
     * Runs a check to verify portfolio risk is within allowed limit
     * @returns RiskCheckRetVal
     *   if failed, returns first violated risk constraint and failure message
     */
    virtual RiskCheckRetVal CheckRiskConstraints(const IPortfolioTracker& portTracker,
                                                 const IOrderTracker& orderTracker,
                                                 const Instrument* instrument) const = 0;

    /**
     * Get constraint with given name
     * @returns const IRiskConstraint*
     *   if failed, returns null
     */
    virtual const IRiskConstraint* GetConstraint(const std::string& name) const = 0;

    /**
    * Get constraint with given name
    * @returns IRiskConstraint*
    *   if failed, returns null
    */
    virtual IRiskConstraint* GetConstraint(const std::string& name) = 0;
};

} // namespace StrategyStudio
} // namespace RCM

#endif