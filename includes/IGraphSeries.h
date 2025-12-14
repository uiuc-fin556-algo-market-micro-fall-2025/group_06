/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IGraphSeries.h
*     Last Update: 2013/02/20
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2013.
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

#ifndef _STRATEGY_STUDIO_LIB_IGRAPHSERIES_H_
#define _STRATEGY_STUDIO_LIB_IGRAPHSERIES_H_

#include <string>
#include "DataTypes.h"

namespace RCM {
namespace StrategyStudio {

/**
 * Interface for contributing data to a graphable series
 *
 * Currently only time series graphing supported, and the timestamp is synched to the strategy's timekeeper
 * To avoid overloading the network and Strategy Manager with data, data points are only sent to the GUI
 * when their timestamps are newer than CUSTOM_GRAPH_THROTTLING_SEC seconds since the time of the last sent value.
 * The throttling interval defaults to 1 second for a live server and 15 seconds for a backtesting server
 */
class IGraphSeries {
public:

    IGraphSeries(){}

    virtual ~IGraphSeries(){}

    /**
     * Adds a new point to the graph series
     * @returns true if a new point was sent, false if the the point was rejected due to the throttling interval
     */
    virtual bool push_back(TimeType timestamp, double data_point) = 0;

    /**
     * Clears all data for the series
     */
    virtual void clear() = 0;

    /**
     * Returns the most recently added data point
     */
    virtual const std::pair<TimeType, double>& last() const = 0;

    /**
     * Returns the name of the series
     */
    virtual const std::string& name() const = 0;
};

} // namespace StrategyStudio
} // namespace RCM

#endif