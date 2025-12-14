/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyCommandEventMsg.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGY_COMMAND_STATE_EVENT_MSG_H_
#define _STRATEGY_STUDIO_LIB_STRATEGY_COMMAND_STATE_EVENT_MSG_H_

#include "EventMsg.h"
#include "DataTypes.h"
#include "Event.h"
#include "StrategyCommands.h"
#include <Utilities/Cast.h>
#include <boost/any.hpp>

namespace RCM {
namespace StrategyStudio {

/**
 * StrategyCommandEventMsg encapsulates a custom command request from the GUI
 */
class StrategyCommandEventMsg : public EventMsg {
public:
    StrategyCommandEventMsg(Event* event);

    StrategyCommandEventMsg(StrategyStudioUInt32 commandID,
                            const boost::any& input,
                            const std::string& user,
                            TimeType issue_time,
                            Event* event);

    std::string name() const;

    StrategyStudioUInt32 command_id() const { return command_id_; }
    void set_command_id(StrategyStudioUInt32 id) { command_id_ = id; }

    boost::any& input_data() { return input_; }
    const boost::any& input_data() const { return input_; }
    void set_input_data(const boost::any& data) { input_ = data; }

    const StrategyCommand* command() const { return command_; }
    void set_command(const StrategyCommand* cmd) { command_ = cmd; }

    TimeType issue_time() const { return issue_time_; }

    const std::string& user() const { return user_; }

private:
    boost::any input_;
    std::string user_;
    TimeType issue_time_;
    const StrategyCommand* command_;
    StrategyStudioUInt32 command_id_;
};

inline std::string StrategyCommandEventMsg::name() const
{
    return "StrategyCommand_" + Utilities::Cast<std::string>()(command_id());
}

} // namespace StrategyStudio
} // namespace RCM

#endif