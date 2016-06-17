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

#ifndef _ORK_TIMER_H_
#define _ORK_TIMER_H_

namespace ork
{

/**
 * A timer to measure time and time intervals.
 * @ingroup core
 */
class ORK_API Timer
{
public:
    /**
     * Creates a new timer.
     */
    Timer();

    /**
     * Destroys this timer.
     */
    virtual ~Timer();

    /**
     * Starts this timer and returns the current time in micro seconds.
     */
    virtual double start();

    /**
     * Returns the delay since the last call to #start() in micro seconds.
     */
    virtual double end();

    /**
     * Returns the delay recorded at the last end() call.
     */
    virtual double getTime();

    /**
     * Returns the average delay at every call to #end() in micro seconds.
     * This won't be accurate if the timer is not stopped.
     */
    virtual double getAvgTime();

    /**
     * Returns the number of calls to start since last reset().
     */
    int getNumCycles() const;

    /**
     * Returns the lowest duration between a start() and an end() call in micro seconds.
     */
    double getMinDuration() const;

    /**
     * Returns the highest duration between a start() and an end() call in micro seconds.
     */
    double getMaxDuration() const;

    /**
     * Resets the statistics (total, average, min, and max durations).
     */
    virtual void reset();

    /**
     * Get a string based on the current date and time of the day.
     * Buffer must be of sufficient length.
     * Format YYYY.MM.DD.HH.MM.SS
     */
    static void getDateTimeString(char* buffer, int bufSize);

    /**
     * Get a string based on the current date.
     * Buffer must be of sufficient length.
     * Format YYYY.MM.DD
     */
    static void getDateString(char* buffer, int bufSize);

    /**
     * Get a string based on the current time of the day.
     * Buffer must be of sufficient length.
     * Format HH.MM.SS
     */
    static void getTimeOfTheDayString(char* buffer, int bufSize);

protected:
    /**
     * Time of last call to #start or #reset.
     */
    double t;

    /**
     * The accumulated elapsed time.
     */
    double totalDuration;

    /**
     * Number of calls to start since last #reset() call.
     */
    int numCycles;

    /**
     * Last recorded duration recorded at #end() call.
     */
    double lastDuration;

    /**
     * The lowest duration between a #start() and an #end() call in micro seconds.
     */
    double minDuration;

    /**
     * The highest duration between a #start() and an #end() call in micro seconds.
     */
    double maxDuration;

    /**
     * True if the timer has a start value.
     */
    bool running;

    /**
     * Returns the current time in microseconds.
     * The origin of time may depend on the platform.
     */
    double getCurrentTime();
};

}

#endif
