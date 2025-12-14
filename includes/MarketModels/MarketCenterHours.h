/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/MarketCenterHours.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_MARKET_CENTER_HOURS_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_MARKET_CENTER_HOURS_H_

#include "MarketCenterSession.h"
#include <Utilities/TimeType.h>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>

namespace RCM {
namespace StrategyStudio {

typedef boost::tuple<char, Utilities::TimeType, Utilities::TimeType, Utilities::TimeType> PreopenOpenCloseTuple;
typedef boost::unordered_map<std::string, PreopenOpenCloseTuple> HolidayMap;

namespace MarketModels {

using RCM::StrategyStudio::Utilities::TimeType;
using RCM::StrategyStudio::Utilities::DateType;

/**
 * Stores information about the hours of operation of a market center for a given calendar date
 */
class MarketCenterHours {
public:
    typedef std::vector<MarketCenterSession> MarketCenterSessions;
    typedef MarketCenterSessions::iterator MarketCenterSessionsIter;
    typedef MarketCenterSessions::const_iterator MarketCenterSessionsConstIter;

public:
    MarketCenterHours(DateType dt);
    MarketCenterHours(const MarketCenterHours& marketCenterHours);
    ~MarketCenterHours();
    MarketCenterHours& operator=(const MarketCenterHours& marketCenterHours);

public:
    DateType date() { return date_; }
    void set_date(const DateType& date);

    MarketCenterSessionsIter market_sessions_begin() { return market_center_sessions_.begin(); }
    MarketCenterSessionsIter market_sessions_end() { return market_center_sessions_.end(); }

    MarketCenterSessionsConstIter market_sessions_begin() const { return market_center_sessions_.begin(); }
    MarketCenterSessionsConstIter market_sessions_end() const { return market_center_sessions_.end(); }

private:
    MarketCenterSessions market_center_sessions_;
    DateType date_;
};

void SetHolidayMap(const HolidayMap* holidayMap);

/**
 * Get the US equity preopen utc time from given date
 */
TimeType USEquityPreOpenUTCTime(DateType dt);

/**
 * Get the US equity open utc time from given date
 */
TimeType USEquityOpenUTCTime(DateType dt);

/**
 * Get the US equity open utc time from given date
 */
TimeType USEquityCloseUTCTime(DateType dt);

/**
 * Get the US equity preopen utc time
 */
TimeType USEquityPreOpenUTCTime();

/**
 * Get the US equity open utc time
 */
TimeType USEquityOpenUTCTime();

/**
 * Get the US equity open utc time
 */
TimeType USEquityCloseUTCTime();

/**
 * Get the US equity preopen local time
 */
TimeType USEquityPreOpenCentralTime();

/**
 * Get the US equity open local time
 */
TimeType USEquityOpenCentralTime();

/**
 * Get the US equity open local time
 */
TimeType USEquityCloseCentralTime();

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif
