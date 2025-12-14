/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/ICloneableEventCallback.h
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

#ifndef _STRATEGY_STUDIO_LIB_ICLONEABLE_EVENT_CALLBACK_H_
#define _STRATEGY_STUDIO_LIB_ICLONEABLE_EVENT_CALLBACK_H_

#include "IEventCallback.h"

namespace RCM {
namespace StrategyStudio {

/**
 * Adds a cloneable method to the IEventCallback interface
 *
 * Used as a base class for registering closure objects to the event processor;
 * see also the IAutoCloneableEventCallback convenience wrapper below for a version that
 * will leverage the derived class's copy constructor to automatically implement the clone function.
 */
class ICloneableEventCallback : public IEventCallback {
public:
    ICloneableEventCallback() {}
    virtual ~ICloneableEventCallback() {}

public:
    /**
     * Provides a deep clone of the object
     */
    virtual ICloneableEventCallback* clone() const = 0;
};

/**
 * Uses the CRTP pattern to provide auto implementation of IClonebleEventCallback's clone() method via the
 * derived class's copy constructor.
 *
 * To use this class, simply define a class like "class MyEventCallback : public IAutoCloneableEventCallback<MyEventCallback>{}"
 */
template <typename Derived>
class IAutoCloneableEventCallback : public ICloneableEventCallback {
public:
    virtual ICloneableEventCallback* clone() const
    {
        return new Derived(static_cast<Derived const&>(*this));
    }
};

} // namespace StrategyStudio
} // namespace RCM

#endif
