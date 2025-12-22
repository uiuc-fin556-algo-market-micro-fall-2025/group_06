#!/usr/bin/env python3
"""
Complete script to add all trading system cells to the notebook.
Run this to add all remaining cells at once.
"""
import json

def add_all_cells():
    with open('data.ipynb', 'r') as f:
        nb = json.load(f)
    
    # All remaining cells to add
    new_cells = []
    
    # Micro Signal Generator
    new_cells.append({
        "cell_type": "code",
        "execution_count": None,
        "metadata": {},
        "source": """# Microstructure Signal Generator
class MicroSignalGenerator:
    def __init__(self, config):
        self.config = config
        self.last_obi = None
    
    def compute_order_book_imbalance(self, book_row):
        bid_vol = 0.0
        ask_vol = 0.0
        for level in range(10):
            bid_px_col = f'bid_px_{level}'
            if bid_px_col not in book_row or pd.isna(book_row[bid_px_col]):
                break
            bid_size = book_row.get(f'bid_sz_{level}', 0)
            ask_size = book_row.get(f'ask_sz_{level}', 0)
            if self.config.obi_decay_type == "exponential":
                weight = np.exp(-self.config.obi_decay_rate * level)
            else:
                weight = 1.0 / (level + 1)
            bid_vol += weight * bid_size
            ask_vol += weight * ask_size
        total_vol = bid_vol + ask_vol
        if total_vol == 0:
            return 0.0
        obi = (bid_vol - ask_vol) / total_vol
        return np.clip(obi, -1.0, 1.0)
    
    def compute_microprice(self, book_row):
        bid_px = book_row['bid_px']
        ask_px = book_row['ask_px']
        bid_sz = book_row['bid_sz']
        ask_sz = book_row['ask_sz']
        mid = (bid_px + ask_px) / 2.0
        total_sz = bid_sz + ask_sz
        if total_sz == 0:
            return 0.0
        microprice = (ask_px * bid_sz + bid_px * ask_sz) / total_sz
        spread = ask_px - bid_px
        if spread == 0:
            return 0.0
        deviation = (microprice - mid) / spread
        return deviation
    
    def compute_spread_confidence(self, book_row):
        spread = book_row['spread']
        bid_sz = book_row['bid_sz']
        ask_sz = book_row['ask_sz']
        if spread > self.config.max_spread_ticks:
            return 0.0
        min_depth = min(bid_sz, ask_sz)
        if min_depth < self.config.min_book_depth:
            return 0.3
        spread_confidence = 1.0 - min(spread / self.config.max_spread_ticks, 1.0)
        depth_confidence = min(min_depth / 10.0, 1.0)
        confidence = 0.6 * spread_confidence + 0.4 * depth_confidence
        return confidence
    
    def compute_micro_alpha(self, book_row):
        obi = self.compute_order_book_imbalance(book_row)
        microprice_dev = self.compute_microprice(book_row)
        confidence = self.compute_spread_confidence(book_row)
        raw_alpha = 0.6 * obi + 0.4 * microprice_dev
        alpha = raw_alpha * confidence
        alpha = np.clip(alpha, -1.0, 1.0)
        self.last_obi = obi
        return {'alpha': alpha, 'obi': obi, 'microprice_dev': microprice_dev, 'confidence': confidence}

micro_gen = MicroSignalGenerator(config)
print("Micro signal generator initialized")""".split('\n')
    })
    
    # Execution Engine (simplified - will add full version)
    new_cells.append({
        "cell_type": "code",
        "execution_count": None,
        "metadata": {},
        "source": """# Execution Engine - Part 1: Classes and initialization
@dataclass
class Trade:
    timestamp: datetime
    side: str
    price: float
    size: int
    position_after: int
    pnl: float
    cumulative_pnl: float
    reason: str

@dataclass
class Position:
    size: int
    avg_price: float
    entry_time: Optional[datetime]
    unrealized_pnl: float

class ExecutionEngine:
    def __init__(self, config):
        self.config = config
        self.position = Position(size=0, avg_price=0.0, entry_time=None, unrealized_pnl=0.0)
        self.trades = []
        self.equity_curve = []
        self.cumulative_pnl = 0.0
        self.last_trade_time = None
        self.trade_count = 0
        self.trades_this_minute = 0
        self.current_minute = None
        self.killed = False
    
    def can_trade(self, timestamp):
        if self.killed:
            return False
        if self.cumulative_pnl < self.config.daily_loss_limit:
            self.killed = True
            return False
        if self.last_trade_time is not None:
            time_since_last = (timestamp - self.last_trade_time).total_seconds() * 1000
            if time_since_last < self.config.trade_cooldown_ms:
                return False
        if self.config.max_trades_per_minute is not None:
            minute = timestamp.minute
            if minute != self.current_minute:
                self.current_minute = minute
                self.trades_this_minute = 0
            if self.trades_this_minute >= self.config.max_trades_per_minute:
                return False
        return True
    
    def compute_fill_price(self, side, book_row):
        if side == 'BUY':
            base_price = book_row['ask_px']
            slippage_direction = 1
        else:
            base_price = book_row['bid_px']
            slippage_direction = -1
        slippage = slippage_direction * self.config.slippage_ticks * self.config.tick_size
        return base_price + slippage

execution = ExecutionEngine(config)
print("Execution engine initialized - Part 1")""".split('\n')
    })
    
    # Add execution methods in next cell
    new_cells.append({
        "cell_type": "code",
        "execution_count": None,
        "metadata": {},
        "source": """# Execution Engine - Part 2: Trade execution and position management
# (Continuing ExecutionEngine class methods)

def execute_trade(self, side, size, price, timestamp, reason=""):
    if not self.can_trade(timestamp) or size == 0:
        return None
    fill_price = price
    old_size = self.position.size
    old_avg_price = self.position.avg_price
    if side == 'BUY':
        new_size = old_size + size
        if old_size == 0:
            new_avg_price = fill_price
        elif old_size > 0:
            new_avg_price = (old_size * old_avg_price + size * fill_price) / new_size
        else:
            if abs(size) >= abs(old_size):
                pnl = abs(old_size) * (old_avg_price - fill_price)
                remaining = size - abs(old_size)
                new_avg_price = fill_price if remaining > 0 else 0.0
            else:
                pnl = size * (old_avg_price - fill_price)
                new_avg_price = old_avg_price
                new_size = old_size + size
    else:
        new_size = old_size - size
        if old_size == 0:
            new_avg_price = fill_price
        elif old_size < 0:
            new_avg_price = (abs(old_size) * old_avg_price + size * fill_price) / abs(new_size)
        else:
            if size >= old_size:
                pnl = old_size * (fill_price - old_avg_price)
                remaining = size - old_size
                new_avg_price = fill_price if remaining > 0 else 0.0
            else:
                pnl = size * (fill_price - old_avg_price)
                new_avg_price = old_avg_price
                new_size = old_size - size
    if side == 'BUY' and old_size < 0:
        realized_pnl = abs(old_size) * (old_avg_price - fill_price) if abs(size) >= abs(old_size) else size * (old_avg_price - fill_price)
    elif side == 'SELL' and old_size > 0:
        realized_pnl = old_size * (fill_price - old_avg_price) if size >= old_size else size * (fill_price - old_avg_price)
    else:
        realized_pnl = 0.0
    fee = self.config.fee_per_contract * size
    realized_pnl -= fee
    self.position.size = new_size
    self.position.avg_price = new_avg_price
    self.position.entry_time = timestamp if new_size != 0 else None
    self.cumulative_pnl += realized_pnl
    trade = Trade(timestamp=timestamp, side=side, price=fill_price, size=size, position_after=new_size, pnl=realized_pnl, cumulative_pnl=self.cumulative_pnl, reason=reason)
    self.trades.append(trade)
    self.last_trade_time = timestamp
    self.trade_count += 1
    self.trades_this_minute += 1
    return trade

def check_exits(self, timestamp, current_price):
    if self.position.size == 0:
        return []
    if self.position.size > 0:
        unrealized_pnl = self.position.size * (current_price - self.position.avg_price)
    else:
        unrealized_pnl = abs(self.position.size) * (self.position.avg_price - current_price)
    self.position.unrealized_pnl = unrealized_pnl
    pnl_per_contract = unrealized_pnl / abs(self.position.size) if self.position.size != 0 else 0
    should_exit = False
    exit_reason = ""
    if pnl_per_contract <= -self.config.sl_ticks:
        should_exit = True
        exit_reason = "STOP_LOSS"
    elif pnl_per_contract >= self.config.tp_ticks:
        should_exit = True
        exit_reason = "TAKE_PROFIT"
    if self.position.entry_time is not None:
        hold_time_ms = (timestamp - self.position.entry_time).total_seconds() * 1000
        if hold_time_ms > self.config.max_hold_time_ms:
            should_exit = True
            exit_reason = "MAX_HOLD_TIME"
    exits = []
    if should_exit:
        exit_size = abs(self.position.size)
        exit_side = 'SELL' if self.position.size > 0 else 'BUY'
        exit_trade = self.execute_trade(exit_side, exit_size, current_price, timestamp, exit_reason)
        if exit_trade:
            exits.append(exit_trade)
    return exits

def update_equity_curve(self, timestamp, current_price):
    if self.position.size != 0:
        if self.position.size > 0:
            unrealized = self.position.size * (current_price - self.position.avg_price)
        else:
            unrealized = abs(self.position.size) * (self.position.avg_price - current_price)
    else:
        unrealized = 0.0
    total_pnl = self.cumulative_pnl + unrealized
    self.equity_curve.append({'timestamp': timestamp, 'realized_pnl': self.cumulative_pnl, 'unrealized_pnl': unrealized, 'total_pnl': total_pnl, 'position': self.position.size, 'price': current_price})

def flatten_position(self, timestamp, current_price):
    if self.position.size == 0:
        return
    exit_size = abs(self.position.size)
    exit_side = 'SELL' if self.position.size > 0 else 'BUY'
    self.execute_trade(exit_side, exit_size, current_price, timestamp, "END_OF_DAY")

# Bind methods to class
ExecutionEngine.execute_trade = execute_trade
ExecutionEngine.check_exits = check_exits
ExecutionEngine.update_equity_curve = update_equity_curve
ExecutionEngine.flatten_position = flatten_position

print("Execution engine methods added")""".split('\n')
    })
    
    # Main backtest loop
    new_cells.append({
        "cell_type": "code",
        "execution_count": None,
        "metadata": {},
        "source": """# Main backtest loop
print(f"Starting backtest for {config.trading_day}")
print("=" * 60)

# Create evaluation timestamps
first_time = timeline_df.iloc[0]['timestamp']
last_time = timeline_df.iloc[-1]['timestamp']
eval_times = []
current = first_time
interval_ms = config.evaluation_interval_ms

while current <= last_time:
    mask = timeline_df['timestamp'] <= current
    if mask.any():
        eval_times.append(current)
    current += timedelta(milliseconds=interval_ms)

print(f"Evaluation points: {len(eval_times)}")
print("Running backtest...")

latest_macro_weight = 0.0
latest_macro_time = None

for i, eval_time in enumerate(eval_times):
    if i % 1000 == 0:
        print(f"Progress: {i}/{len(eval_times)} ({i/len(eval_times)*100:.1f}%)")
    mask = timeline_df['timestamp'] <= eval_time
    if not mask.any():
        continue
    book_row = timeline_df[mask].iloc[-1]
    current_price = book_row['mid_price']
    ohlcv_mask = df_ohlcv_day['ts_event'] <= eval_time
    if ohlcv_mask.any():
        latest_bar = df_ohlcv_day[ohlcv_mask].iloc[-1]
        bar_time = latest_bar['ts_event']
        if latest_macro_time is None or bar_time > latest_macro_time:
            macro_result = macro_gen.compute_macro_weight(eval_time)
            latest_macro_weight = macro_result['weight']
            latest_macro_time = bar_time
    micro_result = micro_gen.compute_micro_alpha(book_row)
    micro_alpha = micro_result['alpha']
    combined_bias = (config.macro_weight_alpha * latest_macro_weight + (1 - config.macro_weight_alpha) * micro_alpha)
    combined_bias = np.clip(combined_bias, -1.0, 1.0)
    target_position = int(round(combined_bias * config.max_position))
    current_position = execution.position.size
    execution.check_exits(eval_time, current_price)
    position_diff = target_position - current_position
    if abs(position_diff) >= 1:
        if position_diff > 0:
            trade_size = min(position_diff, config.max_position - current_position)
            if trade_size > 0:
                fill_price = execution.compute_fill_price('BUY', book_row)
                execution.execute_trade('BUY', trade_size, fill_price, eval_time, f"BIAS={combined_bias:.3f}, MACRO={latest_macro_weight:.3f}, MICRO={micro_alpha:.3f}")
        else:
            trade_size = min(abs(position_diff), config.max_position + current_position)
            if trade_size > 0:
                fill_price = execution.compute_fill_price('SELL', book_row)
                execution.execute_trade('SELL', trade_size, fill_price, eval_time, f"BIAS={combined_bias:.3f}, MACRO={latest_macro_weight:.3f}, MICRO={micro_alpha:.3f}")
    execution.update_equity_curve(eval_time, current_price)
    if execution.killed:
        print(f"\\nKILL SWITCH ACTIVATED at {eval_time}")
        break

if execution.position.size != 0:
    last_price = timeline_df.iloc[-1]['mid_price']
    execution.flatten_position(eval_times[-1], last_price)

print("\\nBacktest complete!")
print("=" * 60)""".split('\n')
    })
    
    # Save outputs
    new_cells.append({
        "cell_type": "code",
        "execution_count": None,
        "metadata": {},
        "source": """# Save outputs
os.makedirs(config.output_dir, exist_ok=True)

if execution.trades:
    trades_df = pd.DataFrame([{'timestamp': t.timestamp, 'side': t.side, 'price': t.price, 'size': t.size, 'position_after': t.position_after, 'pnl': t.pnl, 'cumulative_pnl': t.cumulative_pnl, 'reason': t.reason} for t in execution.trades])
    trades_path = os.path.join(config.output_dir, config.trades_file)
    trades_df.to_csv(trades_path, index=False)
    print(f"Trades saved to {trades_path}")
else:
    print("No trades executed.")

if execution.equity_curve:
    equity_df = pd.DataFrame(execution.equity_curve)
    equity_path = os.path.join(config.output_dir, config.equity_file)
    equity_df.to_csv(equity_path, index=False)
    print(f"Equity curve saved to {equity_path}")""".split('\n')
    })
    
    # Summary statistics
    new_cells.append({
        "cell_type": "code",
        "execution_count": None,
        "metadata": {},
        "source": """# Print summary statistics
print("\\n" + "=" * 60)
print("BACKTEST SUMMARY")
print("=" * 60)

total_trades = len(execution.trades)
final_pnl = execution.cumulative_pnl

if total_trades == 0:
    print("No trades executed.")
else:
    buy_trades = [t for t in execution.trades if t.side == 'BUY']
    sell_trades = [t for t in execution.trades if t.side == 'SELL']
    total_volume = sum(abs(t.size) for t in execution.trades)
    total_fees = sum(config.fee_per_contract * abs(t.size) for t in execution.trades)
    winning_trades = [t for t in execution.trades if t.pnl > 0]
    losing_trades = [t for t in execution.trades if t.pnl < 0]
    win_rate = len(winning_trades) / total_trades * 100 if total_trades > 0 else 0
    avg_win = np.mean([t.pnl for t in winning_trades]) if winning_trades else 0
    avg_loss = np.mean([t.pnl for t in losing_trades]) if losing_trades else 0
    print(f"\\nTrading Day: {config.trading_day}")
    print(f"Total Trades: {total_trades}")
    print(f"  Buy: {len(buy_trades)}, Sell: {len(sell_trades)}")
    print(f"Total Volume: {total_volume} contracts")
    print(f"Total Fees: ${total_fees:.2f}")
    print(f"\\nFinal PnL: ${final_pnl:.2f}")
    print(f"Win Rate: {win_rate:.1f}%")
    if winning_trades:
        print(f"Avg Win: ${avg_win:.2f}")
    if losing_trades:
        print(f"Avg Loss: ${avg_loss:.2f}")
    if winning_trades and losing_trades:
        profit_factor = abs(sum(t.pnl for t in winning_trades) / sum(t.pnl for t in losing_trades))
        print(f"Profit Factor: {profit_factor:.2f}")
    exit_reasons = {}
    for t in execution.trades:
        reason = t.reason
        if reason in ['STOP_LOSS', 'TAKE_PROFIT', 'MAX_HOLD_TIME', 'END_OF_DAY']:
            exit_reasons[reason] = exit_reasons.get(reason, 0) + 1
    if exit_reasons:
        print(f"\\nExit Reasons:")
        for reason, count in exit_reasons.items():
            print(f"  {reason}: {count}")
    if execution.equity_curve:
        equity_series = pd.Series([e['total_pnl'] for e in execution.equity_curve])
        max_drawdown = (equity_series - equity_series.expanding().max()).min()
        print(f"\\nMax Drawdown: ${max_drawdown:.2f}")

print("=" * 60)""".split('\n')
    })
    
    # Convert string sources to list format
    for cell in new_cells:
        if isinstance(cell['source'], str):
            cell['source'] = [line + '\n' for line in cell['source'].split('\n')]
        elif isinstance(cell['source'], list):
            # Already in list format, ensure newlines
            cell['source'] = [line if line.endswith('\n') else line + '\n' for line in cell['source']]
    
    nb['cells'].extend(new_cells)
    
    with open('data.ipynb', 'w') as f:
        json.dump(nb, f, indent=1)
    
    print(f"Added {len(new_cells)} cells. Total cells: {len(nb['cells'])}")

if __name__ == "__main__":
    add_all_cells()
