/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/Bond.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_BOND_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_BOND_H_

#include "Instrument.h"
#include "DerivativesTypes.h"
#include <string>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * Bond represents a bond instrument
 */
class Bond : public Instrument {
public:
    typedef std::vector<boost::gregorian::date> CouponDates;

public:
    Bond(InstrumentID instrID,
         const SymbolTag& symbol,
         MarketType marketType,
         const Markets::MarketCenterIDCollection& marketCenterIDs,
         IOrderBookBuilderPtr pOrderBookBuilder,
         MarketCenterInfoFactory* marketCenterInfoFactory);

    virtual ~Bond();

public:
    /**
     * principal is on the same scale as price, typically scaled to par
     */
    double principal() const { return principal_; }

    boost::gregorian::date maturity() const { return maturity_; }

    /**
     * coupon_rate is the percentage of principal that the bond pays per year
     */
    double coupon_rate() const { return coupon_rate_; }

    /**
     * coupon_frequency is the number of coupons per year
     */
    int coupon_frequency() const { return coupon_frequency_; }

    DayCountConvention day_count_convention() const { return day_count_convention_; }
    const CouponDates& coupon_dates() const { return coupon_dates_; }

    boost::gregorian::date call_date() const { return call_date_; }
    double call_strike() const { return call_strike_; }
    boost::gregorian::date put_date() const { return put_date_; }
    double put_strike() const { return put_strike_; }

    // Setters
    void set_principal(double principal) { principal_ = principal; }
    void set_maturity(boost::gregorian::date maturity) { maturity_ = maturity; }
    void set_coupon_rate(double couponRate) { coupon_rate_ = couponRate; }
    void set_coupon_frequency(int cpnFrequency) { coupon_frequency_ = cpnFrequency; }
    void set_day_count_convention(DayCountConvention dcc) { day_count_convention_ = dcc; }
    void set_coupon_dates(const CouponDates& couponDates) { coupon_dates_ = couponDates; }

    void set_call_date(boost::gregorian::date callDate) { call_date_ = callDate; }
    void set_call_strike(double callStrike) { call_strike_ = callStrike; }
    void set_put_date(boost::gregorian::date putDate) { put_date_ = putDate; }
    void set_put_strike(double putStrike) { put_strike_ = putStrike; }

private:
    Bond(const Bond& bond); // not implemented
    Bond& operator=(const Bond& bond); // not implemented

protected:
    double principal_;
    double coupon_rate_;
    double call_strike_;
    double put_strike_;
    CouponDates coupon_dates_;
    boost::gregorian::date maturity_;
    boost::gregorian::date call_date_;
    boost::gregorian::date put_date_;
    DayCountConvention day_count_convention_;
    int coupon_frequency_;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif