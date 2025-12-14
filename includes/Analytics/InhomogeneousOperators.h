/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Analytics/InhomogenousOperators.h
*     Last Update: 2011/07/01 13:55:14
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2011.
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

#ifndef _STRATEGY_STUDIO_ANALYTICS_INHOMOGENEOUS_OPERATORS_H_
#define _STRATEGY_STUDIO_ANALYTICS_INHOMOGENEOUS_OPERATORS_H_

#include "IIndicator.h"

#include <Utilities/TimeType.h>

#include <utility>
#include <limits>
#include <vector>

namespace RCM {
namespace StrategyStudio {
namespace Analytics {

using Utilities::TimeType;
using Utilities::TimeSpanType;

/**
 * InhomogenousOperatorBase is the base class  for Strategy Studio's collection of inhomogenous
 * time series operators
 *
 * Inhomogenious time series operators are designed to efficiently update transformations of time series data
 * where the value of the transformation depends on the time interval between each update, and where the calculation
 * needs to be performed quickly without large memory or computational burdens. These transformations include things like
 * various averaging techniques, and quick calculations of intraday volatility.
 *
 * These implementations are based on the convolution operators described in Dacorogna et al's 'An Introduction to High-Frequency Finance,'
 * which can be referenced for the rationale and math details behind these classes.
 */
class InhomogeneousOperatorBase : public IIndicator<std::pair<TimeType, double>, double> {
public:
    //because we have a templated base class, these using statements are required in order to bring base class members into visibility for the drived class
    using IIndicator<std::pair<TimeType, double>, double>::input_type;
    using IIndicator<std::pair<TimeType, double>, double>::output_type;

    InhomogeneousOperatorBase();

    virtual ~InhomogeneousOperatorBase(){}

    /**
     * This function is used to input the latest observation and observatino time with
     * which to update the value of the operator.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully
     */
    virtual bool Update(TimeType t, double observation)=0;

    /**
     * This function is used to input the latest observation and observatino time with
     * which to update the value of the operator.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully
     */
    virtual output_type UpdateAndGetValue(TimeType t, double observation){Update(t, observation); return value();}

    /**
     * Returns the most recent timestamp used to update the operator
     */
    TimeType timestamp() const {return m_timestamp;};

public: //Inhereted from IIndicator
    /**
     * Resets the state of the indicator
     */
    virtual bool Reset();

    /**
     * This function is used to input the latest observation with
     * which to update the value of the indicator.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully
     */
    virtual bool Update(const input_type& observation){return Update(observation.first, observation.second); }

    /**
     * This function is used to input the latest observation with
     * which to update the value of the indicator.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully
     */
    virtual const output_type& UpdateAndGetValue(const input_type& observation){Update(observation);return value();}

    /**
     * Returns the current value of the indicator
     */
    virtual const output_type& value() const {return m_value;}

    /**
     * Returns whether the indicator's calculations are fully initialized
     */
    virtual bool fully_initialized() const {return m_initialized;}

protected:
    output_type m_value;
    TimeType m_timestamp;
    TimeSpanType m_timeframe;
    bool m_initialized;
};

/**
 * This operator performs an exponentially weighted average on the input data where the weights
 * assigned to prior observations are correctly scaled to take into account the time between
 * each observations
 */
class OperatorEMA : public InhomogeneousOperatorBase {
public:
    /**
     * Constructor for OperatorEMA
     *
     * @param timeframe The mean lifetime, or range, to use for the EMA calculation
     * @param regularization_span The standard EMA calculation will not update an EMA value if it sees a repeat of the same timestamp. In cases of equal timestamps, we can add a regularization interval so the calculation will adjust for the new observation.
     */
    OperatorEMA(TimeSpanType timeframe, TimeSpanType regularization_span = TimeSpanType());

    virtual bool Reset();

    /**
     * This function is used to input the latest observation with
     * which to update the value of the EMA.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully. Will return false if the input t is less than the previous t, in which case the calculation will be skipped
     */
    virtual bool Update(TimeType t, double observation);

    /**
     * Gets the range of the operator, where range is defined as the weighted average value of the time span from current observation factored into the operator's averaging scheme.
     */
    TimeSpanType range() const {return m_timeframe;}

    /**
     * Gets the aspecte ration of the operator, where the aspect ratio is a measure of the shape of the smoothing kernel.
     * A low aspect ratio means shorter required buildup times (as a multiple of the range) before the indicator converges,
     * as there is less weight in the tail of the kernel.
     *
     * A rectangular kernel would have a value of 2/sqrt(3), ema has an aspect_ratio of sqrt(2)
     */
    double aspect_ratio() const {return sqrt(2.0);}

private:
    TimeSpanType m_regularization_span;
};

/**
 * This operator performs a nested series of EMA calculations, where after the
 * the first EMA, the timestamp interpolation scheme makes a small approximate correction for the
 * sampling decay curve of the prior EMA.
 */
class OperatorIteratedEMA : public InhomogeneousOperatorBase {
public:
    /**
     * Constructor for OperatorIteratedEMA
     *
     * @param timeframe The halflife to use for the EMA calculation
     * @param iteration_count The number of EMA iterations to perform on the data. Must be > 1; 1 will be assumed for bad values
     * @param regularization_span A small value can be used here to create spacing between sequential observations that share the same timestamp.
     */
    OperatorIteratedEMA(TimeSpanType timeframe, int iteration_count, TimeSpanType regularization_span = TimeSpanType());

    virtual bool Reset();

    /**
     * This function is used to input the latest observation with
     * which to update the value of the IteratedEMA.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully. Will return false if the input t not greater than the previous time, as no update to the calculation will occur in this case.
     */
    virtual bool Update(TimeType t, double observation);

    /**
     * Gets the range of the operator, where range is defined as the weighted average value of the time span from current observation factored into the operator's averaging scheme.
     */
    TimeSpanType range() const {return m_timeframe*m_iteration_count;}

    /**
     * Gets the aspecte ration of the operator, where the aspect ratio is a measure of the shape of the smoothing kernel.
     * A low aspect ratio means shorter required buildup times (as a multiple of the range) before the indicator converges,
     * as there is less weight in the tail of the kernel.
     *
     * A rectangular kernel would have a value of 2/sqrt(3), ema has an aspect_ratio of sqrt(2)
     */
    double aspect_ratio() const {return sqrt((m_iteration_count+1)/static_cast<double>(m_iteration_count));}

private:
    std::vector<double> m_nested_values;
    std::vector<double> m_prior_observations;
    TimeSpanType m_regularization_span;
    int m_iteration_count;
};

/**
 * This operator approximates a rolling timeframe simple average using an incremental calculation built
 * from a series of IteratedEMA operators.
 */
class OperatorMA : public InhomogeneousOperatorBase {
public:
    /**
     * Constructor for OperatorMA
     *
     * @param timeframe The desired range of the MA operator, where range is the weighted average value of the time span from current observation factored into the operator's averaging scheme.
     * @param m_iterated_ema_count The number of underlying IteratedEMA operators to use to approximate the moving average. Higher counts bring the number closer to a true MA at the cost of more expensive computation
     * @param regularization_span A small value can be used here to create spacing between sequential observations that share the same timestamp.
     */
    OperatorMA(TimeSpanType timeframe, int iterated_ema_count = 7, TimeSpanType regularization_span = TimeSpanType());

    /**
     * Copy constructor for OperatorMA
     */
    OperatorMA(const OperatorMA& operator_ma);

    virtual ~OperatorMA();

    /**
     * Assignment operator for OperatorMA
     */
    OperatorMA& operator= (const OperatorMA& operator_ma);

    virtual bool Reset();

    /**
     * This function is used to input the latest observation with
     * which to update the value of the MA.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully. Will return false if the input t not greater than the previous time, as no update to the calculation will occur in this case.
     */
    virtual bool Update(TimeType t, double observation);

    /**
     * Gets the range of the operator, where range is defined as the weighted average value of the time span from current observation factored into the operator's averaging scheme.
     */
    TimeSpanType range() const {return m_timeframe;};

    /**
     * Gets the aspecte ration of the operator, where the aspect ratio is a measure of the shape of the smoothing kernel.
     * A low aspect ratio means shorter required buildup times (as a multiple of the range) before the indicator converges,
     * as there is less weight in the tail of the kernel.
     *
     * A rectangular kernel would have a value of 2/sqrt(3), ema has an aspect_ratio of sqrt(2)
     */
    double aspect_ratio() const {return sqrt((4.0 * (m_iterated_ema_count + 2.0)) / static_cast<double>(3.0 * (m_iterated_ema_count + 1.0)));}

private:
    std::vector<OperatorIteratedEMA*> m_emas;
    TimeSpanType m_regularization_span;
    int m_iterated_ema_count;
};

/**
 * OperatorMNorm approximates a moving p-norm incrementally, using the OperatorMA for calculations
 *
 * A p-norm is  defined as {sum(abs(observation)^p) ^ (1/p)}. A p value of 2 corresponds to a
 * Euclidean norm; p should be greater than or equal to 1.
 */
class OperatorPNorm : public InhomogeneousOperatorBase {
public:
    /**
     * Constructor for OperatorMNorm
     *
     * @param timeframe The desired range of the MA operator, where range is the weighted average value of the time span from current observation factored into the operator's averaging scheme.
     * @param p The exponent to use in the norm calculation. Should be greater than or equal to 1.
     * @param m_iterated_ema_count The number of underlying IteratedEMA operators to use to approximate the moving average. Higher counts bring the number closer to a true MA at the cost of more expensive computation
     * @param regularization_span A small value can be used here to create spacing between sequential observations that share the same timestamp.
     */
    OperatorPNorm(TimeSpanType timeframe, double p = 2, int iterated_ema_count = 7, TimeSpanType regularization_span = TimeSpanType());

    virtual bool Reset();

    /**
     * This function is used to input the latest observation with
     * which to update the value of the PNorm.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully. Will return false if the input t is less than the previous t, in which case the calculation will be skipped
     */
    virtual bool Update(TimeType t, double observation);

private:
    OperatorMA m_operator_ma;
    double m_p;
};

/**
 * OperatorVariance incrementally approximates a variance measure.
 *
 * Like the PNorm operator, this class allows a specification of the exponent to use;
 * unlike the PNorm operator, this class also demeans the data, using a OperatorMA to
 * approximate the mean as well.
 */
class OperatorVariance : public InhomogeneousOperatorBase {
public:
    /**
     * Constructor for OperatorVariance
     *
     * @param timeframe The desired range of the MA operator, where range is the weighted average value of the time span from current observation factored into the operator's averaging scheme.
     * @param p The exponent to use in the norm calculation. Should be greater than or equal to 1.
     * @param m_iterated_ema_count The number of underlying IteratedEMA operators to use to approximate the moving averages. Higher counts bring the number closer to a true MA at the cost of more expensive computation
     * @param regularization_span A small value can be used here to create spacing between sequential observations that share the same timestamp.
     */
    OperatorVariance(TimeSpanType timeframe, double p = 2, int iterated_ema_count = 7, TimeSpanType regularization_span = TimeSpanType());

    virtual bool Reset();

    /**
     * This function is used to input the latest observation with
     * which to update the value of the Variance.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully. Will return false if the input t is less than the previous t, in which case the calculation will be skipped
     */
    virtual bool Update(TimeType t, double observation);

private:
    OperatorMA m_operator_ma;
    OperatorMA m_mean;
    double m_p;
};

/**
 * OperatorDifferential incrementally calculates a smoothed difference between the
 * recent values of a variable, and its value at a specified lag
 */
class OperatorDifferential : public InhomogeneousOperatorBase {
public:
    /**
     * Constructor for OperatorDifferential
     *
     * @param timeframe The time window to use for the Differential calculation
     * @param regularization_span A small value can be used here to create spacing between sequential observations that share the same timestamp.
     */
    OperatorDifferential(TimeSpanType timeframe, TimeSpanType regularization_span = TimeSpanType());

    virtual bool Reset();

    /**
     * This function is used to input the latest observation with
     * which to update the value of the Differential.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully. Will return false if the input t is less than the previous t, in which case the calculation will be skipped
     */
    virtual bool Update(TimeType t, double observation);

private:
    OperatorIteratedEMA m_iema_a;
    OperatorIteratedEMA m_iema_b;
    OperatorIteratedEMA m_iema_c;
};

/**
 * OperatorAverageActivityPerSecond incrementally calculates a moving average of quantity per second, and
 * can be used to calculate things like volume per second (by inputing volume) or
 * ticks per second (by using 1.0 as input value every time a tick occurs).
 *
 * With this indicator it is very important to define a positive regularization_span. this is because
 * the operator relies on computing an activity density, namely observation/(t+ regularization_span - previous_t), which would
 * cause a divide by zero if two observations arrive with the same timestamp.
 */
class OperatorActivityPerSecond : public InhomogeneousOperatorBase {
public:
    /**
     * Constructor for OperatorActivityPerSecond
     *
     * @param timeframe The time window to use for the ActivityPerSecond calculation; twice the range of the underlying OperatorMA
     * @param m_iterated_ema_count The number of underlying IteratedEMA operators to use to approximate the moving averages. Higher counts bring the number closer to a true MA at the cost of more expensive computation
     * @param regularization_span A small value can be used here to create spacing between sequential observations that share the same timestamp.
     */
    OperatorActivityPerSecond(TimeSpanType timeframe, int iterated_ema_count = 7, TimeSpanType regularization_span = boost::posix_time::microseconds(10));

    virtual bool Reset();

    /**
     * This function is used to input the latest observation with
     * which to update the value of the AverageActivityPerSecond.
     *
     * Unlike the other Operators, it takes two calls to Update to fully initialize this operator. The first call to Update ignores the value of observation and
     * is used just to set a reference start time for the following observation.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully. Will return false if the input t is less than the previous t, in which case the calculation will be skipped
     */
    virtual bool Update(TimeType t, double observation);

private:
    OperatorMA m_operator_ma;
    TimeSpanType m_regularization_span;
};

/**
 * OperatorDampedActivityPerSecond estimates the average of qunatity per second by incrementally calculating
 * a moving average of seconds per quantity (E[dt/values]),
 * and will return the inverse i.e. 1/E[dt/values] as an approximation for E[values/dt].
 * can be used to calculate things like volume per second (by inputing volume) or
 * ticks per second (by using 1.0 as input value every time a tick occurs).
 *
 * With this indicator it is very important to define a positive regularization_span. this is because
 * the operator relies on computing an activity density, namely observation/(t+ regularization_span - previous_t), which would
 * cause a divide by zero if two observations arrive with the same timestamp.
 */
class OperatorDampedActivityPerSecond : public InhomogeneousOperatorBase {
public:
    /**
     * Constructor for OperatorActivityPerSecond
     *
     * @param timeframe The time window to use for the ActivityPerSecond calculation; twice the range of the underlying OperatorMA
     * @param m_iterated_ema_count The number of underlying IteratedEMA operators to use to approximate the moving averages. Higher counts bring the number closer to a true MA at the cost of more expensive computation
     * @param regularization_span A small value can be used here to create spacing between sequential observations that share the same timestamp.
     */
    OperatorDampedActivityPerSecond(TimeSpanType timeframe, int iterated_ema_count = 7, TimeSpanType regularization_span = boost::posix_time::microseconds(10));

    virtual bool Reset();

    /**
     * This function is used to input the latest observation with
     * which to update the value of the AverageActivityPerSecond.
     *
     * Unlike the other Operators, it takes two calls to Update to fully initialize this operator. The first call to Update ignores the value of observation and
     * is used just to set a reference start time for the following observation.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully. Will return false if the input t is less than the previous t, in which case the calculation will be skipped
     */
    virtual bool Update(TimeType t, double observation);

private:
    OperatorMA m_operator_ma;
    TimeSpanType m_regularization_span;
    double m_epsilon;
};

/**
 * OperatorAverageActivityPerSecond estimates the average of qunatity per second incrementally calculating
 * a moving average of teh quantity and a moving average of the tiem difference,
 * and will return the ratio i.e. E[values]/E[dt] as an approximation for E[values/dt].
 * can be used to calculate things like volume per second (by inputing volume) or
 * ticks per second (by using 1.0 as input value every time a tick occurs).
 *
 * With this indicator it is very important to define a positive regularization_span. this is because
 * the operator relies on computing an activity density, namely observation/(t+ regularization_span - previous_t), which would
 * cause a divide by zero if two observations arrive with the same timestamp.
 */
class OperatorAverageActivityPerSecond : public InhomogeneousOperatorBase {
public:
    /**
     * Constructor for OperatorActivityPerSecond
     *
     * @param timeframe The time window to use for the ActivityPerSecond calculation; twice the range of the underlying OperatorMA
     * @param m_iterated_ema_count The number of underlying IteratedEMA operators to use to approximate the moving averages. Higher counts bring the number closer to a true MA at the cost of more expensive computation
     * @param regularization_span A small value can be used here to create spacing between sequential observations that share the same timestamp.
     */
    OperatorAverageActivityPerSecond(TimeSpanType timeframe, int iterated_ema_count = 7, TimeSpanType regularization_span = boost::posix_time::microseconds(10));

    virtual bool Reset();

    /**
     * This function is used to input the latest observation with
     * which to update the value of the AverageActivityPerSecond.
     *
     * Unlike the other Operators, it takes two calls to Update to fully initialize this operator. The first call to Update ignores the value of observation and
     * is used just to set a reference start time for the following observation.
     *
     * @return Returns true if the observation was accepted and the indicator updated succesfully. Will return false if the input t is less than the previous t, in which case the calculation will be skipped
     */
    virtual bool Update(TimeType t, double observation);

private:
    OperatorMA m_operator_ma_observation;
    OperatorMA m_operator_ma_time;
    TimeSpanType m_regularization_span;
    double m_epsilon;
};

} // namespace Analytics
} // namespace StrategyStudio
} // namespace RCM

#endif