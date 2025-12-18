"""
Main backtest script for the HFT trading system.
Runs a single-day backtest and outputs results.
"""
import pandas as pd
import numpy as np
from datetime import datetime, timedelta
import os
from config import TradingConfig
from data_loader import DataLoader
from macro_signals import MacroSignalGenerator
from micro_signals import MicroSignalGenerator
from execution_engine import ExecutionEngine


class Backtest:
    """Main backtest orchestrator."""
    
    def __init__(self, config: TradingConfig):
        self.config = config
        self.data_loader = DataLoader(config)
        self.macro_gen = MacroSignalGenerator(config, self.data_loader)
        self.micro_gen = MicroSignalGenerator(config)
        self.execution = ExecutionEngine(config)
        
    def run(self):
        """Run the backtest for one trading day."""
        print(f"Starting backtest for {self.config.trading_day}")
        print("=" * 60)
        
        # Create unified timeline
        print("Loading and aligning data...")
        timeline = self.data_loader.create_unified_timeline()
        print(f"Timeline created: {len(timeline)} events")
        
        if len(timeline) == 0:
            print("ERROR: No data found for the specified trading day!")
            return
        
        # Sample timeline at evaluation interval
        timeline['timestamp'] = pd.to_datetime(timeline['timestamp'])
        timeline = timeline.sort_values('timestamp').reset_index(drop=True)
        
        # Downsample to evaluation interval - use timeline directly instead of generating all timestamps
        first_time = timeline.iloc[0]['timestamp']
        last_time = timeline.iloc[-1]['timestamp']
        
        # Sample timeline at evaluation interval using pandas
        interval_ms = self.config.evaluation_interval_ms
        timeline_indexed = timeline.set_index('timestamp')
        
        # Resample to evaluation interval, taking last value in each interval
        timeline_resampled = timeline_indexed.resample(f'{interval_ms}ms').last().dropna()
        
        print(f"Timeline resampled: {len(timeline_resampled)} evaluation points")
        print("Running backtest...")
        
        # Track latest macro weight
        latest_macro_weight = 0.0
        latest_macro_time = None
        
        # Convert to list for iteration - use iloc for efficient access
        eval_times = timeline_resampled.index.tolist()
        
        # Main loop
        for i, eval_time in enumerate(eval_times):
            if i % 100 == 0:
                print(f"Progress: {i}/{len(eval_times)} ({i/len(eval_times)*100:.1f}%)")
            
            # Get book state at eval_time (already resampled)
            book_row = timeline_resampled.iloc[i]
            current_price = book_row['mid_price']
            
            # Update macro weight (only when new bar closes)
            # Check if we have a new OHLCV bar
            ohlcv_bar = self.data_loader.get_ohlcv_for_timestamp(eval_time)
            if ohlcv_bar is not None:
                bar_time = ohlcv_bar['ts_event']
                if latest_macro_time is None or bar_time > latest_macro_time:
                    macro_result = self.macro_gen.compute_macro_weight(eval_time)
                    latest_macro_weight = macro_result['weight']
                    latest_macro_time = bar_time
            
            # Compute micro alpha
            micro_result = self.micro_gen.compute_micro_alpha(book_row)
            micro_alpha = micro_result['alpha']
            
            # Combine signals
            combined_bias = (
                self.config.macro_weight_alpha * latest_macro_weight +
                (1 - self.config.macro_weight_alpha) * micro_alpha
            )
            combined_bias = np.clip(combined_bias, -1.0, 1.0)
            
            # Convert to target position
            target_position = int(round(combined_bias * self.config.max_position))
            current_position = self.execution.position.size
            
            # Check exits first
            self.execution.check_exits(eval_time, current_price)
            
            # Position adjustment (with deadzone)
            position_diff = target_position - current_position
            if abs(position_diff) >= 1:  # Deadzone: don't trade if diff < 1
                if position_diff > 0:
                    # Increase long or reduce short
                    trade_size = min(position_diff, self.config.max_position - current_position)
                    if trade_size > 0:
                        fill_price = self.execution.compute_fill_price('BUY', book_row)
                        self.execution.execute_trade(
                            'BUY', trade_size, fill_price, eval_time,
                            f"BIAS={combined_bias:.3f}, MACRO={latest_macro_weight:.3f}, MICRO={micro_alpha:.3f}"
                        )
                else:
                    # Increase short or reduce long
                    trade_size = min(abs(position_diff), self.config.max_position + current_position)
                    if trade_size > 0:
                        fill_price = self.execution.compute_fill_price('SELL', book_row)
                        self.execution.execute_trade(
                            'SELL', trade_size, fill_price, eval_time,
                            f"BIAS={combined_bias:.3f}, MACRO={latest_macro_weight:.3f}, MICRO={micro_alpha:.3f}"
                        )
            
            # Update equity curve
            self.execution.update_equity_curve(eval_time, current_price)
            
            # Check kill switch
            if self.execution.killed:
                print(f"\nKILL SWITCH ACTIVATED at {eval_time}")
                break
        
        # Flatten position at end of day
        if self.execution.position.size != 0:
            last_price = timeline.iloc[-1]['mid_price']
            self.execution.flatten_position(eval_times[-1], last_price)
        
        print("\nBacktest complete!")
        print("=" * 60)
        
        # Generate outputs
        self.generate_outputs()
        self.print_summary()
    
    def generate_outputs(self):
        """Save trades and equity curve to CSV."""
        os.makedirs(self.config.output_dir, exist_ok=True)
        
        # Save trades
        if self.execution.trades:
            trades_df = pd.DataFrame([
                {
                    'timestamp': t.timestamp,
                    'side': t.side,
                    'price': t.price,
                    'size': t.size,
                    'position_after': t.position_after,
                    'pnl_with_fees': t.pnl,
                    'pnl_no_fees': t.pnl_no_fees,
                    'cumulative_pnl_with_fees': t.cumulative_pnl,
                    'cumulative_pnl_no_fees': t.cumulative_pnl_no_fees,
                    'is_closing_trade': t.is_closing_trade,
                    'reason': t.reason
                }
                for t in self.execution.trades
            ])
            trades_path = os.path.join(self.config.output_dir, self.config.trades_file)
            trades_df.to_csv(trades_path, index=False)
            print(f"Trades saved to {trades_path}")
        else:
            print("No trades executed.")
        
        # Save equity curve
        if self.execution.equity_curve:
            equity_df = pd.DataFrame(self.execution.equity_curve)
            equity_path = os.path.join(self.config.output_dir, self.config.equity_file)
            equity_df.to_csv(equity_path, index=False)
            print(f"Equity curve saved to {equity_path}")
    
    def print_summary(self):
        """Print summary statistics."""
        print("\n" + "=" * 60)
        print("BACKTEST SUMMARY")
        print("=" * 60)
        
        total_trades = len(self.execution.trades)
        final_pnl_with_fees = self.execution.cumulative_pnl
        final_pnl_no_fees = getattr(self.execution, 'cumulative_pnl_no_fees', 0.0)
        
        if total_trades == 0:
            print("No trades executed.")
            return
        
        # Trade statistics
        buy_trades = [t for t in self.execution.trades if t.side == 'BUY']
        sell_trades = [t for t in self.execution.trades if t.side == 'SELL']
        
        total_volume = sum(abs(t.size) for t in self.execution.trades)
        total_fees = sum(self.config.fee_per_contract * abs(t.size) for t in self.execution.trades)
        
        # Only count closing trades for win rate (trades that actually close positions)
        closing_trades = [t for t in self.execution.trades if t.is_closing_trade]
        winning_trades = [t for t in closing_trades if t.pnl > 0]
        losing_trades = [t for t in closing_trades if t.pnl < 0]
        breakeven_trades = [t for t in closing_trades if t.pnl == 0]
        
        closing_count = len(closing_trades)
        win_rate = len(winning_trades) / closing_count * 100 if closing_count > 0 else 0
        
        avg_win = np.mean([t.pnl for t in winning_trades]) if winning_trades else 0
        avg_loss = np.mean([t.pnl for t in losing_trades]) if losing_trades else 0
        
        avg_win_no_fees = np.mean([t.pnl_no_fees for t in winning_trades]) if winning_trades else 0
        avg_loss_no_fees = np.mean([t.pnl_no_fees for t in losing_trades]) if losing_trades else 0
        
        print(f"\nTrading Day: {self.config.trading_day}")
        print(f"Total Trades: {total_trades}")
        print(f"  Buy: {len(buy_trades)}, Sell: {len(sell_trades)}")
        print(f"  Closing Trades: {closing_count}")
        print(f"Total Volume: {total_volume} contracts")
        print(f"Total Fees: ${total_fees:.2f}")
        
        print(f"\n{'='*60}")
        print("PnL SUMMARY")
        print(f"{'='*60}")
        print(f"Final PnL (without fees): ${final_pnl_no_fees:.2f}")
        print(f"Total Fees:                ${total_fees:.2f}")
        print(f"Final PnL (with fees):     ${final_pnl_with_fees:.2f}")
        print(f"\nNote: Final PnL (with fees) = PnL (without fees) - Total Fees")
        
        print(f"\n{'='*60}")
        print("TRADE PERFORMANCE (Closing Trades Only)")
        print(f"{'='*60}")
        print(f"Closing Trades: {closing_count}")
        print(f"  Winning: {len(winning_trades)}")
        print(f"  Losing:  {len(losing_trades)}")
        print(f"  Breakeven: {len(breakeven_trades)}")
        print(f"Win Rate: {win_rate:.1f}%")
        
        if winning_trades:
            print(f"\nWinning Trades:")
            print(f"  Count: {len(winning_trades)}")
            print(f"  Avg PnL (with fees):    ${avg_win:.2f}")
            print(f"  Avg PnL (without fees):  ${avg_win_no_fees:.2f}")
            print(f"  Total PnL (with fees):   ${sum(t.pnl for t in winning_trades):.2f}")
            print(f"  Total PnL (without fees): ${sum(t.pnl_no_fees for t in winning_trades):.2f}")
        
        if losing_trades:
            print(f"\nLosing Trades:")
            print(f"  Count: {len(losing_trades)}")
            print(f"  Avg PnL (with fees):    ${avg_loss:.2f}")
            print(f"  Avg PnL (without fees):  ${avg_loss_no_fees:.2f}")
            print(f"  Total PnL (with fees):   ${sum(t.pnl for t in losing_trades):.2f}")
            print(f"  Total PnL (without fees): ${sum(t.pnl_no_fees for t in losing_trades):.2f}")
        
        if winning_trades and losing_trades:
            total_wins = sum(t.pnl for t in winning_trades)
            total_losses = abs(sum(t.pnl for t in losing_trades))
            profit_factor = total_wins / total_losses if total_losses > 0 else 0
            print(f"\nProfit Factor: {profit_factor:.2f}")
        
        # Exit reasons
        exit_reasons = {}
        for t in self.execution.trades:
            reason = t.reason
            if reason in ['STOP_LOSS', 'TAKE_PROFIT', 'MAX_HOLD_TIME', 'END_OF_DAY']:
                exit_reasons[reason] = exit_reasons.get(reason, 0) + 1
        
        if exit_reasons:
            print(f"\nExit Reasons:")
            for reason, count in exit_reasons.items():
                print(f"  {reason}: {count}")
        
        # Risk metrics
        if self.execution.equity_curve:
            equity_series = pd.Series([e['total_pnl'] for e in self.execution.equity_curve])
            max_drawdown = (equity_series - equity_series.expanding().max()).min()
            print(f"\nMax Drawdown: ${max_drawdown:.2f}")
        
        print("=" * 60)


def main():
    """Main entry point."""
    config = TradingConfig()
    backtest = Backtest(config)
    backtest.run()


if __name__ == "__main__":
    main()
