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

#include "ork/render/TextureBuffer.h"

#include <exception>

#include <GL/glew.h>

#include "ork/render/FrameBuffer.h"

using namespace std;

namespace ork
{

GLenum getTextureInternalFormat(TextureInternalFormat f);

TextureBuffer::TextureBuffer(TextureInternalFormat tf, ptr<GPUBuffer> pixels) : Texture("TextureBuffer", GL_TEXTURE_BUFFER)
{
    int formatSize;
    switch (tf) {
    case R8:
    case R8I:
    case R8UI:
        formatSize = 1;
        break;
    case R16:
    case R16I:
    case R16UI:
    case R16F:
    case RG8:
    case RG8I:
    case RG8UI:
        formatSize = 2;
        break;
    case R32I:
    case R32UI:
    case R32F:
    case RG16:
    case RG16I:
    case RG16UI:
    case RG16F:
    case RGBA8:
    case RGBA8I:
    case RGBA8UI:
        formatSize = 4;
        break;
    case RG32I:
    case RG32UI:
    case RG32F:
    case RGBA16:
    case RGBA16I:
    case RGBA16UI:
    case RGBA16F:
        formatSize = 8;
        break;
    case RGBA32I:
    case RGBA32UI:
    case RGBA32F:
        formatSize = 16;
        break;
    default:
        assert(false); // other formats not allowed for texture buffers
        throw exception();
    }

    Parameters params;
    params.wrapS(CLAMP_TO_EDGE);
    params.wrapT(CLAMP_TO_EDGE);
    params.wrapR(CLAMP_TO_EDGE);
    params.min(NEAREST);
    params.mag(NEAREST);
    params.maxLevel(0);

    Texture::init(tf, params);
    this->w = pixels->getSize() / formatSize;
    this->b = pixels;

    glTexBuffer(textureTarget, getTextureInternalFormat(internalFormat), pixels->getId());

    if (FrameBuffer::getError() != 0) {
        throw exception();
    }
}

TextureBuffer::~TextureBuffer()
{
}

int TextureBuffer::getWidth()
{
    return w;
}

ptr<GPUBuffer> TextureBuffer::getBuffer()
{
    return b;
}

}
