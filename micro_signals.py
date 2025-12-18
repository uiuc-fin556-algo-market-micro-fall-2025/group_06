"""
Microstructure alpha signal generator.
Computes micro signals from order book state (MBP-10 / TBBO).
"""
import pandas as pd
import numpy as np
from typing import Dict, Optional
from config import TradingConfig


class MicroSignalGenerator:
    """Generates microstructure alpha signals from order book data."""
    
    def __init__(self, config: TradingConfig):
        self.config = config
        self.last_obi: Optional[float] = None
        
    def compute_order_book_imbalance(self, book_row: pd.Series) -> float:
        """
        Compute Order Book Imbalance (OBI) from MBP-10 data.
        Returns value in [-1, +1] where +1 = strong bid pressure, -1 = strong ask pressure.
        """
        bid_vol = 0.0
        ask_vol = 0.0
        
        for level in range(10):
            bid_px_col = f'bid_px_{level}'
            ask_px_col = f'ask_px_{level}'
            bid_sz_col = f'bid_sz_{level}'
            ask_sz_col = f'ask_sz_{level}'
            
            if bid_px_col not in book_row or pd.isna(book_row[bid_px_col]):
                break
            
            bid_size = book_row.get(bid_sz_col, 0)
            ask_size = book_row.get(ask_sz_col, 0)
            
            if self.config.obi_decay_type == "exponential":
                weight = np.exp(-self.config.obi_decay_rate * level)
            else:  # linear
                weight = 1.0 / (level + 1)
            
            bid_vol += weight * bid_size
            ask_vol += weight * ask_size
        
        total_vol = bid_vol + ask_vol
        if total_vol == 0:
            return 0.0
        
        obi = (bid_vol - ask_vol) / total_vol
        return np.clip(obi, -1.0, 1.0)
    
    def compute_microprice(self, book_row: pd.Series) -> float:
        """
        Compute microprice (weighted mid) and deviation from mid.
        Returns deviation in price units.
        """
        bid_px = book_row['bid_px']
        ask_px = book_row['ask_px']
        bid_sz = book_row['bid_sz']
        ask_sz = book_row['ask_sz']
        
        mid = (bid_px + ask_px) / 2.0
        
        total_sz = bid_sz + ask_sz
        if total_sz == 0:
            return 0.0
        
        # Weighted mid (microprice)
        microprice = (ask_px * bid_sz + bid_px * ask_sz) / total_sz
        
        # Deviation from mid (normalized by spread)
        spread = ask_px - bid_px
        if spread == 0:
            return 0.0
        
        deviation = (microprice - mid) / spread
        return deviation
    
    def compute_spread_confidence(self, book_row: pd.Series) -> float:
        """
        Compute confidence term based on spread and book depth.
        Returns value in [0, 1] where 1 = high confidence (tight spread, good depth).
        """
        spread = book_row['spread']
        bid_sz = book_row['bid_sz']
        ask_sz = book_row['ask_sz']
        
        # Check spread threshold
        if spread > self.config.max_spread_ticks:
            return 0.0
        
        # Check minimum depth
        min_depth = min(bid_sz, ask_sz)
        if min_depth < self.config.min_book_depth:
            return 0.3  # Low confidence
        
        # Normalize spread confidence (tighter = better)
        spread_confidence = 1.0 - min(spread / self.config.max_spread_ticks, 1.0)
        
        # Depth confidence (more depth = better, but diminishing returns)
        depth_confidence = min(min_depth / 10.0, 1.0)
        
        # Combined confidence
        confidence = 0.6 * spread_confidence + 0.4 * depth_confidence
        return confidence
    
    def compute_micro_alpha(self, book_row: pd.Series) -> Dict[str, float]:
        """
        Compute micro alpha signal A_micro(t) in [-1, +1].
        Returns dict with 'alpha', 'obi', 'microprice_dev', 'confidence'.
        """
        # Compute OBI
        obi = self.compute_order_book_imbalance(book_row)
        
        # Compute microprice deviation
        microprice_dev = self.compute_microprice(book_row)
        
        # Compute confidence
        confidence = self.compute_spread_confidence(book_row)
        
        # Combine OBI and microprice into raw alpha
        # Positive alpha when: strong bid pressure (OBI > 0) AND microprice above mid
        raw_alpha = 0.6 * obi + 0.4 * microprice_dev
        
        # Apply confidence scaling
        alpha = raw_alpha * confidence
        
        # Clamp to [-1, +1]
        alpha = np.clip(alpha, -1.0, 1.0)
        
        self.last_obi = obi
        
        return {
            'alpha': alpha,
            'obi': obi,
            'microprice_dev': microprice_dev,
            'confidence': confidence
        }
