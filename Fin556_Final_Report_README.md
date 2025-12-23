# Hybrid Macro–Micro Trading Strategy

## Overview

This repository documents a hybrid systematic trading strategy that combines:

- **Macro-level directional bias** derived from low-frequency OHLCV data  
- **Microstructure-based execution signals** derived from order book data  

The core design principle is to **use macro signals to determine directional bias and noise filtering**, while relying on **microstructure signals for execution efficiency**, allowing the strategy to seek returns primarily through **speed and positioning discipline rather than leverage**.

The system continuously trades toward a dynamically updated **target position**, adjusting exposure as market conditions evolve.

---

## Strategy Architecture

The strategy operates in five conceptual stages:

1. **Macro Signal Construction**  
   Produces a slow-moving directional signal  
   \[
   W_{\text{macro}}(t) \in [-1, +1]
   \]

2. **Micro Signal Construction**  
   Produces a fast execution signal from order book dynamics  
   \[
   A_{\text{micro}}(t) \in [-1, +1]
   \]

3. **Bias Blending**  
   Macro and micro signals are linearly blended:
   \[
   \text{bias}(t) = \text{clip}\left(
   \alpha W_{\text{macro}}(t) + (1 - \alpha) A_{\text{micro}}(t),
   -1, +1
   \right)
   \]

4. **Target Position Mapping**  
   \[
   \text{target\_position}(t) = \text{round}(\text{bias}(t) \cdot \text{max\_position})
   \]

5. **Execution with Deadzone**  
   Trades are only sent if the difference between current position and target position is at least one contract, reducing churn.

---

## Data Requirements

### Macro Inputs
- OHLCV bars
- Any reasonable candle timeframe
- Close prices are sufficient

### Micro Inputs
At minimum:
- Best bid and ask (NBBO)
- Queue size at top of book

Optional but beneficial:
- Multiple depth levels for computing weighted order book imbalance

---

## Macro Signal Construction

The macro signal is a weighted blend of three subscores:

### 1. EMA Crossover Momentum
- Fast and slow EMAs computed on closes
- Difference normalized by recent volatility
- Normalization ensures comparability across regimes

### 2. Bollinger Regime Detection
- Bollinger Bands used dynamically:
  - **Low volatility** → mean reversion near bands
  - **High volatility** → breakout behavior
- Regime chosen via short-term realized volatility relative to band width

### 3. Trend Filter
- Estimated slope of slow EMA
- Normalized by volatility
- Used as a penalty, not a dominant signal
- Dampens counter-trend macro signals

### Macro Signal Update
The raw macro signal is smoothed:
\[
W_{\text{macro}}(t) = s W_{\text{raw}} + (1 - s) W_{\text{macro}}(t-1)
\]

Additional logic:
- Counter-trend signals are damped (e.g. multiplied by 0.7)
- Final value is clamped to \([-1, +1]\)

---

## Micro Signal Construction

### Weighted Order Book Imbalance (OBI)

\[
\text{OBI} =
\frac{\text{BidVolume} - \text{AskVolume}}
{\text{BidVolume} + \text{AskVolume}}
\]

- Computed across available depth levels
- Deeper levels receive lower weights

### Microprice Deviation
- Compares microprice to mid-price
- Normalized by spread
- Captures short-term directional pressure

### Confidence Scaling
A confidence scalar \(c \in [0,1]\) reduces micro influence when:
- Spread is wide
- Liquidity is thin

### Final Micro Signal
\[
A_{\text{micro}} =
\text{clip}\left(
(0.6 \cdot \text{OBI} + 0.4 \cdot \text{micro\_deviation}) \cdot c,
-1, +1
\right)
\]

---

## Positioning Logic

- Bias is mapped to a discrete target position
- Strategy trades the delta between current and target position
- A **single-contract deadzone** prevents excessive churn
- Linear sizing is intentional for testability and portability

---

## Risk Management

While in a position, the strategy exits immediately if **any** of the following occur:

1. Stop loss is hit  
2. Take profit is hit  
3. Maximum holding time is exceeded  

Additional safeguards:
- **Daily loss limit kill-switch**
- Exit checks occur **before** any re-targeting on the same evaluation tick

### Slippage Modeling
A simple slippage model is included:
- Buy fills: `ask + slippage_ticks * tick_size`
- Sell fills: `bid - slippage_ticks * tick_size`

This ensures conservative PnL estimation even before accounting for latency.

---

## Design Limitations (Known)

- Linear blending may underperform in extreme (2+ sigma) regimes
- Microstructure signals are asset- and day-dependent
- Aggressive retargeting can increase churn
- Cooldowns conflict with high-frequency execution objectives

Kill-switches are intentionally included to stop trading when conditions fall outside modeled assumptions.

---

## Execution Order (Pseudocode)

```python
state:
    W_macro = 0
    latest_bar_time = None

on_each_eval_tick(t, book):

    # 1) Update macro on completed bars
    bar = last_completed_bar_at_or_before(t)
    if bar exists and bar.time > latest_bar_time:
        W_raw = macro_signal_from_history(bar_history_up_to(t))
        W_macro = s * W_raw + (1 - s) * W_macro
        latest_bar_time = bar.time

    # 2) Compute micro signal
    OBI = weighted_imbalance(book.depth_levels)
    micro_dev = (microprice(book.bbo) - mid(book.bbo)) / spread(book.bbo)
    c = confidence_from_spread_and_depth(book.bbo)
    A_micro = clip((0.6 * OBI + 0.4 * micro_dev) * c, -1, +1)

    # 3) Blend & target
    bias = clip(alpha * W_macro + (1 - alpha) * A_micro, -1, +1)
    target_pos = round(bias * max_position)

    # 4) Exit checks
    if position != 0:
        if stop_loss_hit() or take_profit_hit() or hold_time_exceeded():
            flatten_position()

    # 5) Re-target
    delta = target_pos - position
    if abs(delta) >= 1:
        send_order_to_change_position_by(delta)
