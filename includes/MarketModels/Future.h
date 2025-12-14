/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/Future.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_FUTURE_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_FUTURE_H_

#include "Instrument.h"
#include "DerivativesTypes.h"
#include <string>
#include <boost/date_time/gregorian/gregorian_types.hpp>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * Instrument represents a futures contract
 */
class Future : public Instrument {
public:
    Future(InstrumentID instrID,
           const SymbolTag& symbol,
           MarketType marketType,
           const Markets::MarketCenterIDCollection& marketCenterIDs,
           IOrderBookBuilderPtr pOrderBookBuilder,
           MarketCenterInfoFactory* marketCenterInfoFactory);

    virtual ~Future();

    /**
     * Returns the root symbol (product code) of the instrument
     */
    const SymbolTag& root_symbol() const { return root_symbol_; }
    void set_root_symbol(const SymbolTag& rootSymbol) { root_symbol_ = rootSymbol; }

    /**
     * Retrieves the expiration date of the futures contract
     */
    boost::gregorian::date expiry_date() const { return expiry_date_; }
    void set_expiry_date(boost::gregorian::date expiryDate) { expiry_date_ = expiryDate; }

    /**
     * Retrieves the minimum tick size for trading calendar spreads in the futures product
     */
    double spread_tick_size() const { return spread_tick_size_; }
    void set_spread_tick_size(double tickSize) { spread_tick_size_ = tickSize; }

    /**
     * Retrieves the settlement/delivery style for the futures product
     */
    DeliveryType type_of_delivery() { return delivery_type_; }
    void set_type_of_delivery(DeliveryType type) { delivery_type_ = type; }

    /**
     * Retrieves the initial margin requirement for speculators in the futures product
     */
    double initial_margin() const { return initial_margin_; }
    void set_initial_margin(double margin) { initial_margin_ = margin; }

    /**
     * Retrieves the maintenance margin requirement for speculators in the futures product
     */
    double maintenance_margin() const { return maintenance_margin_; }
    void set_maintenance_margin(double margin) { maintenance_margin_ = margin; }

    /**
     * Retrieves the initial margin requirement for members or hedgers
     */
    double member_initial_margin() const { return member_initial_margin_; }
    void set_member_initial_margin(double margin) { member_initial_margin_ = margin; }

    /**
     * Retrieves the maintenance margin requirement for members or hedgers
     */
    double member_maintentance_margin() const { return member_maintenance_margin_; }
    void set_member_maintenance_margin(double margin) { member_maintenance_margin_ = margin; }

    const void* external_properties() const { return external_properties_; }
    void set_external_properties(void* extProperties);

    bool operator==(const Future& future) const;

private:
    Future(const Future& future); // not implemented
    Future& operator=(const Future& future); // not implemented

protected:
    SymbolTag root_symbol_;
    boost::gregorian::date expiry_date_;
    double spread_tick_size_;
    double initial_margin_;
    double maintenance_margin_;
    double member_initial_margin_;
    double member_maintenance_margin_;
    void* external_properties_;
    DeliveryType delivery_type_;
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif