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
