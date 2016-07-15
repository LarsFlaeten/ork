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

#include "DebugCallback.h"

#include "ork/core/Logger.h"

#include <GL/glew.h>

#include <assert.h>

#include <string.h>

#ifndef CALLBACK
#define CALLBACK
#endif

using namespace std;

namespace ork
{

void CALLBACK debugCallback(unsigned int source, unsigned int type,
    unsigned int id, unsigned int severity,
    int length, const char* message, const void* userParam)
{
    char debSource[16];
    switch (source) {
    case GL_DEBUG_SOURCE_API_ARB:
        strcpy(debSource, "OPENGL");
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
        strcpy(debSource, "WINDOWS");
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
        strcpy(debSource, "COMPILER");
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
        strcpy(debSource, "LIBRARY");
        break;
    case GL_DEBUG_SOURCE_APPLICATION_ARB:
        strcpy(debSource, "APPLICATION");
        break;
    //case GL_DEBUG_SOURCE_OTHER_ARB:
    default:
        strcpy(debSource, "UNKNOWN");
    }

    char debType[20];
    switch (type) {
    case GL_DEBUG_TYPE_ERROR_ARB:
        strcpy(debType, "Error");
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
        strcpy(debType, "Deprecated behavior");
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
        strcpy(debType, "Undefined behavior");
        break;
    case GL_DEBUG_TYPE_PORTABILITY_ARB:
        strcpy(debType, "Portability");
        break;
    case GL_DEBUG_TYPE_PERFORMANCE_ARB:
        strcpy(debType, "Performance");
        break;
    //case GL_DEBUG_TYPE_OTHER_ARB:
    default:
        strcpy(debType, "Other");
    }

    if (severity == GL_DEBUG_SEVERITY_HIGH_ARB && Logger::ERROR_LOGGER != NULL) {
        Logger::ERROR_LOGGER->logf(debSource, "%s: %s", debType, message);
    }
    if (severity == GL_DEBUG_SEVERITY_MEDIUM_ARB && Logger::WARNING_LOGGER != NULL) {
        Logger::WARNING_LOGGER->logf(debSource, "%s: %s", debType, message);
    }
    if (severity == GL_DEBUG_SEVERITY_LOW_ARB && Logger::INFO_LOGGER != NULL) {
        Logger::INFO_LOGGER->logf(debSource, "%s: %s", debType, message);
    }
}


}

