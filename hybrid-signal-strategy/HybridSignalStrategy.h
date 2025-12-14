#pragma once

#ifndef _STRATEGY_STUDIO_LIB_EXAMPLES_HYBRID_MACRO_MICRO_STRATEGY_H_
#define _STRATEGY_STUDIO_LIB_EXAMPLES_HYBRID_MACRO_MICRO_STRATEGY_H_

#ifdef _WIN32
#define _STRATEGY_EXPORTS __declspec(dllexport)
#else
#ifndef _STRATEGY_EXPORTS
#define _STRATEGY_EXPORTS
#endif
#endif

#include <Analytics/ScalarRollingWindow.h>
#include <MarketModels/Instrument.h>
#include <Strategy.h>

#include <algorithm>
#include <boost/unordered_map.hpp>
#include <cmath>
#include <sstream>
#include <string>

using namespace RCM::StrategyStudio;
using namespace RCM::StrategyStudio::MarketModels;

inline double clip(double x, double lo, double hi) {
    return std::max(lo, std::min(hi, x));
}

inline int iround(double x) {
    return (int)std::floor(x + 0.5); // round-half-up (works well for small ints)
}

/**
 * Incremental EMA tracker. StrategyStudio also has IncrementalEstimation utilities,
 * but EMA is simple and explicit here.
 */
class EmaTracker {
public:
    EmaTracker() : initialized_(false), alpha_(0.1), value_(0.0) {}

    void Reset(double alpha) {
        initialized_ = false;
        alpha_ = alpha;
        value_ = 0.0;
    }
    void SetAlpha(double alpha) { alpha_ = alpha; }

    double Update(double x) {
        if (!initialized_) {
            value_ = x;
            initialized_ = true;
        } else {
            value_ = alpha_ * x + (1.0 - alpha_) * value_;
        }
        return value_;
    }

    bool initialized() const { return initialized_; }
    double value() const { return value_; }

private:
    bool initialized_;
    double alpha_;
    double value_;
};

/**
 * Macro signal: EMA crossover + Bollinger regime score + trend filter, then smoothed.
 *
 * Mirrors the python logic described in the strategy markdown:
 *  - w_ma in [-0.6, 0.6]
 *  - w_boll in [-0.5, 0.5]
 *  - w_trend in [-0.2, 0.2]
 *  - W_macro(t) = s*W_raw + (1-s)*W_macro(t-1)
 */
class MacroSignal {
public:
    MacroSignal(int ma_fast, int ma_slow, int boll_period, double boll_std, double smoothing_alpha);

    void Reset();
    void OnClose(double close_px);

    bool Ready() const;

    double Wmacro() const { return w_macro_; }
    double Wma() const { return w_ma_; }
    double Wboll() const { return w_boll_; }
    double Wtrend() const { return w_trend_; }

private:
    // parameters
    int ma_fast_period_;
    int ma_slow_period_;
    int boll_period_;
    double boll_std_;
    double smoothing_alpha_;

    // state
    Analytics::ScalarRollingWindow<double> closes_; // for recent volatility + bollinger
    EmaTracker ema_fast_;
    EmaTracker ema_slow_;
    double prev_ema_slow_;

    double w_ma_;
    double w_boll_;
    double w_trend_;
    double w_macro_;
};

/**
 * Micro signal: OBI + microprice deviation, confidence-scaled.
 *
 * If you do not have depth levels, set depth_levels=1 and OBI becomes L1-only:
 *   OBI = (Qb - Qa)/(Qb + Qa)
 *
 * This matches the strategy doc:
 *   A_micro = clip((0.6*OBI + 0.4*micro_dev)*c, -1, +1)
 */
class MicroSignal {
public:
    MicroSignal(std::string obi_decay_type, double obi_decay_rate,
                double max_spread, int min_depth);

    void Reset();

    // Update using just BBO (and optionally pass arrays for depth sizes if available).
    void UpdateBBO(double bid_px, double ask_px, double bid_sz, double ask_sz);

    // Optional: Update depth sizes for OBI with L levels (up to 10).
    void UpdateDepthSizes(const double *bid_sz_L, const double *ask_sz_L, int L);

    // Compute alpha; call after UpdateBBO/UpdateDepthSizes.
    double ComputeAlpha();

    double alpha() const { return alpha_; }
    double obi() const { return obi_; }
    double micro_dev() const { return micro_dev_; }
    double confidence() const { return conf_; }

private:
    double compute_obi_();
    double compute_micro_dev_();
    double compute_conf_();

    // params
    std::string obi_decay_type_;
    double obi_decay_rate_;
    double max_spread_;
    int min_book_depth_;

    // latest book snapshot
    double bid_px_, ask_px_, bid_sz_, ask_sz_;
    double spread_;

    // depth (optional)
    static const int MAX_L = 10;
    double bid_sz_levels_[MAX_L];
    double ask_sz_levels_[MAX_L];
    int L_;

    // outputs
    double obi_;
    double micro_dev_;
    double conf_;
    double alpha_;
};

/**
 * Minimal position state.
 * NOTE: StrategyStudio has portfolio() positions, but keeping a local avg_price + entry_time
 * makes SL/TP/max-hold easy to implement.
 */
struct LocalPositionState {
    int size = 0; // +long, -short
    double avg_price = 0.0;
    Timestamp entry_time; // StrategyStudio timestamp type (platform-specific)
    bool has_entry_time = false;
};

class HybridSignalStrategy : public Strategy {
public:
    HybridSignalStrategy(StrategyID strategyID, const std::string &strategyName, const std::string &groupName);
    ~HybridSignalStrategy();

public: /* IEventCallback */
    virtual void OnTrade(const TradeDataEventMsg &msg) {}
    virtual void OnTopQuote(const QuoteEventMsg &msg);
    virtual void OnQuote(const QuoteEventMsg &msg) {}
    virtual void OnDepth(const MarketDepthEventMsg &msg);
    virtual void OnBar(const BarEventMsg &msg);
    virtual void OnMarketState(const MarketStateEventMsg &msg) {}
    virtual void OnOrderUpdate(const OrderUpdateEventMsg &msg);
    virtual void OnStrategyControl(const StrategyStateControlEventMsg &msg) {}

    void OnResetStrategyState();
    void OnDataSubscription(const DataSubscriptionEventMsg &msg) {}

    void OnStrategyCommand(const StrategyCommandEventMsg &msg);
    void OnParamChanged(StrategyParam &param);

private: /* Strategy */
    virtual void RegisterForStrategyEvents(StrategyEventRegister *eventRegister, DateType currDate);
    virtual void DefineStrategyParams();
    virtual void DefineStrategyCommands();

private:
    // --- core "evaluate" path (called from quote/depth callbacks) ---
    void EvaluateAndTrade(const Instrument *instrument, const Timestamp &now_ts);

    // --- exits / risk ---
    void CheckExits(const Instrument *instrument, const Timestamp &now_ts, double mid_px);

    // --- portfolio adjustment ---
    void AdjustPortfolio(const Instrument *instrument, int desired_position);
    void SendOrder(const Instrument *instrument, int trade_size);

    // --- helpers ---
    bool CanTrade(const Timestamp &now_ts);
    double MidPrice(const Instrument *instrument) const;

private:
    // per-instrument state: macro depends on bar stream (could be per symbol)
    typedef boost::unordered_map<const Instrument *, MacroSignal> MacroMap;
    typedef MacroMap::iterator MacroMapIter;
    MacroMap macro_map_;

    // micro can be per-instrument as well
    typedef boost::unordered_map<const Instrument *, MicroSignal> MicroMap;
    typedef MicroMap::iterator MicroMapIter;
    MicroMap micro_map_;

    // simple position state per instrument
    typedef boost::unordered_map<const Instrument *, LocalPositionState> PosMap;
    PosMap pos_map_;

    // --- parameters (match python config) ---
    // Macro
    int ma_fast_period_;
    int ma_slow_period_;
    int bollinger_period_;
    double bollinger_std_;
    double weight_ma_;
    double weight_boll_;
    double weight_trend_;
    double macro_smoothing_alpha_;

    // Micro
    std::string obi_decay_type_;
    double obi_decay_rate_;
    double max_spread_;
    int min_book_depth_;

    // Blend + sizing
    double macro_weight_alpha_;
    int max_position_;

    // Risk
    double tp_ticks_;
    double sl_ticks_;
    int max_hold_time_ms_;
    double daily_loss_limit_;

    // Execution
    int evaluation_interval_ms_;
    int trade_cooldown_ms_;
    int max_trades_per_minute_;
    double tick_size_;
    double slippage_ticks_;
    double aggressiveness_; // optional offset for repricing / marketability

    // Debug
    bool debug_;

    // --- runtime throttles / kill switch ---
    bool killed_;
    double cumulative_pnl_;
    Timestamp last_eval_ts_;
    bool has_last_eval_;

    Timestamp last_trade_ts_;
    bool has_last_trade_;
    int trades_this_minute_;
    int current_minute_; // from Timestamp (platform dependent)
};

extern "C" {
_STRATEGY_EXPORTS const char *GetType() { return "HybridSignalStrategy"; }

_STRATEGY_EXPORTS IStrategy *CreateStrategy(const char *strategyType,
                                            unsigned strategyID,
                                            const char *strategyName,
                                            const char *groupName) {
    if (strcmp(strategyType, GetType()) == 0) {
        return *(new HybridSignalStrategy(strategyID, strategyName, groupName));
    }
    return NULL;
}

_STRATEGY_EXPORTS const char *GetAuthor() { return "dlariviere"; }
_STRATEGY_EXPORTS const char *GetAuthorGroup() { return "UIUC"; }
_STRATEGY_EXPORTS const char *GetReleaseVersion() { return Strategy::release_version(); }
}

#endif