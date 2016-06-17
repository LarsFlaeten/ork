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

#include "ork/core/GPUTimer.h"

#include <algorithm>

#include "GL/glew.h"

namespace ork
{

GPUTimer::GPUTimer() : Timer()
{
    glGenQueries(1, &query);
}

GPUTimer::~GPUTimer()
{
    glDeleteQueries(1, &query);
}

double GPUTimer::start()
{
    getQueryResult();
    numCycles++;
    glBeginQuery(GL_TIME_ELAPSED, query);
    return 0.0;
}

double GPUTimer::end()
{
    glEndQuery(GL_TIME_ELAPSED);
    return lastDuration;
}

double GPUTimer::getTime()
{
    if (lastDuration == 0) {
        getQueryResult();
    }
    return lastDuration;
}

double GPUTimer::getAvgTime()
{
    if (numCycles == 0) {
        return 0.0;
    }
    return totalDuration / numCycles;
}

void GPUTimer::getQueryResult()
{
    if (numCycles > 0) {
        GLuint64 timeElapsed;
        glGetQueryObjectui64v(query, GL_QUERY_RESULT, &timeElapsed);
        if (timeElapsed != 0) {
            lastDuration = (double) timeElapsed;
            totalDuration += lastDuration;
            minDuration = std::min(lastDuration, minDuration);
            maxDuration = std::max(lastDuration, maxDuration);
        }
    }
}

}
