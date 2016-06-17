/*
 * Ork: a small object-oriented OpenGL Rendering Kernel.
 * Website : http://ork.gforge.inria.fr/
 * Copyright (c) 2008-2015 INRIA - LJK (CNRS - Grenoble University)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors 
 * may be used to endorse or promote products derived from this software without 
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * Ork is distributed under the BSD3 Licence. 
 * For any assistance, feedback and remarks, you can check out the 
 * mailing list on the project page : 
 * http://ork.gforge.inria.fr/
 */
/*
 * Main authors: Eric Bruneton, Antoine Begault, Guillaume Piolat.
 */

#ifndef _ORK_ITERATOR_H_
#define _ORK_ITERATOR_H_

#include <set>
#include <map>

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
    SetIterator(std::set<type> &c);

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
    typename std::set<type>::iterator i;

    /**
     * The iterator position corresponding to the end of the set.
     */
    typename std::set<type>::iterator end;

    /**
     * The empty set used for creating empty iterators.
     */
    static std::set<type> emptySet;
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
    MapIterator(std::map<key, type> &c);

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
    typename std::map<key,type>::iterator i;

    /**
     * The iterator position corresponding to the end of the map.
     */
    typename std::map<key,type>::iterator end;

    /**
     * The empty map used for creating empty iterators.
     */
    static std::map<key,type> emptyMap;
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
    MultiMapIterator(std::multimap<key, type> &c);

    /**
     * Creates a multimap iterator for the values associated with the given key
     * in the given multimap.
     */
    MultiMapIterator(key k, std::multimap<key, type> &c);

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
    typename std::multimap<key,type>::iterator i;

    /**
     * The iterator position corresponding to the end of the multimap.
     */
    typename std::multimap<key,type>::iterator end;

    /**
     * The empty multimap used for creating empty iterators.
     */
    static std::multimap<key,type> emptyMap;
};

template <typename type>
SetIterator<type>::SetIterator() : n(0), i(emptySet.begin()), end(emptySet.end())
{
}

template <typename type>
SetIterator<type>::SetIterator(std::set<type> &c) : n((unsigned int)c.size()), i(c.begin()), end(c.end())
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
std::set<type> SetIterator<type>::emptySet;

template <typename key, typename type>
MapIterator<key, type>::MapIterator() : n(0), i(emptyMap.begin()), end(emptyMap.end())
{
}

template <typename key, typename type>
MapIterator<key, type>::MapIterator(std::map<key, type> &c) : n((unsigned int)c.size()), i(c.begin()), end(c.end())
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
std::map<key,type> MapIterator<key, type>::emptyMap;

template <typename key, typename type>
MultiMapIterator<key, type>::MultiMapIterator() : n(0), i(emptyMap.begin()), end(emptyMap.end())
{
}

template <typename key, typename type>
MultiMapIterator<key, type>::MultiMapIterator(std::multimap<key, type> &c) : n(c.size()), i(c.begin()), end(c.end())
{
}

template <typename key, typename type>
MultiMapIterator<key, type>::MultiMapIterator(key k, std::multimap<key, type> &c)
{
    std::pair<typename std::multimap<key, type>::iterator, typename std::multimap<key, type>::iterator> p;
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
std::multimap<key,type> MultiMapIterator<key, type>::emptyMap;

}

#endif
