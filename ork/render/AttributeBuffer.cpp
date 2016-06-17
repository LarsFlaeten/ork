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

#include "ork/render/AttributeBuffer.h"

#include <exception>
#include <GL/glew.h>

using namespace std;

namespace ork
{

AttributeBuffer::AttributeBuffer(int index, int size, AttributeType t, bool norm, ptr<Buffer> b, int stride, int offset, int divisor) :
        Object("AttributeBuffer"), index(index), size(size), type(t), I(false), L(false), norm(norm), b(b), stride(stride), offset(offset), divisor(divisor)
{
}

AttributeBuffer::AttributeBuffer(int index, int size, AttributeType t, ptr<Buffer> b, int stride, int offset, int divisor) :
        Object("AttributeBuffer"), index(index), size(size), type(t), I(true), L(false), norm(false), b(b), stride(stride), offset(offset), divisor(divisor)
{
}

AttributeBuffer::AttributeBuffer(int index, int size, ptr<Buffer> b, int stride, int offset, int divisor) :
        Object("AttributeBuffer"), index(index), size(size), type(A64F), I(true), L(true), norm(false), b(b), stride(stride), offset(offset), divisor(divisor)
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
