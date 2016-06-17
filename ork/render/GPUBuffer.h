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

#ifndef _ORK_GPU_BUFFER_H_
#define _ORK_GPU_BUFFER_H_

#include <vector>

#include "ork/render/Types.h"
#include "ork/render/Buffer.h"

namespace ork
{

/**
 * A Buffer whose data is on the GPU.
 * @ingroup render
 */
class ORK_API GPUBuffer : public Buffer
{
public:
    /**
     * Creates a new GPU buffer with no associated data.
     */
    GPUBuffer();

    /**
     * Destroys this GPU buffer. The buffer data itself is also destroyed.
     */
    virtual ~GPUBuffer();

    /**
     * Returns the id of this buffer.
     */
    GLuint getId() const;

    /**
     * Returns the size of this buffer.
     */
    int getSize() const;

    /**
     * Sets the content of this buffer. The previous content is erased and
     * replaced by the new one.
     *
     * @param size number of bytes in 'data'.
     * @param data the new buffer data. May be NULL.
     * @param u how this buffer will be used.
     */
    void setData(int size, const void *data, BufferUsage u);

    /**
     * Replaces a part of the content of this buffer.
     *
     * @param target the target to bind to (use 0 for default).
     * @param offset index of the first byte to be replaced.
     * @param size number of bytes in 'data'.
     * @param data the new buffer data.
     */
    void setSubData(int target, int offset, int size, const void *data);

     /**
     * Gets a part of the content of this buffer.
     *
     * @param target the target to bind to (use 0 for default).
     * @param offset index of the first byte to be replaced.
     * @param size number of bytes in 'data'.
     * @param data the new buffer data.
     */
    void getSubData(int target, int offset, int size, void *data);

    /**
     * Maps this buffer into CPU memory and returns a pointer to it. If the
     * access mode is not READ_ONLY, changes made to the mapped buffer in CPU
     * memory are reflected on GPU when the buffer is unmapped.
     *
     * @param a the read and write permissions for this mapped memory region.
     */
    volatile void *map(BufferAccess a);

    /**
     * Returns the mapped data of this buffer, or NULL if it is currently unmapped.
     */
    volatile void *getMappedData();

    /**
     * Unmaps this buffer from CPU memory.
     */
    void unmap();

protected:
    virtual void bind(int target) const;

    /**
     * Returns (void*) offset.
     */
    virtual void *data(int offset) const;

    virtual void unbind(int target) const;

private:
    /**
     * The OpenGL buffer identifier of this buffer (as returned by glGenBuffers).
     */
    GLuint bufferId;

    /**
     * The size of this buffer.
     */
    int size;

    /**
     * The mapped data of this buffer. NULL if the buffer is currently unmapped.
     */
    volatile void *mappedData;

    /**
     * Optional copy of buffer data on CPU.
     */
    unsigned char *cpuData;

    /**
     * The uniform block binding unit to which this buffer is currently bound,
     * or -1 if it is not bound to any uniform block binding unit.
     */
    mutable int currentUniformUnit;

    /**
     * Identifiers of the programs that use this buffer as a uniform block.
     */
    mutable vector<GLuint> programIds;

    /**
     * Adds the given program as a user of this buffer as a uniform block.
     */
    void addUser(GLuint programId) const;

    /**
     * Removes the given program as a user of this buffer as a uniform block.
     */
    void removeUser(GLuint programId) const;

    /**
     * Returns true if the given program uses this buffer as a uniform block.
     */
    bool isUsedBy(GLuint programId) const;

    /**
     * Binds this buffer to a uniform block binding unit not currently used
     * by the given program. If all the uniform block binding units are
     * currently bound, a unit not used by the given program will be unbound
     * and reused to bind this buffer.
     *
     * @param programId the id of a program that must use this buffer as a
     *      uniform block.
     * @return the uniform block binding unit to which this buffer has been
     *      bound, or -1 if no unit was available (meaning that the program
     *      uses too much uniform blocks).
     */
    GLint bindToUniformBufferUnit(int programId) const;

    friend class UniformBufferUnit;

    friend class UniformBufferManager;

    friend class Program;

    friend class FrameBuffer;
};

}

#endif
