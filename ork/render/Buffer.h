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

#ifndef _ORK_BUFFER_H_
#define _ORK_BUFFER_H_

#include "ork/math/vec2.h"
#include "ork/math/vec3.h"
#include "ork/core/Object.h"
#include "ork/render/Types.h"

namespace ork
{

/**
 * An abstract data buffer.
 * @ingroup render
 */
class ORK_API Buffer : public Object
{
public:
    /**
     * A Buffer layout in client memory for transferring pixels to or from GPU.
     */
    class ORK_API Parameters
    {
    public:
        /**
         * Creates a new buffer layout with default parameter values.
         */
        Parameters();

        /**
         * Returns true if little endian mode is used. Default is false.
         */
        bool swapBytes() const;

        /**
         * Returns true if least significant bit is first. Default is false.
         */
        bool leastSignificantBitFirst() const;

        /**
         * Returns the alignment of data in memory. Default is 4 bytes.
         */
        GLint alignment() const;

        /**
         * Returns the compressed size in bytes of the pixels.
         * Default is 0, meaning that the buffer contains uncompressed data.
         */
        GLsizei compressedSize() const;

        /**
         * Returns the 2D subpart of the buffer that must be used for
         * transferring pixels. Default is whole buffer.
         *
         * @return a vector containing the x and y origin of the subpart to
         *      transfer, and the total width of the image.
         */
        vec3<GLint> subImage2D() const;

        /**
         * Returns the 3D subpart of the buffer that must be used for
         * transferring pixels. Default is whole buffer.
         *
         * @return a vector containing the z origin of the subpart to
         *      transfer, and the total height of the image.
         */
        vec2<GLint> subImage3D() const;

        /**
         * Sets the endianness mode to use.
         *
         * @param swapBytes true to use little endian mode.
         */
        Parameters& swapBytes(bool swapBytes);

        /**
         * Sets the bit ordering mode to use.
         *
         * @param leastSignificantBitFirst true if least significant bit is first.
         */
        Parameters& leastSignificantBitFirst(bool leastSignificantBitFirst);

        /**
         * Sets the alignment of data in memory.
         *
         * @param alignment the alignment of data in memory.
         */
        Parameters& alignment(GLint alignment);

        /**
         * Returns the size in bytes of the compressed pixels.
         *
         * @param compressedSize the size in bytes of the compressed pixels,
         *      or 0 if the buffer data is not compressed.
         */
        Parameters& compressedSize(GLsizei compressedSize);

        /**
         * Sets the 2D subpart of the buffer that must be used for
         * transferring pixels.
         *
         * @param skipPixels the x origin of the subpart to transfer.
         * @param skipRows the y origin of the subpart to transfer.
         * @param rowLength the total width of the image.
         */
        Parameters& subImage2D(GLint skipPixels, GLint skipRows, GLint rowLength);

        /**
         * Sets the 3D subpart of the buffer that must be used for
         * transferring pixels.
         *
         * @param skipImages the z origin of the subpart to transfer.
         * @param imageHeight the total height of the image.
         */
        Parameters& subImage3D(GLint skipImages, GLint imageHeight);

    private:
        bool _swapBytes;

        bool _leastSignificantBitFirst;

        GLint _alignment;

        GLsizei _compressedSize;

        vec3<GLint> _subImage2D;

        vec2<GLint> _subImage3D;

        /**
         * True if the parameters are not equal to their default values.
         */
        bool modified;

        /**
         * Sets the OpenGL state corresponding to these parameters.
         */
        void set() const;

        /**
         * Sets the default OpenGL state corresponding to these parameters.
         */
        void unset() const;

        friend class Texture1D;

        friend class Texture1DArray;

        friend class Texture2D;

        friend class Texture2DArray;

        friend class Texture2DMultisample;

        friend class Texture2DMultisampleArray;

        friend class Texture3D;

        friend class TextureCube;

        friend class TextureCubeArray;

        friend class TextureRectangle;

        friend class FrameBuffer;
    };

    /**
     * Creates a new buffer.
     */
    Buffer();

    /**
     * Destroys this buffer.
     */
    virtual ~Buffer();

private:
    /**
     * Binds this buffer to the given target.
     *
     * @param target an OpenGL buffer target (GL_ARRAY_BUFFER, etc).
     */
    virtual void bind(int target) const = 0;

    /**
     * Returns a pointer to the given offset in this data buffer.
     *
     * @param offset an offset from the start of this buffer, in bytes.
     */
    virtual void *data(int offset) const = 0;

    /**
     * Unbinds this buffer from the given target.
     *
     * @param target an OpenGL buffer target (GL_ARRAY_BUFFER, etc).
     */
    virtual void unbind(int target) const = 0;

    /**
     * Notifies this buffer that it received new data on the GL side
     * (via readPixels, transformFeedback, etc). This is only possible
     * for a GPU buffer.
     */
    virtual void dirty() const = 0;

    friend class MeshBuffers;

    friend class FrameBuffer;

    friend class Texture1D;

    friend class Texture1DArray;

    friend class Texture2D;

    friend class Texture2DArray;

    friend class Texture2DMultisample;

    friend class Texture2DMultisampleArray;

    friend class Texture3D;

    friend class TextureCube;

    friend class TextureCubeArray;

    friend class TextureRectangle;

    friend class TransformFeedback;
};

}

#endif
