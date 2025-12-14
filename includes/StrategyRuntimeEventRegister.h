/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyRuntimeEventRegister.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGY_RUNTIME_EVENT_REGISTER_H_
#define _STRATEGY_STUDIO_LIB_STRATEGY_RUNTIME_EVENT_REGISTER_H_

#include "DataTypes.h"
#include "SymbologyTypes.h"
#include "EventType.h"
#include <boost/any.hpp>
#include <utility>

namespace RCM {
namespace StrategyStudio {

class Event;
class IStrategy;
class IRuntimeEventRegister;
class IEventRequester;

/**
 * This is the registration interface that strategies use
 * to dynamically register for run-time events.
 */
class StrategyRuntimeEventRegister {
public:
    StrategyRuntimeEventRegister(IEventRequester* requester, IRuntimeEventRegister* evtRegister);
    virtual ~StrategyRuntimeEventRegister();

public: /* Types of events that clients can register for */
    /**
     * Register to receive a single timed events
     *
     * @param name Name used to label the timed event(s)
     * @param fireTime Specifies the time to trigger the scheduled event
     * @param fireIfPast Specifies whether to deliver the event message if the fireTime precedes the time the strategy initializes.
     * @returns A pointer to the created event
     */
    virtual const Event* RegisterForSingleScheduledEvent(const std::string& name, TimeType fireTime, bool fireIfPast = false);

    /**
     * Register to receive recurring timed events for a strategy
     *
     * @param name Name used to label the timed event(s)
     * @param firstFireTime Specifies the first time to trigger the scheduled events
     * @param endFireTime Specifies the time to stop triggering the scheduled events. Use NULL_TIME_TYPE to continue firing events until the end of the trading day.
     * @param interval Specifies the time between firings
     * @param fireIfPast Specifies whether to deliver event messages scheduled to trigger before the time the strategy initializes.
     * @returns A pointer to the created event
     */
    virtual const Event* RegisterForRecurringScheduledEvents(const std::string& name,
                                                             TimeType firstFireTime,
                                                             TimeType endFireTime,
                                                             TimeDuration interval,
                                                             bool fireIfPast = false);

    /**
     * Used to send strategy notification event messages between strategies
     * @returns true if event msg scheduled for sending, false if target doesn't exist
     */
    virtual bool SendMessageToStrategy(const std::string& target, const std::string& msg);

    /**
     * Used to send strategy notification event messages between strategies with closure argument
     * @returns true if event msg scheduled for sending, false if target doesn't exist
     */
    virtual bool SendMessageToStrategy(const std::string& target,
                                       const std::string& msg,
                                       void* closure);

    /**
     * Publishes a message to a given topic
     */
    virtual bool SendMessageToTopic(const std::string& topic,
                                    const std::string& msg,
                                    const boost::any& closure = boost::any());

    /**
     * Used to send an exogenous fill to a specified strategy.
     *
     * Like with reporting external fills via command line or Strategy Manager, this will update
     * target strategy's positions and trigger OnExternalFill. Target strategy must be able to subscribe to specified symbol.
     *
     * @returns true if event msg scheduled for sending, false if target strategy doesn't exist or if it can't subscribe to the symbol
     */
    virtual bool SendExternalFillToStrategy(const std::string& targetStrategy,
                                            const std::string& symbol,
                                            int size,
                                            double price,
                                            MarketCenterID market,
                                            bool applyFees = true);

private:
    IEventRequester* requester_;
    IRuntimeEventRegister* event_register_;
    friend class StrategyBase;
};

} // namespace StrategyStudio
} // namespace RCM

#endif
