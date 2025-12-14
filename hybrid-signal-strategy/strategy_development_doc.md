# Hybrid Macro–Micro Strategy in StrategyStudio (C++ Template Port)

This is a **StrategyStudio-oriented port** of the Python “hybrid macro + micro” strategy you summarized earlier. It’s written to match the typical **`.h` / `.cpp` plugin template** style (like `SMAReactiveTakerStrategy`), while keeping the same core trading logic:

- Macro signal `W_macro(t)` computed from **bar closes** (EMA crossover + Bollinger regime score + trend filter), smoothed. fileciteturn1file2L52-L72  
- Micro signal `A_micro(t)` computed from **top-of-book / depth** (OBI + microprice deviation), confidence-scaled. fileciteturn1file3L15-L73  
- Combined bias → target position → trade-to-target with exits (SL/TP/max hold) and a kill switch. fileciteturn1file0L7-L19  

> **Important “unknowns” / platform-dependent parts (called out inline):**
> 1. Whether your StrategyStudio data feed provides **full MBP-10 depth** vs only **TopQuote**. If you only have BBO, OBI becomes “L1-only” (still works, just weaker).  
> 2. Fill handling: StrategyStudio provides order/fill events, but APIs vary by deployment. The code below shows a common pattern: update position on `OnOrderUpdate()` when orders complete; in a real setup you’ll want to use fill-specific callbacks/fields (if available) to compute realized PnL precisely.

---

## 1) Strategy mapping: Python loop → StrategyStudio callbacks

Python backtest loop does “evaluate every N ms” and updates macro only on bar boundaries.

In StrategyStudio, the typical mapping is:

- **`OnBar()`** (e.g., 60-second bars) → update the macro features and `W_macro`.
- **`OnTopQuote()` and/or `OnDepth()`** → update micro features and run **re-target + risk** logic *with throttling* (your “evaluation_interval_ms”).

The combined step is the same:

```
bias = clip(alpha * W_macro + (1-alpha) * A_micro, -1, +1)
target_pos = round(bias * max_position)
```
fileciteturn1file3L77-L86

---

## 2) Header file: `HybridMacroMicroStrategy.h`

```cpp
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

#include <Strategy.h>
#include <MarketModels/Instrument.h>
#include <Analytics/ScalarRollingWindow.h>

#include <boost/unordered_map.hpp>
#include <cmath>
#include <sstream>
#include <string>
#include <algorithm>

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

  void Reset(double alpha) { initialized_ = false; alpha_ = alpha; value_ = 0.0; }
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
  Analytics::ScalarRollingWindow<double> closes_;       // for recent volatility + bollinger
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
  void UpdateDepthSizes(const double* bid_sz_L, const double* ask_sz_L, int L);

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
  int size = 0;                  // +long, -short
  double avg_price = 0.0;
  Timestamp entry_time;          // StrategyStudio timestamp type (platform-specific)
  bool has_entry_time = false;
};

class HybridMacroMicroStrategy : public Strategy {
public:
  HybridMacroMicroStrategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName);
  ~HybridMacroMicroStrategy();

public: /* IEventCallback */
  virtual void OnTrade(const TradeDataEventMsg& msg) {}
  virtual void OnTopQuote(const QuoteEventMsg& msg);
  virtual void OnQuote(const QuoteEventMsg& msg) {}
  virtual void OnDepth(const MarketDepthEventMsg& msg);
  virtual void OnBar(const BarEventMsg& msg);
  virtual void OnMarketState(const MarketStateEventMsg& msg) {}
  virtual void OnOrderUpdate(const OrderUpdateEventMsg& msg);
  virtual void OnStrategyControl(const StrategyStateControlEventMsg& msg) {}

  void OnResetStrategyState();
  void OnDataSubscription(const DataSubscriptionEventMsg& msg) {}

  void OnStrategyCommand(const StrategyCommandEventMsg& msg);
  void OnParamChanged(StrategyParam& param);

private: /* Strategy */
  virtual void RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate);
  virtual void DefineStrategyParams();
  virtual void DefineStrategyCommands();

private:
  // --- core "evaluate" path (called from quote/depth callbacks) ---
  void EvaluateAndTrade(const Instrument* instrument, const Timestamp& now_ts);

  // --- exits / risk ---
  void CheckExits(const Instrument* instrument, const Timestamp& now_ts, double mid_px);

  // --- portfolio adjustment ---
  void AdjustPortfolio(const Instrument* instrument, int desired_position);
  void SendOrder(const Instrument* instrument, int trade_size);

  // --- helpers ---
  bool CanTrade(const Timestamp& now_ts);
  double MidPrice(const Instrument* instrument) const;

private:
  // per-instrument state: macro depends on bar stream (could be per symbol)
  typedef boost::unordered_map<const Instrument*, MacroSignal> MacroMap;
  typedef MacroMap::iterator MacroMapIter;
  MacroMap macro_map_;

  // micro can be per-instrument as well
  typedef boost::unordered_map<const Instrument*, MicroSignal> MicroMap;
  typedef MicroMap::iterator MicroMapIter;
  MicroMap micro_map_;

  // simple position state per instrument
  typedef boost::unordered_map<const Instrument*, LocalPositionState> PosMap;
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
  _STRATEGY_EXPORTS const char* GetType() { return "HybridMacroMicroStrategy"; }

  _STRATEGY_EXPORTS IStrategy* CreateStrategy(const char* strategyType,
                                             unsigned strategyID,
                                             const char* strategyName,
                                             const char* groupName) {
    if (strcmp(strategyType, GetType()) == 0) {
      return *(new HybridMacroMicroStrategy(strategyID, strategyName, groupName));
    }
    return NULL;
  }

  _STRATEGY_EXPORTS const char* GetAuthor() { return "Jonathan (port based on Python strategy)"; }
  _STRATEGY_EXPORTS const char* GetAuthorGroup() { return "UIUC"; }
  _STRATEGY_EXPORTS const char* GetReleaseVersion() { return Strategy::release_version(); }
}

#endif
```

---

## 3) Implementation: `HybridMacroMicroStrategy.cpp`

```cpp
#ifdef _WIN32
  #include "stdafx.h"
#endif

#include "HybridMacroMicroStrategy.h"

#include "FillInfo.h"
#include "AllEventMsg.h"
#include "ExecutionTypes.h"
#include <Utilities/Cast.h>
#include <Utilities/utils.h>

#include <iostream>
#include <cassert>

using namespace RCM::StrategyStudio;
using namespace RCM::StrategyStudio::MarketModels;
using namespace RCM::StrategyStudio::Utilities;

// ------------------------ MacroSignal ------------------------

static double stddev(const Analytics::ScalarRollingWindow<double>& w) {
  if (w.size() <= 1) return 0.0;
  double m = w.Mean();
  double acc = 0.0;
  for (size_t i = 0; i < w.size(); ++i) {
    double d = w[i] - m;
    acc += d * d;
  }
  return std::sqrt(acc / (double)(w.size() - 1));
}

MacroSignal::MacroSignal(int ma_fast, int ma_slow, int boll_period, double boll_std, double smoothing_alpha)
: ma_fast_period_(ma_fast),
  ma_slow_period_(ma_slow),
  boll_period_(boll_period),
  boll_std_(boll_std),
  smoothing_alpha_(smoothing_alpha),
  closes_(std::max(100, std::max(ma_slow, boll_period))), // keep enough history
  prev_ema_slow_(0.0),
  w_ma_(0.0), w_boll_(0.0), w_trend_(0.0), w_macro_(0.0)
{
  // EMA alpha for span N: 2/(N+1)
  ema_fast_.Reset(2.0 / (ma_fast_period_ + 1.0));
  ema_slow_.Reset(2.0 / (ma_slow_period_ + 1.0));
}

void MacroSignal::Reset() {
  closes_.clear();
  ema_fast_.Reset(2.0 / (ma_fast_period_ + 1.0));
  ema_slow_.Reset(2.0 / (ma_slow_period_ + 1.0));
  prev_ema_slow_ = 0.0;
  w_ma_ = w_boll_ = w_trend_ = w_macro_ = 0.0;
}

bool MacroSignal::Ready() const {
  return closes_.full(); // conservative
}

void MacroSignal::OnClose(double close_px) {
  closes_.push_back(close_px);

  // update EMAs
  double f = ema_fast_.Update(close_px);
  double s = ema_slow_.Update(close_px);

  // --- 1) w_ma (EMA crossover score) ---
  double ma_diff = f - s;

  // recent volatility = std(last 20 closes) like python
  // (we approximate by taking the last min(20, size) points from closes_)
  int n = (int)std::min<size_t>(20, closes_.size());
  Analytics::ScalarRollingWindow<double> tmp(n);
  for (int i = (int)closes_.size() - n; i < (int)closes_.size(); ++i) tmp.push_back(closes_[i]);
  double recent_vol = stddev(tmp);

  if (recent_vol <= 1e-12) {
    w_ma_ = (ma_diff > 0) ? 0.3 : -0.3;
  } else {
    double normalized = ma_diff / (recent_vol + 1e-8);
    w_ma_ = clip(normalized * 0.5, -0.6, 0.6);
  }

  // --- 2) w_boll (Bollinger regime score) ---
  if ((int)closes_.size() >= boll_period_) {
    // compute SMA/std over last boll_period_ points
    Analytics::ScalarRollingWindow<double> bw(boll_period_);
    for (int i = (int)closes_.size() - boll_period_; i < (int)closes_.size(); ++i) bw.push_back(closes_[i]);
    double sma = bw.Mean();
    double sd = stddev(bw);

    if (sd > 1e-12) {
      double upper = sma + boll_std_ * sd;
      double lower = sma - boll_std_ * sd;
      double bandw = upper - lower;
      if (bandw > 1e-12) {
        double dist_lower = (close_px - lower) / bandw;
        double dist_upper = (close_px - upper) / bandw;

        // vol_percentile proxy: recent_vol / std
        double vol_percentile = recent_vol / (sd + 1e-8);

        double score = 0.0;
        if (vol_percentile < 0.8) {
          // mean reversion regime
          if (dist_lower < 0.2) score = 0.5 * (1.0 - dist_lower / 0.2);
          else if (dist_upper > 0.8) score = -0.5 * ((dist_upper - 0.8) / 0.2);
        } else {
          // breakout regime
          if (dist_upper > 0.8) score = 0.3;
          else if (dist_lower < 0.2) score = -0.3;
        }
        w_boll_ = clip(score, -0.5, 0.5);
      } else {
        w_boll_ = 0.0;
      }
    } else {
      w_boll_ = 0.0;
    }
  } else {
    w_boll_ = 0.0;
  }

  // --- 3) w_trend (slow EMA slope normalized) ---
  // python: slope = (slow_ema[t]-slow_ema[t-1]) / slow_ema[t-1]
  // normalize by recent_vol and clip to [-0.2, 0.2] after scaling.
  if (ema_slow_.initialized()) {
    if (prev_ema_slow_ == 0.0) prev_ema_slow_ = s;
    double slope = (s - prev_ema_slow_) / (prev_ema_slow_ + 1e-8);
    prev_ema_slow_ = s;

    if (recent_vol <= 1e-12) w_trend_ = 0.0;
    else {
      double normalized_slope = slope / (recent_vol + 1e-8);
      w_trend_ = clip(normalized_slope * 10.0, -0.2, 0.2);
    }
  } else {
    w_trend_ = 0.0;
  }

  // --- Combine raw macro weight (weights applied by strategy class) ---
  // Here we just maintain the smoothed W_macro internal state as "w_macro_"
  // The Strategy class will compute raw_weight and then smooth it the same way.
}

// ------------------------ MicroSignal ------------------------

MicroSignal::MicroSignal(std::string obi_decay_type, double obi_decay_rate,
                         double max_spread, int min_depth)
: obi_decay_type_(obi_decay_type),
  obi_decay_rate_(obi_decay_rate),
  max_spread_(max_spread),
  min_book_depth_(min_depth),
  bid_px_(0), ask_px_(0), bid_sz_(0), ask_sz_(0), spread_(0),
  L_(1),
  obi_(0), micro_dev_(0), conf_(0), alpha_(0)
{
  std::fill(bid_sz_levels_, bid_sz_levels_ + MAX_L, 0.0);
  std::fill(ask_sz_levels_, ask_sz_levels_ + MAX_L, 0.0);
}

void MicroSignal::Reset() {
  bid_px_ = ask_px_ = bid_sz_ = ask_sz_ = spread_ = 0.0;
  L_ = 1;
  std::fill(bid_sz_levels_, bid_sz_levels_ + MAX_L, 0.0);
  std::fill(ask_sz_levels_, ask_sz_levels_ + MAX_L, 0.0);
  obi_ = micro_dev_ = conf_ = alpha_ = 0.0;
}

void MicroSignal::UpdateBBO(double bid_px, double ask_px, double bid_sz, double ask_sz) {
  bid_px_ = bid_px; ask_px_ = ask_px;
  bid_sz_ = bid_sz; ask_sz_ = ask_sz;
  spread_ = ask_px_ - bid_px_;

  // default depth = L1 if not updated
  bid_sz_levels_[0] = bid_sz_;
  ask_sz_levels_[0] = ask_sz_;
  L_ = std::max(L_, 1);
}

void MicroSignal::UpdateDepthSizes(const double* bid_sz_L, const double* ask_sz_L, int L) {
  L_ = std::max(1, std::min(L, MAX_L));
  for (int i = 0; i < L_; ++i) {
    bid_sz_levels_[i] = bid_sz_L[i];
    ask_sz_levels_[i] = ask_sz_L[i];
  }
}

double MicroSignal::compute_obi_() {
  double bid_vol = 0.0, ask_vol = 0.0;
  for (int l = 0; l < L_; ++l) {
    double w = 1.0;
    if (obi_decay_type_ == "exponential") w = std::exp(-obi_decay_rate_ * (double)l);
    else w = 1.0 / (double)(l + 1);

    bid_vol += w * bid_sz_levels_[l];
    ask_vol += w * ask_sz_levels_[l];
  }
  double denom = bid_vol + ask_vol;
  if (denom <= 1e-12) return 0.0;
  return clip((bid_vol - ask_vol) / denom, -1.0, 1.0);
}

double MicroSignal::compute_micro_dev_() {
  if (spread_ <= 1e-12) return 0.0;
  double mid = 0.5 * (bid_px_ + ask_px_);
  double total_sz = bid_sz_ + ask_sz_;
  if (total_sz <= 1e-12) return 0.0;

  double microprice = (ask_px_ * bid_sz_ + bid_px_ * ask_sz_) / total_sz;
  return (microprice - mid) / spread_;
}

double MicroSignal::compute_conf_() {
  if (spread_ > max_spread_) return 0.0;

  double min_depth = std::min(bid_sz_, ask_sz_);
  if (min_depth < (double)min_book_depth_) return 0.3;

  double spread_conf = 1.0 - std::min(spread_ / max_spread_, 1.0);
  double depth_conf = std::min(min_depth / 10.0, 1.0);
  return clip(0.6 * spread_conf + 0.4 * depth_conf, 0.0, 1.0);
}

double MicroSignal::ComputeAlpha() {
  obi_ = compute_obi_();
  micro_dev_ = compute_micro_dev_();
  conf_ = compute_conf_();

  double raw = 0.6 * obi_ + 0.4 * micro_dev_;
  alpha_ = clip(raw * conf_, -1.0, 1.0);
  return alpha_;
}

// ------------------------ HybridMacroMicroStrategy ------------------------

HybridMacroMicroStrategy::HybridMacroMicroStrategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName)
: Strategy(strategyID, strategyName, groupName),
  // reasonable defaults; expose via DefineStrategyParams()
  ma_fast_period_(10),
  ma_slow_period_(30),
  bollinger_period_(20),
  bollinger_std_(2.0),
  weight_ma_(0.4),
  weight_boll_(0.4),
  weight_trend_(0.2),
  macro_smoothing_alpha_(0.3),
  obi_decay_type_("exponential"),
  obi_decay_rate_(0.5),
  max_spread_(10.0),
  min_book_depth_(2),
  macro_weight_alpha_(0.6),
  max_position_(10),
  tp_ticks_(0.0025),
  sl_ticks_(0.0025),
  max_hold_time_ms_(30000),
  daily_loss_limit_(-5000.0),
  evaluation_interval_ms_(500),
  trade_cooldown_ms_(200),
  max_trades_per_minute_(60),
  tick_size_(0.0005),
  slippage_ticks_(0.5),
  aggressiveness_(0.0),
  debug_(false),
  killed_(false),
  cumulative_pnl_(0.0),
  has_last_eval_(false),
  has_last_trade_(false),
  trades_this_minute_(0),
  current_minute_(-1)
{}

HybridMacroMicroStrategy::~HybridMacroMicroStrategy() {}

void HybridMacroMicroStrategy::OnResetStrategyState() {
  macro_map_.clear();
  micro_map_.clear();
  pos_map_.clear();
  killed_ = false;
  cumulative_pnl_ = 0.0;
  has_last_eval_ = false;
  has_last_trade_ = false;
  trades_this_minute_ = 0;
  current_minute_ = -1;
}

void HybridMacroMicroStrategy::DefineStrategyParams() {
  // Macro
  params().CreateParam(CreateStrategyParamArgs("ma_fast_period", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, ma_fast_period_));
  params().CreateParam(CreateStrategyParamArgs("ma_slow_period", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, ma_slow_period_));
  params().CreateParam(CreateStrategyParamArgs("bollinger_period", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, bollinger_period_));
  params().CreateParam(CreateStrategyParamArgs("bollinger_std", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, bollinger_std_));
  params().CreateParam(CreateStrategyParamArgs("weight_ma", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, weight_ma_));
  params().CreateParam(CreateStrategyParamArgs("weight_boll", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, weight_boll_));
  params().CreateParam(CreateStrategyParamArgs("weight_trend", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, weight_trend_));
  params().CreateParam(CreateStrategyParamArgs("macro_smoothing_alpha", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, macro_smoothing_alpha_));

  // Micro
  // NOTE: StrategyStudio params may not support string directly; if not, replace with enum/int param.
  // params().CreateParam(CreateStrategyParamArgs("obi_decay_type", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_STRING, obi_decay_type_));
  params().CreateParam(CreateStrategyParamArgs("obi_decay_rate", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, obi_decay_rate_));
  params().CreateParam(CreateStrategyParamArgs("max_spread", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, max_spread_));
  params().CreateParam(CreateStrategyParamArgs("min_book_depth", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, min_book_depth_));

  // Blend + sizing
  params().CreateParam(CreateStrategyParamArgs("macro_weight_alpha", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, macro_weight_alpha_));
  params().CreateParam(CreateStrategyParamArgs("max_position", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, max_position_));

  // Risk
  params().CreateParam(CreateStrategyParamArgs("tp_ticks", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_DOUBLE, tp_ticks_));
  params().CreateParam(CreateStrategyParamArgs("sl_ticks", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_DOUBLE, sl_ticks_));
  params().CreateParam(CreateStrategyParamArgs("max_hold_time_ms", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, max_hold_time_ms_));
  params().CreateParam(CreateStrategyParamArgs("daily_loss_limit", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_DOUBLE, daily_loss_limit_));

  // Execution
  params().CreateParam(CreateStrategyParamArgs("evaluation_interval_ms", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, evaluation_interval_ms_));
  params().CreateParam(CreateStrategyParamArgs("trade_cooldown_ms", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, trade_cooldown_ms_));
  params().CreateParam(CreateStrategyParamArgs("max_trades_per_minute", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, max_trades_per_minute_));
  params().CreateParam(CreateStrategyParamArgs("tick_size", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, tick_size_));
  params().CreateParam(CreateStrategyParamArgs("slippage_ticks", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_DOUBLE, slippage_ticks_));
  params().CreateParam(CreateStrategyParamArgs("aggressiveness", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_DOUBLE, aggressiveness_));

  // Debug
  params().CreateParam(CreateStrategyParamArgs("debug", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_BOOL, debug_));
}

void HybridMacroMicroStrategy::DefineStrategyCommands() {
  commands().AddCommand(StrategyCommand(1, "Cancel All Orders"));
  commands().AddCommand(StrategyCommand(2, "Reset Kill Switch"));
}

void HybridMacroMicroStrategy::RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType /*currDate*/) {
  for (SymbolSetConstIter it = symbols_begin(); it != symbols_end(); ++it) {
    // Macro update cadence: 60s bars is a typical match for 1-min OHLCV.
    eventRegister->RegisterForBars(*it, BAR_TYPE_TIME, 60);

    // Micro updates:
    eventRegister->RegisterForTopQuotes(*it);
    // If depth is available, register for it too (platform dependent).
    eventRegister->RegisterForDepth(*it);
  }
}

double HybridMacroMicroStrategy::MidPrice(const Instrument* instrument) const {
  const TopQuote& tq = instrument->top_quote();
  if (!tq.bid_side().IsValid() || !tq.ask_side().IsValid()) return 0.0;
  return 0.5 * (tq.bid() + tq.ask());
}

bool HybridMacroMicroStrategy::CanTrade(const Timestamp& now_ts) {
  if (killed_) return false;
  if (cumulative_pnl_ < daily_loss_limit_) { killed_ = true; return false; }

  // cooldown
  if (has_last_trade_) {
    // NOTE: Timestamp arithmetic API differs; this is pseudo-ish.
    // Replace with: (now_ts - last_trade_ts_).total_milliseconds() or similar.
    long dt_ms = (long)Utilities::ToMilliseconds(now_ts - last_trade_ts_);
    if (dt_ms < trade_cooldown_ms_) return false;
  }

  // per-minute throttle (platform dependent minute extraction)
  int minute = now_ts.minute(); // if unavailable, compute from epoch
  if (minute != current_minute_) {
    current_minute_ = minute;
    trades_this_minute_ = 0;
  }
  if (max_trades_per_minute_ > 0 && trades_this_minute_ >= max_trades_per_minute_) return false;

  return true;
}

void HybridMacroMicroStrategy::OnBar(const BarEventMsg& msg) {
  const Instrument* inst = &msg.instrument();
  double close_px = msg.bar().close();
  if (close_px <= 0.0) return;

  // get/create per-instrument macro signal
  MacroMapIter it = macro_map_.find(inst);
  if (it == macro_map_.end()) {
    it = macro_map_.insert(std::make_pair(inst, MacroSignal(
      ma_fast_period_, ma_slow_period_, bollinger_period_, bollinger_std_, macro_smoothing_alpha_
    ))).first;
  }

  it->second.OnClose(close_px);

  if (debug_) {
    std::ostringstream ss;
    ss << "OnBar " << inst->symbol() << " close=" << close_px;
    logger().LogToClient(LOGLEVEL_DEBUG, ss.str());
  }
}

void HybridMacroMicroStrategy::OnTopQuote(const QuoteEventMsg& msg) {
  const Instrument* inst = &msg.instrument();
  const TopQuote& tq = inst->top_quote();
  if (!tq.bid_side().IsValid() || !tq.ask_side().IsValid()) return;

  // update micro state from BBO
  MicroMapIter it = micro_map_.find(inst);
  if (it == micro_map_.end()) {
    it = micro_map_.insert(std::make_pair(inst, MicroSignal(
      obi_decay_type_, obi_decay_rate_, max_spread_, min_book_depth_
    ))).first;
  }
  it->second.UpdateBBO(tq.bid(), tq.ask(), tq.bid_size(), tq.ask_size());

  EvaluateAndTrade(inst, msg.adapter_time());
}

void HybridMacroMicroStrategy::OnDepth(const MarketDepthEventMsg& msg) {
  // If depth is available, update OBI with multiple levels.
  // NOTE: the exact API to access multiple levels is feed-dependent.
  // Many StrategyStudio deployments expose only top-of-book via TopQuote.
  const Instrument* inst = &msg.instrument();

  MicroMapIter it = micro_map_.find(inst);
  if (it == micro_map_.end()) {
    it = micro_map_.insert(std::make_pair(inst, MicroSignal(
      obi_decay_type_, obi_decay_rate_, max_spread_, min_book_depth_
    ))).first;
  }

  // PSEUDO: replace with your actual depth accessor.
  // double bidL[10], askL[10]; int L = FillDepthArrays(msg, bidL, askL);
  // it->second.UpdateDepthSizes(bidL, askL, L);

  EvaluateAndTrade(inst, msg.adapter_time());
}

void HybridMacroMicroStrategy::EvaluateAndTrade(const Instrument* instrument, const Timestamp& now_ts) {
  // throttle evaluation (evaluation_interval_ms_)
  if (has_last_eval_) {
    long dt_ms = (long)Utilities::ToMilliseconds(now_ts - last_eval_ts_);
    if (dt_ms < evaluation_interval_ms_) return;
  }
  last_eval_ts_ = now_ts;
  has_last_eval_ = true;

  // need macro + micro
  MacroMapIter mac_it = macro_map_.find(instrument);
  MicroMapIter mic_it = micro_map_.find(instrument);
  if (mic_it == micro_map_.end()) return;

  double w_macro = 0.0, w_ma = 0.0, w_boll = 0.0, w_trend = 0.0;
  if (mac_it != macro_map_.end()) {
    // compute raw macro weight from sub-scores (same blend as python)
    w_ma = mac_it->second.Wma();
    w_boll = mac_it->second.Wboll();
    w_trend = mac_it->second.Wtrend();

    double raw = weight_ma_ * w_ma + weight_boll_ * w_boll + weight_trend_ * w_trend;

    // trend dampening like doc: if against trend, scale 0.7 fileciteturn1file3L1-L11
    if (w_trend < 0 && raw > 0) raw *= 0.7;
    else if (w_trend > 0 && raw < 0) raw *= 0.7;

    raw = clip(raw, -1.0, 1.0);

    // smooth W_macro: W = s*raw + (1-s)*prevW fileciteturn1file3L7-L11
    // We store smoothed value inside MacroSignal? For simplicity store it here:
    static boost::unordered_map<const Instrument*, double> wmacro_prev;
    double prev = wmacro_prev[instrument];
    w_macro = macro_smoothing_alpha_ * raw + (1.0 - macro_smoothing_alpha_) * prev;
    wmacro_prev[instrument] = w_macro;
  } else {
    w_macro = 0.0;
  }

  double a_micro = mic_it->second.ComputeAlpha();

  // Combined bias and target position fileciteturn1file0L7-L19
  double bias = clip(macro_weight_alpha_ * w_macro + (1.0 - macro_weight_alpha_) * a_micro, -1.0, 1.0);
  int target_pos = iround(bias * (double)max_position_);

  // Check exits first (SL/TP/max hold) fileciteturn1file0L11-L15
  double mid = MidPrice(instrument);
  if (mid > 0.0) CheckExits(instrument, now_ts, mid);

  // Re-target
  int current_pos = portfolio().position(instrument); // actual broker/model position
  int delta = target_pos - current_pos;

  if (std::abs(delta) >= 1 && CanTrade(now_ts)) {
    AdjustPortfolio(instrument, target_pos);
  }

  if (debug_) {
    std::ostringstream ss;
    ss << "EVAL " << instrument->symbol()
       << " Wm=" << w_macro << " Am=" << a_micro
       << " bias=" << bias
       << " target=" << target_pos
       << " pos=" << current_pos;
    logger().LogToClient(LOGLEVEL_DEBUG, ss.str());
  }
}

void HybridMacroMicroStrategy::CheckExits(const Instrument* instrument, const Timestamp& now_ts, double mid_px) {
  // Local SL/TP tracking uses local avg_price/entry_time; if you prefer, derive from fills + portfolio.
  LocalPositionState& st = pos_map_[instrument];

  int pos = portfolio().position(instrument);
  if (pos == 0) { st.size = 0; st.avg_price = 0.0; st.has_entry_time = false; return; }

  // If local state isn't initialized, initialize it conservatively.
  if (st.size != pos) {
    st.size = pos;
    st.avg_price = mid_px; // unknown; ideally use fill avg
    st.entry_time = now_ts;
    st.has_entry_time = true;
  }

  double unreal = 0.0;
  if (pos > 0) unreal = (mid_px - st.avg_price) * pos;
  else unreal = (st.avg_price - mid_px) * std::abs(pos);

  double pnl_per_contract = unreal / (double)std::max(1, std::abs(pos));

  bool exit = false;
  std::string reason;

  if (pnl_per_contract <= -sl_ticks_) { exit = true; reason = "STOP_LOSS"; }
  else if (pnl_per_contract >= tp_ticks_) { exit = true; reason = "TAKE_PROFIT"; }

  if (st.has_entry_time) {
    long hold_ms = (long)Utilities::ToMilliseconds(now_ts - st.entry_time);
    if (hold_ms > max_hold_time_ms_) { exit = true; reason = "MAX_HOLD_TIME"; }
  }

  if (exit && CanTrade(now_ts)) {
    AdjustPortfolio(instrument, 0); // flatten
    if (debug_) {
      std::ostringstream ss;
      ss << "EXIT " << instrument->symbol() << " " << reason;
      logger().LogToClient(LOGLEVEL_DEBUG, ss.str());
    }
  }
}

void HybridMacroMicroStrategy::AdjustPortfolio(const Instrument* instrument, int desired_position) {
  int current_position = portfolio().position(instrument);
  int trade_size = desired_position - current_position;
  if (trade_size == 0) return;

  // ensure no working orders before sending a new one (simple taker model)
  if (orders().num_working_orders(instrument) == 0) {
    SendOrder(instrument, trade_size);
  } else {
    // if existing order conflicts with desired direction, cancel it
    const Order* order = *orders().working_orders_begin(instrument);
    if (order) {
      bool order_is_buy = IsBuySide(order->order_side());
      if ((order_is_buy && trade_size < 0) || (!order_is_buy && trade_size > 0)) {
        trade_actions()->SendCancelOrder(order->order_id());
      }
    }
  }
}

void HybridMacroMicroStrategy::SendOrder(const Instrument* instrument, int trade_size) {
  const TopQuote& tq = instrument->top_quote();
  if (!tq.bid_side().IsValid() || !tq.ask_side().IsValid()) return;

  // To behave like a "taker", buy at ask and sell at bid (optionally +/- slippage/aggressiveness).
  double px = 0.0;
  if (trade_size > 0) {
    px = tq.ask() + aggressiveness_ + slippage_ticks_ * tick_size_;
  } else {
    px = tq.bid() - aggressiveness_ - slippage_ticks_ * tick_size_;
  }

  OrderParams p(*instrument,
                std::abs(trade_size),
                px,
                MARKET_CENTER_ID_NASDAQ, // replace with your venue
                (trade_size > 0) ? ORDER_SIDE_BUY : ORDER_SIDE_SELL,
                ORDER_TIF_DAY,
                ORDER_TYPE_LIMIT);

  trade_actions()->SendNewOrder(p);

  last_trade_ts_ = current_time();
  has_last_trade_ = true;
  trades_this_minute_++;
}

void HybridMacroMicroStrategy::OnOrderUpdate(const OrderUpdateEventMsg& msg) {
  // Platform-specific: when you can access fills, update avg_price and realized pnl accurately.
  // Here we just clear local state on completion and let CheckExits re-init on next tick.
  if (msg.completes_order()) {
    const Instrument* inst = msg.order().instrument();
    // if position flattened, clear state
    if (portfolio().position(inst) == 0) {
      pos_map_[inst] = LocalPositionState();
    }
  }
}

void HybridMacroMicroStrategy::OnStrategyCommand(const StrategyCommandEventMsg& msg) {
  switch (msg.command_id()) {
    case 1:
      trade_actions()->SendCancelAll();
      break;
    case 2:
      killed_ = false;
      break;
    default:
      break;
  }
}

void HybridMacroMicroStrategy::OnParamChanged(StrategyParam& param) {
  // handle runtime changes as needed (same style as SMAReactiveTakerStrategy)
  if (param.param_name() == "max_position") param.Get(&max_position_);
  else if (param.param_name() == "tp_ticks") param.Get(&tp_ticks_);
  else if (param.param_name() == "sl_ticks") param.Get(&sl_ticks_);
  else if (param.param_name() == "max_hold_time_ms") param.Get(&max_hold_time_ms_);
  else if (param.param_name() == "daily_loss_limit") param.Get(&daily_loss_limit_);
  else if (param.param_name() == "evaluation_interval_ms") param.Get(&evaluation_interval_ms_);
  else if (param.param_name() == "trade_cooldown_ms") param.Get(&trade_cooldown_ms_);
  else if (param.param_name() == "max_trades_per_minute") param.Get(&max_trades_per_minute_);
  else if (param.param_name() == "aggressiveness") param.Get(&aggressiveness_);
  else if (param.param_name() == "debug") param.Get(&debug_);
}
```

---

## 4) What you may need to adjust for your StrategyStudio install

1) **Depth access (MBP-10):**  
The python micro signal expects depth levels for OBI. If your feed only has top quote, keep `L_=1`. If you *do* have depth, implement the pseudo `FillDepthArrays(...)` in `OnDepth()` and call `UpdateDepthSizes()`. fileciteturn1file3L22-L37  

2) **Timestamp math:**  
The `Utilities::ToMilliseconds(now_ts - last_ts)` pattern is intentionally “pseudo-ish”. StrategyStudio timestamp utilities vary; swap in the correct helper for your environment.

3) **Fill and PnL accounting:**  
The python execution engine tracks `avg_price`, entry time, and realized PnL precisely. In StrategyStudio you’ll want to read fills (FillInfo / order update fields) and update `LocalPositionState.avg_price` on fills, not mid-price initialization.

4) **Venue and order type:**  
`MARKET_CENTER_ID_NASDAQ` is a placeholder. Use whatever your sim/venue expects. If you want true market orders (if supported), you can switch `ORDER_TYPE_LIMIT` to `ORDER_TYPE_MARKET` and drop the slippage offsets.

---

## 5) Sanity check: this still matches the original strategy

- Macro = (MA crossover + Bollinger regime + trend filter), smoothed. fileciteturn1file2L52-L58  
- Micro = (OBI + microprice deviation) * confidence. fileciteturn1file3L15-L73  
- Blend → target pos → trade-to-target; check exits first. fileciteturn1file0L11-L19  

If you want, I can also produce a “minimal” version that drops Bollinger + trend filter (pure EMA + micro) for easier debugging and then add pieces back one at a time.
