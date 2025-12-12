"""
Execution engine for HFT trading system.
Manages positions, fills, risk controls, and trade logging.
"""
import pandas as pd
import numpy as np
from datetime import datetime, timedelta
from typing import List, Dict, Optional
from dataclasses import dataclass
from config import TradingConfig


@dataclass
class Trade:
    """Represents a single trade."""
    timestamp: datetime
    side: str  # 'BUY' or 'SELL'
    price: float
    size: int
    position_after: int
    pnl: float  # PnL with fees
    pnl_no_fees: float  # PnL without fees
    cumulative_pnl: float
    cumulative_pnl_no_fees: float
    reason: str  # Why the trade was executed
    is_closing_trade: bool  # True if this trade closes a position


@dataclass
class Position:
    """Current position state."""
    size: int  # Positive = long, negative = short
    avg_price: float
    entry_time: Optional[datetime]
    unrealized_pnl: float


class ExecutionEngine:
    """Manages execution, positions, and risk controls."""
    
    def __init__(self, config: TradingConfig):
        self.config = config
        self.position = Position(size=0, avg_price=0.0, entry_time=None, unrealized_pnl=0.0)
        self.trades: List[Trade] = []
        self.equity_curve: List[Dict] = []
        self.cumulative_pnl: float = 0.0
        self.cumulative_pnl_no_fees: float = 0.0
        self.last_trade_time: Optional[datetime] = None
        self.trade_count: int = 0
        self.trades_this_minute: int = 0
        self.current_minute: Optional[int] = None
        self.killed: bool = False  # Kill switch flag
        
    def can_trade(self, timestamp: datetime) -> bool:
        """Check if trading is allowed (cooldown, throttle, kill switch)."""
        if self.killed:
            return False
        
        # Check daily loss limit
        if self.cumulative_pnl < self.config.daily_loss_limit:
            self.killed = True
            return False
        
        # Check cooldown
        if self.last_trade_time is not None:
            time_since_last = (timestamp - self.last_trade_time).total_seconds() * 1000
            if time_since_last < self.config.trade_cooldown_ms:
                return False
        
        # Check trades per minute throttle
        if self.config.max_trades_per_minute is not None:
            minute = timestamp.minute
            if minute != self.current_minute:
                self.current_minute = minute
                self.trades_this_minute = 0
            
            if self.trades_this_minute >= self.config.max_trades_per_minute:
                return False
        
        return True
    
    def compute_fill_price(self, side: str, book_row: pd.Series) -> float:
        """
        Compute fill price for a market order.
        Buy fills at ask, sell fills at bid.
        Adds slippage.
        """
        if side == 'BUY':
            base_price = book_row['ask_px']
            slippage_direction = 1  # Pay more
        else:  # SELL
            base_price = book_row['bid_px']
            slippage_direction = -1  # Receive less
        
        slippage = slippage_direction * self.config.slippage_ticks * self.config.tick_size
        fill_price = base_price + slippage
        return fill_price
    
    def execute_trade(self, side: str, size: int, price: float, timestamp: datetime, 
                     reason: str = "") -> Optional[Trade]:
        """
        Execute a trade and update position.
        Returns Trade object if executed, None if rejected.
        """
        if not self.can_trade(timestamp):
            return None
        
        if size == 0:
            return None
        
        # Compute fill price (with slippage)
        # Note: price passed in should already be from book, but we'll use it
        fill_price = price
        
        # Update position
        old_size = self.position.size
        old_avg_price = self.position.avg_price
        
        if side == 'BUY':
            new_size = old_size + size
            if old_size == 0:
                new_avg_price = fill_price
            elif old_size > 0:  # Adding to long
                new_avg_price = (old_size * old_avg_price + size * fill_price) / new_size
            else:  # Covering short
                if abs(size) >= abs(old_size):  # Full cover + new long
                    pnl = abs(old_size) * (old_avg_price - fill_price)
                    remaining = size - abs(old_size)
                    if remaining > 0:
                        new_avg_price = fill_price
                    else:
                        new_avg_price = 0.0
                else:  # Partial cover
                    pnl = size * (old_avg_price - fill_price)
                    new_avg_price = old_avg_price
                    new_size = old_size + size
        else:  # SELL
            new_size = old_size - size
            if old_size == 0:
                new_avg_price = fill_price
            elif old_size < 0:  # Adding to short
                new_avg_price = (abs(old_size) * old_avg_price + size * fill_price) / abs(new_size)
            else:  # Reducing long
                if size >= old_size:  # Full exit + new short
                    pnl = old_size * (fill_price - old_avg_price)
                    remaining = size - old_size
                    if remaining > 0:
                        new_avg_price = fill_price
                    else:
                        new_avg_price = 0.0
                else:  # Partial exit
                    pnl = size * (fill_price - old_avg_price)
                    new_avg_price = old_avg_price
                    new_size = old_size - size
        
        # Compute realized PnL (without fees first)
        is_closing = False
        if side == 'BUY' and old_size < 0:
            # Covering short
            is_closing = True
            if abs(size) >= abs(old_size):
                realized_pnl_no_fees = abs(old_size) * (old_avg_price - fill_price)
            else:
                realized_pnl_no_fees = size * (old_avg_price - fill_price)
        elif side == 'SELL' and old_size > 0:
            # Exiting long
            is_closing = True
            if size >= old_size:
                realized_pnl_no_fees = old_size * (fill_price - old_avg_price)
            else:
                realized_pnl_no_fees = size * (fill_price - old_avg_price)
        else:
            realized_pnl_no_fees = 0.0
        
        # Calculate fees
        fee = self.config.fee_per_contract * size
        realized_pnl_with_fees = realized_pnl_no_fees - fee
        
        # Update position
        self.position.size = new_size
        self.position.avg_price = new_avg_price
        if new_size != 0:
            self.position.entry_time = timestamp
        else:
            self.position.entry_time = None
        
        # Update cumulative PnL (track both with and without fees)
        self.cumulative_pnl += realized_pnl_with_fees
        if not hasattr(self, 'cumulative_pnl_no_fees'):
            self.cumulative_pnl_no_fees = 0.0
        self.cumulative_pnl_no_fees += realized_pnl_no_fees
        
        # Create trade record
        trade = Trade(
            timestamp=timestamp,
            side=side,
            price=fill_price,
            size=size,
            position_after=new_size,
            pnl=realized_pnl_with_fees,
            pnl_no_fees=realized_pnl_no_fees,
            cumulative_pnl=self.cumulative_pnl,
            cumulative_pnl_no_fees=self.cumulative_pnl_no_fees,
            reason=reason,
            is_closing_trade=is_closing
        )
        
        self.trades.append(trade)
        self.last_trade_time = timestamp
        self.trade_count += 1
        self.trades_this_minute += 1
        
        return trade
    
    def check_exits(self, timestamp: datetime, current_price: float) -> List[Trade]:
        """
        Check for exit conditions (stop loss, take profit, max hold time).
        Returns list of exit trades executed.
        """
        exits = []
        
        if self.position.size == 0:
            return exits
        
        # Compute unrealized PnL
        if self.position.size > 0:  # Long position
            unrealized_pnl = self.position.size * (current_price - self.position.avg_price)
        else:  # Short position
            unrealized_pnl = abs(self.position.size) * (self.position.avg_price - current_price)
        
        self.position.unrealized_pnl = unrealized_pnl
        
        # Check stop loss / take profit
        pnl_per_contract = unrealized_pnl / abs(self.position.size) if self.position.size != 0 else 0
        
        should_exit = False
        exit_reason = ""
        
        if pnl_per_contract <= -self.config.sl_ticks:
            should_exit = True
            exit_reason = "STOP_LOSS"
        elif pnl_per_contract >= self.config.tp_ticks:
            should_exit = True
            exit_reason = "TAKE_PROFIT"
        
        # Check max hold time
        if self.position.entry_time is not None:
            hold_time_ms = (timestamp - self.position.entry_time).total_seconds() * 1000
            if hold_time_ms > self.config.max_hold_time_ms:
                should_exit = True
                exit_reason = "MAX_HOLD_TIME"
        
        if should_exit:
            # Flatten position
            exit_size = abs(self.position.size)
            exit_side = 'SELL' if self.position.size > 0 else 'BUY'
            exit_trade = self.execute_trade(exit_side, exit_size, current_price, timestamp, exit_reason)
            if exit_trade:
                exits.append(exit_trade)
        
        return exits
    
    def update_equity_curve(self, timestamp: datetime, current_price: float):
        """Update equity curve with current PnL."""
        # Compute total PnL (realized + unrealized)
        if self.position.size != 0:
            if self.position.size > 0:
                unrealized = self.position.size * (current_price - self.position.avg_price)
            else:
                unrealized = abs(self.position.size) * (self.position.avg_price - current_price)
        else:
            unrealized = 0.0
        
        total_pnl = self.cumulative_pnl + unrealized
        
        self.equity_curve.append({
            'timestamp': timestamp,
            'realized_pnl': self.cumulative_pnl,
            'unrealized_pnl': unrealized,
            'total_pnl': total_pnl,
            'position': self.position.size,
            'price': current_price
        })
    
    def flatten_position(self, timestamp: datetime, current_price: float):
        """Force flatten position (e.g., at end of day)."""
        if self.position.size == 0:
            return
        
        exit_size = abs(self.position.size)
        exit_side = 'SELL' if self.position.size > 0 else 'BUY'
        self.execute_trade(exit_side, exit_size, current_price, timestamp, "END_OF_DAY")
