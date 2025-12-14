/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/CorporateActionEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_CORPORATE_ACTION_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_CORPORATE_ACTION_EVENT_MSG_H_

#include "DataTypes.h"
#include "EventMsg.h"

namespace RCM {
namespace StrategyStudio {

class Event;

/**
 * CorporateActionEventMsg delivers events such as dividends, splits, and spin-offs
 */
class CorporateActionEventMsg : public EventMsg {
public:
    /**
     * Default empty market data msg
     */
    CorporateActionEventMsg(const MarketModels::CorporateAction& action, Event* event);

    std::string name() const;

    const CorporateAction& corporate_action() const { return corporate_action_; }

    CorporateActionType type() const { return corporate_action_.corporate_action_type; }

private:
    MarketModels::CorporateAction corporate_action_;
};

inline std::string CorporateActionEventMsg::name() const
{
    return "Corporate_Event_Type_" + MarketModels::CorporateActionTypeUtils::ToString(corporate_action_.corporate_action_type);
}

} // namespace StrategyStudio
} // namespace RCM

#endif