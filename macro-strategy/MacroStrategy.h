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

#include <MarketModels/Instrument.h>
#include <Strategy.h>

#include <boost/unordered_map.hpp>
#include <cmath>
#include <deque>
#include <sstream>

using namespace RCM::StrategyStudio;

namespace {

inline double Clip(double x, double lo, double hi) {
    return (x < lo) ? lo : ((x > hi) ? hi : x);
}

inline int RoundToInt(double x) {
    return (x >= 0.0) ? static_cast<int>(x + 0.5) : static_cast<int>(x - 0.5);
}

// Simple fixed-capacity rolling window with mean/std support.
class RollingStatsWindow {
public:
    RollingStatsWindow() : cap_(0), sum_(0.0), sumsq_(0.0) {}
    explicit RollingStatsWindow(size_t cap) { Reset(cap); }

    void Reset(size_t cap) {
        cap_ = cap;
        buf_.clear();
        sum_ = 0.0;
        sumsq_ = 0.0;
    }

    void Clear() {
        buf_.clear();
        sum_ = 0.0;
        sumsq_ = 0.0;
    }

    void Push(double x) {
        if (cap_ == 0)
            return;
        if (buf_.size() == cap_) {
            const double old = buf_.front();
            buf_.pop_front();
            sum_ -= old;
            sumsq_ -= old * old;
        }
        buf_.push_back(x);
        sum_ += x;
        sumsq_ += x * x;
    }

    size_t Size() const { return buf_.size(); }
    bool Full() const { return (cap_ > 0) && (buf_.size() == cap_); }

    double Mean() const {
        if (buf_.empty())
            return 0.0;
        return sum_ / static_cast<double>(buf_.size());
    }

    // Population stddev (good enough for signal normalization).
    double Std() const {
        const size_t n = buf_.size();
        if (n < 2)
            return 0.0;
        const double mean = Mean();
        const double var = std::max(0.0, (sumsq_ / static_cast<double>(n)) - mean * mean);
        return std::sqrt(var);
    }

    double Last() const { return buf_.empty() ? 0.0 : buf_.back(); }

private:
    size_t cap_;
    std::deque<double> buf_;
    double sum_;
    double sumsq_;
};

struct InstrumentState {
    // Close history for volatility + Bollinger.
    RollingStatsWindow last20;
    RollingStatsWindow boll;

    // EMA state.
    bool ema_init = false;
    double ema_fast = 0.0;
    double ema_slow = 0.0;
    double prev_ema_slow = 0.0;

    // Smoothed macro weight.
    double w_macro = 0.0;

    // Simple position/PNL bookkeeping (bar-close approximation).
    int bars_in_pos = 0;
    double entry_price = 0.0;
    double realized_pnl = 0.0;

    // Kill-switch per instrument (daily loss limit).
    bool trading_enabled = true;
};

} // namespace

class MacroStrategy : public Strategy {
public:
    typedef boost::unordered_map<const Instrument *, InstrumentState> StateMap;
    typedef StateMap::iterator StateIter;

    MacroStrategy(StrategyID strategyID, const std::string &strategyName, const std::string &groupName);
    ~MacroStrategy();

public: /* IEventCallback */
    virtual void OnTrade(const TradeDataEventMsg &msg) {}
    virtual void OnTopQuote(const QuoteEventMsg &msg) {}
    virtual void OnQuote(const QuoteEventMsg &msg) {}
    virtual void OnDepth(const MarketDepthEventMsg &msg) {}
    virtual void OnBar(const BarEventMsg &msg);
    virtual void OnMarketState(const MarketStateEventMsg &msg) {}
    virtual void OnOrderUpdate(const OrderUpdateEventMsg &msg) {}
    virtual void OnStrategyControl(const StrategyStateControlEventMsg &msg) {}
    void OnResetStrategyState();
    void OnDataSubscription(const DataSubscriptionEventMsg &msg) {}
    void OnStrategyCommand(const StrategyCommandEventMsg &msg);
    void OnParamChanged(StrategyParam &param);

private:
    virtual void RegisterForStrategyEvents(StrategyEventRegister *eventRegister, DateType currDate);
    virtual void DefineStrategyParams();
    virtual void DefineStrategyCommands();

private:
    // --- Trading helpers ---
    void AdjustPortfolio(const Instrument *instrument, int desired_position, double ref_price);
    void SendMarketOrder(const Instrument *instrument, int trade_size);

    // --- Macro signal computation (bars only) ---
    double ComputeMacroWeight(InstrumentState &st, double close);

    // --- Risk/exit logic (bars only) ---
    bool ShouldFlatten(const Instrument *instrument,
                       InstrumentState &st,
                       int current_pos,
                       double close) const;

private:
    // Per-instrument state.
    StateMap state_;

    // === Parameters ===
    bool debug_;

    // Data cadence.
    int bar_interval_sec_;

    // Position sizing.
    int max_position_;

    // Macro parameters (from the summary; micro is omitted).
    int ma_fast_period_;
    int ma_slow_period_;
    int bollinger_period_;
    double bollinger_k_;

    double w_ma_weight_;
    double w_boll_weight_;
    double w_trend_weight_;
    double macro_smoothing_alpha_;

    // Risk.
    int tp_ticks_;
    int sl_ticks_;
    int max_hold_time_ms_;
    double daily_loss_limit_;    // In "price * shares" units (e.g., dollars for equities).
    double contract_multiplier_; // Default 1.0.
    double tick_size_fallback_;  // Used if instrument tick size is unavailable.

    // Execution.
    bool use_market_orders_;
};

extern "C" {
_STRATEGY_EXPORTS const char *GetType();
_STRATEGY_EXPORTS Strategy *CreateStrategy(const char *strategyType,
                                           unsigned strategyID,
                                           const char *strategyName,
                                           const char *groupName);
_STRATEGY_EXPORTS const char *GetAuthor();
_STRATEGY_EXPORTS const char *GetAuthorGroup();
_STRATEGY_EXPORTS const char *GetReleaseVersion();
}

#endif
