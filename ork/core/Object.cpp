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

#undef __STRICT_ANSI__

#include "ork/core/Object.h"

#include <sstream>
#include <iostream>

#include "ork/core/Logger.h"

using namespace std;

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

#ifdef USE_SHARED_PTR
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
