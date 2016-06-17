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

#ifndef _ORK_FACTORY_H_
#define _ORK_FACTORY_H_

#include <map>

#include "ork/core/Object.h"

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
    std::map< K, std::pair<C, int> > values;
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
    typename  std::map< K, std::pair<C, int> >::iterator i = values.find(key);
    if (i == values.end()) {
        // if the objet does not exist yet, we create it
        result = ctor(key);
        uses = 1;
    } else {
        // otherwise we increment its reference counter
        result = i->second.first;
        uses = i->second.second + 1;
    }
    values.insert(std::make_pair(key, std::make_pair(result, uses)));
    return result;
}

template <typename K, typename C>
void Factory<K, C>::put(K key)
{
    typename std::map< K, std::pair<C, int> >::iterator i = values.find(key);
    if (i != values.end()) {
        C value = i->second.first;
        int uses = i->second.second - 1;
        if (uses == 0) {
            // if the reference counter becomes 0 we remove the object from the
            // map. It will be destroyed automatically if it is managed via
            // smart pointers
            values.erase(i);
        } else {
            values.insert(std::make_pair(key, std::make_pair(value, uses)));
        }
    }
}

}

#endif
