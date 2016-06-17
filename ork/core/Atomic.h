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

#ifndef _ATOMIC_H_
#define _ATOMIC_H_

namespace ork
{

//#define SINGLE_THREAD

/**
 * This file defines some atomic operations.
 * Those are supported across MSVC and GCC.
 *
 * Implements needed atomic operations.
 * - atomic_exchange_and_add(*pw, dv)
 *        adds dv to *pw and returns the old value of pw
 *
 * - atomic_increment(*pw)
 *        adds 1 to *pw and returns void
 *
 * - atomic_decrement(*pw)
 *        adds 1 to *pw and returns its *previous* value
 */

#if defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE __attribute__((always_inline))
#endif

#ifdef SINGLE_THREAD

static FORCE_INLINE int atomic_exchange_and_add(int volatile * pw, int dv)
{
    int r = *pw;
    *pw += dv;
    return r;
}

static FORCE_INLINE void atomic_increment(int volatile * pw)
{
    (*pw)++;
}

static FORCE_INLINE int atomic_decrement(int volatile * pw)
{
    return (*pw)--;
}

#elif defined(_MSC_VER) // MSVC

// the intrinsics don't work in release mode, so there is asm

inline __declspec(naked) int __fastcall atomic_exchange_and_add(int volatile * pw, int dv)
{
    //return _InterlockedExchangeAdd(pw, dv);
    __asm
    {
        lock xadd dword ptr [ecx], edx
        mov eax, edx
        ret
    }
}

inline __declspec(naked) void __fastcall atomic_increment(int volatile * pw)
{
    //return _InterlockedExchangeAdd(pw, -1);
    __asm
    {
        mov eax, 1
        lock xadd dword ptr [ecx], eax
        ret
    }
}

inline __declspec(naked) int __fastcall atomic_decrement(int volatile * pw)
{
    //    return _InterlockedDecrement(reinterpret_cast<volatile long* >(pw));
    __asm
    {
        mov eax, 0xffffffff
        lock xadd dword ptr [ecx], eax
        ret
    }
}

#elif defined(__GNUC__) // GCC

/*
#define atomic_exchange_and_add(a,b) __gnu_cxx::__exchange_and_add((a),(b))
#define atomic_increment(a) __gnu_cxx::__atomic_add((a),1)
#define atomic_decrement(a) __gnu_cxx::__exchange_and_add((a),-1)
*/

static inline int atomic_exchange_and_add(int volatile *pw, int dv)
{
    int r;
    __asm__ __volatile__
    (
        "lock\n\t"
        "xadd %1, %0":
        "=m"( *pw ), "=r"( r ): // outputs (%0, %1)
        "m"( *pw ), "1"( dv ): // inputs (%2, %3 == %1)
        "memory", "cc" // clobbers
    );
    return r;
}

static inline void atomic_increment(int volatile *pw)
{
    __asm__ __volatile__
    (
        "lock\n\t"
        "incl %0":
        "=m"( *pw ): // output (%0)
        "m"( *pw ): // input (%1)
        "cc" // clobbers
    );
}

static inline int atomic_decrement(int volatile *pw)
{
    return atomic_exchange_and_add(pw, -1);
}

#else

#error Unsupported compiler

#endif

}

#endif
