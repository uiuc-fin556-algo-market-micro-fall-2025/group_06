/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/MarketModels/CurrencyTypes.h
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

#ifndef _STRATEGY_STUDIO_MARKET_MODELS_CURRENCY_TYPES_H_
#define _STRATEGY_STUDIO_MARKET_MODELS_CURRENCY_TYPES_H_

#include <string>

namespace RCM {
namespace StrategyStudio {
namespace MarketModels {

/**
 * Enumerates different kinds of currencies
 */
enum CurrencyType {
    CURRENCY_TYPE_UNKNOWN = 0,
    CURRENCY_TYPE_AED = 784, // United Arab Emirates dirham
    CURRENCY_TYPE_AFN = 971, // Afghan afghani
    CURRENCY_TYPE_ALL = 8,   // Albanian lek
    CURRENCY_TYPE_AMD = 51,  // Armenian dram
    CURRENCY_TYPE_ANG = 532, // Netherlands Antillean guilder
    CURRENCY_TYPE_AOA = 973, // Angolan kwanza
    CURRENCY_TYPE_ARS = 32,  // Argentine peso
    CURRENCY_TYPE_AUD = 36,  // Australian dollar
    CURRENCY_TYPE_AWG = 533, // Aruban florin
    CURRENCY_TYPE_AZN = 944, // Azerbaijani manat
    CURRENCY_TYPE_BAM = 977, // Bosnia and Herzegovina convertible mark
    CURRENCY_TYPE_BBD = 52,  // Barbados dollar
    CURRENCY_TYPE_BDT = 50,  // Bangladeshi taka
    CURRENCY_TYPE_BGN = 975, // Bulgarian lev
    CURRENCY_TYPE_BHD = 48,  // Bahraini dinar
    CURRENCY_TYPE_BIF = 108, // Burundian franc
    CURRENCY_TYPE_BMD = 60,  // Bermudian dollar(customarily known as Bermuda dollar)
    CURRENCY_TYPE_BND = 96,  // Brunei dollar
    CURRENCY_TYPE_BOB = 68,  // Boliviano
    CURRENCY_TYPE_BOV = 984, // Bolivian Mvdol (funds code)
    CURRENCY_TYPE_BRL = 986, // Brazilian real
    CURRENCY_TYPE_BSD = 44,  // Bahamian dollar
    CURRENCY_TYPE_BTN = 64,  // Bhutanese ngultrum
    CURRENCY_TYPE_BWP = 72,  // Botswana pula
    CURRENCY_TYPE_BYR = 974, // Belarusian ruble
    CURRENCY_TYPE_BZD = 84,  // Belize dollar
    CURRENCY_TYPE_CAD = 124, // Canadian dollar
    CURRENCY_TYPE_CDF = 976, // Congolese franc
    CURRENCY_TYPE_CHE = 947, // WIR Bank(complementary currency)
    CURRENCY_TYPE_CHF = 756, // Swiss franc
    CURRENCY_TYPE_CHW = 948, // WIR Bank(complementary currency)
    CURRENCY_TYPE_CLF = 990, // Unidad de Fomento (funds code)
    CURRENCY_TYPE_CLP = 152, // Chilean peso
    CURRENCY_TYPE_CNY = 156, // Chinese yuan
    CURRENCY_TYPE_CNH = 157, // Deliverable Chinese yuan exchanged outside of China (Hong Kong and Singapore) - not true ISO code
    CURRENCY_TYPE_COP = 170, // Colombian peso
    CURRENCY_TYPE_COU = 970, // Unidad de Valor Real
    CURRENCY_TYPE_CRC = 188, // Costa Rican colon
    CURRENCY_TYPE_CUC = 931, // Cuban convertible peso
    CURRENCY_TYPE_CUP = 192, // Cuban peso
    CURRENCY_TYPE_CVE = 132, // Cape Verde escudo
    CURRENCY_TYPE_CZK = 203, // Czech koruna
    CURRENCY_TYPE_DJF = 262, // Djiboutian franc
    CURRENCY_TYPE_DKK = 208, // Danish krone
    CURRENCY_TYPE_DOP = 214, // Dominican peso
    CURRENCY_TYPE_DZD = 12,  // Algerian dinar
    CURRENCY_TYPE_EEK = 233, // Estonian kroon
    CURRENCY_TYPE_EGP = 818, // Egyptian pound
    CURRENCY_TYPE_ERN = 232, // Eritrean nakfa
    CURRENCY_TYPE_ETB = 230, // Ethiopian birr
    CURRENCY_TYPE_EUR = 978, // Euro
    CURRENCY_TYPE_FJD = 242, // Fiji dollar
    CURRENCY_TYPE_FKP = 238, // Falkland Islands pound
    CURRENCY_TYPE_GBP = 826, // Pound sterling
    CURRENCY_TYPE_GBX = 827, // Pence sterling, common on LSE, not a true ISO code. 1/100 GBP
    CURRENCY_TYPE_GEL = 981, // Georgian lari
    CURRENCY_TYPE_GHS = 936, // Ghanaian cedi
    CURRENCY_TYPE_GIP = 292, // Gibraltar pound
    CURRENCY_TYPE_GMD = 270, // Gambian dalasi
    CURRENCY_TYPE_GNF = 324, // Guinean franc
    CURRENCY_TYPE_GTQ = 320, // Guatemalan quetzal
    CURRENCY_TYPE_GYD = 328, // Guyanese dollar
    CURRENCY_TYPE_HKD = 344, // Hong Kong dollar
    CURRENCY_TYPE_HNL = 340, // Honduran lempira
    CURRENCY_TYPE_HRK = 191, // Croatian kuna
    CURRENCY_TYPE_HTG = 332, // Haitian gourde
    CURRENCY_TYPE_HUF = 348, // Hungarian forint
    CURRENCY_TYPE_IDR = 360, // Indonesian rupiah
    CURRENCY_TYPE_ILS = 376, // Israeli new sheqel
    CURRENCY_TYPE_INR = 356, // Indian rupee
    CURRENCY_TYPE_IQD = 368, // Iraqi dinar
    CURRENCY_TYPE_IRR = 364, // Iranian rial
    CURRENCY_TYPE_ISK = 352, // Icelandic króna
    CURRENCY_TYPE_JMD = 388, // Jamaican dollar
    CURRENCY_TYPE_JOD = 400, // Jordanian dinar
    CURRENCY_TYPE_JPY = 392, // Japanese yen
    CURRENCY_TYPE_KES = 404, // Kenyan shilling
    CURRENCY_TYPE_KGS = 417, // Kyrgyzstani som
    CURRENCY_TYPE_KHR = 116, // Cambodian riel
    CURRENCY_TYPE_KMF = 174, // Comoro franc
    CURRENCY_TYPE_KPW = 408, // North Korean won
    CURRENCY_TYPE_KRW = 410, // South Korean won
    CURRENCY_TYPE_KWD = 414, // Kuwaiti dinar
    CURRENCY_TYPE_KYD = 136, // Cayman Islands dollar
    CURRENCY_TYPE_KZT = 398, // Kazakhstani tenge
    CURRENCY_TYPE_LAK = 418, // Lao kip
    CURRENCY_TYPE_LBP = 422, // Lebanese pound
    CURRENCY_TYPE_LKR = 144, // Sri Lanka rupee
    CURRENCY_TYPE_LRD = 430, // Liberian dollar
    CURRENCY_TYPE_LSL = 426, // Lesotho loti
    CURRENCY_TYPE_LTL = 440, // Lithuanian litas
    CURRENCY_TYPE_LVL = 428, // Latvian lats
    CURRENCY_TYPE_LYD = 434, // Libyan dinar
    CURRENCY_TYPE_MAD = 504, // Moroccan dirham
    CURRENCY_TYPE_MDL = 498, // Moldovan leu
    CURRENCY_TYPE_MGA = 969, // Malagasy ariary
    CURRENCY_TYPE_MKD = 807, // Macedonian denar
    CURRENCY_TYPE_MMK = 104, // Myanma kyat
    CURRENCY_TYPE_MNT = 496, // Mongolian tugrik
    CURRENCY_TYPE_MOP = 446, // Macanese pataca
    CURRENCY_TYPE_MRO = 478, // Mauritanian ouguiya
    CURRENCY_TYPE_MUR = 480, // Mauritian rupee
    CURRENCY_TYPE_MVR = 462, // Maldivian rufiyaa
    CURRENCY_TYPE_MWK = 454, // Malawian kwacha
    CURRENCY_TYPE_MXN = 484, // Mexican peso
    CURRENCY_TYPE_MXV = 979, // Mexican Unidad de Inversion(UDI) (funds code)
    CURRENCY_TYPE_MYR = 458, // Malaysian ringgit
    CURRENCY_TYPE_MZN = 943, // Mozambican metical
    CURRENCY_TYPE_NAD = 516, // Namibian dollar
    CURRENCY_TYPE_NGN = 566, // Nigerian naira
    CURRENCY_TYPE_NIO = 558, // Cordoba oro
    CURRENCY_TYPE_NOK = 578, // Norwegian krone
    CURRENCY_TYPE_NPR = 524, // Nepalese rupee
    CURRENCY_TYPE_NZD = 554, // New Zealand dollar
    CURRENCY_TYPE_OMR = 512, // Omani rial
    CURRENCY_TYPE_PAB = 590, // Panamanian balboa
    CURRENCY_TYPE_PEN = 604, // Peruvian nuevo sol
    CURRENCY_TYPE_PGK = 598, // Papua New Guinean kina
    CURRENCY_TYPE_PHP = 608, // Philippine peso
    CURRENCY_TYPE_PKR = 586, // Pakistani rupee
    CURRENCY_TYPE_PLN = 985, // Polish złoty
    CURRENCY_TYPE_PYG = 600, // Paraguayan guaraní
    CURRENCY_TYPE_QAR = 634, // Qatari rial
    CURRENCY_TYPE_RON = 946, // Romanian new leu
    CURRENCY_TYPE_RSD = 941, // Serbian dinar
    CURRENCY_TYPE_RUB = 643, // Russian rouble
    CURRENCY_TYPE_RWF = 646, // Rwandan franc
    CURRENCY_TYPE_SAR = 682, // Saudi riyal
    CURRENCY_TYPE_SBD = 90,  // Solomon Islands dollar
    CURRENCY_TYPE_SCR = 690, // Seychelles rupee
    CURRENCY_TYPE_SDG = 938, // Sudanese pound
    CURRENCY_TYPE_SEK = 752, // Swedish krona/kronor
    CURRENCY_TYPE_SGD = 702, // Singapore dollar
    CURRENCY_TYPE_SHP = 654, // Saint Helena pound
    CURRENCY_TYPE_SLL = 694, // Sierra Leonean leone
    CURRENCY_TYPE_SOS = 706, // Somali shilling
    CURRENCY_TYPE_SRD = 968, // Surinamese dollar
    CURRENCY_TYPE_STD = 678, // São Tomé and Príncipe dobra
    CURRENCY_TYPE_SYP = 760, // Syrian pound
    CURRENCY_TYPE_SZL = 748, // Lilangeni
    CURRENCY_TYPE_THB = 764, // Thai baht
    CURRENCY_TYPE_TJS = 972, // Tajikistani somoni
    CURRENCY_TYPE_TMT = 934, // Turkmenistani manat
    CURRENCY_TYPE_TND = 788, // Tunisian dinar
    CURRENCY_TYPE_TOP = 776, // Tongan paʻanga
    CURRENCY_TYPE_TRY = 949, // Turkish lira
    CURRENCY_TYPE_TTD = 780, // Trinidad and Tobago dollar
    CURRENCY_TYPE_TWD = 901, // New Taiwan dollar
    CURRENCY_TYPE_TZS = 834, // Tanzanian shilling
    CURRENCY_TYPE_UAH = 980, // Ukrainian hryvnia
    CURRENCY_TYPE_UGX = 800, // Ugandan shilling
    CURRENCY_TYPE_USD = 840, // United States dollar
    CURRENCY_TYPE_USX = 841, // United States cents, not a true ISO code. 1/100 USD
    CURRENCY_TYPE_USN = 997, // United States dollar (next day) (funds code)
    CURRENCY_TYPE_USS = 998, // United States dollar (same day) (funds code) (one source[who?]claims it is no longer used, but it is still on the ISO 4217-MA list)
    CURRENCY_TYPE_UYU = 858, // Uruguayan peso
    CURRENCY_TYPE_UZS = 860, // Uzbekistan som
    CURRENCY_TYPE_VEF = 937, // Venezuelan bolívar fuerte
    CURRENCY_TYPE_VND = 704, // Vietnamese đồng
    CURRENCY_TYPE_VUV = 548, // Vanuatu vatu
    CURRENCY_TYPE_WST = 882, // Samoan tala
    CURRENCY_TYPE_XAF = 950, // CFA franc BEAC
    CURRENCY_TYPE_XAG = 961, // Silver (one troy ounce)
    CURRENCY_TYPE_XAU = 959, // Gold (one troy ounce)
    CURRENCY_TYPE_XBA = 955, // European Composite Unit(EURCO) (bond market unit)
    CURRENCY_TYPE_XBB = 956, // European Monetary Unit(E.M.U.-6) (bond market unit)
    CURRENCY_TYPE_XBC = 957, // European Unit of Account 9(E.U.A.-9) (bond market unit)
    CURRENCY_TYPE_XBD = 958, // European Unit of Account 17(E.U.A.-17) (bond market unit)
    CURRENCY_TYPE_XCD = 951, // East Caribbean dollar
    CURRENCY_TYPE_XDR = 960, // Special Drawing Rights
    CURRENCY_TYPE_XFU = 999, // UIC franc (special settlement currency) (not a real value)
    CURRENCY_TYPE_XOF = 952, // CFA Franc BCEAO
    CURRENCY_TYPE_XPD = 964, // Palladium (onetroy ounce)
    CURRENCY_TYPE_XPF = 953, // CFP franc
    CURRENCY_TYPE_XPT = 962, // Platinum (onetroy ounce)
    CURRENCY_TYPE_XTS = 963, // Code reserved for testing purposes
    CURRENCY_TYPE_YER = 886, // Yemeni rial
    CURRENCY_TYPE_ZAR = 710, // South African rand
    CURRENCY_TYPE_ZMK = 894, // Zambian kwacha
    CURRENCY_TYPE_ZWL = 932  // Zimbabwe dollar
};

/**
 * Contains helpers for dealing with currencies
 */
class CurrencyUtils {
public:
    /**
     * Convert a currency type to a string
     */
    static std::string CurrencyTypeToString(CurrencyType type);

    /**
     * Convert a string to a currency code
     */
    static CurrencyType StringToCurrencyType(const std::string& type);
};

} // namespace MarketModels
} // namespace StrategyStudio
} // namespace RCM

#endif
