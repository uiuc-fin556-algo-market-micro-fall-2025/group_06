/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyInfo.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGYINFO_H_
#define _STRATEGY_STUDIO_LIB_STRATEGYINFO_H_

#include "DataTypes.h"
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace RCM {
namespace StrategyStudio {

/**
 * Holds information before strategy is created
 */
struct StrategyInfo {
public:
    typedef std::set<std::string> SymbolSet;
    typedef SymbolSet::iterator SymbolSetIter;
    typedef SymbolSet::const_iterator SymbolSetConstIter;

public:
    StrategyInfo();

    StrategyInfo(StrategyID strategyID, const std::string& name, const std::string& type, const std::string& group);

    StrategyInfo(StrategyID strategyID,
                 const std::string& name,
                 const std::string& type,
                 const std::string& group,
                 const std::string& account,
                 const std::string& instOwner);

    SymbolSetIter symbols_begin() { return symbol_set.begin(); }
    SymbolSetConstIter symbols_begin() const { return symbol_set.begin(); }
    SymbolSetIter symbols_end() { return symbol_set.end(); }
    SymbolSetConstIter symbols_end() const { return symbol_set.end(); }

    bool AddSymbol(const std::string& symbol);

    /**
     * Creates a comma-delimited list of symbols
     */
    std::string CreateSymbolString() const;

public:
    SymbolSet symbol_set;
    std::string settings_file;
    std::string name;
    std::string type;
    std::string group;
    std::string account;
    std::string instance_owner;
    StrategyRunMode run_mode;
    StrategyID strategy_id;
    double initial_equity;
    int configured_processor_id;
    int effective_processor_id;
    int subscription_mode;
};

typedef std::shared_ptr<StrategyInfo> StrategyInfoPtr;

/**
 * Holds information for a list of strategies
 */
class StrategyInformation {
public:
    typedef std::vector<StrategyInfoPtr> StrategyInfoList;
    typedef StrategyInfoList::iterator StrategyInfoListIter;
    typedef StrategyInfoList::const_iterator StrategyInfoListConstIter;

public:
    StrategyInformation();

    StrategyInfoListIter begin() { return strategy_info_list_.begin(); }
    StrategyInfoListIter end() { return strategy_info_list_.end(); }

    StrategyInfoListConstIter begin() const { return strategy_info_list_.begin(); }
    StrategyInfoListConstIter end() const { return strategy_info_list_.end(); }

    size_t size() const { return strategy_info_list_.size(); }

    void clear() { strategy_info_list_.clear(); }

    /**
     * Add the strategy info to the collection
     */
    void Add(const StrategyInfoPtr& strategyInfo);

    /**
     * Remove the strategy info from the collection
     */
    void Remove(StrategyInfoListIter iter);

    /**
     * Find strategy information by ID
     */
    StrategyInfoListIter Find(StrategyID strategyID);

    void Sort();

private:
    StrategyInfoList strategy_info_list_;
};

class StrategyInfoLoadOrder : public std::binary_function<StrategyInfoPtr, StrategyInfoPtr, bool> {
public:
    bool operator()(const StrategyInfoPtr& x, const StrategyInfoPtr& y) const
    {
        if (x->configured_processor_id >= 0) {
            return (y->configured_processor_id < 0);
        } else if (y->configured_processor_id >= 0) {
            return false;
        } else {
            return (x->symbol_set.size() > y->symbol_set.size());
        }
    }
};

} // namespace StrategyStudio
} // namespace RCM

#endif
