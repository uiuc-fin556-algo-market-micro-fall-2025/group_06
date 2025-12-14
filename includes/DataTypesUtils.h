/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/DataTypesUtils.h
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

#ifndef _STRATEGY_STUDIO_LIB_DATATYPES_UTILS_H_
#define _STRATEGY_STUDIO_LIB_DATATYPES_UTILS_H_

#include "DataTypes.h"
#include <time.h>
#include <vector>

namespace RCM {
namespace StrategyStudio {

class DataTypesUtils {
public:
    static std::string CommandTargetToString(CommandTarget target);
    static bool IsStoppedState(StrategyState state);
    static std::string AuctionTypeToString(AuctionType type);
    static std::string InstrumentStatusToString(InstrumentStatus code);
    static std::string MarketStateToString(MarketState state);
    static MarkingMode StringToMarkingMode(const std::string& mode);
    static std::string MarkingModeToString(MarkingMode mode);
    static std::string ShortSaleRestrictionIndicatorToString(ShortSaleRestrictionIndicator indic);
    static std::string NewsTypeToString(NewsType newsType);
    static std::string SentimentTypeToString(SentimentType sentimentType);
    static std::string SentimentSourceToString(SentimentSource sentimentSource);
    static StrategyRunMode StringToStrategyRunMode(const std::string& runMode);
    static std::string StrategyRunModeToString(StrategyRunMode runMode);
    static StrategyState StringToStrategyState(const std::string& state);
    static std::string StrategyStateToString(StrategyState state);
    static std::string ToString(StrategyState state);
    static StrategyLifecycleState StringToStrategyLifecycleState(const std::string& state);
    static std::string StrategyLifecycleStateToString(StrategyLifecycleState state);
    static std::string TradeActionResultToString(TradeActionResult result);
    static std::string TradeStateToString(TradeState state);
};

} // namespace StrategyStudio
} // namespace RCM

#endif
