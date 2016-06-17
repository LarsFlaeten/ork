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

#include "ork/render/CPUBuffer.h"

#include <GL/glew.h>

#include "ork/render/FrameBuffer.h"

namespace ork
{

CPUBuffer::CPUBuffer(const void *data)
{
    p = data;
}

CPUBuffer::~CPUBuffer()
{
}

void CPUBuffer::bind(int target) const
{
    glBindBuffer(target, 0);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void *CPUBuffer::data(int offset) const
{
    return (void*)((char*)p + offset);
}

void CPUBuffer::unbind(int target) const
{
}

}
