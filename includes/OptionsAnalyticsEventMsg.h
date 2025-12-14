/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/OptionsAnalyticsEventMsg.h
*     Last Update: 2016/12/05 13:55:14
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2016.
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

#ifndef _STRATEGY_STUDIO_LIB_OPTIONS_ANALYTICS_MSG_H_
#define _STRATEGY_STUDIO_LIB_OPTIONS_ANALYTICS_MSG_H_

#include "DataTypes.h"
#include "DataTypesUtils.h"
#include "Event.h"
#include "MarketDataBaseEventMsg.h"

namespace RCM {
namespace StrategyStudio {

/**
 * OptionsAnalyticsEventMsg containing updates of options greeks and implied vols
 */
class OptionsAnalyticsEventMsg : public MarketDataBaseEventMsg {
public:
    OptionsAnalyticsEventMsg(Event* event,
                             Option* option,
                             double implied_vol,
                             double bid_implied_vol,
                             double ask_implied_vol,
                             double delta,
                             double gamma,
                             double theta,
                             double vega,
                             double rho,
                             double reference_price1,
                             double reference_price2,
                             double underlier_reference_price,
                             TimeType source_time,
                             TimeType feed_handler_time,
                             TimeType adapter_time);

    std::string name() const;

    /**
     * Returns a const reference to the relevant instrument
     */
    const Option& option() const { return *option_; }

    /**
     * Returns the implied vol of the option
     *
     * Percentage represented in decimal form, eg .3 means 30%
     */
    double implied_vol() const { return implied_vol_; }

    /**
     * Returns the bid implied vol of the option if available, or quiet_NaN otherwise
     */
    double bid_implied_vol() const { return bid_implied_vol_; }

    /**
     * Returns the ask implied vol of the option if available, or quiet_NaN otherwise
     */
    double ask_implied_vol() const { return ask_implied_vol_; }

    /**
     * Returns the delta of the option
     *
     * Scaled as +-1.0
     */
    double delta() const { return delta_; }

    /**
     * Returns the gamma of the option
     */
    double gamma() const { return gamma_; }

    /**
     * Returns the theta of the option
     */
    double theta() const { return theta_; }

    /**
     * Returns the vega of the option
     */
    double vega() const { return vega_; }

    /**
     * Returns the rho of the option if available, or quiet_NaN otherwise
     */
    double rho() const { return rho_; }

    /**
     * Returns the option price associated with the implied vols and greeks, or the option bid if bid/ask vols are available
     */
    double reference_price1() const { return reference_price1_; }

    /**
     * Returns the option ask price associated with the implied vols and greeks if bid/ask vols are available, or quiet_NaN otherwise
     */
    double reference_price2() const { return reference_price2_; }

    /**
     * Returns the underlier price associated with the implied vols and greeks
     */
    double underlier_reference_price() const { return underlier_reference_price_; }

    /**
     * Gets the upate time of the analytics message
     */
    TimeType source_time() const { return source_time_; }

    /**
     * Gets the time the feedhandler processed the market data event, if applicable and available
     */
    TimeType feed_handler_time() const { return feed_handler_time_; }

    /**
     * Gets the time when the Strategy Server's data adapter received the market data event
     */
    TimeType adapter_time() const { return adapter_time_; }

private: /* Event msg hooks */
    /**
     * Handles updating of the instrument before handing instrument to strategy
     */
    virtual bool PreProcess();

private:
    Option* option_;
    double implied_vol_;
    double bid_implied_vol_;
    double ask_implied_vol_;
    double delta_;
    double gamma_;
    double theta_;
    double vega_;
    double rho_;
    double reference_price1_;
    double reference_price2_;
    double underlier_reference_price_;
    TimeType source_time_;
    TimeType feed_handler_time_;
    TimeType adapter_time_;
};

inline std::string OptionsAnalyticsEventMsg::name() const
{
    return "OPTIONS_ANALYTICS_EVENT_" + option_->symbol();
}

} // namespace StrategyStudio
} // namespace RCM

#endif