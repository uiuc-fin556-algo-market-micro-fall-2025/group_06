/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/QuoteSide.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_QUOTE_SIDE_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_QUOTE_SIDE_H_

#include "OrderTypes.h"
#include "IBookEntry.h"
#include <Utilities/TimeType.h>
#include <ostream>
#include <functional>
#include <cmath>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * Represents a the bid side or offer side of a quote.
 */
class QuoteSide : public IBookEntry {
public:
    QuoteSide();
    QuoteSide(double price, int qty);
    QuoteSide(double price, int qty, TimeType sourceTime, TimeType feedTime, TimeType adapterTime);
    ~QuoteSide() {}

    double price() const { return price_; }
    void set_price(double val) { price_ = val; }

    int size() const { return size_; }
    void set_size(int val) { size_ = val; }

    /**
     * Check if we have any data
     */
    bool HasData() const;

    /**
     * Make sure quote has non-zero size
     */
    bool IsValid() const;

    /**
     * Reset prices
     */
    void Reset();

public:
    bool operator==(const QuoteSide& entry) const;

private:
    double price_;
    int size_;
};

inline bool QuoteSide::HasData() const
{
    return !((price_ == 0) && (size_ == 0));
}

inline bool QuoteSide::IsValid() const
{
    return (size_ != 0);
}

inline std::ostream& operator<<(std::ostream& out, const QuoteSide& entry)
{
    out << entry.size() << " @ " << entry.price();
    return out;
}

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif