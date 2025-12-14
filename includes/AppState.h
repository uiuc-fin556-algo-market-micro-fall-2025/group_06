/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/AppState.h
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

#ifndef _STRATEGY_STUDIO_LIB_APPSTATE_H_
#define _STRATEGY_STUDIO_LIB_APPSTATE_H_

#include <string>

namespace RCM {
namespace StrategyStudio {

enum AppState {
    STATE_UNINITIALIZED = 0,
    STATE_APP_INITIALIZED = 1,
    STATE_APP_STARTING = 2,
    STATE_WAITING_FOR_OPEN = 3,
    STATE_MARKET_OPENING = 4,
    STATE_RUNNING = 5,
    STATE_MARKET_CLOSING = 6 ,
    STATE_WAITING_FOR_SHUTDOWN = 7,
    STATE_APP_STOPPING = 8,
    STATE_SHUT_DOWN = 9,
    STATE_RESTARTING = 10
};

class AppStateUtils {
public:
    static const std::string& ToString(AppState state);
};

} // namespace StrategyStudio
} // namespace RCM

#endif
