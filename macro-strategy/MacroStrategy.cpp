#ifdef _WIN32
#include "stdafx.h"
#endif

#include "MacroStrategy.h"

#include "AllEventMsg.h"
#include "ExecutionTypes.h"
#include <Utilities/utils.h>

#include <algorithm>
#include <cstring>
#include <iostream>

using namespace RCM::StrategyStudio;
using namespace RCM::StrategyStudio::MarketModels;
using namespace RCM::StrategyStudio::Utilities;

MacroStrategy::MacroStrategy(StrategyID strategyID,
                                                   const std::string &strategyName,
                                                   const std::string &groupName)
    : Strategy(strategyID, strategyName, groupName),
      state_(),
      debug_(false),
      bar_interval_sec_(60),
      max_position_(100),
      ma_fast_period_(9),
      ma_slow_period_(30),
      bollinger_period_(20),
      bollinger_k_(2.0),
      w_ma_weight_(0.50),
      w_boll_weight_(0.30),
      w_trend_weight_(0.20),
      macro_smoothing_alpha_(0.20),
      tp_ticks_(20),
      sl_ticks_(10),
      max_hold_time_ms_(300000), // 5 minutes
      daily_loss_limit_(1000.0),
      contract_multiplier_(1.0),
      tick_size_fallback_(0.01),
      use_market_orders_(true) {
}

MacroStrategy::~MacroStrategy() {}

void MacroStrategy::OnResetStrategyState() {
    state_.clear();
}

void MacroStrategy::DefineStrategyParams() {
    params().CreateParam(CreateStrategyParamArgs("debug", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_BOOL, debug_));

    params().CreateParam(CreateStrategyParamArgs("bar_interval_sec", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, bar_interval_sec_));
    params().CreateParam(CreateStrategyParamArgs("max_position", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, max_position_));

    params().CreateParam(CreateStrategyParamArgs("ma_fast_period", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, ma_fast_period_));
    params().CreateParam(CreateStrategyParamArgs("ma_slow_period", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, ma_slow_period_));
    params().CreateParam(CreateStrategyParamArgs("bollinger_period", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, bollinger_period_));
    params().CreateParam(CreateStrategyParamArgs("bollinger_k", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, bollinger_k_));

    params().CreateParam(CreateStrategyParamArgs("w_ma_weight", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, w_ma_weight_));
    params().CreateParam(CreateStrategyParamArgs("w_boll_weight", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, w_boll_weight_));
    params().CreateParam(CreateStrategyParamArgs("w_trend_weight", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, w_trend_weight_));
    params().CreateParam(CreateStrategyParamArgs("macro_smoothing_alpha", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, macro_smoothing_alpha_));

    params().CreateParam(CreateStrategyParamArgs("tp_ticks", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, tp_ticks_));
    params().CreateParam(CreateStrategyParamArgs("sl_ticks", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, sl_ticks_));
    params().CreateParam(CreateStrategyParamArgs("max_hold_time_ms", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, max_hold_time_ms_));
    params().CreateParam(CreateStrategyParamArgs("daily_loss_limit", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_DOUBLE, daily_loss_limit_));

    params().CreateParam(CreateStrategyParamArgs("contract_multiplier", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, contract_multiplier_));
    params().CreateParam(CreateStrategyParamArgs("tick_size_fallback", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, tick_size_fallback_));

    params().CreateParam(CreateStrategyParamArgs("use_market_orders", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_BOOL, use_market_orders_));
}

void MacroStrategy::DefineStrategyCommands() {
    // Keep it minimal (no repricing logic; we don't subscribe to quotes under the "bars only" constraint).
    commands().AddCommand(StrategyCommand(1, "Cancel All Orders"));
}

void MacroStrategy::RegisterForStrategyEvents(StrategyEventRegister *eventRegister, DateType currDate) {
    // Constraint: only RegisterForBars.
    for (SymbolSetConstIter it = symbols_begin(); it != symbols_end(); ++it) {
        eventRegister->RegisterForBars(*it, BAR_TYPE_TIME, bar_interval_sec_);
    }
}

double MacroStrategy::ComputeMacroWeight(InstrumentState &st, double close) {
    const double eps = 1e-9;

    // Maintain rolling windows.
    if (st.last20.Size() == 0)
        st.last20.Reset(20);
    if (st.boll.Size() == 0)
        st.boll.Reset(static_cast<size_t>(std::max(2, bollinger_period_)));

    st.last20.Push(close);
    st.boll.Push(close);

    const double recent_vol = st.last20.Std();

    // --- EMA fast/slow ---
    const double a_fast = 2.0 / (static_cast<double>(std::max(1, ma_fast_period_)) + 1.0);
    const double a_slow = 2.0 / (static_cast<double>(std::max(1, ma_slow_period_)) + 1.0);

    if (!st.ema_init) {
        st.ema_fast = close;
        st.ema_slow = close;
        st.prev_ema_slow = close;
        st.ema_init = true;
    } else {
        st.ema_fast = a_fast * close + (1.0 - a_fast) * st.ema_fast;
        st.prev_ema_slow = st.ema_slow;
        st.ema_slow = a_slow * close + (1.0 - a_slow) * st.ema_slow;
    }

    const double ma_diff = st.ema_fast - st.ema_slow;

    // 4.1 EMA crossover score (w_ma).
    double w_ma = 0.0;
    if (recent_vol < eps) {
        w_ma = (ma_diff > 0.0) ? 0.3 : -0.3;
    } else {
        w_ma = Clip(0.5 * (ma_diff / (recent_vol + eps)), -0.6, 0.6);
    }

    // 4.2 Bollinger-style score (w_boll).
    double w_boll = 0.0;
    if (st.boll.Full()) {
        const double sma = st.boll.Mean();
        const double sd = st.boll.Std();
        const double upper = sma + bollinger_k_ * sd;
        const double lower = sma - bollinger_k_ * sd;
        const double bw = upper - lower;

        if (bw > eps) {
            // Position in band: <0 below lower, >1 above upper.
            const double pos = (close - lower) / bw;

            // Regime: vol_percentile = recent_vol / STD.
            const double vol_percentile = recent_vol / (sd + eps);
            const bool mean_reversion = (vol_percentile < 0.8);

            if (mean_reversion) {
                if (pos < 0.2) {
                    const double strength = Clip((0.2 - pos) / 0.2, 0.0, 1.0);
                    w_boll = +0.5 * strength;
                } else if (pos > 0.8) {
                    const double strength = Clip((pos - 0.8) / 0.2, 0.0, 1.0);
                    w_boll = -0.5 * strength;
                }
            } else { // breakout
                if (pos > 0.8) {
                    const double strength = Clip((pos - 0.8) / 0.2, 0.0, 1.0);
                    w_boll = +0.2 * strength;
                } else if (pos < 0.2) {
                    const double strength = Clip((0.2 - pos) / 0.2, 0.0, 1.0);
                    w_boll = -0.2 * strength;
                }
            }
        }
    }
    w_boll = Clip(w_boll, -0.5, 0.5);

    // 4.3 Trend filter score (w_trend).
    double w_trend = 0.0;
    if (st.ema_init) {
        const double ema_slope = (st.ema_slow - st.prev_ema_slow) / (std::fabs(st.prev_ema_slow) + eps);
        const double normalized_slope = ema_slope / (recent_vol + eps);
        w_trend = Clip(10.0 * normalized_slope, -0.2, 0.2);
    }

    // 4.4 Combine + anti-trend damping + smoothing.
    double w_raw = w_ma_weight_ * w_ma + w_boll_weight_ * w_boll + w_trend_weight_ * w_trend;

    if (w_trend < 0.0 && w_raw > 0.0)
        w_raw *= 0.7;
    if (w_trend > 0.0 && w_raw < 0.0)
        w_raw *= 0.7;

    w_raw = Clip(w_raw, -1.0, 1.0);

    const double s = Clip(macro_smoothing_alpha_, 0.0, 1.0);
    st.w_macro = s * w_raw + (1.0 - s) * st.w_macro;

    return st.w_macro;
}

bool MacroStrategy::ShouldFlatten(const Instrument *instrument,
                                             InstrumentState &st,
                                             int current_pos,
                                             double close) const {
    if (current_pos == 0)
        return false;

    // Bar-count-based holding time approximation (no timestamps under bars-only constraint).
    const int bars_limit = (bar_interval_sec_ > 0)
                               ? std::max(1, (max_hold_time_ms_ + (bar_interval_sec_ * 1000 - 1)) / (bar_interval_sec_ * 1000))
                               : std::max(1, max_hold_time_ms_ / 1000);

    // Tick size fallback: if your Strategy Studio build exposes instrument tick size, set tick_size_fallback_ to 0 and use that.
    const double tick = (tick_size_fallback_ > 0.0) ? tick_size_fallback_ : 0.01;

    // PnL per contract in ticks (approx, using bar close vs entry).
    const double signed_price_move = (close - st.entry_price) * ((current_pos > 0) ? 1.0 : -1.0);
    const double pnl_ticks = signed_price_move / tick;

    if (pnl_ticks <= -static_cast<double>(sl_ticks_))
        return true;
    if (pnl_ticks >= static_cast<double>(tp_ticks_))
        return true;
    if (st.bars_in_pos >= bars_limit)
        return true;

    // Daily loss limit (very rough): realized + current unrealized.
    const double unrealized = (close - st.entry_price) * static_cast<double>(current_pos) * contract_multiplier_;
    const double total_pnl = st.realized_pnl + unrealized;

    if (total_pnl <= -std::fabs(daily_loss_limit_)) {
        return true;
    }

    return false;
}

void MacroStrategy::OnBar(const BarEventMsg &msg) {
    const Instrument *inst = &msg.instrument();
    const double close = msg.bar().close();

    // Get/create state.
    StateIter it = state_.find(inst);
    if (it == state_.end()) {
        InstrumentState st;
        st.last20.Reset(20);
        st.boll.Reset(static_cast<size_t>(std::max(2, bollinger_period_)));
        it = state_.insert(std::make_pair(inst, st)).first;
    }
    InstrumentState &st = it->second;

    if (debug_) {
        std::ostringstream oss;
        oss << inst->symbol() << " close=" << close
            << " pos=" << portfolio().position(inst)
            << " w_macro=" << st.w_macro
            << " entry=" << st.entry_price
            << " bars_in_pos=" << st.bars_in_pos;
        logger().LogToClient(LOGLEVEL_DEBUG, oss.str().c_str());
    }

    if (!st.trading_enabled) {
        // Kill-switch already tripped for this instrument; keep cancelling any stray orders.
        trade_actions()->SendCancelAll();
        return;
    }

    // Update "bars in position" counter.
    const int current_pos = portfolio().position(inst);
    if (current_pos != 0) {
        st.bars_in_pos += 1;
    } else {
        st.bars_in_pos = 0;
        st.entry_price = close; // reset reference
    }

    // --- Compute macro bias (micro omitted by design) ---
    const double w_macro = ComputeMacroWeight(st, close);
    const double bias = Clip(w_macro, -1.0, 1.0);

    // Target position sizing: target_pos = round(bias * max_position).
    const int target_pos = RoundToInt(bias * static_cast<double>(std::max(0, max_position_)));

    // --- Risk exits first (flatten) ---
    if (ShouldFlatten(inst, st, current_pos, close)) {
        // If daily loss limit tripped, disable trading for this instrument after flatten.
        const double unrealized = (close - st.entry_price) * static_cast<double>(current_pos) * contract_multiplier_;
        const double total_pnl = st.realized_pnl + unrealized;
        if (total_pnl <= -std::fabs(daily_loss_limit_)) {
            st.trading_enabled = false;
        }

        AdjustPortfolio(inst, 0, close);
        return;
    }

    // --- Re-target with a 1-contract deadzone.
    if (std::abs(target_pos - current_pos) >= 1) {
        AdjustPortfolio(inst, target_pos, close);

        // Bar-close fill approximation:
        // - If we were flat and target becomes non-zero, set entry reference.
        // - If we flip direction, reset entry.
        const int new_pos_ref = target_pos;
        if (current_pos == 0 && new_pos_ref != 0) {
            st.entry_price = close;
            st.bars_in_pos = 0;
        } else if ((current_pos > 0 && new_pos_ref < 0) || (current_pos < 0 && new_pos_ref > 0)) {
            // Book "realized" on the old side at bar close.
            st.realized_pnl += (close - st.entry_price) * static_cast<double>(current_pos) * contract_multiplier_;
            st.entry_price = close;
            st.bars_in_pos = 0;
        }
    }
}

void MacroStrategy::AdjustPortfolio(const Instrument *instrument, int desired_position, double /*ref_price*/) {
    const int current_pos = portfolio().position(instrument);
    const int trade_size = desired_position - current_pos;

    if (trade_size == 0)
        return;

    // If there are working orders, cancel them before switching direction.
    if (orders().num_working_orders(instrument) > 0) {
        trade_actions()->SendCancelAll(instrument);
    }

    SendMarketOrder(instrument, trade_size);
}

void MacroStrategy::SendMarketOrder(const Instrument *instrument, int trade_size) {
    if (trade_size == 0)
        return;

    const int qty = std::abs(trade_size);
    const OrderSide side = (trade_size > 0) ? ORDER_SIDE_BUY : ORDER_SIDE_SELL;

    // "Taker" behavior: prefer market orders (no quote dependency).
    const OrderType type = use_market_orders_ ? ORDER_TYPE_MARKET : ORDER_TYPE_LIMIT;

    // For ORDER_TYPE_LIMIT (if you disable market orders), we have no quote. Use 0 price; your simulator may reject it.
    const double price = 0.0;

    OrderParams params(*instrument,
                       qty,
                       price,
                       MARKET_CENTER_ID_NASDAQ,
                       side,
                       ORDER_TIF_DAY,
                       type);

    trade_actions()->SendNewOrder(params);
}

void MacroStrategy::OnStrategyCommand(const StrategyCommandEventMsg &msg) {
    switch (msg.command_id()) {
    case 1:
        trade_actions()->SendCancelAll();
        break;
    default:
        logger().LogToClient(LOGLEVEL_DEBUG, "Unknown strategy command received");
        break;
    }
}

void MacroStrategy::OnParamChanged(StrategyParam &param) {
    // Keep this exhaustive so runtime changes take effect.
    if (param.param_name() == "debug") {
        if (!param.Get(&debug_))
            throw StrategyStudioException("Could not get debug");
    } else if (param.param_name() == "bar_interval_sec") {
        if (!param.Get(&bar_interval_sec_))
            throw StrategyStudioException("Could not get bar_interval_sec");
    } else if (param.param_name() == "max_position") {
        if (!param.Get(&max_position_))
            throw StrategyStudioException("Could not get max_position");
    } else if (param.param_name() == "ma_fast_period") {
        if (!param.Get(&ma_fast_period_))
            throw StrategyStudioException("Could not get ma_fast_period");
    } else if (param.param_name() == "ma_slow_period") {
        if (!param.Get(&ma_slow_period_))
            throw StrategyStudioException("Could not get ma_slow_period");
    } else if (param.param_name() == "bollinger_period") {
        if (!param.Get(&bollinger_period_))
            throw StrategyStudioException("Could not get bollinger_period");
    } else if (param.param_name() == "bollinger_k") {
        if (!param.Get(&bollinger_k_))
            throw StrategyStudioException("Could not get bollinger_k");
    } else if (param.param_name() == "w_ma_weight") {
        if (!param.Get(&w_ma_weight_))
            throw StrategyStudioException("Could not get w_ma_weight");
    } else if (param.param_name() == "w_boll_weight") {
        if (!param.Get(&w_boll_weight_))
            throw StrategyStudioException("Could not get w_boll_weight");
    } else if (param.param_name() == "w_trend_weight") {
        if (!param.Get(&w_trend_weight_))
            throw StrategyStudioException("Could not get w_trend_weight");
    } else if (param.param_name() == "macro_smoothing_alpha") {
        if (!param.Get(&macro_smoothing_alpha_))
            throw StrategyStudioException("Could not get macro_smoothing_alpha");
    } else if (param.param_name() == "tp_ticks") {
        if (!param.Get(&tp_ticks_))
            throw StrategyStudioException("Could not get tp_ticks");
    } else if (param.param_name() == "sl_ticks") {
        if (!param.Get(&sl_ticks_))
            throw StrategyStudioException("Could not get sl_ticks");
    } else if (param.param_name() == "max_hold_time_ms") {
        if (!param.Get(&max_hold_time_ms_))
            throw StrategyStudioException("Could not get max_hold_time_ms");
    } else if (param.param_name() == "daily_loss_limit") {
        if (!param.Get(&daily_loss_limit_))
            throw StrategyStudioException("Could not get daily_loss_limit");
    } else if (param.param_name() == "contract_multiplier") {
        if (!param.Get(&contract_multiplier_))
            throw StrategyStudioException("Could not get contract_multiplier");
    } else if (param.param_name() == "tick_size_fallback") {
        if (!param.Get(&tick_size_fallback_))
            throw StrategyStudioException("Could not get tick_size_fallback");
    } else if (param.param_name() == "use_market_orders") {
        if (!param.Get(&use_market_orders_))
            throw StrategyStudioException("Could not get use_market_orders");
    }
}

// ==== Factory / metadata exports ====

extern "C" {

_STRATEGY_EXPORTS const char *GetType() {
    return "MacroStrategy";
}

_STRATEGY_EXPORTS Strategy *CreateStrategy(const char *strategyType,
                                           unsigned strategyID,
                                           const char *strategyName,
                                           const char *groupName) {
    if (strcmp(strategyType, GetType()) == 0) {
        return new MacroStrategy(strategyID, strategyName, groupName);
    }
    return NULL;
}

_STRATEGY_EXPORTS const char *GetAuthor() {
    return "dlariviere";
}

_STRATEGY_EXPORTS const char *GetAuthorGroup() {
    return "UIUC";
}

_STRATEGY_EXPORTS const char *GetReleaseVersion() {
    return Strategy::release_version();
}

} // extern "C"
