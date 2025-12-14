/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IEventCallback.h
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

#ifndef _STRATEGY_STUDIO_LIB_IEVENT_CALLBACK_H_
#define _STRATEGY_STUDIO_LIB_IEVENT_CALLBACK_H_

#include <vector>
#include <utility>
#include <string>

namespace RCM {
namespace StrategyStudio {

class EventMsg;

/**
 * The IEventCallback is the core interface for objects that listen to event messages
 */
class IEventCallback {
public:
    IEventCallback() {}
    virtual ~IEventCallback() {}

public:
    /**
     * Provides name for the listener so we can query who receives an event
     */
    virtual const std::string& event_listener_name() const = 0;

    /**
     * This is the interface to be implemented when an event msg is received
     * @returns true if event processed, false otherwise
     */
    virtual void OnEvent(EventMsg* eventMsg) = 0;
};

typedef std::pair<int, IEventCallback*> EventListenerPair;

inline bool operator<(const EventListenerPair& lhs, const EventListenerPair& rhs)
{
    return (lhs.first < rhs.first);
}

typedef std::vector<EventListenerPair> EventListeners;
typedef EventListeners::iterator EventListenersIter;
typedef EventListeners::const_iterator EventListenersConstIter;

} // namespace StrategyStudio
} // namespace RCM

#endif