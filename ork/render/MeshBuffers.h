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

#ifndef _ORK_MESH_BUFFERS_H_
#define _ORK_MESH_BUFFERS_H_

#include <vector>

#include "ork/math/box3.h"
#include "ork/math/vec4.h"
#include "ork/render/AttributeBuffer.h"

namespace ork
{

class FrameBuffer;

/**
 * A set of AttributeBuffer that represent the vertices and indices of a mesh.
 * Each attribute buffer represents an attribute (position, normal, color, etc)
 * of the vertices of the mesh. A mesh can have an indices array that allows
 * vertices shared between adjacent primitives to be shared in memory. If there
 * is no indices array shared vertices must be duplicated in the vertices array.
 *
 * @ingroup render
 */
class ORK_API MeshBuffers : public Object
{
public:
    /**
     * How the list of vertices of this mesh must be interpreted.
     */
    MeshMode mode;

    /**
     * The number of vertices in this mesh.
     */
    int nvertices;

    /**
     * The number of indices in this mesh.
     */
    int nindices;

    /**
     * The bounding box of this mesh.
     */
    box3f bounds;

    /**
     * The vertex index used for primitive restart. -1 means no restart.
     */
    GLint primitiveRestart;

    /**
     * The number of vertices per patch of this mesh, if mode is PATCHES.
     */
    GLint patchVertices;

    /**
     * Creates a new mesh without any AttributeBuffer.
     */
    MeshBuffers();

    /**
     * Deletes this mesh.
     */
    virtual ~MeshBuffers();

    /**
     * Returns the number of AttributeBuffer in this mesh. This is the
     * number of attributes per vertex.
     */
    int getAttributeCount() const;

    /**
     * Returns the AttributeBuffer whose index is given.
     *
     * @param index an index between 0 and #getAttributeCount (exclusive).
     */
    ptr<AttributeBuffer> getAttributeBuffer(int index) const;

    /**
     * Returns the indices buffer of this mesh.
     */
    ptr<AttributeBuffer> getIndiceBuffer() const;

    /**
     * Adds a vertex attribute buffer to this mesh. This method assumes that
     * this vertex attribute is stored in its own buffer.
     *
     * @param index a vertex attribute index.
     * @param size the number of components in attributes of this kind.
     * @param type the type of each component in attributes of this kind.
     * @param norm if the attribute components must be normalized to 0..1.
     */
    void addAttributeBuffer(int index, int size, AttributeType type, bool norm);

    /**
     * Adds a vertex attribute buffer to this mesh. This method assumes that
     * this vertex attribute is stored interleaved with others in a shared buffer.
     * For instance, for a mesh with a position, normal and color attributes,
     * the data layout is position, normal and color of first vertex, position,
     * normal and color of second vertex, and so on for other vertices.
     *
     * @param index a vertex attribute index.
     * @param size the number of components in attributes of this kind.
     * @param vertexsize the total size of all the vertex attributes.
     * @param type the type of each component in attributes of this kind.
     * @param norm if the attribute components must be normalized to 0..1.
     */
    void addAttributeBuffer(int index, int size, int vertexsize, AttributeType type, bool norm);

     /**
     * Adds a vertex attribute buffer to this mesh. This method does not rely
     * on any storage assumption.
     *
     * @param buffer a vertex attribute buffer.
     */
    void addAttributeBuffer(ptr<AttributeBuffer> buffer);

    /**
     * Sets the indices array buffer of this mesh.
     */
    void setIndicesBuffer(ptr<AttributeBuffer> indices);

    /**
     * Resets the internal %state associated with this mesh. For internal use only.
     */
    void reset() const;

    /**
     * Sets the default value for the given attribute when a MeshBuffers does
     * not specify any Buffer for this attribute.
     *
     * @param index a vertex attribute index. This attribute must be declared with
     *      a floating point type in the program.
     * @param defaultValue the default value to use for this attribute.
     */
    template<class T>
    static void setDefaultAttribute(GLuint index, T defaultValue);

    /**
     * Sets the default value for the given attribute when a MeshBuffers does
     * not specify any Buffer for this attribute.
     *
     * @param index a vertex attribute index. This attribute must be declared with
     *      a floating point type in the program.
     * @param count the number of elements in the 'defaultValue' array.
     * @param defaultValue the default value to use for this attribute.
     * @param normalize true to normalize the components of 'defaultValue'.
     */
    template<class T>
    static void setDefaultAttribute(GLuint index, int count, T *defaultValue, bool normalize = false);

    /**
     * Sets the default value for the given attribute when a MeshBuffers does
     * not specify any Buffer for this attribute.
     *
     * @param index a vertex attribute index. This attribute must be declared with
     *      an integer or unsigned integer type in the program.
     * @param defaultValue the default value to use for this attribute.
     */
    template<class T>
    static void setDefaultAttributeI(GLuint index, T defaultValue);

    /**
     * Sets the default value for the given attribute when a MeshBuffers does
     * not specify any Buffer for this attribute.
     *
     * @param index a vertex attribute index. This attribute must be declared with
     *      an integer or unsigned integer type in the program.
     * @param count the number of elements in the 'defaultValue' array.
     * @param defaultValue the default value to use for this attribute.
     */
    template<class T>
    static void setDefaultAttributeI(GLuint index, int count, T *defaultValue);

    /**
     * Sets the default value for the given attribute when a MeshBuffers does
     * not specify any Buffer for this attribute.
     *
     * @param index a vertex attribute index. This attribute must be declared with
     *      a double type in the program.
     * @param defaultValue the default value to use for this attribute.
     */
    template<class T>
    static void setDefaultAttributeL(GLuint index, T defaultValue);

    /**
     * Sets the default value for the given attribute when a MeshBuffers does
     * not specify any Buffer for this attribute.
     *
     * @param index a vertex attribute index. This attribute must be declared with
     *      a double point type in the program.
     * @param count the number of elements in the 'defaultValue' array.
     * @param defaultValue the default value to use for this attribute.
     */
    template<class T>
    static void setDefaultAttributeL(GLuint index, int count, T *defaultValue);

    /**
     * Sets the default value for the given attribute when a MeshBuffers does
     * not specify any Buffer for this attribute. This method only sets the first
     * component of the specified attribute.
     *
     * @param index a vertex attribute index. This attribute must be declared with
     *      a floating point type in the program.
     * @param defaultValue the default value to use for this attribute,
     *      in packed format.
     * @param isSigned true to use the signed packed format, false to use the
     *      unsigned packed format.
     * @param normalize true to normalize the components of 'defaultValue'.
     */
    static void setDefaultAttributeP1(GLuint index, GLuint defaultValue, bool isSigned, bool normalize = false);

    /**
     * Sets the default value for the given attribute when a MeshBuffers does
     * not specify any Buffer for this attribute. This method only sets the first
     * two components of the specified attribute.
     *
     * @param index a vertex attribute index. This attribute must be declared with
     *      a floating point type in the program.
     * @param defaultValue the default value to use for this attribute,
     *      in packed format.
     * @param isSigned true to use the signed packed format, false to use the
     *      unsigned packed format.
     * @param normalize true to normalize the components of 'defaultValue'.
     */
    static void setDefaultAttributeP2(GLuint index, GLuint defaultValue, bool isSigned, bool normalize = false);

    /**
     * Sets the default value for the given attribute when a MeshBuffers does
     * not specify any Buffer for this attribute. This method only sets the first
     * three components of the specified attribute.
     *
     * @param index a vertex attribute index. This attribute must be declared with
     *      a floating point type in the program.
     * @param defaultValue the default value to use for this attribute,
     *      in packed format.
     * @param isSigned true to use the signed packed format, false to use the
     *      unsigned packed format.
     * @param normalize true to normalize the components of 'defaultValue'.
     */
    static void setDefaultAttributeP3(GLuint index, GLuint defaultValue, bool isSigned, bool normalize = false);

    /**
     * Sets the default value for the given attribute when a MeshBuffers does
     * not specify any Buffer for this attribute. This method sets the four
     * components of the specified attribute.
     *
     * @param index a vertex attribute index. This attribute must be declared with
     *      a floating point type in the program.
     * @param defaultValue the default value to use for this attribute,
     *      in packed format.
     * @param isSigned true to use the signed packed format, false to use the
     *      unsigned packed format.
     * @param normalize true to normalize the components of 'defaultValue'.
     */
    static void setDefaultAttributeP4(GLuint index, GLuint defaultValue, bool isSigned, bool normalize = false);

    /**
     * Sets the default value for the given attribute when a MeshBuffers does
     * not specify any Buffer for this attribute. This method only sets the first
     * 'count' components of the specified attribute.
     *
     * @param index a vertex attribute index. This attribute must be declared with
     *      a floating point type in the program.
     * @param count the number of components of the attribute to set.
     * @param defaultValue the default value to use for this attribute,
     *      in packed format.
     * @param isSigned true to use the signed packed format, false to use the
     *      unsigned packed format.
     * @param normalize true to normalize the components of 'defaultValue'.
     */
    static void setDefaultAttributeP(GLuint index, int count, GLuint *defaultValue, bool isSigned, bool normalize = false);

protected:
    /**
     * Swaps this mesh with the given one.
     */
    virtual void swap(ptr<MeshBuffers> buffers);

    /**
     * Draws a part of this mesh one or more times.
     *
     * @param m how the mesh vertices must be interpreted.
     * @param first the first vertex to draw, or the first indice to draw if
     *      this mesh has indices.
     * @param count the number of vertices to draw, or the number of indices
     *      to draw if this mesh has indices.
     * @param primCount the number of times this mesh must be drawn (with
     *      geometry instancing).
     * @param base the base vertex to use. Only used for meshes with indices.
     */
    void draw(MeshMode m, GLint first, GLsizei count, GLsizei primCount = 1, GLint base = 0) const;

    /**
     * Draws several parts of this mesh. Each part is specified with a first
     * and count parameter as in #draw(). These values are passed in arrays
     * of primCount values.
     *
     * @param m how the mesh vertices must be interpreted.
     * @param firsts an array of primCount 'first vertex' to draw, or an array
     *      of 'first indice' to draw if this mesh has indices.
     * @param counts an array of number of vertices to draw, or an array of
     *      number of indices to draw if this mesh has indices.
     * @param primCount the number of parts of this mesh to draw.
     * @param bases the base vertices to use. Only used for meshes with indices.
     */
    void multiDraw(MeshMode m, GLint *firsts, GLsizei *counts, GLsizei primCount, GLint* bases = 0) const;

    /**
     * Draws a part of this mesh one or more times.
     *
     * @param m how the mesh vertices must be interpreted.
     * @param buf a CPU or GPU buffer containing the 'count', 'primCount',
     *      'first' and 'base' parameters, in this order, as 32 bit integers.
     */
    void drawIndirect(MeshMode m, const Buffer &buf) const;

    /**
     * Draws this mesh with a vertex count resulting from a transform feedback session.
     * Only available with OpenGL 4.0 or more.
     *
     * @param m how the mesh vertices must be interpreted.
     * @param tfb the id of a TransformFeedback containing the results of a transform feedback session.
     * @param stream the stream to draw.
     */
    void drawFeedback(MeshMode m, GLuint tfb, int stream = 0) const;

    /**
     * The AttributeBuffer of this mesh.
     */
    std::vector< ptr<AttributeBuffer> > attributeBuffers;

private:
    /**
     * The indices buffer of this mesh.
     */
    ptr<AttributeBuffer> indicesBuffer;

    /**
     * The currently bound mesh buffers. The buffers of a mesh must be bound
     * before it can be drawn.
     */
    static const MeshBuffers *CURRENT;

    /**
     * The current value of the primitive restart index.
     */
    static int CURRENT_RESTART_INDEX;

    /**
     * The current value of the patch vertices parameter.
     */
    static int CURRENT_PATCH_VERTICES;

    /**
     * The type of the indices of the currently bound mesh.
     */
    static AttributeType type;

    /**
     * The offset of the indices of the currently bound mesh in its indices buffer.
     */
    static void *offset;

    /**
     * Binds the buffers of this mesh, so that it is ready to be drawn.
     */
    void bind() const;

    /**
     * Unbinds the buffers of this mesh, so that another mesh can be bound instead.
     */
    void unbind() const;

    /**
     * Sets this mesh as the current one. Unbinds the currently bound mesh if
     * necessary.
     */
    void set() const;

    friend class TransformFeedback;

    friend class FrameBuffer;
};

}

#endif
