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

#include "ork/core/Timer.h"

#include <algorithm>

// Selects the implementation of getCurrentTime on Windows
// 0: use timeGetTime
// 1: use QueryPerformanceCounter (the default)
// timeGetTime is less precise (milliseconds) but QueryPerformanceCounter
// is less reliable (the cpuFrequency can vary due to power saving modes,
// and counters are not always correctly synchronized between cores, which
// can make time goes backward when a thread is moved from one core to
// another. See http://www.virtualdub.org/blog/pivot/entry.php?id=106)
#ifndef ORK_TIMER_USE_QPC
#define ORK_TIMER_USE_QPC 1
#endif

#if defined( _WIN64 ) || defined( _WIN32 )
#   if ORK_TIMER_USE_QPC
#       include <windows.h>
#   else
#       include <windows.h>
#       include <mmsystem.h>
#   endif
#else
#   include <sys/time.h>
#   include <unistd.h>
#endif

#include <time.h>

using namespace std;

namespace ork
{

#if ORK_TIMER_USE_QPC && (defined( _WIN64 ) || defined( _WIN32 ))
__int64 cpuFrequency = 0;
#endif

Timer::Timer()
{
    numCycles = 0;
    running = false;
    minDuration = 1e9;
    maxDuration = 0.0;
    lastDuration = 0.0;
    totalDuration = 0.0;

#if ORK_TIMER_USE_QPC && (defined( _WIN64 ) || defined( _WIN32 ))
    if (cpuFrequency == 0) {
        QueryPerformanceFrequency((LARGE_INTEGER*) &cpuFrequency);
    }
#endif
}

Timer::~Timer()
{
}

double Timer::start()
{
    running = true;
    numCycles++;
    t = getCurrentTime();
    return t;
}

double Timer::end()
{
    lastDuration = getCurrentTime() - t;
    totalDuration += lastDuration;
    minDuration = min(lastDuration, minDuration);
    maxDuration = max(lastDuration, maxDuration);
    running = false;
    return lastDuration;
}

void Timer::reset()
{
    start();
    numCycles = 0;
    running = false;
    minDuration = 1e9;
    maxDuration = 0.0;
    lastDuration = 0.0;
    totalDuration = 0.0;
}

int Timer::getNumCycles() const
{
    return numCycles;
}

double Timer::getTime()
{
    return lastDuration;
}

double Timer::getAvgTime()
{
    if (numCycles == 0) {
        return 0.0;
    }
    if (running) {
        end();
    }
    return totalDuration / numCycles;
}

double Timer::getMinDuration() const
{
    return minDuration;
}

double Timer::getMaxDuration() const
{
    return maxDuration;
}

void Timer::getDateTimeString(char* buffer, int bufSize)
{
#if defined(_MSC_VER)
    char sdate[64];
    char stimeoftheday[64];
    getDateString(sdate, 64);
    getTimeOfTheDayString(stimeoftheday, 64);
    sprintf(buffer, "%s.%s", sdate, stimeoftheday);
#else // Posix
    time_t aclock;
    time(&aclock);
    struct tm *newtime = localtime(&aclock);
    strftime(buffer, bufSize, "%Y.%m.%d.%H.%M.%S", newtime);
#endif
}

void Timer::getDateString(char* buffer, int bufSize)
{
#if defined(_MSC_VER)
    GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, NULL, "yyyy'.'MM'.'Dd", buffer, 256);
#else // Posix
    time_t aclock;
    time(&aclock);
    struct tm *newtime = localtime(&aclock);
    strftime(buffer, bufSize, "%Y.%m.%d", newtime);
#endif
}

void Timer::getTimeOfTheDayString(char* buffer, int bufSize)
{
#if defined(_MSC_VER)
    GetTimeFormat(LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, NULL, "HH'.'mm'.'ss", buffer, 256);
#else // Posix
    time_t aclock;
    time(&aclock);
    struct tm *newtime = localtime(&aclock);
    strftime(buffer, bufSize, "%H.%M.%S", newtime);
#endif
}

double Timer::getCurrentTime()
{
#if defined( _WIN64 ) || defined( _WIN32 )
#   if ORK_TIMER_USE_QPC
        __int64 tv;
        QueryPerformanceCounter((LARGE_INTEGER*) &tv);
        return tv / double(cpuFrequency) * 1e6;
#   else
        // on Windows > 9x, getTickCount may be as accurate
        // (and does not require to link to winmm.lib)
        return timeGetTime() * 1000.0;
#   endif
#else
    timeval u;
    gettimeofday(&u, NULL);
    return double(u.tv_sec) * 1e6 + double(u.tv_usec);
#endif
}

}
