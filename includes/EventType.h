/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/EventType.h
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

#ifndef _STRATEGY_STUDIO_LIB_EVENT_TYPE_INFO_H_
#define _STRATEGY_STUDIO_LIB_EVENT_TYPE_INFO_H_

#include <string>

namespace RCM {
namespace StrategyStudio {

enum EventType {
    EVENT_TYPE_UNKNOWN = 0,
    EVENT_TYPE_DATA_SOURCE = 1,
    EVENT_TYPE_ORDER_UPDATE = 2,
    EVENT_TYPE_CONTROL = 3,
    EVENT_TYPE_BROADCAST_CONTROL = 4,
    EVENT_TYPE_BAR = 5,
    EVENT_TYPE_CORP_ACTION = 6,
    EVENT_TYPE_APP_STATE_CHANGE = 7,
    EVENT_TYPE_MARKET_DATA = 8,
    EVENT_TYPE_REFERENCE_DATA_SOURCE = 9,
    EVENT_TYPE_NEWS_SOURCE = 10,
    EVENT_TYPE_SENTIMENT = 11,
    EVENT_TYPE_REPLAY = 12,
    EVENT_TYPE_ORDER_RESYNC = 13,
    EVENT_TYPE_SCHEDULED = 14,
    EVENT_TYPE_STRATEGY_NOTIFY = 15,
    EVENT_TYPE_STRATEGY_EXTERNAL_FILL_NOTIFY = 16,
    EVENT_TYPE_EXTERNAL_ORDER = 17,
    EVENT_TYPE_ACCOUNT_FEES_CHANGE = 18,
    EVENT_TYPE_TOPIC_SUBSCRIPTION = 19,
    EVENT_TYPE_INTERNAL_SERVER_NOTIFY = 20
};

enum ScheduledEventType {
    SCHEDULED_EVENT_TYPE_UNKNOWN = 0,
    SCHEDULED_EVENT_TYPE_SINGLE = 1,
    SCHEDULED_EVENT_TYPE_RECURRING = 2
};

class EventTypeUtils {
public:
    static const std::string& ToString(EventType type);
    static const std::string& ToString(ScheduledEventType type);
};

} // namespace StrategyStudio
} // namespace RCM

#endif 