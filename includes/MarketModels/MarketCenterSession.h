/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/MarketCenterSession.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_MARKET_CENTER_SESSION_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_MARKET_CENTER_SESSION_H_

#include <Utilities/TimeType.h>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

using RCM::StrategyStudio::Utilities::TimeType;

/**
 * This class contains the open and close times for a particular market session
 */
class MarketCenterSession {
public:

    MarketCenterSession(TimeType startTime,
                        TimeType endTime);

    MarketCenterSession(const MarketCenterSession& marketCenterSession);
    ~MarketCenterSession();
    MarketCenterSession& operator=(const MarketCenterSession& marketCenterSession);

public:
    const TimeType& start_time() const { return start_time_; }
    const TimeType& end_time() const { return end_time_; }

private:
    TimeType start_time_;
    TimeType end_time_;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif
