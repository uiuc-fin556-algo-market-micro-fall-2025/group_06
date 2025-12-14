/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/MarketCenterTypes.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_MARKET_CENTER_TYPES_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_MARKET_CENTER_TYPES_H_

#include <Utilities/StrategyStudioException.h>
#include <string>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * Lists the supported market centers
 */
enum MarketCenterID {
    MARKET_CENTER_ID_UNKNOWN = 0,
//US EQUITY MARKET_CENTER_IDS
    MARKET_CENTER_ID_AMEX = 1, //MIC:XASE
    MARKET_CENTER_ID_BATS = 2, //MIC:BATS
    MARKET_CENTER_ID_BYX = 3,  //MIC:BATY
    MARKET_CENTER_ID_BSE = 4, //MIC:XBOS
    MARKET_CENTER_ID_EDGA = 5, //MIC:EDGA
    MARKET_CENTER_ID_EDGX = 6, //MIC:EDGX
    MARKET_CENTER_ID_NSX = 7,  //MIC:XCIS
    MARKET_CENTER_ID_FINRA = 8,//MIC: XADF
    MARKET_CENTER_ID_ISE = 9, //MIC:XISE
    MARKET_CENTER_ID_LTSE = 10, //MIC:LTSE
    MARKET_CENTER_ID_MEMX = 11, //MIC:MEMX
    MARKET_CENTER_ID_MIAX = 12, //MIC:EPRL
    MARKET_CENTER_ID_CHICAGO = 13, //MIC:XCHI
    MARKET_CENTER_ID_NYSE = 14, //MIC:XNYS
    MARKET_CENTER_ID_ARCA = 15, //MIC:ARCX
    MARKET_CENTER_ID_NASDAQ = 16, //MIC:XNAS
    MARKET_CENTER_ID_CONSOLIDATED_CTS = 17,
    MARKET_CENTER_ID_CONSOLIDATED_SIP = 18,
    MARKET_CENTER_ID_PHLX = 19, //MIC:XPHL //Note this was rebranded PSX in 2010
    MARKET_CENTER_ID_CBOESTOCK = 20, //MIC:CBSX
    MARKET_CENTER_ID_IEX = 21, //MIC:IEXG
//FUTURES MARKET_CENTER_IDS
    MARKET_CENTER_ID_CME_GLOBEX = 22, //MIC:GLBX, XCME, XCBT, XNYM, XCEC, XMGE
    MARKET_CENTER_ID_ICE_FUTURES = 23, //MIC:IFUS, IFEU, IFLL, IFLX, IFLO, IFUT, IFED
    MARKET_CENTER_ID_EUREX = 24, //MIC:XEUR
    MARKET_CENTER_ID_CBOE_FUTURES = 25, //MIC:XCBF
    MARKET_CENTER_ID_ASX = 26, //MIC:XASX
    //*MARKET_CENTER_ID_BOVESPA, //MIC:BVMF // Grouped with international equities, single MIC trades both asset classes
    MARKET_CENTER_ID_CFFEX = 27, //MIC:CCFX
    MARKET_CENTER_ID_DALIAN_FUTURES = 28, //MIC:XDCE
    MARKET_CENTER_ID_EURONEXT_FUTURES = 29, //MIC:XMAT, XMON, XEUE
    MARKET_CENTER_ID_HKEX_FUTURES = 30, //MIC:XHKF
    MARKET_CENTER_ID_KRX_FUTURES = 31, //MIC:XKFE
    MARKET_CENTER_ID_LME = 32, //MIC:XLME
    MARKET_CENTER_ID_NODAL = 33, //MIC:NODX
    MARKET_CENTER_ID_OSAKA = 34, //MIC:XOSE
    MARKET_CENTER_ID_SHANGHAI_FUTURES = 35, //MIC:XSGE
    MARKET_CENTER_ID_ZHENGZHOU_FUTURES = 36, //MIC:XZCE
    MARKET_CENTER_ID_MONTREAL = 37, //MIX:XMOD
    MARKET_CENTER_ID_NASDAQ_FUTURES = 38, //MIC:XPBT
    MARKET_CENTER_ID_TOCOM = 39, //MIC:XTKT
    MARKET_CENTER_ID_SGX = 40, //MIC::XSCE
//FIXED INCOME VENUES
    MARKET_CENTER_ID_BOND_BTEC = 41, //MIC:BTEC, BrokerTec
    MARKET_CENTER_ID_BOND_ESPD = 42, //MIC:ESPD, Nasdaq OMX eSpeed
//US OPTIONS MARKET_CENTER_IDS
    MARKET_CENTER_ID_AMEX_OPTIONS = 43, //MIC:AMXO
    MARKET_CENTER_ID_ARCA_OPTIONS = 44, //MIC:ARCO
    MARKET_CENTER_ID_BATS_OPTIONS = 45, //MIC:BATO
    MARKET_CENTER_ID_BOX_OPTIONS = 46, //MIC:XBOX
    MARKET_CENTER_ID_CBOE_OPTIONS = 47, //MIC:XCBO
    MARKET_CENTER_ID_C2_OPTIONS = 48, //MIC:C2OX
    MARKET_CENTER_ID_EMERALD_OPTIONS = 49, //MIC:EMLD
    MARKET_CENTER_ID_ISE_OPTIONS = 50, //MIC:XISX
    MARKET_CENTER_ID_OPRA_OPTIONS = 51, //MIC:XFINO
    MARKET_CENTER_ID_NASDAQ_OPTIONS = 52, //MIC:XNDQ
    MARKET_CENTER_ID_PHLX_OPTIONS = 53, //MIC:XPHO
    MARKET_CENTER_ID_GEMINI_OPTIONS = 54, //MIC:GMNI
    MARKET_CENTER_ID_MIAMI_OPTIONS = 55, //MIC:XMIO
    MARKET_CENTER_ID_OMX_BX_OPTIONS = 56, //MIC:XBXO
    MARKET_CENTER_ID_EDGX_OPTIONS = 57, //MIC:EDGO
    MARKET_CENTER_ID_MERCURY_OPTIONS = 58, //MIC:MCRY
    MARKET_CENTER_ID_PEARL_OPTIONS = 59, //MIC:MPRL
//US Equity ATS/DARK POOL CENTERS/SmartOrderRouter
    MARKET_CENTER_ID_SMART_ROUTER = 60,
    MARKET_CENTER_ID_ATS_BIDS = 61, //MIC:BIDS, BIDS Trading
    MARKET_CENTER_ID_ATS_CBX = 62, //MIC:ICBX, Instinet CBX
    MARKET_CENTER_ID_ATS_CRSF = 63, //MIC:CAES, Credit Suisse Crossfinder
    MARKET_CENTER_ID_ATS_CSLP = 64, //MIC:CSLP, Credit Suisse Light Pool
    MARKET_CENTER_ID_ATS_KLINK = 65, //MIC:KNLI, Knight Link
    MARKET_CENTER_ID_ATS_KMATCH = 66, //MIC:KNMX, Knight Match
    MARKET_CENTER_ID_ATS_LAVA = 67, //MIC:LAFL, Lavaflow
    MARKET_CENTER_ID_ATS_LEVEL = 68, //MIC:LEVL, Level ATS
    MARKET_CENTER_ID_ATS_LX = 69, //MIC:BARX, Barlclays LX
    MARKET_CENTER_ID_ATS_MILM = 70, //MIC:NYFX, ConvergEx Millennium
    MARKET_CENTER_ID_ATS_MLXN = 71, //MIC:BAML, Merrill Lynch ATS
    MARKET_CENTER_ID_ATS_POSIT = 72, //MIC:ITGI, ITG POSIT
    MARKET_CENTER_ID_ATS_SIGMA_X = 73, //MIC:SIGX, Goldman Sachs Sigma X MTF
    MARKET_CENTER_ID_ATS_VORTX = 74, //MIC:VTEX, ConvergEx Vortex
    MARKET_CENTER_ID_ATS_CDEL = 75, //MIC:CDEL, Citadel Apogee Dark Pool
    MARKET_CENTER_ID_ATS_DBSX = 76, //MIC:DBSX, Deutsche Bank SuperX Dark Pool
    MARKET_CENTER_ID_ATS_GTCO = 77, //MIC:GTCO, GETCO GETMatched Dark Pool
    MARKET_CENTER_ID_ATS_JPMX = 78, //MIC:JPMX, JP Morgan Dark Pool
    MARKET_CENTER_ID_ATS_PDQ = 79, //MIC:PDQX, PDQ Dark Pool
    MARKET_CENTER_ID_ATS_UBSA = 80, //MIC:UBSA, UBS ATWS Dark Pool
    MARKET_CENTER_ID_MM_TWOSIGMA = 81, //MIC:SOHO
    MARKET_CENTER_ID_MM_VIRTU = 82, //MIC:VFCM
//Canadian dark ATS
    MARKET_CENTER_ID_CA_DARK_ICX = 83, //MIC::XICX, Instinet Canada Cross
    MARKET_CENTER_ID_CA_DARK_LIQUIDNET = 84, //MIC::LICA, Liquidnet Canada ATS
    MARKET_CENTER_ID_CA_DARK_MATCHNOW = 85, //MIC::MATN, Match Now
    MARKET_CENTER_ID_CA_DARK_NASDAQ_CXD = 86, //MIC::XCXD, Nasdaq CXD
//US Options ATS and Routing Engines
    MARKET_CENTER_ID_SMART_ROUTER_OPTIONS = 87,
//FX SPOT TRADING VENUES
    MARKET_CENTER_ID_FX_CURRENEX = 88, //MIC:XCUR, Currenex
    MARKET_CENTER_ID_FX_HOTSPOT = 89, //MIC:HSFX, Hotspot FX
    MARKET_CENTER_ID_FX_EBS = 90,
    MARKET_CENTER_ID_FX_REUTERS = 91,
    MARKET_CENTER_ID_FX_FXALL = 92, //MIC:FXAL
    MARKET_CENTER_ID_FX_LMAX = 93, //MIC:LMAX
    MARKET_CENTER_ID_FX_OTC = 94, //General OTC bank market data
//International Equities
    MARKET_CENTER_ID_CA_AEQUITAS_LIT = 95,
    MARKET_CENTER_ID_CA_AEQUITAS_NEO = 96, //MIC:NEOE
    MARKET_CENTER_ID_CA_ALPHA = 97, //MIC:XATS
    MARKET_CENTER_ID_CA_CSE = 98, //MIC:XCNQ
    MARKET_CENTER_ID_CA_LYNX = 99, //MIC::LYNX
    MARKET_CENTER_ID_CA_NASDAQ_CXC = 100, //MIC:CHIC
    MARKET_CENTER_ID_CA_NASDAQ_CX2 = 101, //MIC:XCX2
    MARKET_CENTER_ID_CA_OMEGA = 102, //MIC::OMGA
    MARKET_CENTER_ID_CA_TSX = 103, //MIC::XTSE
    MARKET_CENTER_ID_CA_TSXV = 104, //MIC::XTSX
    MARKET_CENTER_ID_BOVESPA = 105, //MIC:BVMF, also a futures exchange with same MIC
    MARKET_CENTER_ID_LSE = 106, //MIC:XLON, London Stock Exchange
    MARKET_CENTER_ID_EU_XETR = 107, //MIC:XETR
    MARKET_CENTER_ID_EU_XPAR = 108, //MIC:XPAR, Euronext Paris
    MARKET_CENTER_ID_EU_XAMS = 109, //MIC:XAMS, Euronext Amsterdam
    MARKET_CENTER_ID_EU_XBRU = 110, //MIC:XBRU, Euronext Brussels
    MARKET_CENTER_ID_EU_XLIS = 111, //MIC:XLIS, Euronext Lisbon
    MARKET_CENTER_ID_EU_MTAA = 112, //MIC:MTAA, Borsa Italiana
    MARKET_CENTER_ID_EU_XSWX = 113, //MIC:XSWX, SIX Swiss Exchange
    MARKET_CENTER_ID_EU_XVTX = 114, //MIC:XVTX, SIX Swiss Exchange Blue Chip
    MARKET_CENTER_ID_EU_XMCE = 115, //MIC:XMCE, Bolsa de Madrid
    MARKET_CENTER_ID_EU_XSTO = 116, //MIC:XSTO, OMX Stockholm
    MARKET_CENTER_ID_EU_XHEL = 117, //MIC:XHEL, OMX Helsinki
    MARKET_CENTER_ID_EU_XCSE = 118, //MIC:XCSE, OMX Copenhagon
    MARKET_CENTER_ID_EU_TRQX = 119, //MIC:TRQX, Turquoise
    MARKET_CENTER_ID_EU_CHIX = 120, //MIC:CHIX, Chi-X Europe
    MARKET_CENTER_ID_EU_BATE = 121, //MIC:BATE, BATS Europe
    MARKET_CENTER_ID_AP_XIDX = 122, //MIC:XIDX, Indonesia Stock Exchange
    MARKET_CENTER_ID_AP_XKLS = 123,//MIC:XKLS, Bursa Malaysia
    MARKET_CENTER_ID_AP_XSES = 124 //MIC:XSES, Singapore Exchange
};

#define MAX_EXCHANGES 125

/**
 * Converts a MarketCenterID to its string representation
 */
inline std::string MarketCenterToString(MarketCenterID market)
{
    switch (market) {
        case MARKET_CENTER_ID_AMEX:
            return "AMEX";
        case MARKET_CENTER_ID_BATS:
            return "BATS";
        case MARKET_CENTER_ID_BYX:
            return "BYX";
        case MARKET_CENTER_ID_BSE:
            return "BSE";
        case MARKET_CENTER_ID_EDGA:
            return "EDGA";
        case MARKET_CENTER_ID_EDGX:
            return "EDGX";
        case MARKET_CENTER_ID_NSX:
            return "NSX";
        case MARKET_CENTER_ID_FINRA:
            return "FINRA";
        case MARKET_CENTER_ID_ISE:
            return "ISE";
        case MARKET_CENTER_ID_LTSE:
            return "LTSE";
        case MARKET_CENTER_ID_MEMX:
            return "MEMX";
        case MARKET_CENTER_ID_MIAX:
            return "MIAX";
        case MARKET_CENTER_ID_CHICAGO:
            return "CHICAGO";
        case MARKET_CENTER_ID_NYSE:
            return "NYSE";
        case MARKET_CENTER_ID_ARCA:
            return "ARCA";
        case MARKET_CENTER_ID_NASDAQ:
            return "NASDAQ";
        case MARKET_CENTER_ID_CONSOLIDATED_CTS:
            return "CONSOLIDATED_CTS";
        case MARKET_CENTER_ID_CONSOLIDATED_SIP:
            return "CONSOLIDATED_SIP";
        case MARKET_CENTER_ID_PHLX:
            return "PHLX";
        case MARKET_CENTER_ID_CBOESTOCK:
            return "CBOESTOCK";
        case MARKET_CENTER_ID_IEX:
            return "IEX";
        case MARKET_CENTER_ID_CME_GLOBEX:
            return "CME_GLOBEX";
        case MARKET_CENTER_ID_ICE_FUTURES:
            return "ICE_FUTURES";
        case MARKET_CENTER_ID_EUREX:
            return "EUREX";
        case MARKET_CENTER_ID_CBOE_FUTURES:
            return "CBOE_FUTURES";
        case MARKET_CENTER_ID_ASX:
            return "ASX";
        case MARKET_CENTER_ID_CFFEX:
            return "CFFEX";
        case MARKET_CENTER_ID_DALIAN_FUTURES:
            return "DALIAN_FUTURES";
        case MARKET_CENTER_ID_EURONEXT_FUTURES:
            return "EURONEXT_FUTURES";
        case  MARKET_CENTER_ID_HKEX_FUTURES:
            return "HKEX_FUTURES";
        case MARKET_CENTER_ID_KRX_FUTURES:
            return "KRX_FUTURES";
        case MARKET_CENTER_ID_LME:
            return "LME";
        case MARKET_CENTER_ID_NODAL:
            return "NODAL";
        case MARKET_CENTER_ID_OSAKA:
            return "OSAKA";
        case MARKET_CENTER_ID_SHANGHAI_FUTURES:
            return "SHANGHAI_FUTURES";
        case MARKET_CENTER_ID_ZHENGZHOU_FUTURES:
            return "ZHENGZHOU_FUTURES";
        case MARKET_CENTER_ID_MONTREAL:
            return "MONTREAL";
        case MARKET_CENTER_ID_NASDAQ_FUTURES:
            return "NASDAQ_FUTURES";
        case MARKET_CENTER_ID_TOCOM:
            return "TOCOM";
        case MARKET_CENTER_ID_SGX:
            return "SGX";
        case MARKET_CENTER_ID_BOND_BTEC:
            return "BOND_BTEC";
        case MARKET_CENTER_ID_BOND_ESPD:
            return "BOND_ESPD";
        case MARKET_CENTER_ID_AMEX_OPTIONS:
            return "AMEX_OPTIONS";
        case MARKET_CENTER_ID_ARCA_OPTIONS:
            return "ARCA_OPTIONS";
        case MARKET_CENTER_ID_BATS_OPTIONS:
            return "BATS_OPTIONS";
        case MARKET_CENTER_ID_BOX_OPTIONS:
            return "BOX_OPTIONS";
        case MARKET_CENTER_ID_CBOE_OPTIONS:
            return "CBOE_OPTIONS";
        case MARKET_CENTER_ID_C2_OPTIONS:
            return "C2_OPTIONS";
        case MARKET_CENTER_ID_EMERALD_OPTIONS:
            return "EMERALD_OPTIONS";
        case MARKET_CENTER_ID_ISE_OPTIONS:
            return "ISE_OPTIONS";
        case MARKET_CENTER_ID_OPRA_OPTIONS:
            return "OPRA_OPTIONS";
        case MARKET_CENTER_ID_NASDAQ_OPTIONS:
            return "NASDAQ_OPTIONS";
        case MARKET_CENTER_ID_PHLX_OPTIONS:
            return "PHLX_OPTIONS";
        case MARKET_CENTER_ID_GEMINI_OPTIONS:
            return "GEMINI_OPTIONS";
        case MARKET_CENTER_ID_MIAMI_OPTIONS:
            return "MIAMI_OPTIONS";
        case MARKET_CENTER_ID_OMX_BX_OPTIONS:
            return "OMX_BX_OPTIONS";
        case MARKET_CENTER_ID_EDGX_OPTIONS:
            return "EDGX_OPTIONS";
        case MARKET_CENTER_ID_MERCURY_OPTIONS:
            return "MERCURY_OPTIONS";
        case MARKET_CENTER_ID_PEARL_OPTIONS:
            return "PEARL_OPTIONS";
        case MARKET_CENTER_ID_SMART_ROUTER:
            return "SMART_ROUTER";
        case MARKET_CENTER_ID_ATS_BIDS:
            return "ATS_BIDS";
        case MARKET_CENTER_ID_ATS_CBX:
            return "ATS_CBX";
        case MARKET_CENTER_ID_ATS_CRSF:
            return "ATS_CRSF";
        case MARKET_CENTER_ID_ATS_CSLP:
            return "ATS_CSLP";
        case MARKET_CENTER_ID_ATS_KLINK:
            return "ATS_KLINK";
        case MARKET_CENTER_ID_ATS_KMATCH:
            return "ATS_KMATCH";
        case MARKET_CENTER_ID_ATS_LAVA:
            return "ATS_LAVA";
        case MARKET_CENTER_ID_ATS_LEVEL:
            return "ATS_LEVEL";
        case MARKET_CENTER_ID_ATS_LX:
            return "ATS_LX";
        case MARKET_CENTER_ID_ATS_MILM:
            return "ATS_MILM";
        case MARKET_CENTER_ID_ATS_MLXN:
            return "ATS_MLXN";
        case MARKET_CENTER_ID_ATS_POSIT:
            return "ATS_POSIT";
        case MARKET_CENTER_ID_ATS_SIGMA_X:
            return "ATS_SIGMA_X";
        case MARKET_CENTER_ID_ATS_VORTX:
            return "ATS_VORTX";
        case MARKET_CENTER_ID_ATS_CDEL:
            return "ATS_CDEL";
        case MARKET_CENTER_ID_ATS_DBSX:
            return "ATS_DBSX";
        case MARKET_CENTER_ID_ATS_GTCO:
            return "ATS_GTCO";
        case MARKET_CENTER_ID_ATS_JPMX:
            return "ATS_JPMX";
        case MARKET_CENTER_ID_ATS_PDQ:
            return "ATS_PDQ";
        case MARKET_CENTER_ID_ATS_UBSA:
            return "ATS_UBSA";
        case MARKET_CENTER_ID_MM_TWOSIGMA:
            return "MM_TWOSIGMA";
        case MARKET_CENTER_ID_MM_VIRTU:
            return "MM_VIRTU";
        case MARKET_CENTER_ID_CA_DARK_ICX:
            return "CA_DARK_ICX";
        case MARKET_CENTER_ID_CA_DARK_LIQUIDNET:
            return "CA_DARK_LIQUIDNET";
        case MARKET_CENTER_ID_CA_DARK_MATCHNOW:
            return "CA_DARK_MATCHNOW";
        case MARKET_CENTER_ID_CA_DARK_NASDAQ_CXD:
            return "CA_DARK_NASDAQ_CXD";
        case MARKET_CENTER_ID_SMART_ROUTER_OPTIONS:
            return "SMART_ROUTER_OPTIONS";
        case MARKET_CENTER_ID_FX_CURRENEX:
            return "FX_CURRENEX";
        case MARKET_CENTER_ID_FX_HOTSPOT:
            return "FX_HOTSPOT";
        case MARKET_CENTER_ID_FX_EBS:
            return "FX_EBS";
        case MARKET_CENTER_ID_FX_REUTERS:
            return "FX_REUTERS";
        case MARKET_CENTER_ID_FX_FXALL:
            return "FX_FXALL";
        case MARKET_CENTER_ID_FX_LMAX:
            return "FX_LMAX";
        case MARKET_CENTER_ID_FX_OTC:
            return "FX_OTC";
        case MARKET_CENTER_ID_CA_AEQUITAS_LIT:
            return "CA_AEQUITAS_LIT";
        case MARKET_CENTER_ID_CA_AEQUITAS_NEO:
            return "CA_AEQUITAS_NEO";
        case MARKET_CENTER_ID_CA_ALPHA:
            return "CA_ALPHA";
        case MARKET_CENTER_ID_CA_CSE:
            return "CA_CSE";
        case MARKET_CENTER_ID_CA_LYNX:
            return "CA_LYNX";
        case MARKET_CENTER_ID_CA_NASDAQ_CXC:
            return "CA_NASDAQ_CXC";
        case MARKET_CENTER_ID_CA_NASDAQ_CX2:
            return "CA_NASDAQ_CX2";
        case MARKET_CENTER_ID_CA_OMEGA:
            return "CA_OMEGA";
        case MARKET_CENTER_ID_CA_TSX:
            return "CA_TSX";
        case MARKET_CENTER_ID_CA_TSXV:
            return "CA_TSXV";
        case MARKET_CENTER_ID_BOVESPA:
            return "BOVESPA";
        case MARKET_CENTER_ID_LSE:
            return "LSE";
        case MARKET_CENTER_ID_EU_XETR:
            return "EU_XETR";
        case MARKET_CENTER_ID_EU_XPAR:
            return "EU_XPAR";
        case MARKET_CENTER_ID_EU_XAMS:
            return "EU_XAMS";
        case MARKET_CENTER_ID_EU_XBRU:
            return "EU_XBRU";
        case MARKET_CENTER_ID_EU_XLIS:
            return "EU_XLIS";
        case MARKET_CENTER_ID_EU_MTAA:
            return "EU_MTAA";
        case MARKET_CENTER_ID_EU_XSWX:
            return "EU_XSWX";
        case MARKET_CENTER_ID_EU_XVTX:
            return "EU_XVTX";
        case MARKET_CENTER_ID_EU_XMCE:
            return "EU_XMCE";
        case MARKET_CENTER_ID_EU_XSTO:
            return "EU_XSTO";
        case MARKET_CENTER_ID_EU_XHEL:
            return "EU_XHEL";
        case MARKET_CENTER_ID_EU_XCSE:
            return "EU_XCSE";
        case MARKET_CENTER_ID_EU_TRQX:
            return "EU_TRQX";
        case MARKET_CENTER_ID_EU_CHIX:
            return "EU_CHIX";
        case MARKET_CENTER_ID_EU_BATE:
            return "EU_BATE";
        case MARKET_CENTER_ID_AP_XIDX:
            return "AP_XIDX";
        case MARKET_CENTER_ID_AP_XKLS:
            return "AP_XKLS";
        case MARKET_CENTER_ID_AP_XSES:
            return "AP_XSES";
        default:
            return "UNKNOWN";
    }
}

/**
 * Converts a string to a MarketCenterID, expecting a string as the portion of the enum name following 'MARKET_CENTER_ID_'
 */
MarketCenterID MarketCenterFromString(const std::string& market_center);

/**
 * Converts a MIC string to a MarketCenterID
 */
MarketCenterID MarketCenterFromMic(const std::string& mic);

inline MarketCenterID USEquityExchangeFromChar(char chrExchange)
{
    switch (chrExchange) {
        case 'A':
            return MARKET_CENTER_ID_AMEX;
        case 'B':
            return MARKET_CENTER_ID_BSE;
        case 'C':
            return MARKET_CENTER_ID_NSX;
        case 'D':
            return MARKET_CENTER_ID_FINRA;
        case 'E':
            return MARKET_CENTER_ID_CONSOLIDATED_SIP;
        case 'H':
            return MARKET_CENTER_ID_MIAX;
        case 'I':
            return MARKET_CENTER_ID_ISE;
        case 'J':
            return MARKET_CENTER_ID_EDGA;
        case 'K':
            return MARKET_CENTER_ID_EDGX;
        case 'L':
            return MARKET_CENTER_ID_LTSE;
        case 'M':
            return MARKET_CENTER_ID_CHICAGO;
        case 'N':
            return MARKET_CENTER_ID_NYSE;
        case 'P':
            return MARKET_CENTER_ID_ARCA;
        case 'Q':
            return MARKET_CENTER_ID_NASDAQ;
        case 'S':
            return MARKET_CENTER_ID_CONSOLIDATED_CTS;
        case 'T':
            return MARKET_CENTER_ID_NASDAQ;
        case 'U':
            return MARKET_CENTER_ID_MEMX;
        case 'V':
            return MARKET_CENTER_ID_IEX;
        case 'W':
            return MARKET_CENTER_ID_CBOESTOCK;
        case 'X':
            return MARKET_CENTER_ID_PHLX;
        case 'Y':
            return MARKET_CENTER_ID_BYX;
        case 'Z':
            return MARKET_CENTER_ID_BATS;
        default:
            return MARKET_CENTER_ID_UNKNOWN;
    }
}

/**
 * Returns the MarketCenterID corresponding to a US equity options exchange's character code
 *
 * @param exchange_code A character corresponding to the OPRA Participant ID field
 */
inline MarketModels::MarketCenterID USOptionsExchangeFromChar(char exchange_code)
{
    switch (exchange_code) {
        case 'A':
            return MARKET_CENTER_ID_AMEX_OPTIONS;
        case 'B':
            return MARKET_CENTER_ID_BOX_OPTIONS;
        case 'C':
            return MARKET_CENTER_ID_CBOE_OPTIONS;
        case 'D':
            return MARKET_CENTER_ID_EMERALD_OPTIONS;
        case 'E':
            return MARKET_CENTER_ID_EDGX_OPTIONS;
        case 'H':
            return MARKET_CENTER_ID_GEMINI_OPTIONS;
        case 'I':
            return MARKET_CENTER_ID_ISE_OPTIONS;
        case 'J':
            return MARKET_CENTER_ID_MERCURY_OPTIONS;
        case 'M':
            return MARKET_CENTER_ID_MIAMI_OPTIONS;
        case 'N':
            return MARKET_CENTER_ID_ARCA_OPTIONS;
        case 'O':
            return MARKET_CENTER_ID_OPRA_OPTIONS;
        case 'P':
            return MARKET_CENTER_ID_PEARL_OPTIONS;
        case 'Q':
            return MARKET_CENTER_ID_NASDAQ_OPTIONS;
        case 'T':
            return MARKET_CENTER_ID_OMX_BX_OPTIONS;
        case 'W':
            return MARKET_CENTER_ID_C2_OPTIONS;
        case 'X':
            return MARKET_CENTER_ID_PHLX_OPTIONS;
        case 'Z':
            return MARKET_CENTER_ID_BATS_OPTIONS;
        default:
            return MARKET_CENTER_ID_UNKNOWN;
    }
}

/**
 * Returns whether the market center id is for an equity exchange
 *
 * @param market_center Strategy Studio's market center identifier
 */
inline bool IsEquityExchange(MarketCenterID market_center)
{
    if (market_center != MARKET_CENTER_ID_UNKNOWN && (market_center <= MARKET_CENTER_ID_IEX || market_center >= MARKET_CENTER_ID_CA_AEQUITAS_LIT))
        return true;
    else
        return false;
}

/**
 * Returns whether the market center id is for an equity ATS (dark pool) or smart order router
 *
 * @param market_center Strategy Studio's market center identifier
 */
inline bool IsEquityATS(MarketCenterID market_center)
{
    if (market_center >= MARKET_CENTER_ID_SMART_ROUTER && market_center <= MARKET_CENTER_ID_CA_DARK_NASDAQ_CXD)
        return true;
    else
        return false;
}

/**
 * Returns whether the market center id is for a futures exchange
 *
 * @param market_center Strategy Studio's market center identifier
 */
inline bool IsFuturesExchange(MarketCenterID market_center)
{
    if ((market_center >= MARKET_CENTER_ID_CME_GLOBEX && market_center <= MARKET_CENTER_ID_SGX) || market_center == MARKET_CENTER_ID_BOVESPA)
        return true;
    else
        return false;
}

/**
 * Returns whether the market center id is for an options exchange
 *
 * @param market_center Strategy Studio's market center identifier
 */
inline bool IsOptionsExchange(MarketCenterID market_center)
{
    if (market_center >= MARKET_CENTER_ID_AMEX_OPTIONS && market_center <= MARKET_CENTER_ID_PEARL_OPTIONS)
        return true;
    else
        return false;
}

/**
 * Returns whether the market center id is for an optinos ATS (dark pool) or smart order router
 *
 * @param market_center Strategy Studio's market center identifier
 */
inline bool IsOptionsATS(MarketCenterID market_center)
{
    return (market_center == MARKET_CENTER_ID_SMART_ROUTER_OPTIONS);
}

inline bool IsDerivativesExchange(MarketCenterID market_center)
{
    if (IsFuturesExchange(market_center) || IsOptionsExchange(market_center))
        return true;
    else
        return false;
}

inline bool IsFXMarketCenter(MarketCenterID market_center)
{
    if (market_center >= MARKET_CENTER_ID_FX_CURRENEX && market_center <= MARKET_CENTER_ID_FX_OTC)
        return true;
    else
        return false;
}

inline bool IsBondMarketCenter(MarketCenterID market_center)
{
    if (market_center >= MARKET_CENTER_ID_BOND_BTEC && market_center <= MARKET_CENTER_ID_BOND_ESPD)
        return true;
    else
        return false;
}

enum FeedType {
    FEED_TYPE_NONE = 0, // indicates no feed
    FEED_TYPE_CONSOLIDATED = 1, // US equity consolidated feed
    FEED_TYPE_DIRECT = 2, // direct market center feed
    FEED_TYPE_DEPTH = 3, // depth of book feed
    FEED_TYPE_TOP = 4, // internal calculated top of book feed
    FEED_TYPE_BEST = 5 // uses the best available feed for given data adapter
};

enum MarketType {
    MARKET_TYPE_UNKNOWN = 0,
    MARKET_TYPE_US_EQUITIES = 1,
    MARKET_TYPE_GLOBEX = 2,
    MARKET_TYPE_US_EQUITY_OPTIONS = 3,
    MARKET_TYPE_SPOT_FX = 4,
    MARKET_TYPE_BRAZIL = 5,
    MARKET_TYPE_LONDON = 6,
    MARKET_TYPE_FRANKFORT = 7,
    MARKET_TYPE_PARIS = 8,
    MARKET_TYPE_MILAN = 9,
    MARKET_TYPE_ICE = 10,
    MARKET_TYPE_EUREX = 11,
    MARKET_TYPE_CBOE_FUTURES = 12,
    MARKET_TYPE_BOND = 13,
    MARKET_TYPE_INDONESIA = 14,
    MARKET_TYPE_MALAYSIA = 15,
    MARKET_TYPE_SINGAPORE = 16,
    MARKET_TYPE_CA_EQUITIES = 17,
    MARKET_TYPE_ASX = 18,
    MARKET_TYPE_MONTREAL = 19,
    MARKET_TYPE_NASDAQ_FUTURES = 20,
    MARKET_TYPE_TOCOM = 21,
    MARKET_TYPE_SGX = 22,
    MARKET_TYPE_CFFEX = 23,
    MARKET_TYPE_DALIAN_FUTURES = 24,
    MARKET_TYPE_EURONEXT_FUTURES = 25,
    MARKET_TYPE_HKEX_FUTURES = 26,
    MARKET_TYPE_KRX_FUTURES = 27,
    MARKET_TYPE_LME = 28,
    MARKET_TYPE_NODAL = 29,
    MARKET_TYPE_OSAKA = 30,
    MARKET_TYPE_SHANGHAI_FUTURES = 31,
    MARKET_TYPE_ZHENGZHOU_FUTURES = 32
};

inline std::string MarketTypeToString(MarketType marketType)
{
    switch(marketType) {
        case MARKET_TYPE_US_EQUITIES:
            return "US_EQUITIES";
        case MARKET_TYPE_GLOBEX:
            return "GLOBEX";
        case MARKET_TYPE_US_EQUITY_OPTIONS:
            return "US_EQUITY_OPTIONS";
        case MARKET_TYPE_SPOT_FX:
            return "SPOT_FX";
        case MARKET_TYPE_BRAZIL:
            return "BRAZIL";
        case MARKET_TYPE_LONDON:
            return "LONDON";
        case MARKET_TYPE_FRANKFORT:
            return "FRANKFORT";
        case MARKET_TYPE_PARIS:
            return "PARIS";
        case MARKET_TYPE_MILAN:
            return "MILAN";
        case MARKET_TYPE_ICE:
            return "ICE";
        case MARKET_TYPE_EUREX:
            return "EUREX";
        case MARKET_TYPE_CBOE_FUTURES:
            return "CBOE_FUTURES";
        case MARKET_TYPE_BOND:
            return "BOND";
        case MARKET_TYPE_INDONESIA:
            return "INDONESIA";
        case MARKET_TYPE_MALAYSIA:
            return "MALAYSIA";
        case MARKET_TYPE_SINGAPORE:
            return "SINGAPORE";
        case MARKET_TYPE_CA_EQUITIES:
            return "CA_EQUITIES";
        case MARKET_TYPE_ASX:
            return "ASX";
        case MARKET_TYPE_MONTREAL:
            return "MONTREAL";
        case MARKET_TYPE_NASDAQ_FUTURES:
            return "NASDAQ_FUTURES";
        case MARKET_TYPE_TOCOM:
            return "TOCOM";
        case MARKET_TYPE_SGX:
            return "SGX";
        case MARKET_TYPE_CFFEX:
            return "CFFEX";
        case MARKET_TYPE_DALIAN_FUTURES:
            return "DALIAN_FUTURES";
        case MARKET_TYPE_EURONEXT_FUTURES:
            return "EURONEXT_FUTURES";
        case MARKET_TYPE_HKEX_FUTURES:
            return "HKEX_FUTURES";
        case MARKET_TYPE_KRX_FUTURES:
            return "KRX_FUTURES";
        case MARKET_TYPE_LME:
            return "LME";
        case MARKET_TYPE_NODAL:
            return "NODAL";
        case MARKET_TYPE_OSAKA:
            return "OSAKA";
        case MARKET_TYPE_SHANGHAI_FUTURES:
            return "SHANGHAI_FUTURES";
        case MARKET_TYPE_ZHENGZHOU_FUTURES:
            return "ZHENGZHOU_FUTURES";
        case MARKET_TYPE_UNKNOWN:
        default:
            return "UNKNOWN";
    }
}

/**
 * Converts a string to a MarketType, expecting a string as the portion of the enum name following 'MARKET_TYPE_'
 */
MarketType MarketTypeFromString(const std::string& marketType);

inline FeedType StringToFeedType(const std::string& feedType)
{
    if (feedType.compare("CONSOLIDATED") == 0) {
        return FEED_TYPE_CONSOLIDATED;
    } else if (feedType.compare("DIRECT") == 0) {
        return FEED_TYPE_DIRECT;
    } else if (feedType.compare("DEPTH") == 0) {
        return FEED_TYPE_DEPTH;
    } else if (feedType.compare("TOP") == 0) {
        return FEED_TYPE_TOP;
    } else if (feedType.compare("BEST") == 0) {
        return FEED_TYPE_BEST;
    } else if (feedType.compare("NONE") == 0) {
        return FEED_TYPE_NONE;
    } else {
        throw Utilities::StrategyStudioException("Unknown feed type string");
    }
}

inline std::string ToString(FeedType feedType)
{
    switch(feedType) {
        case FEED_TYPE_NONE:
            return "NONE";
        case FEED_TYPE_CONSOLIDATED:
            return "CONSOLIDATED";
        case FEED_TYPE_DIRECT:
            return "DIRECT";
        case FEED_TYPE_DEPTH:
            return "DEPTH";
        case FEED_TYPE_TOP:
            return "TOP";
        case FEED_TYPE_BEST:
            return"BEST";
        default:
            return "UNKNOWN";
    }
}

/**
 * Converts a MarketCenterID to its string representation
 *
 * Deprecated, use MarketCenterToString
 */
inline std::string ExchangeToString(MarketCenterID exchType) { return MarketCenterToString(exchType); }

/**
 * Converts a MarketCenterID to its string representation
 *
 * Deprecated, use MarketTypeFromString
 */
inline MarketType StringToMarketType(const std::string& marketType) { return MarketTypeFromString(marketType); }

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif
