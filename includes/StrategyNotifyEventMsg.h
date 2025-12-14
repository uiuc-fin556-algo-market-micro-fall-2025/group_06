/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyNotifyEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGY_NOTIFY_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_STRATEGY_NOTIFY_EVENT_MSG_H_

#include "TimePrioritizedEventMsg.h"
#include "Event.h"
#include "DataTypes.h"
#include <ostream>

namespace RCM {
namespace StrategyStudio {

/**
 * StrategyNotifyEventMsg delivers messages from one strategy to another
 */
class StrategyNotifyEventMsg : public TimePrioritizedEventMsg {
public:
    static const int TIMED_EVENT_PRIORITY = 4;

public:
    StrategyNotifyEventMsg(const std::string& sender,
                           const std::string& target,
                           const std::string& msg,
                           Event* event);

    StrategyNotifyEventMsg(const std::string& sender,
                           const std::string& target,
                           const std::string& msg,
                           void* closure,
                           Event* event);

    /**
     * Name of the strategy that sent the message
     */
    const std::string& sender() const { return sender_; }

    /**
     * Name of the destination strategy
     */
    const std::string& target() const { return target_; }

    /**
     * Contents of the inter-strategy message
     */
    const std::string& msg() const { return msg_; }

    /**
     * Arbitrary closure object passed along with the message.
     */
    void* closure() const { return closure_; }

    /**
     * Checks whether the message has an associated closure object
     */
    bool HasClosure() const { return (closure_ != nullptr); }

    bool Trigger(TimeType currTime);

private:
    std::string sender_;
    std::string target_;
    std::string msg_;
    void* closure_;
};

inline std::ostream& operator<<(std::ostream& out, const StrategyNotifyEventMsg& msg)
{
    out << "Strategy notify event. Sender: " << msg.sender() << " Target: " << msg.target() <<
        " Msg: " << msg.msg();

    return out;
}

} // namespace StrategyStudio
} // namespace RCM

#endif