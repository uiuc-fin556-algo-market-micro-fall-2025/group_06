/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyStateControlEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGY_STATE_CONTROL_STATE_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_STRATEGY_STATE_CONTROL_STATE_EVENT_MSG_H_

#include "EventMsg.h"
#include "DataTypes.h"
#include "DataTypesUtils.h"
#include "Event.h"

namespace RCM {
namespace StrategyStudio {

namespace Utilities {
class IParams;
} // namespace Utilities

class IStrategy;
class IStrategyControl;

enum StrategyStateControlEventType {
    STRATEGY_STATE_CONTROL_EVENT_TYPE_UNKNOWN = 0,
    STRATEGY_STATE_CONTROL_EVENT_NOTIFY = 1, // used to notify strategy of a state change
    STRATEGY_STATE_CONTROL_EVENT_REQUEST = 2, // used when client requests strategy to trade
    STRATEGY_STATE_CONTROL_EVENT_RESET_FOR_BACKTEST = 3, // used to change back to start after back test done
    STRATEGY_STATE_CONTROL_EVENT_RESET_LIVE = 4
};

/**
 * StrategyStateControlEventMsg is an event type for notifying Strategies about a change in their state, for instance when the
 * Strategy Studio client application (eg Strategy Manager) instructs a strategy to start or stop.
 */
class StrategyStateControlEventMsg : public EventMsg {
public:
    typedef std::shared_ptr<Utilities::IParams> IParamsSPtr;

public:
    StrategyStateControlEventMsg(StrategyStateControlEventType eventType, 
                                 StrategyState strategyState, 
                                 Event* event, 
                                 TimeType eventTime = NULL_TIME_TYPE, 
                                 IStrategyControl* strategyControl = nullptr, 
                                 IStrategy* strategyClosure = nullptr);

    StrategyStateControlEventMsg(StrategyStateControlEventType eventType, StrategyState strategyState, const IParamsSPtr& params, Event* event);

    std::string name() const;

    /**
     * Returns the current state of the strategy
     */
    const StrategyState& strategy_state() const { return strategy_state_; }
    void set_strategy_state(const StrategyState& state) { strategy_state_ = state; }

    const Utilities::IParams* params() const { return params_.get(); }

    StrategyStateControlEventType control_event_type() const { return strategy_control_type_; }

private:
    /**
     * Handles the implementation logic of creating/deleting strategy
     */
    virtual void PostProcess();

private:
    IParamsSPtr params_;
    IStrategyControl* strategy_control_;
    IStrategy* strategy_closure_;
    StrategyStateControlEventType strategy_control_type_;
    StrategyState strategy_state_;
};

inline std::string StrategyStateControlEventMsg::name() const
{
    return "StrategyStateCntl_State_" + DataTypesUtils::ToString(strategy_state_);
}

} // namespace StrategyStudio
} // namespace RCM

#endif