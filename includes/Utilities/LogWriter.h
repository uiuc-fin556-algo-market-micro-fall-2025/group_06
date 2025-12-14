/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/LogWriter.h
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

#pragma once

#ifndef _STRATEGY_STUDIO_UTILITIES_LOGWRITER_H_
#define _STRATEGY_STUDIO_UTILITIES_LOGWRITER_H_

#include "CUThread.h"
#include "LogTypes.h"
#include "Mutex.h"
#include "SafeQueue.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <set>
#include <vector>
#include <cstdio>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

class LogEntry;

/**
 * LogWriter writes LogEntry objects to files
 * Closes its FILE*s on destruction
 */
class LogWriter : public CUThread {
public:
    typedef std::vector<FILE*> Files;
    typedef Files::iterator FilesIter;
    typedef Files::const_iterator FilesConstIter;

    typedef std::vector<std::string> Names;

public:
    LogWriter();
    ~LogWriter();

    void Shutdown();

    int AddFile(const std::string& filename, bool appendMode = true, bool binaryMode = false);

    int AddConsole();

    bool RemoveFile(int fileID);

    void Reset(int fileID, bool appendMode = true, bool binaryMode = false);

    bool Log(LogEntry* le);

public:
    static boost::gregorian::date GetFileCreationDate(const std::string& filename, bool binaryFile = false);

private:
    LogWriter(const LogWriter &);
    LogWriter& operator=(const LogWriter &);

    virtual bool _ThreadBody();

    void _Stop();

private:
    SafeQueue<LogEntry*> m_msgQueue;
    Files m_files;
    Names m_filenames;
    volatile bool m_bShutdown;
    Mutex m_filesMutex;
};

typedef boost::shared_ptr<LogWriter> LogWriterPtr;

/*---------------------------------------------------------------------*/

/**
 * LogEntry represents a message to log
 */
class LogEntry {
public:
    typedef std::set<int> Files;
    typedef Files::iterator FilesIter;
    typedef Files::const_iterator FilesConstIter;

public:
    LogEntry(const Files& files, Utilities::LogLevel level);
    LogEntry(const Files& files, Utilities::LogLevel level, const char* msg);
    ~LogEntry();

public:
    const Files& files() const { return m_files; }

    void set_reset(bool flag) { m_bReset = flag; }
    bool is_reset() const { return m_bReset; }

    void set_remove_files(bool flag) { m_bRemoveFiles = flag; }
    bool is_remove_files() const { return m_bRemoveFiles; }

public:
    void MoveConstructMessage(char** input_msg);
    const char* ToString();

private:
    LogEntry();
    LogEntry(const LogEntry& le);
    LogEntry& operator=(const LogEntry& le);


private:
    Files m_files;
    char* m_msg;
    boost::posix_time::ptime m_ts;
    Utilities::LogLevel m_level;
    bool m_isFormatted;
    bool m_bReset;
    bool m_bRemoveFiles;
};

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif
