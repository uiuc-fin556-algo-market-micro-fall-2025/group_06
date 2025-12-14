/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Analytics/TimeSeriesContainers.h
*     Last Update: 2010/09/30 13:55:14
*     Contents: TimeSeriesBase, TimeSeries, RollingWindow class definitions
*     Distribution: External, defines time series collection interfaces for use by strategy developers
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

#ifndef _STRATEGY_STUDIO_ANALYTICS_SERIES_CONTAINERS_H_
#define _STRATEGY_STUDIO_ANALYTICS_SERIES_CONTAINERS_H_

#include <boost/circular_buffer.hpp>
#include <vector>
#include <utility>
#include <functional>
#include <iterator>
#include <stdexcept>

namespace RCM {
namespace StrategyStudio {
namespace Analytics {

//forward declarations
template <typename T1,typename T2> struct mutable_pair;

template<typename T1, typename T2>
inline mutable_pair<T1, T2> make_mutable_pair(T1 val1, T2 val2);

/**
 * Provides a pair structure in which the second element is always mutable.
 *
 * This simple utility struct helps Strategy Studio's time series containers protect sort order by keeping the time stamp constant while
 * still allowing changes to the mapped_value. It follows the conventions of std::pair and provides implicit conversion
 * to and construction from an std::pair
 */
template <typename T1,typename T2>
struct mutable_pair {
    typedef T1 first_type;
    typedef T2 second_type;

    mutable_pair():first(T1()),second(T2()){}
    mutable_pair(const T1& f,const T2& s):first(f),second(s){}
    mutable_pair(const std::pair<T1,T2>& p):first(p.first),second(p.second){}

    operator std::pair<T1,T2>(){ return make_mutable_pair<T1,T2>(first,second);}

    T1 first;
    mutable T2 second;
};

/**
 * Helper function for easy construction of a mutable_pair
 */
template<typename T1, typename T2>
inline mutable_pair<T1, T2> make_mutable_pair(T1 val1, T2 val2)
{
    return (mutable_pair<T1, T2>(val1, val2));
}

/**
 * Common base class for Strategy Studio's time series containers
 *
 * Strategy Studio's time series containers enforce a sorted state, but assume data is usually added in sequence.
 * While these containers are robust to out-of-order data, if out of order insertion is common,
 * there are better containers to use.
 * Usage semantics and sort safety are similar to associative containers such as std::multimap,
 * with the exception that index operator[]() is always based on fast random access vector-style index number lookup,
 * rather than key comparison. Furthermore, iterators and const_iterators are identicle and both semi-mutable; modifications
 * to TimeKey are always prohibited, but mapped_type is mutable, even when accessed through a const_iterator or value_type.
 * These containers do not provide built in thread safety.
 */
template<class TimeKey, class T, class C, class Derived, class Cmp = std::less<TimeKey> >
class TimeSeriesBase {
public:
    /**
     * Using container_type's iterator and reverse_iterator is not safe, as algorithms my use them to assign value_type or modify key_type, invalidating sorted status
     * This safer workaround is to only allow const iterators, allowing modifications to mapped_type by using mutable in the underlying element storage type
     */
    typedef typename C::const_iterator iterator;
    typedef typename C::const_reverse_iterator reverse_iterator;
    typedef typename C::const_iterator const_iterator;
    typedef typename C::const_reverse_iterator const_reverse_iterator;

    typedef TimeKey key_type;
    typedef T mapped_type;
    typedef mutable_pair<TimeKey, T> value_type;
    typedef C container_type;

    typedef Cmp key_compare;

    typedef typename C::allocator_type::pointer pointer;
    typedef typename C::allocator_type::const_pointer const_pointer;
    typedef typename C::allocator_type::reference reference;
    typedef typename C::allocator_type::const_reference const_reference;
    typedef typename C::size_type size_type;
    typedef typename C::difference_type difference_type;

    class value_compare :  public std::binary_function<value_type, value_type, bool> {
    protected:
        Cmp cmp;
    public:
        bool operator()(const value_type& x, const value_type& y) const { return cmp(x.first, y.first);}
        value_compare(Cmp c) : cmp(c) {}
    };

    key_compare key_comp() const {return cmp;}
    value_compare value_comp() const {return value_compare(cmp);}

    iterator insert(const key_type& t, const mapped_type& mt)
    {
        return static_cast<Derived*>(this)->insert(make_mutable_pair(t, mt));
    }

    iterator insert(const value_type& val)
    {
        return static_cast<Derived*>(this)->insert(val);
    }

    void clear() {c.clear();}

    /**
     * Finds the mapped_type value of most recent observation prior to specified time, using binary search
     *
     * @param t time stamp before which to look for the closest observation
     * @throws out_of_range exception if specified time is precedes the beginning of the collection
     */
    mapped_type& most_recent(const key_type& t)
    {
        //note all calls to functions like upper_bound, lower_bound, binary_search must compare on the value_type of the container. To avoid
        //making a pair with a dummy mapped_type() we'd have to store the keys and values in separate containers, and we could no longer offer multimap style semantics
        typename C::iterator it = std::upper_bound(c.begin(), c.end(), make_mutable_pair(t, mapped_type()), vcmp);
        if (it == c.begin()) throw(std::out_of_range("Input Time of function [most_recent] precedes first element of the time series"));
        return (--it)->second;
    }

    const mapped_type& most_recent(const key_type& t) const
    {
        typename C::const_iterator it = std::upper_bound(c.begin(), c.end(), make_mutable_pair(t, mapped_type()), vcmp);
        if (it==c.begin()) throw(std::out_of_range("Input Time of function [most_recent] precedes first element of the time series"));
        return (--it)->second;
    }

    /**
     * Finds the mapped_type value of the first observation at exactly the specified time, using binary search
     *
     * @param t time stamp at which to look for an observation
     * @throws out_of_range exception if no observation exists at the input time
     */
    mapped_type& at(const key_type& t)
    {
        iterator fnd = find(t);
        if (fnd==end()) throw(std::out_of_range("Input Time of function [at] was not found in time series"));
        return fnd->second;
    }

    const mapped_type& at(const key_type& t) const
    {
        const_iterator fnd = find(t);
        if (fnd==end()) throw(std::out_of_range("Input Time of function [at] was not found in time series"));
        return fnd->second;
    }

    /**
     * Finds the first observation at exactly the specified time, using binary search
     *
     * @param t time stamp at which to look for an observation
     * @returns an iterator to the observation, or if no observation matches the input time, returns end()
     */
    iterator find(const key_type& t)
    {
        typename C::iterator lb = std::lower_bound(c.begin(), c.end(), make_mutable_pair(t, mapped_type()), vcmp);
        if (lb != c.end() && !cmp(t, lb->first))
            return iterator(lb);
        else return end();
    }

    const_iterator find(const key_type& t) const
    {
        typename C::iterator lb = std::lower_bound(c.begin(), c.end(), make_mutable_pair(t, mapped_type()), vcmp);
        if (lb != c.end() && !cmp(t, lb->first))
            return lb;
        else return end();
    }

    /**
     * Checks whether the time series contains an observation at exactly the specified time, using binary search
     *
     * @param t time stamp at which to look for an observation
     * @returns true if an observation is found, otherwise false
     */
    bool containsTime(const key_type& t)
    {
        return std::binary_search(c.begin(), c.end(), make_mutable_pair(t, mapped_type()),vcmp);
    }

    bool empty() const {return c.empty();}
    size_type size() const {return c.size();}

    mapped_type& first() {return c.front().second;}
    const mapped_type& first() const {return c.front().second;}

    mapped_type& last() { return c.back().second;}
    const mapped_type& last() const { return c.back().second;}

    //don't allow non-const access to value_type, else users could violate sort by modifying the timekey
    const_reference front() const {return c.front();}
    const_reference back() const { return c.back();}

    /**
     * Returns the observation at the nth position index in the container.
     *
     * Note, unlike stl associative containers, this will not perform a lookup based on time key
     * To search by time, use find for exact macthes, or most_recent to get the most recent observation before an input time.
     *
     * @param n position at which to look for an observation, which will be in sorted order
     * @returns reference to the observation at the specified index
     */
    mapped_type& operator[] (size_type n) {return c[n].second;}
    const mapped_type& operator[] (size_type n) const {return c[n].second;}

    /**
     * Returns the observation n before the last observation in the container.
     *
     * @param n number of positions before the last observation to lookup
     * @returns reference to the observation at the specified index
     */
    mapped_type& nth_ago(size_type n) {return c[c.size()-n-1].second;}
    const mapped_type& nth_ago(size_type n) const {return c[c.size()-n-1].second;}

    iterator begin(){ return iterator(c.begin());}
    const_iterator begin() const {return c.begin();}
    iterator end(){ return iterator(c.end());}
    const_iterator end() const {return c.end();}
    reverse_iterator rbegin(){ return reverse_iterator(iterator( c.rbegin().base()));}
    const_reverse_iterator rbegin() const {return c.rbegin();}
    reverse_iterator rend(){ return reverse_iterator(iterator( c.rend().base()));}
    const_reverse_iterator rend() const { return c.rend();}

protected:
    C c;
    Cmp cmp;
    value_compare vcmp;

    explicit TimeSeriesBase(const Cmp& compare = Cmp()) : c(C()), cmp(compare), vcmp(value_compare(cmp)) {}
    TimeSeriesBase(const TimeSeriesBase<TimeKey,T,C,Derived,Cmp>& ts):c(ts.c),cmp(ts.cmp),vcmp(ts.vcmp){}
};

/**
 * Time series container specialized for storing a cumulating pool of data.
 */
template<class TimeKey, class T, class Cmp = typename std::less<TimeKey> >
class TimeSeries : public TimeSeriesBase<TimeKey, T, std::vector< mutable_pair<TimeKey, T> >, TimeSeries<TimeKey, T, Cmp>,Cmp> {
public:

    //because we have a templated base class, these using statements are required in order to bring base class members into visibility for the drived class
    using TimeSeriesBase<TimeKey, T, std::vector< mutable_pair<TimeKey, T> >, TimeSeries<TimeKey, T, Cmp>,Cmp>::c;
    using TimeSeriesBase<TimeKey, T, std::vector< mutable_pair<TimeKey, T> >, TimeSeries<TimeKey, T, Cmp>,Cmp>::cmp;
    using TimeSeriesBase<TimeKey, T, std::vector< mutable_pair<TimeKey, T> >, TimeSeries<TimeKey, T, Cmp>,Cmp>::vcmp;
    using TimeSeriesBase<TimeKey, T, std::vector< mutable_pair<TimeKey, T> >, TimeSeries<TimeKey, T, Cmp>,Cmp>::end;

    /**
     * Constructor for the cumulating TimeSeries
     *
     * @param n Preallocates n observation items.
     */
    explicit TimeSeries(typename TimeSeries<TimeKey, T, Cmp>::size_type n = 0, const Cmp& compare = Cmp()):
        TimeSeriesBase<TimeKey, T, typename std::vector< mutable_pair<TimeKey, T> >, TimeSeries<TimeKey, T, Cmp>,Cmp>(compare)
    {
        c.reserve(n);
    }

    TimeSeries(const TimeSeries<TimeKey, T, Cmp>& ts):
        TimeSeriesBase<TimeKey, T, typename std::vector< mutable_pair<TimeKey, T> >, TimeSeries<TimeKey, T, Cmp>,Cmp>(ts){};

    TimeSeries& operator= (TimeSeries<TimeKey, T, Cmp>& ts)
    {
        c = ts.c;
        cmp = ts.cmp;
        vcmp = ts.vcmp;
        return *this;
    }

    typename TimeSeries::iterator insert(const typename TimeSeries::key_type& t, const typename TimeSeries::mapped_type& mt)
    {
        return insert(make_mutable_pair(t, mt));
    }

    typename TimeSeries::iterator insert(const typename TimeSeries::value_type& val)
    {
        if (c.size()>0 && cmp(val.first,c.back().first)) {
            return typename TimeSeries::iterator(c.insert(std::upper_bound(c.begin(), c.end(), val, vcmp), val));
        } else {
            c.push_back(val);
            return --end();
        }
    }
};

/**
 * Time series container specialized for storing a rolling window of observations, containing up to a fixed number of observations.
 */
template<class TimeKey, class T, class Cmp = std::less<TimeKey> >
class RollingWindow : public TimeSeriesBase<TimeKey, T, boost::circular_buffer<mutable_pair<TimeKey, T> >, RollingWindow<TimeKey, T, Cmp>, Cmp> {
public:

    //because we have a templated base class, these using statements are required in order to bring base class members into visibility for the drived class
    using TimeSeriesBase<TimeKey, T, boost::circular_buffer<mutable_pair<TimeKey, T> >, RollingWindow<TimeKey, T, Cmp>, Cmp>::c;
    using TimeSeriesBase<TimeKey, T, boost::circular_buffer<mutable_pair<TimeKey, T> >, RollingWindow<TimeKey, T, Cmp>, Cmp>::cmp;
    using TimeSeriesBase<TimeKey, T, boost::circular_buffer<mutable_pair<TimeKey, T> >, RollingWindow<TimeKey, T, Cmp>, Cmp>::vcmp;
    using TimeSeriesBase<TimeKey, T, boost::circular_buffer<mutable_pair<TimeKey, T> >, RollingWindow<TimeKey, T, Cmp>, Cmp>::end;

    /**
     * Constructor for the RollingWindow of time series data
     *
     * @param n The number of observations to keep a window of; the size of the rolling window.
     */
    explicit RollingWindow(typename RollingWindow<TimeKey, T, Cmp>::size_type n = 0, const Cmp& compare = Cmp()): TimeSeriesBase<TimeKey, T, boost::circular_buffer<mutable_pair<TimeKey, T> >, RollingWindow<TimeKey, T, Cmp>, Cmp>(compare)
    {
        c.set_capacity(n);
    }

    RollingWindow(const RollingWindow<TimeKey, T, Cmp>& rw): TimeSeriesBase<TimeKey, T, boost::circular_buffer<mutable_pair<TimeKey, T> >, RollingWindow<TimeKey, T, Cmp>, Cmp>(rw){}

    RollingWindow& operator= (RollingWindow<TimeKey, T, Cmp>& rw)
    {
        c = rw.c;
        cmp = rw.cmp;
        vcmp = rw.vcmp;
        return *this;
    }

    /**
     * Indicates whether the RollowingWindow is filled to capacity, meaning whether it currently has as many data points as its window size
     */
    bool full() const
    {
        return c.full();
    }

    /**
     * Acessor which retrieves the number of points the rolling window is configured to contain
     */
    typename RollingWindow::size_type window_size() const
    {
        return c.capacity();
    }

    typename RollingWindow::iterator insert(const typename RollingWindow::key_type& t, const typename RollingWindow::mapped_type& mt)
    {
        return insert(make_mutable_pair(t, mt));
    }

    typename RollingWindow::iterator insert(const typename RollingWindow::value_type& val)
    {
        //boost::circular_buffer's insert function will not insert when pos points to begin() & window is full, this is the desired behaviour.
        //note in this case insert's return value will still be begin()
        if (c.size()>0 && cmp(val.first,c.back().first)) {
            return typename RollingWindow::iterator(c.insert(std::upper_bound(c.begin(), c.end(), val, vcmp),val));
        } else {
            c.push_back(val);
            return --end();
        }
    }
};


/**
 * Time series container specialized for storing a fixed time span of observations, containing a variable number of observations.
 *
 * @param TimeKey The class used to represent a time type
 * @param T The class making up the observed value in the time series
 * @param TimeSpan The class used to represent a period of time; may be different than the TimeKey which represents a moment in time
 */
template<class TimeKey, class T, class TimeSpan, class Cmp = std::less<TimeKey> >
class TimeWindow : public TimeSeriesBase<TimeKey, T, std::deque<mutable_pair<TimeKey, T> >, TimeWindow<TimeKey, T, TimeSpan, Cmp>, Cmp> {
public:

    //because we have a templated base class, these using statements are required in order to bring base class members into visibility for the drived class
    using TimeSeriesBase<TimeKey, T, std::deque<mutable_pair<TimeKey, T> >, TimeWindow<TimeKey, T, TimeSpan, Cmp>, Cmp>::c;
    using TimeSeriesBase<TimeKey, T, std::deque<mutable_pair<TimeKey, T> >, TimeWindow<TimeKey, T, TimeSpan, Cmp>, Cmp>::cmp;
    using TimeSeriesBase<TimeKey, T, std::deque<mutable_pair<TimeKey, T> >, TimeWindow<TimeKey, T, TimeSpan, Cmp>, Cmp>::vcmp;
    using TimeSeriesBase<TimeKey, T, std::deque<mutable_pair<TimeKey, T> >, TimeWindow<TimeKey, T, TimeSpan, Cmp>, Cmp>::end;
    using TimeSeriesBase<TimeKey, T, std::deque<mutable_pair<TimeKey, T> >, TimeWindow<TimeKey, T, TimeSpan, Cmp>, Cmp>::begin;

    /**
     * Constructor for the RollingWindow of time series data
     *
     * @param window_size The period of time for which to keep data;
     */
    explicit TimeWindow(TimeSpan window_size, const Cmp& compare = Cmp()):
        TimeSeriesBase<TimeKey, T, std::deque<mutable_pair<TimeKey, T> >, TimeWindow<TimeKey, T, TimeSpan, Cmp>, Cmp>(compare), m_window_size(window_size){}

    TimeWindow(const TimeWindow<TimeKey, T, Cmp>& rw): TimeSeriesBase<TimeKey, T, std::deque<mutable_pair<TimeKey, T> >, TimeWindow<TimeKey, T, TimeSpan, Cmp>, Cmp>(rw), m_window_size(rw.m_window_size){};

    TimeWindow& operator= (TimeWindow<TimeKey, T, Cmp>& rw)
    {
        c=rw.c;
        cmp=rw.cmp;
        vcmp=rw.vcmp;
        m_window_size=rw.m_window_size;
        return *this;
    }

    /**
     * Acessor which retrieves the period of time for which the window is configured to store data
     */
    TimeSpan window_size()
    {
        return m_window_size;
    }

    /**
     * Tells the container a new current time. Used to delete stale data without inserting a new observation.
     *
     * @param t The new timestamp
     */
    void UpdateCurrentTime(TimeKey t)
    {
        while (c.size()>0 && cmp(c.front().first+m_window_size, t)) {
            c.pop_front();
        }

        while (c.size()>0 && cmp(t,c.back().first)) {
            c.pop_back();
        }
    };

    typename TimeWindow::iterator insert(const typename TimeWindow::key_type& t, const typename TimeWindow::mapped_type& mt)
    {
        return insert(make_mutable_pair(t, mt));
    }

    typename TimeWindow::iterator insert(const typename TimeWindow::value_type& val)
    {
        //delete values from front that fall out of range
        while (c.size()>0 && cmp(c.front().first+m_window_size, val.first)) {
            c.pop_front();
        }

        if (c.size()>0 && cmp(val.first,c.back().first)) {
            //if we're trying to  inserting to the beginning but the resulting timespan is too big insert nothing and return iterator to begin()
            if (cmp(val.first+m_window_size, c.back().first)) {
                return begin();
            }
            return typename TimeWindow::iterator(c.insert(std::upper_bound(c.begin(), c.end(), val, vcmp),val));
        } else {
            c.push_back(val);
            return --end();
        }
    }

private:
    TimeSpan m_window_size;
};

template <class TimeKey, class T, class C, class Derived, class Cmp>
inline bool operator == (const TimeSeriesBase<TimeKey, T, C, Derived, Cmp>& lhs, const TimeSeriesBase<TimeKey, T, C, Derived, Cmp>& rhs)
{
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class TimeKey, class T, class C, class Derived, class Cmp>
inline bool operator != (const TimeSeriesBase<TimeKey, T, C, Derived, Cmp>& lhs, const TimeSeriesBase<TimeKey, T, C, Derived, Cmp>& rhs)
{
    return !(lhs == rhs);
}

template <class TimeKey, class T, class C, class Derived, class Cmp>
inline bool operator < (const TimeSeriesBase<TimeKey, T, C, Derived, Cmp>& lhs, const TimeSeriesBase<TimeKey, T, C, Derived, Cmp>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class TimeKey, class T, class C, class Derived, class Cmp>
inline bool operator > (const TimeSeriesBase<TimeKey, T, C, Derived, Cmp>& lhs, const TimeSeriesBase<TimeKey, T, C, Derived, Cmp>& rhs)
{
    return rhs < lhs;
}

template <class TimeKey, class T, class C, class Derived, class Cmp>
inline bool operator <= (const TimeSeriesBase<TimeKey, T, C, Derived, Cmp>& lhs, const TimeSeriesBase<TimeKey, T, C, Derived, Cmp>& rhs)
{
    return !(rhs < lhs);
}

template <class TimeKey, class T, class C, class Derived, class Cmp>
inline bool operator >= (const TimeSeriesBase<TimeKey, T, C, Derived, Cmp>& lhs, const TimeSeriesBase<TimeKey, T, C, Derived, Cmp>& rhs)
{
    return !(lhs < rhs);
}

} // namespace Analytics
} // namespace StrategyStudio
} // namespace RCM

#endif