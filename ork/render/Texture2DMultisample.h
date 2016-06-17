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

#ifndef _ORK_TEXTURE_2D_MULTISAMPLE_H_
#define _ORK_TEXTURE_2D_MULTISAMPLE_H_

#include "ork/render/Buffer.h"
#include "ork/render/Texture.h"

namespace ork
{

/**
 * A 2D texture with multiple samples per pixel.
 * @ingroup render
 */
class ORK_API Texture2DMultisample : public Texture
{
public:
    /**
     * Creates a new 2D texture with multiple samples per pixel.
     *
     * @param w the width of this texture in pixels.
     * @param h the height of this texture in pixels.
     * @param samples the number of samples per pixel.
     * @param tf texture data format on GPU.
     * @param fixedLocations true to use fixed sample locations for all pixels.
     */
    Texture2DMultisample(int w, int h, int samples, TextureInternalFormat tf, bool fixedLocations);

    /**
     * Destroys this 2D texture with multiple samples per pixel.
     */
    virtual ~Texture2DMultisample();

    /**
     * Returns the width of this texture.
     */
    int getWidth();

    /**
     * Returns the height of this texture.
     */
    int getHeight();

    /**
     * Returns the number of samples per pixel of this texture.
     */
    int getSamples();

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
     * The number of samples per pixel of this texture.
     */
    int samples;
};

}

#endif
