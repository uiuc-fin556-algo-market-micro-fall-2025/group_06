/*================================================================================                               
*     Source: Testable.h                                                        
*     Contents:                                     
*     Distribution:          
*                                                                                                                
*                                                                                                                
*     Copyright (c) Lime Brokerage LLC, 2009 - 2015.
*     All rights reserved.                                                                                       
*                                                                                                                
*     This software is part of Licensed material, which is the property of Lime Brokerage LLC ("Company"), 
*     and constitutes Confidential Information of the Company.                                                  
*     Unauthorized use, modification, duplication or distribution is strictly prohibited by Federal law.         
*     No title to or ownership of this software is hereby transferred.                                          
*                                                                                                                
*     The software is provided "as is", and in no event shall the Company or any of its affiliates or successors be liable for any 
*     damages, including any lost profits or other incidental or consequential damages relating to the use of this software.       
*     The Company makes no representations or warranties, express or implied, with regards to this software.                        
/*================================================================================*/   

#pragma once 

#ifndef _STRATEGY_STUDIO_LIB_TESTABLE_H_
#define _STRATEGY_STUDIO_LIB_TESTABLE_H_

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

// safe bool idiom - replace with simple operator overload with C++11
class TestableBase {
protected:
    typedef void (TestableBase::*bool_type)() const;
    void unsupported() const {} // for catching unsupported operations at compile time

    TestableBase() {}
    TestableBase(const TestableBase&) {}
    TestableBase& operator=(const TestableBase&) { return *this; }
    ~TestableBase() {}
};

template <typename T> class Testable : public TestableBase {
public:
    operator bool_type() const {
        return (static_cast<const T*>(this))->test()
            ? &Testable::unsupported : 0;
    }
protected:
    // implement in CRTP derived class:  bool test() const;
    // uncomment this and make destructor virtual if we don't want to use CRTP
    //virtual bool test() const=0;
    ~Testable() {}
};

// disable comparisons between instances
template <typename T, typename U>
bool operator==(const Testable<T>& lhs, const Testable<U>& rhs) {
    lhs.unsupported();
    return false;
}

template <typename T, typename U>
bool operator!=(const Testable<T>& lhs, const Testable<U>& rhs) {
    lhs.unsupported();
    return false;
}

// example usage
//
//class Foo : public Testable <Foo> {
//public:
//    Foo(bool isValid = false) : m_isValid(isValid) {}
//    bool test() const { return m_isValid; }
//
//private:
//    bool m_isValid;
//};
//
//Foo a;
//if (a) {
//}

}   // namespace Utilities
}   // namespace StrategyStudio
}   // namespace LimeBrokerage

#endif  // _STRATEGY_STUDIO_LIB_TESTABLE_H_
