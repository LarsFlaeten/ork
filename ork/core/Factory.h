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

#ifndef _ORK_FACTORY_H_
#define _ORK_FACTORY_H_

#include <map>

#include "ork/core/Object.h"

using namespace std;

namespace ork
{

/**
 * A factory to create and destroy shared objects. Each object is constructed
 * from a key, and is destructed when it is no longer used. Clients of this
 * factory must call #get to get the object corresponding to a given key, and
 * must call #put when this object is no longer used. The object will be
 * destroyed automatically when all clients have called #put (if this object is
 * managed via smart pointers).
 * @ingroup core
 */
template <typename K, typename C>
class Factory : public Object
{
public:
    /**
     * Function pointer to the factory objects constructor.
     */
    typedef C (*constructor)(K key);

    /**
     * Creates a factory that creates objects from keys with the given constructor function.
     *
     * @param ctor a constructor function.
     */
    Factory(constructor ctor);

    /**
     * Returns the object corresponding to the given key. If this object does
     * not exist yet it is created with the constructor function of this
     * factory. Otherwise a reference counter is incremented to know when this
     * object is no longer used.
     */
    C get(K key);

    /**
     * Releases the object corresponding to the given key. If the reference
     * count of this object becomes 0, the object is automatically destroyed.
     */
    void put(K key);

private:
    /**
     * The constructor used to create the objects from keys.
     */
    constructor ctor;

    /**
     * The objects created by this factory, associated with their corresponding
     * keys. Each object has a reference counter used to automatically destroy
     * objects when they are no longer used.
     */
    map< K, pair<C, int> > values;
};

template <typename K, typename C>
Factory<K, C>::Factory(constructor ctor) : Object("Factory"), ctor(ctor)
{
}

template <typename K, typename C>
C Factory<K, C>::get(K key)
{
    C result;
    int uses;
    typename  map< K, pair<C, int> >::iterator i = values.find(key);
    if (i == values.end()) {
        // if the objet does not exist yet, we create it
        result = ctor(key);
        uses = 1;
    } else {
        // otherwise we increment its reference counter
        result = i->second.first;
        uses = i->second.second + 1;
    }
    values.insert(make_pair(key, make_pair(result, uses)));
    return result;
}

template <typename K, typename C>
void Factory<K, C>::put(K key)
{
    typename map< K, pair<C, int> >::iterator i = values.find(key);
    if (i != values.end()) {
        C value = i->second.first;
        int uses = i->second.second - 1;
        if (uses == 0) {
            // if the reference counter becomes 0 we remove the object from the
            // map. It will be destroyed automatically if it is managed via
            // smart pointers
            values.erase(i);
        } else {
            values.insert(make_pair(key, make_pair(value, uses)));
        }
    }
}

}

#endif
