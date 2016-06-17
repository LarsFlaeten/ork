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
