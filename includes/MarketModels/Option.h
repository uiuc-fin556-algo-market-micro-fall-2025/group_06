/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/Option.h
*     Last Update: 2016/12/05 13:15:14
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2009 - 2016.
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_OPTIONS_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_OPTIONS_H_

#include "Instrument.h"
#include "DerivativesTypes.h"
#include <string>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * Option class represents a vanilla derivative on an equity or future
 */
class Option : public Instrument {
public:
    Option(InstrumentID instrID,
           const SymbolTag& symbol,
           OptionType optionType,
           MarketType marketType,
           const Markets::MarketCenterIDCollection& marketCenterIDs,
           IOrderBookBuilderPtr pOrderBookBuilder,
           MarketCenterInfoFactory* marketCenterInfoFactory);

    ~Option();

    /**
     * Returns the symbol of the underlier
     */
    const SymbolTag& underlier_symbol() const { return underlier_symbol_; }
    void set_underlier_symbol(const SymbolTag& symbol) { underlier_symbol_ = symbol; }

    /**
     * Returns whether the option is a put or a call
     */
    OptionType put_call() const { return option_type_; }
    void set_put_call(OptionType putCall) { option_type_ = putCall; }

    /**
     * Returns the expiration date of the option
     */
    DateType expiration_date() const { return expiration_date_; }
    void set_expiration_date(DateType expiry) { expiration_date_ = expiry; }

    /**
     * Returns the strike of the option
     */
    double strike() const { return strike_; }
    void set_strike(double strike) { strike_ = strike; }

    /**
     * Returns the exercise style of the option
     */
    ExerciseStyle exercise_style() const { return exercise_style_; }
    void set_exercise_style(ExerciseStyle style) { exercise_style_ = style; }

    /**
     * Returns the delivery type of the option
     */
    DeliveryType delivery_type() const { return delivery_type_; }
    void set_delivery_type(DeliveryType deliveryType) { delivery_type_ = deliveryType; }

    /**
     * Returns number of units of underlier deliverable upon exercise
     *
     * May differ from multiplier for adjusted contracts
     */
    int units() const { return units_; }
    void set_units(int units) { units_ = units; }

    /**
     * Returns the cash amount deliverable upon exercise
     *
     * May be non-zero for adjusted contracts
     */
    double deliverable_cash() const { return deliverable_cash_; }
    void set_deliverable_cash(double deliverableCash) { deliverable_cash_ = deliverableCash; }

    /**
     * Returns whether the option has non-standard deliverables
     */
    bool is_adjusted_contract() const { return is_adjusted_contract_; }
    void set_is_adjusted_contract(bool isAdjustedContract) { is_adjusted_contract_ = isAdjustedContract; }

    /**
     * Returns the daily open interest number if available, or -1 otherwise
     */
    int open_interest() const { return open_interest_; }
    void set_open_interest(int openInterest) { open_interest_ = openInterest; }

    /**
     * Returns the implied vol of the option, or quiet_NaN if options analytics are not present
     *
     * Percentage represented in decimal form, eg .3 means 30%
     */
    double implied_vol() const { return implied_vol_; }
    void set_implied_vol(double ivol) { implied_vol_ = ivol; }

    /**
     * Returns the bid implied vol of the option if available, or quiet_NaN otherwise
     */
    double bid_implied_vol() const { return bid_implied_vol_; }
    void set_bid_implied_vol(double ivol) { bid_implied_vol_ = ivol; }

    /**
     * Returns the ask implied vol of the option if available, or quiet_NaN otherwise
     */
    double ask_implied_vol() const { return ask_implied_vol_; }
    void set_ask_implied_vol(double ivol) { ask_implied_vol_ = ivol; }

    /**
     * Returns the delta of the option if available, or quiet_NaN otherwise
     *
     * Scaled as +-1.0
     */
    double delta() const { return delta_; }
    void set_delta(double delta) { delta_ = delta; }

    /**
     * Returns the gamma of the option if available, or quiet_NaN otherwise
     */
    double gamma() const { return gamma_; }
    void set_gamma(double gamma) { gamma_ = gamma; }

    /**
     * Returns the theta of the option if available, or quiet_NaN otherwise
     */
    double theta() const { return theta_; }
    void set_theta(double theta) { theta_ = theta; }

    /**
     * Returns the vega of the option if available, or quiet_NaN otherwise
     */
    double vega() const { return vega_; }
    void set_vega(double vega) { vega_ = vega; }

    /**
     * Returns the rho of the option if available, or quiet_NaN otherwise
     */
    double rho() const { return rho_; }
    void set_rho(double rho) { rho_ = rho; }

    /**
     * Returns the option price associated with the implied vols and greeks, or the option bid if bid/ask vols are available
     *
     * Retuns quiet_NaN if no options analytics are present
     */
    double reference_price1() const { return reference_price1_; }
    void set_reference_price1(double price) { reference_price1_ = price; }

    /**
     * Returns the option ask price associated with the implied vols and greeks if bid/ask vols are available, or quiet_NaN otherwise
     */
    double reference_price2() const { return reference_price2_; }
    void set_reference_price2(double price) { reference_price2_ = price; }

    /**
     * Returns the underlier price associated with the implied vols and greeks if available, or quiet_NaN otherwise
     */
    double underlier_reference_price() const { return underlier_reference_price_; }
    void set_underlier_reference_price(double price) { underlier_reference_price_ = price; }

    const void* external_properties() const { return external_properties_; }
    void set_external_properties(void* external_properties);

private:
    Option(const Option& option); // not implemented
    Option& operator=(const Option& option); // not implemented

protected:
    SymbolTag underlier_symbol_;
    DateType expiration_date_;
    void* external_properties_;
    double deliverable_cash_;
    double strike_;
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
    int units_;
    int open_interest_;
    OptionType option_type_;
    ExerciseStyle exercise_style_;
    DeliveryType delivery_type_;
    bool is_adjusted_contract_;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif