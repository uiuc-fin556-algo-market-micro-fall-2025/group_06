/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Utilities/GenericMap.h
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

#ifndef _STRATEGY_STUDIO_UTILITIES_GENERIC_MAP_H_
#define _STRATEGY_STUDIO_UTILITIES_GENERIC_MAP_H_

#include "StrategyStudioException.h"
#include <map>
#include <cassert>

namespace RCM {
namespace StrategyStudio {
namespace Utilities {

template<typename K, typename V>
class GenericMap {
public:
    typedef std::map<K, V> Container;
    typedef typename Container::iterator ContainerIter;
    typedef typename Container::const_iterator ContainerConstIter;

public:
    GenericMap();
    virtual ~GenericMap();

    ContainerIter begin() { return m_container.begin(); }
    ContainerIter end() { return m_container.end(); }

    ContainerConstIter begin() const { return m_container.begin(); }
    ContainerConstIter end() const { return m_container.end(); }

    /**
     * Adds a piece of data to the container
     * @returns true if added, false if not added
     */
    bool Add(const K& key, const V& val);

    /**
     * Clear the container
     */
    void Clear();

    /**
     * Check if container contains the given key (via == operator)
     * @returns true if container contains an equivalent key, false otherwise
     */
    bool ContainsKey(const K& key) const;

    /**
     * Return the count in the container
     * @returns size of the container
     */
    size_t Count() const;

    /**
     * Performs a find using an equivalent object
     * @returns Iterator to object or to end
     */
    ContainerIter Find(const K& key);
    ContainerConstIter Find(const K& key) const;

    /**
     * Gets the given value via key
     * If not present, a new key is created with a default value
     * @returns Reference to object
     */
    V& Get(const K& key);
    const V& Get(const K& key) const;

    /**
     * Check if the container is empty
     * @returns true if empty, false otherwise
     */
    bool IsEmpty() const;

    /**
     * Removes an item from the container
     * @returns true
     */
    bool Remove(int index);
    bool Remove(const K& key);
    bool Remove(ContainerConstIter it);

    /**
     * Update a piece of data within the container
     * @returns true if updated, false if not added
     */
    bool Update(const K& key, const V& val);

protected:
    Container m_container;
};

template<typename K, typename V>
GenericMap<K,V>::GenericMap():
    m_container()
{
}

template<typename K, typename V>
GenericMap<K,V>::~GenericMap()
{
}

template<typename K, typename V>
bool GenericMap<K,V>::Add(const K& key, const V& val)
{
    if ((m_container.insert(std::make_pair(key, val))).second)
        return true;
    else
        return false;
}

template<typename K, typename V>
void GenericMap<K,V>::Clear()
{
    m_container.clear();
}

template<typename K, typename V>
bool GenericMap<K,V>::ContainsKey(const K& key) const
{
    return (m_container.count(key) > 0);
}

template<typename K, typename V>
size_t GenericMap<K,V>::Count() const
{
    return m_container.size();
}

template<typename K, typename V>
typename GenericMap<K,V>::ContainerIter GenericMap<K,V>::Find(const K& key)
{
    return m_container.find(key);
}

template<typename K, typename V>
typename GenericMap<K,V>::ContainerConstIter GenericMap<K,V>::Find(const K& key) const
{
    return m_container.find(key);
}

template<typename K, typename V>
V& GenericMap<K,V>::Get(const K& key)
{
    return m_container[key];
}

template<typename K, typename V>
const V& GenericMap<K,V>::Get(const K& key) const
{
    return m_container[key];
}

template<typename K, typename V>
bool GenericMap<K,V>::IsEmpty() const
{
    return m_container.empty();
}

template<typename K, typename V>
bool GenericMap<K,V>::Remove(const K& key)
{
    m_container.erase(key);
    return true;
}

template<typename K, typename V>
bool GenericMap<K,V>::Remove(typename GenericMap<K,V>::ContainerConstIter it)
{
    m_container.erase(it);
    return true;
}

} // namespace Utilities
} // namespace StrategyStudio
} // namespace RCM

#endif
