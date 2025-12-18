"""
Macro TA weight system.
Computes point-based directional weights from OHLCV bars.
"""
import pandas as pd
import numpy as np
from typing import Dict
from config import TradingConfig
from data_loader import DataLoader


class MacroSignalGenerator:
    """Generates macro TA signals from OHLCV bars."""
    
    def __init__(self, config: TradingConfig, data_loader: DataLoader):
        self.config = config
        self.data_loader = data_loader
        self.last_weight: float = 0.0  # For smoothing
        
    def compute_ma_crossover_score(self, ohlcv: pd.DataFrame) -> float:
        """
        Compute MA crossover score in [-1, +1].
        Positive = bullish (fast MA > slow MA)
        """
        if len(ohlcv) < self.config.ma_slow_period:
            return 0.0
        
        closes = ohlcv['close'].values
        fast_ma = pd.Series(closes).ewm(span=self.config.ma_fast_period, adjust=False).mean().iloc[-1]
        slow_ma = pd.Series(closes).ewm(span=self.config.ma_slow_period, adjust=False).mean().iloc[-1]
        
        if pd.isna(fast_ma) or pd.isna(slow_ma):
            return 0.0
        
        # Distance between MAs normalized by recent volatility
        ma_diff = fast_ma - slow_ma
        recent_vol = closes[-min(20, len(closes)):].std()
        
        if recent_vol == 0 or pd.isna(recent_vol):
            # Simple sign-based score
            score = 0.3 if ma_diff > 0 else -0.3
        else:
            # Normalize by volatility and clamp
            normalized_diff = ma_diff / (recent_vol + 1e-8)
            score = np.clip(normalized_diff * 0.5, -0.6, 0.6)
        
        return score
    
    def compute_bollinger_score(self, ohlcv: pd.DataFrame) -> float:
        """
        Compute Bollinger Bands score in [-1, +1].
        Positive = long-leaning (price near/below lower band, mean reversion)
        Negative = short-leaning (price near/above upper band)
        """
        if len(ohlcv) < self.config.bollinger_period:
            return 0.0
        
        closes = ohlcv['close'].values
        prices = pd.Series(closes)
        
        # Compute Bollinger Bands
        sma = prices.rolling(window=self.config.bollinger_period).mean().iloc[-1]
        std = prices.rolling(window=self.config.bollinger_period).std().iloc[-1]
        
        if pd.isna(sma) or pd.isna(std) or std == 0:
            return 0.0
        
        upper_band = sma + self.config.bollinger_std * std
        lower_band = sma - self.config.bollinger_std * std
        current_price = closes[-1]
        band_width = upper_band - lower_band
        
        if band_width == 0:
            return 0.0
        
        # Distance from bands (normalized by band width)
        dist_from_lower = (current_price - lower_band) / band_width
        dist_from_upper = (current_price - upper_band) / band_width
        
        # Mean reversion vs breakout regime
        # Use ATR or band width percentile to determine regime
        recent_vol = closes[-min(20, len(closes)):].std()
        vol_percentile = recent_vol / (std + 1e-8)  # Relative volatility
        
        # If low volatility (mean reversion regime)
        if vol_percentile < 0.8:
            # Mean reversion: buy near lower, sell near upper
            if dist_from_lower < 0.2:  # Below lower band
                score = 0.5 * (1.0 - dist_from_lower / 0.2)  # Stronger as further below
            elif dist_from_upper > 0.8:  # Above upper band
                score = -0.5 * ((dist_from_upper - 0.8) / 0.2)  # Stronger as further above
            else:
                # Near middle: neutral
                score = 0.0
        else:
            # High volatility (breakout regime): opposite
            if dist_from_upper > 0.8:  # Strong breakout above
                score = 0.3  # Follow momentum
            elif dist_from_lower < 0.2:  # Strong breakdown below
                score = -0.3  # Follow momentum
            else:
                score = 0.0
        
        return np.clip(score, -0.5, 0.5)
    
    def compute_trend_filter(self, ohlcv: pd.DataFrame) -> float:
        """
        Compute trend filter score.
        Positive = uptrend (dampens short signals)
        Negative = downtrend (dampens long signals)
        """
        if len(ohlcv) < self.config.ma_slow_period:
            return 0.0
        
        closes = ohlcv['close'].values
        slow_ema = pd.Series(closes).ewm(span=self.config.ma_slow_period, adjust=False).mean()
        
        if len(slow_ema) < 2:
            return 0.0
        
        # Slope of slow EMA
        ema_slope = (slow_ema.iloc[-1] - slow_ema.iloc[-2]) / (slow_ema.iloc[-2] + 1e-8)
        
        # Normalize by recent price volatility
        recent_vol = closes[-min(20, len(closes)):].std()
        if recent_vol == 0 or pd.isna(recent_vol):
            return 0.0
        
        normalized_slope = ema_slope / (recent_vol + 1e-8)
        # Convert to [-0.2, +0.2] range for trend filter
        score = np.clip(normalized_slope * 10, -0.2, 0.2)
        
        return score
    
    def compute_macro_weight(self, timestamp: pd.Timestamp) -> Dict[str, float]:
        """
        Compute macro weight W_macro(t) in [-1, +1] and sub-scores.
        Returns dict with 'weight', 'w_ma', 'w_boll', 'w_trend'.
        """
        ohlcv = self.data_loader.get_ohlcv_history(timestamp, lookback_bars=100)
        
        if ohlcv is None or len(ohlcv) < max(self.config.ma_slow_period, self.config.bollinger_period):
            # Not enough data, return neutral
            return {
                'weight': 0.0,
                'w_ma': 0.0,
                'w_boll': 0.0,
                'w_trend': 0.0
            }
        
        # Compute sub-scores
        w_ma = self.compute_ma_crossover_score(ohlcv)
        w_boll = self.compute_bollinger_score(ohlcv)
        w_trend = self.compute_trend_filter(ohlcv)
        
        # Combine with weights
        raw_weight = (
            self.config.weight_ma * w_ma +
            self.config.weight_bollinger * w_boll +
            self.config.weight_trend * w_trend
        )
        
        # Apply trend filter: dampen signals against trend
        if w_trend < 0 and raw_weight > 0:
            raw_weight *= 0.7  # Dampen long signals in downtrend
        elif w_trend > 0 and raw_weight < 0:
            raw_weight *= 0.7  # Dampen short signals in uptrend
        
        # Clamp to [-1, +1]
        raw_weight = np.clip(raw_weight, -1.0, 1.0)
        
        # Apply smoothing (EMA)
        self.last_weight = (
            self.config.macro_smoothing_alpha * raw_weight +
            (1 - self.config.macro_smoothing_alpha) * self.last_weight
        )
        
        return {
            'weight': self.last_weight,
            'w_ma': w_ma,
            'w_boll': w_boll,
            'w_trend': w_trend
        }
