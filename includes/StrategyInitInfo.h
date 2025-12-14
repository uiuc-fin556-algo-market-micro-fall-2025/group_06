/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyInitInfo.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGYINITINFO_H_
#define _STRATEGY_STUDIO_LIB_STRATEGYINITINFO_H_

#include "DataTypes.h"
#include "StrategyCommands.h"
#include <Utilities/GenericMap.h>
#include <string>
#include <map>

namespace RCM {
namespace StrategyStudio {

class StrategyParam;

class StrategyInitInfo : public Utilities::GenericMap<std::string, StrategyParam*> {
public:
    typedef Container ParamInfo;
    typedef ParamInfo::iterator ParamInfoIter;
    typedef ParamInfo::const_iterator ParamInfoConstIter;

public:
    StrategyInitInfo();
    StrategyInitInfo(const std::string& name);
    ~StrategyInitInfo();

    const std::string& name() const { return name_; }
    void set_name(const std::string& name) { name_ = name; }

    StrategyCommands& commands() { return strategy_commands_; }
    const StrategyCommands& commands() const { return strategy_commands_; }
    void set_commands(const StrategyCommands& rhs) { strategy_commands_ = rhs; }

private:
    std::string name_;
    StrategyCommands strategy_commands_;
};

} // namespace StrategyStudio
} // namespace RCM

#endif
