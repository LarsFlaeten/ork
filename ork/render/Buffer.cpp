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
