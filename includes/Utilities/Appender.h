/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/Appender.h
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

#ifndef _STRATEGY_STUDIO_UTILITIES_APPENDER_H_
#define _STRATEGY_STUDIO_UTILITIES_APPENDER_H_

#include "Layout.h"
#include <ostream>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

class Appender {
public:
    Appender();
    virtual ~Appender();

    /**
     * Derived appenders have their own stream to which we will log to
     */
    virtual std::ostream& GetStream() = 0;

    /**
     * Lets derived class test a given appender for equality
     * @returns true if appenders are equal, false otherwise
     */
    virtual bool Compare(const Appender& appender) const = 0;

    bool operator==(const Appender& appender) const;

};

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif
