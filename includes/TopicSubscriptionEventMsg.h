/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/TopicSubscriptionEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_TOPIC_SUBSCRIPTION_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_TOPIC_SUBSCRIPTION_EVENT_MSG_H_

#include "TimePrioritizedEventMsg.h"
#include "Event.h"
#include "DataTypes.h"
#include <boost/any.hpp>
#include <ostream>

namespace RCM {
namespace StrategyStudio {

/**
 * TopicSubscriptionEventMsg delivers the contents of a message broadcast to a named topic
 */
class TopicSubscriptionEventMsg : public TimePrioritizedEventMsg {
public:
    static const int TIMED_EVENT_PRIORITY = 4;

public:
    TopicSubscriptionEventMsg(const std::string& topic,
                              const std::string& msg,
                              Event* event);

    TopicSubscriptionEventMsg(const std::string& topic,
                              const std::string& msg,
                              const boost::any& closure,
                              Event* event);

    /**
     * Name of the topic to which the message was broadcast
     */
    const std::string& topic() const { return topic_; }

    /**
     * Contents of the message
     */
    const std::string& msg() const { return msg_; }

    /**
     * Arbitrary closure object passed along with the message.
     */
    const boost::any& closure() const { return closure_; }

    bool Trigger(TimeType currTime);

private:
    std::string topic_;
    std::string msg_;
    boost::any closure_;
};

inline std::ostream& operator<<(std::ostream& out, const TopicSubscriptionEventMsg& msg)
{
    out << "Topic subscription event. Topic: " << msg.topic() << " Msg: " << msg.msg();
    return out;
}

} // namespace StrategyStudio
} // namespace RCM

#endif