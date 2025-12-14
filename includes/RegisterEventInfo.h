/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/RegisterEventInfo.h
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

#ifndef _STRATEGY_STUDIO_LIB_REGISTER_EVENT_INFO_H_
#define _STRATEGY_STUDIO_LIB_REGISTER_EVENT_INFO_H_

#include "DataTypes.h"
#include "DataSourceTypes.h"
#include "BarDataTypes.h"
#include "EventType.h"
#include "ICloneableEventCallback.h"
#include <FlashProtocol/FlashTypes.h>

#include <ostream>

namespace RCM {
namespace StrategyStudio {

struct RegisterMarketDataParams {
public:
    RegisterMarketDataParams();
    RegisterMarketDataParams(const std::string& sourceType, const SymbolTag& symbol);
    std::string ToString() const;
    static std::string BuildEventString(const SymbolTag& symbol);
    std::ostream& operator<<(std::ostream& out) const;

public:
    std::string source_type; // type of data source
    SymbolTag symbol;
};

struct RegisterNewsParams {
public:
    RegisterNewsParams();
    RegisterNewsParams(const std::string& sourceType, NewsType newsType, const SymbolTag& symbol);
    std::string ToString() const;
    static std::string BuildEventString(const std::string& sourceType, NewsType newsType, const SymbolTag& symbol);
    std::ostream& operator<<(std::ostream& out) const;

public:
    std::string source_type;    // type of data source
    NewsType news_type;
    SymbolTag symbol;
};

struct RegisterSentimentParams {
public:
    RegisterSentimentParams();
    RegisterSentimentParams(const std::string& sourceType, SentimentType sentimentType, const SymbolTag& symbol);
    std::string ToString() const;
    static std::string BuildEventString(const std::string& sourceType, SentimentType sentimentType, const SymbolTag& symbol);
    std::ostream& operator<<(std::ostream& out) const;

public:
    std::string source_type; // type of data source
    SentimentType sentiment_type;
    SymbolTag symbol;
};

struct RegisterOrderUpdateEventParams {
public:
    RegisterOrderUpdateEventParams();
    RegisterOrderUpdateEventParams(StrategyID target);
    std::string ToString() const;
    static std::string BuildEventString(StrategyID target);
    std::ostream& operator<<(std::ostream& out) const;

public:
    StrategyID target_strategy;
};

struct RegisterControlEventParams {
public:
    RegisterControlEventParams();
    RegisterControlEventParams(StrategyID target);
    std::string ToString() const;
    static std::string BuildEventString(StrategyID target);
    std::ostream& operator<<(std::ostream& out) const;

public:
    StrategyID target_strategy;
};

struct RegisterBroadcastControlEventParams {
public:
    RegisterBroadcastControlEventParams() {}
    std::string ToString() const;
    static std::string BuildEventString();
    std::ostream& operator<<(std::ostream& out) const;
};

struct RegisterBarsParams {
public:
    RegisterBarsParams();
    RegisterBarsParams(const std::string& source_type, const SymbolTag& symbol, const BarDefinition& barDefinition);
    std::string ToString() const;
    static std::string BuildEventString(const SymbolTag& symbol, const BarDefinition& barDefinition);
    std::ostream& operator<<(std::ostream& out) const;

public:
    std::string source_type; // type of data source
    SymbolTag symbol;
    BarDefinition bar_definition;
};

struct RegisterCorpActionEventParams {
public:
    RegisterCorpActionEventParams(const std::string& targetName);
    std::string ToString() const;
    static std::string BuildEventString(const std::string& targetName);
    std::ostream& operator<<(std::ostream& out) const;

public:
    std::string target_name;
};

struct RegisterScheduledEventParams {
public: /* named constructor idiom */
    static RegisterScheduledEventParams SingleTimedEventParam(const std::string& name,
                                                              TimeType fireTime,
                                                              bool fireIfPast = false);

    static RegisterScheduledEventParams RecurringTimedEventParam(const std::string& name,
                                                                 TimeType firstFireTime,
                                                                 TimeDuration interval,
                                                                 bool fireIfPast = false);

    static RegisterScheduledEventParams RecurringTimedEventParam(const std::string& name,
                                                                 TimeType firstFireTime,
                                                                 TimeType endFireTime,
                                                                 TimeDuration interval,
                                                                 bool fireIfPast = false);

public:
    std::string ToString() const;

    static std::string BuildEventString(const std::string& name,
                                        ScheduledEventType type,
                                        TimeType fireTime,
                                        TimeType endFireTime,
                                        TimeDuration interval,
                                        bool fireIfPast);

    std::ostream& operator<<(std::ostream& out) const;

private:

    RegisterScheduledEventParams();

    RegisterScheduledEventParams(const std::string& name,
                                 ScheduledEventType type,
                                 TimeType fireTime,
                                 TimeDuration interval,
                                 bool fireIfPast);

    RegisterScheduledEventParams(const std::string& name,
                                 ScheduledEventType type,
                                 TimeType fireTime,
                                 TimeType endFireTime,
                                 TimeDuration interval,
                                 bool fireIfPast);

public:
    std::string name;
    ScheduledEventType type;
    TimeType fire_time;
    TimeType end_fire_time;
    TimeDuration interval;
    bool fire_if_past;
};

struct RegisterExternalOrderParams {
public:
    RegisterExternalOrderParams() : symbol() {}
    RegisterExternalOrderParams(const std::string& symbol) : symbol(symbol) {}
    RegisterExternalOrderParams(const char* symbol) : symbol(symbol) {}

public:
    std::string ToString() const;

public:
    std::string symbol;
};

struct RegisterTopicSubscriptionParams {
public:
    RegisterTopicSubscriptionParams(const std::string& topic);
    std::string ToString() const;
    static std::string BuildEventString(const std::string& topic);
    std::ostream& operator<<(std::ostream& out) const;

public:
    std::string topic;
};

/**
 * Allows registration of a closure object that can maintain state and be triggered whenever
 * another event is fired.
 * Note: A deep copy of the passed object is made if not created
 */
struct RegisterClosureEventParams {
public:
    RegisterClosureEventParams(const ICloneableEventCallback& closureObject, Event* parentEvent);

    std::string ToString() const;

    static std::string BuildEventString(const ICloneableEventCallback& closureObject);

    std::ostream& operator<<(std::ostream& out) const;

public:
    const ICloneableEventCallback* closure_object;
    Event* parent_event;
};

} // namespace StrategyStudio
} // namespace RCM

#endif