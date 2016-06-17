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

#include "ork/render/AttributeBuffer.h"

#include <exception>
#include <GL/glew.h>

using namespace std;

namespace ork
{

AttributeBuffer::AttributeBuffer(int index, int size, AttributeType t, bool norm, ptr<Buffer> b, int stride, int offset, int divisor) :
        Object("AttributeBuffer"), index(index), size(size), type(t), I(false), L(false), norm(norm), b(b), stride(stride), offset(offset), divisor(0)
{
}

AttributeBuffer::AttributeBuffer(int index, int size, AttributeType t, ptr<Buffer> b, int stride, int offset, int divisor) :
        Object("AttributeBuffer"), index(index), size(size), type(t), I(true), L(false), norm(false), b(b), stride(stride), offset(offset), divisor(0)
{
}

AttributeBuffer::AttributeBuffer(int index, int size, ptr<Buffer> b, int stride, int offset, int divisor) :
        Object("AttributeBuffer"), index(index), size(size), type(A64F), I(true), L(true), norm(false), b(b), stride(stride), offset(offset), divisor(0)
{
}

AttributeBuffer::~AttributeBuffer()
{
}

int AttributeBuffer::getSize()
{
    return size;
}

AttributeType AttributeBuffer::getType()
{
    return type;
}

int AttributeBuffer::getAttributeSize()
{
    int size;
    switch (type) {
    case A8I:
    case A8UI:
        size = 1;
        break;
    case A16I:
    case A16UI:
    case A16F:
        size = 2;
        break;
    case A32I:
    case A32UI:
    case A32F:
        size = 4;
        break;
    case A64F:
        size = 8;
        break;
    case A32I_2_10_10_10_REV:
    case A32UI_2_10_10_10_REV:
        return 4;
    default:
        assert(false);
        throw exception();
    }
    return this->size * size;
}

ptr<Buffer> AttributeBuffer::getBuffer()
{
    return b;
}

int AttributeBuffer::getStride()
{
    return stride;
}

int AttributeBuffer::getOffset()
{
    return offset;
}

void AttributeBuffer::setBuffer(ptr<Buffer> b)
{
    this->b = b;
}

}
