/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/IParams.h
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

#ifndef _STRATEGY_STUDIO_UTILITIES_IPARAMS_H_
#define _STRATEGY_STUDIO_UTILITIES_IPARAMS_H_

#include "Cast.h"

#include <memory>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

/**
 * Provides an interface to retrieve parameters
 */
class IParams {
public:
    IParams();

    virtual ~IParams() = default;
    IParams(const IParams&) = default;
    IParams& operator=(const IParams&) = default;
    IParams(IParams&&) = default;
    IParams& operator=(IParams&&) = default;

    /**
     * Check if the key is present with the parameters list
     */
    virtual bool Contains(const std::string& key) const = 0;

    /**
     * Retrieve the value associated with a given key
     * @return value as string, or empty string if not found
     */
    virtual std::string Get(const std::string& key) const = 0;

    /**
     * Retrieve the value associated with a given key by reference
     * @return value as string, throws if not found
     */
    virtual const std::string& GetByRef(const std::string& key) const = 0;

    /**
     * Adds a parameter to the IParams
     */
    virtual void AddParam(const std::string& name, const std::string& value)=0;

    template<typename T>
    void AddParam(const std::string& name, const T& value);
};

template<typename T>
void IParams::AddParam(const std::string& name, const T& value)
{
    AddParam(name, Utilities::Cast<std::string>()(value));
}

typedef std::unique_ptr<IParams> IParamsUPtr;
typedef std::shared_ptr<IParams> IParamsSPtr;

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif
