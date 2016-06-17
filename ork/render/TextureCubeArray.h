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

#ifndef _ORK_TEXTURE_CUBE_ARRAY_H_
#define _ORK_TEXTURE_CUBE_ARRAY_H_

#include "ork/render/Buffer.h"
#include "ork/render/Texture.h"

namespace ork
{

/**
 * A cube array texture.
 * @ingroup render
 */
class ORK_API TextureCubeArray : public Texture
{
public:
    /**
     * Creates a new cube texture.
     *
     * @param w the width of this texture in pixels.
     * @param h the height of this texture in pixels.
     * @param l the number of layers of this texture.
     * @param tf texture data format on GPU.
     * @param f the texture components in 'pixels'.
     * @param t the type of each component in 'pixels'.
     * @param params optional additional texture parameters.
     * @param s optional pixel storage parameters for 'pixels'.
     * @param pixels the pixels to be written into this texture. The pixels
     *      must be specified as in a 3D texture, with one face per layer.
     *      The faces must be specified in the following order: POSITIVE_X,
     *      NEGATIVE_X, POSITIVE_Y, NEGATIVE_Y, POSITIVE_Z, and NEGATIVE_Z of
     *      the first cube map layer, then the six faces of the second layer,
     *      in the same order, and so on for all layers.
     */
    TextureCubeArray(int w, int h, int l, TextureInternalFormat tf, TextureFormat f, PixelType t,
        const Parameters &params, Buffer::Parameters s, const Buffer &pixels);

    /**
     * Destroys this cube texture.
     */
    virtual ~TextureCubeArray();

    /**
     * Returns the width of this texture.
     */
    int getWidth();

    /**
     * Returns the height of this texture.
     */
    int getHeight();

    /**
     * Returns the number of layers of this texture.
     */
    int getLayers();

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
     * The number of layers of this texture.
     */
    int l;

    /**
     * Creates a new unitialized Cube texture.
     */
    TextureCubeArray();

    /**
     * Initializes this texture.
     *
     * @param w the width of this texture in pixels.
     * @param h the height of this texture in pixels.
     * @param l the number of layers of this texture.
     * @param tf texture data format on GPU.
     * @param f the texture components in 'pixels'.
     * @param t the type of each component in 'pixels'.
     * @param params optional additional texture parameters.
     * @param s optional pixel storage parameters for 'pixels'.
     * @param pixels the pixels to be written into this texture. The pixels
     *      must be specified as in a 3D texture, with one face per layer.
     *      The faces must be specified in the following order: POSITIVE_X,
     *      NEGATIVE_X, POSITIVE_Y, NEGATIVE_Y, POSITIVE_Z, and NEGATIVE_Z of
     *      the first cube map layer, then the six faces of the second layer,
     *      in the same order, and so on for all layers.
     */
    void init(int w, int h, int l, TextureInternalFormat tf, TextureFormat f, PixelType t,
        const Parameters &params, Buffer::Parameters s, const Buffer &pixels);

    virtual void swap(ptr<Texture> t);
};

}

#endif
