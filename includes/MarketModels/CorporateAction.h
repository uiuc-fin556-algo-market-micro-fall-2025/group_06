/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/CorporateAction.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_CORPORATE_ACTION_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_CORPORATE_ACTION_H_

#include "Instrument.h"
#include "CurrencyTypes.h"
#include <string>
#include <Utilities/TimeType.h>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

enum CorporateActionType {
    CORP_ACTION_TYPE_UNKNOWN,
    CORP_ACTION_TYPE_SYMBOL_CHANGE,
    CORP_ACTION_TYPE_CASH_DIVIDEND,
    CORP_ACTION_TYPE_STOCK_DIVIDEND,
    CORP_ACTION_TYPE_STOCK_SPLIT,
    CORP_ACTION_TYPE_OTHER_CURRENCY_DIVIDEND,
    CORP_ACTION_TYPE_SPINOFF
};

class CorporateActionTypeUtils {
public:
    static std::string ToString(CorporateActionType type);
    static CorporateActionType FromString(const std::string& corporateActionType);
};

/**
 * Corporate Action Information
 * Different corporate actions types use different fields
 */
struct CorporateAction {
public:
    /**
     * Default corporate action
     */
    CorporateAction();

    /**
     * Basic constructor
     */
    CorporateAction(const Instrument* instrument, CorporateActionType type);

    /**
     * Creates a cash dividend corporate action
     */
    CorporateAction(const Instrument* instrument, double cash);

    /**
     * Creates a cash dividend corporate action
     */
    CorporateAction(const Instrument* instrument, double cash, Utilities::DateType activityDate);

    /**
     * Creates a cash dividend corporate action in another currency
     */
    CorporateAction(const Instrument* instrument, double cash, CurrencyType currencyType);

    /**
     * Creates a cash dividend corporate action in another currency
     */
    CorporateAction(const Instrument* instrument, double cash, CurrencyType currencyType, Utilities::DateType activityDate);

    /**
     * Creates a cash dividend or split dividend
     * This constructor expects a type of either cash dividend or stock dividend
     */
    CorporateAction(const Instrument* instrument,
                    CorporateActionType type,
                    double additiveFactor,
                    double multiplicativeFactory);

    /**
     * Creates a cash dividend or split dividend
     * This constructor expects a type of either cash dividend or stock dividend
     */
    CorporateAction(const Instrument* instrument,
                    CorporateActionType type,
                    double additiveFactor,
                    double multiplicativeFactory,
                    Utilities::DateType activityDate);

    /**
     * Creates a split corporate action
     */
    CorporateAction(const Instrument* instrument, const std::string& oldSymbol, const std::string& newSymbol);

    /**
     * Creates a split corporate action
     */
    CorporateAction(const Instrument* instrument,
                    const std::string& oldSymbol,
                    const std::string& newSymbol,
                    Utilities::DateType activityDate);

    /**
     * Complete constructor type
     */
    CorporateAction(const Instrument* instrument,
                    CorporateActionType corpActionType,
                    const std::string& oldSymbol,
                    const std::string& newSymbol,
                    double fAdditiveFactor,
                    double fMultiplicativeFactor,
                    CurrencyType currencyType,
                    Utilities::DateType activityDate);

    CorporateAction(const CorporateAction& corpAction);

    CorporateAction& operator=(const CorporateAction& corpAction);

public:
    const Instrument* instrument;
    CorporateActionType corporate_action_type;
    std::string old_symbol;
    std::string new_symbol;
    double additive_factor;
    double multiplicative_factor;
    CurrencyType currency_type;
    Utilities::DateType activity_date;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif