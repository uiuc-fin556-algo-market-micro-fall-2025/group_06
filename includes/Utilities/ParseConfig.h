/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/ParseConfig.h
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

#ifndef _STRATEGY_STUDIO_UTILITIES_PARSECONFIG_H_
#define _STRATEGY_STUDIO_UTILITIES_PARSECONFIG_H_

#include "IParams.h"
#include "Cast.h"
#include <string>
#include <map>
#include <ostream>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

/**
 * This is a helper class for representing key-value strings.
 * Note: This could potentially be optimized in the future.
 */
class ParseConfig : public IParams {
public:
    typedef std::map<std::string, std::string> Params;
    typedef Params::iterator ParamsIter;
    typedef Params::const_iterator ParamsConstIter;

public:
    ParseConfig();
    ParseConfig(const Params& params);

    /**
     * Initializes directly via file
     */
    ParseConfig(const std::string& file, char delim='=', char comment='#');

    ParamsIter begin() { return m_params.begin(); }
    ParamsIter end() { return m_params.end(); }

    ParamsConstIter begin() const { return m_params.begin(); }
    ParamsConstIter end() const { return m_params.end(); }

    size_t size() const { return m_params.size(); }

    /**
     * Adds the param to the parameter list if the key is not present
     */
    void AddParam(const std::string& key, const std::string& value);

    /**
     * Adds the param to the parameter list if the key is not present
     * Casts the arg to a string before adding it
     */
    template<typename Arg>
    void AddParam(const std::string& key, const Arg& value)
    {
        AddParam(key, Cast<std::string>()(value));
    }

    /**
     * Clear the params
     */
    void Clear();

    /**
     * Check if the key is present with the parameters list
     */
    bool Contains(const std::string& key) const;

    /**
     * Retrieve the value associated with a given key
     * @return value as string, or empty string if not found
     */
    std::string Get(const std::string& key) const;

    /**
     * Retrieve the value associated with a given key by reference
     * @return value as string, throws if not found
     */
    const std::string& GetByRef(const std::string& key) const;

    /**
    * Parses a vector of key/value string pairs separated by a delimiter
    */
    bool Parse(const std::vector<std::string>& pairs, char delim);

    /**
    * Parses a vector of key/value string pairs separated by a delimiter
    */
    bool Parse(std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end, char delim);

    /**
    * Parses a single single and inserts into collection
    * @return denotes whether insertion was successful
    */
    bool ParseLine(const std::string& s, char delim = '=', int currLine = 0);

    /**
     * Reads the contents of a file to populate the parameters
     * @returns true if read completed, false otherwise
     */
    bool ReadParams(const std::string& file, char delim='=', char comment='#');

    /**
     * Updates the given key with the given value, does nothing if key is not present
     * @returns true if key updated, false if key not found
     */
    bool UpdateParam(const std::string& key, const std::string& value);

private:
    Params m_params;
};

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif
