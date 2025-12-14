/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/GenericRecord.h
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

#ifndef _STRATEGY_STUDIO_UTILITIES_GENERIC_RECORD_H_
#define _STRATEGY_STUDIO_UTILITIES_GENERIC_RECORD_H_

#include "StrategyStudioException.h"
#include <vector>
#include <algorithm>
#include <cassert>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

template<typename T>
class GenericRecord {
public:
    typedef std::vector<T> Container;
    typedef typename Container::iterator ContainerIter;
    typedef typename Container::const_iterator ContainerConstIter;
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

public:
    GenericRecord();
    explicit GenericRecord(int initialSize);
    virtual ~GenericRecord();

    ContainerIter begin() { return m_container.begin(); }
    ContainerIter end() { return m_container.end(); }

    ContainerConstIter begin() const { return m_container.begin(); }
    ContainerConstIter end() const { return m_container.end(); }

    /**
     * Adds a piece of data to the container
     * @returns true if added, false if not added
     */
    bool Add(const T& record);

    /**
     * Clear the container
     */
    void Clear();

    /**
     * Check if container contains the given key (via == operator)
     * @returns true if container contains an equivalent item, false otherwise
     */
    bool Contains(const T& item) const;

    /**
     * Return the count in the container
     * @returns size of the container
     */
    size_t Count() const;

    /**
     * Performs a find using an equivalent object
     * @returns Iterator to object or to end
     */
    ContainerIter Find(const T& item);
    ContainerConstIter Find(const T& item) const;

    /**
     * Check if the container is empty
     * @returns true if empty, false otherwise
     */
    bool IsEmpty() const;

protected:
    Container m_container;
};

template<typename T>
GenericRecord<T>::GenericRecord():
    m_container()
{
}

template<typename T>
GenericRecord<T>::~GenericRecord()
{
}

template<typename T>
bool GenericRecord<T>::Add(const T& item)
{
    m_container.push_back(item);
    return true;
}

template<typename T>
void GenericRecord<T>::Clear()
{
    m_container.clear();
}

template<typename T>
bool GenericRecord<T>::Contains(const T& item) const
{
    for (ContainerConstIter it = m_container.begin(); it != m_container.end(); ++it) {
        if (*it == item) return true;
    }

    return false;
}

template<typename T>
size_t GenericRecord<T>::Count() const
{
    return m_container.size();
}

template<typename T>
typename GenericRecord<T>::ContainerIter GenericRecord<T>::Find(const T& item)
{
    return find(m_container.begin(), m_container.end(), item);
}

template<typename T>
typename GenericRecord<T>::ContainerConstIter GenericRecord<T>::Find(const T& item) const
{
    return find(m_container.begin(), m_container.end().item);
}

template<typename T>
bool GenericRecord<T>::IsEmpty() const
{
    return m_container.empty();
}

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif
