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

#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#if defined(_MSC_VER) // MSVC
#include <intrin.h>
#endif

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

#define atomic_exchange_and_add(pw,dv) _InterlockedExchangeAdd((volatile long*)(pw),(dv))
#define atomic_increment(pw) (_InterlockedIncrement((volatile long*)(pw)))
#define atomic_decrement(pw) (_InterlockedDecrement((volatile long*)(pw))+1)
#elif defined(__GNUC__) // GCC

#define atomic_exchange_and_add(pw,dv) __sync_fetch_and_add((volatile long*)(pw), dv)
#define atomic_increment(pw) __sync_fetch_and_add((volatile long*)(pw), 1)
#define atomic_decrement(pw) __sync_fetch_and_sub((volatile long*)(pw), 1)

#else

#error Unsupported compiler

#endif

}

#endif
