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

#include "ork/render/Query.h"

#include <GL/glew.h>

namespace ork
{

Query::Query(QueryType type) : Object("Query"), type(type), resultAvailable(false), resultRead(false)
{
    glGenQueries(1, &id);
    switch (type) {
    case PRIMITIVES_GENERATED:
        target = GL_PRIMITIVES_GENERATED;
        break;
    case TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN:
        target = GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN;
        break;
    case SAMPLES_PASSED:
        target = GL_SAMPLES_PASSED;
        break;
    case ANY_SAMPLES_PASSED:
        target = GL_ANY_SAMPLES_PASSED;
        break;
    case TIME_STAMP:
        target = GL_TIME_ELAPSED;
        break;
    }
}

Query::~Query()
{
    glDeleteQueries(1, &id);
}

QueryType Query::getType()
{
    return type;
}

GLuint Query::getId()
{
    return id;
}

void Query::begin()
{
    glBeginQuery(target, id);
    if (target == GL_TIME_ELAPSED) {
        glQueryCounter(id, GL_TIMESTAMP);
    }
}

void Query::end()
{
    glEndQuery(target);
}

bool Query::available()
{
    if (!resultAvailable) {
        GLuint result;
        glGetQueryObjectuiv(id, GL_QUERY_RESULT_AVAILABLE, &result);
        resultAvailable = result != 0;
    }
    return resultAvailable;
}

GLuint64 Query::getResult()
{
    if (!resultRead) {
        glGetQueryObjectui64v(id, GL_QUERY_RESULT, &result);
        resultRead = true;
    }
    return result;
}

}
