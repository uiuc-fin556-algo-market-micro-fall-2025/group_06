#pragma once

#ifndef _STRATEGY_STUDIO_LIB_EXAMPLES_SIMPLE_MOMENTUM_STRATEGY_H_
#define _STRATEGY_STUDIO_LIB_EXAMPLES_SIMPLE_MOMENTUM_STRATEGY_H_

#ifdef _WIN32
    // Windows export
    #define _STRATEGY_EXPORTS __declspec(dllexport)
#else
    // Linux/GCC export
    #define _STRATEGY_EXPORTS __attribute__((visibility("default")))
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
#include <cmath> // Added for RSI calculation

using namespace RCM::StrategyStudio;

enum DesiredPositionSide {
    DESIRED_POSITION_SIDE_SHORT = -1,
    DESIRED_POSITION_SIDE_FLAT = 0,
    DESIRED_POSITION_SIDE_LONG = 1
};

/**
 * @class RSILogic
 * @brief Calculates RSI and generates trading signals.
 * This class replaces the original Momentum class.
 */
class RSILogic {
public:
    RSILogic(int rsi_period = 14) : 
        m_period(rsi_period),
        m_gains(rsi_period), 
        m_losses(rsi_period),
        m_last_price(0.0) {}

    void Reset() {
        m_gains.clear();
        m_losses.clear();
        m_last_price = 0.0;
    }

    DesiredPositionSide Update(double val) {
        // Need at least one previous price to calculate change
        if (m_last_price < 0.01) {
            m_last_price = val;
            return DESIRED_POSITION_SIDE_FLAT; // Not enough data yet
        }

        double change = val - m_last_price;
        m_last_price = val;

        double gain = (change > 0) ? change : 0.0;
        double loss = (change < 0) ? -change : 0.0;

        m_gains.push_back(gain);
        m_losses.push_back(loss);

        if (!FullyInitialized()) {
            return DESIRED_POSITION_SIDE_FLAT; // Waiting for windows to fill
        }

        double avg_gain = m_gains.Mean();
        double avg_loss = m_losses.Mean();

        // Handle divide-by-zero case (all gains, no losses)
        if (avg_loss < 1e-9) {
            // RSI would be 100. Return SELL (SHORT) signal as it's over 70.
            return DESIRED_POSITION_SIDE_SHORT; 
        }

        double rs = avg_gain / avg_loss;
        double rsi = 100.0 - (100.0 / (1.0 + rs));

        // Your requested logic
        if (rsi > 70.0) {
            return DESIRED_POSITION_SIDE_SHORT; // Sell signal
        } else if (rsi < 30.0) {
            return DESIRED_POSITION_SIDE_LONG;  // Buy signal
        } else {
            return DESIRED_POSITION_SIDE_FLAT; // Hold
        }
    }

    bool FullyInitialized() { return (m_gains.full() && m_losses.full()); }

    int m_period;
    Analytics::ScalarRollingWindow<double> m_gains;
    Analytics::ScalarRollingWindow<double> m_losses;
    double m_last_price;
};

class Junup2Strategy : public Strategy {
public:
    // Renamed MomentumMap to RSILogicMap
    typedef boost::unordered_map<const Instrument*, RSILogic> RSILogicMap;
    typedef RSILogicMap::iterator RSILogicMapIterator;

    typedef boost::unordered_map<std::string, double> TradePriceMap;
    typedef TradePriceMap::iterator TradePriceMapIterator;

public:
    Junup2Strategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName);
    ~Junup2Strategy();

public: /* from IEventCallback */
    virtual void OnTrade(const TradeDataEventMsg& msg);
    virtual void OnTopQuote(const QuoteEventMsg& msg);
    virtual void OnQuote(const QuoteEventMsg& msg);
    virtual void OnDepth(const MarketDepthEventMsg& msg);
    virtual void OnBar(const BarEventMsg& msg);
    virtual void OnMarketState(const MarketStateEventMsg& msg){};
    virtual void OnOrderUpdate(const OrderUpdateEventMsg& msg);
    virtual void OnStrategyControl(const StrategyStateControlEventMsg& msg){}
    void OnResetStrategyState();
    void OnDataSubscription(const DataSubscriptionEventMsg& msg){}
    void OnStrategyCommand(const StrategyCommandEventMsg& msg);
    void OnParamChanged(StrategyParam& param);

private: // Helper functions specific to this strategy
    //helper strategy functions here for sending orders etc
private: /* from Strategy */
    virtual void RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate);
    virtual void DefineStrategyParams();
    virtual void DefineStrategyCommands();

private:
    // Renamed m_momentum_map
    RSILogicMap m_rsi_logic_map;
    boost::unordered_map<const Instrument*, OrderID> m_instrument_order_id_map;
    TradePriceMap m_tradeprice_map;
    
    // Renamed m_momentum
    RSILogic* m_rsi_logic;
    // Unchanged OnTrade logic variables
    double m_dia_last_trade_price;
    double m_average_dia_ratio;
    int m_num_dia_ratio_observations;
    
    // Strategy parameters
    double m_max_notional; // This was in the header but not initialized
    double m_aggressiveness;
    int m_position_size;
    
    // Replaced window sizes with rsi_period
    int m_rsi_period;
    bool m_debug_on;
};

// These are the C-style function DECLARATIONS
// The platform will look for these.
extern "C" {
    _STRATEGY_EXPORTS const char* GetType();
    _STRATEGY_EXPORTS IStrategy* CreateStrategy(const char* strategyType,
        unsigned strategyID,
        const char* strategyName,
        const char* groupName);
    _STRATEGY_EXPORTS const char* GetAuthor();
    _STRATEGY_EXPORTS const char* GetAuthorGroup();
    _STRATEGY_EXPORTS const char* GetReleaseVersion();
}

#endif
