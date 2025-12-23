# FIN556 Final Project  
## Hybrid Macro–Micro Trading Strategy

---

## Introduction and Overview

This strategy can be summarized into two parts: a pricing function and a system that continuously trades from the current price to the target price. We combine a general macro view using a computationally light data set (OHLCV) for determining overall trade direction, and a computationally heavy data set (order book) for execution.

The idea is simple: use macro concepts to determine directional bias and filter noise, and use speed rather than leverage to achieve returns beyond the risk-free rate.

---

## Algorithm Steps

The algorithm is executed in the following stages:

1. **The Macro Signal**  
   W_macro(t) is bounded between −1 and +1.  
   It is computed at bar boundaries and smoothed over time.

2. **The Micro Signal**  
   A_micro(t) is bounded between −1 and +1.  
   It is computed at every evaluation tick using order book data.

3. **Bias Merge**  
   The two signals are linearly combined as follows:  

   bias(t) = clip( alpha · W_macro(t) + (1 − alpha) · A_micro(t),  −1, +1  )

4. **Bias to Target Position**  
   The bias is converted into a discrete target position:  

   target_position(t) = round( bias(t) · max_position )

5. **Execution**  
   The strategy trades toward the target position using a single-contract deadzone.  
   The deadzone is used to reduce execution churn.

---

## Data Requirements

The data required by the strategy consists of macro inputs and micro inputs.

### Macro Inputs

The macro signal uses OHLCV bars at a chosen candle timeframe. Since this signal is only used to determine directional bias, close prices are sufficient and no higher-frequency data is required.

### Micro Inputs

At a minimum, the micro signal requires:
- Best bid and best ask prices
- Queue size at the top of book

The strategy benefits from deeper order book data in order to compute weighted order book imbalance across multiple depth levels, but this is not strictly required.

---

## Signal Construction

The macro signal is constructed through a weighted combination of three subscores:

1. EMA crossover momentum  
2. Bollinger mean reversion versus breakout behavior  
3. A trend filter  

Each subscore is normalized and bounded to ensure comparability across market regimes.

---

### EMA Crossover Momentum

Fast and slow exponential moving averages are computed on close prices. The difference between them is normalized by recent volatility to prevent exaggerated signals during high-volatility regimes.

Normalizing by volatility allows the signal strength to represent confidence rather than raw magnitude.

---

### Bollinger Subscore

After computing Bollinger Bands, the strategy dynamically determines whether the current regime is mean-reverting or breakout-oriented. This decision is made using a short-term ratio of realized volatility to Bollinger band width.

- Low volatility favors mean reversion near the bands  
- High volatility favors breakout continuation  

This avoids the common failure mode of treating Bollinger signals as universally mean-reverting.

---

### Trend Filter

The slope of the slow EMA is estimated and normalized by volatility, then constrained to a narrow range. The trend filter does not dominate the macro signal, but instead penalizes macro signals that strongly conflict with the prevailing trend.

---

### Combining the Macro Signal

The raw macro signal is computed as a weighted sum of the subscores. Counter-trend signals are dampened before smoothing. The macro signal is then smoothed using an exponential update:

W_macro(t) = s · W_raw + (1 − s) · W_macro(t − 1)

The final value is clamped between −1 and +1. The macro signal is intentionally slow-moving to avoid overriding microstructure logic during short-term fluctuations.

---

## Micro Signal Construction

The micro signal combines three components:
- Weighted order book imbalance
- Microprice deviation at the touch
- A confidence term based on spread and depth

---

### Weighted Order Book Imbalance (OBI)

Order book imbalance is computed as:

OBI = (BidVolume − AskVolume) / (BidVolume + AskVolume)

This value is computed across available depth levels, with decreasing weights assigned to liquidity further from the best prices.

---

### Microprice Deviation

The microprice deviation compares the microprice to the mid-price and normalizes the result by the bid-ask spread. This captures short-term price pressure while remaining scale-invariant.

---

### Confidence Term

A confidence scalar c, bounded between 0 and 1, reduces the influence of the micro signal when the order book becomes unreliable. This occurs when spreads widen, liquidity thins, or market conditions deteriorate.

Microstructure signals are most reliable when the order book is tight and liquid.

---

### Final Micro Signal

The final micro signal is computed as:

A_micro(t) = clip( (0.6 · OBI + 0.4 · micro_deviation) · c, −1, +1 )

---

## Bias Blending and Positioning

The macro and micro signals are blended using the parameter alpha. The resulting bias is mapped to a discrete target position using max_position.

The strategy continuously trades toward this target position, subject to a single-contract deadzone.

---

## Risk Management

While the strategy holds a position, the position is flattened if any of the following conditions are met:

1. Stop loss  
2. Take profit  
3. Maximum holding time  

Exit checks occur before any retargeting on the same evaluation tick.

A daily loss limit is also included as a global kill switch.

---

### Slippage Modeling

A basic slippage model is applied:

- Buy fills occur at:  
  ask + slippage_ticks · tick_size

- Sell fills occur at:  
  bid − slippage_ticks · tick_size

This ensures conservative performance estimates before accounting for latency.

---

## Design Limitations

Several limitations are explicitly acknowledged:

- Linear blending may underperform during extreme market regimes  
- Microstructure signals are asset- and day-dependent  
- Aggressive retargeting may cause overtrading  

Kill switches are used to halt trading when conditions fall outside modeled assumptions rather than forcing adaptation.

---

## Portability

The strategy follows a strict update order:

1. Update macro signal at completed bar boundaries  
2. Compute micro signal at every evaluation tick  
3. Blend signals and compute target position  
4. Check exit conditions  
5. Retarget position if necessary  

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

    # 4) Check exits
    if position != 0:
        if stop_loss_hit() or take_profit_hit() or hold_time_exceeded():
            flatten_position()

    # 5) Re-target
    delta = target_pos - position
    if abs(delta) >= 1:
        send_order_to_change_position_by(delta)
