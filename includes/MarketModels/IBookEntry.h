/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/IBookEntry.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_IBOOK_ENTRY_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_IBOOK_ENTRY_H_

#include <Utilities/TimeType.h>
#include "MDBaseObject.h"

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * IBookEntry is an interface to be implemented by market data order books
 */
class IBookEntry : public MDBaseObject {
public:
    IBookEntry();
    IBookEntry(TimeType sourceTime, TimeType feedHandlerTime, TimeType adapterTime);

public: /* Public interface of IBookEntry */
    /**
     * Get the price
     */
    virtual double price() const = 0;

    /**
     * Get the size
     */
    virtual int size() const = 0;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif
