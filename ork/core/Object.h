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

#ifndef _ORK_OBJECT_H_
#define _ORK_OBJECT_H_

#include <cstdio>
#include <cassert>

#ifdef USE_SHARED_PTR
#ifdef _MSC_VER
#include <memory>
#define TR1 std
#else
#include <tr1/memory>
#define TR1 std::tr1
#endif
#else
#include "ork/core/Atomic.h"
#endif

#ifndef NDEBUG
#include <map>
#include <set>
#endif

#ifndef NULL
#define NULL 0
#endif

// ---------------------------------------------------------------------------
// Static and dynamic assertions
// ---------------------------------------------------------------------------

#define CONCATENATE(arg1, arg2) arg1 ## arg2
#define CONCAT(arg1, arg2) CONCATENATE(arg1, arg2)
// remove below, it creates issues with static_assert which takes two
// arguments after C++11, and static_assert is not used in any Ork code
//#define static_assert(cond) typedef int CONCAT(ASSERTION_FAILED_AT_LINE_, __LINE__)[(cond) ? 1 : -1]

#ifndef NDEBUG
#undef assert
#define assert(e) if (!(e)) { ::assertAndSegfault(#e, __FILE__, __LINE__); }
ORK_API void assertAndSegfault(const char* a, const char* f, int l);
#endif

// ---------------------------------------------------------------------------
// Portable file functions
// ---------------------------------------------------------------------------

ORK_API void fopen(FILE **f, const char* fileName, const char *mode);

ORK_API void fseek64(FILE *f, long long offset, int origin);

// ---------------------------------------------------------------------------
// Object and smart pointer classes
// ---------------------------------------------------------------------------

/**
 * \defgroup ork ork
 * Provides the OpenGL Rendering Kernel (Ork).
 */
namespace ork
{

/**
 * @defgroup core core
 * @ingroup ork
 * Provides basic smart pointer classes, and logger and timer utilities.
 */

/**
 * An object with a reference counter. The reference count is managed by
 * the ptr and static_ptr classes to count the number of references to this
 * object. When this counter becomes 0 the object is automatically
 * destroyed. Instances of this class or of derived classes must NOT be
 * explicitely destroyed with the delete operator.
 * @ingroup core
 */
class ORK_API Object
{
public:
    /**
     * Creates a new object.
     *
     * @param type the name of the class of this object. For debug only.
     */
    Object(const char *type);

    /**
     * Destroys this object.
     */
    virtual ~Object();

    /**
     * Returns the name of the class of this object. For debug only.
     */
    const char* getClass() const;

    /**
     * Returns a string representation of this object.
     */
    virtual const char* toString();

    /**
     * Sets all static references to NULL.
     */
    static void exit();

#ifdef KEEP_OBJECT_REFERENCES
    /**
     * Returns a set containing all instances of a class, or NULL is non was created.
     */
    static std::set<Object*>* findAllInstances(const char* className);
#endif

protected:
    /**
     * The method called when the reference count of this object becomes 0.
     * The default implementation of this method deletes the object.
     */
    virtual void doRelease()
    {
        delete this;
    }

public: // in fact should be private, but then ptr could not access these members
#ifdef USE_SHARED_PTR
    /*
     * A weak pointer to this object.
     */
    TR1::weak_ptr<Object> ref_this;

    /*
     * Calls the #doRelease method on the given object.
     */
    static void release(Object *o)
    {
        if (o != NULL) {
            o->doRelease();
        }
    }
#else
    /*
     * Increments the reference counter of this object.
     */
    inline void acquire()
    {
        atomic_increment(&references);
    }

    /*
     * Decrements the reference counter of this object.
     */
    inline void release()
    {
        if (atomic_decrement(&references) == 1) {
            doRelease();
        }
    }
#endif

    /*
     * A static reference to an object.
     */
    class static_ref
    {
    protected:
        /**
         * Link to the next static reference.
         */
        static_ref *next;

        /**
         * Creates a new static reference and adds it to the list of all
         * static references.
         */
        static_ref()
        {
            next = Object::statics;
            Object::statics = this;
        }

        /**
         * Destroys this static reference.
         */
        virtual ~static_ref()
        {
        }

        /**
         * Sets this static reference to NULL.
         */
        virtual void erase() = 0;

        friend class Object;
    };

private:
#ifndef NDEBUG
    /**
     * The class of this object.
     */
    char *type;
#endif

#ifndef USE_SHARED_PTR
    /**
     * The number of references to this object.
     */
    int references;
#endif

#ifndef NDEBUG
    /**
     * The total number of objects currently allocated in memory.
     */
    static unsigned int count;

    /**
     * The number of objects currently allocated in memory, for each class.
     */
    static std::map<char*, int> *counts;
#endif

#ifdef KEEP_OBJECT_REFERENCES
    /**
     * Reference to all objects created (auto-list)
     */
    static map<char*, set<Object*>* >* instances;
#endif

    /**
     * The list of all static references.
     */
    static static_ref *statics;

    friend class static_ref;
};

/**
 * A strong pointer to an Object.
 * @ingroup core
 */
#ifdef USE_SHARED_PTR
template <class T>
class ptr : public TR1::shared_ptr<T>
{
private:
    template<typename D>
    inline ptr(T* target, D d) : TR1::shared_ptr<T>(target, d)
    {
        target->ref_this = TR1::weak_ptr<T>(*this);
    }

public:
    /**
     * Creates a pointer pointing to NULL.
     */
    inline ptr() : TR1::shared_ptr<T>()
    {
    }

    /**
     * Creates a pointer to the given object.
     */
    inline ptr(T *target) :
        TR1::shared_ptr<T>(target == NULL ? TR1::shared_ptr<T>() :
            target->ref_this.expired() ? ptr<T>(target, Object::release) :
                TR1::dynamic_pointer_cast<T>(TR1::shared_ptr<Object>(target->ref_this)))
    {
    }

    /**
     * Creates a pointer as a copy of the given pointer.
     */
    template<class U>
    inline ptr(const TR1::shared_ptr<U> &p) : TR1::shared_ptr<T>(p)
    {
    }

    /**
     * Creates a pointer as a copy of the given pointer.
     */
    template<class U>
    inline ptr(const TR1::weak_ptr<U> &p) : TR1::shared_ptr<T>(p)
    {
    }

    /**
     * Casts this pointer to a pointer of the given type with dynamic_cast.
     */
    template <class U>
    inline ptr<U> cast() const
    {
        return TR1::dynamic_pointer_cast<U>(*this);
    }
};
#else
template <class T>
class ptr
{
public:
    /**
     * Creates a strong pointer pointing to NULL.
     */
    inline ptr() : target(0)
    {
    }

    /**
     * Creates a strong pointer to the given object.
     */
    inline ptr(T *target) : target(target)
    {
        if (target != 0) {
            target->acquire();
        }
    }

    /**
     * Creates a strong pointer as a copy of the given pointer.
     */
    inline ptr(const ptr<T> &p) : target(p.get())
    {
        if (target != 0) {
            target->acquire();
        }
    }

    /**
     * Creates a strong pointer as a copy of the given pointer.
     */
    template<class U>
    inline ptr(const ptr<U> &p) : target(p.get())
    {
        if (target != 0) {
            target->acquire();
        }
    }

    /**
     * Destroys this strong pointer.
     */
    inline ~ptr()
    {
        if (target != 0) {
            T* oldTarget = target;
            target = 0;
            oldTarget->release();
        }
    }

    /**
     * Assigns the given pointer to this strong pointer.
     */
    inline void operator=(const ptr<T> &v)
    {
        // acquire must be done before release
        // to correctly handle self assignment cases
        if (v.target != 0) {
            v.target->acquire();
        }
        T* oldTarget = target;
        target = v.target;
        if (oldTarget != 0) {
            oldTarget->release();
        }
    }

    /**
     * Returns the target object of this strong pointer.
     */
    inline T *operator->() const
    {
        assert(target != NULL);
        return target;
    }

    /**
     * Returns the target object of this strong pointer.
     */
    inline T &operator*() const
    {
        assert(target != NULL);
        return *target;
    }

    /**
     * Returns the target object of this strong pointer.
     */
    inline T *get() const
    {
        return target;
    }

    /**
     * Returns true if this pointer and the given pointer point to the same
     * object.
     */
    inline bool operator==(const ptr<T> &v) const
    {
        return target == v.target;
    }

    /**
     * Returns true if this pointer and the given pointer point to different
     * objects.
     */
    inline bool operator!=(const ptr<T> &v) const
    {
        return target != v.target;
    }

    /**
     * Compares the addresses of the objects pointed by this pointer and by
     * the given pointer. Returns true if the address of the object pointed by
     * this pointer is less than the address of the object pointed by the
     * given pointer.
     */
    inline bool operator<(const ptr<T> &v) const
    {
        return target < v.target;
    }

    /**
     * Returns true if this strong pointer points to the given object.
     */
    inline bool operator==(const T *target) const
    {
        return this->target == target;
    }

    /**
     * Returns true if this strong pointer does not point to the given object.
     */
    inline bool operator!=(const T *target) const
    {
        return this->target != target;
    }

    /**
     * Casts this strong pointer to a strong pointer of the given type.
     */
    template<class U>
    inline ptr<U> cast() const
    {
        return ptr<U>(dynamic_cast<U*>(target));
    }

protected:
    /**
     * The object pointed by this strong pointer.
     */
    T *target;
};
#endif

/**
 * A static pointer to an Object.
 * static_ptr must be used instead of ptr for static variables.
 * @ingroup core
 */
template <class T>
class static_ptr : public ptr<T>, Object::static_ref
{
public:
    /**
     * Creates a static pointer pointing to NULL.
     */
    inline static_ptr() : ptr<T>(), static_ref()
    {
    }

    /**
     * Creates a static pointer to the given object.
     */
    inline explicit static_ptr(T *target) : ptr<T>(target), static_ref()
    {
    }

    /**
     * Destroys this static pointer.
     */
    inline ~static_ptr()
    {
    }

    /**
     * Assigns the given pointer value to this static pointer.
     */
    inline void operator=(const ptr<T> &v)
    {
        ptr<T>::operator=(v);
    }

protected:
    virtual void erase()
    {
        operator=(NULL);
    }
};

}

#undef TR1

#endif
