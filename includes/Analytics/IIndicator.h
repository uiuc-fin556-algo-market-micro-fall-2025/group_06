/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Analytics/IIndicator.h
*     Last Update: 2011/07/01 13:55:14
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2011.
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

#ifndef _STRATEGY_STUDIO_ANALYTICS_IINDICATOR_H_
#define _STRATEGY_STUDIO_ANALYTICS_IINDICATOR_H_

namespace RCM {
namespace StrategyStudio {
namespace Analytics {

/**
 * IIndicator is the class interface for Strategy Studio's collection of technical indicators
 * and time series operators
 */
template <typename InputType, typename OutputType>
class IIndicator {
public:
    typedef InputType input_type;
    typedef OutputType output_type;

public:
    virtual ~IIndicator(){}

    /**
     * Resets the state of the indicator
     */
    virtual bool Reset() = 0;

    /**
     * This function is used to input the latest observation with
     * which to update the value of the indicator.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully
     */
    virtual bool Update(const input_type& observation)=0;

    /**
     * This function is used to input the latest observation with
     * which to update the value of the indicator.
     *
     * @return Returns the updated value of the indicator, or the previous value if the update fails
     */
    virtual const output_type& UpdateAndGetValue(const input_type& observation) {Update(observation); return value();}

    /**
     * Returns the current value of the indicator
     */
    virtual const output_type& value() const = 0;

    /**
     * Returns whether the indicator's calculations are fully initialized
     */
    virtual bool fully_initialized() const = 0;
};

} // namespace Analytics
} // namespace StrategyStudio
} // namespace RCM

#endif