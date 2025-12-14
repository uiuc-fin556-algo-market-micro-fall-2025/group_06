/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyParamCollection.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGY_PARAM_COLLECTION_H_
#define _STRATEGY_STUDIO_LIB_STRATEGY_PARAM_COLLECTION_H_

#include "StrategyParam.h"
#include <Utilities/IParams.h>
#include <boost/unordered_map.hpp>
#include <string>
#include <map>

namespace RCM {
namespace StrategyStudio {

namespace Utilities {
class SerializedStream;
}

/**
 * CreateStrategyParamArgs is a struct used to describe a Strategy Parameter when adding new
 * parameters to a strategy. Specifically, you pass a CreateStrategyParamArgs into the
 * CreateParam() function in the StrategyParamCollection class.
 */
struct CreateStrategyParamArgs {
public:
    /**
     * Constructor for CreateStrategyParamArgs
     *
     * @param paramName The name of the parameter to create
     * @param paramsType The parameter type, which describes whether and when the parameter is editable via the Strategy Manager
     * @param valueType The value/data type of the parameter
     */
    CreateStrategyParamArgs(const std::string& paramName,
                            StrategyParamType paramType,
                            ValueType valueType);
    /**
     * Constructor for CreateStrategyParamArgs
     *
     * @param paramName The name of the parameter to create
     * @param paramsType The parameter type, which describes whether and when the parameter is editable via the Strategy Manager
     * @param valueType The value/data type of the parameter
     * @param defaultValue The default value to use when creating a new instance of a strategy
     */
    CreateStrategyParamArgs(const std::string& paramName,
                            StrategyParamType paramType,
                            ValueType valueType,
                            const boost::any& defaultValue,
                            double min_constraint = std::numeric_limits<double>::quiet_NaN(),
                            double max_constraint = std::numeric_limits<double>::quiet_NaN(),
                            double max_percent_change_constraint = std::numeric_limits<double>::quiet_NaN());

    bool operator==(const CreateStrategyParamArgs& rhs) const;

public:
    /**
     * Name of the parameter
     */
    std::string param_name;

    /**
     * The parameter type, which describes whether and when the parameter is editable via the Strategy Manager
     */
    StrategyParamType param_type;

    /**
     * The value/data type of the parameter
     */
    ValueType value_type;

    /**
     * The default value for the parameter
     */
    boost::any default_value;

    /**
     * The minimum bound for the parameter value; will apply if set for params of INT or DOUBLE value types
     */
    double min;

    /**
     * The maximum bound for the parameter value; will apply if set for params of INT or DOUBLE value types
     */
    double max;

    /**
     * The maximum allowed absolute percent change for each update to the parameter; will apply if set for params of INT or DOUBLE value types
     *
     * This should be specified in decimal form
     */
    double max_percent_change;
};

/**
 * StrategyParamCollection is a collection for holding and managing individual strategy params
 */
class StrategyParamCollection {
public:
    typedef boost::unordered_map<std::string, StrategyParam*, boost::hash<std::string> > Params;
    typedef Params::iterator ParamsIter;
    typedef Params::const_iterator ParamsConstIter;

public:
    StrategyParamCollection();
    explicit StrategyParamCollection(IStrategyParamListener* listener);
    ~StrategyParamCollection();

    IStrategyParamListener* listener() { return listener_; }
    const IStrategyParamListener* listener() const { return listener_; }
    void set_listener(IStrategyParamListener* listener) { listener_ = listener; }

    ParamsIter params_begin() { return params_.begin(); }
    ParamsIter params_end() { return params_.end(); }

    ParamsConstIter params_begin() const { return params_.begin(); }
    ParamsConstIter params_end() const { return params_.end(); }

    int size() const { return static_cast<int>(params_.size()); }

    /**
     * Check if params exists by name
     */
    bool Contains(const std::string& paramName) const;

    /**
     * Create a strategy param using the passed arguments
     * @returns StrategyParam object if created, nullptr if params by name already exists or invalid args
     */
    StrategyParam* CreateParam(const CreateStrategyParamArgs& args);

    /**
     * Retrieve param by name
     */
    StrategyParam* GetParam(const std::string& name);
    const StrategyParam* GetParam(const std::string& name) const;

    /**
    * Get the datum associated with given param name and write it to passed out arg
    * @throws StrategyStudioException if types for data and param type do not match
    * @returns true if param exists and out arg written, false otherwise
    */
    template<typename T>
    bool GetValue(const std::string& paramName, T* outValue) const;

    /**
     * Set the datum on the given param with name
     * @throws StrategyStudioException if types for data and param type do not match
     * @returns StrategyParamRetVal to indicate whether the parameter was seccesfully set
     */
    template<typename T>
    StrategyParamRetVal SetValue(const std::string& paramName, const T& value);

    /**
     * Save PortfolioTracker state to stream
     */
    void SaveToBinaryStream(Utilities::SerializedStream& out) const;

    /**
     * Load PortfolioTracker state from stream
     */
    void LoadFromBinaryStream(Utilities::SerializedStream& in);

    /**
     * Indexer retrieval by name
     */
    StrategyParam* operator[](const std::string& name);
    const StrategyParam* operator[](const std::string& name) const;

    bool operator==(const StrategyParamCollection& rhs) const;

private:
    /**
     * Logs a message to the listener interface if present
     */
    void LogParamMsg(const std::string& msg) const;

private:
    Params params_;
    IStrategyParamListener* listener_;
};

template<typename T>
inline bool StrategyParamCollection::GetValue(const std::string& paramName, T* outValue) const
{
    const StrategyParam* param = GetParam(paramName);

    if (param) {
        return param->Get(outValue);
    } else {
        return false;
    }
}

template<typename T>
inline StrategyParamRetVal StrategyParamCollection::SetValue(const std::string& paramName, const T& value)
{
    StrategyParam* param = GetParam(paramName);

    if (param) {
        return param->SetValue(value);
    } else {
        return STRATEGY_PARAM_RETVAL_UNKNOWN;
    }
}

} // namespace StrategyStudio
} // namespace RCM

#endif
