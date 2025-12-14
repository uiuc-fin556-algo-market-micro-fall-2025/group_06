/*===================================================================================================================================
*     Copyright (c) RCM-X, 2009 - 2011.                                                                                *
*     All rights reserved.                                                                                                          *
*                                                                                                                                   *
*     This software is part of Licensed material, which is the property of RCM-X ("Company"),                          *
*     Unauthorized use, modification, duplication or distribution is strictly prohibited by Federal law.                            *
*                                                                                                                                   *
*     No title to or ownership of this software is hereby transferred.                                                              *
*                                                                                                                                   *
*     The software is provided "as is", and in no event shall the Company or any of its affiliates or successors be liable for any  *
*     damages, including any lost profits or other incidental or consequential damages relating to the use of this software.        *
*     The Company makes no representations or warranties, express or implied, with regards to this software.                        *
====================================================================================================================================*/

#pragma once

#ifndef _STRATEGY_STUDIO_FLASH_PROTOCOL_FLASH_TYPES_H_
#define _STRATEGY_STUDIO_FLASH_PROTOCOL_FLASH_TYPES_H_

#include <Utilities/StdTypes.h>

#include <boost/shared_ptr.hpp>

#include <string>
#include <utility>

namespace RCM {
namespace StrategyStudio {
namespace FlashProtocol {

typedef unsigned char byte;
typedef byte* FlashBuffer;
typedef uint16_t ConnectionId;
typedef std::pair<boost::shared_ptr<byte[]>, std::size_t> SharedBuffer;


class FlashTypes {
public:
    static const StrategyStudioUInt16 UNKNOWN = 0;
    static const StrategyStudioUInt16 LOGIN_REQUEST = 1;
    static const StrategyStudioUInt16 LOGIN_RESPONSE = 2;
    static const StrategyStudioUInt16 LOGOUT_REQUEST = 3;

    static const StrategyStudioUInt16 SERVER_COMMAND_REQUEST = 4;
    static const StrategyStudioUInt16 SERVER_COMMAND_RESPONSE = 5;

    static const StrategyStudioUInt16 STRATEGY_INIT_NOTIFY = 6;
    static const StrategyStudioUInt16 STRATEGY_INIT_REQUEST = 7;
    static const StrategyStudioUInt16 STRATEGY_INIT_RESPONSE = 8;

    static const StrategyStudioUInt16 INFO_NOTIFY = 9;
    static const StrategyStudioUInt16 INSTRUMENT_DATA_NOTIFY = 10;

    static const StrategyStudioUInt16 USER_REQUEST = 11;
    static const StrategyStudioUInt16 USER_RESPONSE = 12;
    static const StrategyStudioUInt16 USER_NOTIFY = 13;

    static const StrategyStudioUInt16 ACCOUNT_REQUEST = 14;
    static const StrategyStudioUInt16 ACCOUNT_RESPONSE = 15;
    static const StrategyStudioUInt16 ACCOUNT_NOTIFY = 16;

    static const StrategyStudioUInt16 GROUP_REQUEST = 17;
    static const StrategyStudioUInt16 GROUP_RESPONSE = 18;
    static const StrategyStudioUInt16 GROUP_NOTIFY = 19;

    static const StrategyStudioUInt16 EXCHANGE_FEES_REQUEST = 20;
    static const StrategyStudioUInt16 EXCHANGE_FEES_RESPONSE = 21;
    static const StrategyStudioUInt16 EXCHANGE_FEES_NOTIFY = 22;

    static const StrategyStudioUInt16 SIM_PARAMS_REQUEST = 23;
    static const StrategyStudioUInt16 SIM_PARAMS_RESPONSE = 24;
    static const StrategyStudioUInt16 SIM_PARAMS_NOTIFY = 25;

    static const StrategyStudioUInt16 STRATEGY_RESULTS_REQUEST = 26;
    static const StrategyStudioUInt16 STRATEGY_RESULTS_RESPONSE = 27;
    static const StrategyStudioUInt16 STRATEGY_RESULTS_NOTIFY = 28;

    static const StrategyStudioUInt16 STRATEGY_PARAMS_REQUEST = 29;
    static const StrategyStudioUInt16 STRATEGY_PARAMS_RESPONSE = 30;
    static const StrategyStudioUInt16 STRATEGY_PARAMS_NOTIFY = 31;

    static const StrategyStudioUInt16 STRATEGY_STATECHANGE_REQUEST = 32;
    static const StrategyStudioUInt16 STRATEGY_STATECHANGE_RESPONSE = 33;
    static const StrategyStudioUInt16 STRATEGY_STATECHANGE_NOTIFY = 34;

    static const StrategyStudioUInt16 STRATEGY_FILL_NOTIFY = 35;
    static const StrategyStudioUInt16 STRATEGY_ORDER_NOTIFY = 36;
    static const StrategyStudioUInt16 STRATEGY_POSITION_NOTIFY = 37;
    static const StrategyStudioUInt16 STRATEGY_PROFITLOSS_NOTIFY = 38;
    static const StrategyStudioUInt16 STRATEGY_PNL_HISTORY_NOTIFY = 39;
    static const StrategyStudioUInt16 STRATEGY_TRADE_NOTIFY = 40;
    static const StrategyStudioUInt16 STRATEGY_DAILY_REPORT_NOTIFY = 41;

    static const StrategyStudioUInt16 RESEND_BEGIN_NOTIFY = 42;
    static const StrategyStudioUInt16 RESEND_END_NOTIFY = 43;
    static const StrategyStudioUInt16 STRATEGY_TYPE_NOTIFY = 44;
    static const StrategyStudioUInt16 SYMBOL_PRICE_NOTIFY = 45;
    static const StrategyStudioUInt16 STRATEGY_EXTERNAL_FILL_NOTIFY = 46;

    static const StrategyStudioUInt16 HEARTBEAT = 47;

    static const StrategyStudioUInt16 ACCOUNT_FEES_REQUEST = 48;
    static const StrategyStudioUInt16 ACCOUNT_FEES_RESPONSE = 49;
    static const StrategyStudioUInt16 ACCOUNT_FEES_NOTIFY = 50;

    static const StrategyStudioUInt16 STRATEGY_COMMAND_REQUEST = 51;
    static const StrategyStudioUInt16 STRATEGY_COMMAND_RESPONSE = 52;

    static const StrategyStudioUInt16 STRATEGY_ORDER_REQUEST = 53;
    static const StrategyStudioUInt16 STRATEGY_ORDER_RESPONSE = 54;

    static const StrategyStudioUInt16 STRATEGY_RESET_REQUEST = 55;
    static const StrategyStudioUInt16 STRATEGY_RESET_RESPONSE = 56;
    static const StrategyStudioUInt16 STRATEGY_RESET_NOTIFY = 57;

    static const StrategyStudioUInt16 CORPORATE_ACTION_NOTIFY = 58;

    static const StrategyStudioUInt16 STRATEGY_GRAPH_SERIES_INIT_NOTIFY = 59;
    static const StrategyStudioUInt16 STRATEGY_GRAPH_SERIES_ADD_NOTIFY = 60;
    static const StrategyStudioUInt16 STRATEGY_GRAPH_SERIES_CLEAR_NOTIFY = 61;

    static const StrategyStudioUInt16 STRATEGY_COMMAND_NOTIFY = 62;

    static const StrategyStudioUInt16 EXECUTION_PROVIDER_NOTIFY = 63;

    static const StrategyStudioUInt16 TICK_SIZE_REGIME_NOTIFY = 64;
    static const StrategyStudioUInt16 TICK_SIZE_REGIME_REQUEST = 65;
    static const StrategyStudioUInt16 TICK_SIZE_REGIME_RESPONSE = 66;

    static const StrategyStudioUInt16 TICK_SIZE_MULTIPLIER_NOTIFY = 67;
    static const StrategyStudioUInt16 TICK_SIZE_MULTIPLIER_REQUEST = 68;
    static const StrategyStudioUInt16 TICK_SIZE_MULTIPLIER_RESPONSE = 69;

    static std::string ToString(StrategyStudioUInt16 flashType)
    {
        std::string m_text;

        switch (flashType)
        {
            case FlashTypes::LOGIN_REQUEST: m_text = "LOGIN_REQUEST"; break;
            case FlashTypes::LOGIN_RESPONSE: m_text = "LOGIN_RESPONSE"; break;
            case FlashTypes::LOGOUT_REQUEST: m_text = "LOGOUT_REQUEST"; break;

            case FlashTypes::SERVER_COMMAND_REQUEST: m_text = "SERVER_COMMAND_REQUEST"; break;
            case FlashTypes::SERVER_COMMAND_RESPONSE: m_text = "SERVER_COMMAND_RESPONSE"; break;

            case FlashTypes::STRATEGY_INIT_NOTIFY: m_text = "STRATEGY_INIT_NOTIFY"; break;
            case FlashTypes::STRATEGY_INIT_REQUEST: m_text = "STRATEGY_INIT_REQUEST"; break;
            case FlashTypes::STRATEGY_INIT_RESPONSE: m_text = "STRATEGY_INIT_RESPONSE"; break;

            case FlashTypes::INFO_NOTIFY: m_text = "INFO_NOTIFY"; break;
            case FlashTypes::INSTRUMENT_DATA_NOTIFY: m_text = "INSTRUMENT_DATA_NOTIFY"; break;

            case FlashTypes::USER_REQUEST: m_text = "USER_REQUEST"; break;
            case FlashTypes::USER_RESPONSE: m_text = "USER_RESPONSE"; break;
            case FlashTypes::USER_NOTIFY: m_text = "USER_NOTIFY"; break;

            case FlashTypes::ACCOUNT_REQUEST: m_text = "ACCOUNT_REQUEST"; break;
            case FlashTypes::ACCOUNT_RESPONSE: m_text = "ACCOUNT_RESPONSE"; break;
            case FlashTypes::ACCOUNT_NOTIFY: m_text = "ACCOUNT_NOTIFY"; break;

            case FlashTypes::GROUP_REQUEST: m_text = "GROUP_REQUEST"; break;
            case FlashTypes::GROUP_RESPONSE: m_text = "GROUP_RESPONSE"; break;
            case FlashTypes::GROUP_NOTIFY: m_text = "GROUP_NOTIFY"; break;

            case FlashTypes::EXCHANGE_FEES_REQUEST: m_text = "EXCHANGE_FEES_REQUEST"; break;
            case FlashTypes::EXCHANGE_FEES_RESPONSE: m_text = "EXCHANGE_FEES_RESPONSE"; break;
            case FlashTypes::EXCHANGE_FEES_NOTIFY: m_text = "EXCHANGE_FEES_NOTIFY"; break;

            case FlashTypes::SIM_PARAMS_REQUEST: m_text = "SIM_PARAMS_REQUEST"; break;
            case FlashTypes::SIM_PARAMS_RESPONSE: m_text = "SIM_PARAMS_RESPONSE"; break;
            case FlashTypes::SIM_PARAMS_NOTIFY: m_text = "SIM_PARAMS_NOTIFY"; break;

            case FlashTypes::STRATEGY_RESULTS_REQUEST: m_text = "STRATEGY_RESULTS_REQUEST"; break;
            case FlashTypes::STRATEGY_RESULTS_RESPONSE: m_text = "STRATEGY_RESULTS_RESPONSE"; break;
            case FlashTypes::STRATEGY_RESULTS_NOTIFY: m_text = "STRATEGY_RESULTS_NOTIFY"; break;

            case FlashTypes::STRATEGY_PARAMS_REQUEST: m_text = "STRATEGY_PARAMS_REQUEST"; break;
            case FlashTypes::STRATEGY_PARAMS_RESPONSE: m_text = "STRATEGY_PARAMS_RESPONSE"; break;
            case FlashTypes::STRATEGY_PARAMS_NOTIFY: m_text = "STRATEGY_PARAMS_NOTIFY"; break;

            case FlashTypes::STRATEGY_STATECHANGE_REQUEST: m_text = "STRATEGY_STATECHANGE_REQUEST"; break;
            case FlashTypes::STRATEGY_STATECHANGE_RESPONSE: m_text = "STRATEGY_STATECHANGE_RESPONSE"; break;
            case FlashTypes::STRATEGY_STATECHANGE_NOTIFY: m_text = "STRATEGY_STATECHANGE_NOTIFY"; break;

            case FlashTypes::STRATEGY_FILL_NOTIFY: m_text = "STRATEGY_FILL_NOTIFY"; break;
            case FlashTypes::STRATEGY_ORDER_NOTIFY: m_text = "STRATEGY_ORDER_NOTIFY"; break;
            case FlashTypes::STRATEGY_POSITION_NOTIFY: m_text = "STRATEGY_POSITION_NOTIFY"; break;
            case FlashTypes::STRATEGY_PROFITLOSS_NOTIFY: m_text = "STRATEGY_PROFITLOSS_NOTIFY"; break;
            case FlashTypes::STRATEGY_PNL_HISTORY_NOTIFY: m_text = "STRATEGY_PNL_HISTORY_NOTIFY"; break;
            case FlashTypes::STRATEGY_TRADE_NOTIFY: m_text = "STRATEGY_TRADE_NOTIFY"; break;
            case FlashTypes::STRATEGY_DAILY_REPORT_NOTIFY: m_text = "STRATEGY_DAILY_REPORT_NOTIFY"; break;

            case FlashTypes::RESEND_BEGIN_NOTIFY: m_text = "RESEND_BEGIN_NOTIFY"; break;
            case FlashTypes::RESEND_END_NOTIFY: m_text = "RESEND_END_NOTIFY"; break;
            case FlashTypes::STRATEGY_TYPE_NOTIFY: m_text = "STRATEGY_TYPE_NOTIFY"; break;
            case FlashTypes::SYMBOL_PRICE_NOTIFY: m_text = "SYMBOL_PRICE_NOTIFY"; break;
            case FlashTypes::STRATEGY_EXTERNAL_FILL_NOTIFY: m_text = "STRATEGY_EXTERNAL_FILL_NOTIFY"; break;

            case FlashTypes::ACCOUNT_FEES_REQUEST: m_text = "ACCOUNT_FEES_REQUEST"; break;
            case FlashTypes::ACCOUNT_FEES_RESPONSE: m_text = "ACCOUNT_FEES_RESPONSE"; break;
            case FlashTypes::ACCOUNT_FEES_NOTIFY: m_text = "ACCOUNT_FEES_NOTIFY"; break;

            case FlashTypes::STRATEGY_COMMAND_REQUEST: m_text = "STRATEGY_COMMAND_REQUEST"; break;
            case FlashTypes::STRATEGY_COMMAND_RESPONSE: m_text = "STRATEGY_COMMAND_RESPONSE"; break;

            case FlashTypes::STRATEGY_ORDER_REQUEST: m_text = "STRATEGY_ORDER_REQUEST"; break;
            case FlashTypes::STRATEGY_ORDER_RESPONSE: m_text = "STRATEGY_ORDER_RESPONSE"; break;

            case FlashTypes::HEARTBEAT: m_text = "HEARTBEAT"; break;

            case FlashTypes::STRATEGY_RESET_REQUEST: m_text = "STRATEGY_RESET_REQUEST"; break;
            case FlashTypes::STRATEGY_RESET_RESPONSE: m_text = "STRATEGY_RESET_RESPONSE"; break;
            case FlashTypes::STRATEGY_RESET_NOTIFY: m_text = "STRATEGY_RESET_NOTIFY"; break;

            case FlashTypes::CORPORATE_ACTION_NOTIFY: m_text = "CORPORATE_ACTION_NOTIFY"; break;

            case FlashTypes::STRATEGY_GRAPH_SERIES_INIT_NOTIFY: m_text = "STRATEGY_GRAPH_SERIES_INIT_NOTIFY"; break;
            case FlashTypes::STRATEGY_GRAPH_SERIES_ADD_NOTIFY: m_text = "STRATEGY_GRAPH_SERIES_ADD_NOTIFY"; break;
            case FlashTypes::STRATEGY_GRAPH_SERIES_CLEAR_NOTIFY: m_text = "STRATEGY_GRAPH_SERIES_CLEAR_NOTIFY"; break;

            case FlashTypes::STRATEGY_COMMAND_NOTIFY: m_text = "STRATEGY_COMMAND_NOTIFY"; break;

            case FlashTypes::EXECUTION_PROVIDER_NOTIFY: m_text = "EXECUTION_PROVIDER_NOTIFY"; break;

            case FlashTypes::TICK_SIZE_REGIME_NOTIFY: m_text = "TICK_SIZE_REGIME_NOTIFY"; break;
            case FlashTypes::TICK_SIZE_REGIME_REQUEST: m_text = "TICK_SIZE_REGIME_REQUEST"; break;
            case FlashTypes::TICK_SIZE_REGIME_RESPONSE: m_text = "TICK_SIZE_REGIME_RESPONSE"; break;

            case FlashTypes::TICK_SIZE_MULTIPLIER_NOTIFY: m_text = "TICK_SIZE_MULTIPLIER_NOTIFY"; break;
            case FlashTypes::TICK_SIZE_MULTIPLIER_REQUEST: m_text = "TICK_SIZE_MULTIPLIER_REQUEST"; break;
            case FlashTypes::TICK_SIZE_MULTIPLIER_RESPONSE: m_text = "TICK_SIZE_MULTIPLIER_RESPONSE"; break;

            default:
                m_text = "UNKNOWN";
                break;
        }

        return m_text;
    }
};

enum StrategyRunMode {
    STRATEGY_RUN_MODE_UNKNOWN = 0,
    STRATEGY_RUN_MODE_BACKTESTING = 1,
    STRATEGY_RUN_MODE_LIVE_SIMULATION = 2,
    STRATEGY_RUN_MODE_LIVE_PRODUCTION = 3,
    STRATEGY_RUN_MODE_LOCAL_DEVELOPMENT = 4
};

enum LoginResult {
    LOGIN_UNKNOWN = 0,
    LOGIN_REJECT = 1,
    LOGIN_SUCCESS = 2
};

enum CommandResult {
    RESULT_UNKNOWN = 0,
    RESULT_SUCCESS = 1,
    RESULT_FAILED = 2
};

enum CommandType {
    COMMAND_UNKNOWN = 0,
    COMMAND_START = 1,
    COMMAND_STOP = 2,
    COMMAND_PAUSE = 3,
    COMMAND_SHUTDOWN = 4,
    COMMAND_TERMINATE = 5,
    COMMAND_STRATEGY_RECHECK = 6,
    COMMAND_CMDLINE = 7
};

enum CommandTarget {
    TARGET_UNKNOWN = 0,
    TARGET_STRATEGY_INSTANCE = 1,
    TARGET_STRATEGY_TYPE = 2,
    TARGET_STRATEGY_GROUP = 3,
    // TARGET_BACKTEST_INSTANCE = 4,
    TARGET_PROCESSOR = 5,
    TARGET_SERVER = 6
};

enum StrategyInitType {
    STRATEGY_INIT_TYPE_UNKNOWN = 0,
    STRATEGY_INIT_TYPE_GROUP = 1,
    STRATEGY_INIT_TYPE_TYPE = 2,
    STRATEGY_INIT_TYPE_INSTANCE = 3
};

enum StrategyState {
    STRATEGY_STATE_UNKNOWN = 0,
    STRATEGY_STATE_INIT = 1, // strategy being initialized
    STRATEGY_STATE_READY = 2, // strategy ready for running
    STRATEGY_STATE_RUNNING = 3, // strategy is running
    STRATEGY_STATE_SUSPENDED = 4, // strategy is suspended or paused
    STRATEGY_STATE_STOPPED = 5, // strategy is stopped (stopped state)
    STRATEGY_STATE_TERMINATED = 6, // strategy is terminated (stopped state)
    STRATEGY_STATE_BACKTEST_FINISHED = 7 // strategy has finished backtesting (stopped state)
};

enum InfoMsgType {
    INFO_MSG_TYPE_UNKNOWN = 0,
    INFO_MSG_TYPE_SERVER = 1,
    INFO_MSG_TYPE_GROUP = 2,
    INFO_MSG_TYPE_STRATEGY_TYPE = 3,
    INFO_MSG_TYPE_STRATEGY_INST = 4,
    INFO_MSG_TYPE_SYMBOL_PRICES = 5,
    INFO_MSG_TYPE_SYMBOL_PL = 6,
    INFO_MSG_TYPE_BACKTEST_PL = 7
};

enum TableOperation {
    TABLE_OP_UNKNOWN = 0,
    TABLE_OP_ADD = 1,
    TABLE_OP_MODIFY = 2,
    TABLE_OP_DELETE = 3,
    TABLE_OP_SELECT_RECORD = 4,
    TABLE_OP_SELECT_TABLE = 5
};

enum OperationResult {
    OP_RESULT_UNKNOWN = 0,
    OP_RESULT_SUCCESS = 1,
    OP_RESULT_FAIL = 2
};

enum OrderRequestType {
    ORDER_UNKNOWN = 0,
    ORDER_SUBMIT_NEW = 1,
    ORDER_CANCEL = 2,
    ORDER_REPLACE = 3
};

enum OrderResult {
    ORDER_RESULT_UNKNOWN = 0,
    ORDER_RESULT_SUCCESS = 1,
    ORDER_RESULT_REJECTED = 2
};

enum ConnectionState {
    CONN_STATE_STARTUP,
    CONN_STATE_CONNECTED,
    CONN_STATE_DISCONNECTED
};

enum QueryType {
    QUERY_QUOTES_TRADES = 0,
    QUERY_TRADES = 1
};

enum LoginType {
    LOGIN_TYPE_NORMAL = 0,
    LOGIN_TYPE_CMD_LINE = 1 // SPECIAL LOGIN MEANT FOR SENDING ONLY CMD LINE
};

/**
 * Used for describing data types that require an input
 */
enum InputType {
    INPUT_TYPE_NO_INPUT = 0,
    INPUT_TYPE_INT_INPUT = 1,
    INPUT_TYPE_DOUBLE_INPUT = 2,
    INPUT_TYPE_CHAR_INPUT = 3,
    INPUT_TYPE_STRING_INPUT = 4,
    INPUT_TYPE_BOOL_INPUT = 5
};

/**
 * Used to control our write thread for the socket
 */
enum SocketSendMode {
    SEND_SYNCHRONOUS,
    SEND_ASYNCH_AUTO_START,
    SEND_ASYNCH_MANUAL_START
};

} // namespace FlashProtocol
} // namespace StrategyStudio
} // namespace RCM

#endif
