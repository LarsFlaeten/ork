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

#ifndef _ORK_TEXTURE_2D_H_
#define _ORK_TEXTURE_2D_H_

#include "ork/render/Buffer.h"
#include "ork/render/Texture.h"

namespace ork
{

/**
 * A 2D texture.
 * @ingroup render
 */
class ORK_API Texture2D : public Texture
{
public:
    /**
     * Creates a new 2D texture.
     *
     * @param w the width of this texture in pixels.
     * @param h the height of this texture in pixels.
     * @param tf texture data format on GPU.
     * @param f the texture components in 'pixels'.
     * @param t the type of each component in 'pixels'.
     * @param params optional additional texture parameters.
     * @param s optional pixel storage parameters for 'pixels'.
     * @param pixels the pixels to be written into this texture.
     */
    Texture2D(int w, int h, TextureInternalFormat tf, TextureFormat f, PixelType t,
        const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels);

    /**
     * Destroys this 2D texture.
     */
    virtual ~Texture2D();

    /**
     * Returns the width of this texture.
     */
    int getWidth();

    /**
     * Returns the height of this texture.
     */
    int getHeight();

    /**
     * Replaces the content of this texture.
     *
     * @param w the width of the new texture content, in pixels.
     * @param h the height of the new texture content, in pixels.
     * @param f the texture components in 'pixels'.
     * @param t the type of each component in 'pixels'.
     * @param pixels the pixels to be written into this texture.
     */
    void setImage(int w, int h, TextureFormat f, PixelType t, const Buffer &pixels);

    /**
     * Replaces a part of the content of this texture.
     *
     * @param level the LOD level to be changed.
     * @param x lower left corner of the part to be replaced, in pixels.
     * @param y lower left corner of the part to be replaced, in pixels.
     * @param w the width of the part to be replaced, in pixels.
     * @param h the height of the part to be replaced, in pixels.
     * @param f the texture components in 'pixels'.
     * @param t the type of each component in 'pixels'.
     * @param s optional pixel storage parameters for 'pixels'.
     * @param pixels the pixels to be written into this texture LOD level.
     */
    void setSubImage(int level, int x, int y, int w, int h, TextureFormat f, PixelType t, const Buffer::Parameters &s, const Buffer &pixels);

    /**
     * Replaces a part of the content of this texture.
     *
     * @param level the LOD level to be changed.
     * @param x lower left corner of the part to be replaced, in pixels.
     * @param y lower left corner of the part to be replaced, in pixels.
     * @param w the width of the part to be replaced, in pixels.
     * @param h the height of the part to be replaced, in pixels.
     * @param s the size of 'pixels' in bytes.
     * @param pixels the pixels to be written into this texture LOD level.
     */
    void setCompressedSubImage(int level, int x, int y, int w, int h, int s, const Buffer &pixels);

protected:
    /**
     * The width of this texture.
     */
    int w;

    /**
     * The height of this texture.
     */
    int h;

    /**
     * Creates a new unitialized 2D texture.
     */
    Texture2D();

    /**
     * Creates a new unintialized 2D texture.
     *
     * @param target a texture type (2D, 2DArray, etc).
     */
    Texture2D(int target);

    /**
     * Initializes this texture.
     *
     * @param w the width of this texture in pixels.
     * @param h the height of this texture in pixels.
     * @param tf texture data format on GPU.
     * @param f the texture components in 'pixels'.
     * @param t the type of each component in 'pixels'.
     * @param params optional additional texture parameters.
     * @param s optional pixel storage parameters for 'pixels'.
     * @param pixels the pixels to be written into this texture.
     */
    void init(int w, int h, TextureInternalFormat tf, TextureFormat f, PixelType t,
        const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels);

    virtual void swap(ptr<Texture> t);
};

}

#endif
