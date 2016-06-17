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

#ifndef _ORK_TEXTURE_CUBE_H_
#define _ORK_TEXTURE_CUBE_H_

#include "ork/render/Buffer.h"
#include "ork/render/Texture.h"

namespace ork
{

/**
 * A cube texture.
 * @ingroup render
 */
class ORK_API TextureCube : public Texture
{
public:
    /**
     * Creates a new cube texture.
     *
     * @param w the width of this texture in pixels.
     * @param h the height of this texture in pixels.
     * @param tf texture data format on GPU.
     * @param f the texture components in 'pixels'.
     * @param t the type of each component in 'pixels'.
     * @param params optional additional texture parameters.
     * @param s optional pixel storage parameters for 'pixels'.
     * @param pixels the pixels to be written into this texture. The cube
     *      faces must be specified in the following order: POSITIVE_X,
     *      NEGATIVE_X, POSITIVE_Y, NEGATIVE_Y, POSITIVE_Z, NEGATIVE_Z.
     */
    TextureCube(int w, int h, TextureInternalFormat tf, TextureFormat f, PixelType t,
        const Parameters &params, Buffer::Parameters s[6], ptr<Buffer> pixels[6]);

    /**
     * Destroys this cube texture.
     */
    virtual ~TextureCube();

    /**
     * Returns the width of this texture.
     */
    int getWidth();

    /**
     * Returns the height of this texture.
     */
    int getHeight();

    /**
     * Replaces a part of one face of this cube texture.
     *
     * @param cf the cube face to be set.
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
    void setSubImage(CubeFace cf, int level, int x, int y, int w, int h, TextureFormat f, PixelType t, const Buffer::Parameters &s, const Buffer &pixels);

    /**
     * Replaces a part of one face of this cube texture.
     *
     * @param cf the cube face to be set.
     * @param level the LOD level to be changed.
     * @param x lower left corner of the part to be replaced, in pixels.
     * @param y lower left corner of the part to be replaced, in pixels.
     * @param w the width of the part to be replaced, in pixels.
     * @param h the height of the part to be replaced, in pixels.
     * @param s the size of 'pixels' in bytes.
     * @param pixels the pixels to be written into this texture LOD level.
     */
    void setCompressedSubImage(CubeFace cf, int level, int x, int y, int w, int h, int s, const Buffer &pixels);

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
     * Creates a new unitialized Cube texture.
     */
    TextureCube();

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
     * @param pixels the pixels to be written into this texture. The cube
     *      faces must be specified in the following order: POSITIVE_X,
     *      NEGATIVE_X, POSITIVE_Y, NEGATIVE_Y, POSITIVE_Z, NEGATIVE_Z.
     */
    void init(int w, int h, TextureInternalFormat tf, TextureFormat f, PixelType t,
        const Parameters &params, Buffer::Parameters s[6], ptr<Buffer> pixels[6]);

    virtual void swap(ptr<Texture> t);
};

}

#endif
