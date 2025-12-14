/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/LogTypes.h
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

#ifndef _STRATEGY_STUDIO_UTILITIES_LOG_TYPES_H_
#define _STRATEGY_STUDIO_UTILITIES_LOG_TYPES_H_

#define LOGDEBUG RCM::StrategyStudio::Utilities::LOGLEVEL_DEBUG
#define LOGINFO  RCM::StrategyStudio::Utilities::LOGLEVEL_INFO
#define LOGWARN  RCM::StrategyStudio::Utilities::LOGLEVEL_WARN
#define LOGERROR RCM::StrategyStudio::Utilities::LOGLEVEL_ERROR
#define LOGFATAL RCM::StrategyStudio::Utilities::LOGLEVEL_FATAL
#define LEND	 RCM::StrategyStudio::Utilities::LogEnd()

#include <boost/algorithm/string.hpp>
#include <string>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

enum LogLevel {
    LOGLEVEL_DEBUG = 0,
    LOGLEVEL_INFO = 1,
    LOGLEVEL_WARN = 2,
    LOGLEVEL_ERROR = 3,
    LOGLEVEL_FATAL = 4
};

inline const char* LogLevelToCStr(LogLevel level)
{
    const char* const strDebug = "DEBUG";
    const char* const strInfo = "INFO";
    const char* const strWarn = "WARN";
    const char* const strError = "ERROR";
    const char* const strFatal = "FATAL";

    switch (level) {
        case LOGLEVEL_INFO:
            return strInfo;
        case LOGLEVEL_WARN:
            return strWarn;
        case LOGLEVEL_ERROR:
            return strError;
        case LOGLEVEL_FATAL:
            return strFatal;
        case LOGLEVEL_DEBUG: // fall-through
        default:
            return strDebug;
    }
}

inline LogLevel LogLevelFromString(const std::string& logLevel)
{
    std::string level = boost::to_upper_copy(logLevel);
    if (level == "DEBUG")
        return LOGLEVEL_DEBUG;
    else if (level == "INFO")
        return LOGLEVEL_INFO;
    else if (level == "WARN")
        return LOGLEVEL_WARN;
    else if (level == "ERROR")
        return LOGLEVEL_ERROR;
    else if (level == "FATAL")
        return LOGLEVEL_FATAL;
    else
        return LOGLEVEL_DEBUG;
}

/**
 * Used as the terminating character in a log statement
 */
class LogEnd {
public:
    LogEnd() {}
};

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif
