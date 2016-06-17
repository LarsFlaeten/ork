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

#ifndef _ORK_GPU_TIMER_H_
#define _ORK_GPU_TIMER_H_

#include "ork/core/Timer.h"

namespace ork
{

/**
 * A timer to measure time and time intervals on GPU. Since GPU computations
 * are asynchroneous, we can't just use CPU time to check the duration of
 * an operation.
 * @ingroup core
 */
class ORK_API GPUTimer : public Timer
{
public:
    /**
     * Creates a new GPUTimer.
     */
    GPUTimer();

    /**
     * Destroys this timer.
     */
    virtual ~GPUTimer();

    /**
     * Starts this timer and returns the current time in micro seconds.
     */
    virtual double start();

    /**
     * Returns the delay since the last call to #start() in micro seconds.
     */
    virtual double end();

    /**
     * See Timer#getTime().
     */
    virtual double getTime();

    /**
     * See Timer#getAvgTime().
     */
    virtual double getAvgTime();

protected:
    unsigned int query; ///< GPU query used to measure time on GPU.

    void getQueryResult(); ///< Returns the result of the query #query.
};

}

#endif
