/*==================================================================================
*    Copyright (c) RCM-X, 2012.
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

#ifndef _STRATEGY_STUDIO_ANALYTICS_BOLINGER_BANDS_INDICATOR_H_
#define _STRATEGY_STUDIO_ANALYTICS_BOLINGER_BANDS_INDICATOR_H_

#include "IIndicator.h"
#include "ScalarRollingWindow.h"

namespace RCM {
namespace StrategyStudio {
namespace Analytics {

/**
 * IIndicator is the class interface for Strategy Studio's collection of technical indicators
 * and time series operators
 */
class BollingerBandsIndicator : public IIndicator<double, double> {
public:
    using IIndicator<double, double>::input_type;
    using IIndicator<double, double>::output_type;

public:
    /**
     *Create a bollinger bands indictor using given window size and using nstd for the width of the bands
     */
    BollingerBandsIndicator(size_t window_size, double nstd);
    ~BollingerBandsIndicator();

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
    virtual bool Update(const input_type& observation);

    /**
     * This function is used to input the latest observation with
     * which to update the value of the indicator.
     *
     * @return Returns the updated value of the indicator, or the previous value if the update fails
     */
    virtual const output_type& UpdateAndGetValue(const input_type& observation);

    /**
     * Returns the last value calculated for the BVal
     */
    virtual const output_type& value() const;

    /**
     * Returns whether the indicator's calculations are fully initialized
     */
    virtual bool fully_initialized() const;

    /**
     * Returns the current value of the indicator divided by the band width, i.e. the location in the band
     * 0 is the lower band, 1 is the upper band (the value can be smaller than 0 and larger than 1)
     */
    const output_type BValue();

    /**
     * Returns the value of x relative to the indicator bands, i.e. the location in the band
     * 0 is the lower band, 1 is the upper band (the value can be smaller than 0 and larger than 1)
     */
    const output_type BValue(double x);

    /**
     * Gets the mean value
     */
    const output_type Mean();

    /**
     * Gets the upper band
     */
    const output_type UpperBand();

    /**
     * Gets the lower band
     */
    const output_type LowerBand();

    /**
     * Gets the band width (upper -lower)/mean
     */
    const output_type BandWidth();

    /**
     * Set a new value for the volatility multiplier used to calculate teh band width
     */
    void set_nstd(double nstd) {m_nstd = nstd;}

private:
    bool m_update_flag;
    double m_bval;
    ScalarRollingWindow<double,double> m_rolling_window;
    double m_nstd;
};

} // namespace Analytics
} // namespace StrategyStudio
} // namespace RCM

#endif
