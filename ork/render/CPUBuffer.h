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

#ifndef _ORK_CPU_BUFFER_H_
#define _ORK_CPU_BUFFER_H_

#include "ork/render/Buffer.h"

namespace ork
{

/**
 * A Buffer whose data is on the CPU.
 * @ingroup render
 */
class ORK_API CPUBuffer : public Buffer
{
public:
    /**
     * Creates a new CPU buffer with the given data.
     *
     * @param data the buffer data. May be NULL.
     */
    CPUBuffer(const void *data = 0);

    /**
     * Destroys this CPU buffer. The buffer data itself is NOT destroyed.
     */
    virtual ~CPUBuffer();

protected:
    virtual void bind(int target) const;

    virtual void *data(int offset) const;

    virtual void unbind(int target) const;

private:
    /**
     * The buffer data. May be NULL.
     */
    const void *p;

    friend class FrameBuffer;
};

}

#endif
