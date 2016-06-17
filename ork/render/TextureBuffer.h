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
