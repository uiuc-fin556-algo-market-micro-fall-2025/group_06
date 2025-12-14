/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/SentimentActivityEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_SENTIMENT_ACTIVITY_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_SENTIMENT_ACTIVITY_EVENT_MSG_H_

#include "DataTypes.h"
#include "EventMsg.h"

namespace RCM {
namespace StrategyStudio {

/**
 * SentimentActivityEventMsg is an event message that deliveres information about recent sentiment activity
 */
class SentimentActivityEventMsg : public EventMsg {
public:
    SentimentActivityEventMsg(Event* event, Utilities::TimeType eventTime);

public:
    /**
     * Gets and sets the sentiment entity
     * @return returns the sentiment entity
     */
    const std::string& entity() const { return entity_; }
    void set_entity(const std::string& entity) { entity_ = entity; }

    /**
     * Gets and sets the sentiment type
     * @return returns the sentiment type
     */
    SentimentType sentiment_type() const { return sentiment_type_; }
    void set_sentiment_type(SentimentType sentiment_type) { sentiment_type_ = sentiment_type; }

    /**
     * Gets and sets the sentiment source
     * @return returns the sentiment source
     */
    SentimentSource sentiment_source() const { return sentiment_source_; }
    void set_sentiment_source(SentimentSource sentiment_source) { sentiment_source_ = sentiment_source; }

    /**
     * Gets and sets the source time
     * @return returns the source time
     */
    TimeType source_time() const { return source_time_; }
    void set_source_time(TimeType source_time) { source_time_ = source_time; }

    /**
     * Gets and sets the adapter time
     * @return returns the adapter time, or quiet_NaN if not available
     */
    TimeType adapter_time() const { return adapter_time_; }
    void set_adapter_time(TimeType adapter_time) { adapter_time_ = adapter_time; }

    /**
     * s1mr_s - 1 minute rated s
     * @return returns the s1mr_s value from SMA
     */
    double s1mr_s() const { return s1mr_s_; }
    void set_s1mr_s(double s1mr_s) { s1mr_s_ = s1mr_s; }

    /**
    * s1mr_raws - 1 minute rated raw s
    * @return returns the s1mr_raws value from SMA, or quiet_NaN if not available
    */
    double s1mr_raws() const { return s1mr_raws_; }
    void set_s1mr_raws(double s1mr_raws) { s1mr_raws_ = s1mr_raws; }

    /**
     * s1mr_sdispersion - 1 minute rated sdispersion
     * @return returns the s1mr_sdispersion value from SMA, or quiet_NaN if not available
     */
    double s1mr_sdispersion() const { return s1mr_sdispersion_; }
    void set_s1mr_sdispersion(double s1mr_sdispersion) { s1mr_sdispersion_ = s1mr_sdispersion; }

    /**
     * s1mr_svolume - 1 minute rated tweet volume
     * @return returns the s1mr_svolume, or -1 if not available
     */
    int s1mr_svolume() const { return s1mr_svolume_; }
    void set_s1mr_svolume(int s1mr_svolume) { s1mr_svolume_ = s1mr_svolume; }

private:
    std::string entity_;
    Utilities::TimeType source_time_;
    Utilities::TimeType adapter_time_;

    double s1mr_s_;
    double s1mr_raws_;
    double s1mr_sdispersion_;
    int s1mr_svolume_;

    SentimentType sentiment_type_;
    SentimentSource sentiment_source_;
};

} // namespace StrategyStudio
} // namespace RCM

#endif