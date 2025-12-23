# FIN556 Final Project  
## Hybrid Macro–Micro Trading Strategy

---

## Introduction and Overview

This strategy can be summarized into two parts: a pricing function and a system that continuously trades from the current price to the target price. We combine a general macro view using a computationally light data-set (OHLCV) for the overall direction of trades, and then a computationally heavy data-set (orderbook) for execution.

The idea is simple: use macro-concepts to determine directional bias and parse noise, use speed instead of straight leverage to achieve returns beyond the risk-free rate.

**Implementation note:**  
This separation is intentional. Macro signals are slow, robust, and regime-aware, while microstructure signals are fast, fragile, and execution-focused. Decoupling them allows independent tuning and failure isolation.

---

## Algorithm Outline

The algorithm is done in steps:

1. The Macro Signal:  
   \[
   W_{\text{macro}}(t) \in [-1, +1]
   \]  
   computed at bar boundaries and smoothed.

2. The Micro Signal:  
   \[
   A_{\text{micro}}(t) \in [-1, +1]
   \]  
   computed at every evaluation tick from the order book.

3. Bias Merge:  
   \[
   \text{bias}(t) =
   \text{clip}\big(
   \alpha W_{\text{macro}}(t)
   + (1 - \alpha) A_{\text{micro}}(t),
   -1, +1
   \big)
   \]

4. Bias to Target Position:  
   \[
   \text{target\_position}(t)
   = \text{round}(\text{bias}(t) \cdot \text{max\_position})
   \]

5. Trade toward the target position with a single-contract deadzone.  
   - Deadzone is used to reduce churn.

**Design rationale:**  
Linear blending and discrete targeting are chosen deliberately for interpretability and debuggability. Nonlinear mappings were avoided to ensure that failures could be traced to specific signal components.

---

## Data Requirements

### Macro Inputs

This is just OHLCV bars in whichever candle timeframe we choose. As this is primarily used for a macro-bias, we do not need anything more granular than close data.

**Commentary:**  
Using closes avoids sensitivity to intrabar noise while remaining robust across assets and data vendors.

---

### Micro Inputs

At the very minimum, we require:
- Best bid and best ask
- Queue size at the top-of-market price

Additionally, the algorithm benefits from deeper orderbook data in order to compute deeper orderbook imbalances (OBI).

**Commentary:**  
Deeper book data improves signal stability but is not strictly required, making the strategy deployable across venues with varying data quality.

---

## Signal Construction

The macro-signal is constructed through a weighted mix of three subscores:

1. EMA crossover momentum  
2. Bollinger mean reversion vs breakout  
3. A trend filter  

Each subscore is normalized and bounded to ensure comparability.

---

### EMA Crossover Momentum

We use fast and slow EMAs computed on closes, then take the difference normalized by recent volatility. This normalization prevents large leverage slips or exaggerated signals during high-volatility regimes.

**Interpretation:**  
Volatility normalization allows the signal strength to reflect *confidence* rather than raw magnitude, which stabilizes behavior across different market regimes.

---

### Bollinger Subscore

After computing Bollinger Bands, the strategy determines whether the current regime is mean-reverting or breakout-based by comparing short-term realized volatility to the Bollinger band sigma.

- Low volatility → mean reversion near bands  
- High volatility → breakout continuation  

**Interpretation:**  
Bollinger signals are regime-dependent. Treating them as universally mean-reverting leads to predictable failure modes, which this approach explicitly avoids.

---

### Trend Filter

The slope of the slow EMA is estimated and normalized by volatility, then constrained within a narrow range. This filter does not dominate the macro signal but penalizes counter-trend setups.

**Design choice:**  
The trend filter acts as a stabilizer rather than a driver, preventing large macro allocations against dominant directional flow.

---

### Macro Signal Combination

The raw macro signal is computed as a weighted sum of the subscores, then smoothed:

\[
W_{\text{macro}}(t)
= s W_{\text{raw}} + (1 - s) W_{\text{macro}}(t - 1)
\]

Additional logic includes:
- Dampening counter-trend signals (e.g. multiplying by 0.7)
- Clamping the result to \([-1, +1]\)

**Interpretation:**  
The macro signal is intentionally slow-moving. Excessively reactive macro signals tend to override microstructure logic and degrade execution quality.

---

## Micro Signal Construction

The micro signal combines:
- Weighted order book imbalance
- Microprice deviation at the touch
- A confidence scalar based on spread and liquidity

---

### Weighted Order Book Imbalance (OBI)

\[
\text{OBI}
=
\frac{\text{BidVolume} - \text{AskVolume}}
{\text{BidVolume} + \text{AskVolume}}
\]

This value is computed across multiple depth levels, with decreasing weights applied to deeper prices.

**Interpretation:**  
Liquidity further from the touch matters less but still conveys directional pressure, especially during transitions.

---

### Microprice Deviation

The microprice deviation is computed using the NBBO compared to the mid-price, normalized by the spread.

**Interpretation:**  
This term captures short-term price pressure while remaining scale-invariant to spread changes.

---

### Confidence Term

A confidence scalar \( c \in [0,1] \) reduces micro influence when:
- Spread widens
- Depth thins
- Order book becomes unstable

Microstructure signals are strongest when liquidity is tight and deep.

---

### Final Micro Alpha

\[
A_{\text{micro}}
=
\text{clip}\big(
(0.6 \cdot \text{OBI}
+ 0.4 \cdot \text{micro\_deviation}) \cdot c,
-1, +1
\big)
\]

**Design rationale:**  
The weighting reflects empirical intuition: imbalance dominates direction, while microprice refines timing.

---

## Bias Blending and Execution

The macro and micro signals are linearly blended using parameter \( \alpha \). The resulting bias is converted into a discrete target position via `max_position`.

The strategy continuously rebalances by trading toward the target, subject to a one-contract deadzone.

**Portability note:**  
The simplicity of the sizing and blending logic makes the strategy easy to test, audit, and port across backtesting and live environments.

---

## Risk Management

While the algorithm is in a position, it is flattened if any of the following occur:

1. Stop loss  
2. Take profit  
3. Maximum holding time  

Exit checks occur before retargeting on the same evaluation tick.

Additionally, a daily loss limit acts as a global kill-switch.

---

### Slippage Modeling

A basic slippage model is included:
- Buy fills at `ask + slippage_ticks * tick_size`
- Sell fills at `bid - slippage_ticks * tick_size`

This ensures conservative performance estimates before accounting for latency.

---

## Design Limitations

Known limitations include:
- Linear blending may underperform in extreme regimes
- Micro inputs are asset- and day-dependent
- Aggressive retargeting may cause overtrading

These limitations are acknowledged intentionally, with kill-switches used to halt trading under uncertainty rather than force adaptation.

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
        W_macro = s*W_raw + (1-s)*W_macro
        latest_bar_time = bar.time

    # 2) Micro alpha
    OBI = weighted_imbalance(book.depth_levels)
    micro_dev = (microprice(book.bbo) - mid(book.bbo)) / spread(book.bbo)
    c = confidence_from_spread_and_depth(book.bbo)
    A_micro = clip((0.6*OBI + 0.4*micro_dev)*c, -1, +1)

    # 3) Combine & target
    bias = clip(alpha*W_macro + (1-alpha)*A_micro, -1, +1)
    target_pos = round(bias * max_position)

    # 4) Check exits
    if position != 0:
        if stop_loss_hit() or take_profit_hit() or hold_time_exceeded():
            flatten_position()

    # 5) Re-target
    delta = target_pos - position
    if abs(delta) >= 1:
        send_order_to_change_position_by(delta)
