/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyEventRegister.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGY_EVENT_REGISTER_H_
#define _STRATEGY_STUDIO_LIB_STRATEGY_EVENT_REGISTER_H_

#include "DataTypes.h"
#include "SymbologyTypes.h"
#include "BarDataTypes.h"
#include "EventType.h"
#include "ICloneableEventCallback.h"
#include <boost/any.hpp>
#include <utility>

namespace RCM {
namespace StrategyStudio {

class Event;
class IStrategy;
class IEventRegister;
class IEventRequester;

typedef std::pair<const Event*, const Instrument*> EventInstrumentPair;

// macro for a null instrument registration
#define NULL_EVENT_INSTRUMENT_PAIR (EventInstrumentPair(static_cast<const Event*>(nullptr), static_cast<const Instrument*>(nullptr)))

/**
 * This is the registration interface that strategies use to subscribe to
 * events at the beginning of a new trading day in Strategy::RegisterForStrategyEvents
 */
class StrategyEventRegister {
public:
    StrategyEventRegister(IEventRequester* requester, IEventRegister* evtRegister);
    virtual ~StrategyEventRegister();

public:
    /**
     * Register to receive market data from the server's primary market data adapter
     *
     * Note: market data registration happens automatically for any symbol in the strategy instance's symbol list configuration;
     * this only needs to be called if additional programmatic subscriptions are necessary.
     *
     * @param symbol The symbol of the instrument for which the strategy wishes to receive market data
     * @param data_source (Optional) Overrides default configured adapters in subscription request. 
                                     Caution: instrument market data is shared at the event processor level, overriding this will be additive with requests made by other subscribers.
     * @returns Pointer to the event the strategy registered for along with the associated instrument; returns nulls if subscription failed
     */
    virtual EventInstrumentPair RegisterForMarketData(const SymbolTag& symbol, const std::string& data_source = "");

    /**
     * Register to receive Bars calculated from market data
     *
     * Will also automatically register for the underlying market data if this has not already been done
     *
     * @param symbol The symbol to calculate bars for
     * @param type The desired bar interval type
     * @param interval The desired bar interval size
     * @param p_value (Optional) The exponent to use to compute realized variance; relevant only when selecting BAR_TYPE_VARIANCE
     * @param alignment_time (Optional) The time to which time bars will align; relevant only when selecting BAR_TYPE_TIME
     * @param data_source (Optional) Optionally passed along to implicit market data registration request, see RegisterForMarketData documentation for more information
     * @returns Pair containing the created event and a the instrument associated with the bar subscription; returns nulls if registration fails
     */
    virtual EventInstrumentPair RegisterForBars(const SymbolTag& symbol, BarType type, double interval, double p_value = 2.0, TimeType alignment_time = boost::posix_time::not_a_date_time, const std::string& data_source = "");

    /**
     * Registers in batch for options market data on instruments meeting the input criteria on underlier, strikes, and expiry
     *
     * @param underlierSymbol symbol of the underlier
     * @param optionType specifies whether to subscribe to puts, calls, or both
     * @param minStrike Minimum strike
     * @param maxStrike Maximum strike
     * @param minExpiry Minimum expiry
     * @param maxExpiry Maximum expiry
     * @param includeAdjustedContracts specifies whether to include options with non-standard terms
     * @param data_source (Optional) Optionally passed along to implicit market data registration request, see RegisterForMarketData documentation for more information
     * @returns List of subscribed instruments
     */
    virtual InstrumentList RegisterForOptions(const SymbolTag& underlierSymbol,
                                              OptionType optionType = MarketModels::OPTION_TYPE_BOTH,
                                              double minStrike = MIN_STRIKE,
                                              double maxStrike = MAX_STRIKE,
                                              DateType minExpiry = MIN_EXPIRY,
                                              DateType maxExpiry = MAX_EXPIRY,
                                              bool includeAdjustedContracts = true,
                                              const std::string& data_source = "");

    /**
     * Register for futures
     *
     * @param baseSymbol exact underlying product code or symbol prefix
     * @param minMaturity (Optional) Minimum maturity
     * @param maxMaturity (Optional) Maximum maturity
     * @param includeSpreads (Optional) include spreads (default false)
     * @param data_source (Optional) Optionally passed along to implicit market data registration request, see RegisterForMarketData documentation for more information
     * @returns List of subscribed instruments
     */
    virtual InstrumentList RegisterForFutures(const std::string& baseSymbol,
                                              DateType minMaturity = MIN_EXPIRY,
                                              DateType maxMaturity = MAX_EXPIRY,
                                              bool includeSpreads = false,
                                              const std::string& data_source = "");

    /**
     * Register to receive news data
     *
     * @param entity The name of the topic to recieve news for, for company specific news, corresponds to symbol
     * @param type Type of news subscription to make
     * @param data_source (Optional) The type name of the data adapter to use
     * @returns A pointer to the created event
     */
    virtual const Event* RegisterForNews(const std::string& entity, NewsType type = NEWS_TYPE_SPECIFIC_ENTITY, const std::string& data_source = "");

    /**
     * Register to receive news data
     *
     * @param entity The name of the topic to recieve sentiment information for, for company specific news, corresponds to symbol
     * @param entity The sentiment ontology (determines both the type of entity and the sentiment message type) to subscribe for
     * @param data_source (Optional) The type name of the data adapter to use
     * @returns A pointer to the created event
     */
    virtual const Event* RegisterForSentiment(const std::string& entity, SentimentType type = SENTIMENT_TYPE_TICKER, const std::string& data_source = "");

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
     * Register for topic subscription messages
     * @returns A pointer to the created event
     */
    virtual const Event* RegisterForTopicSubscription(const std::string& topic);

    /**
     * Allows a strategy to register a closure object that will receive event messages from the parent event
     *
     * Note the object passed in will be deep copied via the interface's "clone()" method the first time it is registered on a processor,
     * and the object's "event_listener_name()" function is used for identification, ie to determine whether or not the object is already registered.
     * Subsequent registration requests will simply return a pointer to the object created during the initial registration.
     *
     * @param closure_object The object that wishes to receive event messages.
     * @param parent_event The event to send to the closure object
     * @returns A pointer to the singleton (event processor scope) instance of the closure object registered; returns null if request fails
     */
    virtual ICloneableEventCallback* RegisterForClosureEvent(const ICloneableEventCallback& closureObject, const Event* parentEvent);

    /**
     * Register to receive requests for order facilitation from a 3rd party platform
     *
     * @param symbol Symbol to receive external orders for
     * @returns Created event
     */
    virtual const Event* RegisterForExternalOrders(const SymbolTag& symbol, const std::string& order_source = "");

private:
    IEventRequester* requester_;
    IEventRegister* event_register_;
    friend class StrategyBase;
};

} // namespace StrategyStudio
} // namespace RCM

#endif
