/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/Cast.h
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

#ifndef _STRATEGY_STUDIO_UTILITIES_CAST_H_
#define _STRATEGY_STUDIO_UTILITIES_CAST_H_

#pragma warning (push)
#pragma warning( disable: 4996)

#include "StdTypes.h"
#include <boost/cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstdlib>
#include <sstream>
#include <string>
#include <string.h>
#include <stdio.h>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

/**
 * Cast is a helper class for performing casts between types.
 * It defaults to boost::lexical_cast but performs quicker
 * casts for value types.
 */
template<typename T>
class Cast {
public:
    Cast();
    ~Cast();
    T operator()(const T& datum) const;
};

template<typename T>
inline T Cast<T>::operator()(const T& datum) const
{
    return boost::lexical_cast<T>(datum);
}

template<>
class Cast<int> {
public:
    Cast() {}
    ~Cast() {}

    int operator()(const char* datum) const
    {
        return atoi(datum);
    }

    int operator()(char* const datum) const
    {
        return atoi(datum);
    }

    int operator()(const std::string& datum) const
    {
        return atoi(datum.c_str());
    }

    template<typename T>
    int operator()(const T& datum) const
    {
        return static_cast<int>(datum);
    }
};

template<>
class Cast<StrategyStudioInt64> {
public:
    Cast() {}
    ~Cast() {}

    StrategyStudioInt64 operator()(const char* datum) const
    {
        return atol(datum);
    }

    StrategyStudioInt64 operator()(char* const datum) const
    {
        return atol(datum);
    }

    StrategyStudioInt64 operator()(const std::string& datum) const
    {
        return atol(datum.c_str());
    }

    template<typename T>
    StrategyStudioInt64 operator()(const T& datum) const
    {
        return static_cast<StrategyStudioInt64>(datum);
    }
};

template<>
class Cast<unsigned> {
public:
    Cast() {}
    ~Cast() {}

    unsigned operator()(const char* datum) const
    {
        return static_cast<unsigned>(atoi(datum));
    }

    unsigned operator()(char* const datum) const
    {
        return static_cast<unsigned>(atoi(datum));
    }

    unsigned operator()(const std::string& datum) const
    {
        return static_cast<unsigned>(atoi(datum.c_str()));
    }

    template<typename T>
    unsigned operator()(const T& datum) const
    {
        return static_cast<int>(datum);
    }
};

template<>
class Cast<bool> {
public:
    Cast() {}
    ~Cast() {}

    bool operator()(const char* datum) const
    {
        if ( (strcmp(datum, "true") == 0) || (strcmp(datum, "TRUE") == 0) || (strcmp(datum, "1") == 0) )
            return true;
        else
            return false;
    }

    bool operator()(char* const datum) const
    {
        if ( (strcmp(datum, "true") == 0) || (strcmp(datum, "TRUE") == 0) || (strcmp(datum, "1") == 0) )
            return true;
        else
            return false;
    }

    bool operator()(const std::string& datum) const
    {
        if ( (datum.compare("true") == 0) || (datum.compare("TRUE") == 0) || (datum.compare("1") == 0) )
            return true;
        else
            return false;
    }

    bool operator()(int datum) const
    {
        if (datum == 1)
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator()(const T& datum) const
    {
        return static_cast<int>(datum);
    }
};

template<>
class Cast<double> {
public:
    Cast() {}
    ~Cast() {}

    double operator()(const char* datum) const
    {
        return atof(datum);
    }

    double operator()(char* const datum) const
    {
        return atof(datum);
    }

    double operator()(const std::string& datum) const
    {
        return atof(datum.c_str());
    }

    template<typename T>
    double operator()(const T& datum) const
    {
        return static_cast<double>(datum);
    }
};

template<>
class Cast<char> {
public:
    Cast() {}
    ~Cast() {}

    // Just takes first part of string
    char operator()(const char* datum) const
    {
        return datum[0];
    }

    char operator()(char* const datum) const
    {
        return datum[0];
    }

    char operator()(const std::string& datum) const
    {
        return datum[0];
    }

    template<typename T>
    char operator()(const T& datum) const
    {
        return static_cast<int>(datum);
    }
};

template<>
class Cast<boost::posix_time::ptime> {
public:
    Cast() {}
    ~Cast() {}

    // expecting format yyyy-mm-dd
    boost::posix_time::ptime operator()(const char* datum) const
    {
        char buf[10];
        strncpy(buf, datum, 4);
        buf[4] = '\0';
        unsigned short year = static_cast<unsigned short>(atoi(buf));
        strncpy(buf, datum + 5, 2);
        buf[2] = '\0';
        unsigned short  month = static_cast<unsigned short>(atoi(buf));
        strncpy(buf, datum + 8, 2);
        unsigned short  day = static_cast<unsigned short>(atoi(buf));

        boost::gregorian::date dt(year, month, day);

        return boost::posix_time::ptime(dt);
    }

    boost::posix_time::ptime operator()(const std::string& datum) const
    {
        return operator()(datum.c_str());
    }

    template<typename T>
    boost::posix_time::ptime operator()(const T& datum) const
    {
        return static_cast<boost::posix_time::ptime>(datum);
    }
};

template<>
class Cast<std::string> {
public:
    Cast() {}
    ~Cast() {}

    std::string operator()(int datum) const
    {
        char buf[24] = {};
#ifdef _WIN32
        _itoa_s(datum, buf, 24, 10);
#else
        snprintf(buf, 24, "%i", datum);
#endif
            return buf;
    }

    // Note that this returns the reference to itself
    const std::string& operator()(const std::string& datum) const
    {
        return datum;
    }

    std::string operator()(char* datum)
    {
        return datum;
    }

    std::string operator()(const char* datum)
    {
        return datum;
    }

    template<typename T>
    std::string operator()(const T& datum) const
    {
        //std::ostringstream ss;
        //ss << datum;
        //return ss.str();
        return boost::lexical_cast<std::string>(datum);
    }
};

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#pragma warning (pop)

#endif
