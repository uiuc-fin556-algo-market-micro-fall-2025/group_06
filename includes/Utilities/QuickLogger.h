/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/QuickLogger.h
*     Last Update: 2011/02/08 13:10:00
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

#ifndef _STRATEGY_STUDIO_UTILITIES_QUICKLOGGER_H_
#define _STRATEGY_STUDIO_UTILITIES_QUICKLOGGER_H_

// disable warning about deprecated _snprintf and strncpy
#pragma warning (push)
#pragma warning ( disable:4996 )

#include "LogTypes.h"
#include "LogWriter.h"
#include "Mutex.h"
#include "Cast.h"
#include <boost/shared_ptr.hpp>

#include <string>
#include <set>
#include <cstdio>
#include <cstdarg>
#include <limits>

#ifdef _WIN32
    #define DLL_LOCAL
#else
    #define _snprintf snprintf
    #define DLL_LOCAL __attribute__ ((visibility ("hidden")))
#endif

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

class LogEntry;

/**
 * Strategy Studio's QuickLogger class, providing efficient multithreaded logging for the Strategy Server
 */
class QuickLogger {
public:
    typedef std::set<int> FileIDSet;
    typedef FileIDSet::iterator FileIDSetIter;
    typedef FileIDSet::const_iterator FileIDSetConstIter;

    typedef std::map<std::string, int> FileIDMap;
    typedef FileIDMap::iterator FileIDMapIter;
    typedef FileIDMap::const_iterator FileIDMapConstIter;

public:
    static const int ALL_FILES;

public:
    /**
     * Constructor which initializes a logger on a new thread
     */
    QuickLogger(const std::string& name,
                LogLevel level,
                int num_days_archive = ALL_FILES);

    virtual ~QuickLogger();

    /**
     * Gets the log level threshold for log messages sent to this logger. Any message
     * with a log level below the logger's level will be ignored
     */
    LogLevel level() const { return m_level; }
    void set_level(LogLevel level) { m_level = level; }

    /**
     * Gets the name of the logger
     */
    const std::string& name() const	{ return m_name; }

    /**
     * Adds a file to the list of output file destinations for the logger
     */
    virtual bool AddFile(const std::string& fileName, bool appendMode = true, bool binaryMode = false);

    /**
     * Tells the logger to output log messages to the Strategy Server's console
     */
    bool AddConsole();

    /**
     * Removes a file from the list of output file destinations for this logger
     */
    bool RemoveFile(const std::string& filename);

    /**
     * Truncates and reopens each of the files associated with the QuickLogger
     */
    void Reset();

    /**
     * Logs a message
     * @param level A LogLevel which indicates the severity of the log
     * @param msg The message to log
     */
    void Log(LogLevel level, const char* msg);

     /**
     * Overload of above that takes a string. Actually calls the above.
     */
    void Log(LogLevel level, const std::string& msg);

    /**
     * Logs a message
     *
     * @param level A LogLevel which indicates the severity of the log
     * @param msg The base message to log
     * @param val A number or value associated with the base msg, allows you to efficiently log name/value pairs without performing your own string concatenation
     */
    template<class T>
    void Log(LogLevel level, const char* msg, const T& val);

    /**
     * Logs a message using a printf() style formatted string
     *
     * @param level A LogLevel which indicates the severity of the log
     * @param formatted_message The base message containing formatting tags
     */
    void LogFormatted(LogLevel level, const char* formattedMessage, ...);

    void LogFormatted(LogLevel level,
                      const char* formattedMessage,
                      va_list argp1,
                      va_list argp2);

    void SetDaysToArchive(int num_days_archive);

    void CleanOldFiles();

    bool IsLevelLoggable(LogLevel level) const { return level >= m_level; }

private:
    QuickLogger(const QuickLogger&);
    QuickLogger& operator=(const QuickLogger&);

protected:
    bool InsertFileID(const std::string& filename, int fileID);
    bool IsLoggable(LogLevel level) { return !m_bShutdown && (level >= m_level); }

    const std::string m_name;
    LogLevel m_level;
    FileIDSet m_fileIDSet;
    FileIDMap m_fileIDMap;
    volatile bool m_bShutdown;
    Mutex m_filesMutex;
    LogWriterPtr m_logWriter;
    int m_num_days_archive;

    DLL_LOCAL static LogWriterPtr s_logWriter;
    DLL_LOCAL static Mutex s_logWriterMutex;
};

typedef boost::shared_ptr<QuickLogger> QuickLoggerPtr;

template<class T>
inline void QuickLogger::Log(LogLevel level, const char* msg, const T& val)
{
    if (IsLoggable(level)) {
        std::string val_str(Cast<std::string>()(val));

        std::size_t len = strlen(msg);
        std::size_t full_len = len + val_str.length();
        char* full_msg = new char[full_len + 1];
        if (!full_msg)
            return;

        // snprintf returns negative on error
        int chars_written = _snprintf(full_msg, full_len + 1, "%s%s", msg, val_str.c_str());
        if (chars_written < 0 ||
            static_cast<std::size_t>(chars_written) != full_len) {
            delete[] full_msg;
            return;
        }

        LogEntry* entry = new LogEntry(m_fileIDSet, level);
        entry->MoveConstructMessage(&full_msg);
        m_logWriter->Log(entry);
    }
}

// lexical cast is slower than snprintf for double so specialize this for doubles
template<>
inline void QuickLogger::Log<double>(LogLevel level, const char* msg, const double& val)
{
    if (IsLoggable(level)) {
        // Call snprintf once to determine length of buffer needed,
        //  then again to construct the final message
        int full_len = _snprintf(0, 0, "%s%g", msg, val);
        char* full_msg = new char[full_len + 1];
        if (!full_msg)
            return;

        if (_snprintf(full_msg, full_len + 1, "%s%g", msg, val) != full_len) {
            delete[] full_msg;
            return;
        }

        LogEntry* entry = new LogEntry(m_fileIDSet, level);
        entry->MoveConstructMessage(&full_msg);
        m_logWriter->Log(entry);
    }
}

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#pragma warning (pop)

#endif