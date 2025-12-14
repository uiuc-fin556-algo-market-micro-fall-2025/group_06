/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyStudioException.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGY_STUDIO_EXCEPTION_H_
#define _STRATEGY_STUDIO_LIB_STRATEGY_STUDIO_EXCEPTION_H_

#include <stdexcept>
#include <string>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

class StrategyStudioException : public std::exception {
public:
    StrategyStudioException(const std::string& msg);
    virtual ~StrategyStudioException() throw();
    virtual const char* what() const throw();

private:
    std::string m_msg;
};


class StrategyStudioRuntimeError : public std::runtime_error {
public:
    StrategyStudioRuntimeError(const char* msg) : std::runtime_error(msg) {}
    StrategyStudioRuntimeError(const std::string& msg) : std::runtime_error(msg) {}
};

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif
