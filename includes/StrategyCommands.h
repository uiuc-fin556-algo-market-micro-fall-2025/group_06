/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyCommands.h
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

#ifndef _STRATEGY_STUDIO_LIB_STATEGY_COMMANDS_H_
#define _STRATEGY_STUDIO_LIB_STATEGY_COMMANDS_H_

#include "DataTypes.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <string>
#include <map>

namespace RCM {
namespace StrategyStudio {

class StrategyCommands;

/**
 * Information encapsulating data for strategy command
 */
class StrategyCommand {
public:
    StrategyCommand();

    StrategyCommand(StrategyStudioUInt32 commandID,
                    const std::string& commandName,
                    InputType inputType = INPUT_TYPE_NO_INPUT,
                    const std::string& inputPrompt = "");

    StrategyStudioUInt32 command_id() const { return command_id_; }
    void set_command_id(StrategyStudioUInt32 id) { command_id_ = id; }

    const std::string& command_name() const { return command_name_; }
    void set_command_name(const std::string& commandName) { command_name_ = commandName; }

    const std::string& input_prompt() const { return input_prompt_; }
    void set_input_prompt(const std::string& inputPrompt) { input_prompt_ = inputPrompt; }

    InputType input_type() const { return input_type_; }
    void set_input_type(InputType inputType) { input_type_ = inputType; }

    bool operator<(const StrategyCommand& rhs) const { return (command_id_ < rhs.command_id_); }

    friend class StrategyCommands;

private:
    std::string command_name_;
    std::string input_prompt_;
    StrategyStudioUInt32 command_id_;
    InputType input_type_;
};

/**
 * StrategyCommands is a collection for holding and managing individual strategy commands
 */
class StrategyCommands {
public:
    typedef boost::multi_index::multi_index_container<StrategyCommand,
        boost::multi_index::indexed_by<
            // sort by StrategyCommand::operator <
            boost::multi_index::ordered_unique<boost::multi_index::member<StrategyCommand,
                StrategyStudioUInt32, &StrategyCommand::command_id_> >,
            boost::multi_index::hashed_unique<boost::multi_index::member<StrategyCommand,
                std::string, &StrategyCommand::command_name_> >
            >
        > StrategyCommandSet;

    typedef StrategyCommandSet::iterator StrategyCommandSetIter;
    typedef StrategyCommandSet::const_iterator StrategyCommandSetConstIter;

public:
    StrategyCommands();

    StrategyCommandSetIter begin() { return commands_.begin(); }
    StrategyCommandSetIter end() { return commands_.end(); }

    StrategyCommandSetConstIter begin() const { return commands_.begin(); }
    StrategyCommandSetConstIter end() const { return commands_.end(); }

    /**
     * Adds a strategy command to the commands collection
     * Note: Name and ID need to be unique within the system
     * @returns Ptr to strategy command if created, NULL otherwise
     */
    const StrategyCommand* AddCommand(const StrategyCommand& command);

    /**
     * Check if command exists by name
     */
    bool Contains(const std::string& commandName) const;

    /**
     * Check if command exists by name
     */
    bool Contains(StrategyStudioUInt32 commandID) const;

    /**
     * Retrieve command by id
     * @returns Ptr to command
     */
    const StrategyCommand* GetCommand(StrategyStudioUInt32 id) const;

private:
    StrategyCommandSet commands_;
};

} // namespace StrategyStudio
} // namespace RCM

#endif
