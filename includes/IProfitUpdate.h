/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IProfitUpdate.h
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

#ifndef _STRATEGY_STUDIO_LIB_IPROFITUPDATE_H_
#define _STRATEGY_STUDIO_LIB_IPROFITUPDATE_H_

#include "MarketModels/CurrencyTypes.h"

namespace RCM {
namespace StrategyStudio {

struct FillInfo;
class PositionRecordBase;

/**
 * Interface used for notifying any listeners when profit is updated on this record
 */
class IProfitUpdate {
public:
    /**
     * Updates the realized pnl for an instrument when a new trade occrs
     */
    virtual void OnFill(const PositionRecordBase* portRecord,
                         const FillInfo& fillInfo,
                         double totalPL,
                         double rPL,
                         double oldTotalPL,
                         double oldrPL,
                         double notional,
                         double oldNotional,
                         double execCosts,
                         double exchangeRate,
                         double proceeds) = 0;

    /**
     * Updates the unrealized pnl for an isntrument on price change
     */
    virtual void UpdatePNL(PositionRecordBase* portRecord,
                           double totalPL,
                           double rPL,
                           double oldTotalPL,
                           double oldrPL,
                           double notional,
                           double oldNotional,
                           double exchangeRate,
                           double oldExchangeRate) = 0;

    /**
     * Updates a currency's position
     */
    virtual void UpdateCurrencyPos(PositionRecordBase* portRecord,
                           MarketModels::CurrencyType currency,
                           double position, double oldPosition,
                           double rate) = 0;
    /*
     * Returns the home currency type
     */
    virtual MarketModels::CurrencyType home_currency() const = 0;
};

} // namespace StrategyStudio
} // namespace RCM

#endif 