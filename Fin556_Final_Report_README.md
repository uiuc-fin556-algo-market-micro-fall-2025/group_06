# FIN556 Final Project  
## Hybrid Macro–Micro Trading Strategy

---

## Introduction and Overview

This strategy can be summarized into two parts: a pricing function and a system that continuously trades from the current price to the target price. We combine a general macro view using a computationally light data-set (OHLCV) for the overall direction of trades, and then a computationally heavy data-set (order book) for execution.

The idea is simple: use macro concepts to determine directional bias and parse noise, and use speed instead of straight leverage to achieve returns beyond the risk-free rate.

This separation allows the strategy to remain robust at the signal level while relying on microstructure only where it adds value: execution.

---

## Algorithm Steps

The algorithm is executed in the following stages:

1. **The Macro Signal**

   $$
   W_{\text{macro}}(t) \in [-1, +1]
   $$

   Computed at bar boundaries and smoothed over time.

2. **The Micro Signal**

   $$
   A_{\text{micro}}(t) \in [-1, +1]
   $$

   Computed at every evaluation tick using order book data.

3. **Bias Merge**

   $$
   \text{bias}(t)
   =
   \text{clip}\!\left(
   \alpha\,W_{\text{macro}}(t)
   +
   (1-\alpha)\,A_{\text{micro}}(t),
   -1,\,+1
   \right)
   $$

4. **Bias to Target Position**

   $$
   \text{target\_position}(t)
   =
   \text{round}\!\left(
   \text{bias}(t)\cdot\text{max\_position}
   \right)
   $$

5. **Execution**

   Trade toward the target position using a single-contract deadzone.

   - The deadzone is used to reduce churn.

The linear structure of the signal blending and sizing is intentional to ensure interpretability and ease of testing.

---

## Data Requirements

### Macro Inputs

This consists solely of OHLCV bars at a chosen candle timeframe. Since the macro signal is used only to determine directional bias, no higher-frequency data than close prices is required.

This choice reduces noise sensitivity and improves cross-asset robustness.

---

### Micro Inputs

At a minimum, the micro signal requires:
- Best bid and best ask prices
- Queue size at the top of book

The strategy benefits from deeper order book data in order to compute weighted order book imbalance across multiple depth levels.

Deeper data improves signal quality but is not strictly required, allowing deployment across venues with varying data availability.

---

## Signal Construction

The macro signal is constructed as a weighted mix of three subscores:

1. EMA crossover momentum  
2. Bollinger mean reversion versus breakout behavior  
3. A trend filter  

Each subscore is normalized and bounded to ensure stability across regimes.

---

### EMA Crossover Momentum

Fast and slow EMAs are computed on close prices. The difference between them is normalized by recent volatility to prevent exaggerated signals during high-volatility regimes.

Normalizing by volatility allows the signal strength to reflect confidence rather than magnitude.

---

### Bollinger Subscore

After computing Bollinger Bands, the strategy dynamically chooses between mean-reversion and breakout behavior using a short-term ratio of realized volatility to band width.

- Low volatility favors mean reversion near the bands  
- High volatility favors breakout continuation  

This avoids the common failure mode of treating Bollinger signals as universally mean-reverting.

---

### Trend Filter

The slope of the slow EMA is estimated and normalized by volatility, then constrained to a narrow range. The trend filter does not dominate the macro signal but penalizes signals that strongly conflict with the prevailing trend.

This acts as a stabilizing force rather than a primary driver.

---

### Macro Signal Combination

The raw macro signal is computed as a weighted sum of the subscores and then smoothed:

$$
W_{\text{macro}}(t)
=
s\,W_{\text{raw}}
+
(1-s)\,W_{\text{macro}}(t-1)
$$

Counter-trend signals are dampened before smoothing, and the final macro value is clamped to the range $[-1, +1]$.

The macro signal is intentionally slow-moving to avoid overriding microstructure logic during short-term fluctuations.

---

## Micro Signal Construction

The micro signal combines:
- Weighted order book imbalance
- Microprice deviation at the touch
- A confidence term based on spread and depth

---

### Weighted Order Book Imbalance (OBI)

$$
\text{OBI}
=
\frac{\text{BidVolume} - \text{AskVolume}}
{\text{BidVolume} + \text{AskVolume}}
$$

This value is computed across available depth levels, with decreasing weights assigned to liquidity further from the touch.

Liquidity farther from the best prices contributes less but still conveys directional pressure.

---

### Microprice Deviation

The microprice deviation compares the microprice to the mid-price and normalizes the result by the spread.

This captures short-term price pressure while remaining scale-invariant to spread changes.

---

### Confidence Term

A confidence scalar $c \in [0,1]$ reduces micro influence when:
- The spread widens
- The order book becomes thin
- Liquidity quality deteriorates

Microstructure signals are most reliable when the book is tight and liquid.

---

### Final Micro Alpha

$$
A_{\text{micro}}
=
\text{clip}\!\left(
(0.6\,\text{OBI}
+
0.4\,\text{micro\_deviation})\cdot c,
-1,\,+1
\right)
$$

The weighting reflects the dominance of imbalance for direction and microprice for timing.

---

## Bias Blending and Positioning

The macro and micro signals are blended linearly using parameter $\alpha$. The resulting bias is mapped to a discrete target position using `max_position`.

The strategy continuously trades toward this target position, subject to a single-contract deadzone.

This simplicity improves auditability and portability across backtesting and live environments.

---

## Risk Management

While the algorithm is in a position, the position is flattened if any of the following conditions are met:

1. Stop loss  
2. Take profit  
3. Maximum holding time  

Exit checks occur before any re-targeting on the same evaluation tick.

A daily loss limit is also included as a global kill-switch.

---

### Slippage Modeling

A basic slippage model is applied:
- Buy fills occur at  
  $$
  \text{ask} + \text{slippage\_ticks}\cdot\text{tick\_size}
  $$
- Sell fills occur at  
  $$
  \text{bid} - \text{slippage\_ticks}\cdot\text{tick\_size}
  $$

This ensures conservative performance estimates prior to accounting for latency.

---

## Design Limitations

Several limitations are acknowledged:

- Linear blending may underperform in extreme regimes  
- Microstructure signals are asset- and day-dependent  
- Aggressive retargeting may cause overtrading  

Kill-switches are used to halt trading when conditions fall outside modeled assumptions rather than forcing adaptation.

---

## Execution Order (Pseudocode)

```python
state:
    W_macro = 0
    latest_bar_time = None

on_each_eval_tick(t, book):

    # 1) Update macro at bar closes
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
