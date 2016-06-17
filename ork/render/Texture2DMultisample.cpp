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

#include "ork/render/Texture2DMultisample.h"

#include <exception>

#include <GL/glew.h>

#include "ork/render/FrameBuffer.h"

namespace ork
{

GLenum getTextureInternalFormat(TextureInternalFormat f);

Texture2DMultisample::Texture2DMultisample(int w, int h, int samples, TextureInternalFormat tf, bool fixedLocations) :
    Texture("Texture2DMultisample", GL_TEXTURE_2D_MULTISAMPLE)
{
    glGenTextures(1, &textureId);
    assert(textureId > 0);

    this->internalFormat = tf;
    this->w = w;
    this->h = h;
    this->samples = samples;

    bindToTextureUnit();

    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, getTextureInternalFormat(tf), w, h, fixedLocations);

    if (FrameBuffer::getError() != 0) {
        throw exception();
    }
}

Texture2DMultisample::~Texture2DMultisample()
{
}

int Texture2DMultisample::getWidth()
{
    return w;
}

int Texture2DMultisample::getHeight()
{
    return h;
}

int Texture2DMultisample::getSamples()
{
    return samples;
}

}
