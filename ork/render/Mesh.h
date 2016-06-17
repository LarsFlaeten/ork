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

#ifndef _ORK_MESH_H_
#define _ORK_MESH_H_

#include <cstring> // for memcpy

#include <GL/glew.h>

#include "ork/render/CPUBuffer.h"
#include "ork/render/GPUBuffer.h"
#include "ork/render/MeshBuffers.h"

namespace ork
{

class FrameBuffer;

/**
 * A MeshBuffers wrapper that provides a convenient API to define the mesh content.
 * @ingroup render
 *
 * @tparam vertex the type of the vertices of this mesh.
 * @tparam index the type of the indices of this mesh.
 */
template <class vertex, class index> class Mesh : public Object
{
public:
    /**
     * Creates a new mesh.
     *
     * @param m how the list of vertices of this mesh must be interpreted.
     * @param usage how the data should be handled.
     * @param vertexCount the initial capacity of the vertex array.
     * @param indiceCount the initial capacity of the indice array.
     */
    Mesh(MeshMode m, MeshUsage usage, int vertexCount = 4, int indiceCount = 4);

    /**
     * Creates a new mesh.
     *
     * @param target the mesh buffers wrapped by this mesh.
     * @param m how the list of vertices of this mesh must be interpreted.
     * @param usage how the data should be handled.
     * @param vertexCount the initial capacity of the vertex array.
     * @param indiceCount the initial capacity of the indice array.
     */
    Mesh(ptr<MeshBuffers> target, MeshMode m, MeshUsage usage, int vertexCount = 4, int indiceCount = 4);

    /**
     * Deletes this mesh.
     */
    virtual ~Mesh();

    /**
     * Returns the interpretation mode of the vertices of this mesh.
     */
    inline MeshMode getMode() const;

    /**
     * Returns the number of vertices in this mesh.
     */
    inline int getVertexCount() const;

    /**
     * Returns a vertex of this mesh.
     *
     * @param i a vertex index.
     */
    inline vertex getVertex(int i) const;

    /**
     * Returns the number of indices of this mesh.
     */
    inline int getIndiceCount() const;

    /**
     * Returns an indice of this mesh.
     *
     * @param i an indice index.
     */
    inline index getIndice(int i) const;

    /**
     * Returns the vertex index used for primitive restart. -1 means no restart.
     */
    inline GLint getPrimitiveRestart() const;

    /**
     * Returns the number of vertices per patch in this mesh, if #getMode() is PATCHES.
     */
    inline GLint getPatchVertices() const;

    /**
     * Returns the MeshBuffers wrapped by this Mesh instance.
     */
    inline ptr<MeshBuffers> getBuffers() const;

    /**
     * Declares an attribute of the vertices of this mesh.
     *
     * @param id a vertex attribute index.
     * @param size the number of components in attributes of this kind.
     * @param type the type of each component in attributes of this kind.
     * @param norm if the attribute components must be normalized to 0..1.
     */
    inline void addAttributeType(int id, int size, AttributeType type, bool norm);

    /**
     * Sets the capacity of the vertex and indice array of this mesh. Does
     * nothing if the provided sizes are smaller than the current ones.
     *
     * @param vertexCount the new vertex array capacity.
     * @param indiceCount the new indice array capacity.
     */
    inline void setCapacity(int vertexCount, int indiceCount);

    /**
     * Adds a vertex to this mesh.
     *
     * @param v a vertex.
     */
    inline void addVertex(const vertex &v);

    /**
     * Adds vertices this mesh.
     *
     * @param v a pointer to a vertex array.
     * @param count number of vertices
     */
    inline void addVertices(const vertex *v, int count);

    /**
     * Adds an indice to this mesh.
     *
     * @param i a vertex index.
     */
    inline void addIndice(index i);

    /**
     * Sets the interpretation mode of the vertices of this mesh.
     */
    inline void setMode(MeshMode mode);

    /**
     * Changes a vertex of this mesh.
     */
    inline void setVertex(int i, const vertex &v);

    /**
     * Changes an indice of this mesh.
     */
    inline void setIndice(int i, index ind);

    /**
     * Sets the vertex index used for primitive restart. -1 means no restart.
     */
    inline void setPrimitiveRestart(GLint restart);

    /**
     * Sets the number of vertices per patch in this mesh, if #getMode() is PATCHES.
     */
    inline void setPatchVertices(GLint vertices);

    /**
     * Removes all the vertices and indices of this mesh.
     */
    inline void clear();

    /**
     * Clears the MeshBuffers.
     */
    inline void clearBuffers();


private:
    /**
     * The usage of this mesh.
     */
    MeshUsage usage;

    /**
     * The Buffer containing the vertices data.
     */
    mutable ptr<Buffer> vertexBuffer;

    /**
     * The Buffer containing the indices data.
     */
    mutable ptr<Buffer> indexBuffer;

    /**
     * True if the vertex data has changed since last call to #uploadVertexDataToGPU.
     */
    mutable bool vertexDataHasChanged;

    /**
     * True if the index data has changed since last call to #uploadIndexDataToGPU.
     */
    mutable bool indexDataHasChanged;

    /**
     * True if the CPU or GPU mesh buffers have been created.
     */
    mutable bool created;

    /**
     * How the list of vertices of this mesh must be interpreted.
     */
    MeshMode m;

    /**
     * The vertices of this mesh.
     */
    vertex *vertices;

    /**
     * The capacity of the vertex array.
     */
    int verticesLength;

    /**
     * The actual number of vertices.
     */
    int verticesCount;

    /**
     * The indices of this mesh.
     */
    index *indices;

    /**
     * The capacity of the indice array.
     */
    int indicesLength;

    /**
     * The actual number of indices.
     */
    int indicesCount;

    /**
     * The vertex index used for primitive restart. -1 means no restart.
     */
    GLint primitiveRestart;

    /**
     * The number of vertices per patch in this mesh, if #getMode() is PATCHES.
     */
    GLint patchVertices;

    /**
     * The MeshBuffers wrapped by this Mesh.
     */
    mutable ptr<MeshBuffers> buffers;

    /**
     * Resizes the vertex array to expand its capacity.
     */
    void resizeVertices(int newSize);

    /**
     * Resizes the indice array to expand its capacity.
     */
    void resizeIndices(int newSize);
    /**
     * Creates the CPU of GPU buffers based on the current content of the
     * vertex and indice arrays.
     */
    void createBuffers() const;
    /**
     * Send the vertices to the GPU.
     */
    void uploadVertexDataToGPU(BufferUsage u) const;

    /**
     * Send the indices to the GPU.
     */
    void uploadIndexDataToGPU(BufferUsage u) const;

    friend class FrameBuffer;
};

template<class vertex, class index>
Mesh<vertex, index>::Mesh(MeshMode m, MeshUsage usage, int vertexCount, int indiceCount) :
    Object("Mesh"), usage(usage), vertexBuffer(NULL), indexBuffer(NULL), created(false), m(m), buffers(new MeshBuffers())
{
	// Lars added check since CPU mesh(VB) does not allways work:
	// Block this until CPUBuffers are fixed
    // TODO: Remove when CPUBuffers have been fixed
    assert(usage != CPU);
    vertices = new vertex[vertexCount];
    verticesLength = vertexCount;
    verticesCount = 0;
    indices = new index[indiceCount];
    indicesLength = indiceCount;
    indicesCount = 0;
    primitiveRestart = -1;
    patchVertices = 0;
    vertexDataHasChanged = true;
    indexDataHasChanged = true;
}

template<class vertex, class index>
Mesh<vertex, index>::Mesh(ptr<MeshBuffers> target, MeshMode m, MeshUsage usage, int vertexCount, int indiceCount) :
    Object("Mesh"), usage(usage), created(false), m(m), buffers(target)
{
   	// Lars added check since CPU mesh(VB) does not work:
	// Block this until CPUBuffers are fixed
    // TODO: Remove when CPUBuffers have been fixed
    assert(usage != CPU);
    vertices = new vertex[vertexCount];
    verticesLength = vertexCount;
    verticesCount = 0;
    indices = new index[indiceCount];
    indicesLength = indiceCount;
    indicesCount = 0;
    primitiveRestart = -1;
    patchVertices = 0;
    vertexDataHasChanged = true;
    indexDataHasChanged = true;
}

template<class vertex, class index>
Mesh<vertex, index>::~Mesh()
{
    delete[] vertices;
    delete[] indices;
}

template<class vertex, class index>
MeshMode Mesh<vertex, index>::getMode() const
{
    return m;
}

template<class vertex, class index>
int Mesh<vertex, index>::getVertexCount() const
{
    return verticesCount;
}

template<class vertex, class index>
vertex Mesh<vertex, index>::getVertex(int i) const
{
    return vertices[i];
}

template<class vertex, class index>
int Mesh<vertex, index>::getIndiceCount() const
{
    return indicesCount;
}

template<class vertex, class index>
index Mesh<vertex, index>::getIndice(int i) const
{
    return indices[i];
}

template<class vertex, class index>
GLint Mesh<vertex, index>::getPrimitiveRestart() const
{
    return primitiveRestart;
}

template<class vertex, class index>
GLint Mesh<vertex, index>::getPatchVertices() const
{
    return patchVertices;
}

template<class vertex, class index>
void Mesh<vertex, index>::uploadVertexDataToGPU(BufferUsage u) const
{
    ptr<GPUBuffer> vb = vertexBuffer.cast<GPUBuffer>();
    assert(vb != NULL); // check it's a GPU mesh
    vb->setData(verticesCount * sizeof(vertex), vertices, u);
    vertexDataHasChanged = false;
}

template<class vertex, class index>
void Mesh<vertex, index>::uploadIndexDataToGPU(BufferUsage u) const
{
    ptr<GPUBuffer> ib = indexBuffer.cast<GPUBuffer>();
    assert(ib != NULL);
    ib->setData(indicesCount * sizeof(index), indices, u);
    indexDataHasChanged = false;
}

template<class vertex, class index>
ptr<MeshBuffers> Mesh<vertex, index>::getBuffers() const
{
    if (!created) {
        createBuffers();
    }

    if ((usage == GPU_DYNAMIC) || (usage == GPU_STREAM)) { // upload data to GPU if needed
        BufferUsage u = usage == GPU_DYNAMIC ? DYNAMIC_DRAW : STREAM_DRAW;
        if (vertexDataHasChanged) {
            uploadVertexDataToGPU(u);
        }
        if ((indicesCount != 0) && indexDataHasChanged) {
            uploadIndexDataToGPU(u);
        }
    }

    buffers->primitiveRestart = primitiveRestart;
    buffers->patchVertices = patchVertices;

    return buffers;
}

template<class vertex, class index>
void Mesh<vertex, index>::addAttributeType(int id, int size, AttributeType type, bool norm)
{
    buffers->addAttributeBuffer(id, size, sizeof(vertex), type, norm);
}

template<class vertex, class index>
void Mesh<vertex, index>::setCapacity(int vertexCount, int indiceCount)
{
    if (verticesCount < vertexCount) {
        resizeVertices(vertexCount);
    }
    if (indicesCount < indiceCount) {
        resizeIndices(indiceCount);
    }
}

template<class vertex, class index>
void Mesh<vertex, index>::addVertex(const vertex &v)
{
    if (verticesCount == verticesLength) {
        resizeVertices(2 * verticesLength);
    }
    vertices[verticesCount++] = v;
    vertexDataHasChanged = true;
}

template<class vertex, class index>
void Mesh<vertex, index>::addVertices(const vertex *v, int count)
{
    for (int i = 0; i < count; ++i) {
        addVertex(v[i]);
    }
}

template<class vertex, class index>
void Mesh<vertex, index>::addIndice(index i)
{
    if (indicesCount == indicesLength) {
        resizeIndices(2 * indicesLength);
    }
    indices[indicesCount++] = i;
    indexDataHasChanged = true;
}

template<class vertex, class index>
void Mesh<vertex, index>::setMode(MeshMode mode)
{
    m = mode;
}

template<class vertex, class index>
void Mesh<vertex, index>::setVertex(int i, const vertex &v)
{
    vertices[i] = v;
    vertexDataHasChanged = true;
}

template<class vertex, class index>
void Mesh<vertex, index>::setIndice(int i, index ind)
{
    indices[i] = ind;
    indexDataHasChanged = true;
}

template<class vertex, class index>
void Mesh<vertex, index>::setPrimitiveRestart(GLint restart)
{
    primitiveRestart = restart;
}

template<class vertex, class index>
void Mesh<vertex, index>::setPatchVertices(GLint vertices)
{
    patchVertices = vertices;
}

template<class vertex, class index>
void Mesh<vertex, index>::resizeVertices(int newSize)
{
    vertex *newVertices = new vertex[newSize];
    memcpy(newVertices, vertices, verticesLength * sizeof(vertex));
    delete[] vertices;
    vertices = newVertices;
    verticesLength = newSize;
    if (created) {
        buffers->reset();
        created = false;
    }
}

template<class vertex, class index>
void Mesh<vertex, index>::resizeIndices(int newSize)
{
    index *newIndices = new index[newSize];
    memcpy(newIndices, indices, indicesLength * sizeof(index));
    delete[] indices;
    indices = newIndices;
    indicesLength = newSize;
    if (created) {
        buffers->reset();
        created = false;
    }
}

template<class vertex, class index>
void Mesh<vertex, index>::clear()
{
    verticesCount = 0;
    indicesCount = 0;
    vertexDataHasChanged = true;
    indexDataHasChanged = true;
    if (created) {
        buffers->reset();
        buffers->setIndicesBuffer(NULL);
        created = false;
    }
}

template<class vertex, class index>
void Mesh<vertex, index>::clearBuffers()
{
    if (created) {
        buffers->reset();
        created = false;
    }
}

template<class vertex, class index>
void Mesh<vertex, index>::createBuffers() const
{
    if (usage == GPU_STATIC || usage == GPU_DYNAMIC || usage ==  GPU_STREAM) {
        GPUBuffer *gpub = new GPUBuffer();
        vertexBuffer = ptr<Buffer>(gpub);
        if (usage == GPU_STATIC) {
            uploadVertexDataToGPU(STATIC_DRAW);
        }
    } else if (usage == CPU) {
        CPUBuffer *cpub = new CPUBuffer(vertices);
        vertexBuffer = ptr<Buffer>((Buffer*)cpub);
    }

    assert(buffers->getAttributeCount() > 0);
    for (int i = 0; i < buffers->getAttributeCount(); ++i) {
        buffers->getAttributeBuffer(i)->setBuffer(vertexBuffer);
    }

    if (indicesCount != 0) {
        if (usage == GPU_STATIC || usage == GPU_DYNAMIC || usage == GPU_STREAM) {
            GPUBuffer *gpub = new GPUBuffer();
            indexBuffer = ptr<Buffer>(gpub);
            if (usage == GPU_STATIC) {
                uploadIndexDataToGPU(STATIC_DRAW);
            }
        } else if (usage == CPU) {
            CPUBuffer *cpub = new CPUBuffer(indices);
            indexBuffer = ptr<Buffer>(cpub);
        }

        AttributeType type;
        switch (sizeof(index)) {
        case 1:
            type = A8UI;
            break;
        case 2:
            type = A16UI;
            break;
        default:
            type = A32UI;
            break;
        }
        buffers->setIndicesBuffer(new AttributeBuffer(0, 1, type, false, indexBuffer));
    }
    buffers->mode = m;
    buffers->nvertices = verticesCount;
    buffers->nindices = indicesCount;
    created = true;
}



}

#endif
