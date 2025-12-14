#ifdef _WIN32
    #include "stdafx.h"
#endif

#include "SMAReactiveTakerStrategy.h"

#include "FillInfo.h"
#include "AllEventMsg.h"
#include "ExecutionTypes.h"
#include <Utilities/Cast.h>
#include <Utilities/utils.h>

#include <math.h>
#include <iostream>
#include <cassert>

using namespace RCM::StrategyStudio;
using namespace RCM::StrategyStudio::MarketModels;
using namespace RCM::StrategyStudio::Utilities;

using namespace std;

SMAReactiveTakerStrategy::SMAReactiveTakerStrategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName):
    Strategy(strategyID, strategyName, groupName),
    momentum_map_(),    
    position_size_(100),
    debug_(false),
    short_window_size_(9),
    long_window_size_(30)
{
}

SMAReactiveTakerStrategy::~SMAReactiveTakerStrategy()
{
}

void SMAReactiveTakerStrategy::OnResetStrategyState()
{
    momentum_map_.clear();
}

void SMAReactiveTakerStrategy::DefineStrategyParams()
{
    params().CreateParam(CreateStrategyParamArgs("position_size", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, position_size_));
    params().CreateParam(CreateStrategyParamArgs("short_window_size", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, short_window_size_));
    params().CreateParam(CreateStrategyParamArgs("long_window_size", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, long_window_size_));
    params().CreateParam(CreateStrategyParamArgs("debug", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_BOOL, debug_));
}

void SMAReactiveTakerStrategy::DefineStrategyCommands() {
    // Define any commands your strategy needs. Example:
    commands().AddCommand(StrategyCommand(1, "Reprice Existing Orders"));
    commands().AddCommand(StrategyCommand(2, "Cancel All Orders"));
}

void SMAReactiveTakerStrategy::RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate)
{    
    for (SymbolSetConstIter it = symbols_begin(); it != symbols_end(); ++it) {
        eventRegister->RegisterForBars(*it, BAR_TYPE_TIME, 1);
    }
}

void SMAReactiveTakerStrategy::OnBar(const BarEventMsg& msg)
{
    if (debug_) {
        ostringstream str;
        str << msg.instrument().symbol() << ": " << msg.bar();
        logger().LogToClient(LOGLEVEL_DEBUG, str.str().c_str());
    }

    // Check if we're already tracking the momentum object for this instrument; if not, create a new one
    MomentumMapIterator momentum_iter = momentum_map_.find(&msg.instrument());
    if (momentum_iter == momentum_map_.end()) {
        momentum_iter = momentum_map_.insert(make_pair(&msg.instrument(), Momentum(short_window_size_, long_window_size_))).first;
    }

    Momentum& momentum = momentum_iter->second;
    momentum.Update(msg.bar().close());  // Update momentum with the current close price

    if (momentum.FullyInitialized()) {
        // Define buy/sell signal logic based on the new requirements
        double prev_close = momentum.GetPreviousClose();
        double current_close = msg.bar().close();
        double current_sma = momentum.GetSMA();
        double prev_sma = momentum.GetPreviousSMA();

        DesiredPositionSide side = DesiredPositionSide::NEUTRAL; // Default to neutral

        if (current_close > prev_close && prev_close > prev_sma && current_close > current_sma) {
            side = DesiredPositionSide::LONG;  // Buy signal
        } else if (current_close < prev_close && prev_close < prev_sma && current_close < current_sma) {
            side = DesiredPositionSide::SHORT;  // Sell signal
        }

        AdjustPortfolio(&msg.instrument(), position_size_ * side);
    }
}

void SMAReactiveTakerStrategy::OnOrderUpdate(const OrderUpdateEventMsg& msg) {
}

void SMAReactiveTakerStrategy::AdjustPortfolio(const Instrument* instrument, int desired_position)
{
    int trade_size = desired_position - portfolio().position(instrument);

    if (trade_size != 0) {
        if (orders().num_working_orders(instrument) == 0) {
            SendOrder(instrument, trade_size);
        } else {  
            const Order* order = *orders().working_orders_begin(instrument);
            if (order && ((IsBuySide(order->order_side()) && trade_size < 0) || 
                         ((IsSellSide(order->order_side()) && trade_size > 0)))) {
                trade_actions()->SendCancelOrder(order->order_id());
            }
        }
    }
}

void SMAReactiveTakerStrategy::SendOrder(const Instrument* instrument, int trade_size)
{
    if (!instrument->top_quote().ask_side().IsValid() || !instrument->top_quote().ask_side().IsValid()) {
        std::stringstream ss;
        ss << "Skipping trade due to lack of two sided quote"; 
        logger().LogToClient(LOGLEVEL_DEBUG, ss.str());
        return;
     }

    double price = trade_size > 0 ? instrument->top_quote().bid() : instrument->top_quote().ask();

    OrderParams params(*instrument,
                       abs(trade_size),
                       price,
                       MARKET_CENTER_ID_NASDAQ,
                       (trade_size > 0) ? ORDER_SIDE_BUY : ORDER_SIDE_SELL,
                       ORDER_TIF_DAY,
                       ORDER_TYPE_LIMIT);

    std::cout << "SendTradeOrder(): about to send new order for size "
            << trade_size
            << " at $"
            << price
            << " for symbol "
            << instrument->symbol()
            << std::endl;

    trade_actions()->SendNewOrder(params);
}

void SMAReactiveTakerStrategy::RepriceAll()
{
    for (IOrderTracker::WorkingOrdersConstIter ordit = orders().working_orders_begin(); ordit != orders().working_orders_end(); ++ordit) {
        Reprice(*ordit);
    }
}

void SMAReactiveTakerStrategy::Reprice(Order* order)
{
    OrderParams params = order->params();
    params.price = (order->order_side() == ORDER_SIDE_BUY) ? order->instrument()->top_quote().bid() + aggressiveness_ : order->instrument()->top_quote().ask() - aggressiveness_;
    trade_actions()->SendCancelReplaceOrder(order->order_id(), params);
}

void SMAReactiveTakerStrategy::OnStrategyCommand(const StrategyCommandEventMsg& msg)
{
    switch (msg.command_id()) {
        case 1:
            RepriceAll();
            break;
        case 2:
            trade_actions()->SendCancelAll();
            break;
        default:
            logger().LogToClient(LOGLEVEL_DEBUG, "Unknown strategy command received");
            break;
    }
}

void SMAReactiveTakerStrategy::OnParamChanged(StrategyParam& param)
{    
    if (param.param_name() == "position_size") {
        if (!param.Get(&position_size_))
            throw StrategyStudioException("Could not get position_size");
    } else if (param.param_name() == "short_window_size") {
        if (!param.Get(&short_window_size_))
            throw StrategyStudioException("Could not get short_window_size");
    } else if (param.param_name() == "long_window_size") {
        if (!param.Get(&long_window_size_))
            throw StrategyStudioException("Could not get long_window_size");
    } else if (param.param_name() == "debug") {
        if (!param.Get(&debug_))
            throw StrategyStudioException("Could not get debug");
    } 
}
