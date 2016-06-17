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

#undef __STRICT_ANSI__

#include "ork/core/Object.h"

#include <sstream>
#include <iostream>

#include "ork/core/Logger.h"

void fopen(FILE **f, const char* fileName, const char *mode)
{
#ifdef _MSC_VER
    fopen_s(f, fileName, mode);
#else
    *f = fopen(fileName, mode);
#endif
}

void fseek64(FILE *f, long long offset, int origin)
{
#ifdef _MSC_VER
    _fseeki64(f, offset, origin);
#else
    fseeko64(f, offset, origin);
#endif
}

#ifndef NDEBUG
void assertAndSegfault(const char* a, const char* f, int l)
{
    if (ork::Logger::ERROR_LOGGER != NULL) {
        ostringstream msg;
        msg << "Assertion failed " << a << " (file " << f << " line " << l << ")";
        ork::Logger::ERROR_LOGGER->log("ASSERTION", msg.str());
    }
    *((int*) 0) = 0;
}
#endif

namespace ork
{

#ifndef NDEBUG
unsigned int Object::count = 0;

map<char*,int>* Object::counts = NULL;
#endif

#ifdef KEEP_OBJECT_REFERENCES
map<char*, set<Object*>* >* Object::instances = NULL;
#endif

#ifdef USED_SHARED_PTR
Object::Object(const char *type)
#else
Object::Object(const char *type) : references(0)
#endif
{
#ifndef NDEBUG
    // sets the type of this object
    this->type = (char*) type;
    // increments the global instance counter
    ++count;
    // increments the instance counter of the 'type' class
    if (counts == NULL) {
        counts = new map<char*,int>;
    }

    map<char*,int>::iterator classCount = counts->find((char *)type);

    if (classCount == counts->end()) {
        (*counts)[(char *)type] = 1;
    } else {
        (*counts)[(char *)type] += 1;
    }
#endif

#ifdef KEEP_OBJECT_REFERENCES
    // insert into the instances set
    if (instances == NULL) {
        instances = new map<char*, set<Object*>* >();
    }
    map<char*, set<Object*>* >::iterator classInstances = instances->find((char *)type);
    if (classInstances == instances->end()) {
        set<Object*>* s = new set<Object*>();
        (*instances)[(char *)type] = s;
    }
    (*instances)[(char *)type]->insert(this);
#endif

#ifndef NDEBUG
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->log("CORE", "'" + string(type) + "' object created");
    }
#endif
}

Object::~Object()
{
#ifdef KEEP_OBJECT_REFERENCES
    // remove from references
    (*instances)[(char *)type]->erase(this);
#endif

#ifndef NDEBUG
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->log("CORE", "'" + string(type) + "' object deleted");
    }
    // decrements the global instance counter
    assert(count > 0);
    --count;
    // decrements the instance counter of the 'type' class
    assert((*counts)[type] > 0);
    (*counts)[type] -= 1;
#endif
}

const char* Object::getClass() const
{
#ifndef NDEBUG
    return type;
#else
    return "";
#endif
}

const char* Object::toString()
{
    return getClass();
}

void Object::exit()
{
    static_ref *last = statics;
    while (last != 0) {
        last->erase();
        last = last->next;
    }
#ifndef NDEBUG
    // if some objects have not been destroyed (memory leak)...
    if (count != 0) {
        // prints how many objects of each class have not been destroyed
        map<char*,int>::iterator i = counts->begin();
        while (i != counts->end()) {
            if (i->second != 0) {
                cerr << i->second << " remaining instance(s) of " << i->first << endl;
            }
#ifdef KEEP_OBJECT_REFERENCES
            set<Object*>* remainingInstances = findAllInstances(i->first);
            for (set<Object*>::iterator j = remainingInstances->begin(); j != remainingInstances->end(); ++j) {
                cerr << "\t" << (*j)->toString() << endl;
            }
#endif
            i++;
        }
        assert(false);
    }
#endif
}

#ifdef KEEP_OBJECT_REFERENCES
set<Object*>* Object::findAllInstances(const char* className)
{
    if (instances == NULL) {
        return NULL;
    }

    map<char*, set<Object*>* >::iterator classInstances = instances->find((char *)className);

    if (classInstances == instances->end()) {
        return NULL;
    } else {
        return classInstances->second;
    }
}
#endif

Object::static_ref *Object::statics = 0;

}
