/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Event.h
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

#ifndef _STRATEGY_STUDIO_LIB_EVENT_H_
#define _STRATEGY_STUDIO_LIB_EVENT_H_

#include "DataTypes.h"
#include "EventType.h"
#include "IEventCallback.h"
#include "ServerDebugLogBase.h"

namespace RCM {
namespace StrategyStudio {

class EventMsg;

/**
 * Identifies an event within the Strategy Server
 *
 * Events allow chaining, thus an Event is also a listener, if it receives an event
 * then it can forward the event to all its listeners
 */
class Event final {
public:
    Event(EventType type, const std::string& eventName, ProcessorID procID, void* submitter_closure);

    EventType type() const { return event_type_; }
    const std::string& name() const { return event_name_; }

    /**
     * Gets processor id
     */
    ProcessorID processor_id() const { return processor_id_; }

    /**
     * Gets the event submitter for this event's processor
     */
    void* submitter_closure() const { return submitter_closure_; }

    EventListenersIter listener_begin() { return event_listeners_.begin(); }
    EventListenersIter listener_end() { return event_listeners_.end(); }

    EventListenersConstIter listener_begin() const { return event_listeners_.begin(); }
    EventListenersConstIter listener_end() const { return event_listeners_.end(); }

    /**
     * Adds the event listener to the event
     */
    bool AddEventListener(int priority, IEventCallback* eventListener);

    /**
     * Remove the event listener
     * @returns true if event listener was removed, false otherwise
     */
    bool RemoveEventListener(IEventCallback* eventListener);

public:
    /**
     * This is the interface to trigger an event
     * @returns true if event processed, false otherwise
     */
    void TriggerEvent(EventMsg* eventMsg);

private:
    EventListeners event_listeners_;
    std::string event_name_;
    void* submitter_closure_;
    ProcessorID processor_id_;
    EventType event_type_;
};

inline void Event::TriggerEvent(EventMsg* eventMsg)
{
    EventListenersIter end_it = event_listeners_.end();
    for (EventListenersIter it = event_listeners_.begin(); it != end_it; ++it) {
        SERVER_DEBUG_LOG("Sending event to listener " + it->second->event_listener_name());
        it->second->OnEvent(eventMsg);
        SERVER_DEBUG_LOG("Finished sending event to listener " + it->second->event_listener_name());
    }
}

} // namespace StrategyStudio
} // namespace RCM

#endif