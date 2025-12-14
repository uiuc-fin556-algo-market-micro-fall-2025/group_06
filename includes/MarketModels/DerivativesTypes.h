/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/DerivativesTypes.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_DERIVATIVE_TYPES_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_DERIVATIVE_TYPES_H_

#include <Utilities/StdTypes.h>
#include <string>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

typedef StrategyStudioInt64 OrderID;

/**
 * Lists the supported delivery/settlement types
 */
enum DeliveryType {
    DELIVERY_TYPE_UNKNOWN = 0,
    DELIVERY_TYPE_CASH = 1,
    DELIVERY_TYPE_PHYSICAL = 2
};

/**
 * Converts from a character abbreviation to a DeliveryType
 */
inline DeliveryType CharToDeliveryType(char type) 
{
    if (type == 'C')
        return DELIVERY_TYPE_CASH;
    else if (type == 'P')
        return DELIVERY_TYPE_PHYSICAL;
    else
        return DELIVERY_TYPE_UNKNOWN;
}

/**
 * Converts from a DeliveryType to character
 */
inline std::string DeliveryTypeToChar(DeliveryType type) 
{
    if (type == DELIVERY_TYPE_CASH)
        return "C";
    else if (type == DELIVERY_TYPE_PHYSICAL)
        return "P";
    else
        return "";
}

/**
 * Lists the supported option payoff types
 */
enum OptionType {
    OPTION_TYPE_UNKNOWN = 0,
    OPTION_TYPE_CALL = 1,
    OPTION_TYPE_PUT = 2,
    OPTION_TYPE_BOTH = 3 // Not really an option type but specifies case where we want both call and put
};

/**
 * Converts from a character abbreviation to a OptionType
 */
inline OptionType CharToOptionType(char type) 
{
    if (type == 'C')
        return OPTION_TYPE_CALL;
    else if (type == 'P')
        return OPTION_TYPE_PUT;
    else
        return OPTION_TYPE_UNKNOWN;
}

/**
 * Converts from a OptionType to character
 */
inline std::string OptionTypeToChar(OptionType type) 
{
    if (type == OPTION_TYPE_CALL)
        return "C";
    else if (type == OPTION_TYPE_PUT)
        return "P";
    else
        return "Unknown";
}

/**
 * Lists the supported exercise styles
 */
enum ExerciseStyle {
    EXERCISE_STYLE_UNKNOWN = 0,
    EXERCISE_STYLE_EUROPEAN = 1,
    EXERCISE_STYLE_AMERICAN = 2
};

/**
 * Converts from a character abbreviation to an ExerciseStyle
 */
inline ExerciseStyle CharToExcerciseStyle(char style) 
{
    if (style == 'A')
        return EXERCISE_STYLE_AMERICAN;
    else if (style == 'E')
        return EXERCISE_STYLE_EUROPEAN;
    else
        return EXERCISE_STYLE_UNKNOWN;
}

/**
 * Converts from an ExerciseStyle to character abbreviation
 */
inline std::string ExcerciseStyleToChar(ExerciseStyle style) 
{
    if (style == EXERCISE_STYLE_AMERICAN)
        return "A";
    else if (style == EXERCISE_STYLE_EUROPEAN)
        return "E";
    else
        return "Unknown";
}

/**
 * Lists the day count conventions
 */
enum DayCountConvention {
    DAY_COUNT_CONVENTION_UNKNOWN = 0,
    DAY_COUNT_CONVENTION_ACTACT = 1,
    DAY_COUNT_CONVENTION_ACTAFB = 2,
    DAY_COUNT_CONVENTION_EU30360 = 3,
    DAY_COUNT_CONVENTION_US30360 = 4,
    DAY_COUNT_CONVENTION_ACT365 = 5,
    DAY_COUNT_CONVENTION_ACT360 = 6
};

/**
 * Converts from a number to DayCountConvention
 */
inline DayCountConvention ToDayCountConvention(int dayCountConv) 
{
    switch (dayCountConv) {
        case 1:
            return DAY_COUNT_CONVENTION_ACTACT;
        case 2:
            return DAY_COUNT_CONVENTION_ACTAFB;
        case 3:
            return DAY_COUNT_CONVENTION_EU30360;
        case 4:
            return DAY_COUNT_CONVENTION_US30360;
        case 5:
            return DAY_COUNT_CONVENTION_ACT365;
        case 6:
            return DAY_COUNT_CONVENTION_ACT360;
        default:
            return DAY_COUNT_CONVENTION_UNKNOWN;
    }
}

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif