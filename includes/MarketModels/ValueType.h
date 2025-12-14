/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/ValueType.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_VALUE_TYPE_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_VALUE_TYPE_H_

#include <time.h>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

enum ValueType {
    VALUE_TYPE_UNKNOWN = 0,
    VALUE_TYPE_INT = 1,
    VALUE_TYPE_DOUBLE = 2,
    VALUE_TYPE_CHAR = 3,
    VALUE_TYPE_STRING = 4,
    VALUE_TYPE_BOOL = 5
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif