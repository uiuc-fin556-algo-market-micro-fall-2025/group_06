/*================================================================================
*     Source: ../RCM/StrategyStudio/examples/strategies/SMAReactiveTakerStrategy/SMAReactiveTakerStrategy.h
*     Last Update: 2021/04/15 13:55:14
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2011 - 2021.
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
/*================================================================================*/

#pragma once

#ifndef _STRATEGY_STUDIO_LIB_EXAMPLES_SMA_REACTIVE_TAKER_STRATEGY_H_
#define _STRATEGY_STUDIO_LIB_EXAMPLES_SMA_REACTIVE_TAKER_STRATEGY_H_

#ifdef _WIN32
    #define _STRATEGY_EXPORTS __declspec(dllexport)
#else
    #ifndef _STRATEGY_EXPORTS
    #define _STRATEGY_EXPORTS
    #endif
#endif

#include <Strategy.h>
#include <Analytics/ScalarRollingWindow.h>
#include <Analytics/InhomogeneousOperators.h>
#include <Analytics/IncrementalEstimation.h>
#include <MarketModels/Instrument.h>
#include <Utilities/ParseConfig.h>

#include <vector>
#include <map>
#include <iostream>

using namespace RCM::StrategyStudio;

enum DesiredPositionSide {
    DESIRED_POSITION_SIDE_SHORT=-1,
    DESIRED_POSITION_SIDE_FLAT=0,
    DESIRED_POSITION_SIDE_LONG=1,
    NEUTRAL = 0,
    LONG = 1,
    SHORT = -1
};

class Momentum {
public:
    Momentum(int short_window_size = 10, int long_window_size = 30) : short_window_(short_window_size), long_window_(long_window_size) {}

    void Reset()
    {
        short_window_.clear();
        long_window_.clear();
    }

    DesiredPositionSide Update(double val)
    {
        short_window_.push_back(val);
        long_window_.push_back(val);
        if (short_window_.Mean() > long_window_.Mean()) {
            return DESIRED_POSITION_SIDE_LONG;
        } else {
            return DESIRED_POSITION_SIDE_SHORT;
        }
    }

    bool FullyInitialized() { return (short_window_.full() && long_window_.full()); }

    double GetPreviousClose() {
        return short_window_.size() > 1 ? short_window_[short_window_.size() - 2] : 0.0;
    }

    double GetSMA() {
        return long_window_.Mean();
    }

    double GetPreviousSMA() {
        if (long_window_.size() > 1) {
            double total = 0.0;
            for (size_t i = 0; i < long_window_.size() - 1; ++i) {
                total += long_window_[i];
            }
            return total / (long_window_.size() - 1);
        }
        return 0.0;
    }

    Analytics::ScalarRollingWindow<double> short_window_;
    Analytics::ScalarRollingWindow<double> long_window_;
};

class SMAReactiveTakerStrategy : public Strategy {
public:
    typedef boost::unordered_map<const Instrument*, Momentum> MomentumMap; 
    typedef MomentumMap::iterator MomentumMapIterator; 

public:
    SMAReactiveTakerStrategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName);
    ~SMAReactiveTakerStrategy();

public: /* from IEventCallback */
    virtual void OnTrade(const TradeDataEventMsg& msg){}
    virtual void OnTopQuote(const QuoteEventMsg& msg){}    
    virtual void OnQuote(const QuoteEventMsg& msg){}
    virtual void OnDepth(const MarketDepthEventMsg& msg){}
    virtual void OnBar(const BarEventMsg& msg);
    virtual void OnMarketState(const MarketStateEventMsg& msg){};
    virtual void OnOrderUpdate(const OrderUpdateEventMsg& msg);
    virtual void OnStrategyControl(const StrategyStateControlEventMsg& msg){}
    void OnResetStrategyState();
    void OnDataSubscription(const DataSubscriptionEventMsg& msg){}
    void OnStrategyCommand(const StrategyCommandEventMsg& msg);
    void OnParamChanged(StrategyParam& param);

private: // Helper functions specific to this strategy
    void AdjustPortfolio(const Instrument* instrument, int desired_position);
    void SendOrder(const Instrument* instrument, int trade_size);
    void RepriceAll();
    void Reprice(Order* order);

private: /* from Strategy */
    virtual void RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate); 
    virtual void DefineStrategyParams();
    virtual void DefineStrategyCommands();

private:
    MomentumMap momentum_map_;
    double aggressiveness_;
    int position_size_;
    int short_window_size_;
    int long_window_size_;
    bool debug_;
};

extern "C" {
    _STRATEGY_EXPORTS const char* GetType()
    {
        return "SMAReactiveTakerStrategy";
    }

    _STRATEGY_EXPORTS IStrategy* CreateStrategy(const char* strategyType,
                                   unsigned strategyID,
                                   const char* strategyName,
                                   const char* groupName) {
        if (strcmp(strategyType, GetType()) == 0) {
            return *(new SMAReactiveTakerStrategy(strategyID, strategyName, groupName));
        } else {
            return NULL;
        }
    }

    _STRATEGY_EXPORTS const char* GetAuthor() {
        return "dlariviere";
    }

    _STRATEGY_EXPORTS const char* GetAuthorGroup() {
        return "UIUC";
    }

    _STRATEGY_EXPORTS const char* GetReleaseVersion() {
        return Strategy::release_version();
    }
}

#endif
