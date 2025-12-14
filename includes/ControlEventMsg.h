/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/ControlEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_CONTROL_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_CONTROL_EVENT_MSG_H_

#include "TimePrioritizedEventMsg.h"
#include "Event.h"
#include "DataTypes.h"
#include <Utilities/IParams.h>

#include <boost/any.hpp>

namespace RCM {
namespace StrategyStudio {

enum ControlEventType {
    CONTROL_EVENT_TYPE_UNKNOWN,
    CONTROL_EVENT_TYPE_PNL_REPORT, // request strategy to send current pnl information
    CONTROL_EVENT_TYPE_POS_REPORT, // request strategy to send current position information
    CONTROL_EVENT_TYPE_PARAMS_UPDATE, // request strategy to update params
    CONTROL_EVENT_TYPE_END_OF_DAY_PNL_PERIOD,// request strategy to handle end of day PnL period
    CONTROL_EVENT_TYPE_INITIALIZE_DAY_PNL_PERIOD,// request strategy to send initial day PnL message
    CONTROL_EVENT_TYPE_SERIALIZE_STATE, // request strategy to save snapshots, settings, and results
    CONTROL_EVENT_TYPE_RESET_FOR_NEW_DAY, // request to reset strategy for new day
    CONTROL_EVENT_TYPE_FORCE_CANCEL_ALL_ORDERS, // have strategies force cancel all orders, used to emulate cancel-on-disconnect for live sim servers
    CONTROL_EVENT_TYPE_REMOVE_STUCK_ORDER // used to forcibly cancel an order that is stuck in a pending state
};

class ControlEventTypeUtils {
public:
    static const std::string& ToString(ControlEventType type);
};

/**
 * ControlEventMsg an event type for marshalling data about the strategy
 */
class ControlEventMsg : public TimePrioritizedEventMsg {
public:
    typedef std::shared_ptr<Utilities::IParams> IParamsSPtr;

public:
    ControlEventMsg(ControlEventType eventType, Event* event);

    ControlEventMsg(ControlEventType eventType, TimeType fireTime, Event* event, const IParamsSPtr& params = IParamsSPtr());

    ControlEventMsg(ControlEventType eventType, Event* event, const IParamsSPtr& params, const std::string& userName = "", ConnectionId connID = 0);

    ControlEventMsg(ControlEventType eventType, Event* event, const boost::any& closure, const std::string& userName = "", ConnectionId connID = 0);

    std::string name() const;

    const Utilities::IParams* params() const { return params_.get(); }

    const boost::any& closure() const { return closure_; }

    std::string user_name() const { return user_name_; }

    ConnectionId connection_id() const { return connection_id_; }

    ControlEventType control_event_type() const { return control_event_type_; }

    bool Trigger(TimeType /*currTime*/) { return true; }

private:
    std::string user_name_;
    IParamsSPtr params_;
    ControlEventType control_event_type_;
    ConnectionId connection_id_;
    boost::any closure_;
};

inline std::string ControlEventMsg::name() const
{
    return "Control_" + ControlEventTypeUtils::ToString(control_event_type_);
}

} // namespace StrategyStudio
} // namespace RCM

#endif
