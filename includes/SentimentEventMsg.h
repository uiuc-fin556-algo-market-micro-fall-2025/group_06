/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/SentimentEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_SENTIMENT_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_SENTIMENT_EVENT_MSG_H_

#include "DataTypes.h"
#include "EventMsg.h"

namespace RCM {
namespace StrategyStudio {

/**
 * SentimentEventMsg is an event message that gets triggered when a Sentiment event arrives
 */
class SentimentEventMsg : public EventMsg {
public:
    SentimentEventMsg(Event* event, Utilities::TimeType eventTime);

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
    * @return returns the adapter time
    */
    TimeType adapter_time() const { return adapter_time_; }
    void set_adapter_time(TimeType adapter_time) { adapter_time_ = adapter_time; }

    /**
     * Raw-S - Is the summation of individual Tweet sentiment scores over the last 24hrs. Summation is not exponentially weighted.
     * @return returns the Raw-S value from SMA, or quiet_NaN if not available
     */
    double raw_s() const { return raw_s_; }
    void set_raw_s(double raw_s) { raw_s_ = raw_s; }

    /**
     * Raw S-Mean - 20 day moving average of Raw-S
     * @return returns the Raw S-Mean, or quiet_NaN if not available
     */
    double raw_s_mean() const { return raw_s_mean_; }
    void set_raw_s_mean(double raw_s_mean) { raw_s_mean_ = raw_s_mean; }

    /**
     * Raw S-Score - Normalized value of Raw-S
     * @return returns the Raw S-Score, or quiet_NaN if not available
     */
    double raw_s_score() const { return raw_s_score_; }
    void set_raw_s_score(double raw_s_score) { raw_s_score_ = raw_s_score; }

    /**
     * Raw S-Volatility - 20 day moving average of Raw-S
     * @return returns the Raw S-Volatility, or quiet_NaN if not available
     */
    double raw_s_volatility() const { return raw_s_volatility_; }
    void set_raw_s_volatility(double raw_s_volatility) { raw_s_volatility_ = raw_s_volatility; }

    /**
     * S - Is the exponentially weight summation of individual Tweet sentiment scores over the last 24hrs with a 16-hour half-life.
     * @return returns the S, or quiet_NaN if not available
     */
    double s() const { return s_; }
    void set_s(double s) { s_ = s; }

    /**
     * S-Buzz - Measurement of unusual volume activity provided by SMA
     * @return returns the S-Buzz, or quiet_NaN if not available
     */
    double s_buzz() const { return s_buzz_; }
    void set_s_buzz(double s_buzz) { s_buzz_ = s_buzz; }

    /**
     * S-Delta - Change in S-Score over a look-back period
     * @return returns the S-Delta, or quiet_NaN if not available
     */
    double s_delta() const { return s_delta_; }
    void set_s_delta(double s_delta) { s_delta_ = s_delta; }

    /**
     * S-Dispersion - Measurement of the tweet source concentration contributing to an S-Score
     * @return returns the S-Dispersion, or quiet_NaN if not available
     */
    double s_dispersion() const { return s_dispersion_; }
    void set_s_dispersion(double s_dispersion) { s_dispersion_ = s_dispersion; }

    /**
     * S-Mean - 20 day moving average of the exponentially weighted sentiment estimate
     * @return returns the S-Mean, or quiet_NaN if not available
     */
    double s_mean() const { return s_mean_; }
    void set_s_mean(double s_mean) { s_mean_ = s_mean; }

    /**
     * S-Score - Normalized value of the exponentially weighted sentiment estimate (SMA's S-Score)
     * @return returns the S-Score, or quiet_NaN if not available
     */
    double s_score() const {  return s_score_; }
    void set_s_score(double s_score) { s_score_ = s_score; }

    /**
     * SV-Mean - 20 day moving average of volume
     * @return returns the SV-Mean, or quiet_NaN if not available
     */
    double s_vmean() const { return s_vmean_; }
    void set_s_vmean(double sv_mean) { s_vmean_ = sv_mean; }

    /**
     * S-Volatility -  20 day moving standard deviation of the exponentially weighted sentiment estimate
     * @return returns the S-Volatility, or quiet_NaN if not available
     */
    double s_volatility() const { return s_volatility_; }
    void set_s_volatility(double s_volatility) { s_volatility_ = s_volatility; }

    /**
     * S-Volume - Indicative tweet volume used to compute the sentiment estimate
     * @return returns the S-Volume, or -1 if not available
     */
    int s_volume() const { return s_volume_; }
    void set_s_volume(int s_volume) { s_volume_ = s_volume; }

    /**
     * S-VScore - Normalized value of volume
     * @return returns the S-VScore, or quiet_NaN if not available
     */
    double s_vscore() const { return s_vscore_; }
    void set_s_vscore(double s_vscore) { s_vscore_ = s_vscore; }

    /**
     * S-VVolatility - 20 day moving standard deviation of volume
     * @return returns the S-VVolatility, or quiet_NaN if not available
     */
    double s_vvolatility() const { return s_vvolatility_; }
    void set_s_vvolatility(double s_vvolatility) { s_vvolatility_ = s_vvolatility; }

    /**
     * Raw-S-50 - Is the summation of individual Tweet sentiment scores over the last 50-days. Summation is not exponentially weighted.
     * @return returns the Raw-S-50, or quiet_NaN if not available
     */
    double raw_s_50() const { return raw_s_50_; }
    void set_raw_s_50(double raw_s_50) { raw_s_50_ = raw_s_50; }

    /**
     * Raw-S-Mean-50 - Is the 50-day average of Raw-S-50.
     * @return returns the Raw-S-Mean-50, or quiet_NaN if not available
     */
    double raw_s_mean_50() const { return raw_s_mean_50_; }
    void set_raw_s_mean_50(double raw_s_mean_50) { raw_s_mean_50_ = raw_s_mean_50; }

    /**
     * Raw-S-Volatility-50 - Is the 50-day Standard Deviation of Raw-S-50.
     * @return returns the Raw-S-Volatility-50, or quiet_NaN if not available
     */
    double raw_s_volatility_50() const { return raw_s_volatility_50_; }
    void set_raw_s_volatility_50(double raw_s_volatility_50) { raw_s_volatility_50_ = raw_s_volatility_50; }

    /**
     * Raw-S-Score-50 - Is the S-Score using non exponential summations with a 50 day look back period.
     * @return returns the Raw-S-Score-50, or quiet_NaN if not available
     */
    double raw_s_score_50() const { return raw_s_score_50_; }
    void set_raw_s_score_50(double raw_s_score_50) { raw_s_score_50_ = raw_s_score_50; }

    /**
     * Raw-S-Velocity-50 - Is the one-day change in Raw-S-50
     * @return returns the Raw-S-Velocity-50, or quiet_NaN if not available
     */
    double raw_s_velocity_50() const { return raw_s_velocity_50_; }
    void set_raw_s_velocity_50(double raw_s_velocity_50) { raw_s_velocity_50_ = raw_s_velocity_50; }

    /**
     * Raw-S-Accel-50 - Is the one-day change in Raw-S-Velocity-50
     * @return returns the Raw-S-Accel-50, or quiet_NaN if not available
     */
    double raw_s_accel_50() const { return raw_s_accel_50_; }
    void set_raw_s_accel_50(double raw_s_accel_50) { raw_s_accel_50_ = raw_s_accel_50; }

    /**
     * Raw-S-200 - Is the summation of individual Tweet sentiment scores over the last 200-days. Summation is not exponentially weighted.
     * @return returns the Raw-S-200, or quiet_NaN if not available
     */
    double raw_s_200() const { return raw_s_200_; }
    void set_raw_s_200(double raw_s_200) { raw_s_200_ = raw_s_200; }

    /**
     * Raw-S-Mean-200 - Is the 200-day average of Raw-S-200.
     * @return returns the Raw-S-Mean-200, or quiet_NaN if not available
     */
    double raw_s_mean_200() const { return raw_s_mean_200_; }
    void set_raw_s_mean_200(double raw_s_mean_200) { raw_s_mean_200_ = raw_s_mean_200; }

    /**
     * Raw-S-Volatility-200 - Is the 200-day Standard Deviation of Raw-S-200.
     * @return returns the Raw-S-Volatility-200, or quiet_NaN if not available
     */
    double raw_s_volatility_200() const { return raw_s_volatility_200_; }
    void set_raw_s_volatility_200(double raw_s_volatility_200) { raw_s_volatility_200_ = raw_s_volatility_200; }

    /**
     * Raw-S-Score-200 - Is the S-Score using non exponential summations with a 200 day look back period.
     * @return returns the Raw-S-Score-200, or quiet_NaN if not available
     */
    double raw_s_score_200() const { return raw_s_score_200_; }
    void set_raw_s_score_200(double raw_s_score_200) { raw_s_score_200_ = raw_s_score_200; }

    /**
     * Raw-S-Velocity-200 - Is the one-day change in Raw-S-200
     * @return returns the Raw-S-Velocity-200, or quiet_NaN if not available
     */
    double raw_s_velocity_200() const { return raw_s_velocity_200_; }
    void set_raw_s_velocity_200(double raw_s_velocity_200) { raw_s_velocity_200_ = raw_s_velocity_200; }

    /**
     * Raw-S-Accel-200 - Is the one-day change in Raw-S-Velocity-200
     * @return returns the Raw-S-Accel-200, or quiet_NaN if not available
     */
    double raw_s_accel_200() const { return raw_s_accel_200_; }
    void set_raw_s_accel_200(double raw_s_accel_200) { raw_s_accel_200_ = raw_s_accel_200; }

private:
    std::string entity_;
    Utilities::TimeType source_time_;
    Utilities::TimeType adapter_time_;

    double raw_s_;
    double raw_s_mean_;
    double raw_s_score_;
    double raw_s_volatility_;
    double s_;
    double s_buzz_;
    double s_delta_;
    double s_dispersion_;
    double s_mean_;
    double s_score_;
    double s_volatility_;
    double s_vmean_;
    double s_vscore_;
    double s_vvolatility_;
    double raw_s_50_;
    double raw_s_mean_50_;
    double raw_s_volatility_50_;
    double raw_s_score_50_;
    double raw_s_accel_50_;
    double raw_s_velocity_50_;
    double raw_s_200_;
    double raw_s_mean_200_;
    double raw_s_volatility_200_;
    double raw_s_score_200_;
    double raw_s_accel_200_;
    double raw_s_velocity_200_;
    int s_volume_;

    SentimentType sentiment_type_;
    SentimentSource sentiment_source_;
};

} // namespace StrategyStudio
} // namespace RCM

#endif