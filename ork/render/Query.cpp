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
    resultRead = false;
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
