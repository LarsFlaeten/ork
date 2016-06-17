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

#ifndef _ORK_RENDER_BUFFER_H_
#define _ORK_RENDER_BUFFER_H_

#include "ork/render/Buffer.h"

namespace ork
{

class FrameBuffer;

/**
 * A buffer for offscreen rendering.
 * @ingroup render
 */
class ORK_API RenderBuffer : public Object
{
public:
    /**
     * A pixel format for a RenderBuffer.
     */
    enum RenderBufferFormat {
        R8, ///< &nbsp;
        R8_SNORM, ///< &nbsp;
        R16, ///< &nbsp;
        R16_SNORM, ///< &nbsp;
        RG8, ///< &nbsp;
        RG8_SNORM, ///< &nbsp;
        RG16, ///< &nbsp;
        RG16_SNORM, ///< &nbsp;
        R3_G3_B2, ///< &nbsp;
        RGB4, ///< &nbsp;
        RGB5, ///< &nbsp;
        RGB8, ///< &nbsp;
        RGB8_SNORM, ///< &nbsp;
        RGB10, ///< &nbsp;
        RGB12, ///< &nbsp;
        RGB16, ///< &nbsp;
        RGB16_SNORM, ///< &nbsp;
        RGBA2, ///< &nbsp;
        RGBA4, ///< &nbsp;
        RGB5_A1, ///< &nbsp;
        RGBA8, ///< &nbsp;
        RGBA8_SNORM, ///< &nbsp;
        RGB10_A2, ///< &nbsp;
        RGB10_A2UI, ///< &nbsp;
        RGBA12, ///< &nbsp;
        RGBA16, ///< &nbsp;
        RGBA16_SNORM, ///< &nbsp;
        SRGB8, ///< &nbsp;
        SRGB8_ALPHA8, ///< &nbsp;
        R16F, ///< &nbsp;
        RG16F, ///< &nbsp;
        RGB16F, ///< &nbsp;
        RGBA16F, ///< &nbsp;
        R32F, ///< &nbsp;
        RG32F, ///< &nbsp;
        RGB32F, ///< &nbsp;
        RGBA32F, ///< &nbsp;
        R11F_G11F_B10F, ///< &nbsp;
        RGB9_E5, ///< &nbsp;
        R8I, ///< &nbsp;
        R8UI, ///< &nbsp;
        R16I, ///< &nbsp;
        R16UI, ///< &nbsp;
        R32I, ///< &nbsp;
        R32UI, ///< &nbsp;
        RG8I, ///< &nbsp;
        RG8UI, ///< &nbsp;
        RG16I, ///< &nbsp;
        RG16UI, ///< &nbsp;
        RG32I, ///< &nbsp;
        RG32UI, ///< &nbsp;
        RGB8I, ///< &nbsp;
        RGB8UI, ///< &nbsp;
        RGB16I, ///< &nbsp;
        RGB16UI, ///< &nbsp;
        RGB32I, ///< &nbsp;
        RGB32UI, ///< &nbsp;
        RGBA8I, ///< &nbsp;
        RGBA8UI, ///< &nbsp;
        RGBA16I, ///< &nbsp;
        RGBA16UI, ///< &nbsp;
        RGBA32I, ///< &nbsp;
        RGBA32UI, ///< &nbsp;
        DEPTH_COMPONENT, ///< &nbsp;
        DEPTH_COMPONENT16, ///< &nbsp;
        DEPTH_COMPONENT24, ///< &nbsp;
        DEPTH_COMPONENT32, ///< &nbsp;
        DEPTH_COMPONENT32F, ///< &nbsp;
        DEPTH24_STENCIL8, ///< &nbsp;
        DEPTH32F_STENCIL8, ///< &nbsp;
        STENCIL_INDEX, ///< &nbsp;
        STENCIL_INDEX1, ///< &nbsp;
        STENCIL_INDEX4, ///< &nbsp;
        STENCIL_INDEX8, ///< &nbsp;
        STENCIL_INDEX16 ///< &nbsp;
    };

    /**
     * Creates a new offscreen render buffer.
     *
     * @param type the pixel format for this buffer.
     * @param width the width of the buffer.
     * @param height the height of the buffer.
     * @param samples the number of samples per pixel,
     *      or 0 if multisampling is not needed.
     */
    RenderBuffer(RenderBufferFormat type, int width, int height, int samples = 0);

    /**
     * Deletes this render buffer.
     */
    virtual ~RenderBuffer();

    /**
     * Returns the id of this render buffer.
     */
    GLuint getId() const;

private:
    /**
     * The id of this render buffer.
     */
    GLuint bufferId;

    friend class FrameBuffer;
};

}

#endif
