"""
Data loading and timeline alignment module.
Loads OHLCV, TBBO, and MBP-10 data and creates a unified event timeline.
"""
import pandas as pd
import numpy as np
from datetime import datetime
from typing import Tuple, Optional
from config import TradingConfig


class DataLoader:
    """Loads and aligns market data from CSV files."""
    
    def __init__(self, config: TradingConfig):
        self.config = config
        self.ohlcv_df: Optional[pd.DataFrame] = None
        self.tbbo_df: Optional[pd.DataFrame] = None
        self.mbp_df: Optional[pd.DataFrame] = None
        
    def load_ohlcv(self) -> pd.DataFrame:
        """Load OHLCV bars from CSV."""
        df = pd.read_csv(self.config.ohlcv_file)
        df['ts_event'] = pd.to_datetime(df['ts_event'], format='ISO8601')
        df = df.sort_values('ts_event').reset_index(drop=True)
        self.ohlcv_df = df
        return df
    
    def load_tbbo(self) -> pd.DataFrame:
        """Load TBBO data from CSV."""
        df = pd.read_csv(self.config.tbbo_file)
        df['ts_recv'] = pd.to_datetime(df['ts_recv'], format='ISO8601')
        df['ts_event'] = pd.to_datetime(df['ts_event'], format='ISO8601')
        # Use ts_recv as primary timestamp for execution
        df = df.sort_values('ts_recv').reset_index(drop=True)
        self.tbbo_df = df
        return df
    
    def load_mbp(self) -> pd.DataFrame:
        """Load MBP-10 data from CSV."""
        df = pd.read_csv(self.config.mbp_file)
        df['ts_recv'] = pd.to_datetime(df['ts_recv'], format='ISO8601')
        df['ts_event'] = pd.to_datetime(df['ts_event'], format='ISO8601')
        df = df.sort_values('ts_recv').reset_index(drop=True)
        self.mbp_df = df
        return df
    
    def filter_trading_day(self, df: pd.DataFrame, time_col: str = 'ts_recv') -> pd.DataFrame:
        """Filter data to the specified trading day."""
        day_start = self.config.get_trading_day_start()
        day_end = self.config.get_trading_day_end()
        
        mask = (df[time_col] >= day_start) & (df[time_col] <= day_end)
        return df[mask].copy()
    
    def create_unified_timeline(self) -> pd.DataFrame:
        """
        Create a unified event timeline using book timestamps (ts_recv) as primary clock.
        Merges TBBO and MBP-10 data, keeping latest book state at each timestamp.
        Simplified and optimized version.
        """
        print("  Loading TBBO data...")
        tbbo = self.load_tbbo()
        print(f"  TBBO loaded: {len(tbbo)} rows")
        
        print("  Loading MBP data...")
        mbp = self.load_mbp()
        print(f"  MBP loaded: {len(mbp)} rows")
        
        # Filter to trading day
        print("  Filtering to trading day...")
        tbbo = self.filter_trading_day(tbbo, 'ts_recv')
        mbp = self.filter_trading_day(mbp, 'ts_recv')
        print(f"  After filtering - TBBO: {len(tbbo)}, MBP: {len(mbp)}")
        
        if len(tbbo) == 0 and len(mbp) == 0:
            return pd.DataFrame()
        
        print("  Creating unified timeline...")
        
        # Build result rows - prefer MBP, fallback to TBBO
        result_rows = []
        
        # Get all unique timestamps from both
        all_timestamps = sorted(set(
            tbbo['ts_recv'].tolist() + mbp['ts_recv'].tolist()
        ))
        print(f"  Total unique timestamps: {len(all_timestamps)}")
        
        # Create lookup dictionaries for faster access
        tbbo_dict = {row['ts_recv']: row for _, row in tbbo.iterrows()}
        mbp_dict = {row['ts_recv']: row for _, row in mbp.iterrows()}
        
        # Process each timestamp
        for i, ts in enumerate(all_timestamps):
            if i % 10000 == 0 and i > 0:
                print(f"    Processing: {i}/{len(all_timestamps)} ({i/len(all_timestamps)*100:.1f}%)")
            
            mbp_row = mbp_dict.get(ts)
            tbbo_row = tbbo_dict.get(ts)
            
            # Prefer MBP if available
            if mbp_row is not None:
                row = {
                    'timestamp': ts,
                    'bid_px': mbp_row['bid_px_00'],
                    'ask_px': mbp_row['ask_px_00'],
                    'bid_sz': mbp_row['bid_sz_00'],
                    'ask_sz': mbp_row['ask_sz_00'],
                    'spread': mbp_row['ask_px_00'] - mbp_row['bid_px_00'],
                    'mid_price': (mbp_row['bid_px_00'] + mbp_row['ask_px_00']) / 2.0,
                }
                # Add MBP-10 levels
                for level in range(10):
                    row[f'bid_px_{level}'] = mbp_row.get(f'bid_px_{level:02d}', np.nan)
                    row[f'ask_px_{level}'] = mbp_row.get(f'ask_px_{level:02d}', np.nan)
                    row[f'bid_sz_{level}'] = mbp_row.get(f'bid_sz_{level:02d}', 0)
                    row[f'ask_sz_{level}'] = mbp_row.get(f'ask_sz_{level:02d}', 0)
            elif tbbo_row is not None:
                row = {
                    'timestamp': ts,
                    'bid_px': tbbo_row['bid_px_00'],
                    'ask_px': tbbo_row['ask_px_00'],
                    'bid_sz': tbbo_row['bid_sz_00'],
                    'ask_sz': tbbo_row['ask_sz_00'],
                    'spread': tbbo_row['ask_px_00'] - tbbo_row['bid_px_00'],
                    'mid_price': (tbbo_row['bid_px_00'] + tbbo_row['ask_px_00']) / 2.0,
                }
                # Only top level for TBBO
                for level in range(1, 10):
                    row[f'bid_px_{level}'] = np.nan
                    row[f'ask_px_{level}'] = np.nan
                    row[f'bid_sz_{level}'] = 0
                    row[f'ask_sz_{level}'] = 0
            else:
                continue
            
            result_rows.append(row)
        
        timeline_df = pd.DataFrame(result_rows)
        print(f"  Timeline created: {len(timeline_df)} rows")
        return timeline_df
    
    def get_ohlcv_for_timestamp(self, timestamp: datetime) -> Optional[pd.Series]:
        """
        Get the latest OHLCV bar that closed before or at the given timestamp.
        Returns None if no bar available.
        """
        if self.ohlcv_df is None:
            self.load_ohlcv()
        
        ohlcv = self.filter_trading_day(self.ohlcv_df, 'ts_event')
        
        # Find the latest bar where ts_event <= timestamp
        mask = ohlcv['ts_event'] <= timestamp
        if not mask.any():
            return None
        
        latest_bar = ohlcv[mask].iloc[-1]
        return latest_bar
    
    def get_ohlcv_history(self, timestamp: datetime, lookback_bars: int = 100) -> pd.DataFrame:
        """
        Get OHLCV history up to the given timestamp.
        Returns DataFrame with at most lookback_bars rows.
        """
        if self.ohlcv_df is None:
            self.load_ohlcv()
        
        ohlcv = self.filter_trading_day(self.ohlcv_df, 'ts_event')
        
        # Get all bars where ts_event <= timestamp
        mask = ohlcv['ts_event'] <= timestamp
        history = ohlcv[mask].tail(lookback_bars)
        return history
