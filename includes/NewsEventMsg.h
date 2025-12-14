/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/NewsEventMsg.h
*     Last Update: 2011/09/12 13:55:14
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2009 - 2011.
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

#ifndef _STRATEGY_STUDIO_LIB_NEWS_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_NEWS_EVENT_MSG_H_

#include "EventMsg.h"

namespace RCM {
namespace StrategyStudio {

/**
 * NewsEventMsg is an event message that gets triggered when a News event arrives
 */
class NewsEventMsg : public EventMsg {
public:
    NewsEventMsg(Event* event, Utilities::TimeType eventTime);

    /**
     * Check the validity of each of the three possible measurements values
     */
    bool is_min_valid() const { return flex_range_status_.status.min_valid; }
    bool is_mid_valid() const { return flex_range_status_.status.mid_valid; }
    bool is_max_valid() const { return flex_range_status_.status.max_valid; }

    /**
     * Determine how to interpret the three numeric measurement values
     */
    bool is_min_exclusive() const { return flex_range_status_.status.min_exclusive; }
    bool is_max_exclusive() const { return flex_range_status_.status.max_exclusive; }
    bool is_approximate() const { return flex_range_status_.status.approximate; }

    // set the status byte for the observation, which describes its validity
    // and whether the measurement value was verified and/or derived
    void set_status(boost::uint8_t byte) { observation_status_.byte = byte; }

    // set the flex-range status byte, including which values are valid
    // and the characteristics of the values
    void set_flex_range_status(boost::uint8_t byte) { flex_range_status_.byte = byte; }

    /**
     * Returns a string describing the data type of the measurement values
     * (e.g., decimal, boolean, flex_range, etc.)
     */
    const std::string& measurement_type() const { return measurement_type_; }
    void set_measurement_type(const std::string& type) { measurement_type_ = type; }

    /**
     * Returns the entity for this News Event as represented in the news feed
     */
    const std::string& entity() const { return entity_; }
    void set_entity(const std::string& entity) { entity_ = entity; }

    /**
     * Returns the metric this event is reporting, e.g., FIN.EPS, FIN.REV, etc.
     */
    const std::string& measure() const { return measure_; }
    void set_measure(const std::string& measure) { measure_ = measure; }

    /**
     * Describes the period for the measurement
     */
    const std::string& period() const { return period_; }
    void set_period(const std::string& period) { period_ = period; }

    /**
     * Returns the lower-bound measurement value; only valid if is_min_valid() == true
     */
    double lower_bound() const { return lower_bound_; }
    void set_lower_bound(double lowerBound) { flex_range_status_.status.min_valid = true; lower_bound_ = lowerBound; }

    /**
     * Returns the mid measurement value; only valid if is_mid_valid() == true
     */
    double mid() const { return mid_; }
    void set_mid(double mid) { flex_range_status_.status.mid_valid = true; mid_ = mid; }

    /**
     * Returns the upper-bound measurement value; only valid if is_max_valid() == true
     */
    double upper_bound() const { return upper_bound_; }
    void set_upper_bound(double upperBound) { flex_range_status_.status.max_valid = true; upper_bound_ = upperBound; }

public:
    /**
     * Checks several characteristics of the message to determine whether it is complete and valid
     */
    bool IsValid() const;

private:
    struct ObservationStatus {
        bool observed : 1;
        bool valid : 1;
        bool verified : 1;
        bool derived : 1;
        boost::uint8_t bits0to3 : 4;
    };

    struct FlexRangeStatus {
        bool min_valid : 1;
        bool mid_valid : 1;
        bool max_valid : 1;
        boost::uint8_t m_bit4 : 1;
        bool min_exclusive : 1;
        bool approximate : 1;
        bool max_exclusive : 1;
        boost::uint8_t bit0 : 1;
    };

    union FRStatusUnion	{
        FlexRangeStatus status;
        boost::uint8_t byte;
    } flex_range_status_;

    union ObservationStatusUnion {
        ObservationStatus status;
        boost::uint8_t byte;
    } observation_status_;

    std::string measurement_type_;
    std::string entity_;
    std::string measure_;
    std::string period_;
    double lower_bound_;
    double mid_;
    double upper_bound_;
};

} // namespace StrategyStudio
} // namespace RCM

#endif