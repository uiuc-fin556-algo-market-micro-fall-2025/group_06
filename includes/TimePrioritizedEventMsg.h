/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/TimePrioritizedEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_TIME_PRIORITIZED_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_TIME_PRIORITIZED_EVENT_MSG_H_

#include "EventMsg.h"
#include <MarketModels/ITimePrioritized.h>

namespace RCM {
namespace StrategyStudio {

/**
 * TimePrioritizedEventMsg adds the time prioritization on the
 */
class TimePrioritizedEventMsg : public EventMsg, public MarketModels::ITimePrioritized {
public:
    TimePrioritizedEventMsg(EventMsgType eventMsgType, Event* event);
    TimePrioritizedEventMsg(EventMsgType eventMsgType, Event* event, Utilities::TimeType eventTime);
    TimePrioritizedEventMsg(EventMsgType eventMsgType, Event* event, int priority);
    TimePrioritizedEventMsg(EventMsgType eventMsgType, Event* event, Utilities::TimeType eventTime, int priority);

public: /* From ITimePrioritized */
    /**
     * Triggers any logic related to firing
     * Note: This should appropriately set the next fire time
     * @returns true if further processing should occur, false otherwise
     */
    virtual bool Trigger(Utilities::TimeType currTime)=0;

    bool purge_daily() const { return purge_daily_; }
    void set_purge_daily(bool purge) { purge_daily_ = purge; }

private:
    bool purge_daily_;
};

} // namespace StrategyStudio
} // namespace RCM

#endif