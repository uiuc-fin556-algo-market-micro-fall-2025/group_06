/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/utils.h
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

#ifndef _STRATEGY_STUDIO_UTILITIES_UTILS_H_
#define _STRATEGY_STUDIO_UTILITIES_UTILS_H_

#pragma warning (push)
#pragma warning( disable: 4996)

#include "StdTypes.h"
#include <stdio.h>
#include <cstring>
#include <ctype.h>
#include <string>
#include <cmath>
#include <sys/stat.h>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/call_traits.hpp>
#include <boost/tokenizer.hpp>
#include <functional>
#include <algorithm>
#include <istream>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

struct PlaceholderIsPointer {};
struct PlaceholderIsValue {};

template <typename T>   // 'T' is a pointer type
inline bool PointerValueEqual(const T& lhs, const T& rhs)
{
    return *lhs == *rhs;
}

/**
 * Compares two stl-compliant containers to see all of their members are equal
 */
template<typename Container>
inline bool ContainerCompare(const Container& lhs, const Container& rhs)
{
    return lhs == rhs;
}

/**
 * Compares two stl-compliant containers to see all of their members are equal with pointer members
 */
template<typename Container>
inline bool ContainerPtrCompare(const Container& lhs, const Container& rhs)
{
    return ((lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin(), PointerValueEqual<typename Container::value_type>));
}

/**
 * Compares two stl-compliant associative containers to see all of their members are equal
 */
template<typename Container>
inline bool MapContainerCompare(const Container& lhs, const Container& rhs)
{
    return lhs == rhs;  // std::pair has operator==
}

/**
 * Compares two stl-compliant associative containers to see all of their members are equal with pointer members
 */
template<typename Container>
inline bool MapContainerPtrCompare(const Container& lhs, const Container& rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    // can't use std::equal since we can't customize Container::value_compare
    for (typename Container::const_iterator lhs_it = lhs.begin(), rhs_it = rhs.begin(); lhs_it != lhs.end(); ++lhs_it, ++rhs_it)
    {
        // reflexive test on key using associative container's ordering comparator
        if ((lhs.key_comp()(lhs_it->first, rhs_it->first) || lhs.key_comp()(rhs_it->first, lhs_it->first)) || (*(lhs_it->second) != *(rhs_it->second)))
            return false;
    }

    return true;
}

/**
 * Compares two stl-compliant associative containers to see all of their members are equal with value key and pointer member
 */
template<typename Container>
inline bool MapContainerKeyPtrCompare(const Container& lhs, const Container& rhs)
{
    return MapContainerPtrCompare(lhs, rhs);
}

/**
 * Copies all files within a source directory with a given extension to a target directory
 */
void CopyFilesInDirToTargetDir(const std::string& sourceDir,
                               const std::string& targetDir,
                               const std::string& fileExtension);

inline bool DoubleEquals(double left, double right, double epsilon)
{
    return (fabs(left - right) < epsilon);
}

template<int Precision>
struct DoubleEqualsCompPrecision : public std::binary_function<double, double, bool> {
    DoubleEqualsCompPrecision()
    {
        m_fEpsilon = pow(10.0, -Precision);
    }

    bool operator()(double lhs, double rhs) const
    {
        return DoubleEquals(lhs, rhs, m_fEpsilon);
    }

    double m_fEpsilon;
};

struct DoubleEqualsCompPrecision2: public std::binary_function<double, double, bool> {
    bool operator()(double lhs, double rhs) const
    {
        return DoubleEquals(lhs, rhs, .01);
    }
};


struct DoubleEqualsCompPrecision3: public std::binary_function<double, double, bool> {
    bool operator()(double lhs, double rhs) const
    {
        return DoubleEquals(lhs, rhs, .001);
    }
};

struct DoubleEqualsCompPrecision4: public std::binary_function<double, double, bool> {
    bool operator()(double lhs, double rhs) const
    {
        return DoubleEquals(lhs, rhs, .0001);
    }
};

struct DoubleEqualsCompPrecision5: public std::binary_function<double, double, bool> {
    bool operator()(double lhs, double rhs) const
    {
        return DoubleEquals(lhs, rhs, .00001);
    }
};

struct DoubleEqualsCompPrecision6: public std::binary_function<double, double, bool> {
    bool operator()(double lhs, double rhs) const
    {
        return DoubleEquals(lhs, rhs, .000001);
    }
};

struct DoubleEqualsCompPrecision7 : public std::binary_function<double, double, bool> {
    bool operator()(double lhs, double rhs) const
    {
        return DoubleEquals(lhs, rhs, .0000001);
    }
};

struct DoubleEqualsCompPrecision8 : public std::binary_function<double, double, bool> {
    bool operator()(double lhs, double rhs) const
    {
        return DoubleEquals(lhs, rhs, .00000001);
    }
};

struct DoubleEqualsCompPrecision9 : public std::binary_function<double, double, bool> {
    bool operator()(double lhs, double rhs) const
    {
        return DoubleEquals(lhs, rhs, 1e-9);
    }
};

typedef DoubleEqualsCompPrecision9 DoubleEqualsNotionalComp;
typedef DoubleEqualsCompPrecision9 DoubleEqualsCostComp;
typedef DoubleEqualsCompPrecision9 DoubleEqualsPnLComp;
typedef DoubleEqualsCompPrecision9 DoubleEqualsPriceComp;
extern const double PRICE_EPSILON;
extern const double QUANTITY_EPSILON;

template<typename T>
struct ByValuePtrLessComp: public std::binary_function<const T*, const T*, bool> {
    bool operator()(const T* lhs, const T* rhs) const
    {
        return (*lhs < *rhs);
    }
};

inline std::pair<std::string, std::string> ExtractToken(const std::string& token, char delim = '=')
{
    typedef std::pair<std::string, std::string> RetPair;

    size_t pos;
    if ((pos = token.find_first_of(delim)) == std::string::npos) {
        return RetPair();
    }

    // there could be a space here that we want to remove
    if ( (pos < (token.length() - 1) ) && (token[pos + 1] == ' ')) ++pos;

    return RetPair(token.substr(0, pos), token.substr(pos + 1, token.length() - pos));
}

inline std::string ExtractRight(const std::string& token, char delim = '=')
{
    size_t pos;
    if ((pos = token.find_first_of(delim)) == std::string::npos) {
        return std::string("");
    }

    // there could be a space here that we want to remove
    if ( (pos < (token.length() - 1) ) && (token[pos + 1] == ' ')) ++pos;

    return token.substr(pos + 1, token.length() - pos);
}

/**
 * Extract data into a string from an istream until a delim is hit
 */
inline std::string ExtractString(std::istream& in, char delim)
{
    std::string buf;
    char c;

    while (in.peek() != delim) {
        in >> c;
        buf += c;
    }

    in >> c;
    assert(c == delim);

    return buf;
}

/**
 * Read out data for a type into var
 */
template<typename T>
inline void ExtractType(std::istream& in, T* out, char delim)
{
    in >> *out;

    char c;
    in >> c;
    assert(c == delim);
}

/**
 * Round double to specified number of decimal places
 */
// all [current] uses are with static 'places', so make it a template like PriceToIntKeyFunc()
template <int N>
inline double RoundDouble(double num)
{
    static const double multiplier = pow(10.0, N);
    return (static_cast<StrategyStudioInt64>(num * multiplier + ((num < 0) ? -0.5 : 0.5)) / multiplier);
}

inline std::ostream& PrintScaledSeconds(std::ostream& out, double secs)
{
    double num = fabs(secs);

    if (num > 1) {
        out << num << " secs ";
    } else if (num >= .001) {
        out << (num * 1000) << " millisecs ";
    } else if (num >= .000001) {
        out << (num * 1000000) << " microsecs ";
    } else {
        out << (num * 1000000000) << " nanosecs ";
    }

    return out;
}

inline std::ostream& PrintScaledMilliseconds(std::ostream& out, double msecs)
{
    double num = fabs(msecs);

    if (num >= 1000) {
        out << (num / 1000.0) << " secs ";
    } else if (num > 1) {
        out << num << " millisecs ";
    } else if (num >= .001) {
        out << (num * 1000) << " microsecs ";
    } else {
        out << (num * 1000000) << " nanosecs ";
    }

    return out;
}

inline std::ostream& PrintScaledMicroseconds(std::ostream& out, double usecs)
{
    double num = fabs(usecs);

    if (num >= 1000000) {
        out << (num / 1000000.0) << " secs ";
    } else if (num >= 1000) {
        out << (num / 1000.0) << " millisecs ";
    } else if (num > 1) {
        out << num << " microsecs ";
    } else {
        out << (num * 1000) << " nanosecs ";
    }

    return out;
}

/**
 * Normalizes a price of double type to a key of int type by multiplying it
 * to a precision N. Rounding is specific to positive prices.
 */
template<int N>
class PriceToIntKeyFunc {
public:

    static constexpr int EXPONENT = N;

    StrategyStudioInt64 operator()(double price) const
    {
        static_assert(N >= 0 && N < sizeof(powers_of_ten) / sizeof(powers_of_ten[0]), "bad PriceToIntKeyFunc exponent");
        static constexpr double multiplier = powers_of_ten[N];
        return static_cast<StrategyStudioInt64>(price * multiplier + ((price < 0) ? -0.5 : 0.5));
    }

    static double reverse(StrategyStudioInt64 key)
    {
        static_assert(N >= 0 && N < sizeof(powers_of_ten) / sizeof(powers_of_ten[0]), "bad PriceToIntKeyFunc exponent");
        static constexpr double multiplier = 1.0 / powers_of_ten[N];
        return (key * multiplier);
    }

private:
    static constexpr double powers_of_ten[] = { 1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12 };
};

typedef PriceToIntKeyFunc<8> PriceHasher;

/**
 * Check if signs are equal for int
 */
template<typename NumericType>
inline bool SignsEqual(NumericType v1, NumericType v2) {
    return (((v1 > 0) - (v1 < 0)) == ((v2 > 0) - (v2 < 0)));
}

/**
 * Trim characters (marked by delim) from the beginning and end of a string
 * @returns c-style trimmed string
 */
inline char* trim(char* buf, char delim = ' ') {
    int len = static_cast<int>(strlen(buf));
    if (len < 2) {
        return buf;
    }

    int i = 0;
    int stripFromEnd = 0;
    int stripFromBeg = 0;
    //char* temp_buf = new char(len + 1);

    i = len - 1;
    while( (buf[i] == delim) && (--i > 0) )	{
        ++stripFromEnd;
    }

    i = 0;
    while( (buf[i] == delim) && (++i < len) ) {
        ++stripFromBeg;
    }

    int new_len = len - (stripFromBeg + stripFromEnd);
    if (new_len < 1) {
        buf[0] = '\0';
        return buf;
    }

    if ( (stripFromEnd > 0) || (stripFromBeg > 0) )	{
        strncpy(buf, &buf[stripFromBeg], new_len);
        buf[new_len] = '\0';
    }

    return buf;
}

/**
 * Converts a string to uppercase
 */
inline std::string StringToUpper(std::string strToConvert)
{//change each element of the string to upper case

   for (unsigned int i = 0; i < strToConvert.length(); i++) {
      strToConvert[i] = static_cast<char>(toupper(strToConvert[i]));
   }
   return strToConvert;//return the converted string
}

/**
 * Converts a string to lowercase
 */
inline std::string StringToLower(std::string strToConvert)
{//change each element of the string to lower case
   for (unsigned int i = 0; i < strToConvert.length(); i++) {
      strToConvert[i] = static_cast<char>(tolower(strToConvert[i]));
   }
   return strToConvert;//return the converted string
}

/**
 * Check if a file exists
 */
inline bool FileExists(const std::string& strFilename)
{
  struct stat stFileInfo;
  bool blnReturn;
  int intStat;

  // Attempt to get the file attributes
  intStat = stat(strFilename.c_str(),&stFileInfo);
  if(intStat == 0) {
    // We were able to get the file attributes
    // so the file obviously exists.
    blnReturn = true;
  } else {
    // We were not able to get the file attributes.
    // This may mean that we don't have permission to
    // access the folder which contains this file. If you
    // need to do that level of checking, lookup the
    // return values of stat which will give you
    // more details on why stat failed.
    blnReturn = false;
  }

  return(blnReturn);
}

/**
 * Functor for comparing objects via name
 * Assumes implementing class has function GetName() which returns name of object
 */
template<typename T>
struct NameComparer {
    NameComparer(const std::string& targetName): m_targetName(targetName)
    {
    }

    bool operator()(const T& item) const
    {
        return (m_targetName.compare(item.GetName()) == 0);
    }

    std::string m_targetName;
};

template<typename T, typename Iter>
const T* FindByName(Iter begin, Iter end, const std::string& name)
{
    Iter findIt = std::find_if(begin, end, NameComparer<T>(name));
    return (findIt != end) ? &*findIt : nullptr;
}

/**
 * Pointer less comparison
 */
template<typename T>
class PtrLessComparer {
public:

    bool operator()(const T* lhs, const T* rhs) const
    {
        return (*lhs < * rhs);
    }
};

/**
 * Pointer greater comparison
 */
template<typename T>
class PtrGreaterComparer {
public:

    bool operator()(const T* lhs, const T* rhs) const
    {
        return (*lhs > * rhs);
    }
};

/**
 * Tokenize a line by delim char and push to container using a back inserter
 */
template<typename BackInserter>
void Tokenize(const std::string& line, const std::string& delims, BackInserter back_it) {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(delims.c_str(), "", boost::keep_empty_tokens);
    tokenizer tokens(line, sep);

    for (tokenizer::iterator tok_it = tokens.begin(); tok_it != tokens.end(); ++tok_it) {
        *back_it = *tok_it;
    }
}

/**
 * Split a string, up to a maximum number of elements
 *
 * If there are more tokens than expected, remainder of string is allocated to the last element
 *
 * Precondition: outVector initialized and empty
 */
inline void split_n(std::vector<std::string>* outVector, const std::string& text, const std::string& delim, int count)
{
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

    boost::char_separator<char> sep(delim.c_str(),"", boost::keep_empty_tokens);
    tokenizer tokens(text, sep);
    tokenizer::iterator tok_iter = tokens.begin();
    int i = 1;
    std::size_t length = 0;
    while(tok_iter != tokens.end() && i < count) {
        length += tok_iter->size()+1;
        outVector->push_back(*tok_iter++);
        ++i;
    }
    if(tok_iter != tokens.end()) {
        if(text.size()<=length)	{
            outVector->push_back("");
        } else {
            outVector->push_back(text.substr(length));
        }
    }
}

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#pragma warning (pop)

#endif
