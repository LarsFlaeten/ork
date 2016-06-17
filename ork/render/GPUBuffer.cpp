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

#include "ork/render/GPUBuffer.h"

#include <cassert>

#include <GL/glew.h>

#include "ork/core/Logger.h"
#include "ork/render/FrameBuffer.h"

// glMapBuffer and glUnmapBuffer seem inefficient.
// This option avoids theses calls by using a copy of
// the buffer data on CPU.

#define CUSTOM_MAP_BUFFER 1

namespace ork
{

GLenum getBufferUsage(BufferUsage u);

GLenum getBufferAccess(BufferAccess a);

#define MAX_UNIFORM_BUFFER_UNITS 64

/**
 * A uniform buffer unit.
 * Used to bind buffers used as uniform blocks in programs.
 */
class UniformBufferUnit
{
public:
    UniformBufferUnit(GLuint unit) :
        unit(unit), lastBindingTime(0), currentBufferBinding(NULL)
    {
    }

    void bind(const GPUBuffer *buffer, unsigned int time)
    {
        lastBindingTime = time;

        if (currentBufferBinding != NULL) {
            currentBufferBinding->currentUniformUnit = -1;
        }
        currentBufferBinding = buffer;
        if (currentBufferBinding != NULL) {
            currentBufferBinding->currentUniformUnit = unit;
        }

        if (buffer == NULL) {
            glBindBufferBase(GL_UNIFORM_BUFFER, unit, 0);
        } else {
            // TODO add support for glBindBufferRange
            //glBindBufferRange(GL_UNIFORM_BUFFER, unit, buffer->getId(), offset, size);
            glBindBufferBase(GL_UNIFORM_BUFFER, unit, buffer->getId());
        }
        assert(FrameBuffer::getError() == GL_NO_ERROR);
    }

    unsigned int getLastBindingTime() const
    {
        return lastBindingTime;
    }

    bool isFree() const
    {
        return currentBufferBinding == NULL;
    }

    const GPUBuffer *getCurrentBufferBinding() const
    {
        return currentBufferBinding;
    }

private:
    GLuint unit;

    unsigned int lastBindingTime;

    const GPUBuffer *currentBufferBinding;
};

/**
 * A uniform buffer unit manager.
 */
class UniformBufferManager
{
public:
    UniformBufferManager() : time(0)
    {
        GLuint maxUnits = getMaxUniformBufferUnits();
        for (GLuint i = 0; i < maxUnits; ++i) {
            units[i] = new UniformBufferUnit(i);
        }
    }

    int findFreeUnit(int programId)
    {
        for (GLuint i = 0; i < maxUnits; ++i) {
            if (units[i]->isFree()) {
                return i;
            }
        }

        int bestUnit = -1;
        unsigned int oldestBindingTime = time;

        for (GLuint i = 0; i < maxUnits; ++i) {
            const GPUBuffer *buffer = units[i]->getCurrentBufferBinding();
            if (!buffer->isUsedBy(programId)) {
                unsigned int bindingTime = units[i]->getLastBindingTime();
                if (bestUnit == -1 || bindingTime < oldestBindingTime) {
                    bestUnit = i;
                    oldestBindingTime = bindingTime;
                }
            }
        }

        assert(bestUnit != -1);
        return bestUnit;
    }

    void bind(GLuint i, const GPUBuffer *buffer)
    {
        units[i]->bind(buffer, time++);
    }

    void unbind(const GPUBuffer *buffer)
    {
        for (GLuint i = 0; i < maxUnits; ++i) {
            if (units[i]->getCurrentBufferBinding() == buffer) {
                units[i]->bind(NULL, time++);
            }
        }
    }

    void unbindAll()
    {
        for (GLuint i = 0; i < maxUnits; ++i) {
            units[i]->bind(NULL, 0);
        }
        time = 0;
    }

    static unsigned int getMaxUniformBufferUnits()
    {
        if (maxUnits == 0) {
            GLint maxUniformBufferBindings;
            GLint v, f, g, h;
            glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxUniformBufferBindings);
            glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &v);
            glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &f);
            glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &g);
            glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &h);
            maxUnits = std::min(maxUniformBufferBindings, MAX_UNIFORM_BUFFER_UNITS);
            maxUnits = std::min(maxUnits, GLuint(v));
            maxUnits = std::min(maxUnits, GLuint(f));
            maxUnits = std::min(maxUnits, GLuint(g));
            maxUnits = std::min(maxUnits, GLuint(h));

            if (Logger::DEBUG_LOGGER != NULL) {
                Logger::DEBUG_LOGGER->logf("OPENGL", "MAX_UNIFORM_BUFFER_BINDINGS = %d", maxUniformBufferBindings);
            }
        }
        return maxUnits;
    }

private:
    UniformBufferUnit* units[MAX_UNIFORM_BUFFER_UNITS];

    unsigned int time;

    static GLuint maxUnits;
};

GLuint UniformBufferManager::maxUnits = 0;

static UniformBufferManager* UNIFORM_BUFFER_MANAGER = NULL;

GPUBuffer::GPUBuffer() : size(0), mappedData(NULL), cpuData(NULL), currentUniformUnit(-1)
{
    if (UNIFORM_BUFFER_MANAGER == NULL) {
        UNIFORM_BUFFER_MANAGER = new UniformBufferManager();
    }

    glGenBuffers(1, &bufferId);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

GPUBuffer::~GPUBuffer()
{
    UNIFORM_BUFFER_MANAGER->unbind(this);

    if (cpuData != NULL) {
        delete[] cpuData;
    }

    glDeleteBuffers(1, &bufferId);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

GLuint GPUBuffer::getId() const
{
    return bufferId;
}

int GPUBuffer::getSize() const
{
    return size;
}

void GPUBuffer::setData(int size, const void *data, BufferUsage u)
{
    assert(mappedData == NULL);
    this->size = size;
    glBindBuffer(GL_COPY_WRITE_BUFFER, bufferId);
    glBufferData(GL_COPY_WRITE_BUFFER, size, data, getBufferUsage(u));
    glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
    assert(FrameBuffer::getError() == GL_NO_ERROR);

    if (cpuData != NULL) {
        delete[] cpuData;
        cpuData = NULL;
    }
#ifdef CUSTOM_MAP_BUFFER
    if (size < 1024) {
        cpuData = new unsigned char[size];
        if (data != NULL) {
            memcpy(cpuData, (unsigned char*) data, size);
        }
    }
#endif
}

void GPUBuffer::setSubData(int target, int offset, int size, const void *data)
{
    assert(mappedData == NULL);
    glBindBuffer(GL_COPY_WRITE_BUFFER, bufferId);
    glBufferSubData(GL_COPY_WRITE_BUFFER, offset, size, data);
    glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
    assert(FrameBuffer::getError() == GL_NO_ERROR);

    if (cpuData != NULL) {
        memcpy(cpuData + offset, (unsigned char*) data, size);
    }
}

void GPUBuffer::getSubData(int target, int offset, int size, void *data)
{
    assert(mappedData == NULL);
    glBindBuffer(GL_COPY_READ_BUFFER, bufferId);
    glGetBufferSubData(GL_COPY_READ_BUFFER, offset, size, data);
    glBindBuffer(GL_COPY_READ_BUFFER, 0);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

volatile void *GPUBuffer::map(BufferAccess a)
{
    assert(mappedData == NULL);

    if (cpuData != NULL) {
        mappedData = cpuData;
    } else {
        glBindBuffer(GL_COPY_READ_BUFFER, bufferId);
        mappedData = glMapBuffer(GL_COPY_READ_BUFFER, getBufferAccess(a));
        glBindBuffer(GL_COPY_READ_BUFFER, 0);
        assert(FrameBuffer::getError() == GL_NO_ERROR);
    }

    return mappedData;
}

volatile void *GPUBuffer::getMappedData()
{
    return mappedData;
}

void GPUBuffer::unmap()
{
    assert(mappedData != NULL);

    if (cpuData != NULL) {
        glBindBuffer(GL_COPY_WRITE_BUFFER, bufferId);
        glBufferSubData(GL_COPY_WRITE_BUFFER, 0, size, cpuData);
        glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
        assert(FrameBuffer::getError() == GL_NO_ERROR);
    } else {
        glBindBuffer(GL_COPY_READ_BUFFER, bufferId);
        glUnmapBuffer(GL_COPY_READ_BUFFER);
        glBindBuffer(GL_COPY_READ_BUFFER, 0);
        assert(FrameBuffer::getError() == GL_NO_ERROR);
    }

    mappedData = NULL;
}

void GPUBuffer::bind(int target) const
{
    glBindBuffer(target, bufferId);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void *GPUBuffer::data(int offset) const
{
    return (void*) offset;
}

void GPUBuffer::unbind(int target) const
{
    glBindBuffer(target, 0);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void GPUBuffer::addUser(GLuint programId) const
{
    assert(!isUsedBy(programId));
    programIds.push_back(programId);
}

void GPUBuffer::removeUser(GLuint programId) const
{
    vector<GLuint>::iterator i = find(programIds.begin(), programIds.end(), programId);
    assert(i != programIds.end());
    programIds.erase(i);
}

bool GPUBuffer::isUsedBy(GLuint programId) const
{
    return find(programIds.begin(), programIds.end(), programId) != programIds.end();
}

GLint GPUBuffer::bindToUniformBufferUnit(int programId) const
{
    assert(programId != 0);

    GLint unit = currentUniformUnit;
    if (unit == -1) {
        unit = UNIFORM_BUFFER_MANAGER->findFreeUnit(programId);
    }

    UNIFORM_BUFFER_MANAGER->bind(GLuint(unit), this);

    return unit;
}

}
