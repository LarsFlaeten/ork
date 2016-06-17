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

#include "ork/render/Uniform.h"

#include <GL/glew.h>

#include "ork/render/FrameBuffer.h"

namespace ork
{

#ifdef ORK_NO_GLPROGRAMUNIFORM
Uniform::Uniform(const char *type, Program *program, UniformBlock *block, const string &name, GLint location) :
    Object(type), program(program), block(block), name(name), location(location), dirty(false)
{
}
#else
Uniform::Uniform(const char *type, Program *program, UniformBlock *block, const string &name, GLint location) :
    Object(type), program(program), block(block), name(name), location(location)
{
}
#endif

Uniform::~Uniform()
{
}

string Uniform::getName() const
{
    return name;
}

#ifdef ORK_NO_GLPROGRAMUNIFORM
void Uniform::setValueIfCurrent()
{
    if (block != NULL) {
        return;
    }
    if (program == Program::CURRENT) {
        setValue();
        dirty = false;
    } else {
        dirty = true;
    }
}
#endif

volatile void *Uniform::mapBuffer(GLint offset) const
{
    return block->mapBuffer(offset);
}

// ----------------------------------------------------------------------------

const char uniform1f[] = "Uniform1f";

const char uniform1d[] = "Uniform1d";

const char uniform1i[] = "Uniform1i";

const char uniform1ui[] = "Uniform1ui";

const char uniform1b[] = "Uniform1b";

template<> void Uniform1f::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform1f(location, value);
#else
    glProgramUniform1fEXT(program->getId(), location, value);
#endif
}

template<> void Uniform1d::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform1d(location, value);
#else
    glProgramUniform1d(program->getId(), location, value);
#endif
}

template<> void Uniform1i::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform1i(location, value);
#else
    glProgramUniform1iEXT(program->getId(), location, value);
#endif
}

template<> void Uniform1ui::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform1ui(location, value);
#else
    glProgramUniform1uiEXT(program->getId(), location, value);
#endif
}

template<> void Uniform1b::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform1ui(location, value ? 1 : 0);
#else
    glProgramUniform1uiEXT(program->getId(), location, value ? 1 : 0);
#endif
}

// ----------------------------------------------------------------------------

const char uniform2f[] = "Uniform2f";

const char uniform2d[] = "Uniform2d";

const char uniform2i[] = "Uniform2i";

const char uniform2ui[] = "Uniform2ui";

const char uniform2b[] = "Uniform2b";

template<> void Uniform2f::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform2f(location, value.x, value.y);
#else
    glProgramUniform2fEXT(program->getId(), location, value.x, value.y);
#endif
}

template<> void Uniform2d::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform2d(location, value.x, value.y);
#else
    glProgramUniform2d(program->getId(), location, value.x, value.y);
#endif
}

template<> void Uniform2i::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform2i(location, value.x, value.y);
#else
    glProgramUniform2iEXT(program->getId(), location, value.x, value.y);
#endif
}

template<> void Uniform2ui::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform2ui(location, value.x, value.y);
#else
    glProgramUniform2uiEXT(program->getId(), location, value.x, value.y);
#endif
}

template<> void Uniform2b::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform2ui(location, value.x ? 1 : 0, value.y ? 1 : 0);
#else
    glProgramUniform2uiEXT(program->getId(), location, value.x ? 1 : 0, value.y ? 1 : 0);
#endif
}

// ----------------------------------------------------------------------------

const char uniform3f[] = "Uniform3f";

const char uniform3d[] = "Uniform3d";

const char uniform3i[] = "Uniform3i";

const char uniform3ui[] = "Uniform3ui";

const char uniform3b[] = "Uniform3b";

template<> void Uniform3f::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform3f(location, value.x, value.y, value.z);
#else
    glProgramUniform3fEXT(program->getId(), location, value.x, value.y, value.z);
#endif
}

template<> void Uniform3d::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform3d(location, value.x, value.y, value.z);
#else
    glProgramUniform3d(program->getId(), location, value.x, value.y, value.z);
#endif
}

template<> void Uniform3i::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform3i(location, value.x, value.y, value.z);
#else
    glProgramUniform3iEXT(program->getId(), location, value.x, value.y, value.z);
#endif
}

template<> void Uniform3ui::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform3ui(location, value.x, value.y, value.z);
#else
    glProgramUniform3uiEXT(program->getId(), location, value.x, value.y, value.z);
#endif
}

template<> void Uniform3b::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform3ui(location, value.x ? 1 : 0, value.y ? 1 : 0, value.z ? 1 : 0);
#else
    glProgramUniform3uiEXT(program->getId(), location, value.x ? 1 : 0, value.y ? 1 : 0, value.z ? 1 : 0);
#endif
}

// ----------------------------------------------------------------------------

const char uniform4f[] = "Uniform4f";

const char uniform4d[] = "Uniform4d";

const char uniform4i[] = "Uniform4i";

const char uniform4ui[] = "Uniform4ui";

const char uniform4b[] = "Uniform4b";

template<> void Uniform4f::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform4f(location, value.x, value.y, value.z, value.w);
#else
    glProgramUniform4fEXT(program->getId(), location, value.x, value.y, value.z, value.w);
#endif
}

template<> void Uniform4d::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform4d(location, value.x, value.y, value.z, value.w);
#else
    glProgramUniform4d(program->getId(), location, value.x, value.y, value.z, value.w);
#endif
}

template<> void Uniform4i::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform4i(location, value.x, value.y, value.z, value.w);
#else
    glProgramUniform4iEXT(program->getId(), location, value.x, value.y, value.z, value.w);
#endif
}

template<> void Uniform4ui::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform4ui(location, value.x, value.y, value.z, value.w);
#else
    glProgramUniform4uiEXT(program->getId(), location, value.x, value.y, value.z, value.w);
#endif
}

template<> void Uniform4b::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniform4ui(location, value.x ? 1 : 0, value.y ? 1 : 0, value.z ? 1 : 0, value.w ? 1 : 0);
#else
    glProgramUniform4uiEXT(program->getId(), location, value.x ? 1 : 0, value.y ? 1 : 0, value.z ? 1 : 0, value.w ? 1 : 0);
#endif
}

// ----------------------------------------------------------------------------

const char uniformMatrix2f[] = "UniformMatrix2f";

const char uniformMatrix3f[] = "UniformMatrix3f";

const char uniformMatrix4f[] = "UniformMatrix4f";

const char uniformMatrix2x3f[] = "UniformMatrix2x3f";

const char uniformMatrix2x4f[] = "UniformMatrix2x4f";

const char uniformMatrix3x2f[] = "UniformMatrix3x2f";

const char uniformMatrix3x4f[] = "UniformMatrix3x4f";

const char uniformMatrix4x2f[] = "UniformMatrix4x2f";

const char uniformMatrix4x3f[] = "UniformMatrix4x3f";

const char uniformMatrix2d[] = "UniformMatrix2d";

const char uniformMatrix3d[] = "UniformMatrix3d";

const char uniformMatrix4d[] = "UniformMatrix4d";

const char uniformMatrix2x3d[] = "UniformMatrix2x3d";

const char uniformMatrix2x4d[] = "UniformMatrix2x4d";

const char uniformMatrix3x2d[] = "UniformMatrix3x2d";

const char uniformMatrix3x4d[] = "UniformMatrix3x4d";

const char uniformMatrix4x2d[] = "UniformMatrix4x2d";

const char uniformMatrix4x3d[] = "UniformMatrix4x3d";

template<> void UniformMatrix2f::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix2fv(location, 1, true, value);
#else
    glProgramUniformMatrix2fvEXT(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix<MAT3F, GLfloat, 3, 3, uniformMatrix3f, valueMatrix3f>::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix3fv(location, 1, true, value);
#else
    glProgramUniformMatrix3fvEXT(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix<MAT4F, GLfloat, 4, 4, uniformMatrix4f, valueMatrix4f>::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix4fv(location, 1, true, value);
#else
    glProgramUniformMatrix4fvEXT(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix2x3f::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix2x3fv(location, 1, true, value);
#else
    glProgramUniformMatrix2x3fvEXT(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix2x4f::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix2x4fv(location, 1, true, value);
#else
    glProgramUniformMatrix2x4fvEXT(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix3x2f::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix3x2fv(location, 1, true, value);
#else
    glProgramUniformMatrix3x2fvEXT(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix3x4f::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix3x4fv(location, 1, true, value);
#else
    glProgramUniformMatrix3x4fvEXT(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix4x2f::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix4x2fv(location, 1, true, value);
#else
    glProgramUniformMatrix4x2fvEXT(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix4x3f::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix4x3fv(location, 1, true, value);
#else
    glProgramUniformMatrix4x3fvEXT(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix2d::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix2dv(location, 1, true, value);
#else
    glProgramUniformMatrix2dv(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix<MAT3D, GLdouble, 3, 3, uniformMatrix3d, valueMatrix3d>::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix3dv(location, 1, true, value);
#else
    glProgramUniformMatrix3dv(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix<MAT4D, GLdouble, 4, 4, uniformMatrix4d, valueMatrix4d>::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix4dv(location, 1, true, value);
#else
    glProgramUniformMatrix4dv(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix2x3d::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix2x3dv(location, 1, true, value);
#else
    glProgramUniformMatrix2x3dv(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix2x4d::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix2x4dv(location, 1, true, value);
#else
    glProgramUniformMatrix2x4dv(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix3x2d::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix3x2dv(location, 1, true, value);
#else
    glProgramUniformMatrix3x2dv(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix3x4d::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix3x4dv(location, 1, true, value);
#else
    glProgramUniformMatrix3x4dv(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix4x2d::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix4x2dv(location, 1, true, value);
#else
    glProgramUniformMatrix4x2dv(program->getId(), location, 1, true, value);
#endif
}

template<> void UniformMatrix4x3d::setValue()
{
#ifdef ORK_NO_GLPROGRAMUNIFORM
    glUniformMatrix4x3dv(location, 1, true, value);
#else
    glProgramUniformMatrix4x3dv(program->getId(), location, 1, true, value);
#endif
}

// ----------------------------------------------------------------------------

UniformSampler::UniformSampler(UniformType type, Program *program, UniformBlock *block, const string &name, GLint location) :
    Uniform("UniformSampler", program, block, name, location), type(type), unit(-1)
{
}

UniformSampler::~UniformSampler()
{
}

UniformType UniformSampler::getType() const
{
    return type;
}

ptr<Sampler> UniformSampler::getSampler() const
{
    return sampler;
}

void UniformSampler::setSampler(const ptr<Sampler> sampler)
{
    this->sampler = sampler;
    if (program != NULL && program == Program::CURRENT) {
        setValue();
    }
}

ptr<Texture> UniformSampler::get() const
{
    return value;
}

void UniformSampler::set(ptr<Texture> value)
{
    if (program != NULL) {
        if (this->value != NULL) {
            this->value->removeUser(program->getId());
        }
        if (value != NULL) {
            value->addUser(program->getId());
        }
    }
    this->value = value;
    if (program != NULL && program == Program::CURRENT) {
        setValue();
    }
}

void UniformSampler::setValue(ptr<Value> v)
{
    ptr<ValueSampler> vs = v.cast<ValueSampler>();
    //setSampler(vs->getSampler());
    set(vs->get());
}

void UniformSampler::setValue()
{
    if (value != NULL && location != -1) {
        GLint newUnit = value->bindToTextureUnit(sampler, program->getId());
        assert(newUnit >= 0);
        if (newUnit != unit) {
#ifdef ORK_NO_GLPROGRAMUNIFORM
            glUniform1i(location, newUnit);
#else
            glProgramUniform1iEXT(program->getId(), location, newUnit);
#endif
            assert(FrameBuffer::getError() == 0);
            unit = newUnit;
        }
    }
}

// ----------------------------------------------------------------------------

static_ptr<Factory<string, ptr<GPUBuffer> > > UniformBlock::buffers
    (new Factory<string, ptr<GPUBuffer> >(UniformBlock::newBuffer));

class UniformBlockBuffer : public GPUBuffer
{
public:
    string name;

    UniformBlockBuffer(const string name) : GPUBuffer(), name(name)
    {
    }
};

UniformBlock::UniformBlock(Program *program, const string &name, GLuint index, GLuint size) :
    Object("UniformBlock"), program(program), name(name), index(index), size(size), buffer(NULL)
{
}

UniformBlock::~UniformBlock()
{
    setBuffer(NULL);
}

ptr<GPUBuffer> UniformBlock::newBuffer(string name)
{
    return new UniformBlockBuffer(name);
}

string UniformBlock::getName() const
{
    return name;
}

ptr<GPUBuffer> UniformBlock::getBuffer() const
{
    return buffer;
}

ptr<Uniform> UniformBlock::getUniform(const string &name) const
{
    map<string, ptr<Uniform> >::const_iterator i = uniforms.find(name);
    if (i == uniforms.end()) {
        i = uniforms.find(getName() + "." + name);
        if (i == uniforms.end()) {
            return NULL;
        }
    }
    return i->second;
}

void UniformBlock::setBuffer(ptr<GPUBuffer> buffer)
{
    if (this->buffer != NULL && isMapped()) {
        unmapBuffer();
        ptr<UniformBlockBuffer> b = this->buffer.cast<UniformBlockBuffer>();
        if (b != NULL) {
            buffers->put(b->name);
        }
    }
    this->buffer = buffer;
}

bool UniformBlock::isMapped() const
{
    assert(buffer != NULL);
    return buffer->getMappedData() != NULL;
}

volatile void *UniformBlock::mapBuffer(GLint offset)
{
    assert(buffer != NULL);
    volatile void *result = buffer->getMappedData();
    if (result == NULL) {
        result = buffer->map(READ_WRITE);
    }
    return (void*) (((unsigned char*) result) + offset);
}

void UniformBlock::unmapBuffer()
{
    assert(buffer != NULL && buffer->getMappedData() != NULL);
    buffer->unmap();
}

}
