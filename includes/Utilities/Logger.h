/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/Logger.h
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

#ifndef _STRATEGY_STUDIO_UTILITIES_LOGGER_H_
#define _STRATEGY_STUDIO_UTILITIES_LOGGER_H_

#include "Appender.h"
#include "Mutex.h"
#include "LogTypes.h"
#include <vector>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

class Appender;
class Layout;
class LoggerProxy;

//typedef std::unique_ptr<LoggerProxy> LoggerAutoPtr;

/**
 * Main logger class
 * Note: This class deletes any associated appenders and layouts
 */
typedef std::unique_ptr<LoggerProxy> LoggerAutoPtr;

class Logger {
public:
    typedef std::vector<Appender*> Appenders;
    typedef Appenders::iterator AppendersIter;
    typedef Appenders::const_iterator AppendersConstIter;

public:

    Logger(const std::string& name,
           LogLevel level,
           bool needsLock = true);

    Logger(const std::string& name,
           LogLevel level,
           Layout* layout,
           bool needsLock = true);

    ~Logger();

    bool GetFlushOutputFlag() const;

    /**
     * Not to be used after initialization.  Not thread safe.
     */
    void SetFlushOutputFlag(bool flag);

    LogLevel GetLevel() const;

    const char* GetName() const;

    bool IsLockable() const;

    bool IsLocked() const;

    /**
     * Adds the given appender to the logger
     * @returns true if appender added, false if given appender is already present
     */
    bool AddAppender(Appender* appender);

    /**
     * Remove the given appender from the logger
     * NOTE: Not to be used in a multi-threaded context
     * @returns true if appender was removed, fails if given appender not present
     */
    bool RemoveAppender(Appender* appender);

    /**
     * Locks the associated mutex
     * @throws If logger is already locked
     */
    void Lock();

    /**
     * Unlock the associated mutex
     * @throws If logger is not locked
     */
    void Unlock();

    LoggerAutoPtr operator()(LogLevel statementLevel);

private:
    void Prepend();
    void PostPend();

    template<typename T>
    void Log(const T& datum);

private:
    const std::string m_name;
    LogLevel m_level;
    Appenders m_appenders;
    Layout* m_layout;
    volatile bool m_bNeedsLock;
    volatile bool m_bAttemptingLock;
    mutable bool m_bIsLocked;
    bool m_bFlushOutput;
    Mutex m_localMutex;

    static Mutex s_loggerMutex;

    friend class LoggerProxy;
};

/*---------------------------------------------------------------------*/

class LoggerPtr {
public:
    LoggerPtr(Logger* logger);
    Logger* operator*() const;
    Logger* operator->() const;

private:
    Logger* m_logger;
};

/*---------------------------------------------------------------------*/

struct ProxyCountHolder {
public:

    ProxyCountHolder()
    {
        s_proxyCount = 0;
    }

    volatile int s_proxyCount;
};

/*---------------------------------------------------------------------*/

class LoggerProxy {
public:
    LoggerProxy(Logger* logger, bool isActive, bool needsLock);
    ~LoggerProxy();

    bool IsActive() const { return m_bIsActive; }

    template<typename T>
    LoggerProxy& operator<<(const T& datum);

private:
    LoggerProxy(const LoggerProxy& proxy);
    LoggerProxy& operator=(const LoggerProxy& proxy);

private:
    Logger* m_logger;
    bool m_bIsActive;
    bool m_bNeedsLock;

    static ProxyCountHolder s_proxyCountHolder; // counts how many logger proxies are created
};

//typedef boost::shared_ptr<Logger> LoggerPtr;

/*---------------------------------------------------------------------*/

inline const char* ToString(LogLevel logLevel)
{
    const char* const debugName = "DEBUG";
    const char* const infoName =  "INFO";
    const char* const warnName =  "WARN";
    const char* const errorName = "ERROR";
    const char* const fatalName = "FATAL";

    switch (logLevel) {
        case LOGLEVEL_INFO:
            return infoName;
        case LOGLEVEL_WARN:
            return warnName;
        case LOGLEVEL_ERROR:
            return errorName;
        case LOGLEVEL_FATAL:
            return fatalName;
        case LOGLEVEL_DEBUG: // fall-through
        default:
            return debugName;
    }
}

/*---------------------------------------------------------------------*/

template<typename T>
inline void Logger::Log(const T& datum)
{
    for (AppendersIter it = m_appenders.begin(); it != m_appenders.end(); ++it)	{
        std::ostream& stream = (*it)->GetStream();
        stream << datum;
    }
}

/*---------------------------------------------------------------------*/

template<typename T>
inline LoggerProxy& LoggerProxy::operator<<(const T& datum)
{
    if (m_bIsActive) {
        m_logger->Log(datum);
    }

    return *this;
}

template<typename T>
inline LoggerAutoPtr& operator<<(LoggerAutoPtr& pLogger, const T& datum)
{
    *pLogger << datum;
    return pLogger;
}

template<typename T>
inline LoggerAutoPtr operator<<(LoggerAutoPtr pLogger, const T& datum)
{
    *pLogger << datum;
    return pLogger;
}

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif
