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

#ifndef _ORK_ATTRIBUTE_BUFFER_H_
#define _ORK_ATTRIBUTE_BUFFER_H_

#include "ork/render/Types.h"
#include "ork/render/Buffer.h"

namespace ork
{

class MeshBuffers;

class FrameBuffer;

/**
 * A vertex attribute buffer. Such a buffer contains the values of one
 * vertex attribute for a list of vertices. A vertex attribute is a vector
 * of one or more components of the same type. Examples of vertex attributes
 * are its position, normal, uv coordinates, color, etc. So, for example, a
 * position attribute buffer contains the positions of a list of vertices, a
 * color attribute buffer contains the colors of a list of vertices, etc.
 *
 * An AttributeBuffer describes how these values are organized, based on an
 * offset and a stride parameters. The values themselves are stored in a
 * Buffer object. Note that several AttributeBuffer can share the same Buffer
 * object. So several vertex attributes can be stored in a single buffer. For
 * instance all positions, followed by all normals, followed by all colors,
 * etc. Or the position, normal and color of the first vertex, followed by the
 * position, normal and color of the second vertex, and so on.
 *
 * @ingroup render
 */
class ORK_API AttributeBuffer : public Object
{
public:
    /**
     * Creates a new attribute buffer for floating point attributes.
     * The attribute is supposed to be declared as floating point in
     * the vertex shader, but its value can be defined from integer
     * values (which can then be normalized to 0..1 or not when converted
     * to floating point).
     *
     * @param index a vertex attribute index.
     * @param size the number of components in attributes of this kind.
     * @param t the type of each component in attributes of this kind.
     * @param norm if the attribute components must be normalized to 0..1.
     * @param b the buffer containing the actual attribute values.
     * @param stride the offset between two consecutive attribute values in b.
     * @param offset the offset of the first attribute value in b.
     * @param divisor how many times each attribute much be instanced, or 0
     *      to disable attribute instancing.
     */
    AttributeBuffer(int index, int size, AttributeType t, bool norm, ptr<Buffer> b, int stride = 0, int offset = 0, int divisor = 0);

    /**
     * Creates a new attribute buffer for signed or unsigned integer attributes.
     * The attribute is supposed to be declared as signed or unsigned integer
     * type in the vertex shader, and its value must be defined from integer
     * values.
     *
     * @param index a vertex attribute index.
     * @param size the number of components in attributes of this kind.
     * @param t the type of each component in attributes of this kind. Must be
     *      a signed or unsigned integer type.
     * @param b the buffer containing the actual attribute values.
     * @param stride the offset between two consecutive attribute values in b.
     * @param offset the offset of the first attribute value in b.
     * @param divisor how many times each attribute much be instanced, or 0
     *      to disable attribute instancing.
     */
    AttributeBuffer(int index, int size, AttributeType t, ptr<Buffer> b, int stride = 0, int offset = 0, int divisor = 0);

    /**
     * Creates a new attribute buffer for double precision attributes.
     * The attribute is supposed to be declared as double precision floating point
     * type in the vertex shader, and its value must be defined from double
     * values.
     *
     * @param index a vertex attribute index.
     * @param size the number of components in attributes of this kind.
     * @param b the buffer containing the actual attribute values.
     * @param stride the offset between two consecutive attribute values in b.
     * @param offset the offset of the first attribute value in b.
     * @param divisor how many times each attribute much be instanced, or 0
     *      to disable attribute instancing.
     */
    AttributeBuffer(int index, int size, ptr<Buffer> b, int stride = 0, int offset = 0, int divisor = 0);

    /**
     * Destroys this attribute buffer.
     */
    virtual ~AttributeBuffer();

    /**
     * Returns the number of components in attributes of this kind.
     */
    int getSize();

    /**
     * Returns the type of each component in attributes of this kind.
     */
    AttributeType getType();

    /**
     * Returns the size of one attribute value. This size is the number of
     * components per attribute, times the size of each component (which
     * depends on its type: byte, int, float, etc). Except for the packed
     * formats A32I_2_10_10_10_REV and A32UI_2_10_10_10_REV.
     */
    int getAttributeSize();

    /**
     * Returns the buffer that contains the actual data of this attribute
     * buffer.
     */
    ptr<Buffer> getBuffer();

    /**
     * Sets the buffer that contains the actual data of this attribute
     * buffer.
     */
    void setBuffer(ptr<Buffer> b);

    /**
     * Returns the offset between two consecutive attribute values in this
     * attribute buffer.
     */
    int getStride();

    /**
     * Returns the offset of the first attribute value in this attribute
     * buffer.
     */
    int getOffset();

    /**
     * Returns the number of times each attribute much be instanced, or 0
     * if attribute instancing is disabled for this attribute.
     */
    int getDivisor();

private:
    /**
     * A vertex attribute index.
     */
    int index;

    /**
     * The number of components in attributes of this kind.
     */
    int size;

    /**
     * The type of each component in attributes of this kind.
     */
    AttributeType type;

    /**
     * True if the attribute is declared with an integer type in shaders.
     */
    bool I;

    /**
     * True if the attribute is declared with a double floating point precision type in shaders.
     */
    bool L;

    /**
     * True if the attribute components must be normalized to 0..1.
     */
    bool norm;

    /**
     * The buffer that contains the actual vertex attribute values.
     */
    ptr<Buffer> b;

    /**
     * The offset between two consecutive attribute values in #b.
     */
    int stride;

    /**
     * The offset of the first attribute value in #b.
     */
    int offset;

    /**
     * How many times each attribute much be instanced, or 0 to disable
     * attribute instancing.
     */
    int divisor;

    friend class MeshBuffers;

    friend class FrameBuffer;
};

}

#endif
