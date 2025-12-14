/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/ReplayEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_REPLAY_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_REPLAY_EVENT_MSG_H_

#include "EventMsg.h"
#include "Event.h"
#include "DataTypes.h"
#include <Utilities/IParams.h>
#include <FlashProtocol/FlashBase.h>

namespace RCM {
namespace StrategyStudio {

enum ReplayEventType {
    REPLAY_EVENT_TYPE_UNKNOWN = 0,
    REPLAY_EVENT_TYPE_ORDER = 1,
    REPLAY_EVENT_TYPE_FILL = 2,
    REPLAY_EVENT_TYPE_PARAMS = 3,
    REPLAY_EVENT_TYPE_EXTERNAL_FILL = 4,
    REPLAY_EVENT_TYPE_RESET = 5,
    REPLAY_EVENT_TYPE_DAILY_REPORT = 6
};

class ReplayEventTypeUtils {
public:
    static const std::string& ToString(ReplayEventType type);
};

/**
 * ReplayEventMsg an event type for marshalling data about the strategy
 */
class ReplayEventMsg : public EventMsg {
public:
    ReplayEventMsg(ReplayEventType eventType, Event* event);

    // This one is used for passing flash messages
    ReplayEventMsg(ReplayEventType eventType,
                   const FlashProtocol::FlashBase* msg,
                   Event* event);

    std::string name() const;

    const FlashProtocol::FlashBase *Message() const { return message_; }
    ReplayEventType replay_event_type() const { return replay_event_type_; }

private:
    const FlashProtocol::FlashBase* message_;
    ReplayEventType replay_event_type_;
};

inline std::string ReplayEventMsg::name() const
{
    return "Replay_" + ReplayEventTypeUtils::ToString(replay_event_type_);
}

} // namespace StrategyStudio
} // namespace RCM

#endif