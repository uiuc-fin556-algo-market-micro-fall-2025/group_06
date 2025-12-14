/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/ServerDebugLogBase.h
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

#ifndef _STRATEGY_STUDIO_LIB_SERVER_DEBUG_LOG_BASE_H_
#define _STRATEGY_STUDIO_LIB_SERVER_DEBUG_LOG_BASE_H_

#ifdef SERVER_MODE_DEBUG

#include <Utilities/LogTypes.h>
#include <cassert>

#define SERVER_DEBUG_LOG(msg) { if (s_serverDebugLogger && s_loggingEnabled) { (*s_serverDebugLogger)(Utilities::LOGLEVEL_DEBUG) << (msg); } }

namespace RCM {
namespace StrategyStudio {

namespace Utilities {
    class Logger;
} // namespace Utilities

/**
 * Provides a base interface and objects for using a main server debug logger
 */
class ServerDebugLogBase {
public:
    ServerDebugLogBase() {}

    virtual ~ServerDebugLogBase() = default;
    ServerDebugLogBase(const ServerDebugLogBase&) = default;
    ServerDebugLogBase& operator=(const ServerDebugLogBase&) = default;
    ServerDebugLogBase(ServerDebugLogBase&&) = default;
    ServerDebugLogBase& operator=(ServerDebugLogBase&&) = default;

    /**
     * Get the server debug logger
     */
    static Utilities::Logger* server_debug_logger() { return s_serverDebugLogger; }

    /**
     * Set the server debug logger
     */
    static void set_server_debug_logger(Utilities::Logger* logger) { s_serverDebugLogger = logger; }

    /**
     * Set the flag for enabling or disabling logging
     */
    static void server_logging_enabled(bool flag) { s_loggingEnabled = flag; }

    /**
     * Set to static server debug logger if not set
     */
    static void InitServerDebugLogger(Utilities::Logger& debugLogger);

protected:
    static Utilities::Logger* s_serverDebugLogger;
    static bool s_loggingEnabled;
};

} // namespace StrategyStudio
} // namespace RCM

#else

#define SERVER_DEBUG_LOG(msg) ;

namespace RCM {
namespace StrategyStudio {

/**
 * Empty base class used if server debug logging disabled
 */
class ServerDebugLogBase {
public:
    ServerDebugLogBase() {}

    virtual ~ServerDebugLogBase() = default;
    ServerDebugLogBase(const ServerDebugLogBase&) = default;
    ServerDebugLogBase& operator=(const ServerDebugLogBase&) = default;
    ServerDebugLogBase(ServerDebugLogBase&&) = default;
    ServerDebugLogBase& operator=(ServerDebugLogBase&&) = default;
};

} // namespace StrategyStudio
} // namespace RCM

#endif

#endif
