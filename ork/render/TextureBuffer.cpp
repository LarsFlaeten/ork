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
