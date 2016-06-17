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

#include "ork/render/Texture2DMultisampleArray.h"

#include <exception>

#include <GL/glew.h>

#include "ork/render/FrameBuffer.h"

namespace ork
{

GLenum getTextureInternalFormat(TextureInternalFormat f);

Texture2DMultisampleArray::Texture2DMultisampleArray(int w, int h, int l, int samples, TextureInternalFormat tf, bool fixedLocations) :
    Texture("Texture2DMultisampleArray", GL_TEXTURE_2D_MULTISAMPLE_ARRAY)
{
    glGenTextures(1, &textureId);
    assert(textureId > 0);

    this->internalFormat = tf;
    this->w = w;
    this->h = h;
    this->l = l;
    this->samples = samples;

    bindToTextureUnit();

    glTexImage3DMultisample(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, samples, getTextureInternalFormat(tf), w, h, l, fixedLocations);

    if (FrameBuffer::getError() != 0) {
        throw exception();
    }
}

Texture2DMultisampleArray::~Texture2DMultisampleArray()
{
}

int Texture2DMultisampleArray::getWidth()
{
    return w;
}

int Texture2DMultisampleArray::getHeight()
{
    return h;
}

int Texture2DMultisampleArray::getLayers()
{
    return l;
}

int Texture2DMultisampleArray::getSamples()
{
    return samples;
}

}
