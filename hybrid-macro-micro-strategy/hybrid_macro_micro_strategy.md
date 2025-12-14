# Hybrid Macro–Micro HFT Strategy (OHLCV + Order Book)

This document summarizes the **trading strategy logic** implemented in the provided Python system, focusing only on the signal construction, position targeting, and risk/exit rules. The goal is to make the strategy easy to re-implement in another language or trading platform.

---

## 1) High-level idea

The strategy produces a **continuous directional bias** in `[-1, +1]` by blending:

- **Macro signal (slow)** from OHLCV bars (trend + mean-reversion components), and
- **Micro signal (fast)** from the limit order book (order book imbalance + microprice)

That bias is converted into a **target position** (integer contracts), and the strategy trades to move the current position toward the target subject to risk constraints.

---

## 2) Inputs required

### 2.1 Macro inputs (OHLCV bars)
You need a stream of OHLCV bars (e.g., **1-minute** bars). Only `close` is required for the current formulation.

### 2.2 Micro inputs (order book / BBO)
At each evaluation time you need (at minimum):

- Best bid price `Pb`, best ask price `Pa`
- Best bid size `Qb`, best ask size `Qa`
- Optionally depth up to 10 levels: `{bid_px_l, bid_sz_l, ask_px_l, ask_sz_l}` for `l=0..9`

Derived values:
- `mid = (Pb + Pa) / 2`
- `spread = Pa - Pb`

---

## 3) Signal pipeline overview

1. Compute **macro weight** `W_macro(t) ∈ [-1, +1]` on OHLCV-bar boundaries and smooth it.
2. Compute **micro alpha** `A_micro(t) ∈ [-1, +1]` on every evaluation tick.
3. Blend into **combined bias**:

`bias(t) = clip( α * W_macro(t) + (1-α) * A_micro(t), -1, +1 )`

4. Convert to **target position**:

`target_pos(t) = round( bias(t) * max_position )`

5. Trade toward `target_pos` with a 1-contract deadzone.

---

## 4) Macro signal: `W_macro(t)` from OHLCV

The macro signal is a weighted blend of three sub-scores:

- `w_ma`: EMA crossover score (momentum / trend-following)
- `w_boll`: Bollinger-style score (mean reversion vs breakout regime)
- `w_trend`: Trend filter score (dampens signals against the prevailing slow trend)

### 4.1 EMA crossover score `w_ma`
Compute a **fast EMA** and **slow EMA** over closes.

- `EMA_fast = EMA(close, span=ma_fast_period)`
- `EMA_slow = EMA(close, span=ma_slow_period)`
- `ma_diff = EMA_fast - EMA_slow`
- `recent_vol = std(last 20 closes)`

Score:
- If `recent_vol` is ~0: `w_ma = +0.3` if `ma_diff > 0` else `-0.3`
- Else:

`w_ma = clip( 0.5 * (ma_diff / (recent_vol + eps)), -0.6, +0.6 )`

### 4.2 Bollinger-style score `w_boll`
Compute Bollinger bands over closes:

- `SMA = mean(close, window=bollinger_period)`
- `STD = std(close, window=bollinger_period)`
- `upper = SMA + k * STD`
- `lower = SMA - k * STD`
- `band_width = upper - lower`

Normalize location inside the band:
- `dist_from_lower = (price - lower)/band_width`
- `dist_from_upper = (price - upper)/band_width`

Regime test (as implemented):
- `recent_vol = std(last 20 closes)`
- `vol_percentile = recent_vol / (STD + eps)`
- If `vol_percentile < 0.8` → **mean reversion** regime
- Else → **breakout** regime

Score rules (mean reversion):
- If `dist_from_lower < 0.2`: go long (stronger as closer/below lower band)
- If `dist_from_upper > 0.8`: go short (stronger as closer/above upper band)
- Else: 0

Score rules (breakout):
- If `dist_from_upper > 0.8`: mild long bias
- If `dist_from_lower < 0.2`: mild short bias
- Else: 0

Finally: `w_boll = clip(w_boll, -0.5, +0.5)`

### 4.3 Trend filter `w_trend`
Compute slow EMA (same period as `ma_slow_period`) and estimate its slope:

- `slow_ema = EMA(close, span=ma_slow_period)`
- `ema_slope = (slow_ema[t] - slow_ema[t-1]) / (slow_ema[t-1] + eps)`
- `recent_vol = std(last 20 closes)`
- `normalized_slope = ema_slope / (recent_vol + eps)`
- `w_trend = clip( 10 * normalized_slope, -0.2, +0.2 )`

### 4.4 Combine macro subscores + anti-trend damping + smoothing

Raw macro weight:

`W_raw = w_ma_weight*w_ma + w_boll_weight*w_boll + w_trend_weight*w_trend`

Then dampen signals against trend:
- If `w_trend < 0` and `W_raw > 0`: `W_raw *= 0.7`
- If `w_trend > 0` and `W_raw < 0`: `W_raw *= 0.7`

Clamp: `W_raw = clip(W_raw, -1, +1)`

Smooth with EMA-like recursion:

`W_macro(t) = s*W_raw + (1-s)*W_macro(t-1)`

where `s = macro_smoothing_alpha`.

---

## 5) Micro signal: `A_micro(t)` from the order book

Micro alpha uses:
- **Order Book Imbalance (OBI)** across depth levels, and
- **Microprice deviation** at the top of book,
then scales by a **confidence** term from spread and depth.

### 5.1 Weighted Order Book Imbalance (OBI)
For `L` levels (up to 10):

- Choose weights `w_l`:
  - Exponential: `w_l = exp(-obi_decay_rate * l)`
  - Linear: `w_l = 1/(l+1)`

Aggregate depth:
- `BidVol = Σ_l w_l * bid_sz_l`
- `AskVol = Σ_l w_l * ask_sz_l`

Imbalance:

`OBI = (BidVol - AskVol) / (BidVol + AskVol)`

Clamp to `[-1, +1]`.

### 5.2 Microprice deviation
Given best bid/ask and sizes:

- `mid = (Pb + Pa)/2`
- `microprice = (Pa*Qb + Pb*Qa)/(Qb + Qa)`
- `spread = Pa - Pb`

Normalize deviation:

`micro_dev = (microprice - mid) / spread`

If `spread == 0`, treat as 0.

### 5.3 Confidence term
Confidence `c ∈ [0, 1]` is derived from spread and top-of-book depth:

1) Spread gate:
- If `spread > max_spread_ticks` → `c = 0` (don’t trust micro signal)

2) Low depth penalty:
- If `min(Qb, Qa) < min_book_depth` → `c = 0.3`

3) Otherwise:
- `spread_conf = 1 - min(spread/max_spread_ticks, 1)`
- `depth_conf  = min(min(Qb, Qa)/10, 1)`
- `c = 0.6*spread_conf + 0.4*depth_conf`

### 5.4 Combine into micro alpha
Raw micro alpha:

`A_raw = 0.6*OBI + 0.4*micro_dev`

Apply confidence:

`A_micro = clip( A_raw * c, -1, +1 )`

---

## 6) Combined bias and target position

Blend macro and micro:

`bias = clip( α * W_macro + (1-α) * A_micro, -1, +1 )`

Convert to target integer position:

`target_pos = round( bias * max_position )`

**Deadzone**: only trade if `abs(target_pos - current_pos) >= 1`.

---

## 7) Entry/exit & position management rules

This strategy is **always-on** during the session and continuously re-targets position.

### 7.1 Re-targeting trades
At each evaluation tick:

- Compute `delta = target_pos - current_pos`
- If `delta > 0`: buy `delta` (or the max allowed by position limit)
- If `delta < 0`: sell `abs(delta)` (or the max allowed by position limit)

### 7.2 Exits (risk and holding constraints)
If currently in a position, exit (flatten) when **any** triggers:

- **Stop loss**: `pnl_per_contract <= -sl_ticks`
- **Take profit**: `pnl_per_contract >= tp_ticks`
- **Max hold time**: holding duration `> max_hold_time_ms`

This exit happens *before* the strategy tries to re-target the position on the same tick.

---

## 8) Execution assumptions (platform-agnostic)
The original implementation models “marketable” behavior via book prices + slippage:

- If buying: fill at `ask + slippage_ticks*tick_size`
- If selling: fill at `bid - slippage_ticks*tick_size`

In a real trading platform, you can implement this using:
- Market orders, or
- Aggressive limits (crossing the spread), and incorporate expected slippage/fees in evaluation.

---

## 9) Reference pseudocode (portable)

```pseudo
state:
  W_macro = 0
  latest_bar_time = None

on_each_eval_tick(t, book):
  # --- 1) Update macro at bar closes ---
  bar = last_completed_bar_at_or_before(t)
  if bar exists and bar.time > latest_bar_time:
    W_raw = macro_signal_from_history(bar_history_up_to(t))
    W_macro = s*W_raw + (1-s)*W_macro
    latest_bar_time = bar.time

  # --- 2) Micro alpha each tick ---
  OBI = weighted_imbalance(book.depth_levels)
  micro_dev = (microprice(book.bbo) - mid(book.bbo)) / spread(book.bbo)
  c = confidence_from_spread_and_depth(book.bbo)
  A_micro = clip((0.6*OBI + 0.4*micro_dev)*c, -1, +1)

  # --- 3) Combine & target position ---
  bias = clip(alpha*W_macro + (1-alpha)*A_micro, -1, +1)
  target_pos = round(bias * max_position)

  # --- 4) Check exits first ---
  if position != 0:
    if stop_loss_hit() or take_profit_hit() or hold_time_exceeded():
      flatten_position()

  # --- 5) Re-target ---
  delta = target_pos - position
  if abs(delta) >= 1:
    send_order_to_change_position_by(delta)  # obey position limits
```

---

## 10) Parameter checklist (what you need to expose in other platforms)

**Macro**
- `ma_fast_period`, `ma_slow_period`
- `bollinger_period`, `bollinger_std`
- `weight_ma`, `weight_bollinger`, `weight_trend`
- `macro_smoothing_alpha`

**Micro**
- `obi_decay_type`, `obi_decay_rate`
- `max_spread_ticks`, `min_book_depth`

**Blend + position**
- `macro_weight_alpha`
- `max_position`

**Risk**
- `tp_ticks`, `sl_ticks`
- `max_hold_time_ms`
- `daily_loss_limit` (kill switch)

**Execution**
- `slippage_ticks`, `tick_size`
- `trade_cooldown_ms`, `max_trades_per_minute`

---

## Notes on intended behavior

- Macro pushes the strategy toward **trend-following** (EMA crossover) while also having a **Bollinger-based** mean-reversion component depending on volatility regime.
- Micro pushes the strategy toward the side with **resting liquidity imbalance** and a **microprice shift** (expected short-term direction), scaled down in wide-spread / thin-book conditions.
- Target position sizing is linear in bias, with hard caps and simple exits.
