/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/ConditionCodes.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_CONDITION_CODES_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_CONDITION_CODES_H_

#include <string>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * CQS AND UQDF STYLE CONDITIONS. comments indicate which SIPs have condition defined in current documentation, and whether condition implies halt, ineligility for being part of NBBO or FinraBBO, ineligibility for NMS auto-execution protections
 * @version  11/8/2011
 *
 * UQDF condition codes are more broad, reasons for halts are handled in a different message type
 * Halt type conditions accompanied by zero price/sizes mean halt, halt type conditions with nonzero prices give trading range indications prior to reopening
 */
enum QuoteCondition {
    QUOTE_COND_UNKNOWN = 0,
    QUOTE_COND_REGULAR,             //'R':CQS/UTP. OPRA:' '
    QUOTE_COND_OPENING,             //'O':CQS/UTP
    QUOTE_COND_SLOW_ASK,            //'A':CQS/UTP. ask-NoRegNMS
    QUOTE_COND_SLOW_BID,            //'B':CQS/UTP. bid-NoRegNMS
    QUOTE_COND_CLOSING,             //'C':CQS NoNBBO NoRegNMS  ...there may be two closing quotes when there are MarketOnClose transactions
    QUOTE_COND_NEWS_DISSEMINATION,  //'D':CQS Halt NoNBBO NoRegNMS
    QUOTE_COND_SLOW_BID_LRPGAP,     //'E':CQS bid-noNBBO bid-NoRegNMS
    QUOTE_COND_SLOW_ASK_LRPGAP,     //'F':CQS ask-noNBBO ask-NoRegNMS
    QUOTE_COND_FAST,                //'F':UTP   NoNBBO NoRegNMS ...may have been deprecated post REG NMS but still in UTP documentation
    QUOTE_COND_RANGE_INDICATION,    //'G':CQS NoNBBO, NoRegNMS
    QUOTE_COND_SLOW,                //'H':CQS/UTP  NoRegNMS
    QUOTE_COND_IMBALANCE,           //'I':CQS/UTP   Halt NoNBBO NoRegNMS
    QUOTE_COND_RELATED_NEWS_DISS,   //'J':CQS   Halt NoNBBO NoRegNMS
    QUOTE_COND_RELATED_NEWS_PEND,   //'K':CQS   Halt NoNBBO NoRegNMS
    QUOTE_COND_MM_CLOSED,           //'L':CQS/UTP  Halt NoNBBO NoRegNMS
    QUOTE_COND_ADDITIONAL_INFO,     //'M':CQS  Halt NoNBBO NoRegNMS //now called volatility trading pause in CQS
    QUOTE_COND_NON_FIRM,            //'N':CQS/UTP NoNBBO NoRegNMS
    QUOTE_COND_NEWS_PENDING,        //'P':CQS  Halt NoNBBO NoRegNMS
    QUOTE_COND_RELATED_MORE_INFO,   //'Q':CQS Halt NoNBBO NoRegNMS
    QUOTE_COND_DUE_TO_RELATED,      //'S':CQS Halt NoNBBO NoRegNMS
    QUOTE_COND_RESUME,              //'T':CQS NoBBO, zero price/size indicates quoting is no longer suspended
    QUOTE_COND_SLOW_LRP,            //'U':CQS/UTP NoBBO, NoRegNMS
    QUOTE_COND_VIEW_COMMON,         //'V':CQS Halt NoNBBO NoRegNMS
    QUOTE_COND_SLOW_SET_LIST,       //'W':CQS NoRegNMS
    QUOTE_COND_EQUIPMENT,           //'X':CQS Halt NoNBBO NoRegNMS
    QUOTE_COND_ORDER_INFLUX,        //'X':UTP Halt NoNBBO NoRegNMS ...may have been deprecated post REG NMS but still in UTP documentation
    QUOTE_COND_REG_ONE_SIDED,       //'Y':UTP the side that has a quote is a regular quote, nothing exists on the other side
    QUOTE_COND_SUB_PENNY,           //'Y':CQS  Halt NoNBBO NoRegNMS
    QUOTE_COND_NO_OPEN_OR_RESUME,   //'Z': Halt NoNBBO NoRegNMS
    QUOTE_COND_LIMIT_PRICE_BAND,	//'0': CQS NoNBBO
    QUOTE_COND_MARKET_WIDE_CIRCUIT_BREAKER_1, //'1': CQS Halt NoNBBO ... SP500 7% decline
    QUOTE_COND_MARKET_WIDE_CIRCUIT_BREAKER_2, //'2': CQS Halt NoNBBO ... SP500 13% decline
    QUOTE_COND_MARKET_WIDE_CIRCUIT_BREAKER_3, //'3': CQS Halt NoNBBO ... SP500 30% decline
    QUOTE_COND_ON_DEMAND_INTRADAY_AUCTION,    //'4': CQS/UTP Halt NoNBBO
    QUOTE_COND_REPUBLISHED_LIMIT_PRICE_BAND,  //'9': CQS NoNBBO
    //PRE-RegNMS conditions...indicated more liquidity was available than indicated by the official quote sizes/////
    QUOTE_COND_DEPTH_ON_OFFER,        //'A' PreRegNMS
    QUOTE_COND_DEPTH_ON_BID,          //'B' PreRegNMS
    QUOTE_COND_DEPTH_ON_BID_AND_OFFER,//'H' PreRegNMS
    //OPRA SPECIFIC QUOTE CONDITIONS
    QUOTE_COND_OPRA_NONFIRM,          //'F' NoNBBO
    QUOTE_COND_OPRA_INDICATIVE_VALUE, //'I' NoNBBO
    QUOTE_COND_OPRA_ROTATION,         //'R' NoNBBO
    QUOTE_COND_OPRA_HALTED,           //'T' Halt NoNBBO
    QUOTE_COND_OPRA_AUTO,             //'A' Eligible for automatic execution
    QUOTE_COND_OPRA_BID_INTEREST,     //'B' bid contains customer interest
    QUOTE_COND_OPRA_OFFER_INTEREST,   //'O' offer contains customer interest
    QUOTE_COND_OPRA_CUSTOMER_INTEREST,//'C' both sides contain customer interest
    QUOTE_COND_OPRA_OFFER_NOTFIRM,    //'X' offer not firm, ask-NoNBBO
    QUOTE_COND_OPRA_BID_NOTFIRM,      //'Y' offer not firm, bid-NoNBBO
};

/**
 *CTS AND UTDF STYLE CONDITIONS.
 *@version 11/26/2013
 */
enum TradeCondition {
    TRADE_COND_UNKNOWN,
    TRADE_COND_REGULAR,                         //CTS/UTP''or'@',  OPRA ' ', LSE 'AT'
    //US EQUITY CONDITIONS
      //*******settlement indicators
    TRADE_COND_CASH_TRADE,                      //CTS/UTP'C'
    TRADE_COND_NEXT_DAY,                        //CTS/UTP'N'
    TRADE_COND_SELLER,                          //CTS/UTP'R' SETTLEMENT DATE NEGOTIATED BY SELLER
    TRADE_COND_YELLOW_FLAG,                     //UTP'Y'
      //********trade through exemption indicators
    TRADE_COND_INTERMARKET_SWEEP,               //CTS/UTP'F'
    TRADE_COND_OPENING_PRINTS,                  //CTS/UTP'O'
    TRADE_COND_DERIVATIVELY_PRICED,             //CTS/UTP'4'
    TRADE_COND_REOPENING,                       //CTS/UTP'5'
    TRADE_COND_CLOSING_PRINTS,                  //CTS/UTP'6'
    TRADE_COND_QUALIFIED_CONTINGENT_TRADE,      //CTS/UTP'7'
    TRADE_COND_611_EXEMPT,                      //UTP'8'
    TRADE_COND_CORRECTED_CONSOLIDATED_CLOSE,    //CTS/UTP'9' CORRECTION TO LISTING MARKET'S CLOSE PRICE, VOLUME SHOULD BE ZERO
      //*********extended hours/sequence indicators
    TRADE_COND_SOLD_LAST,                       //CTS/UTP'L' REPORTED IN SEQUENCE BUT AT A DELAY,
    TRADE_COND_EXTENDED_HOURS,                  //CTS/UTP'T' UTP CALLS THIS FORM_T
    TRADE_COND_EXTENDED_HOURS_SOLD,             //CTS/UTP'U' EXTENDED HOURS REPORTED OUT OF SEQUENCE
    TRADE_COND_SOLD_SALE,                       //CTS/UTP'Z' TRADE REPORTED LATE AND OUT OF SEQUENCE
      //*********SRO required detail
    TRADE_COND_ACQUISITION,                     //UTP'A'
    TRADE_COND_AVG_PRICE_TRADES,                //CTS'B'UTP'W': PRICE IS AN AVERAGE OF TRANSACTIONS OVER SOME PERIOD OF TIME
    TRADE_COND_BUNCHED,                         //UTP'B' TWO OR MORE REGULAR TRADES WITHIN 60 SECS
    TRADE_COND_DISTRIBUTION,                    //UTP'D'  ...a block trade that does not adversely affact prices
    TRADE_COND_AUTOMATIC_EXECUTION,             //CTS'E'
    TRADE_COND_OPENING_REOPENING_DETAIL,        //CTS'G'
    TRADE_COND_BUNCHED_SOLD,                    //UTP'G' LATE REPORTED BUNCHED TRADE
    TRADE_COND_INTRADAY_DETAIL,                 //CTS'H'
    TRADE_COND_PRICE_VARIATION,                 //CTS'H'/UTP'H'
    TRADE_COND_ODD_LOT,                         //CTS/UTP'I'
    TRADE_COND_RULE_127,                        //CTS'K' NYSE BLOCK POSITION
    TRADE_COND_RULE_155,                        //CTS/UTP'K' AMEX ONLY
    TRADE_COND_MARKET_CENTER_CLOSE_PRICE,       //CTS/UTP'M'
    TRADE_COND_MARKET_CENTER_OFFICIAL_OPEN,     //CTS/UTP'Q'
    TRADE_COND_PRIOR_REFRENCE,                  //CTS/UTP'P' TRADES THAT OCCUR BASED ON AN EARLIER SET PRICE POINT
    TRADE_COND_SPLIT_TRADE,                     //UTP'S' TWO MARKET MAKERS SPLIT AN EXECUTION
    TRADE_COND_CONTINGENT_TRADE,                //UTP/CTS'V'
    TRADE_COND_CROSS_TRADE,                     //UTP/CTS'X'
    TRADE_COND_STOPPED_STOCK_REGULAR,           //UTP'1'
    TRADE_COND_STOPPED_STOCK_SOLD,              //UTP'2' LATE
    TRADE_COND_STOPPED_STOCK_SOLD_SALE,         //UTP'3' LATE AND OUT OF SEQUENCE
      //*********OLD UTP/CTS CODES
    TRADE_COND_CASH_ONLY,                       //TAQ'A'
    TRADE_COND_NEXT_DAY_ONLY,                   //TAQ'D'
    TRADE_COND_CAP_ELECTION,                    //TAQ'I'
    TRADE_COND_STOCK_OPTION_TRADE,              //UTP/CTS'V'
      //*********MONTHLY TAQ CODES
    TRADE_COND_NYSE_DIRECTPLUS,                 //TAQ'E' NYSE AUTO EXEC FOR SMALL TRANSACTIONS
    TRADE_COND_BSE_ILA,                         //TAQ'E' BOSTON ELECTRONIC EXECUTION WHEN BSE IS NBBO
    TRADE_COND_OPENED_LAST,                     //TAQ'O' OPENING TRADE REPORTED IN SEQUENCE BUT LATE
    TRADE_COND_OPENED_AFTER_HALT,               //TAQ'X'
    TRADE_COND_CROSSING_SESSION,                //TAQ'8''9'
      //*********ORDER BOOK TRADE CONDITIONS
    TRADE_COND_OB_HIDDEN_LIQUIDITY,             //order book execution against hidden liquidity
    TRADE_COND_OB_OPENING_CROSSING_SESSION,     //order book opening crossing session execution
    TRADE_COND_OB_CLOSING_CROSSING_SESSION,     //order book closing crossing session execution
    TRADE_COND_OB_HALT_CROSSING_SESSION,        //order book halt/ipo crossing session execution
    TRADE_COND_OB_INTRADAY_CROSSING_SESSION,    //order book intraday/ session execution
    TRADE_COND_OB_EXEC_AT_PRICE_PRINTABLE,      //order book execution at price other than order price; printable
    TRADE_COND_OB_EXEC_AT_PRICE_NOT_PRINTABLE,  //order book execution at price other than order price; non-printable, volume will be duplicated in a later message
    //US Options Market (OPRA) CONDITIONS
    TRADE_COND_OPRA_CANC,                       //OPRA 'A': Cancel of a previously reported trade, other than last or opening trade
    TRADE_COND_OPRA_OSEQ,                       //OPRA 'B': Late and out of sequence
    TRADE_COND_OPRA_CNCL,                       //OPRA 'C': Cancel of the last trade
    TRADE_COND_OPRA_LATE,                       //OPRA 'D': Late but in correct sequence
    TRADE_COND_OPRA_CNCO,                       //OPRA 'E': Cancel of first trade of day, given other trades already reported
    TRADE_COND_OPRA_OPEN,                       //OPRA 'F': Opening trade, reported late and out of sequence
    TRADE_COND_OPRA_CNOL,                       //OPRA 'G': Cancel of the one and only trade for the day
    TRADE_COND_OPRA_OPNL,                       //OPRA 'H': Open trade, reported late but in sequence
    TRADE_COND_OPRA_AUTO,                       //OPRA 'I': Auto execution
    TRADE_COND_OPRA_REOP,                       //OPRA 'J': Reopen of previously halted contract
    TRADE_COND_OPRA_AJST,                       //OPRA 'K': Trade in a contract whose terms have been adjusted for a corporate action; Deprecated 2019-11-04
    TRADE_COND_OPRA_SPRD,                       //OPRA 'L': Trade is part of a spread trade with two options of the same class; Deprecated 2019-11-04
    TRADE_COND_OPRA_STDL,                       //OPRA 'M': Trade is part of a spread trade two options, put and call; Deprecated 2019-11-04
    TRADE_COND_OPRA_STPD,                       //OPRA 'N': Trade price agreed at by floor personal, conditioned on execution of a non-stopped trade in same series at same price; Deprecated 2019-11-04
    TRADE_COND_OPRA_CSTP,                       //OPRA 'O': Cancel of stopped transaction; Deprecated
    TRADE_COND_OPRA_BWRT,                       //OPRA 'P': Option portion of an execution tied to stock; Deprecated 2019-11-04
    TRADE_COND_OPRA_CMBO,                       //OPRA 'Q': Part of a Combo execution, purchase of call and sale of a put in the same underlying; Deprecated 2019-11-04
    TRADE_COND_OPRA_SPIM,                       //OPRA 'R': Execution of an order stopped at price that did not constitute a Trade Through at the time order was placed. Don't use to update 'last'; Deprecated 2019-11-04
    TRADE_COND_OPRA_ISOI,                       //OPRA 'S': Trade resulting from an Intermarket Sweep Order
    TRADE_COND_OPRA_BNMT,                       //OPRA 'T': Execution price tied to a benchmark. Do not use to update 'last' price; Deprecated 2019-11-04
    TRADE_COND_OPRA_XMPT,                       //OPRA 'X': Trade is Trade Through Exempt; Deprecated 2019-11-04
    TRADE_COND_OPRA_SLAN,                       //OPRA 'a': Single Leg Auction Non ISO
    TRADE_COND_OPRA_SLAI,                       //OPRA 'b': Single Leg Auction ISO
    TRADE_COND_OPRA_SLCN,                       //OPRA 'c': Single Leg Cross Non ISO
    TRADE_COND_OPRA_SCLI,                       //OPRA 'd': Single Leg Cross ISO
    TRADE_COND_OPRA_SLFT,                       //OPRA 'e': Single Leg Floor Trade
    TRADE_COND_OPRA_MLET,                       //OPRA 'f': Multi Leg auto-electronic trade
    TRADE_COND_OPRA_MLAT,                       //OPRA 'g': Multi Leg Auction
    TRADE_COND_OPRA_MLCT,                       //OPRA 'h': Multi Leg Cross
    TRADE_COND_OPRA_MLFT,                       //OPRA 'i': Multi Leg floor trade
    TRADE_COND_OPRA_MESL,                       //OPRA 'j': Multi Leg auto-electronic trade against single leg(s)
    TRADE_COND_OPRA_TLAT,                       //OPRA 'k': Stock Options Auction
    TRADE_COND_OPRA_MASL,                       //OPRA 'l': Multi Leg Auction against single leg(s)
    TRADE_COND_OPRA_MFSL,                       //OPRA 'm': Multi Leg floor trade against single leg(s)
    TRADE_COND_OPRA_TLET,                       //OPRA 'n': Stock Options auto-electronic trade
    TRADE_COND_OPRA_TLCT,                       //OPRA 'o': Stock Options Cross
    TRADE_COND_OPRA_TLFT,                       //OPRA 'p': Stock Options floor trade
    TRADE_COND_OPRA_TESL,                       //OPRA 'q': Stock Options auto-electronic trade against single  leg(s)
    TRADE_COND_OPRA_TASL,                       //OPRA 'r': Stock Options Auction against signle leg(s)
    TRADE_COND_OPRA_TFSL,                       //OPRA 's': Stock Options floor trade against single leg(s)
    TRADE_COND_OPRA_CBMO,                       //OPRA 't': Multi Leg Floor Trade of Proprietary Products; The trade price may be outside the current NBBO
    TRADE_COND_OPRA_MCTP,                       //OPRA 'u': Multilateral Compression Trade of Proprietary Products. Trades are executed outside of regular trading hours at prices derived from end of day markets. Trades do not update Open, High, Low, and Closing Prices.
    TRADE_COND_OPRA_EXHT,                       //OPRA 'v': Transaction represents a trade that was executed outside of regular market hours. Trades do not update Open, High, Low, and Closing Prices.
    // London Stock Exchange Conditions
    TRADE_COND_LSE_UNCROSSING,                  //LSE 'UT': Uncrossing auction trade (automatic)
    TRADE_COND_LSE_CLOSING_CROSS,               //LSE 'PT': Closing price crossing session trade (automatic)
    TRADE_COND_LSE_ORDINARY_OFF_BOOK,           //LSE 'O' : Ordinary manual trade (off-book)
    TRADE_COND_LSE_ORDINARY_DELAYED,            //LSE 'OK': Ordinary trade with delayed publication (off-book)
    TRADE_COND_LSE_NEGOTIATED,                  //LSE 'NT': Negotiated trade (off book)
    TRADE_COND_LSE_NEGOTIATED_DELAYED,          //LSE 'NK': Negotiated trade with delayed publication (off-book)
    TRADE_COND_LSE_LATE_CORRECTION,             //LSE 'LC': Late trade correction (off-book)
    TRADE_COND_LSE_DELAYED_LATE_CORRECTION,     //LSE 'GC': Cancel of GILT & UKGT trades after date of submission (off-book)
    TRADE_COND_LSE_NOT_TO_MARK,                 //LSE 'NM': Not to mark. LSE grants permission for non publication of transaction (off-book)
    TRADE_COND_LSE_NOT_TO_MARK_CANCEL,          //LSE 'NC': Cancellation of NM trade ofter date of submission (off-book)
    TRADE_COND_LSE_OTC,                         //LSE 'OT': OTC trade, immediate publication (off-book)
    TRADE_COND_LSE_OTC_DELAYED,                 //LSE 'TK': OTC trade, delayed publication requested (off-book)
    TRADE_COND_LSE_OTC_CANCEL,                  //LSE 'OC': Cancellation of OTC trade after date of publication (off-book)
    TRADE_COND_LSE_INTER_FUND_TRANSFER,         //LSE 'IF': Inter fund transfer with delayed publication (off-book)
    TRADE_COND_LSE_PREVIOUS_DAY_CONTRA,         //LSE 'PC': Previous day contra (off-book)
    TRADE_COND_LSE_SI,                          //LSE 'SI': SI trade (off-book)
    TRADE_COND_LSE_SI_DELAYED,                  //LSE 'SK': SI trade with delayed publication (off-book)
    TRADE_COND_LSE_SI_CANCEL,                   //LSE 'SC': Cancellation of SI trade from prior date (off-book)
    TRADE_COND_LSE_LATE_TRADE,                  //LSE 'M': Late Trade (specific to off-book trades)
    TRADE_COND_LSE_OPENING_AUCTION,             //LSE 'UTR': Trade from opening price auction
    TRADE_COND_LSE_AESP_AUCTION,                //LSE 'UTW': Trade from Auction Execution Suspected Auction
    TRADE_COND_LSE_PERIODIC_AUCTION,            //LSE 'UT5': Trade from Periodic Auction
    TRADE_COND_LSE_CLOSING_AUCTION,             //LSE 'UTj': Trade from closing price auction
    TRADE_COND_LSE_EDSP_AUCTION,                //LSE 'UTw': Trade for Exchange Delivery Settlement Price Auction
    TRADE_COND_LSE_RESUME_AUCTION,               //LSE 'UTz': Trade from Resume Auction
    TRADE_COND_MAX_VALUE = TRADE_COND_LSE_RESUME_AUCTION
};

// function pointer that takes a char and returns a TradeCondition
typedef TradeCondition(*TradeConditionConverter)(char);


/**
 * Convert total view itch crossing session trade code to trade condition
 */
inline TradeCondition ConvertTVItchCrossingSessionTradeCodeToTradeCond(char crossing)
{
    switch (crossing) {
        case 'O':
            return TRADE_COND_OB_OPENING_CROSSING_SESSION;
        case 'C':
            return TRADE_COND_OB_CLOSING_CROSSING_SESSION;
        case 'H':
            return TRADE_COND_OB_HALT_CROSSING_SESSION;
        case 'I':
            return TRADE_COND_OB_INTRADAY_CROSSING_SESSION;
        default:
            return TRADE_COND_UNKNOWN;
    }
}


class QuoteConditionUtil {
public:

    static std::string QuoteConditionToString(QuoteCondition cond);

    /**
     * Returns the quote condition corresponding to the CQS character code
     *
     * @param condition_code A character corresponding to the CQS condition code
     */
    static QuoteCondition CQSQuoteConditionFromChar(char condition_code);

    /**
     * Returns the quote condition corresponding to the UQDF character code
     *
     * @param condition_code A character corresponding to the UQDF condition code
     */
    static QuoteCondition UQDFQuoteConditionFromChar(char condition_code);

    /**
     * Returns the quote condition corresponding to the OPRA character code
     *
     * @param condition_code A character corresponding to the OPRA condition code
     */
    static QuoteCondition OPRAQuoteConditionFromChar(char condition_code);
};

class TradeConditionUtil {
public:

    static std::string TradeConditionToString(TradeCondition condition);

    /**
    * Returns the trade condition corresponding to the CTS character code
    *
    * @param condition_code A character corresponding to the CTS condition code
    */
    static TradeCondition CTSTradeConditionFromChar(char condition_code);

    /**
     * Returns the trade condition corresponding to the UTDF character code
     *
     * @param condition_code A character corresponding to the UTDF condition code
     */
    static TradeCondition UTDFTradeConditionFromChar(char condition_code);

    /**
    * Returns the trade condition corresponding to the CTS character code
    * This provides the old mapping from before Contingent Trade was introduced
    *
    * @param condition_code A character corresponding to the CTS condition code
    */
    static TradeCondition CTSTradeConditionFromCharPre20150803(char condition_code);

    /**
    * Returns the trade condition corresponding to the CTS character code
    * This provides the old mapping from before Odd Lot was introduced
    *
    * @param condition_code A character corresponding to the CTS condition code
    */
    static TradeCondition CTSTradeConditionFromCharPre20131209(char condition_code);

    /**
    * Returns the trade condition corresponding to the UTDF character code
    * This provides the old mapping from before Contingent Trade was introduced
    *
    * @param condition_code A character corresponding to the UTDF condition code
    */
    static TradeCondition UTDFTradeConditionFromCharPre20150803(char condition_code);

    /**
    * Returns the trade condition corresponding to the UTDF character code
    * This provides the old mapping from before Odd Lot was introduced
    *
    * @param condition_code A character corresponding to the UTDF condition code
    */
    static TradeCondition UTDFTradeConditionFromCharPre20131209(char condition_code);

    /**
     * Returns the trade condition corresponding to the OPRA character code
     *
     * @param condition_code A character corresponding to the OPRA condition code
     */
    static TradeCondition OPRATradeConditionFromChar(char condition_code);

    /**
     * Returns the trade condition corresponding to the LSE abbreviation
     *
     * @param condition A string corresponding to LSE's abbreviation
     */
    static TradeCondition LSETradeConditionFromAbbreviation(const std::string& condition_code);

    /**
     * Returns the trade condition corresponding to the LSE late trades/auction indicators
     *
     * @param condition_code A character corresponding to the auction indicator
     */
    static TradeCondition LSETradeConditionFromAuctionChar(char condition_code);
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif
