/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyLogger.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGYLOGGER_H_
#define _STRATEGY_STUDIO_LIB_STRATEGYLOGGER_H_

#include <Utilities/QuickLogger.h>
#include <string>
#include <sstream>
#include <memory>
#include "DataTypes.h"

namespace RCM {
namespace StrategyStudio {

class IClientMsgTransport;
class ITimeKeeper;

/**
 * Provides a default logger for the strategy. This will log to a log file associated with the Processor the strategy
 * gets assigned for. We do not create a log file per strategy, because it may be important to look at the interaction between the
 * multiple strategies assigned to a Processor, and because in the presence of many strategy instances, a log per instance could overrun
 * the c runtime library or the OS's file handle limit.
 */
class StrategyLogger {
public:
    StrategyLogger(const Utilities::QuickLoggerPtr& logger, StrategyID strategyID, const std::string& strategyName);

    StrategyLogger(const Utilities::QuickLoggerPtr& logger,
                   StrategyID strategyID,
                   const std::string& strategyName,
                   ITimeKeeper* timeKeeper,
                   IClientMsgTransport* transport);

    /**
     * Logs a message
     *
     * @param level A LogLevel which indicates the severity of the log
     * @param msg The message to log
     */
    void Log(Utilities::LogLevel level, const char* msg);

    /**
     * Logs a message
     *
     * @param level A LogLevel which indicates the severity of the log
     * @param msg The message to log
     */
    void Log(Utilities::LogLevel level, const std::string& msg);

    /**
     * Logs a message
     *
     * @param level A LogLevel which indicates severity of the log
     * @param msg The message to log
     * @param logToClient Indicates whether message should be sent to Strategy Manager client as well as the log file
     */
    void Log(Utilities::LogLevel level, const char* msg, bool logToClient);

    /**
     * Logs a message
     *
     * @param level A LogLevel which indicates the severity of the log
     * @param msg The base message to log
     * @param val A number or value associated with the base msg, allows you to efficiently log name/value pairs without performing your own string concatenation
     */
    template<class T>
    void Log(Utilities::LogLevel level, const char* msg, const T& val);

    /**
     * Logs a message
     *
     * @param level A LogLevel which indicates the severity of the log
     * @param msg The base message to log
     * @param val A number or value associated with the base msg, allows you to efficiently log name/value pairs without performing your own string concatenation
     * @param logToClient Indicates whether message should be sent to Strategy Manager client as well as the log file
     */
    template<class T>
    void Log(Utilities::LogLevel level, const char* msg, const T& val, bool logToClient);

    /**
     * Logs a message to the Strategy Manager client
     *
     * @param level A LogLevel which indicates the severity of the log
     * @param msg The message to log
     */
    void LogToClient(Utilities::LogLevel level, const std::string & msg);

    /**
     * Logs a message to the Strategy Manager client
     *
     * @param level A LogLevel which indicates the severity of the log
     * @param msg The message to log
     */
    void LogToClient(Utilities::LogLevel level, const char* msg);

    /**
     * Logs a message using a printf() style formatted string
     *
     * @param level A LogLevel which indicates the severity of the log
     * @param formattedMessage The base message containing formatting tags
     */
    void LogFormatted(Utilities::LogLevel level, const char* formattedMessage, ...);

    bool IsLevelLoggable(Utilities::LogLevel level) const { return logger_->IsLevelLoggable(level); }

    void set_timekeeper(ITimeKeeper* timeKeeper);

    void set_transport(IClientMsgTransport* transport);

    void set_logger(Utilities::QuickLoggerPtr logger);

private:
    StrategyLogger &operator=(const StrategyLogger &);

    Utilities::QuickLoggerPtr logger_;
    StrategyID strategy_id_;
    const std::string strategy_name_;
    IClientMsgTransport* client_msg_transport_;
    ITimeKeeper* time_keeper_;
};

template<class T>
void StrategyLogger::Log(Utilities::LogLevel level, const char* msg, const T& val)
{
    if (!logger_->IsLevelLoggable(level)) return;
    std::stringstream ss;
    ss << strategy_name_ << " " << msg;
    logger_->Log(level, ss.str().c_str(), val);
}

template<class T>
void StrategyLogger::Log(Utilities::LogLevel level, const char* msg, const T& val, bool logToClient)
{
    static const size_t STRATEGY_LOGGER_BUF_SIZE = 300;

    if (logToClient) {
        char buf[STRATEGY_LOGGER_BUF_SIZE]; // fixed size assumption
        std::size_t len = strnlen(msg, STRATEGY_LOGGER_BUF_SIZE - 1);

        std::strncpy(buf, msg, len);
        std::strncpy(buf + len, Utilities::Cast<std::string>()(val).c_str(), STRATEGY_LOGGER_BUF_SIZE -1 - len);

        LogToClient(level, buf);
    }

    if (!logger_->IsLevelLoggable(level)) return;

    std::stringstream ss;
    ss << strategy_name_ << " " << msg;
    logger_->Log(level, ss.str().c_str(), val);
}

} // namespace StrategyStudio
} // namespace RCM

#endif
