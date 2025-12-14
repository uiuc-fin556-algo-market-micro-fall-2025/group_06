/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Analytics/RollingWindow.h
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

#ifndef _STRATEGY_STUDIO_ANALYTICS_ROLLING_WINDOW_H_
#define _STRATEGY_STUDIO_ANALYTICS_ROLLING_WINDOW_H_

#include <boost/circular_buffer.hpp>
#include <cmath>
#include <iostream>

namespace RCM {
namespace StrategyStudio {
namespace Analytics {

/**
 * Simple class for containing a rolling window of simple numeric data and getting summary statistics about the data in the window.
 *
 * This class can be used instead of the RollingWindow time series type for basic rolling calculations when the timestamp is not
 * important or when more extensive indicators are not needed. This container does not provide built in thread safety.
 */
template <typename T, typename StatsType=double>
class ScalarRollingWindow {
public:
    typedef boost::circular_buffer<T> C;

    //all iterators need to be const. allowing non const iterators would allow elements to be modified without triggering an update of the summary stats
    typedef typename C::const_iterator iterator;
    typedef typename C::const_reverse_iterator reverse_iterator;
    typedef typename C::const_iterator const_iterator;
    typedef typename C::const_reverse_iterator const_reverse_iterator;

    typedef typename C::value_type value_type;
    typedef typename C::allocator_type::pointer pointer;
    typedef typename C::allocator_type::const_pointer const_pointer;
    typedef typename C::allocator_type::reference reference;
    typedef typename C::allocator_type::const_reference const_reference;

    typedef typename C::size_type size_type;
    typedef typename C::difference_type difference_type;

public:
    /**
     * Class constructor
     *
     * @param window_size The number of observations to store in the rolling window
     */
    explicit ScalarRollingWindow(size_t window_size):
        c(window_size),
        m_rolling_sum(0),
        m_online_mean(0),
        m_online_m2(0)
        {}

    iterator begin()  { return c.begin(); }
    iterator end()  { return c.end(); }

    const_iterator begin() const { return c.begin(); }
    const_iterator end() const { return c.end(); }

    reverse_iterator rbegin() { return c.rbegin();}
    reverse_iterator rend() { return c.rend();}

    const_reverse_iterator rbegin() const {return c.rbegin();}
    const_reverse_iterator rend() const { return c.rend();}

    /**
     * Gets the data point at the index position in the rolling window
     *
     * Only a const version of this function is supplied, as non const access would invalidate the summary stats
     *
     * Precondition: 0 <= index < size()
     * @param index The position of the element to retrieve
     * @returns A const reference to the element at the index position
     */
    const_reference operator[](size_type index) const {return c[index];}

    /**
     * Gets the data point at the first position in the rolling window
     *
     * Only a const version of this function is supplied, as non const access would invalidate the summary stats
     *
     * Precondition: !empty()
     * @returns A const reference to the element at the first position
     */
    const_reference front() const {return c.front();}

    /**
     * Gets the data point at the last position in the rolling window
     *
     * Only a const version of this function is supplied, as non const access would invalidate the summary stats
     *
     * Precondition: !empty()
     * @returns A const reference to the element at the first position
     */
    const_reference back() const { return c.back();}

    /**
     * Gets the number of data points contained in the rolling window
     */
    size_type size() const { return c.size(); }

    /**
     * Gets the window size, meaning the maximum number of data points the rolling window is configured to contain
     */
    size_type window_size() const { return c.capacity(); }

    bool empty() const { return c.empty();}

    /**
     * Checks whether the number of data points contained by the rolling window equals the window size.
     */
    bool full() const { return c.full(); }

    /**
     * Inserts a new value to the end of the rolling window. If the window is full, the oldest data point will fall out of the window.
     *
     * This function assumes users add data to the window in a meaningful order
     */
    void push_back(T val)
    {
        size_type N = c.size();
        StatsType delta = 0;
        if (c.full()) {
            StatsType f=c.front();
            --N;
            m_rolling_sum -= f;
            delta = (f - m_online_mean);
            m_online_mean = (N > 0) ? m_online_mean - delta / N : 0;
            m_online_m2 = (N > 0) ? m_online_m2 - delta * (f - m_online_mean) : 0;
        }

        c.push_back(val);
        ++N;
        m_rolling_sum += val;
        delta = (val - m_online_mean);
        m_online_mean = (N < 2) ? val : m_online_mean + delta / N;
        m_online_m2 = m_online_m2 + delta * (val - m_online_mean);
    }

    /**
     * Removes all stored elements from the RollingWindow and resets internal calculations
     *
     * Invalidates all iterators except for end()
     */
    void clear()
    {
        c.clear();
        m_rolling_sum = 0;
        m_online_mean = 0;
        m_online_m2 = 0;
    }

    /**
     * Calculates the average value of the data points in the rolling window
     *
     * Precondition: size() > 0
     */
    StatsType Mean() const
    {
        return m_online_mean;//(m_rolling_sum / static_cast<StatsType>(c.size()));
    }

    /**
     * Calculates the standard deviation of the data points in the rolling window
     *
     * Precondition: size() > 1
     */
    StatsType StdDev() const
    {
        size_type N = c.size();
        return ((N > 1 && m_online_m2 > 0) ? std::sqrt(m_online_m2 / (N - 1)) : 0); //return the unbiased estimator
    }

    /**
     * Calculates the sum of the data points in the rolling window
     */
    StatsType Sum() const
    {
        return m_rolling_sum;
    }

    /**
     * Calculates the ZScore of the most recent point added to the rolling window.
     *
     * The ZScore is also known as a standard score, it indicates how many standard deviations an abservation lies above or below its mean
     * Precondition: size()>1
     */
    StatsType ZScore() const
    {
        return (c.back() - Mean()) / StdDev();
    }

    /**
     * Calculates the ZScore of the observation located at the specified index in the rolling window.
     *
     * The ZScore is also known as a standard score, it indicates how many standard deviations an abservation lies above or below its mean
     * Precondition: size()>1
     */
    StatsType ZScore(size_type index) const
    {
        return (c[index] - Mean()) / StdDev();
    }

private:
    C c;
    StatsType m_rolling_sum;
    StatsType m_online_mean;
    StatsType m_online_m2;
};

template<typename T, typename StatsType>
inline std::ostream& operator<<(std::ostream& out, const ScalarRollingWindow<T,StatsType>& window)
{
    out << "WINDOW " << window.window_size()
        << " NumPoints: " << window.size()
        << " Sum: " << window.Sum()
        << " Mean: " << window.Mean()
        << " StdDev: " << window.StdDev()
        << " Data: ";

    for (typename ScalarRollingWindow<T,StatsType>::const_iterator it = window.begin(); it != window.end(); ++it) {
        out << *it << " ";
    }

    return out;
}

} // namespace Analytics
} // namespace StrategyStudio
} // namespace RCM

#endif