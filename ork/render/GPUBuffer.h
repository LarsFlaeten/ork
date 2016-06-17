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
     * @param offset index of the first byte to be replaced.
     * @param size number of bytes in 'data'.
     * @param data the new buffer data.
     */
    void setSubData(int offset, int size, const void *data);

     /**
     * Gets a part of the content of this buffer.
     *
     * @param offset index of the first byte to be replaced.
     * @param size number of bytes in 'data'.
     * @param data the new buffer data.
     */
    void getSubData(int offset, int size, void *data);

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

    virtual void dirty() const;

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
     * True if cpuData is dirty because the buffer data has changed on GPU
     * (via readPixels, transformFeedback, etc).
     */
    mutable bool isDirty;

    /**
     * The uniform block binding unit to which this buffer is currently bound,
     * or -1 if it is not bound to any uniform block binding unit.
     */
    mutable int currentUniformUnit;

    /**
     * Identifiers of the programs that use this buffer as a uniform block.
     */
    mutable std::vector<GLuint> programIds;

    /**
     * Adds the given program as a user of this buffer as a uniform block.
     */
    void addUser(GLuint programId) const;

    /**
     * Removes the given program as a user of this buffer as a uniform block.
     */
    void removeUser(GLuint programId) const;

    /**
     * Returns true if one of the given programs uses this buffer as a uniform block.
     */
    bool isUsedBy(const std::vector<GLuint> &programIds) const;

    /**
     * Binds this buffer to a uniform block binding unit not currently used
     * by the given programs. If all the uniform block binding units are
     * currently bound, a unit not used by the given programs will be unbound
     * and reused to bind this buffer.
     *
     * @param programIds the ids of programs that must use this buffer as a
     *      uniform block.
     * @return the uniform block binding unit to which this buffer has been
     *      bound, or -1 if no unit was available (meaning that the program
     *      uses too much uniform blocks).
     */
    GLint bindToUniformBufferUnit(const std::vector<GLuint> &programIds) const;

    friend class UniformBufferUnit;

    friend class UniformBufferManager;

    friend class UniformBlock;

    friend class Program;

    friend class FrameBuffer;
};

}

#endif
