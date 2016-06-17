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

#include "ork/render/Buffer.h"

#include <GL/glew.h>

namespace ork
{

Buffer::Parameters::Parameters() :
    _swapBytes(false), _leastSignificantBitFirst(false), _alignment(4),
    _compressedSize(0), _subImage2D(0, 0, 0), _subImage3D(0, 0), modified(false)
{
}

bool Buffer::Parameters::swapBytes() const
{
    return _swapBytes;
}

bool Buffer::Parameters::leastSignificantBitFirst() const
{
    return _leastSignificantBitFirst;
}

GLint Buffer::Parameters::alignment() const
{
    return _alignment;
}

GLsizei Buffer::Parameters::compressedSize() const
{
    return _compressedSize;
}

vec3<GLint> Buffer::Parameters::subImage2D() const
{
    return _subImage2D;
}

vec2<GLint> Buffer::Parameters::subImage3D() const
{
    return _subImage3D;
}

Buffer::Parameters& Buffer::Parameters::swapBytes(bool swapBytes)
{
    _swapBytes = swapBytes;
    modified = true;
    return *this;
}

Buffer::Parameters& Buffer::Parameters::leastSignificantBitFirst(bool leastSignificantBitFirst)
{
    _leastSignificantBitFirst = leastSignificantBitFirst;
    modified = true;
    return *this;
}

Buffer::Parameters& Buffer::Parameters::alignment(GLint alignment)
{
    _alignment = alignment;
    modified = true;
    return *this;
}

Buffer::Parameters& Buffer::Parameters::compressedSize(GLsizei compressedSize)
{
    _compressedSize = compressedSize;
    // do NOT set modified to true!
    return *this;
}

Buffer::Parameters& Buffer::Parameters::subImage2D(GLint skipPixels, GLint skipRows, GLint rowLength)
{
    _subImage2D = vec3<GLint>(skipPixels, skipRows, rowLength);
    modified = true;
    return *this;
}

Buffer::Parameters& Buffer::Parameters::subImage3D(GLint skipImages, GLint imageHeight)
{
    _subImage3D = vec2<GLint>(skipImages, imageHeight);
    modified = true;
    return *this;
}

void Buffer::Parameters::set() const
{
    if (modified) {
        glPixelStorei(GL_UNPACK_SWAP_BYTES, _swapBytes);
        glPixelStorei(GL_UNPACK_LSB_FIRST, _leastSignificantBitFirst);
        glPixelStorei(GL_UNPACK_ALIGNMENT, _alignment);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, _subImage2D.x);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, _subImage2D.y);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, _subImage2D.z);
        glPixelStorei(GL_UNPACK_SKIP_IMAGES, _subImage3D.x);
        glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, _subImage3D.y);
    }
}

void Buffer::Parameters::unset() const
{
    if (modified) {
        glPixelStorei(GL_UNPACK_SWAP_BYTES, false);
        glPixelStorei(GL_UNPACK_LSB_FIRST, false);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_SKIP_IMAGES, 0);
        glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);
    }
}

Buffer::Buffer() : Object("Buffer")
{
}

Buffer::~Buffer()
{
}

}
