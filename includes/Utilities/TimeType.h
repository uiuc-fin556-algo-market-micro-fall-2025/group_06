/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/TimeType.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_TIME_TYPE_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_TIME_TYPE_H_

#include "StdTypes.h"

#include <string>
#include <iomanip>
#include <cstdlib>

#pragma warning (push)
#pragma warning( disable: 4996)
#pragma warning( disable: 4244)

#define USE_BOOST_TIME_TYPE
//#define USE_TIMET_TIME_TYPE

#if defined(USE_TIMET_TIME_TYPE) && defined(USE_BOOST_TIME_TYPE)
#error Only can use one type of time type
#endif

#if !defined(USE_TIMET_TIME_TYPE) && !defined(USE_BOOST_TIME_TYPE)
#error Must choose a time type
#endif

#ifdef USE_BOOST_TIME_TYPE
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <time.h>
#elif defined(USE_TIMET_TIME_TYPE)
#include <time.h>
#endif

#define NULL_TIME_TYPE Utilities::DefaultValueHolder::DEFAULT_PTIME
#define NULL_DATE_TYPE Utilities::DefaultValueHolder::DEFAULT_DATE

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

#ifdef USE_BOOST_TIME_TYPE

class DefaultValueHolder {
public:
    static boost::posix_time::ptime DEFAULT_PTIME;
    static boost::gregorian::date DEFAULT_DATE;
};

typedef boost::gregorian::date DateType;
typedef boost::posix_time::ptime TimeType;
typedef boost::posix_time::time_duration TimeSpanType;
typedef boost::posix_time::time_duration TimeDuration;
typedef boost::local_time::time_zone_ptr TimeZonePtr;

/**
 * Gets the current time, with microsecond resolution when possible on the hardware platform, represented in UTC
 */
inline TimeType Now()
{
    // Implementation adapted from boost::posix_time::microsecond_clock::universal_time() but bypasses overhead constructing date from y,m,d and from std::tm/gmtime intermediary
#ifdef BOOST_HAS_GETTIMEOFDAY
    timeval tv;
    gettimeofday(&tv, 0); //gettimeofday does not support TZ adjust on Linux.
    return TimeType(DateType(2440588), boost::posix_time::seconds(static_cast<long>(tv.tv_sec)) + boost::posix_time::microseconds(static_cast<long>(tv.tv_usec)));
#elif defined(BOOST_HAS_FTIME)
    boost::winapi::FILETIME_ ft;
    boost::winapi::GetSystemTimeAsFileTime(&ft);
    boost::uint64_t ft_as_integer = ((static_cast<boost::uint64_t>(ft.dwHighDateTime) << 32) | static_cast<boost::uint64_t>(ft.dwLowDateTime)) - 116444736000000000ULL;
    boost::uint64_t micros = ft_as_integer / 10U;
    return TimeType(DateType(2440588), boost::posix_time::seconds(static_cast<long>(micros / 1000000UL)) + boost::posix_time::microseconds(static_cast<long>(micros % 1000000UL)));
#else
#error Internal Boost.DateTime error: BOOST_DATE_TIME_HAS_HIGH_PRECISION_CLOCK is defined, however neither gettimeofday nor FILETIME support is detected.
    return NULL_TIME_TYPE;
#endif
}

/**
 * Gets the current day according to the local clock
 */
inline TimeType TodayLocal()
{
    return TimeType(boost::gregorian::day_clock::local_day());
}

/**
 * Gets the current day according to the UTC clock
 */
inline TimeType TodayUTC()
{
    return TimeType(boost::gregorian::day_clock::universal_day());
}

inline std::string TodayAsString()
{
    return boost::gregorian::to_iso_string(boost::gregorian::day_clock::universal_day());
}

/**
 * Returns a string representation of the TimeType
 */
inline std::string TimeTypeToString(TimeType time)
{
    return boost::posix_time::to_simple_string(time);
}

/**
 * Returns a string representation of the TimeType in central
 */
inline std::string UtcTimeTypeToCentralTimeString(TimeType time)
{
    typedef boost::date_time::local_adjustor<boost::posix_time::ptime, -6, boost::posix_time::us_dst> us_central;
    time = us_central::utc_to_local(time);
    return boost::posix_time::to_simple_string(time);
}

/**
 * Returns a string representation of the TimeType
 */
inline std::string ToString(TimeType time)
{
    return boost::posix_time::to_simple_string(time);
}

/**
 * Returns a string representation of a time duration
 */
inline std::string ToString(TimeDuration td)
{
    return boost::posix_time::to_simple_string(td);
}

/**
 * Returns a string representation of a date
 */
inline std::string ToString(boost::gregorian::date dt)
{
    return boost::gregorian::to_simple_string(dt);
}

/**
 * Returns a ISO extended string representation of a date. Example 2016-01-25
 */
inline std::string ToExtendedString(boost::gregorian::date dt)
{
    return boost::gregorian::to_iso_extended_string(dt);
}

/**
 * Converts UTC to local time based on machine's time zone
 *
 * Warning: this function assumes machine's local timezone is set correctly
 */
inline TimeType ConvertUTCToLocal(TimeType time)
{
    return boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(time);
}

/**
 * Converts UTC to local time based on supplied timeZone
 */
inline TimeType ConvertUTCToLocal(TimeType t, TimeZonePtr timeZone)
{
    return boost::local_time::local_date_time(t, timeZone).local_time();
}

/**
 * Returns a string representation of the TimeType
 */
inline std::string ToLocalTimeString(TimeType time)
{
    return boost::posix_time::to_simple_string(ConvertUTCToLocal(time));
}

/**
 * Checks if a supplied Local TimeType is during a specified timeZone's DST period
 */
inline bool IsLocalTimeDst(TimeType t, TimeZonePtr timeZone)
{
    return timeZone->has_dst() && (t > timeZone->dst_local_start_time(t.date().year())) && (t < timeZone->dst_local_end_time(t.date().year()));
}

/**
 * Converts Local to UTC time based on machine's time zone
 *
 * Warning: this function assumes machine's local timezone is set correctly
 */
inline TimeType ConvertLocalToUTC(TimeType time)
{
    TimeType tmp = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(time);
    return time + (time-tmp);
}

/**
 * Converts Local to UTC time based on supplied timeZone
 */
inline TimeType ConvertLocalToUTC(TimeType t, TimeZonePtr timeZone)
{
    TimeDuration dstOffset(0, 0, 0, 0);
    if (IsLocalTimeDst(t, timeZone))
        dstOffset = timeZone->dst_offset();

    return t - timeZone->base_utc_offset() - dstOffset;
}

/**
 * Converts UTC to US Eastern timezone
 *
 * eastern timezone is utc-5 plus dst adjustment
 */
inline TimeType ConvertUTCToUSEastern(TimeType time)
{

    typedef boost::date_time::local_adjustor<boost::posix_time::ptime, -5, boost::posix_time::us_dst> us_eastern;
    return us_eastern::utc_to_local(time);
}

/**
 * Converts US Eastern to UTC timezone
 *
 * eastern timezone is utc-5 plus dst adjustment
 */
inline TimeType ConvertUSEasternToUTC(TimeType time, boost::date_time::dst_flags dst = boost::date_time::calculate)
{
    typedef boost::date_time::local_adjustor<boost::posix_time::ptime, -5, boost::posix_time::us_dst> us_eastern;
    return us_eastern::local_to_utc(time, dst);
}

/**
 * Converts US Eastern to UTC timezone
 * time is expected to be formatted like hh[:mm]
 * eastern timezone is utc-5 plus dst adjustment
 */
inline TimeType ConvertUSEasternToUTC(DateType date, const std::string& time, boost::date_time::dst_flags dst = boost::date_time::calculate)
{
    typedef boost::date_time::local_adjustor<boost::posix_time::ptime, -5, boost::posix_time::us_dst> us_eastern;
    int hours = 0;
    int minutes = 0;
    size_t pos = time.find(':');
    if (pos > 0) {
        hours = atoi(time.substr(0, pos-1).c_str());
        minutes = atoi(time.substr(pos).c_str());
    }
    else
        hours = atoi(time.c_str());
    return us_eastern::local_to_utc(TimeType(date, boost::posix_time::hours(hours) + boost::posix_time::minutes(minutes)), dst);
}

/**
 * Converts UTC to US Central timezone
 *
 * central timezone is utc-6 plus dst adjustment
 */
inline TimeType ConvertUTCToUSCentral(TimeType time)
{
    typedef boost::date_time::local_adjustor<boost::posix_time::ptime, -6, boost::posix_time::us_dst> us_central;
    return us_central::utc_to_local(time);
}

/**
 * Converts US Central to UTC timezone
 *
 * central timezone is utc-6 plus dst adjustment
 */
inline TimeType ConvertUSCentralToUTC(TimeType time, boost::date_time::dst_flags dst = boost::date_time::calculate)
{
    typedef boost::date_time::local_adjustor<boost::posix_time::ptime, -6, boost::posix_time::us_dst> us_central;
    return us_central::local_to_utc(time, dst);
}

/**
 * Converts UTC to US Pacific timezone
 *
 * pacific timezone is utc-8 plus dst adjustment
 */
inline TimeType ConvertUTCToUSPacific(TimeType time)
{
    typedef boost::date_time::local_adjustor<boost::posix_time::ptime, -8, boost::posix_time::us_dst> us_pacific;
    return us_pacific::utc_to_local(time);
}

inline boost::posix_time::ptime PTimeFromTimeT(time_t time)
{
    return boost::posix_time::from_time_t(time);
}

inline void AddSecondsToTimeType(TimeType& timeType, int seconds)
{
    timeType += boost::posix_time::seconds(seconds);
}

inline TimeType AddSecondsWithTimeType(TimeType timeType, int seconds)
{
    return timeType + boost::posix_time::seconds(seconds);
}

inline TimeType AddSecondsWithTimeType(TimeType timeType, unsigned seconds)
{
    return timeType + boost::posix_time::seconds(seconds);
}

inline TimeType AddSecondsWithTimeType(TimeType timeType, double seconds)
{
    int milliseconds = static_cast<int>((seconds - static_cast<int>(seconds)) * 1000);

    return (milliseconds == 0) ? (timeType + boost::posix_time::seconds(static_cast<int>(seconds))) :
        ( timeType + boost::posix_time::seconds(static_cast<int>(seconds)) +
            boost::posix_time::milliseconds(static_cast<int>(milliseconds)) );

}

inline void AddMilliSecondsToTimeType(TimeType& timeType, int ms)
{
    timeType += boost::posix_time::milliseconds(ms);
}

inline TimeType AddMilliSecondsWithTimeType(TimeType timeType, int ms)
{
    return timeType + boost::posix_time::milliseconds(ms);
}

inline void AddMicroSecondsToTimeType(TimeType& timeType, int mc)
{
    timeType += boost::posix_time::microseconds(mc);
}

inline TimeType AddMicroSecondsWithTimeType(TimeType timeType, int mc)
{
    return timeType + boost::posix_time::microseconds(mc);
}

inline TimeType AddMinutesWithTimeType(TimeType timeType, int min)
{
    return timeType + boost::posix_time::minutes(min);
}

inline TimeType AddHoursWithTimeType(TimeType timeType, int hours)
{
    return timeType + boost::posix_time::hours(hours);
}

inline TimeType NullTimeType()
{
    return NULL_TIME_TYPE;
}

inline TimeType DateStringToTimeType(const std::string& str)
{
    boost::gregorian::date dt = boost::gregorian::from_string(str);
    return TimeType(dt, boost::posix_time::time_duration());
}

inline std::string TimeTypeToSqlDateString(TimeType time)
{
    std::ostringstream str;

    //const boost::posix_time::time_duration& td = time.time_of_day();
    const boost::gregorian::date& dt = time.date();

    // Note: fix later
    //int milliseconds = 0;

    str << std::setw(4) << std::setfill('0') << dt.year() << "-";
    str << std::setw(2) << static_cast<int>(dt.month()) << "-";
    str << std::setw(2) << static_cast<int>(dt.day());

    return str.str();
}

inline TimeType TransportDateTimeToTimeType(StrategyStudioInt64 microsecondsSinceEpoch)
{
    return TimeType(DefaultValueHolder::DEFAULT_DATE, boost::posix_time::microseconds(microsecondsSinceEpoch));
}

inline StrategyStudioInt64 TimeTypeToTransportDateTime(TimeType time)
{
    return (time - DefaultValueHolder::DEFAULT_PTIME).total_microseconds();
}

/**
 * Convert a SQL string (YYYY-MM-DD) to TimeType
 */
inline TimeType TimeTypeFromSqlDateCString(const char* sqlString)
{
    char buf[10];
    strncpy(buf, sqlString, 4);
    buf[4] = '\0';
    int year = atoi(buf);
    strncpy(buf, sqlString + 5, 2);
    buf[2] = '\0';
    int month = atoi(buf);
    strncpy(buf, sqlString + 8, 2);
    int day = atoi(buf);

    boost::gregorian::date dt((unsigned short)year, (unsigned short)month, (unsigned short)day);

    return boost::posix_time::ptime(dt);
}

/**
 * Get todays utc time with a given offset
 */
inline TimeType TodaysTimeUTC(int hours, int min, int sec)
{
    return boost::posix_time::ptime(boost::gregorian::day_clock::universal_day(),
        boost::posix_time::hours(hours) + boost::posix_time::minutes(min) + boost::posix_time::seconds(sec));
}

/**
 * Get todays utc time with a given offset
 */
inline TimeType TodaysTimeLocal(int hours, int min, int sec)
{
    return boost::posix_time::ptime(boost::gregorian::day_clock::local_day(),
        boost::posix_time::hours(hours) + boost::posix_time::minutes(min) + boost::posix_time::seconds(sec));
}

/**
 * Get todays utc time with a given offset
 */
inline TimeType TodaysTimeLocal(int hours, int min, int sec, int milliseconds)
{
    return boost::posix_time::ptime(boost::gregorian::day_clock::local_day(),
        boost::posix_time::hours(hours) + boost::posix_time::minutes(min) +
        boost::posix_time::seconds(sec) + boost::posix_time::milliseconds(milliseconds));
}

/**
 * Convert a SQL string (YYYY-MM-DD) to TimeType
 */
inline TimeType TimeTypeFromSqlDateString(const std::string& str)
{
    return TimeTypeFromSqlDateCString(str.c_str());
}

inline boost::gregorian::date today()
{
    return boost::gregorian::day_clock::local_day();
}

#elif defined(USE_TIMET_TIME_TYPE)

#define NULL_TIME_TYPE 0

typedef unsigned long TimeType;

inline TimeType Now()
{
    return static_cast<long>(time(NULL));
}

inline std::string TimeTypeToString(TimeType time)
{
    time_t timet = static_cast<time_t>(time);
#ifdef WIN32
    char buf[30];
    struct tm timeinfo;
    localtime_s(&timeinfo, &timet);
    asctime_s(buf, 30, &timeinfo);
    buf[strlen(buf) - 1] = '\0';
    return buf;

#elif WIN64
    char buf[30];
    struct tm timeinfo;
    localtime_s(timeinfo, &timet);
    asctime_s(buf, 30, &timeinfo);
    return buf;
#else
    struct tm* timeinfo;
    timeinfo = localtime(&timet);
    return asctime(timeinfo);
#endif
}

#else

#error Invalid time type selected

#endif

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#pragma warning (pop)

#endif
