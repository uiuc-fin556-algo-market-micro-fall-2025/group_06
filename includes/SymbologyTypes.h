/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/SymbologyTypes.h
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

#ifndef _STRATEGY_STUDIO_LIB_SYMBOLOGY_TYPES_H_
#define _STRATEGY_STUDIO_LIB_SYMBOLOGY_TYPES_H_

#include <string>
#include <utility>

namespace RCM {
namespace StrategyStudio {

/**
 * These types should be the same ids used in the reference db
 */
enum SymbologyType {
    SYMBOLOGY_TYPE_UNKNOWN = 0,
    SYMBOLOGY_TYPE_RCM = 1
};

inline std::string SymbologyTypeToString(SymbologyType type)
{
    switch (type) {
        case SYMBOLOGY_TYPE_RCM:
            return "SYMBOLOGY_TYPE_RCM";
        case SYMBOLOGY_TYPE_UNKNOWN: // fall-through
        default:
            return "SYMBOLOGY_TYPE_UNKNOWN";
    }
}

inline SymbologyType StringToSymbologyType(const std::string& type)
{
    if (type.compare("SYMBOLOGY_TYPE_RCM") == 0) {
        return SYMBOLOGY_TYPE_RCM;
    } else {
        return SYMBOLOGY_TYPE_UNKNOWN;
    }
}

typedef std::string SymbolTag;
typedef std::pair<SymbolTag, SymbologyType> LinkedSymbol;

} // namespace StrategyStudio
} // namespace RCM

#endif
