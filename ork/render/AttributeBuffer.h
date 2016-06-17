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
