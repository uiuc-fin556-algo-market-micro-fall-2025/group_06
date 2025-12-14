/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/DataTypes.h
*     Last Update: 2010/09/30 13:55:14
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2009 - 2010.
*     All rights reserved.
*
*     This software is part of Licensed material, which is the property of RCM-X ("Company"),
*     and constitutes Confidential Information of the Company.
*     Unauthorized use, modification, duplication or distribution is strictly prohibited by Federal law.
*     No title to or ownership of this software is hereby transferred.
*
*     The software is provided "as is", and in no event shall the Company or any of its affiliates or successors be liable for any
*     damages, including any lost profits or other incidental or consequential damages relating to the use of this software.
*     The Company makes no representations or warranties, express or implied, with regards to this software.
==================================================================================*/

#pragma once

#ifndef _STRATEGY_STUDIO_LIB_DATATYPES_H_
#define _STRATEGY_STUDIO_LIB_DATATYPES_H_

#include "PreprocessorOptions.h"
#include <FlashProtocol/FlashTypes.h>
#include <Utilities/StdTypes.h>
#include <MarketModels/CurrencyPair.h>
#include <MarketModels/CurrencyTypes.h>
#include <Utilities/TimeType.h>
#include <MarketModels/Bond.h>
#include <MarketModels/ValueType.h>
#include <MarketModels/Instrument.h>
#include <MarketModels/Quote.h>
#include <MarketModels/NBBOQuote.h>
#include <MarketModels/QuoteSide.h>
#include <MarketModels/ConditionCodes.h>
#include <MarketModels/Trade.h>
#include <MarketModels/TradeSide.h>
#include <MarketModels/AccountFees.h>
#include <MarketModels/MarketCenterFees.h>
#include <MarketModels/CorporateAction.h>
#include <MarketModels/Stock.h>
#include <MarketModels/Future.h>
#include <MarketModels/Option.h>
#include <MarketModels/OrderTypes.h>
#include <MarketModels/IOrderBook.h>
#include <MarketModels/IAggrOrderBook.h>
#include <time.h>
#include <vector>
#include <Utilities/utils.h>
#include <Utilities/QuickLogger.h>
#include <utility>

#include <bitset>

// ND 4-13-11: Had problems with the below.
//#if defined(WIN32) || defined(WIN64)
//#ifndef _WIN32_WINDOWS
//#define _WIN32_WINDOWS 0x0501
//#endif
//#endif

// defines priority for events (lower gets triggered first)
#define CLOSURE_EVENT_PRIORITY                      1
#define SCHED_TIMED_EVENT_PRIORITY                  3
#define CURRENCY_RATE_EVENT_PRIORITY                4
#define FILL_SIMULATOR_EVENT_PRIORITY               5
#define GENERAL_CONTROL_EVENT_PRIORITY              10
#define BROADCAST_CONTROL_EVENT_PRIORITY            11
#define ORDER_UPDATE_EVENT_PRIORITY                 12
#define MARKET_DATA_EVENT_PRIORITY                  15
#define MARKET_DATA_DEPTH_EVENT_PRIORITY            16
#define MARKET_DATA_DIRECT_EVENT_PRIORITY           17
#define MARKET_DATA_CONSOLIDATED_EVENT_PRIORITY     18
#define MARKET_DATA_TOP_EVENT_PRIORITY              19
#define SENTIMENT_EVENT_PRIORITY                    20
#define NEWS_EVENT_PRIORITY                         25
#define BAR_DATA_EVENT_PRIORITY                     30
#define TOPIC_SUBSCRIPTION_EVENT_PRIORITY           40

// Helper macros for subscriptions
#define MIN_STRIKE 0
#define MAX_STRIKE 999999
#define MIN_EXPIRY boost::gregorian::date(1970, boost::gregorian::Jan, 1)
#define MAX_EXPIRY boost::gregorian::date(2099, boost::gregorian::Jan, 1)

namespace RCM {
namespace StrategyStudio {

class Event;

// MarketModels object
using MarketModels::CorporateAction;
using MarketModels::CorporateActionType;
using MarketModels::CurrencyType;
using MarketModels::MarketCenterID;
using MarketModels::Instrument;
using MarketModels::InstrumentType;
using MarketModels::Stock;
using MarketModels::Future;
using MarketModels::Option;
using MarketModels::SymbolTag;
using MarketModels::Quote;
using MarketModels::NBBOQuote;
using MarketModels::QuoteSide;
using MarketModels::QuoteCondition;
using MarketModels::FeedType;
using MarketModels::IOrderBook;
using MarketModels::IAggrOrderBook;
using MarketModels::InstrumentID;
using MarketModels::INSTRUMENT_TYPE_EQUITY;
using MarketModels::INSTRUMENT_TYPE_FUTURE;
using MarketModels::INSTRUMENT_TYPE_OPTION;
using MarketModels::INSTRUMENT_TYPE_FX;
using MarketModels::INSTRUMENT_TYPE_BOND;
using MarketModels::INSTRUMENT_TYPE_INDEX;
using MarketModels::INSTRUMENT_TYPE_FUND;
using MarketModels::INSTRUMENT_TYPE_UNKNOWN;
using MarketModels::ValueType;
using MarketModels::QuoteCondition;
using MarketModels::TradeCondition;
using MarketModels::Trade;
using MarketModels::TradeSide;
using MarketModels::AccountFees;
using MarketModels::MarketCenterFees;
using MarketModels::DeliveryType;
using MarketModels::DELIVERY_TYPE_UNKNOWN;
using MarketModels::DELIVERY_TYPE_CASH;
using MarketModels::DELIVERY_TYPE_PHYSICAL;
using MarketModels::OptionType;
using MarketModels::OPTION_TYPE_UNKNOWN;
using MarketModels::OPTION_TYPE_CALL;
using MarketModels::OPTION_TYPE_PUT;
using MarketModels::OPTION_TYPE_BOTH;
using MarketModels::OrderID;
using MarketModels::ExerciseStyle;
using MarketModels::EXERCISE_STYLE_UNKNOWN;
using MarketModels::EXERCISE_STYLE_EUROPEAN;
using MarketModels::EXERCISE_STYLE_AMERICAN;
using MarketModels::MARKET_CENTER_ID_UNKNOWN;
using MarketModels::MARKET_CENTER_ID_AMEX;
using MarketModels::MARKET_CENTER_ID_BATS;
using MarketModels::MARKET_CENTER_ID_BYX;
using MarketModels::MARKET_CENTER_ID_BSE;
using MarketModels::MARKET_CENTER_ID_EDGA;
using MarketModels::MARKET_CENTER_ID_EDGX;
using MarketModels::MARKET_CENTER_ID_NSX;
using MarketModels::MARKET_CENTER_ID_FINRA;
using MarketModels::MARKET_CENTER_ID_ISE;
using MarketModels::MARKET_CENTER_ID_LTSE;
using MarketModels::MARKET_CENTER_ID_MEMX;
using MarketModels::MARKET_CENTER_ID_MIAX;
using MarketModels::MARKET_CENTER_ID_CHICAGO;
using MarketModels::MARKET_CENTER_ID_NYSE;
using MarketModels::MARKET_CENTER_ID_ARCA;
using MarketModels::MARKET_CENTER_ID_NASDAQ;
using MarketModels::MARKET_CENTER_ID_CONSOLIDATED_CTS;
using MarketModels::MARKET_CENTER_ID_CONSOLIDATED_SIP;
using MarketModels::MARKET_CENTER_ID_PHLX;
using MarketModels::MARKET_CENTER_ID_CBOESTOCK;
using MarketModels::MARKET_CENTER_ID_IEX;
using MarketModels::MARKET_CENTER_ID_CME_GLOBEX;
using MarketModels::MARKET_CENTER_ID_ICE_FUTURES;
using MarketModels::MARKET_CENTER_ID_EUREX;
using MarketModels::MARKET_CENTER_ID_CBOE_FUTURES;
using MarketModels::MARKET_CENTER_ID_ASX;
using MarketModels::MARKET_CENTER_ID_CFFEX;
using MarketModels::MARKET_CENTER_ID_DALIAN_FUTURES;
using MarketModels::MARKET_CENTER_ID_EURONEXT_FUTURES;
using MarketModels::MARKET_CENTER_ID_HKEX_FUTURES;
using MarketModels::MARKET_CENTER_ID_KRX_FUTURES;
using MarketModels::MARKET_CENTER_ID_LME;
using MarketModels::MARKET_CENTER_ID_NODAL;
using MarketModels::MARKET_CENTER_ID_OSAKA;
using MarketModels::MARKET_CENTER_ID_SHANGHAI_FUTURES;
using MarketModels::MARKET_CENTER_ID_ZHENGZHOU_FUTURES;
using MarketModels::MARKET_CENTER_ID_MONTREAL;
using MarketModels::MARKET_CENTER_ID_NASDAQ_FUTURES;
using MarketModels::MARKET_CENTER_ID_TOCOM;
using MarketModels::MARKET_CENTER_ID_SGX;
using MarketModels::MARKET_CENTER_ID_BOND_BTEC;
using MarketModels::MARKET_CENTER_ID_BOND_ESPD;
using MarketModels::MARKET_CENTER_ID_AMEX_OPTIONS;
using MarketModels::MARKET_CENTER_ID_ARCA_OPTIONS;
using MarketModels::MARKET_CENTER_ID_BATS_OPTIONS;
using MarketModels::MARKET_CENTER_ID_BOX_OPTIONS;
using MarketModels::MARKET_CENTER_ID_CBOE_OPTIONS;
using MarketModels::MARKET_CENTER_ID_C2_OPTIONS;
using MarketModels::MARKET_CENTER_ID_EMERALD_OPTIONS;
using MarketModels::MARKET_CENTER_ID_ISE_OPTIONS;
using MarketModels::MARKET_CENTER_ID_OPRA_OPTIONS;
using MarketModels::MARKET_CENTER_ID_NASDAQ_OPTIONS;
using MarketModels::MARKET_CENTER_ID_PHLX_OPTIONS;
using MarketModels::MARKET_CENTER_ID_GEMINI_OPTIONS;
using MarketModels::MARKET_CENTER_ID_MIAMI_OPTIONS;
using MarketModels::MARKET_CENTER_ID_OMX_BX_OPTIONS;
using MarketModels::MARKET_CENTER_ID_EDGX_OPTIONS;
using MarketModels::MARKET_CENTER_ID_MERCURY_OPTIONS;
using MarketModels::MARKET_CENTER_ID_PEARL_OPTIONS;
using MarketModels::MARKET_CENTER_ID_SMART_ROUTER;
using MarketModels::MARKET_CENTER_ID_ATS_BIDS;
using MarketModels::MARKET_CENTER_ID_ATS_CBX;
using MarketModels::MARKET_CENTER_ID_ATS_CRSF;
using MarketModels::MARKET_CENTER_ID_ATS_CSLP;
using MarketModels::MARKET_CENTER_ID_ATS_KLINK;
using MarketModels::MARKET_CENTER_ID_ATS_KMATCH;
using MarketModels::MARKET_CENTER_ID_ATS_LAVA;
using MarketModels::MARKET_CENTER_ID_ATS_LEVEL;
using MarketModels::MARKET_CENTER_ID_ATS_LX;
using MarketModels::MARKET_CENTER_ID_ATS_MILM;
using MarketModels::MARKET_CENTER_ID_ATS_MLXN;
using MarketModels::MARKET_CENTER_ID_ATS_POSIT;
using MarketModels::MARKET_CENTER_ID_ATS_SIGMA_X;
using MarketModels::MARKET_CENTER_ID_ATS_VORTX;
using MarketModels::MARKET_CENTER_ID_ATS_CDEL;
using MarketModels::MARKET_CENTER_ID_ATS_DBSX;
using MarketModels::MARKET_CENTER_ID_ATS_GTCO;
using MarketModels::MARKET_CENTER_ID_ATS_JPMX;
using MarketModels::MARKET_CENTER_ID_ATS_PDQ;
using MarketModels::MARKET_CENTER_ID_ATS_UBSA;
using MarketModels::MARKET_CENTER_ID_SMART_ROUTER_OPTIONS;
using MarketModels::MARKET_CENTER_ID_FX_CURRENEX;
using MarketModels::MARKET_CENTER_ID_FX_HOTSPOT;
using MarketModels::MARKET_CENTER_ID_FX_EBS;
using MarketModels::MARKET_CENTER_ID_FX_REUTERS;
using MarketModels::MARKET_CENTER_ID_FX_FXALL;
using MarketModels::MARKET_CENTER_ID_FX_LMAX;
using MarketModels::MARKET_CENTER_ID_FX_OTC;
using MarketModels::MARKET_CENTER_ID_BOVESPA;
using MarketModels::MARKET_CENTER_ID_LSE;
using MarketModels::MARKET_CENTER_ID_EU_XETR;
using MarketModels::MARKET_CENTER_ID_EU_XPAR;
using MarketModels::MARKET_CENTER_ID_EU_XAMS;
using MarketModels::MARKET_CENTER_ID_EU_XBRU;
using MarketModels::MARKET_CENTER_ID_EU_XLIS;
using MarketModels::MARKET_CENTER_ID_EU_MTAA;
using MarketModels::MARKET_CENTER_ID_EU_XSWX;
using MarketModels::MARKET_CENTER_ID_EU_XVTX;
using MarketModels::MARKET_CENTER_ID_EU_XMCE;
using MarketModels::MARKET_CENTER_ID_EU_XSTO;
using MarketModels::MARKET_CENTER_ID_EU_XHEL;
using MarketModels::MARKET_CENTER_ID_EU_XCSE;
using MarketModels::MARKET_CENTER_ID_EU_TRQX;
using MarketModels::MARKET_CENTER_ID_EU_CHIX;
using MarketModels::MARKET_CENTER_ID_EU_BATE;
using MarketModels::MARKET_CENTER_ID_AP_XIDX;
using MarketModels::MARKET_CENTER_ID_AP_XKLS;
using MarketModels::MARKET_CENTER_ID_AP_XSES;
using MarketModels::FEED_TYPE_CONSOLIDATED;
using MarketModels::FEED_TYPE_DIRECT;
using MarketModels::FEED_TYPE_DEPTH;
using MarketModels::FEED_TYPE_TOP;
using MarketModels::FEED_TYPE_CONSOLIDATED;
using MarketModels::FEED_TYPE_DIRECT;
using MarketModels::FEED_TYPE_DEPTH;
using MarketModels::TRADE_SIDE_BUY;
using MarketModels::TRADE_SIDE_SELL;
using MarketModels::TRADE_SIDE_UNKNOWN;
using MarketModels::VALUE_TYPE_UNKNOWN;
using MarketModels::VALUE_TYPE_INT;
using MarketModels::VALUE_TYPE_DOUBLE;
using MarketModels::VALUE_TYPE_CHAR;
using MarketModels::VALUE_TYPE_STRING;
using MarketModels::VALUE_TYPE_BOOL;

using FlashProtocol::StrategyRunMode;
using FlashProtocol::CommandType;
using FlashProtocol::CommandTarget;
using FlashProtocol::StrategyState;

using FlashProtocol::STRATEGY_RUN_MODE_UNKNOWN;
using FlashProtocol::STRATEGY_RUN_MODE_BACKTESTING;
using FlashProtocol::STRATEGY_RUN_MODE_LIVE_SIMULATION;
using FlashProtocol::STRATEGY_RUN_MODE_LIVE_PRODUCTION;
using FlashProtocol::STRATEGY_RUN_MODE_LOCAL_DEVELOPMENT;

using FlashProtocol::STRATEGY_STATE_UNKNOWN;
using FlashProtocol::STRATEGY_STATE_INIT;
using FlashProtocol::STRATEGY_STATE_READY;
using FlashProtocol::STRATEGY_STATE_RUNNING;
using FlashProtocol::STRATEGY_STATE_SUSPENDED;
using FlashProtocol::STRATEGY_STATE_STOPPED;
using FlashProtocol::STRATEGY_STATE_TERMINATED;
using FlashProtocol::STRATEGY_STATE_BACKTEST_FINISHED;

using FlashProtocol::TARGET_PROCESSOR;
using FlashProtocol::TARGET_STRATEGY_GROUP;
using FlashProtocol::TARGET_STRATEGY_INSTANCE;
using FlashProtocol::TARGET_STRATEGY_TYPE;
using FlashProtocol::TARGET_UNKNOWN;

using FlashProtocol::InputType;
using FlashProtocol::INPUT_TYPE_NO_INPUT;
using FlashProtocol::INPUT_TYPE_INT_INPUT;
using FlashProtocol::INPUT_TYPE_DOUBLE_INPUT;
using FlashProtocol::INPUT_TYPE_STRING_INPUT;
using FlashProtocol::INPUT_TYPE_BOOL_INPUT;

using Utilities::TimeType;
using Utilities::DateType;
using Utilities::TimeDuration;

typedef unsigned Size;
typedef double Price;
typedef double PriceOffset;

typedef unsigned ProcessorID;
typedef StrategyStudioUInt64 StrategyID;
typedef StrategyStudioUInt64 TradeID;

using FlashProtocol::ConnectionId;

typedef std::vector<const Instrument*> InstrumentList;

enum MarketState {
    MARKET_STATE_UNKNOWN = 0,
    MARKET_STATE_PREOPEN = 1,
    MARKET_STATE_OPEN = 2,
    MARKET_STATE_CLOSE = 3
};

enum MarkingMode {
    MARKING_MODE_UNKNOWN,
    MARKING_MODE_MARK_TO_MARKET,
    MARKING_MODE_MARK_TO_MID,
    MARKING_MODE_MARK_TO_LAST,
    MARKING_MODE_MARK_TO_CLOSE
};

enum StrategyLifecycleState {
    STRATEGY_LIFECYCLE_STATE_UNKNOWN = 0,
    STRATEGY_LIFECYCLE_STATE_NORMAL = 1,
    STRATEGY_LIFECYCLE_STATE_NEEDS_APPROVAL = 2,
    STRATEGY_LIFECYCLE_STATE_EXPIRED = 3,
    STRATEGY_LIFECYCLE_STATE_TERMINATED = 4
};

enum TradeActionResult {
    TRADE_ACTION_RESULT_UNKNOWN, // unknown trade action
    TRADE_ACTION_RESULT_SUCCESSFUL, // trade action was successful
    TRADE_ACTION_RESULT_FAILED_OMS, // trade action could not be sent to the execution provider
    TRADE_ACTION_RESULT_FAILED_INVALID_STRATEGY_STATE, // trade action filtered because of the state of the strategy
    TRADE_ACTION_RESULT_FAILED_INVALID_SERVER_STATE, // trade action filtered because of the state of the server
    TRADE_ACTION_RESULT_FAILED_INVALID_MARKET_STATE, // trade action filtered because of the market hours
    TRADE_ACTION_RESULT_REJECT_RISK, // trade action filtered because of risk violation
    TRADE_ACTION_RESULT_FAILED_EXEC_HANDLER_NOT_LOADED // exechandler is not loaded
};

typedef std::pair<std::string, std::string> Param;
typedef std::vector<Param> ParamList;
typedef std::map<std::string, ValueType> ParamsInfo;

/**
 * Market center auction types
 */
enum AuctionType {
    AUCTION_TYPE_UNKNOWN = 0,
    AUCTION_TYPE_OPEN = 1,
    AUCTION_TYPE_MARKET = 2,
    AUCTION_TYPE_HALT = 3,
    AUCTION_TYPE_CLOSE = 4,
    AUCTION_TYPE_NONE = 5,
    AUCTION_TYPE_REGULATORY = 6, //NYSE flags large closing imbalances (>50k shares) at 3:45PM EST. If this occurs, MOC/LOC orders may be placed to counter the imbalance until close.
    AUCTION_TYPE_IPO = 7 // Used in BATS Auction feed
};

/**
 * Instrument status
 */
enum InstrumentStatus {
    INSTRUMENT_STATUS_UNKNOWN = 0,
    INSTRUMENT_STATUS_SHORT_SALE_RESTRICTION_UPDATE = 1,
    INSTRUMENT_STATUS_TRADING_HALT = 2,
    INSTRUMENT_STATUS_RESUME = 3,
    INSTRUMENT_STATUS_QUOTE_RESUME = 4,
    INSTRUMENT_STATUS_OPENING_DELAY = 5,
    INSTRUMENT_STATUS_NO_OPEN_NO_RESUME = 6,
    INSTRUMENT_STATUS_PRICE_INDICATION = 7,
    INSTRUMENT_STATUS_TRADING_RANGE_INDICATION = 8,
    INSTRUMENT_STATUS_IMBALANCE_BUY = 9,
    INSTRUMENT_STATUS_IMBALANCE_SELL = 10,
    INSTRUMENT_STATUS_MARKET_ON_CLOSE_IMBALANCE_BUY = 11,
    INSTRUMENT_STATUS_MARKET_ON_CLOSE_IMBALANCE_SELL = 12,
    INSTRUMENT_STATUS_NO_MARKET_IMBALANCE = 13,
    INSTRUMENT_STATUS_NO_MARKET_ON_CLOSE_IMBALANCE = 14,
    INSTRUMENT_STATUS_LIMIT_UP_LIMIT_DOWN = 15,
    INSTRUMENT_STATUS_EXCHANGE_TRADING_HALT = 16,
    INSTRUMENT_STATUS_EXCHANGE_QUOTE_RESUME = 17,
    INSTRUMENT_STATUS_REQUEST_FOR_CROSS = 18,
    INSTRUMENT_STATUS_REQUEST_FOR_CROSS_DONE = 19,
    INSTRUMENT_STATUS_IMPLIED_ON = 20,
    INSTRUMENT_STATUS_IMPLIED_OFF = 21
};

enum ShortSaleRestrictionIndicator {
    SHORT_SALE_RESTRICTION_UNKNOWN = 0,
    SHORT_SALE_RESTRICTION_ACTIVATED = 1,
    SHORT_SALE_RESTRICTION_IN_EFFECT = 2,
    SHORT_SALE_RESTRICTION_CONTINUED = 3,
    SHORT_SALE_RESTRICTION_DEACTIVATED = 4,
    SHORT_SALE_RESTRICTION_NOT_IN_EFFECT = 5,
    SHORT_SALE_RESTRICTION_NO_UPDATE = 6
};

enum NewsType {
    NEWS_TYPE_CORPORATE,
    NEWS_TYPE_GOVERNMENT,
    NEWS_TYPE_SPECIFIC_ENTITY
};

enum SentimentType {
    SENTIMENT_TYPE_TICKER,
    SENTIMENT_TYPE_ETF,
    SENTIMENT_TYPE_INDEX,
    SENTIMENT_TYPE_FOREX,
    SENTIMENT_TYPE_FUTURE,
    SENTIMENT_TYPE_CRYPTO,
    SENTIMENT_TYPE_STOCK_ACTIVITY,
    SENTIMENT_TYPE_ETF_ACTIVITY,
    SENTIMENT_TYPE_FOREX_ACTIVITY,
    SENTIMENT_TYPE_FUTURE_ACTIVITY,
    SENTIMENT_TYPE_CRYPTO_ACTIVITY
};

enum SentimentSource {
    SENTIMENT_SOURCE_TWITTER,
    SENTIMENT_SOURCE_STOCKTWITS
};

/**
 * Server enum for distinguishing various types on server
 */
enum ServerObjType {
    SERVER_OBJ_TYPE_UNKNOWN = 0,
    SERVER_OBJ_TYPE_TRADING_STRATEGY = 1,
    SERVER_OBJ_TYPE_EXECUTION_ALGO = 2,
    SERVER_OBJ_TYPE_EXEC_HANDLER = 3,
    SERVER_OBJ_TYPE_DATA_ADAPTER = 4,
    SERVER_OBJ_TYPE_PROCESSOR = 5
};

enum TradeState {
    TRADE_STATE_UNKNOWN = -1,
    TRADE_STATE_OPENED = 0,
    TRADE_STATE_ACTIVE = 1,
    TRADE_STATE_CLOSED = 2
};

} // namespace StrategyStudio
} // namespace RCM

#endif
