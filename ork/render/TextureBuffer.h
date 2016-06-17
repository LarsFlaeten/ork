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

#ifndef _ORK_TEXTURE_BUFFER_H_
#define _ORK_TEXTURE_BUFFER_H_

#include "ork/render/GPUBuffer.h"
#include "ork/render/Texture.h"

namespace ork
{

/**
 * A buffer texture. A buffer texture is similar to a one-dimensional texture.
 * However, unlike other texture types, the texel array is not stored as part
 * of the texture. Instead, a buffer object is attached to a buffer texture
 * and the texel array is taken from the data store of an attached buffer
 * object.  When the contents of a buffer object's data store are modified,
 * those changes are reflected in the contents of any buffer texture to which
 * the buffer object is attached.  Also unlike other textures, buffer
 * textures do not have multiple image levels; only a single data store is
 * available.
 *
 * @ingroup render
 */
class ORK_API TextureBuffer : public Texture
{
public:
    /**
     * Creates a new buffer texture.
     *
     * @param tf texture data format in 'pixels'.
     * @param pixels the buffer holding the texture's texel array.
     */
    TextureBuffer(TextureInternalFormat tf, ptr<GPUBuffer> pixels);

    /**
     * Destroys this buffer texture.
     */
    virtual ~TextureBuffer();

    /**
     * Returns the width of this texture.
     */
    int getWidth();

    /**
     * Returns the buffer holding the texture's texel array.
     */
    ptr<GPUBuffer> getBuffer();

protected:
    /**
     * The width of this texture.
     */
    int w;

    /**
     * The buffer holding the texture's texel array.
     */
    ptr<GPUBuffer> b;
};

}

#endif
