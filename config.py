"""
Configuration module for the HFT trading system.
All parameters are centralized here for easy tuning.
"""
import os
from dataclasses import dataclass, field
from datetime import datetime, timezone
from typing import Optional
from pathlib import Path


@dataclass
class TradingConfig:
    """Main configuration for the trading system."""
    
    # Base directory for data files (defaults to config file's directory)
    _base_dir: Path = field(default_factory=lambda: Path(__file__).parent, init=False, repr=False)
    
    # Data files (will be resolved relative to base_dir)
    ohlcv_file: str = "copper_continuous_1min_jan2024.csv"
    tbbo_file: str = "copper_continuous_tbbo_jan2024.csv"
    mbp_file: str = "copper_continuous_mbp-10_3days.csv"
    
    def __post_init__(self):
        """Resolve relative file paths to absolute paths."""
        # Resolve data file paths
        if not os.path.isabs(self.ohlcv_file):
            self.ohlcv_file = str(self._base_dir / self.ohlcv_file)
        if not os.path.isabs(self.tbbo_file):
            self.tbbo_file = str(self._base_dir / self.tbbo_file)
        if not os.path.isabs(self.mbp_file):
            self.mbp_file = str(self._base_dir / self.mbp_file)
        # Resolve output directory
        if not os.path.isabs(self.output_dir):
            self.output_dir = str(self._base_dir / self.output_dir)
    
    # Trading day selection (YYYY-MM-DD)
    trading_day: str = "2024-01-02"  # Choose a specific day
    
    # Session boundaries (UTC) - using 1 hour window for faster testing
    session_start: str = "14:00:00"  # 1 hour window
    session_end: str = "15:00:00"
    
    # Macro TA parameters
    ma_fast_period: int = 10  # Fast MA period (bars)
    ma_slow_period: int = 30  # Slow MA period (bars)
    bollinger_period: int = 20  # Bollinger Bands period
    bollinger_std: float = 2.0  # Number of standard deviations
    macro_smoothing_alpha: float = 0.3  # EMA smoothing for macro weights
    
    # Macro weight contributions (should sum to ~1.0)
    weight_ma: float = 0.4  # MA crossover weight
    weight_bollinger: float = 0.4  # Bollinger Bands weight
    weight_trend: float = 0.2  # Trend filter weight
    
    # Signal combination
    macro_weight_alpha: float = 0.6  # Macro vs micro blend (0.6 = 60% macro, 40% micro)
    
    # Microstructure parameters
    obi_decay_type: str = "exponential"  # "linear" or "exponential"
    obi_decay_rate: float = 0.5  # For exponential: exp(-rate * level)
    max_spread_ticks: float = 10.0  # Don't trade if spread > this (in price units)
    min_book_depth: int = 2  # Minimum levels required to trade
    
    # Position and risk limits
    max_position: int = 10  # Maximum position size (contracts)
    daily_loss_limit: float = -5000.0  # Stop trading if PnL < this
    max_hold_time_ms: int = 30000  # Maximum holding time (30 seconds)
    
    # Trade management
    tp_ticks: float = 0.0025  # Take profit (in price units, ~10 ticks for copper)
    sl_ticks: float = 0.0025  # Stop loss (in price units)
    
    # Execution parameters
    evaluation_interval_ms: int = 500  # Evaluate every N milliseconds (increased for faster testing)
    trade_cooldown_ms: int = 200  # Cooldown after each trade
    max_trades_per_minute: Optional[int] = 60  # Optional throttle
    
    # Transaction costs
    fee_per_contract: float = 0.5  # Fee per contract (dollars)
    slippage_ticks: float = 0.5  # Slippage in ticks (0.5 = half tick)
    tick_size: float = 0.0005  # Minimum price increment for copper
    
    # Output
    output_dir: str = "output"
    trades_file: str = "trades.csv"
    equity_file: str = "equity_curve.csv"
    
    def get_trading_day_start(self) -> datetime:
        """Get the start datetime for the trading day."""
        dt = datetime.fromisoformat(f"{self.trading_day}T{self.session_start}")
        return dt.replace(tzinfo=timezone.utc)
    
    def get_trading_day_end(self) -> datetime:
        """Get the end datetime for the trading day."""
        dt = datetime.fromisoformat(f"{self.trading_day}T{self.session_end}")
        return dt.replace(tzinfo=timezone.utc)

