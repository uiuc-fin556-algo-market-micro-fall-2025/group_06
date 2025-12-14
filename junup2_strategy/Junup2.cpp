#ifdef _WIN32
    #include "stdafx.h"
#endif

#include "Junup2.h"
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

Junup2Strategy::Junup2Strategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName):

    Strategy(strategyID, strategyName, groupName),
    m_rsi_logic_map(),
    m_tradeprice_map(),
    m_instrument_order_id_map(),
    m_rsi_logic(0),
    m_aggressiveness(0),
    m_position_size(100),
    m_debug_on(true),
    m_rsi_period(14),
    m_dia_last_trade_price(0),
    m_average_dia_ratio(0.0),
    m_num_dia_ratio_observations(0)
{
    //this->set_enabled_pre_open_data_flag(true);
    //this->set_enabled_pre_open_trade_flag(true);
    //this->set_enabled_post_close_data_flag(true);
    //this->set_enabled_post_close_trade_flag(true);
}

Junup2Strategy::~Junup2Strategy()

{
}

void Junup2Strategy::OnResetStrategyState() {
    m_rsi_logic_map.clear();
    m_instrument_order_id_map.clear();
    m_rsi_logic = 0;
}

void Junup2Strategy::DefineStrategyParams() {
    CreateStrategyParamArgs arg1("aggressiveness", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_DOUBLE, m_aggressiveness);
    params().CreateParam(arg1);

    CreateStrategyParamArgs arg2("position_size", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, m_position_size);
    params().CreateParam(arg2);

    CreateStrategyParamArgs arg3("rsi_period", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, m_rsi_period);
    params().CreateParam(arg3);

    CreateStrategyParamArgs arg4("debug", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_BOOL, m_debug_on);
    params().CreateParam(arg4);
}

void Junup2Strategy::DefineStrategyCommands() {
    StrategyCommand command1(1, "Reprice Existing Orders");
    commands().AddCommand(command1);

    StrategyCommand command2(2, "Cancel All Orders");
    commands().AddCommand(command2);
}

void Junup2Strategy::RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate) {
    for (SymbolSetConstIter it = symbols_begin(); it != symbols_end(); ++it) {
        eventRegister->RegisterForBars(*it, BAR_TYPE_TIME, 10);
    }
}

void Junup2Strategy::OnTrade(const TradeDataEventMsg& msg) {
    std::cout << "OnTrade(): (" << msg.adapter_time() << "): " << msg.instrument().symbol() << ": " << msg.trade().size() << " @ $" << msg.trade().price();
    if ( msg.instrument().symbol().compare("DIA")!=0) {
        m_tradeprice_map[msg.instrument().symbol()] = msg.trade().price();
    } else {
        m_dia_last_trade_price = msg.trade().price();
    }
    double djia_index = 0;
    for (TradePriceMapIterator iter = m_tradeprice_map.begin(); iter!=m_tradeprice_map.end(); ++iter) {
        djia_index += iter->second;
    }
    if (m_dia_last_trade_price!=0 and m_tradeprice_map.size()==31) {
        double ratio = djia_index / m_dia_last_trade_price;
        double new_average_ratio = ((m_average_dia_ratio*m_num_dia_ratio_observations) + ratio) / (m_num_dia_ratio_observations+1);

        std::cout << "; \t Dumping all symbols; " <<
            "sum is " << djia_index <<
            " DIA last trade price is $" << m_dia_last_trade_price <<
            " ratio is " << ratio <<
            " previous average ratio was " << m_average_dia_ratio <<
            " new average ratio is " << new_average_ratio;
        m_average_dia_ratio = new_average_ratio;
        m_num_dia_ratio_observations++;
    }
    std::cout << std::endl;
}

void Junup2Strategy::OnTopQuote(const QuoteEventMsg& msg) {
    std::cout << "OnTopQuote(): (" << msg.adapter_time() << "): " << msg.instrument().symbol() << ": " <<
        msg.instrument().top_quote().bid_size() << " @ $"<< msg.instrument().top_quote().bid() <<
        msg.instrument().top_quote().ask_size() << " @ $"<< msg.instrument().top_quote().ask() <<
        std::endl;
}



void Junup2Strategy::OnQuote(const QuoteEventMsg& msg) {
    std::cout << "OnQuote(): (" << msg.adapter_time() << "): " << msg.instrument().symbol() << ": " <<
        msg.instrument().top_quote().bid_size() << " @ $"<< msg.instrument().top_quote().bid() <<
        msg.instrument().top_quote().ask_size() << " @ $"<< msg.instrument().top_quote().ask() <<
        std::endl;
}

void Junup2Strategy::OnDepth(const MarketDepthEventMsg& msg) {
    std::cout << "OnDepth(): (" << msg.adapter_time() << "): " << msg.instrument().symbol() << ": " <<
        msg.instrument().top_quote().bid_size() << " @ $"<< msg.instrument().top_quote().bid() <<
        msg.instrument().top_quote().ask_size() << " @ $"<< msg.instrument().top_quote().ask() <<
        std::endl;
}

void Junup2Strategy::OnBar(const BarEventMsg& msg) {
    if (m_debug_on) {
        ostringstream str;
        str << "OnBar: " << msg.instrument().symbol() << ": " << msg.bar();
        logger().LogToClient(LOGLEVEL_DEBUG, str.str().c_str());
    }

    if(msg.bar().close() < .01) return;

    RSILogicMapIterator iter = m_rsi_logic_map.find(&msg.instrument());
    if (iter != m_rsi_logic_map.end()) {
        m_rsi_logic = &iter->second;
    } else {
        // Create new RSILogic object with the strategy's m_rsi_period
        m_rsi_logic = &m_rsi_logic_map.insert(make_pair(&msg.instrument(),RSILogic(m_rsi_period))).first->second;
    }

    // Update the RSI logic with the new bar's close price
    DesiredPositionSide side = m_rsi_logic->Update(msg.bar().close());
    // If the RSI windows are full, proceed with trading logic
    if(m_rsi_logic->FullyInitialized()) {
        // 1. Get current position using 'position_size()'
        //    This is the older function name.
        int current_position = this->portfolio().position(&msg.instrument()); 

        // 2. Calculate target position
        int target_position = m_position_size * side;

        // 3. Calculate difference
        int shares_to_trade = target_position - current_position;

        // 4. Send the order using 'SendNewOrder()'
        //    We must now specify the side and order type with enums.
        if (shares_to_trade != 0) {
            OrderParams params;
	    params.instrument = &msg.instrument();
	    params.order_type = ORDER_TYPE_MARKET;
	    params.price = 0.0;

	    if (shares_to_trade > 0) {
                params.order_side = ORDER_SIDE_BUY;
		params.quantity = shares_to_trade;
	    } else {
                params.order_side = ORDER_SIDE_SELL;
		params.quantity = -shares_to_trade;
	    }
            trade_actions()->SendNewOrder(params);
        }
    }
}

void Junup2Strategy::OnOrderUpdate(const OrderUpdateEventMsg& msg) {
    std::cout << "OnOrderUpdate(): " << msg.update_time() << msg.name() << std::endl;
    if(msg.completes_order()) {
        m_instrument_order_id_map[msg.order().instrument()] = 0;
        std::cout << "OnOrderUpdate(): order is complete; " << std::endl;
    }
}

void Junup2Strategy::OnStrategyCommand(const StrategyCommandEventMsg& msg) {
    switch (msg.command_id()) {
    case 1:
        // RepriceAll(); // This was commented out in the original
        break;
    case 2:
        trade_actions()->SendCancelAll();
        break;
    default:
        logger().LogToClient(LOGLEVEL_DEBUG, "Unknown strategy command received");
        break;
    }
}

void Junup2Strategy::OnParamChanged(StrategyParam& param) {
    if (param.param_name() == "aggressiveness") {                         
        if (!param.Get(&m_aggressiveness))
            throw StrategyStudioException("Could not get m_aggressiveness");
    } else if (param.param_name() == "position_size") {
        if (!param.Get(&m_position_size))
            throw StrategyStudioException("Could not get position size");
    } else if (param.param_name() == "rsi_period") { // Changed from short/long
        if (!param.Get(&m_rsi_period))
            throw StrategyStudioException("Could not get rsi_period");
    } else if (param.param_name() == "debug") {
        if (!param.Get(&m_debug_on))
            throw StrategyStudioException("Could not get debug");
    } 
}

extern "C" {

_STRATEGY_EXPORTS const char* GetType()
{
    return "Junup2Strategy";
}

_STRATEGY_EXPORTS IStrategy* CreateStrategy(const char* strategyType,
                                            unsigned strategyID,
                                            const char* strategyName,
                                            const char* groupName)
{
    if (strcmp(strategyType, GetType()) == 0) {
        return *(new Junup2Strategy(strategyID, strategyName, groupName));
    } else {
        return NULL;
    }
}

_STRATEGY_EXPORTS const char* GetAuthor() {
    return "dlariviere";
}

_STRATEGY_EXPORTS const char* GetAuthorGroup() {
    return "UIUC";
}

_STRATEGY_EXPORTS const char* GetReleaseVersion() {
    return Strategy::release_version();
}

}

