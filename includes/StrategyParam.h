/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/StrategyParam.h
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

#ifndef _STRATEGY_STUDIO_LIB_STRATEGY_PARAM_H_
#define _STRATEGY_STUDIO_LIB_STRATEGY_PARAM_H_

#include "DataTypes.h"
#include <Utilities/StrategyStudioException.h>
#include <Utilities/IParam.h>
#include <boost/any.hpp>
#include <cassert>

namespace RCM {
namespace StrategyStudio {

class StrategyParam;

enum StrategyParamType {
    STRATEGY_PARAM_TYPE_UNKNOWN = 0,
    STRATEGY_PARAM_TYPE_STARTUP = 1,
    STRATEGY_PARAM_TYPE_RUNTIME = 2,
    STRATEGY_PARAM_TYPE_READONLY = 3,
    STRATEGY_PARAM_TYPE_SYSTEM = 4
};

enum StrategyParamRetVal {
    STRATEGY_PARAM_RETVAL_UNKNOWN = 0,
    STRATEGY_PARAM_RETVAL_SUCCESS = 1,
    STRATEGY_PARAM_RETVAL_INCORRECT_TYPE = 2,
    STRATEGY_PARAM_RETVAL_MIN_CONSTRAINT_VIOLATED = 3,
    STRATEGY_PARAM_RETVAL_MAX_CONSTRAINT_VIOLATED = 4,
    STRATEGY_PARAM_RETVAL_MAX_PERCENT_CHANGE_CONSTRAINT_VIOLATED = 5,
    STRATEGY_PARAM_RETVAL_ON_PARAM_VALIDATE_FAILURE = 6
};

inline std::string StrategyParamRetValToString(StrategyParamRetVal value)
{
    std::string strValue;

    switch (value) {
        case STRATEGY_PARAM_RETVAL_SUCCESS: strValue = "SUCCESS"; break;
        case STRATEGY_PARAM_RETVAL_INCORRECT_TYPE: strValue = "INCORRECT TYPE"; break;
        case STRATEGY_PARAM_RETVAL_MIN_CONSTRAINT_VIOLATED: strValue = "MIN CONSTRAINT VIOLATED"; break;
        case STRATEGY_PARAM_RETVAL_MAX_CONSTRAINT_VIOLATED: strValue = "MAX CONSTRAINT VIOLATED"; break;
        case STRATEGY_PARAM_RETVAL_MAX_PERCENT_CHANGE_CONSTRAINT_VIOLATED: strValue = "MAX PERCENT CHANGE CONSTRAINT VIOLATED"; break;
        case STRATEGY_PARAM_RETVAL_ON_PARAM_VALIDATE_FAILURE: strValue = "ON_PARAM_VALIDATE_FAILURE"; break;
        default:
        case STRATEGY_PARAM_RETVAL_UNKNOWN: strValue = "UNKNOWN"; break;
    }

    return strValue;
}

/**
 * Interface that can be implemented to receive notification of actions to params
 */
class IStrategyParamListener {
public:
    IStrategyParamListener();
    virtual ~IStrategyParamListener();

public:
    /**
     * Triggered when the given param is changed
     * Param is passed as arg to event
     * RetVal is
     */
    virtual void OnParamChange(StrategyParam& param, StrategyParamRetVal retVal)=0;

    /**
     * Allow for user/strategy accept/reject of parameter change
     */
    virtual bool OnParamValidate(StrategyParam& param) = 0;

    /**
     * Called when a parameter is successfully changed
     */
    virtual void SendParamChangeSuccess(StrategyParam& param) = 0;

    /**
     * Called when a parameter change fails
     */
    virtual void SendParamChangeFailure(StrategyParam& param, const std::string &reason, const std::string &attemptedValue) = 0;

    /**
     * Log a message regarding a parameter
     */
    virtual void LogParamMsg(const std::string& msg) const = 0;

    virtual StrategyState state() const = 0;
};

template<typename U>
class StrategyParamEqualsImpl;

template<typename U>
class StrategyParamSetValueImpl;

template<typename U, typename T>
class StrategyParamEqualsImplWrapper;

template<typename U, typename T>
class StrategyParamSetValueImplWrapper;

class StrategyParam : public Utilities::IParam {
public:
    /**
     * Name of the parameter
     */
    std::string param_name() const { return param_name_; }

    /**
     * The parameter type, which describes whether and when the parameter is editable via the Strategy Manager
     */
    StrategyParamType param_type() const { return param_type_; }

    /**
     * The value/data type of the parameter
     */
    ValueType value_type() const { return value_type_; }

    /**
     * The minimum bound for the parameter value; will apply if set for params of INT or DOUBLE value types
     */
    double min_constraint() const { return min_; }

    /**
     * The maximum bound for the parameter value; will apply if set for params of INT or DOUBLE value types
     */
    double max_constraint() const { return max_; }

    /**
     * The maximum allowed absolute percent change for each update to the parameter; will apply if set for params of INT or DOUBLE value types
     *
     * This is be specified in decimal form
     */
    double max_allowed_percent_change() const { return max_percent_change_; }

    /**
     * Compares the passed arg to the contained datum
     * @returns true if types equal, or false if types unequal or no equality
     */
    bool Equals(const boost::any& datum) const;

    /**
     * Compares the passed arg to the contained datum
     * @returns true if types equal, or false if types unequal or no equality
     */
    template<typename T>
    bool Equals(const T& datum) const;

    /**
     * Populate data from string, calls IStrategyParamListener interface to validate and notify of changes
     */
    StrategyParamRetVal FromString(const std::string &buf);

    /**
     * Get datum as int
     * @returns true if out arg was set, otherwise false otherwise (occurs if type mismatch)
     */
    bool Get(int* outArg) const;

    /**
     * Get datum as double
     * @returns true if out arg was set, otherwise false otherwise (occurs if type mismatch)
     */
    bool Get(double *outArg) const;

    /**
     * Get datum as char
     * @returns true if out arg was set, otherwise false otherwise (occurs if type mismatch)
     */
    bool Get(char* outArg) const;

    /**
     * Get datum as string
     * @returns true if out arg was set, otherwise false otherwise (occurs if type mismatch)
     */
    bool Get(std::string* outArg) const;

    /**
     * Get datum as bool
     * @returns true if out arg was set, otherwise false otherwise (occurs if type mismatch)
     */
    bool Get(bool *outArg) const;

    /**
     * Get the datum and write to outArg
     * @returns true if out arg was set, otherwise false otherwise (occurs if type mismatch)
     */
    template<typename T>
    bool Get(T* outArg) const;

    /**
     * Set the datum for this param
     * @returns Results of operation if succeeded or failed
     */
    template<typename T>
    StrategyParamRetVal SetValue(const T& datum);

    /**
     * Set the datum for this param from a boost any
     */
    StrategyParamRetVal SetValue(const boost::any& datum);

    /**
     * Get the datum value as string
     * @returns data as string
     */
    std::string ToString() const;

    const boost::any& datum() const { return datum_; }

    bool operator==(const StrategyParam& rhs) const;

private:
    StrategyParam(const std::string& paramName,
                  StrategyParamType paramType,
                  ValueType valueType,
                  double min,
                  double max,
                  double maxPercentChange,
                  IStrategyParamListener* listener = nullptr);

    StrategyParamRetVal validate(double);

    template<typename T>
    bool Set(const T& datum);

    void set_listener(IStrategyParamListener* listener) { listener_ = listener; }

private:
    std::string param_name_;
    StrategyParamType param_type_;
    ValueType value_type_;
    boost::any datum_;
    double min_;
    double max_;
    double max_percent_change_;
    IStrategyParamListener* listener_;
    friend class StrategyParamCollection;
};

template<typename T>
inline bool StrategyParam::Equals(const T& datum) const
{
    switch (value_type_) {
        case VALUE_TYPE_INT: {
            return StrategyParamEqualsImplWrapper<int, T>()(datum, datum_);
        }
        case VALUE_TYPE_DOUBLE: {
            return StrategyParamEqualsImplWrapper<double, T>()(datum, datum_);
        }
        case VALUE_TYPE_CHAR: {
            return StrategyParamEqualsImplWrapper<char, T>()(datum, datum_);
        }
        case VALUE_TYPE_STRING: {
            return StrategyParamEqualsImplWrapper<std::string, T>()(datum, datum_);
        }
        case VALUE_TYPE_BOOL: {
            return StrategyParamEqualsImplWrapper<bool, T>()(datum, datum_);
        }
        case VALUE_TYPE_UNKNOWN: // fall-through
        default:
           return false;
    }
}

inline bool StrategyParam::Get(int* outArg) const
{
    if (value_type_ != VALUE_TYPE_INT) return false;

    assert(datum_.type() == typeid(int));

    *outArg = boost::any_cast<int>(datum_);
    return true;
}

inline bool StrategyParam::Get(double *outArg) const
{
    if (value_type_ != VALUE_TYPE_DOUBLE) return false;

    assert(datum_.type() == typeid(double));

    *outArg = boost::any_cast<double>(datum_);
    return true;
}

inline bool StrategyParam::Get(char* outArg) const
{
    if (value_type_ != VALUE_TYPE_CHAR) return false;

    assert(datum_.type() == typeid(char));

    *outArg = boost::any_cast<char>(datum_);
    return true;
}

inline bool StrategyParam::Get(std::string* outArg) const
{
    if (value_type_ != VALUE_TYPE_STRING) return false;

    assert(datum_.type() == typeid(std::string));

    *outArg = boost::any_cast<std::string>(datum_);
    return true;
}

inline bool StrategyParam::Get(bool *outArg) const
{
    if (value_type_ != VALUE_TYPE_BOOL) return false;

    assert(datum_.type() == typeid(bool));

    *outArg = boost::any_cast<bool>(datum_);
    return true;
}

template<typename T>
bool StrategyParam::Get(T* outArg) const
{
    switch (value_type_) {
        case VALUE_TYPE_INT:
            if (typeid(int) == typeid(T)) {
                *outArg = boost::any_cast<int>(datum_);
                return true;
            } else {
                break;
            }
        case VALUE_TYPE_DOUBLE:
            if (typeid(double) == typeid(T)) {
                *outArg = boost::any_cast<double>(datum_);
                return true;
            } else {
                break;
            }
        case VALUE_TYPE_CHAR:
            if (typeid(char) == typeid(T)) {
                *outArg = boost::any_cast<char>(datum_);
                return true;
            } else {
                break;
            }
        case VALUE_TYPE_STRING:
            if (typeid(std::string) == typeid(T)) {
                *outArg = boost::any_cast<std::string>(datum_);
                return true;
            } else {
                break;
            }
        case VALUE_TYPE_BOOL:
            if (typeid(bool) == typeid(T)) {
                *outArg = boost::any_cast<bool>(datum_);
                return true;
            } else {
                break;
            }
        case VALUE_TYPE_UNKNOWN: // fall-through
        default:
           return false;
    }
}

template<typename T>
inline StrategyParamRetVal StrategyParam::SetValue(const T& datum)
{
    StrategyParamRetVal result = STRATEGY_PARAM_RETVAL_SUCCESS;
    StrategyParam newValue(*this);

    // This set does not call listener, so it's safe and won't be recursive
    bool setable = newValue.Set(datum);
    if (!setable) {
        result = STRATEGY_PARAM_RETVAL_INCORRECT_TYPE;
    } else {
        switch (value_type_) {
            case VALUE_TYPE_INT: {
                result = validate(Utilities::Cast<double>()(datum));

                if (result == STRATEGY_PARAM_RETVAL_SUCCESS) {
                    if (listener_)
                        result = listener_->OnParamValidate(newValue) ? STRATEGY_PARAM_RETVAL_SUCCESS : STRATEGY_PARAM_RETVAL_ON_PARAM_VALIDATE_FAILURE;

                    if (result == STRATEGY_PARAM_RETVAL_SUCCESS) {
                        if (!StrategyParamSetValueImplWrapper<int, T>()(datum, datum_))
                            result = STRATEGY_PARAM_RETVAL_INCORRECT_TYPE;
                    }
                }
                break;
            }
            case VALUE_TYPE_DOUBLE: {
                result = validate(Utilities::Cast<double>()(datum));

                if (result == STRATEGY_PARAM_RETVAL_SUCCESS) {
                    if (listener_)
                        result = listener_->OnParamValidate(newValue) ? STRATEGY_PARAM_RETVAL_SUCCESS : STRATEGY_PARAM_RETVAL_ON_PARAM_VALIDATE_FAILURE;

                    if (result == STRATEGY_PARAM_RETVAL_SUCCESS) {
                        if (!StrategyParamSetValueImplWrapper<double, T>()(datum, datum_))
                            result = STRATEGY_PARAM_RETVAL_INCORRECT_TYPE;
                    }
                }
                break;
            }
            case VALUE_TYPE_CHAR: {
                if (listener_)
                    result = listener_->OnParamValidate(newValue) == true ? STRATEGY_PARAM_RETVAL_SUCCESS : STRATEGY_PARAM_RETVAL_ON_PARAM_VALIDATE_FAILURE;

                if (result == STRATEGY_PARAM_RETVAL_SUCCESS) {
                    if (!StrategyParamSetValueImplWrapper<char, T>()(datum, datum_))
                        result = STRATEGY_PARAM_RETVAL_INCORRECT_TYPE;
                }
                break;
            }
            case VALUE_TYPE_STRING: {
                if (listener_)
                    result = listener_->OnParamValidate(newValue) == true ? STRATEGY_PARAM_RETVAL_SUCCESS : STRATEGY_PARAM_RETVAL_ON_PARAM_VALIDATE_FAILURE;

                if (result == STRATEGY_PARAM_RETVAL_SUCCESS) {
                    if (!StrategyParamSetValueImplWrapper<std::string, T>()(datum, datum_))
                        result = STRATEGY_PARAM_RETVAL_INCORRECT_TYPE;
                }

                break;
            }
            case VALUE_TYPE_BOOL: {
                if (listener_)
                    result = listener_->OnParamValidate(newValue) == true ? STRATEGY_PARAM_RETVAL_SUCCESS : STRATEGY_PARAM_RETVAL_ON_PARAM_VALIDATE_FAILURE;

                if (result == STRATEGY_PARAM_RETVAL_SUCCESS) {
                    if (!StrategyParamSetValueImplWrapper<bool, T>()(datum, datum_))
                        result = STRATEGY_PARAM_RETVAL_INCORRECT_TYPE;
                }
                break;
            }
            case VALUE_TYPE_UNKNOWN: // fall-through
            default:
                result =  STRATEGY_PARAM_RETVAL_INCORRECT_TYPE;
        }
    }

    if ((result == STRATEGY_PARAM_RETVAL_SUCCESS) && listener_) {
        listener_->OnParamChange(*this, STRATEGY_PARAM_RETVAL_SUCCESS);
        listener_->SendParamChangeSuccess(*this);
    } else if (listener_) {
        std::string attemptedValue = Utilities::Cast<std::string>()(datum);
        listener_->SendParamChangeFailure(*this, StrategyParamRetValToString(result), attemptedValue);
    }

    return result;
}

template<typename T>
inline bool StrategyParam::Set(const T& datum)
{
    bool result = true;

    switch (value_type_) {
        case VALUE_TYPE_INT: {
            if (!StrategyParamSetValueImplWrapper<int, T>()(datum, datum_))
                result = false;
            break;
        }
        case VALUE_TYPE_DOUBLE: {
            if (!StrategyParamSetValueImplWrapper<double, T>()(datum, datum_))
                result = false;
            break;
        }
        case VALUE_TYPE_CHAR: {
            if (!StrategyParamSetValueImplWrapper<char, T>()(datum, datum_))
                result = false;
            break;
        }
        case VALUE_TYPE_STRING: {
            if (!StrategyParamSetValueImplWrapper<std::string, T>()(datum, datum_))
                result = false;
            break;
        }
        case VALUE_TYPE_BOOL: {
            if (!StrategyParamSetValueImplWrapper<bool, T>()(datum, datum_))
                result = false;
            break;
        }
        case VALUE_TYPE_UNKNOWN: // fall-through
        default:
            result =  false;
    }

    return result;
}

/*-----------------------------------------------------------------------------------------*/

template<typename U>
class StrategyParamEqualsImpl {
public:
    template<typename T>
    bool operator()(const T& lhs, const boost::any& datum)
    {
        U val = lhs;
        return (val == boost::any_cast<T>(datum));
    }
};

template<>
class StrategyParamEqualsImpl<int> {
public:
    bool operator()(int lhs, const boost::any& datum)
    {
        return (lhs == boost::any_cast<int>(datum));
    }

    bool operator()(double , const boost::any& )
    {
        return false;
    }

    bool operator()(char lhs, const boost::any& datum)
    {
        int val = lhs;
        return (val == boost::any_cast<int>(datum));
    }

    bool operator()(const std::string& , const boost::any& )
    {
        return false;
    }

    bool operator()(bool , const boost::any& )
    {
        return false;
    }

    template<typename T>
    bool operator()(const T& , const boost::any& )
    {
        return false;
    }
};

template<>
class StrategyParamEqualsImpl<double> {
public:
    bool operator()(int lhs, const boost::any& datum)
    {
        double val = lhs;
        return (val == boost::any_cast<double>(datum));
    }

    bool operator()(double lhs, const boost::any& datum)
    {
        return (lhs == boost::any_cast<double>(datum));
    }

    bool operator()(char , const boost::any& )
    {
        return false;
    }

    bool operator()(const std::string& , const boost::any& )
    {
        return false;
    }

    bool operator()(bool , const boost::any& )
    {
        return false;
    }

    template<typename T>
    bool operator()(const T& , const boost::any& )
    {
       return false;
    }
};

template<>
class StrategyParamEqualsImpl<char> {
public:
   bool operator()(int , const boost::any& )
    {
        return false;
    }

    bool operator()(double , const boost::any& )
    {
        return false;
    }

    bool operator()(char lhs, const boost::any& datum)
    {
        return (lhs == boost::any_cast<char>(datum));
    }

    bool operator()(const std::string& , const boost::any& )
    {
        return false;
    }

    bool operator()(bool , const boost::any& )
    {
        return false;
    }

    template<typename T>
    bool operator()(const T& , const boost::any& )
    {
        return false;
    }
};

template<>
class StrategyParamEqualsImpl<std::string> {
public:
    bool operator()(int , const boost::any& )
    {
        return false;
    }

    bool operator()(double , const boost::any& )
    {
        return false;
    }

    bool operator()(char , const boost::any& )
    {
        return false;
    }

    bool operator()(const std::string& lhs, const boost::any& datum)
    {
        return (lhs == boost::any_cast<std::string>(datum));
    }

    bool operator()(bool , const boost::any& )
    {
        return false;
    }

    template<typename T>
    bool operator()(const T& , const boost::any& )
    {
        return false;
    }
};

template<>
class StrategyParamEqualsImpl<bool> {
public:
   bool operator()(int , const boost::any& )
    {
        return false;
    }

    bool operator()(double , const boost::any& )
    {
        return false;
    }

    bool operator()(char , const boost::any& )
    {
        return false;
    }

    bool operator()(const std::string& , const boost::any& )
    {
        return false;
    }

    bool operator()(bool lhs, const boost::any& datum)
    {
        return (lhs == boost::any_cast<bool>(datum));
    }

    template<typename T>
    bool operator()(const T& , const boost::any& )
    {
        return false;
    }
};

/*-----------------------------------------------------------------------------------------*/

template<typename U>
class StrategyParamSetValueImpl {
public:
    template<typename T>
    void operator()(T lhs, boost::any& datum)
    {
        U val = lhs;
        datum = val;
    }
};

template<>
class StrategyParamSetValueImpl<int> {
public:
    bool operator()(int lhs, boost::any& datum) const
    {
        datum = lhs;
        return true;
    }

    bool operator()(double , boost::any& ) const
    {
        return false;
    }

    bool operator()(char lhs, boost::any& datum) const
    {
        datum = lhs;
        return true;
    }

    bool operator()(const std::string& , boost::any& ) const
    {
        return false;
    }

    bool operator()(bool lhs, boost::any& datum) const
    {
        int val = lhs;
        datum = val;
        return true;
    }

    template<typename T>
    bool operator()(const T& lhs, boost::any& datum) const
    {
        int val = lhs;
        datum = val;
        return true;
    }
};

template<>
class StrategyParamSetValueImpl<double> {
public:
    bool operator()(int lhs, boost::any& datum) const
    {
        double val = lhs;
        datum = val;
        return true;
    }

    bool operator()(double lhs, boost::any& datum) const
    {
        datum = lhs;
        return true;
    }

    bool operator()(char lhs, boost::any& datum) const
    {
        double val = lhs;
        datum = val;
        return true;
    }

    bool operator()(const std::string& , boost::any& ) const
    {
        return false;
    }

    bool operator()(bool , boost::any& ) const
    {
        return false;
    }

    template<typename T>
    bool operator()(const T& lhs, boost::any& datum) const
    {
        double val = lhs;
        datum = val;
        return true;
    }
};

template<>
class StrategyParamSetValueImpl<char> {
public:
    bool operator()(int , boost::any& ) const
    {
        return false;
    }

    bool operator()(double , boost::any& ) const
    {
        return false;
    }

    bool operator()(char lhs, boost::any& datum) const
    {
        datum = lhs;
        return true;
    }

    bool operator()(const std::string& , boost::any& ) const
    {
        return false;
    }

    bool operator()(bool , boost::any& ) const
    {
        return false;
    }

    template<typename T>
    bool operator()(const T& lhs, boost::any& datum) const
    {
        char val = lhs;
        datum = val;
        return true;
    }
};

template<>
class StrategyParamSetValueImpl<std::string> {
public:
    bool operator()(int lhs, boost::any& datum) const
    {
        datum = Utilities::Cast<std::string>()(lhs);
        return true;
    }

    bool operator()(double lhs, boost::any& datum) const
    {
        datum = Utilities::Cast<std::string>()(lhs);
        return true;
    }

    bool operator()(char lhs, boost::any& datum) const
    {
        datum = Utilities::Cast<char>()(lhs);
        return true;
    }

    bool operator()(const std::string& lhs, boost::any& datum) const
    {
        datum = lhs;
        return true;
    }

    bool operator()(bool lhs, boost::any& datum) const
    {
        datum = Utilities::Cast<std::string>()(lhs);
        return true;
    }

    template<typename T>
    bool operator()(const T& lhs, boost::any& datum) const
    {
        datum = Utilities::Cast<std::string>()(lhs);
        return true;
    }
};

template<>
class StrategyParamSetValueImpl<bool> {
public:
    bool operator()(int lhs, boost::any& datum) const
    {
        bool val = (lhs != 0);
        datum = val;
        return true;
    }

    bool operator()(double lhs, boost::any& datum) const
    {
        bool val = (lhs != 0);
        datum = val;
        return false;
    }

    bool operator()(char , boost::any& ) const
    {
        return false;
    }

    bool operator()(const std::string& lhs, boost::any& datum) const
    {
        bool val = Utilities::Cast<bool>()(lhs);
        datum = val;
        return val;
    }

    bool operator()(bool lhs, boost::any& datum) const
    {
        datum = lhs;
        return true;
    }

    template<typename T>
    bool operator()(const T& lhs, boost::any& datum) const
    {
        bool val = lhs;
        datum = val;
        return true;
    }
};

/*-----------------------------------------------------------------------------------------*/

template<typename U, typename T>
class StrategyParamEqualsImplWrapper {
public:
    template<typename V>
    bool operator()(const V& lhs, const boost::any& datum)
    {
        return StrategyParamEqualsImpl<U>()(lhs, datum);
    }
};

template<typename U, typename T>
class StrategyParamSetValueImplWrapper {
public:
    template<typename V>
    bool operator()(const V& lhs, boost::any& datum)
    {
        return StrategyParamSetValueImpl<U>()(lhs, datum);
    }
};

} // namespace StrategyStudio
} // namespace RCM

#endif
