/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/DataSourceTypes.h
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

#ifndef _STRATEGY_STUDIO_LIB_DATA_SOURCE_TYPES_H_
#define _STRATEGY_STUDIO_LIB_DATA_SOURCE_TYPES_H_

#include <string>
#include <ostream>

namespace RCM {
namespace StrategyStudio {

/**
 * List the various states for a data source
 */
enum DataSourceState {
    DATA_SOURCE_STATE_UNKNOWN = 0,
    DATA_SOURCE_STATE_UNINITIALIZED = 1,
    DATA_SOURCE_STATE_INIT = 2,
    DATA_SOURCE_STATE_AWAITING_START = 3,
    DATA_SOURCE_STATE_STARTED_REPLAY = 4,
    DATA_SOURCE_STATE_STARTED_LIVE = 5,
    DATA_SOURCE_STATE_STOPPED = 6
};

/**
 * Enumerates types of data we can subscribe to
 */
enum DataSubscriptionType {
    DATA_SUBSCRIPTION_TYPE_UNKNOWN = 0,
    DATA_SUBSCRIPTION_TYPE_MARKET_DATA = 1, // subscription to market data
    DATA_SUBSCRIPTION_TYPE_NEWS = 2, // subscription to news data
    DATA_SUBSCRIPTION_TYPE_SENTIMENT = 3, // subscription to sentiment data
    DATA_SUBSCRIPTION_TYPE_EXTERNAL_ORDERS = 4 // subscription to orders submitted via a 3rd party platform
};

/**
 * Enumerates possible states for data subscriptions
 */
enum DataSubscriptionState {
    DATA_SUBSCRIPTION_UNKNOWN = 0,
    DATA_SUBSCRIPTION_UNINITIALIZED = 1, // subscription uninitialized
    DATA_SUBSCRIPTION_SUBSCRIPTION_PENDING = 2, // subscription known by adapter but not subscribed yet
    DATA_SUBSCRIPTION_SUBSCRIBED = 3, // subscription suceeded and active
    DATA_SUBSCRIPTION_REJECTED = 4, // subscription could not be establishd
    DATA_SUBSCRIPTION_UNSUBSCRIBED = 5, // subscription has been unsubscribed
    DATA_SUBSCRIPTION_WARNING = 6, // subscription is reporting a problem but continuing to send data
    DATA_SUBSCRIPTION_FAILURE = 7, // subscription has failed with an error
};

/**
 * Enumerates possible reasons for change in data subscription state
 */
enum DataSubscriptionAlert {
    DATA_SUBSCRIPTION_ALERT_UNKNOWN = 0,
    DATA_SUBSCRIPTION_ALERT_CREDENTIALS_FAILED = 1,
    DATA_SUBSCRIPTION_ALERT_CONNECTION_LOST = 2,
    DATA_SUBSCRIPTION_ALERT_INSTRUMENT_UNKNOWN = 3,
    DATA_SUBSCRIPTION_ALERT_NO_DATA = 4,
    DATA_SUBSCRIPTION_ALERT_DATA_GAP_DETECTED = 5,
    DATA_SUBSCRIPTION_ALERT_SLOW_DATA = 6,
    DATA_SUBSCRIPTION_ALERT_CONNECTED = 7
};

class DataSourceTypesUtils {
public:
    static const std::string& ToString(DataSourceState source);
    static const std::string& ToString(DataSubscriptionState source);
};

inline std::string DataSubscriptionStateToString(DataSubscriptionState state)
{
    switch (state) {
        case DATA_SUBSCRIPTION_UNINITIALIZED:
            return "DATA_SUBSCRIPTION_UNINITIALIZED";
        case DATA_SUBSCRIPTION_SUBSCRIPTION_PENDING:
            return "DATA_SUBSCRIPTION_SUBSCRIPTION_PENDING";
        case DATA_SUBSCRIPTION_SUBSCRIBED:
            return "DATA_SUBSCRIPTION_SUBSCRIBED";
        case DATA_SUBSCRIPTION_REJECTED:
            return "DATA_SUBSCRIPTION_REJECTED";
        case DATA_SUBSCRIPTION_UNSUBSCRIBED:
            return "DATA_SUBSCRIPTION_UNSUBSCRIBED";
        case DATA_SUBSCRIPTION_WARNING:
            return "DATA_SUBSCRIPTION_WARNING";
        case DATA_SUBSCRIPTION_FAILURE:
            return "DATA_SUBSCRIPTION_FAILURE";
        case DATA_SUBSCRIPTION_UNKNOWN: // fall-through
        default:
            return "DATA_SUBSCRIPTION_UNKNOWN";
    }
}

inline std::string DataSubscriptionTypeToString(DataSubscriptionType type)
{
    switch (type) {
        case DATA_SUBSCRIPTION_TYPE_MARKET_DATA:
            return "DATA_SUBSCRIPTION_TYPE_MARKET_DATA";
        case DATA_SUBSCRIPTION_TYPE_NEWS:
            return "DATA_SUBSCRIPTION_TYPE_NEWS";
        case DATA_SUBSCRIPTION_TYPE_SENTIMENT:
            return "DATA_SUBSCRIPTION_TYPE_SENTIMENT";
        case DATA_SUBSCRIPTION_TYPE_EXTERNAL_ORDERS:
            return "DATA_SUBSCRIPTION_TYPE_EXTERNAL_ORDERS";
        case DATA_SUBSCRIPTION_TYPE_UNKNOWN: // fall-through
        default:
            return "DATA_SUBSCRIPTION_TYPE_UNKNOWN";
    }
}

inline std::string DataSubscriptionAlertToString(DataSubscriptionAlert state)
{
    switch (state) {
        case DATA_SUBSCRIPTION_ALERT_CREDENTIALS_FAILED:
            return "DATA_SUBSCRIPTION_ALERT_CREDENTIALS_FAILED";
        case DATA_SUBSCRIPTION_ALERT_CONNECTION_LOST:
            return "DATA_SUBSCRIPTION_ALERT_CONNECTION_LOST";
        case DATA_SUBSCRIPTION_ALERT_INSTRUMENT_UNKNOWN:
            return "DATA_SUBSCRIPTION_ALERT_INSTRUMENT_UNKNOWN";
        case DATA_SUBSCRIPTION_ALERT_NO_DATA:
            return "DATA_SUBSCRIPTION_ALERT_NO_DATA";
        case DATA_SUBSCRIPTION_ALERT_DATA_GAP_DETECTED:
            return "DATA_SUBSCRIPTION_ALERT_DATA_GAP_DETECTED";
        case DATA_SUBSCRIPTION_ALERT_SLOW_DATA:
            return "DATA_SUBSCRIPTION_ALERT_SLOW_DATA";
        case DATA_SUBSCRIPTION_ALERT_CONNECTED:
            return "DATA_SUBSCRIPTION_ALERT_CONNECTED";
        case DATA_SUBSCRIPTION_ALERT_UNKNOWN: // fall-through
        default:
            return "DATA_SUBSCRIPTION_ALERT_UNKNOWN";
    }
}

inline std::ostream& operator<<(std::ostream& out, DataSubscriptionState state)
{
    out << DataSubscriptionStateToString(state);
    return out;
}

inline std::ostream& operator<<(std::ostream& out, DataSubscriptionAlert alert)
{
    out << DataSubscriptionAlertToString(alert);
    return out;
}

} // namespace StrategyStudio
} // namespace RCM

#endif
