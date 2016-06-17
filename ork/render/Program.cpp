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

#include "ork/render/Program.h"

#include <GL/glew.h>
#include <set>

#include "ork/resource/ResourceTemplate.h"
#include "ork/render/FrameBuffer.h"

#define GL_DOUBLE_MAT2x3 0x8F49
#define GL_DOUBLE_MAT2x4 0x8F4A
#define GL_DOUBLE_MAT3x2 0x8F4B
#define GL_DOUBLE_MAT3x4 0x8F4C
#define GL_DOUBLE_MAT4x2 0x8F4D
#define GL_DOUBLE_MAT4x3 0x8F4E

namespace ork
{

GLenum getStage(Stage s);

Program *Program::CURRENT = NULL;

Program::Program() : Object("Program")
{
}

Program::Program(const vector< ptr<Module> > &modules) : Object("Program")
{
    init(modules);
}

Program::Program(ptr<Module> module) : Object("Program")
{
    vector< ptr<Module> > modules;
    modules.push_back(module);
    init(modules);
}

void Program::init(const vector< ptr<Module> > &modules)
{
    this->modules = modules;

    // creates the program
    programId = glCreateProgram();
    assert(programId > 0);

    int feedbackVaryingCount = 0;

    // attach all the shader objects
    vector< ptr<Module> >::iterator i;
    for (i = this->modules.begin(); i != this->modules.end(); ++i) {
        (*i)->users.insert(this);
        if ((*i)->vertexShaderId != -1) {
            glAttachShader(programId, (*i)->vertexShaderId);
        }
        if ((*i)->tessControlShaderId != -1) {
            glAttachShader(programId, (*i)->tessControlShaderId);
        }
        if ((*i)->tessEvalShaderId != -1) {
            glAttachShader(programId, (*i)->tessEvalShaderId);
        }
        if ((*i)->geometryShaderId != -1) {
            glAttachShader(programId, (*i)->geometryShaderId);
        }
        if ((*i)->fragmentShaderId != -1) {
            glAttachShader(programId, (*i)->fragmentShaderId);
        }
        feedbackVaryingCount += (*i)->feedbackVaryings.size();
    }

    // intializes the transform feedback varyings
    if (feedbackVaryingCount > 0) {
        int index =  0;
        int interleaved = 0;
        const char **varyings = new const char*[feedbackVaryingCount];
        for (i = this->modules.begin(); i != this->modules.end(); ++i) {
            vector<string>::iterator j = (*i)->feedbackVaryings.begin();
            for ( ; j != (*i)->feedbackVaryings.begin(); ++j) {
                varyings[index++] = (*j).c_str();
            }
            if ((*i)->feedbackMode != 0) {
                if (interleaved == 0 || (*i)->feedbackMode == interleaved) {
                    interleaved = (*i)->feedbackMode;
                } else {
                    assert(false);
                }
            }
        }
        delete[] varyings;
        assert(interleaved != 0);

        glTransformFeedbackVaryings(programId, feedbackVaryingCount, varyings,
            interleaved == 1 ? GL_INTERLEAVED_ATTRIBS : GL_SEPARATE_ATTRIBS);
    }

    // link everything together
    GLint linked;
    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        // if a link error occured ...
        GLint logLength;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
        if ((Logger::ERROR_LOGGER != NULL) && (logLength > 0)) {
            GLsizei length;
            char *log = new char[logLength];
            glGetProgramInfoLog(programId, GLsizei(logLength), &length, log);
            Logger::ERROR_LOGGER->log("LINKER", string(log));
            delete[] log;
            assert(false);
        }
        glDeleteProgram(programId);
        programId = -1;
        throw exception();
    }

    GLint maxNameLength;
    GLint maxBlockNameLength;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
    glGetProgramiv(programId, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &maxBlockNameLength);
    maxNameLength = max(maxNameLength, maxBlockNameLength);

    char* buf = new char[maxNameLength];

    GLint nUniforms;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &nUniforms);

    std::set<string> newBlocks;

    for (GLuint i = 0; i < (GLuint) nUniforms; ++i) {
        GLsizei length;
        GLint type;
        GLint size;
        GLint blockIndex;
        GLint offset;
        GLint arrayStride;
        GLint matrixStride;
        GLint isRowMajor;
        glGetActiveUniformName(programId, i, GLsizei(maxNameLength), &length, buf);
        glGetActiveUniformsiv(programId, 1, &i, GL_UNIFORM_TYPE, &type);
        glGetActiveUniformsiv(programId, 1, &i, GL_UNIFORM_SIZE, &size);
        glGetActiveUniformsiv(programId, 1, &i, GL_UNIFORM_BLOCK_INDEX, &blockIndex);
        if (blockIndex == -1) {
            offset = glGetUniformLocation(programId, buf);
        } else {
            glGetActiveUniformsiv(programId, 1, &i, GL_UNIFORM_OFFSET, &offset);
        }
        glGetActiveUniformsiv(programId, 1, &i, GL_UNIFORM_ARRAY_STRIDE, &arrayStride);
        glGetActiveUniformsiv(programId, 1, &i, GL_UNIFORM_MATRIX_STRIDE, &matrixStride);
        glGetActiveUniformsiv(programId, 1, &i, GL_UNIFORM_IS_ROW_MAJOR, &isRowMajor);

        string name = string(buf);

        ptr<UniformBlock> b = NULL;
        if (blockIndex != -1) {
            glGetActiveUniformBlockName(programId, GLuint(blockIndex), GLsizei(maxNameLength), &length, buf);
            string blockName = string(buf);

            map<string, ptr<UniformBlock> >::iterator it = uniformBlocks.find(blockName);
            if (it == uniformBlocks.end()) {
                GLint blockSize;
                glGetActiveUniformBlockiv(programId, GLuint(blockIndex), GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
                b = new UniformBlock(this, blockName, GLuint(blockIndex), GLuint(blockSize));
                uniformBlocks.insert(make_pair(blockName, b));
            } else {
                b = it->second;
            }
        }

        UniformBlock *block = b.get();

        for (GLint j = 0; j < size; ++j) {
            ptr<Uniform> u = NULL;

            string uname;
            if (size == 1) {
                uname = name;
            } else {
                ostringstream oss;
                oss << name << '[' << j << ']';
                uname = oss.str();
            }

            GLint uoffset;
            if (block == NULL) {
                uoffset = glGetUniformLocation(programId, uname.c_str());
            } else {
                uoffset = offset + j * arrayStride;
            }

            switch (type) {
            case GL_FLOAT:
                u = new Uniform1f(this, block, uname, GLuint(uoffset));
                break;
            case GL_FLOAT_VEC2:
                u = new Uniform2f(this, block, uname, GLuint(uoffset));
                break;
            case GL_FLOAT_VEC3:
                u = new Uniform3f(this, block, uname, GLuint(uoffset));
                break;
            case GL_FLOAT_VEC4:
                u = new Uniform4f(this, block, uname, GLuint(uoffset));
                break;
            case GL_DOUBLE:
                u = new Uniform1d(this, block, uname, GLuint(uoffset));
                break;
            case GL_DOUBLE_VEC2:
                u = new Uniform2d(this, block, uname, GLuint(uoffset));
                break;
            case GL_DOUBLE_VEC3:
                u = new Uniform3d(this, block, uname, GLuint(uoffset));
                break;
            case GL_DOUBLE_VEC4:
                u = new Uniform4d(this, block, uname, GLuint(uoffset));
                break;
            case GL_INT:
                u = new Uniform1i(this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_VEC2:
                u = new Uniform2i(this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_VEC3:
                u = new Uniform3i(this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_VEC4:
                u = new Uniform4i(this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT:
                u = new Uniform1ui(this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_VEC2:
                u = new Uniform2ui(this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_VEC3:
                u = new Uniform3ui(this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_VEC4:
                u = new Uniform4ui(this, block, uname, GLuint(uoffset));
                break;
            case GL_BOOL:
                u = new Uniform1b(this, block, uname, GLuint(uoffset));
                break;
            case GL_BOOL_VEC2:
                u = new Uniform2b(this, block, uname, GLuint(uoffset));
                break;
            case GL_BOOL_VEC3:
                u = new Uniform3b(this, block, uname, GLuint(uoffset));
                break;
            case GL_BOOL_VEC4:
                u = new Uniform4b(this, block, uname, GLuint(uoffset));
                break;
            case GL_FLOAT_MAT2:
                u = new UniformMatrix2f(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_FLOAT_MAT3:
                u = new UniformMatrix3f(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_FLOAT_MAT4:
                u = new UniformMatrix4f(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_FLOAT_MAT2x3:
                u = new UniformMatrix2x3f(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_FLOAT_MAT2x4:
                u = new UniformMatrix2x4f(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_FLOAT_MAT3x2:
                u = new UniformMatrix3x2f(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_FLOAT_MAT3x4:
                u = new UniformMatrix3x4f(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_FLOAT_MAT4x2:
                u = new UniformMatrix4x2f(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_FLOAT_MAT4x3:
                u = new UniformMatrix4x3f(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_DOUBLE_MAT2:
                u = new UniformMatrix2d(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_DOUBLE_MAT3:
                u = new UniformMatrix3d(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_DOUBLE_MAT4:
                u = new UniformMatrix4d(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_DOUBLE_MAT2x3:
                u = new UniformMatrix2x3d(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_DOUBLE_MAT2x4:
                u = new UniformMatrix2x4d(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_DOUBLE_MAT3x2:
                u = new UniformMatrix3x2d(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_DOUBLE_MAT3x4:
                u = new UniformMatrix3x4d(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_DOUBLE_MAT4x2:
                u = new UniformMatrix4x2d(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_DOUBLE_MAT4x3:
                u = new UniformMatrix4x3d(this, block, uname, GLuint(uoffset), GLuint(matrixStride), int(isRowMajor));
                break;
            case GL_SAMPLER_1D:
            case GL_SAMPLER_1D_SHADOW:
                u = new UniformSampler(SAMPLER_1D, this, block, uname, GLuint(uoffset));
                break;
            case GL_SAMPLER_2D:
            case GL_SAMPLER_2D_SHADOW:
                u = new UniformSampler(SAMPLER_2D, this, block, uname, GLuint(uoffset));
                break;
            case GL_SAMPLER_3D:
                u = new UniformSampler(SAMPLER_3D, this, block, uname, GLuint(uoffset));
                break;
            case GL_SAMPLER_CUBE:
            case GL_SAMPLER_CUBE_SHADOW:
                u = new UniformSampler(SAMPLER_CUBE, this, block, uname, GLuint(uoffset));
                break;
            case GL_SAMPLER_1D_ARRAY:
            case GL_SAMPLER_1D_ARRAY_SHADOW:
                u = new UniformSampler(SAMPLER_1D_ARRAY, this, block, uname, GLuint(uoffset));
                break;
            case GL_SAMPLER_2D_ARRAY:
            case GL_SAMPLER_2D_ARRAY_SHADOW:
                u = new UniformSampler(SAMPLER_2D_ARRAY, this, block, uname, GLuint(uoffset));
                break;
            case GL_SAMPLER_CUBE_MAP_ARRAY:
            case GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW:
                u = new UniformSampler(SAMPLER_CUBE_MAP_ARRAY, this, block, uname, GLuint(uoffset));
                break;
            case GL_SAMPLER_2D_MULTISAMPLE:
                u = new UniformSampler(SAMPLER_2D_MULTISAMPLE, this, block, uname, GLuint(uoffset));
                break;
            case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
                u = new UniformSampler(SAMPLER_2D_MULTISAMPLE_ARRAY, this, block, uname, GLuint(uoffset));
                break;
            case GL_SAMPLER_BUFFER:
                u = new UniformSampler(SAMPLER_BUFFER, this, block, uname, GLuint(uoffset));
                break;
            case GL_SAMPLER_2D_RECT:
            case GL_SAMPLER_2D_RECT_SHADOW:
                u = new UniformSampler(SAMPLER_2D_RECT, this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_SAMPLER_1D:
                u = new UniformSampler(INT_SAMPLER_1D, this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_SAMPLER_2D:
                u = new UniformSampler(INT_SAMPLER_2D, this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_SAMPLER_3D:
                u = new UniformSampler(INT_SAMPLER_3D, this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_SAMPLER_CUBE:
                u = new UniformSampler(INT_SAMPLER_CUBE, this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_SAMPLER_1D_ARRAY:
                u = new UniformSampler(INT_SAMPLER_1D_ARRAY, this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_SAMPLER_2D_ARRAY:
                u = new UniformSampler(INT_SAMPLER_2D_ARRAY, this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_SAMPLER_CUBE_MAP_ARRAY:
                u = new UniformSampler(INT_SAMPLER_CUBE_MAP_ARRAY, this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_SAMPLER_2D_MULTISAMPLE:
                u = new UniformSampler(INT_SAMPLER_2D_MULTISAMPLE, this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
                u = new UniformSampler(INT_SAMPLER_2D_MULTISAMPLE_ARRAY, this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_SAMPLER_BUFFER:
                u = new UniformSampler(INT_SAMPLER_BUFFER, this, block, uname, GLuint(uoffset));
                break;
            case GL_INT_SAMPLER_2D_RECT:
                u = new UniformSampler(INT_SAMPLER_2D_RECT, this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_SAMPLER_1D:
                u = new UniformSampler(UNSIGNED_INT_SAMPLER_1D, this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_SAMPLER_2D:
                u = new UniformSampler(UNSIGNED_INT_SAMPLER_2D, this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_SAMPLER_3D:
                u = new UniformSampler(UNSIGNED_INT_SAMPLER_3D, this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_SAMPLER_CUBE:
                u = new UniformSampler(UNSIGNED_INT_SAMPLER_CUBE, this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
                u = new UniformSampler(UNSIGNED_INT_SAMPLER_1D_ARRAY, this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
                u = new UniformSampler(UNSIGNED_INT_SAMPLER_2D_ARRAY, this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY:
                u = new UniformSampler(UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY, this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
                u = new UniformSampler(UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE, this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
                u = new UniformSampler(UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_SAMPLER_BUFFER:
                u = new UniformSampler(UNSIGNED_INT_SAMPLER_BUFFER, this, block, uname, GLuint(uoffset));
                break;
            case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
                u = new UniformSampler(UNSIGNED_INT_SAMPLER_2D_RECT, this, block, uname, GLuint(uoffset));
                break;
            default:
                assert(false);
                break;
            }

            uniforms.insert(make_pair(uname, u));
            if (b != NULL) {
                b->uniforms.insert(make_pair(uname, u));
            }
            if (u.cast<UniformSampler>() != NULL) {
                uniformSamplers.push_back(u.cast<UniformSampler>());
            }
        }
    }
    delete[] buf;

    // finds GPUBuffer suitable for the blocks used in this Program
    for (map<string, ptr<UniformBlock> >::iterator it = uniformBlocks.begin(); it != uniformBlocks.end(); ++it) {
        ptr<UniformBlock> u = it->second;
        ostringstream oss;
        oss << u->getName() << "-" << u->size << "-" << u->uniforms.size(); //example : deformation-8-32
        ptr<GPUBuffer> buffer = UniformBlock::buffers->get(oss.str());
        if (buffer->getSize() == 0) {
            buffer->setData(u->size, NULL, DYNAMIC_DRAW);
            newBlocks.insert(u->getName());
        }
        u->setBuffer(buffer);
    }

    // sets the initial values of the uniforms
    for (i = this->modules.begin(); i != this->modules.end(); ++i) {
        ptr<Module> module = *i;
        map<string, ptr<Value> >::iterator j = module->initialValues.begin();
        while (j != module->initialValues.end()) {
            ptr<Value> v = j->second;
            ptr<Uniform> u = NULL;

            map<string, ptr<Uniform> >::iterator k = uniforms.find(j->first);
            if (k != uniforms.end()) {
                u = k->second;
            }

            if (u != NULL && u->block != NULL && newBlocks.find(u->block->getName()) == newBlocks.end()) {
                // do not set initial values for uniforms in already existing
                // uniform blocks, to avoid overriding the values of their uniforms
                u = NULL;
            }

            if (u != NULL) {
                ptr<ValueSampler> vs = v.cast<ValueSampler>();
                ptr<UniformSampler> us = u.cast<UniformSampler>();
                assert(u->getName() == v->getName());
                if (u->getType() == v->getType() || (us != NULL && vs != NULL)) {
                    u->setValue(v);
                }
            }
            ++j;
        }
    }
}

Program::~Program()
{
    if (CURRENT == this) {
        CURRENT = NULL;
    }
    if (programId != -1) {
        updateTextureUsers(false);
        updateUniforms(NULL);
    }

    for (unsigned int i = 0; i < modules.size(); ++i) {
        modules[i]->users.erase(this);
    }

    glDeleteProgram(programId);
}

int Program::getId() const
{
    return programId;
}

int Program::getModuleCount() const
{
    return modules.size();
}

ptr<Module> Program::getModule(int index) const
{
    return modules[index];
}

ptr<Uniform> Program::getUniform(const string &name)
{
    map<string, ptr<Uniform> >::iterator i = uniforms.find(name);
    if (i == uniforms.end()) {
//        Logger::WARNING_LOGGER->logf("RENDER", "Missing Uniform %s", name.c_str());
        return NULL;
    }
    return i->second;
}

ptr<UniformBlock> Program::getUniformBlock(const string &name)
{
    map<string, ptr<UniformBlock> >::iterator i = uniformBlocks.find(name);
    if (i == uniformBlocks.end()) {
        return NULL;
    }
    return i->second;
}

void Program::swap(ptr<Program> p)
{
    updateTextureUsers(false);
    p->updateTextureUsers(false);

    updateUniformBlocks(false);
    p->updateUniformBlocks(false);

    std::swap(modules, p->modules);
    std::swap(programId, p->programId);
    std::swap(uniforms, p->uniforms);
    std::swap(uniformBlocks, p->uniformBlocks);

    map<string, ptr<Uniform> >::iterator i = p->uniforms.begin();
    while (i != p->uniforms.end()) {
        ptr<Uniform> u = i->second;
        map<string, ptr<Uniform> >::iterator j = uniforms.find(u->getName());
        if (j != uniforms.end()) {
            ptr<Uniform> pu = j->second;
            if (pu->getType() == u->getType()) {
                std::swap(i->second, j->second);
                std::swap(i->second->location, j->second->location);
                j->second->program = this;
#ifdef ORK_NO_GLPROGRAMUNIFORM
                j->second->dirty = true;
#else
                j->second->setValue();
#endif
            }
        } else {
            // 'u' is no longer an uniform of this program; we store it in the
            // oldUniforms map to reuse this object if this uniform becomes a
            // member of this program again, in future versions
            oldUniforms[u->getName()] = u;
        }
        ++i;
    }

    i = oldUniforms.begin();
    while (i != oldUniforms.end()) {
        ptr<Uniform> oldU = i->second;
        map<string, ptr<Uniform> >::iterator j = uniforms.find(oldU->getName());
        if (j != uniforms.end()) {
            // if an uniform of this program corresponds to an old uniform object,
            // we reuse the old uniform object (so that clients do not have to
            // update their references to the uniforms of this program)
            ptr<Uniform> u = j->second;
            if (u != oldU && u->getType() == oldU->getType()) {
                std::swap(i->second, j->second);
                std::swap(i->second->location, j->second->location);
                j->second->program = this;
#ifdef ORK_NO_GLPROGRAMUNIFORM
                j->second->dirty = true;
#else
                j->second->setValue();
#endif
            }
            oldUniforms.erase(i++);
        } else {
            ++i;
        }
    }

    map<string, ptr<UniformBlock> >::iterator k = uniformBlocks.begin();
    while (k != uniformBlocks.end()) {
        ptr<UniformBlock> b = k->second;
        map<string, ptr<UniformBlock> >::iterator i = p->uniformBlocks.find(b->getName());
        if (i != p->uniformBlocks.end()) {
            ptr<UniformBlock> pb = i->second;
            std::swap(k->second, i->second);
        }
        ++k;
    }

    updateUniforms(this);
    p->updateUniforms(&(*p));

    updateTextureUsers(true);
    p->updateTextureUsers(true);

    if (CURRENT == this) {
        CURRENT = NULL;
    }
}

bool Program::checkSamplers()
{
    for (unsigned int i = 0; i < uniformSamplers.size(); ++i) {
        ptr<UniformSampler> u = uniformSamplers[i];
        if (u->location != -1 && u->get() == NULL) {
            if (Logger::ERROR_LOGGER != NULL) {
                Logger::ERROR_LOGGER->log("OPENGL", "Sampler not bound " + u->getName());
            }
            return false;
        }
    }
    return true;
}

void Program::set()
{
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->log("RENDER", "Switching Program");
    }
    assert(FrameBuffer::getError() == 0);
    if (CURRENT != this) {
        CURRENT = this;
        glUseProgram(programId);

        for (unsigned int i = 0; i < uniformSamplers.size(); ++i) {
            uniformSamplers[i]->setValue();
        }

        map<string, ptr<UniformBlock> >::iterator j = uniformBlocks.begin();
        while (j != uniformBlocks.end()) {
            ptr<UniformBlock> u = j->second;
            GLint unit = u->buffer->bindToUniformBufferUnit(programId);
            assert(unit >= 0);
            glUniformBlockBinding(programId, u->index, GLuint(unit));
            j++;
        }

        assert(FrameBuffer::getError() == 0);
    }

    map<string, ptr<UniformBlock> >::iterator j = uniformBlocks.begin();

    if (Logger::DEBUG_LOGGER != NULL && Logger::DEBUG_LOGGER->hasTopic("RENDER")) {
        ostringstream oss;
        int nBlocks = 0;
        while (j != uniformBlocks.end()) {
            ptr<UniformBlock> u = j->second;
            if (u->isMapped()) {
                oss << j->first.c_str() << ";";
                ++nBlocks;
            }
            ++j;
        }
        if (nBlocks > 0) {
            Logger::DEBUG_LOGGER->logf("RENDER", "Updating %d block(s) [%s]", nBlocks, oss.str().c_str());
        }
        j = uniformBlocks.begin();
    }

    while (j != uniformBlocks.end()) {
        ptr<UniformBlock> u = j->second;
        if (u->isMapped()) {
            u->unmapBuffer();
        }
        j++;
    }

#ifdef ORK_NO_GLPROGRAMUNIFORM
    map<string, ptr<Uniform> >::iterator i = uniforms.begin();
    while (i != uniforms.end()) {
        ptr<Uniform> u = i->second;
        if (u->dirty) {
            u->setValue();
            u->dirty = false;
        }
        i++;
    }
#endif
}

void Program::updateTextureUsers(bool add)
{
    for (unsigned int i = 0; i < uniformSamplers.size(); ++i) {
        ptr<UniformSampler> us = uniformSamplers[i];
        ptr<Texture> t = us->get();
        if (t != NULL) {
            if (add) {
                t->addUser(us->program->getId());
            } else {
                t->removeUser(us->program->getId());
                us->unit = -1;
            }
        }
    }
}

void Program::updateUniformBlocks(bool add)
{
    if (add) {
        map<string, ptr<UniformBlock> >::iterator j = uniformBlocks.begin();
        while (j != uniformBlocks.end()) {
            ptr<UniformBlock> b = j->second;
            map<string, ptr<Uniform> >::iterator i = b->uniforms.begin();
            while (i != b->uniforms.end()) {
                uniforms.insert(make_pair(i->second->getName(), i->second));
                ++i;
            }
            ++j;
        }

    } else {
        map<string, ptr<Uniform> >::iterator i = uniforms.begin();
        while (i != uniforms.end()) {
            if (i->second->block != NULL) {
                uniforms.erase(i++);
            } else {
                ++i;
            }
        }
    }
}

void Program::updateUniforms(Program *owner)
{
    uniformSamplers.clear();

    map<string, ptr<Uniform> >::iterator i = uniforms.begin();
    while (i != uniforms.end()) {
        ptr<Uniform> u = i->second;
        u->program = owner;

        ptr<UniformSampler> us = u.cast<UniformSampler>();
        if (us != NULL) {
            uniformSamplers.push_back(us);
        }
        ++i;
    }

    map<string, ptr<UniformBlock> >::iterator k = uniformBlocks.begin();
    while (k != uniformBlocks.end()) {
        ptr<UniformBlock> b = k->second;
        b->program = owner;
        if (b->buffer != NULL && b->isMapped()) {
            b->unmapBuffer();
        }
        map<string, ptr<Uniform> >::iterator i = b->uniforms.begin();
        while (i != b->uniforms.end()) {
            i->second->program = owner;
            i->second->block = b.get();
            ++i;
        }
        ++k;
    }

    if (owner != NULL) {
        updateUniformBlocks(true);
    }
}

/// @cond RESOURCES

class ProgramResource : public ResourceTemplate<30, Program>
{
public:
    ProgramResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<30, Program>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        vector< ptr<Module> > modules;
        checkParameters(desc, e, "name,");
        const TiXmlNode *n = e->FirstChild();
        while (n != NULL) {
            const TiXmlElement *f = n->ToElement();
            if (f != NULL) {
                if (strcmp(f->Value(), "module") != 0) {
                    if (Logger::ERROR_LOGGER != NULL) {
                        log(Logger::ERROR_LOGGER, desc, f, "Invalid subelement '" + f->ValueStr() + "'");
                    }
                    throw exception();
                }
                checkParameters(desc, f, "name,");
                const char* moduleName = f->Attribute("name");
                if (moduleName == NULL) {
                    if (Logger::ERROR_LOGGER != NULL) {
                        log(Logger::ERROR_LOGGER, desc, f, "Missing 'name' attribute");
                    }
                    throw exception();
                }
                ptr<Module> module;
                try {
                    module = manager->loadResource(string(moduleName)).cast<Module>();
                } catch (...) {
                }
                if (module == NULL) {
                    if (Logger::ERROR_LOGGER != NULL) {
                        log(Logger::ERROR_LOGGER, desc, f, "Cannot find '" + string(moduleName) + "' module");
                    }
                    throw exception();
                }
                modules.push_back(module);
            }
            n = n->NextSibling();
        }
        init(modules);
    }

    virtual bool prepareUpdate()
    {
        bool changed = false;

        if (Resource::prepareUpdate()) {
            changed = true;
        } else if (manager != NULL) {
            for (unsigned int i = 0; i < modules.size(); ++i) {
                if (dynamic_cast<Resource*>(&(*modules[i]))->changed()) {
                    changed = true;
                    break;
                }
            }
        }

        if (changed) {
            oldValue = NULL;
            try {
                oldValue = new ProgramResource(manager, name, newDesc == NULL ? desc : newDesc);
            } catch (...) {
            }
            if (oldValue != NULL) {
                swap(oldValue);
                return true;
            }
            return false;
        }
        return true;
    }
};

extern const char program[] = "program";

static ResourceFactory::Type<program, ProgramResource> ProgramType;

/// @endcond

}
