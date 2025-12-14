/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyIncludes.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGY_INCLUDES_H_
#define _STRATEGY_STUDIO_LIB_STRATEGY_INCLUDES_H_

/**
 * This files is a helper include file for including all headers needed for core strategy development
 */

#include "AllEventMsg.h"
#include "StrategyParam.h"
#include "StrategyParamCollection.h"
#include "IFillTracker.h"
#include "IOrderTracker.h"
#include "IPortfolioTracker.h"
#include "IPositionRecord.h"
#include "IRiskConstraint.h"
#include "IRiskProfile.h"
#include "IStrategyGraphs.h"
#include "ITradeTracker.h"
#include "RegisterEventInfo.h"
#include "StrategyEventRegister.h"
#include "StrategyRuntimeEventRegister.h"
#include "StrategyInitInfo.h"
#include "StrategyLogger.h"
#include "StrategyTradeActions.h"
#include <MarketModels/AccountFees.h>
#include <MarketModels/IPriceLevel.h>
#include <Utilities/ParseConfig.h>
#include <Utilities/IParams.h>
#include <Utilities/StrategyStudioException.h>
#include <Utilities/QuickLogger.h>

#include <boost/unordered_set.hpp>

using RCM::StrategyStudio::MarketModels::AccountFees;
using RCM::StrategyStudio::Utilities::ParseConfig;
using RCM::StrategyStudio::Utilities::IParams;
using RCM::StrategyStudio::Utilities::StrategyStudioException;

#endif
