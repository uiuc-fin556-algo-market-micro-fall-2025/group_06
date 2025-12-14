/*================================================================================
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_WORKUP_DATA_TYPES_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_WORKUP_DATA_TYPES_H_

#include "MDBaseObject.h"
#include "MarketCenterTypes.h"

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

enum WorkupSide {
    WORKUP_SIDE_BID_AND_ASK = 0,
    WORKUP_SIDE_BID = 1,
    WORKUP_SIDE_ASK = 2
};

enum WorkupState {
    WORKUP_STATE_NO_WORKUP = 0, // no workup is active
    WORKUP_STATE_PRIVATE = 1, // private phase (may not be used by all instruments)
    WORKUP_STATE_PUBLIC = 2, // public phase
    WORKUP_STATE_FIXING_PHASE = 3
};

enum WorkupReasonForChange {
    WORKUP_REASON_FOR_CHANGE_NORMAL = 0,
    WORKUP_REASON_FOR_CHANGE_ERRONEOUS = 1,
    WORKUP_REASON_FOR_CHANGE_TRADE_THROUGH = 2,
    WORKUP_REASON_FOR_CHANGE_OWNER_CHANGED = 3,
    WORKUP_REASON_FOR_CHANGE_ADVANCED_FIXING_TERMINATED = 4
};

/**
 * Contains information on a workup event
 */
class Workup : public MDBaseObject {
public:
    Workup(MarketCenterID marketCenter,
           double price,
           WorkupState state,
           Utilities::TimeDuration remainingTime,
           WorkupReasonForChange changeReason,
           WorkupSide aggressiveSide,
           boost::uint64_t aggressiveOrderId,
           boost::uint64_t passiveOrderId,
           int workupId,
           Utilities::TimeType sourceTime,
           Utilities::TimeType feedHandlerTime,
           Utilities::TimeType adapterTime);

    virtual ~Workup();

public:
    /**
     * Gets the market center on which the workup event occurred
     */
    MarketCenterID market_center() const { return market_center_; }

    /**
     * Gets the price of the current workup
     */
    double workup_price() const { return price_; }

    /**
     * Gets the state of the current workup
     */
    WorkupState workup_state() const { return state_; }

    /**
     * Gets the time remaining in the current workup
     * Note that this may not be populated by all providers
     */
    Utilities::TimeDuration remaining_time() const { return remaining_time_; }

    /**
     * Gets the reason for the current workup state transition
     */
    WorkupReasonForChange reason_for_change() const { return change_reason_; }

    /**
     * Gets the side of the aggressive order that triggered the workup
     */
    WorkupSide aggressive_side() const { return aggressive_side_; }

    /**
     * Gets the side of the passive order that triggered the workup
     */
    WorkupSide passive_side() const
    {
        return aggressive_side_ == WORKUP_SIDE_BID ? WORKUP_SIDE_ASK : WORKUP_SIDE_BID;
    }

    /**
     * Gets the order id for the aggressive order that triggered the workup
     * Note that this may not always be populated depending on venue and configuration
     */
    boost::uint64_t aggressive_order_id() const { return aggressive_order_id_; }

    /**
     * Gets the order id for the passive order that triggered the workup
     * Note that this may not always be populated depending on venue and configuration
     */
    boost::uint64_t passive_order_id() const { return passive_order_id_; }

    /**
     * Gets the current workup's id
     */
    int workup_id() const { return workup_id_; }

private:
    MarketCenterID market_center_;
    double price_;
    WorkupState state_;
    Utilities::TimeDuration remaining_time_;
    WorkupReasonForChange change_reason_;
    WorkupSide aggressive_side_;
    boost::uint64_t aggressive_order_id_;
    boost::uint64_t passive_order_id_;
    int workup_id_;
};

inline std::string WorkupSideToString(WorkupSide side)
{
    switch (side) {
        case WORKUP_SIDE_BID_AND_ASK:
            return "BID_AND_ASK";
        case WORKUP_SIDE_BID:
            return "BID";
        case WORKUP_SIDE_ASK:
            return "ASK";
        default:
            return "UNKNOWN";
    }
}

inline std::string WorkupStateToString(WorkupState state)
{
    switch (state) {
        case WORKUP_STATE_NO_WORKUP:
            return "NO_WORKUP";
        case WORKUP_STATE_PRIVATE:
            return "PRIVATE";
        case WORKUP_STATE_PUBLIC:
            return "PUBLIC";
        case WORKUP_STATE_FIXING_PHASE:
            return "FIXING_PHASE";
        default:
            return "UNKNOWN";
    }
}

inline std::string WorkupReasonForChangeToString(WorkupReasonForChange reason)
{
    switch (reason) {
        case WORKUP_REASON_FOR_CHANGE_NORMAL:
            return "NORMAL";
        case WORKUP_REASON_FOR_CHANGE_ERRONEOUS:
            return "ERRONEOUS";
        case WORKUP_REASON_FOR_CHANGE_TRADE_THROUGH:
            return "TRADE_THROUGH";
        case WORKUP_REASON_FOR_CHANGE_OWNER_CHANGED:
            return "OWNER_CHANGED";
        case WORKUP_REASON_FOR_CHANGE_ADVANCED_FIXING_TERMINATED:
            return "ADVANCED_FIXING_TERMINATED";
        default:
            return "UNKNOWN";
    }
}

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif
