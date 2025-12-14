/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IGraphSeriesCollection.h
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

#ifndef _STRATEGY_STUDIO_LIB_IGRAPHSERIESCOLLECTION_H_
#define _STRATEGY_STUDIO_LIB_IGRAPHSERIESCOLLECTION_H_

#include <string>
#include "DataTypes.h"
#include "IGraphSeries.h"
#include <boost/unordered_map.hpp>

namespace RCM {
namespace StrategyStudio {

/**
 * Collection of graph series
 */
class IGraphSeriesCollection {
public:
    typedef boost::unordered_map<std::string, IGraphSeries*> GraphSeriesMap;
    typedef GraphSeriesMap::iterator GraphSeriesMapIter;
    typedef GraphSeriesMap::const_iterator GraphSeriesMapConstIter;

public:
    IGraphSeriesCollection() {}

    virtual ~IGraphSeriesCollection() {}

    /**
     * Adds a new series to the graph series collection
     *
     * @param series_name A unique name to associate with the data series
     * @returns a pair with a pointer to the named series, along with bool set to false if the series was already present.
     */
    virtual std::pair<IGraphSeries*, bool> add(const std::string& series_name) = 0;

    /**
     * Returns the number of graph series in the collection
     */
    virtual std::size_t size() const = 0;

    virtual GraphSeriesMapIter  begin() = 0;
    virtual GraphSeriesMapConstIter begin() const = 0;

    virtual GraphSeriesMapIter end() = 0;
    virtual GraphSeriesMapConstIter end() const = 0;

    /**
     * Retrieves a graph series by its name
     *
     * This operator will automatically try adding the series if it does not yet exist
     */
    virtual IGraphSeries* operator[] (const std::string& series_name) = 0;
};

} // namespace StrategyStudio
} // namespace RCM

#endif