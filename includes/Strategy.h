/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Strategy.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGY_H_
#define _STRATEGY_STUDIO_LIB_STRATEGY_H_

#include "StrategyIncludes.h"

namespace RCM {
namespace StrategyStudio {

class StrategyBase;

/**
 * This Strategy class provides the interface and common functionality for trading strategies within Strategy Studio.
 *
 * All Strategy Studio strategies should derive from this class, which provides event hooks such as OnTrade, OnQuote, and OnDepth
 * for receiving market data, instructions from the Client, and information on trading activity.
 *
 * The class also provides accessors such as portfolio(), orders(), and fills() where you can find information about the
 * state of your positions, your trading activity, and your PnL. It also provides trade_actions(), your starting point
 * for sending out orders.
 *
 */
class Strategy {
public:
    typedef boost::unordered_set<SymbolTag> SymbolSet;
    typedef SymbolSet::iterator SymbolSetIter;
    typedef SymbolSet::const_iterator SymbolSetConstIter;

    typedef boost::unordered_map<SymbolTag, const Instrument*> InstrumentSet;
    typedef InstrumentSet::iterator InstrumentSetIter;
    typedef InstrumentSet::const_iterator InstrumentSetConstIter;

public:
    Strategy(StrategyID strategyID, const std::string& strategyName, const std::string& strategyGroup);
    virtual ~Strategy();
    operator IStrategy*();
    static const char* release_version();

public: /* Overridable hooks for events */
    /**
     * Triggered when request to reset strategy is called
     * Derived strategies should override this for resetting their own state
     */
    virtual void OnResetStrategyState() = 0;

    /**
     * Define any params for use by the strategy
     */
    virtual void DefineStrategyParams() = 0;

    /**
     * Define any strategy commands for use by the strategy
     */
    virtual void DefineStrategyCommands();

    /**
     * Provides an ideal place during strategy initialization to define custom strategy graphs using graphs().series().add(...)
     */
    virtual void DefineStrategyGraphs();

    /**
     * Event registration triggered for derived classes
     */
    virtual void RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate) = 0;

    /**
     * This event triggers whenever trade message arrives from a market data source.
     */
    virtual void OnTrade(const TradeDataEventMsg& msg);

    /**
     * This event triggers whenever aggregate volume at best price changes, based
     * on the best available source of liquidity information for the instrument.
     *
     * If the quote datasource only provides ticks that change the NBBO, top quote will be set to NBBO
     */
    virtual void OnTopQuote(const QuoteEventMsg& msg);

    /**
     * This event triggers whenever a new quote for a market center arrives from a consolidate or direct quote feed,
     * or when the market center's best price from a depth of book feed changes.
     *
     * User can check if quote is from consolidated or direct, or derived from a depth feed. This will not fire if
     * the data source only provides quotes that affect the official NBBO, as this is not enough information to accurately
     * mantain the state of each market center's quote.
     */
    virtual void OnQuote(const QuoteEventMsg& msg);

    /**
     * This event triggers whenever a order book message arrives. This will be the first thing that
     * triggers if an order book entry impacts the exchange's DirectQuote or Strategy Studio's TopQuote calculation.
     */
    virtual void OnDepth(const MarketDepthEventMsg& msg);

    /**
     * This event triggers whenever a market center sends indicative data about its price setting auctions.
     */
    virtual void OnImbalance(const ImbalanceEventMsg& msg);

    /**
     * This event triggers whenever an instrument enters a new workup state
     */
    virtual void OnWorkup(const WorkupEventMsg& msg);

    /**
     * This event triggers whenever a data feed sends alerts about the trading status of an instrument.
     */
    virtual void OnInstrumentAlert(const InstrumentAlertEventMsg& msg);

    /**
     * This event triggers whenever a Limit-Up/Limit-Down price band update arrives from a Consolidated US equity feed.
     */
    virtual void OnPriceBand(const PriceBandEventMsg& msg);

    /**
     * This event triggers whenever a market data feed delivers an update to estimated fair value of an index or ETP IIPV/IIV
     */
    virtual void OnIndex(const IndexEventMsg& msg);

    /**
     * This event triggers whenever a news event arrives
     */
    virtual void OnNews(const NewsEventMsg& msg);

    /**
     * This event triggers whenever a new sentiment message arrives
     */
    virtual void OnSentiment(const SentimentEventMsg& msg);

    /**
     * This event triggers whenever a new sentiment activity message arrives
     */
    virtual void OnSentiment(const SentimentActivityEventMsg& msg);

    /**
     * This event triggers whenever a Bar interval completes for an instrument
     */
    virtual void OnBar(const BarEventMsg& msg);

    /**
     * This event triggers whenever there's an update to options analytics calculations
     */
    virtual void OnOptionsAnalytics(const OptionsAnalyticsEventMsg& msg);

    /**
     * This event triggers when a corporate action occurs for an instrument
     */
    virtual void OnCorporateAction(const CorporateActionEventMsg& msg);

    /**
     * This event contains alerts about the state of the market
     */
    virtual void OnMarketState(const MarketStateEventMsg& msg);

    /**
     * This event triggers whenever new information arrives about a strategy's orders
     */
    virtual void OnOrderUpdate(const OrderUpdateEventMsg& msg);

    /**
     * This event triggers whenever new information arrives about an external order
     */
    virtual void OnExternalFill(const ExternalFillEventMsg &msg);

    /**
     * This event triggers whenever a custom strategy command is sent from the client
     */
    virtual void OnStrategyCommand(const StrategyCommandEventMsg& msg);

    /**
     * This event contains strategy control commands arriving from the Strategy Studio client application (eg Strategy Manager)
     */
    virtual void OnStrategyControl(const StrategyStateControlEventMsg& msg);

    /**
     * This event contains alerts about the status of a market data source
     */
    virtual void OnDataSubscription(const DataSubscriptionEventMsg& msg);

    /**
     * This event contains alerts about the status of the Strategy Server process
     */
    virtual void OnAppStateChange(const AppStateEventMsg& msg);

    /**
     * This event contains timed events requested by the strategy
     */
    virtual void OnScheduledEvent(const ScheduledEventMsg& msg);

    /**
     * This event contains a strategy notification from another part of the system
     */
    virtual void OnStrategyMessage(const StrategyNotifyEventMsg& msg);

    /**
     * This event contains a topic subscription msg
     */
    virtual void OnTopicMessage(const TopicSubscriptionEventMsg& msg);

    /**
     * This event contains external orders submitted by the Strategy Manager or a
     * 3rd party platform.
     *
     * A default implementation of this event callback will forward on order requests
     * automatically if they come from the Strategy Manager. This can be overloaded,
     * and the default implementation is provided inline in Strategy.h.
     *
     * @returns true to acknowledge the order, false to indicate a NACK (ie to disable manual order entry)
     */
    virtual bool OnExternalOrder(ExternalOrderEventMsg& msg);

    /**
     * Triggered before OnParamChanged to allow customized parameter validation; gets called after built in bounds checking on
     * numeric parameter types.
     *
     * Will be called on any attempt to set a parameter, including during strategy initialization when default parameter values
     * are set in the call to CreateParam and when any persisted values are loaded. Will also trigger after OnResetStrategyState
     * to remind the strategy of the current parameter values.
     *
     * @return true to accept a parameter value, false to reject it
     */
    virtual bool OnParamValidate(StrategyParam&) { return true; }

    /**
     * Notifies strategy for every succesfull change in the value of a strategy parameter.
     *
     * Will be called any time a new parameter value passes validation, including during strategy initialization when default parameter values
     * are set in the call to CreateParam and when any persisted values are loaded. Will also trigger after OnResetStrategyState
     * to remind the strategy of the current parameter values.
     */
    virtual void OnParamChanged(StrategyParam& param) = 0;

public: /* Methods for working with strategies */
    /**
     * Retrieve the name of the strategy
     */
    const std::string& name() const;

    /**
     * Retrieve the type of the strategy
     */
    const std::string& type() const;

    /**
     * Retrieve state of the strategy
     */
    StrategyState state() const;

    /**
     * Retrieve the user who owns the instance
     */
    const std::string& instance_owner() const;

    /**
     * Retrieve account owner id
     */
    const std::string& account_owner() const;

    /**
     * Retrieve the name of the user who authored the strategy
     */
    const std::string& author() const;

    /**
     * Retrieve the name of the group
     */
    const std::string& group() const;

    /**
     * Retrieve strategy params
     */
    StrategyParamCollection& params();
    const StrategyParamCollection& params() const;

    /**
     * Retrieve strategy commands object
     */
    StrategyCommands& commands();
    const StrategyCommands& commands() const;

    /**
     * Accesses the strategy's custom graphing interface
     */
    IStrategyGraphs& graphs();
    const IStrategyGraphs& graphs() const;

    /**
     * Returns the number of instruments the Strategy Manager requested subscription for
     */
    size_t SymbolCount() const;

    /**
     * Returns iterator to the beginning of the symbols the Strategy Manager requested subscription for
     */
    SymbolSetConstIter symbols_begin() const;

    /**
     * Returns iterator to the end of the symbols the Strategy Manager requested subscription for
     */
    SymbolSetConstIter symbols_end() const;

    /**
     * Returns iterator to the beginning of the instruments the strategy has subscribed to
     */
    InstrumentSetConstIter instrument_begin() const;

    /**
     * Returns iterator to the end of the instruments the strategy has subscribed to
     */
    InstrumentSetConstIter instrument_end() const;

    /**
     * If strategy is subscribed to the specified symbol, return iterator to the element of the intruments map,
     * otherwise returns instrument_end().
     */
    InstrumentSetConstIter instrument_find(const std::string& symbol) const;

    /**
     * Retrieve the portfolio tracker
     */
    const IPortfolioTracker& portfolio() const;

    /**
     * Retrieve the order tracker
     */
    const IOrderTracker& orders() const;

    /**
     * Retrieve the fill tracker
     */
    const IFillTracker& fills() const;

    /**
     * Retrieve the trade tracker
     */
    const ITradeTracker& trades() const;

    /**
     * Retrieve the risk profile of the strategy
     */
    const IRiskProfile& risk_profile() const;

    /**
     * Retrieves the account fees
     */
    const AccountFees& account_fees() const;

    /**
     * Retrieves an indicative currency rate based on combination of 
     * data from server's static rates file and FX instrument subscriptions
     * made on the Strategy's event processor. Cross rates may be chained from
     * multiple quoted rates. Note missing rates presently default to 1.0.
     */
    double GetCurrencyRate(CurrencyType base, CurrencyType quote);

    /**
     * Accesses the strategy's logger
     */
    const StrategyLogger& logger() const;
    StrategyLogger& logger();

    /**
     * Accesses the trade actions allowed to the strategy
     */
    StrategyTradeActions* trade_actions() const;

    /**
     * Check the market state if we are enabled for trading
     */
    bool CheckEnabledTradingForMarketState() const;

    /**
     * Returns a flag saying whether this strategy is enabled for post-close data
     */
    bool enabled_post_close_data_flag() const;

    /**
     * Sets a flag specifying whether this strategy is enabled for post-close data
     */
    void set_enabled_post_close_data_flag(bool flag);

    /**
     * Returns a flag saying whether this strategy can send trades post-close
     */
    bool enabled_post_close_trade_flag() const;

    /**
     * Sets a flag specifying whether this strategy can send trades post-close
     */
    void set_enabled_post_close_trade_flag(bool flag);

    /**
     * Returns a flag saying whether this strategy is enabled for pre-open data
     */
    bool enabled_pre_open_data_flag() const;

    /**
     * Sets a flag specifying whether this strategy is enabled for pre-open data
     */
    void set_enabled_pre_open_data_flag(bool flag);

    /**
     * Returns a flag saying whether this strategy can send trades pre-open
     */
    bool enabled_pre_open_trade_flag() const;

    /**
     * Sets a flag specifying whether this strategy can send trades pre-open
     */
    void set_enabled_pre_open_trade_flag(bool flag);

    /**
     * Returns a flag saying whether this strategy will cancel working orders upon stopping
     */
    bool enabled_cancel_on_stop_flag() const;

    /**
     * Sets a flag specifying whether this strategy will cancel working orders upon stopping
     * Note that if this option is disabled it is advisable to expose Cancel All through
     * some other means, such as through a strategy command.
     */
    void set_enabled_cancel_on_stop_flag(bool flag);

    /**
     * Retrieve the run mode for this strategy
     */
    StrategyRunMode run_mode() const;

    /**
     * Get interface for doing runtime event register
     */
    StrategyRuntimeEventRegister* runtime_event_register();

    /**
     * Returns the ID of the event Processor the strategy is running on
     */
    ProcessorID processor_id() const;

protected: /* Helper functions usable by strategies */
    /**
     * Programatically pause a strategy
     *
     * Note if the pause request succeeds, OnStrategyControl will be invoked before this function returns;
     * it does not queue up an independent event.
     */
    bool Pause();

    /**
     * Programatically stop a strategy
     *
     * Note if the stop request succeeds, OnStrategyControl will be invoked before this function returns;
     * it does not queue up an independent event.
     */
    bool Stop();

    /**
     * Programatically trigger a strategy comand
     *
     * Note OnStrategyCommand will be invoked before this function returns;
     * it does not queue up an independent event.
     */
    void TriggerStrategyCommand(StrategyStudioUInt32 id, const boost::any& input = boost::any());

private: /* Meant to be called by server */
    StrategyBase* impl_;
};

/**
 * Default handling for external order submissions
 */
inline bool Strategy::OnExternalOrder(ExternalOrderEventMsg& msg)
{
    bool result = false;

    if (msg.source() == ExternalOrderEventMsg::EXTERNAL_ORDER_SOURCE_STRATEGY_MANAGER) {
        switch (msg.order_request_type()) {
            case FlashProtocol::ORDER_SUBMIT_NEW: {
                TradeActionResult taResult = trade_actions()->SendNewOrder(msg.params(), msg.account().empty() ? account_owner() : msg.account());
                if (taResult == TRADE_ACTION_RESULT_SUCCESSFUL) {
                    result = true;
                } else {
                    std::string message = "Client NEW Order with ID = " + msg.client_order_id() + ", Rejected with status = " + DataTypesUtils::TradeActionResultToString(taResult);
                    logger().LogToClient(Utilities::LOGLEVEL_ERROR, message);
                }
                break;
            }
            case FlashProtocol::ORDER_CANCEL: {
                TradeActionResult taResult = trade_actions()->SendCancelOrder(msg.params().order_id);
                if (taResult == TRADE_ACTION_RESULT_SUCCESSFUL) {
                    result = true;
                } else {
                    std::string message = "Client CANCEL Order with ID = " + msg.client_order_id() + ", Rejected with status = " + DataTypesUtils::TradeActionResultToString(taResult);
                    logger().LogToClient(Utilities::LOGLEVEL_ERROR, message);
                }
                break;
            }
            case FlashProtocol::ORDER_REPLACE: {
                TradeActionResult taResult = trade_actions()->SendCancelReplaceOrder(msg.params().order_id, msg.params());
                if (taResult == TRADE_ACTION_RESULT_SUCCESSFUL) {
                    result = true;
                } else {
                    std::string message = "Client CANCEL REPLACE Order with ID = " + msg.client_order_id() + ", Rejected with status = " + DataTypesUtils::TradeActionResultToString(taResult);
                    logger().LogToClient(Utilities::LOGLEVEL_ERROR, message);
                }
                break;
            }
            default:
                break;
        }
    }
    return result;
}

} // namespace StrategyStudio
} // namespace RCM

#endif
