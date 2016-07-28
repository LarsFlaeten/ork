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

#include "ork/render/MeshBuffers.h"

#include <GL/glew.h>

#include "ork/math/half.h"
#include "ork/render/Program.h"
#include "ork/render/FrameBuffer.h"
#include "ork/resource/ResourceTemplate.h"

using namespace std;

namespace ork
{

GLenum getAttributeType(AttributeType t);

GLenum getMeshMode(MeshMode m);

MeshBuffers::MeshBuffers() :
    Object("MeshBuffers"), mode(POINTS), nvertices(0), nindices(0), primitiveRestart(-1), patchVertices(0)
{
}

MeshBuffers::~MeshBuffers()
{
    if (CURRENT == this) {
        unbind();
        CURRENT = NULL;
    }
}

int MeshBuffers::getAttributeCount() const
{
    return (int) attributeBuffers.size();
}

ptr<AttributeBuffer> MeshBuffers::getAttributeBuffer(int index) const
{
    return attributeBuffers[index];
}

ptr<AttributeBuffer> MeshBuffers::getIndiceBuffer() const
{
    return indicesBuffer;
}

void MeshBuffers::addAttributeBuffer(int index, int size, AttributeType type, bool norm)
{
    ptr<AttributeBuffer> a = new AttributeBuffer(index, size, type, norm, NULL);
    attributeBuffers.push_back(a);
}

void MeshBuffers::addAttributeBuffer(int index, int size, int vertexsize, AttributeType type, bool norm)
{
    int offset;
    if (attributeBuffers.size() > 0) {
        ptr<AttributeBuffer> ab = attributeBuffers[attributeBuffers.size() - 1];
        offset = ab->getOffset() + ab->getAttributeSize();
    } else {
        offset = 0;
    }
    ptr<AttributeBuffer> a = new AttributeBuffer(index, size, type, norm, NULL, vertexsize, offset);
    attributeBuffers.push_back(a);
}

void MeshBuffers::addAttributeBuffer(ptr<AttributeBuffer> buffer)
{
    attributeBuffers.push_back(buffer);
}

void MeshBuffers::setIndicesBuffer(ptr<AttributeBuffer> indices)
{
    indicesBuffer = indices;
}

void MeshBuffers::bind() const
{
    assert(attributeBuffers.size() > 0);
    // binds the attribute buffers for each attribute
    for (int i = (int) attributeBuffers.size() - 1; i >= 0; --i) {
        ptr<AttributeBuffer> a = attributeBuffers[i];
        ptr<Buffer> b = a->b;
        b->bind(GL_ARRAY_BUFFER);
        int index = a->index;
        if (a->I) {
            glVertexAttribIPointer(index, a->size, getAttributeType(a->type), a->stride, b->data(a->offset));
        } else if (a->L) {
            glVertexAttribLPointer(index, a->size, getAttributeType(a->type), a->stride, b->data(a->offset));
        } else {
            glVertexAttribPointer(index, a->size, getAttributeType(a->type), a->norm, a->stride, b->data(a->offset));
        }
        glVertexAttribDivisor(index, a->divisor);
        glEnableVertexAttribArray(index);
    }
    assert(FrameBuffer::getError() == 0);
    // binds the indices buffer, if any
    if (indicesBuffer != NULL) {
        ptr<Buffer> b = indicesBuffer->b;
        b->bind(GL_ELEMENT_ARRAY_BUFFER);
        type = indicesBuffer->type;
        offset = b->data(indicesBuffer->offset);
    }
    assert(FrameBuffer::getError() == 0);
}

void MeshBuffers::unbind() const
{
    for (int i = (int) attributeBuffers.size() - 1; i >= 0; --i) {
        ptr<AttributeBuffer> a = attributeBuffers[i];
        int index = a->index;
        glDisableVertexAttribArray(index);
    }
    assert(glGetError() == 0);
}

void MeshBuffers::set() const
{
    if (CURRENT != NULL) {
        CURRENT->unbind();
    }
    bind();
    CURRENT = this;
}

void MeshBuffers::reset() const
{
    if (CURRENT == this) {
        unbind();
        CURRENT = NULL;
    }
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, GLshort defaultValue)
{
    glVertexAttrib1s(index, defaultValue);
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, vec2<GLshort> defaultValue)
{
    glVertexAttrib2s(index, defaultValue.x, defaultValue.y);
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, vec3<GLshort> defaultValue)
{
    glVertexAttrib3s(index, defaultValue.x, defaultValue.y, defaultValue.z);
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, vec4<GLshort> defaultValue)
{
    glVertexAttrib4s(index, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w);
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, GLfloat defaultValue)
{
    glVertexAttrib1f(index, defaultValue);
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, vec2<GLfloat> defaultValue)
{
    glVertexAttrib2f(index, defaultValue.x, defaultValue.y);
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, vec3<GLfloat> defaultValue)
{
    glVertexAttrib3f(index, defaultValue.x, defaultValue.y, defaultValue.z);
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, vec4<GLfloat> defaultValue)
{
    glVertexAttrib4f(index, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w);
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, GLdouble defaultValue)
{
    glVertexAttrib1d(index, defaultValue);
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, vec2<GLdouble> defaultValue)
{
    glVertexAttrib2d(index, defaultValue.x, defaultValue.y);
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, vec3<GLdouble> defaultValue)
{
    glVertexAttrib3d(index, defaultValue.x, defaultValue.y, defaultValue.z);
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, vec4<GLdouble> defaultValue)
{
    glVertexAttrib4d(index, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w);
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, vec4<GLubyte> defaultValue)
{
    glVertexAttrib4Nub(index, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w);
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, int count, GLshort *defaultValue, bool normalize)
{
    switch(count) {
        case 1:
            glVertexAttrib1sv(index, defaultValue);
            break;
        case 2:
            glVertexAttrib2sv(index, defaultValue);
            break;
        case 3:
            glVertexAttrib3sv(index, defaultValue);
            break;
        case 4:
            if (normalize) {
                glVertexAttrib4Nsv(index, defaultValue);
            } else {
                glVertexAttrib4sv(index, defaultValue);
            }
            break;
    }
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, int count, GLfloat *defaultValue, bool normalize)
{
    switch(count) {
        case 1:
            glVertexAttrib1fv(index, defaultValue);
            break;
        case 2:
            glVertexAttrib2fv(index, defaultValue);
            break;
        case 3:
            glVertexAttrib3fv(index, defaultValue);
            break;
        case 4:
            glVertexAttrib4fv(index, defaultValue);
            break;
    }
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, int count, GLdouble *defaultValue, bool normalize)
{
    switch(count) {
        case 1:
            glVertexAttrib1dv(index, defaultValue);
            break;
        case 2:
            glVertexAttrib2dv(index, defaultValue);
            break;
        case 3:
            glVertexAttrib3dv(index, defaultValue);
            break;
        case 4:
            glVertexAttrib4dv(index, defaultValue);
            break;
    }
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, int count, GLbyte *defaultValue, bool normalize)
{
    assert(count == 4);
    if (normalize) {
        glVertexAttrib4Nbv(index, defaultValue);
    } else {
        glVertexAttrib4bv(index, defaultValue);
    }
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, int count, GLint *defaultValue, bool normalize)
{
    assert(count == 4);
    if (normalize) {
        glVertexAttrib4Niv(index, defaultValue);
    } else {
        glVertexAttrib4iv(index, defaultValue);
    }
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, int count, GLubyte *defaultValue, bool normalize)
{
    assert(count == 4);
    if (normalize) {
        glVertexAttrib4Nubv(index, defaultValue);
    } else {
        glVertexAttrib4ubv(index, defaultValue);
    }
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, int count, GLushort *defaultValue, bool normalize)
{
    assert(count == 4);
    if (normalize) {
        glVertexAttrib4Nusv(index, defaultValue);
    } else {
        glVertexAttrib4usv(index, defaultValue);
    }
}

template<>
void MeshBuffers::setDefaultAttribute(GLuint index, int count, GLuint *defaultValue, bool normalize)
{
    assert(count == 4);
    if (normalize) {
        glVertexAttrib4Nuiv(index, defaultValue);
    } else {
        glVertexAttrib4uiv(index, defaultValue);
    }
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, GLint defaultValue)
{
    glVertexAttribI1i(index, defaultValue);
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, vec2<GLint> defaultValue)
{
    glVertexAttribI2i(index, defaultValue.x, defaultValue.y);
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, vec3<GLint> defaultValue)
{
    glVertexAttribI3i(index, defaultValue.x, defaultValue.y, defaultValue.z);
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, vec4<GLint> defaultValue)
{
    glVertexAttribI4i(index, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w);
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, GLuint defaultValue)
{
    glVertexAttribI1ui(index, defaultValue);
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, vec2<GLuint> defaultValue)
{
    glVertexAttribI2ui(index, defaultValue.x, defaultValue.y);
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, vec3<GLuint> defaultValue)
{
    glVertexAttribI3ui(index, defaultValue.x, defaultValue.y, defaultValue.z);
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, vec4<GLuint> defaultValue)
{
    glVertexAttribI4ui(index, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w);
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, int count, GLint *defaultValue)
{
    switch (count) {
        case 1:
            glVertexAttribI1iv(index, defaultValue);
            break;
        case 2:
            glVertexAttribI2iv(index, defaultValue);
            break;
        case 3:
            glVertexAttribI3iv(index, defaultValue);
            break;
        case 4:
            glVertexAttribI4iv(index, defaultValue);
            break;
    }
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, int count, GLuint *defaultValue)
{
    switch (count) {
        case 1:
            glVertexAttribI1uiv(index, defaultValue);
            break;
        case 2:
            glVertexAttribI2uiv(index, defaultValue);
            break;
        case 3:
            glVertexAttribI3uiv(index, defaultValue);
            break;
        case 4:
            glVertexAttribI4uiv(index, defaultValue);
            break;
    }
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, int count, GLbyte *defaultValue)
{
    assert(count == 4);
    glVertexAttribI4bv(index, defaultValue);
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, int count, GLubyte *defaultValue)
{
    assert(count == 4);
    glVertexAttribI4ubv(index, defaultValue);
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, int count, GLshort *defaultValue)
{
    assert(count == 4);
    glVertexAttribI4sv(index, defaultValue);
}

template<>
void MeshBuffers::setDefaultAttributeI(GLuint index, int count, GLushort *defaultValue)
{
    assert(count == 4);
    glVertexAttribI4usv(index, defaultValue);
}

template<>
void MeshBuffers::setDefaultAttributeL(GLuint index, GLdouble defaultValue)
{
    glVertexAttribL1d(index, defaultValue);
}

template<>
void MeshBuffers::setDefaultAttributeL(GLuint index, vec2<GLdouble> defaultValue)
{
    glVertexAttribL2d(index, defaultValue.x, defaultValue.y);
}

template<>
void MeshBuffers::setDefaultAttributeL(GLuint index, vec3<GLdouble> defaultValue)
{
    glVertexAttribL3d(index, defaultValue.x, defaultValue.y, defaultValue.z);
}

template<>
void MeshBuffers::setDefaultAttributeL(GLuint index, vec4<GLdouble> defaultValue)
{
    glVertexAttribL4d(index, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w);
}

template<>
void MeshBuffers::setDefaultAttributeL(GLuint index, int count, GLdouble *defaultValue)
{
    switch (count) {
        case 1:
            glVertexAttribL1dv(index, defaultValue);
            break;
        case 2:
            glVertexAttribL2dv(index, defaultValue);
            break;
        case 3:
            glVertexAttribL3dv(index, defaultValue);
            break;
        case 4:
            glVertexAttribL4dv(index, defaultValue);
            break;
    }
}

void MeshBuffers::setDefaultAttributeP1(GLuint index, GLuint defaultValue, bool isSigned, bool normalize)
{
    glVertexAttribP1ui(index, isSigned ? GL_INT_2_10_10_10_REV : GL_UNSIGNED_INT_2_10_10_10_REV,
        normalize, defaultValue);
}

void MeshBuffers::setDefaultAttributeP2(GLuint index, GLuint defaultValue, bool isSigned, bool normalize)
{
    glVertexAttribP2ui(index, isSigned ? GL_INT_2_10_10_10_REV : GL_UNSIGNED_INT_2_10_10_10_REV,
        normalize, defaultValue);
}

void MeshBuffers::setDefaultAttributeP3(GLuint index, GLuint defaultValue, bool isSigned, bool normalize)
{
    glVertexAttribP3ui(index, isSigned ? GL_INT_2_10_10_10_REV : GL_UNSIGNED_INT_2_10_10_10_REV,
        normalize, defaultValue);
}

void MeshBuffers::setDefaultAttributeP4(GLuint index, GLuint defaultValue, bool isSigned, bool normalize)
{
    glVertexAttribP4ui(index, isSigned ? GL_INT_2_10_10_10_REV : GL_UNSIGNED_INT_2_10_10_10_REV,
        normalize, defaultValue);
}

void MeshBuffers::setDefaultAttributeP(GLuint index, int count, GLuint *defaultValue, bool isSigned, bool normalize)
{
    switch (count) {
        case 1:
            glVertexAttribP1uiv(index, isSigned ? GL_INT_2_10_10_10_REV : GL_UNSIGNED_INT_2_10_10_10_REV,
                normalize, defaultValue);
            break;
        case 2:
            glVertexAttribP2uiv(index, isSigned ? GL_INT_2_10_10_10_REV : GL_UNSIGNED_INT_2_10_10_10_REV,
                normalize, defaultValue);
            break;
        case 3:
            glVertexAttribP3uiv(index, isSigned ? GL_INT_2_10_10_10_REV : GL_UNSIGNED_INT_2_10_10_10_REV,
                normalize, defaultValue);
            break;
        case 4:
            glVertexAttribP4uiv(index, isSigned ? GL_INT_2_10_10_10_REV : GL_UNSIGNED_INT_2_10_10_10_REV,
                normalize, defaultValue);
            break;
    }
}

void MeshBuffers::swap(ptr<MeshBuffers> buffers)
{
    std::swap(mode, buffers->mode);
    std::swap(nvertices, buffers->nvertices);
    std::swap(nindices, buffers->nindices);
    std::swap(bounds, buffers->bounds);
    std::swap(attributeBuffers, buffers->attributeBuffers);
    std::swap(indicesBuffer, buffers->indicesBuffer);
}

void MeshBuffers::draw(MeshMode m, GLint first, GLsizei count, GLsizei primCount, GLint base) const
{
    if (CURRENT != this) {
        set();
    }

    if (primitiveRestart != CURRENT_RESTART_INDEX) {
        if (primitiveRestart >= 0) {
            glEnable(GL_PRIMITIVE_RESTART);
            glPrimitiveRestartIndex(GLuint(primitiveRestart));
        } else {
            glDisable(GL_PRIMITIVE_RESTART);
        }
        CURRENT_RESTART_INDEX = primitiveRestart;
    }
    if (patchVertices > 0 && patchVertices != CURRENT_PATCH_VERTICES) {
        glPatchParameteri(GL_PATCH_VERTICES, patchVertices);
    }

    if (indicesBuffer == NULL) {
        if (primCount == 1) {
            glDrawArrays(getMeshMode(m), first, count);
        } else {
            glDrawArraysInstanced(getMeshMode(m), first, count, primCount);
        }
    } else {
        void *indices = offset;
        if (first > 0) {
            indices = (void*) (((unsigned char*) offset) + first * indicesBuffer->getAttributeSize());
        }
        if (base == 0) {
            if (primCount == 1) {
                glDrawElements(getMeshMode(m), count, getAttributeType(type), indices);
            } else {
                glDrawElementsInstanced(getMeshMode(m), count, getAttributeType(type), indices, primCount);
            }
        } else {
            if (primCount == 1) {
                glDrawElementsBaseVertex(getMeshMode(m), count, getAttributeType(type), indices, base);
            } else {
                glDrawElementsInstancedBaseVertex(getMeshMode(m), count, getAttributeType(type), indices, primCount, base);
            }
        }
    }

#ifndef NDEBUG
    GLenum err = glGetError();
    if (err != 0) {
        if (Program::CURRENT == NULL || !Program::CURRENT->checkSamplers()) {
            if (Logger::ERROR_LOGGER != NULL) {
                ostringstream oss;
                oss << "OpenGL error " << err << ", returned string '" << gluErrorString(err) << "'";
                Logger::ERROR_LOGGER->log("RENDER", oss.str());
                Logger::ERROR_LOGGER->flush();
            }
            assert(err == 0);
        }
    }
#endif
}

void MeshBuffers::multiDraw(MeshMode m, GLint *firsts, GLsizei *counts, GLsizei primCount, GLint *bases) const
{
    if (CURRENT != this) {
        set();
    }

    if (primitiveRestart != CURRENT_RESTART_INDEX) {
        if (primitiveRestart >= 0) {
            glEnable(GL_PRIMITIVE_RESTART);
            glPrimitiveRestartIndex(GLuint(primitiveRestart));
        } else {
            glDisable(GL_PRIMITIVE_RESTART);
        }
        CURRENT_RESTART_INDEX = primitiveRestart;
    }
    if (patchVertices > 0 && patchVertices != CURRENT_PATCH_VERTICES) {
        glPatchParameteri(GL_PATCH_VERTICES, patchVertices);
    }

    if (indicesBuffer == NULL) {
        glMultiDrawArrays(getMeshMode(m), firsts, counts, primCount);
    } else {
        int size = indicesBuffer->getAttributeSize();
        GLvoid **indices = new void*[primCount];
        for (int i = 0; i < primCount; ++i) {
            indices[i] = (void*) (((unsigned char*) offset) + firsts[i] * size);
        }
        if (bases == NULL) {
            glMultiDrawElements(getMeshMode(m), counts, getAttributeType(type), (const GLvoid**) indices, primCount);
        } else {
            glMultiDrawElementsBaseVertex(getMeshMode(m), counts, getAttributeType(type), indices, primCount, bases);
        }
        delete[] indices;
    }

#ifndef NDEBUG
    GLenum err = glGetError();
    if (err != 0) {
        if (Program::CURRENT == NULL || Program::CURRENT->checkSamplers()) {
            if (Logger::ERROR_LOGGER != NULL) {
                ostringstream oss;
                oss << "OpenGL error " << err << ", returned string '" << gluErrorString(err) << "'";
                Logger::ERROR_LOGGER->log("RENDER", oss.str());
                Logger::ERROR_LOGGER->flush();
            }
            assert(err == 0);
        }
    }
#endif
}

void MeshBuffers::drawIndirect(MeshMode m, const Buffer &buf) const
{
    if (CURRENT != this) {
        set();
    }

    if (primitiveRestart != CURRENT_RESTART_INDEX) {
        if (primitiveRestart >= 0) {
            glEnable(GL_PRIMITIVE_RESTART);
            glPrimitiveRestartIndex(GLuint(primitiveRestart));
        } else {
            glDisable(GL_PRIMITIVE_RESTART);
        }
        CURRENT_RESTART_INDEX = primitiveRestart;
    }
    if (patchVertices > 0 && patchVertices != CURRENT_PATCH_VERTICES) {
        glPatchParameteri(GL_PATCH_VERTICES, patchVertices);
    }

    buf.bind(GL_DRAW_INDIRECT_BUFFER);
    if (indicesBuffer == NULL) {
        glDrawArraysIndirect(getMeshMode(m), buf.data(0));
    } else {
        glDrawElementsIndirect(getMeshMode(m), getAttributeType(type), buf.data(0));
    }
    buf.unbind(GL_DRAW_INDIRECT_BUFFER);

#ifndef NDEBUG
    GLenum err = glGetError();
    if (err != 0) {
        if (Program::CURRENT == NULL || Program::CURRENT->checkSamplers()) {
            if (Logger::ERROR_LOGGER != NULL) {
                ostringstream oss;
                oss << "OpenGL error " << err << ", returned string '" << gluErrorString(err) << "'";
                Logger::ERROR_LOGGER->log("RENDER", oss.str());
                Logger::ERROR_LOGGER->flush();
            }
            assert(err == 0);
        }
    }
#endif
}

void MeshBuffers::drawFeedback(MeshMode m, GLuint tfb, int stream) const
{
    if (CURRENT != this) {
        set();
    }

    if (primitiveRestart != CURRENT_RESTART_INDEX) {
        if (primitiveRestart >= 0) {
            glEnable(GL_PRIMITIVE_RESTART);
            glPrimitiveRestartIndex(GLuint(primitiveRestart));
        } else {
            glDisable(GL_PRIMITIVE_RESTART);
        }
        CURRENT_RESTART_INDEX = primitiveRestart;
    }
    if (patchVertices > 0 && patchVertices != CURRENT_PATCH_VERTICES) {
        glPatchParameteri(GL_PATCH_VERTICES, patchVertices);
    }

    glDrawTransformFeedbackStream(getMeshMode(m), tfb, stream);

#ifndef NDEBUG
    GLenum err = glGetError();
    if (err != 0) {
        if (Program::CURRENT == NULL || Program::CURRENT->checkSamplers()) {
            if (Logger::ERROR_LOGGER != NULL) {
                ostringstream oss;
                oss << "OpenGL error " << err << ", returned string '" << gluErrorString(err) << "'";
                Logger::ERROR_LOGGER->log("RENDER", oss.str());
                Logger::ERROR_LOGGER->flush();
            }
            assert(err == 0);
        }
    }
#endif
}

const MeshBuffers *MeshBuffers::CURRENT = NULL;

int MeshBuffers::CURRENT_RESTART_INDEX = -1;

int MeshBuffers::CURRENT_PATCH_VERTICES = 0;

AttributeType MeshBuffers::type;

void *MeshBuffers::offset;

/// @cond RESOURCES

class MeshResource : public ResourceTemplate<0, MeshBuffers>
{
public:
    MeshResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<0, MeshBuffers>(manager, name, desc)
    {
        char buf[256];
        istringstream in(string((char*) desc->getData(), desc->getSize()));
        e = e == NULL ? desc->descriptor : e;

        try {
            in >> bounds.xmin;
            in >> bounds.xmax;
            in >> bounds.ymin;
            in >> bounds.ymax;
            in >> bounds.zmin;
            in >> bounds.zmax;

            in >> buf;

            if (strcmp(buf, "points") == 0) {
                mode = POINTS;
            } else if (strcmp(buf, "lines") == 0) {
                mode = LINES;
            } else if (strcmp(buf, "linesadjacency") == 0) {
                mode = LINES_ADJACENCY;
            } else if (strcmp(buf, "linestrip") == 0) {
                mode = LINE_STRIP;
            } else if (strcmp(buf, "linestripadjacency") == 0) {
                mode = LINE_STRIP_ADJACENCY;
            } else if (strcmp(buf, "triangles") == 0) {
                mode = TRIANGLES;
            } else if (strcmp(buf, "trianglesadjacency") == 0) {
                mode = TRIANGLES_ADJACENCY;
            } else if (strcmp(buf, "trianglestrip") == 0) {
                mode = TRIANGLE_STRIP;
            } else if (strcmp(buf, "trianglestripadjacency") == 0) {
                mode = TRIANGLE_STRIP_ADJACENCY;
            } else if (strcmp(buf, "trianglefan") == 0) {
                mode = TRIANGLE_FAN;
            } else {
                if (Logger::ERROR_LOGGER != NULL) {
                    log(Logger::ERROR_LOGGER, desc, e, "Invalid mesh topology '" + string(buf) + "'");
                }
                throw exception();
            }

            unsigned int attributeCount;
            in >> attributeCount;

            int vertexSize = 0;
            int* attributeIds = new int[attributeCount];
            unsigned int *attributeComponents = new unsigned int[attributeCount];
            AttributeType *attributeTypes = new AttributeType[attributeCount];
            bool *attributeNorms = new bool[attributeCount];

            try {
                for (unsigned int i = 0; i < attributeCount; ++i) {
                    in >> attributeIds[i];
                    in >> attributeComponents[i];

                    in >> buf;
                    if (strcmp(buf, "byte") == 0) {
                        attributeTypes[i] = A8I;
                        vertexSize += attributeComponents[i] * 1;
                    } else if (strcmp(buf, "ubyte") == 0) {
                        attributeTypes[i] = A8UI;
                        vertexSize += attributeComponents[i] * 1;
                    } else if (strcmp(buf, "short") == 0) {
                        attributeTypes[i] = A16I;
                        vertexSize += attributeComponents[i] * 2;
                    } else if (strcmp(buf, "ushort") == 0) {
                        attributeTypes[i] = A16UI;
                        vertexSize += attributeComponents[i] * 2;
                    } else if (strcmp(buf, "int") == 0) {
                        attributeTypes[i] = A32I;
                        vertexSize += attributeComponents[i] * 4;
                    } else if (strcmp(buf, "uint") == 0) {
                        attributeTypes[i] = A32UI;
                        vertexSize += attributeComponents[i] * 4;
                    } else if (strcmp(buf, "float") == 0) {
                        attributeTypes[i] = A32F;
                        vertexSize += attributeComponents[i] * 4;
                    } else if (strcmp(buf, "double") == 0) {
                        attributeTypes[i] = A64F;
                        vertexSize += attributeComponents[i] * 8;
                    } else {
                        if (Logger::ERROR_LOGGER != NULL) {
                            log(Logger::ERROR_LOGGER, desc, e, "Invalid mesh vertex component type '" + string(buf) + "'");
                        }
                        throw exception();
                    }

                    in >> buf;
                    if (strcmp(buf, "true") == 0) {
                        attributeNorms[i] = true;
                    } else if (strcmp(buf, "false") == 0) {
                        attributeNorms[i] = false;
                    } else {
                        if (Logger::ERROR_LOGGER != NULL) {
                            log(Logger::ERROR_LOGGER, desc, e, "Invalid mesh vertex normalization '" + string(buf) + "'");
                        }
                        throw exception();
                    }
                }
            } catch (...) {
                delete[] attributeIds;
                delete[] attributeComponents;
                delete[] attributeTypes;
                delete[] attributeNorms;
                throw exception();
            }

            for (unsigned int i = 0; i < attributeCount; ++i) {
                addAttributeBuffer(attributeIds[i], attributeComponents[i],
                    vertexSize, attributeTypes[i], attributeNorms[i]);
            }
            delete[] attributeIds;
            delete[] attributeComponents;
            delete[] attributeTypes;
            delete[] attributeNorms;

            unsigned int vertexCount;
            in >> vertexCount;
            nvertices = vertexCount;

            unsigned char* vertexBuffer = new unsigned char[vertexCount * vertexSize];
            unsigned int offset = 0;

            for (unsigned int i = 0; i < vertexCount; ++i) {
                for (unsigned int j = 0; j < attributeCount; ++j) {
                    ptr<AttributeBuffer> ab = getAttributeBuffer(j);
                    for (int k = 0; k < ab->getSize(); ++k) {
                        switch (ab->getType()) {
                            case A8I: {
                                int ic;
                                in >> ic;
                                char c = (char) ic;
                                memcpy(vertexBuffer + offset, &c, sizeof(char));
                                offset += sizeof(char);
                                break;
                            }
                            case A8UI: {
                                int iuc;
                                in >> iuc;
                                unsigned char uc = (unsigned char) iuc;
                                memcpy(vertexBuffer + offset, &uc, sizeof(unsigned char));
                                offset += sizeof(unsigned char);
                                break;
                            }
                            case A16I: {
                                short s;
                                in >> s;
                                memcpy(vertexBuffer + offset, &s, sizeof(short));
                                offset += sizeof(short);
                                break;
                            }
                            case A16UI: {
                                unsigned short us;
                                in >> us;
                                memcpy(vertexBuffer + offset, &us, sizeof(unsigned short));
                                offset += sizeof(unsigned short);
                                break;
                            }
                            case A32I: {
                                int si;
                                in >> si;
                                memcpy(vertexBuffer + offset, &si, sizeof(int));
                                offset += sizeof(int);
                                break;
                            }
                            case A32UI: {
                                unsigned int ui;
                                in >> ui;
                                memcpy(vertexBuffer + offset, &ui, sizeof(unsigned int));
                                offset += sizeof(unsigned int);
                                break;
                            }
                            case A16F: {
                                half h;
                                float f;
                                in >> f;
                                h = f;
                                memcpy(vertexBuffer + offset, &h, sizeof(half));
                                offset += sizeof(half);
                                break;
                            }
                            case A32F: {
                                float f;
                                in >> f;
                                memcpy(vertexBuffer + offset, &f, sizeof(float));
                                offset += sizeof(float);
                                break;
                            }
                            case A64F: {
                                double d;
                                in >> d;
                                memcpy(vertexBuffer + offset, &d, sizeof(double));
                                offset += sizeof(double);
                                break;
                            }

                            // not handled (don't know why)
                            case A32I_2_10_10_10_REV:
                            case A32UI_2_10_10_10_REV:
                            case A32I_FIXED:
                            {
                                assert(false); // unsupported
                                break;
                            }
                        }
                    }
                }
            }

            ptr<GPUBuffer> gpub = new GPUBuffer();
            gpub->setData(vertexCount * vertexSize, vertexBuffer, STATIC_DRAW);
            for (int i = 0; i < getAttributeCount(); ++i) {
                getAttributeBuffer(i)->setBuffer(gpub);
            }
            delete[] vertexBuffer;

            unsigned int indiceCount;
            in >> indiceCount;
            nindices = indiceCount;

            if (nindices > 0) {
                int indiceSize;
                AttributeType type;
                if (vertexCount < 256) {
                    indiceSize = 1;
                    type = A8UI;
                } else if (vertexCount < 65536) {
                    indiceSize = 2;
                    type = A16UI;
                } else {
                    indiceSize = 4;
                    type = A32UI;
                }

                unsigned char* indiceBuffer = new unsigned char[indiceCount * indiceSize];
                offset = 0;

                if (indiceSize == 1) {
                    for (unsigned int i = 0; i < indiceCount; ++i) {
                        int ic;
                        in >> ic;
                        unsigned char c = (unsigned char) ic;
                        memcpy(indiceBuffer + offset, &c, 1);
                        offset += 1;
                    }
                } else if (indiceSize == 2) {
                    for (unsigned int i = 0; i < indiceCount; ++i) {
                        int ic;
                        in >> ic;
                        unsigned short c = (unsigned short) ic;
                        memcpy(indiceBuffer + offset, &c, 2);
                        offset += 2;
                    }
                } else {
                    for (unsigned int i = 0; i < indiceCount; ++i) {
                        int ic;
                        in >> ic;
                        char c = (char) ic;
                        memcpy(indiceBuffer + offset, &c, indiceSize);
                        offset += indiceSize;
                    }
                }

                gpub = new GPUBuffer();
                gpub->setData(indiceCount * indiceSize, indiceBuffer, STATIC_DRAW);
                setIndicesBuffer(new AttributeBuffer(0, 1, type, false, gpub));

                delete []indiceBuffer;
            }

            desc->clearData();
        } catch (...) {
            desc->clearData();
            throw exception();
        }
    }
};

extern const char mesh[] = "mesh";

static ResourceFactory::Type<mesh, MeshResource> MeshType;

/// @endcond

}
