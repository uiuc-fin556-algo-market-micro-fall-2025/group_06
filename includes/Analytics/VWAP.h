/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Analytics/VWAP.h
*     Last Update: 2012/2/29 13:00:00
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2012.
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
#ifndef _STRATEGY_STUDIO_ANALYTICS_VWAP_H_
#define _STRATEGY_STUDIO_ANALYTICS_VWAP_H_

#include <math.h>
#include <deque>
#include <Utilities/TimeType.h>

namespace RCM {
namespace StrategyStudio {
namespace Analytics {

class VWAP {
public:
    /**
     * create a vwap indicator that will roll on time
     * input the time span and the maximum trade size (use -1 to ignore)
     */
    VWAP(Utilities::TimeSpanType window_size_time, int ignore_vol_above = -1);

    /**
     * create a vwap that will roll on volume
     * input the time span and the maximum trade size (use -1 to ignore)
     */
    VWAP(int window_size_volume, int ignore_vol_above = -1);

    ~VWAP();

    /**
     * Add an observation to the VWAP
     */
    bool Update(Utilities::TimeType t, int volume, double price);

    /**
     * Returns the maximum volume threshold (indicator ignores trades above this)
     */
    int max_volume() const {return m_max_vol;}

    /**
     * Set the maximum volume allowed (will ignore trades above this)
     */
    void set_max_volume(int mv) {m_max_vol=mv;}

    /**
     * Returns whether the vwap sample window is defined in terms of volume instead of time interval
     */
    bool is_volume_based() const {return m_is_volume_based;}

    /**
     * Gets the size of the VWAP sample window in terms of trade volume
     */
    int window_volume() const {return m_tot_vol;}

    /**
     * Set the window size using volume
     */
    void set_window_size(int window_size_volume) {m_tot_vol=window_size_volume;}

    /**
     * Gets the size of the VWAP sample window in terms of time span
     */
    Utilities::TimeSpanType window_time_span() const {return m_delta_t;}

    /**
     * Set the window size using time
     */
    void set_window_time_span(Utilities::TimeSpanType window_size_time) {m_delta_t=window_size_time;}

    /**
     * returns the current window's total volume.
     * Pass the current time for a time based vwap, if not it will use the last update time.
     */
    int current_window_volume();
    int current_window_volume(Utilities::TimeType now);

    /**
     * Return the current window time span.
     * Pass the current time for a time based vwap, if not it will use the last update time.
     */
    Utilities::TimeSpanType current_window_time_span();
    Utilities::TimeSpanType current_window_time_span(Utilities::TimeType now);

    /**
     * Get the time since the last trade
     */
    Utilities::TimeSpanType time_since_last() const {return Utilities::Now()-m_last_t;}

    /**
     * check if the VWAP is full
     */
    bool full() const {return m_filled_flag;}

    /**
     * Get the weighted mean. Pass the current time for a time based vwap, if not it will use the last update time.
     */
    double Mean();
    double Mean(Utilities::TimeType now);

    /**
     * Get the weighted standard deviation. Pass the current time for a time based vwap, if not it will use the last update time.
     */
    double StdDev();
    double StdDev(Utilities::TimeType now);

    /**
     * Get the weighted z_score of the last entry. Pass the current time for a time based vwap, if not it will use the last update time.
     */
    double ZScore();
    double ZScore(Utilities::TimeType now);

    /**
     * Get the weighted z_score of the given value. Pass the current time for a time based vwap, if not it will use the last update time.
     */
    double ZScore(double p);
    double ZScore(double p,Utilities::TimeType now);

    /**
     * Reset the VWAP
     */
    void Reset();

private:
    /**
     * Update the time span
     */
    bool UpdateTimeSpan(Utilities::TimeType now);

    /**
     * Update the volume span
     */
    bool UpdateVolumeSpan();

    /**
     * Make sure the moments are updated
     */
    void UpdateMoments();

    /**
     * Add or remove values form the running quantities (AddRemove>0 is adding)
     */
    void UpdateVars(int& vol, double& p, Utilities::TimeType& t, int AddRemove);

private:
    int m_tot_vol, m_max_vol;
    double m_last_p, m_sum_p, m_sum_v, m_sum_p2;
    double m_sum_pv, m_sum_p2v;
    double m_vwap, m_std_vwap;
    int m_N;
    Utilities::TimeSpanType m_delta_t;
    Utilities::TimeSpanType m_time_span;
    Utilities::TimeType m_last_t;

    bool m_is_volume_based;
    std::deque<Utilities::TimeType> m_t_que;
    std::deque<int> m_v_que;
    std::deque<double> m_p_que;
    bool m_filled_flag;
};

} // namespace Analytics
} // namespace StrategyStudio
} // namespace RCM

#endif