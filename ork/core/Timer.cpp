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
    if (cpuFrequency == 0)
    {
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
    if (numCycles == 0)
    {
        return 0.0;
    }
    if (running)
    {
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
