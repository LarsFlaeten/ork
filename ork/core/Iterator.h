/*
 * Ork: a small object-oriented OpenGL Rendering Kernel.
 * Copyright (c) 2008-2010 INRIA
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.
 */

/*
 * Authors: Eric Bruneton, Antoine Begault, Guillaume Piolat.
 */

#ifndef _ORK_ITERATOR_H_
#define _ORK_ITERATOR_H_

#include <set>
#include <map>

using namespace std;

namespace ork
{

/**
 * A set iterator.
 * @ingroup core
 */
template <typename type>
class SetIterator
{
public:
    /**
     * Creates a set iterator for an empty set.
     */
    SetIterator();

    /**
     * Creates a set iterator for the given set.
     */
    SetIterator(set<type> &c);

    /**
     * Returns the size of the set for which this iterator has been created.
     */
    unsigned int size();

    /**
     * Returns true if the iteration is not yet finished.
     */
    bool hasNext();

    /**
     * Returns the element at the current iterator position.
     * The iterator position is then incremented.
     */
    type next();

private:
    /**
     * The size of the set for which this iterator has been created.
     */
    unsigned int n;

    /**
     * The current iterator position.
     */
    typename set<type>::iterator i;

    /**
     * The iterator position corresponding to the end of the set.
     */
    typename set<type>::iterator end;

    /**
     * The empty set used for creating empty iterators.
     */
    static set<type> emptySet;
};

/**
 * A map iterator.
 * @ingroup core
 */
template <typename key, typename type>
class MapIterator
{
public:
    /**
     * Creates a map iterator for an empty map.
     */
    MapIterator();

    /**
     * Creates a map iterator for the given map.
     */
    MapIterator(map<key, type> &c);

    /**
     * Returns the size of the map for which this iterator has been created.
     */
    unsigned int size();

    /**
     * Returns true if the iteration is not yet finished.
     */
    bool hasNext();

    /**
     * Returns the element at the current iterator position.
     * The iterator position is then incremented.
     */
    type next();

    /**
     * Returns the element at the current iterator position.
     * The iterator position is then incremented.
     *
     * @param[out] k the key of this element.
     */
    type next(key &k);

private:
    /**
     * The size of the map for which this iterator has been created.
     */
    unsigned int n;

    /**
     * The current iterator position.
     */
    typename map<key,type>::iterator i;

    /**
     * The iterator position corresponding to the end of the map.
     */
    typename map<key,type>::iterator end;

    /**
     * The empty map used for creating empty iterators.
     */
    static map<key,type> emptyMap;
};

/**
 * A multimap iterator.
 * @ingroup core
 */
template <typename key, typename type>
class MultiMapIterator
{
public:
    /**
     * Creates a multimap iterator for an empty multimap.
     */
    MultiMapIterator();

    /**
     * Creates a multimap iterator for the given multimap.
     */
    MultiMapIterator(multimap<key, type> &c);

    /**
     * Creates a multimap iterator for the values associated with the given key
     * in the given multimap.
     */
    MultiMapIterator(key k, multimap<key, type> &c);

    /**
     * Returns the size of the multimap for which this iterator has been created.
     */
    unsigned int size();

    /**
     * Returns true if the iteration is not yet finished.
     */
    bool hasNext();

    /**
     * Returns the element at the current iterator position.
     * The iterator position is then incremented.
     */
    type next();

    /**
     * Returns the element at the current iterator position.
     * The iterator position is then incremented.
     */
    type next(key &k);

private:
    /**
     * The size of the multimap for which this iterator has been created.
     */
    unsigned int n;

    /**
     * The current iterator position.
     */
    typename multimap<key,type>::iterator i;

    /**
     * The iterator position corresponding to the end of the multimap.
     */
    typename multimap<key,type>::iterator end;

    /**
     * The empty multimap used for creating empty iterators.
     */
    static multimap<key,type> emptyMap;
};

template <typename type>
SetIterator<type>::SetIterator() : n(0), i(emptySet.begin()), end(emptySet.end())
{
}

template <typename type>
SetIterator<type>::SetIterator(set<type> &c) : n((unsigned int)c.size()), i(c.begin()), end(c.end())
{
}

template <typename type>
unsigned int SetIterator<type>::size()
{
    return n;
}

template <typename type>
bool SetIterator<type>::hasNext()
{
    return i != end;
}

template <typename type>
type SetIterator<type>::next()
{
    return *(i++);
}

template <typename type>
set<type> SetIterator<type>::emptySet;

template <typename key, typename type>
MapIterator<key, type>::MapIterator() : n(0), i(emptyMap.begin()), end(emptyMap.end())
{
}

template <typename key, typename type>
MapIterator<key, type>::MapIterator(map<key, type> &c) : n((unsigned int)c.size()), i(c.begin()), end(c.end())
{
}

template <typename key, typename type>
unsigned int MapIterator<key, type>::size()
{
    return n;
}

template <typename key, typename type>
bool MapIterator<key, type>::hasNext()
{
    return i != end;
}

template <typename key, typename type>
type MapIterator<key, type>::next()
{
    return (*(i++)).second;
}

template <typename key, typename type>
type MapIterator<key, type>::next(key &k)
{
    k = (*i).first;
    return (*(i++)).second;
}

template <typename key, typename type>
map<key,type> MapIterator<key, type>::emptyMap;

template <typename key, typename type>
MultiMapIterator<key, type>::MultiMapIterator() : n(0), i(emptyMap.begin()), end(emptyMap.end())
{
}

template <typename key, typename type>
MultiMapIterator<key, type>::MultiMapIterator(multimap<key, type> &c) : n(c.size()), i(c.begin()), end(c.end())
{
}

template <typename key, typename type>
MultiMapIterator<key, type>::MultiMapIterator(key k, multimap<key, type> &c)
{
    std::pair<typename multimap<key, type>::iterator, typename multimap<key, type>::iterator> p;
    p = c.equal_range(k);
    n = (unsigned int) c.count(k);
    i = p.first;
    end = p.second;
}

template <typename key, typename type>
unsigned int MultiMapIterator<key, type>::size()
{
    return n;
}

template <typename key, typename type>
bool MultiMapIterator<key, type>::hasNext()
{
    return i != end;
}

template <typename key, typename type>
type MultiMapIterator<key, type>::next()
{
    return (*(i++)).second;
}

template <typename key, typename type>
type MultiMapIterator<key, type>::next(key &k)
{
    k = (*i).first;
    return (*(i++)).second;
}

template <typename key, typename type>
multimap<key,type> MultiMapIterator<key, type>::emptyMap;

}

#endif
