/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/ExternalFillEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_EXTERNAL_FILL_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_EXTERNAL_FILL_EVENT_MSG_H_

#include "EventMsg.h"
#include "Event.h"
#include "FillInfo.h"
#include "TimePrioritizedEventMsg.h"
#include <Utilities/IParams.h>

namespace RCM {
namespace StrategyStudio {

/**
 * ExternalFillEventMsg delivers notifications of fills made outside of the Strategy Studio platform that
 * are being reported for attribution to a Strategy Studio strategy.
 */
class ExternalFillEventMsg : public TimePrioritizedEventMsg {
public:
    static const int TIMED_EVENT_PRIORITY = 4;

public:
    ExternalFillEventMsg(const std::string& symbol,
                         const FillInfo& fillInfo,
                         Event* event,
                         bool applyFees = true,
                         int expectedLegCount = 1,
                         const std::string& legId = "");

    std::string name() const;

    const std::string& symbol() const { return symbol_; }

    const FillInfo& fill() const { return fill_info_; }

    /**
     * the leg count on a best effort basis
     * complex orders will be 0 if unknown
     * all other orders default to 1
     */
    int expected_leg_count() const { return expected_leg_count_; }

    const std::string& leg_id() const { return leg_id_; }

    bool apply_fees() const { return apply_fees_; }

public: //from TimePrioritizedEventMsg
    bool Trigger(TimeType currTime);

private:
    FillInfo fill_info_;
    std::string symbol_;
    std::string leg_id_;
    int expected_leg_count_;
    bool apply_fees_;
};

inline std::string ExternalFillEventMsg::name() const
{
    return "ExternalFillEventMsg";
}

} // namespace StrategyStudio
} // namespace RCM

#endif
