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

#ifndef _ORK_PROGRAM_H_
#define _ORK_PROGRAM_H_

#include "ork/render/Module.h"
#include "ork/render/Uniform.h"

namespace ork
{

class FrameBuffer;

/**
 * A GPU program. A GPU program can define vertex, tessellation, geometry and
 * fragment programs. It is made of one or more Module, themselves made of one
 * or more GLSL shaders.
 *
 * @ingroup render
 */
class ORK_API Program : public Object
{
public:
    /**
     * Creates a new program.
     *
     * @param modules the modules that will compose this program.
     * @param separable true to enable the separate use of the shaders of
     *      this program (see #Program(ptr<Program>, ptr<Program>, ptr<Program>, ptr<Program>, ptr<Program>)).
     */
    Program(const std::vector< ptr<Module> > &modules, bool separable = false);

    /**
     * Creates a new program.
     *
     * @param module the single module that will compose this program.
     * @param separable true to enable the separate use of the shaders of
     *      this program (see #Program(ptr<Program>, ptr<Program>, ptr<Program>, ptr<Program>, ptr<Program>)).
     */
    Program(ptr<Module> module, bool separable = false);

    /**
     * Creates a new program from a compiled representation.
     *
     * @param format the format of the compiled reprensentation.
     * @param length the length of the 'binary' array.
     * @param binary the compiled program code.
     * @param separable true to enable the separate use of the shaders of
     *      this program (see #Program(ptr<Program>, ptr<Program>, ptr<Program>, ptr<Program>, ptr<Program>)).
     */
    Program(GLenum format, GLsizei length, unsigned char *binary, bool separable = false);

    /**
     * Creates a program from individual shaders in separable programs.
     *
     * @param vertex the program whose vertex shader must be used.
     * @param tessControl the program whose tessellation control shader must be used.
     * @param tessEval the program whose tessellation evaluation shader must be used.
     * @param geometry the program whose geometry shader must be used.
     * @param fragment the program whose fragment shader must be used.
     */
    Program(ptr<Program> vertex, ptr<Program> tessControl, ptr<Program> tessEval, ptr<Program> geometry, ptr<Program> fragment);

    /**
     * Deletes this program.
     */
    virtual ~Program();

    /**
     * Returns the id of this program.
     */
    virtual int getId() const;

    /**
     * Returns the number of Module objects in this program.
     */
    int getModuleCount() const;

    /**
     * Returns the Module of this program whose index is given.
     */
    ptr<Module> getModule(int index) const;

    /**
     * Returns the uniforms of this program.
     */
    std::vector< ptr<Uniform> > getUniforms() const;

    /**
     * Returns the uniform of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    ptr<Uniform> getUniform(const std::string &name);

    /**
     * Returns the uniform1f of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform1f> getUniform1f(const std::string &name);

    /**
     * Returns the uniform1d of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform1d> getUniform1d(const std::string &name);

    /**
     * Returns the uniform1i of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform1i> getUniform1i(const std::string &name);

    /**
     * Returns the uniform1ui of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform1ui> getUniform1ui(const std::string &name);

    /**
     * Returns the uniform1b of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform1b> getUniform1b(const std::string &name);

    /**
     * Returns the uniform2f of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform2f> getUniform2f(const std::string &name);

    /**
     * Returns the uniform2d of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform2d> getUniform2d(const std::string &name);

    /**
     * Returns the uniform2i of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform2i> getUniform2i(const std::string &name);

    /**
     * Returns the uniform2ui of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform2ui> getUniform2ui(const std::string &name);

    /**
     * Returns the uniform2b of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform2b> getUniform2b(const std::string &name);

    /**
     * Returns the uniform3f of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform3f> getUniform3f(const std::string &name);

    /**
     * Returns the uniform3d of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform3d> getUniform3d(const std::string &name);

    /**
     * Returns the uniform3i of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform3i> getUniform3i(const std::string &name);

    /**
     * Returns the uniform3ui of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform3ui> getUniform3ui(const std::string &name);

    /**
     * Returns the uniform3b of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform3b> getUniform3b(const std::string &name);

    /**
     * Returns the uniform4f of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform4f> getUniform4f(const std::string &name);

    /**
     * Returns the uniform4d of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform4d> getUniform4d(const std::string &name);

    /**
     * Returns the uniform4i of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform4i> getUniform4i(const std::string &name);

    /**
     * Returns the uniform4ui of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform4ui> getUniform4ui(const std::string &name);

    /**
     * Returns the uniform4b of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform4b> getUniform4b(const std::string &name);

    /**
     * Returns the uniformMatrix2f of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix2f> getUniformMatrix2f(const std::string &name);

    /**
     * Returns the uniformMatrix2d of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix2d> getUniformMatrix2d(const std::string &name);

    /**
     * Returns the uniformMatrix3f of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix3f> getUniformMatrix3f(const std::string &name);

    /**
     * Returns the uniformMatrix3d of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix3d> getUniformMatrix3d(const std::string &name);

    /**
     * Returns the uniformMatrix4f of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix4f> getUniformMatrix4f(const std::string &name);

    /**
     * Returns the uniformMatrix4d of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix4d> getUniformMatrix4d(const std::string &name);

    /**
     * Returns the uniformMatrix2x3f of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix2x3f> getUniformMatrix2x3f(const std::string &name);

    /**
     * Returns the uniformMatrix2x3d of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix2x3d> getUniformMatrix2x3d(const std::string &name);

    /**
     * Returns the uniformMatrix2x4f of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix2x4f> getUniformMatrix2x4f(const std::string &name);

    /**
     * Returns the uniformMatrix2x4d of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix2x4d> getUniformMatrix2x4d(const std::string &name);

    /**
     * Returns the uniformMatrix3x2f of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix3x2f> getUniformMatrix3x2f(const std::string &name);

    /**
     * Returns the uniformMatrix3x2d of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix3x2d> getUniformMatrix3x2d(const std::string &name);

    /**
     * Returns the uniformMatrix3x4f of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix3x4f> getUniformMatrix3x4f(const std::string &name);

    /**
     * Returns the uniformMatrix3x4d of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix3x4d> getUniformMatrix3x4d(const std::string &name);

    /**
     * Returns the uniformMatrix4x2f of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix4x2f> getUniformMatrix4x2f(const std::string &name);

    /**
     * Returns the uniformMatrix4x2d of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix4x2d> getUniformMatrix4x2d(const std::string &name);

    /**
     * Returns the uniformMatrix4x3f of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix4x3f> getUniformMatrix4x3f(const std::string &name);

    /**
     * Returns the uniformMatrix4x2d of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix4x3d> getUniformMatrix4x3d(const std::string &name);

    /**
     * Returns the uniform sampler of this program whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformSampler> getUniformSampler(const std::string &name);

    /**
     * Returns the uniform subroutine of this program whose name is given.
     *
     * @param stage a shader type (vertex, fragment, etc).
     * @param name a GLSL uniform subroutine name.
     * @return the uniform of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformSubroutine> getUniformSubroutine(Stage stage, const std::string &name);

    /**
     * Returns the uniform block of this program whose name is given.
     *
     * @param name a GLSL uniform block name.
     * @return the uniform block of this program whose name is given,
     *       or NULL if there is no such uniform.
     */
    ptr<UniformBlock> getUniformBlock(const std::string &name);

    /**
     * Returns a compiled version of this program.
     *
     * @param[out] length the length of the returned array.
     * @param[out] format the format of the returned array.
     * @return a compiled version of this program.
     */
    unsigned char *getBinary(GLsizei &length, GLenum &format);

protected:
    /**
     * The modules of this program.
     */
    std::vector< ptr<Module> > modules;

    /**
     * Creates an unitialized program.
     */
    Program();

    /**
     * Initializes this program.
     *
     * @param modules the modules that will compose this program.
     * @param separable true to enable the separate use of the shaders of
     *      this program (see #Program(ptr<Program>, ptr<Program>, ptr<Program>, ptr<Program>, ptr<Program>)).
     */
    void init(const std::vector< ptr<Module> > &modules, bool separable);

    /**
     * Initializes this program from a compiled representation.
     *
     * @param format the format of the compiled reprensentation.
     * @param length the length of the 'binary' array.
     * @param binary the compiled program code.
     * @param separable true to enable the separate use of the shaders of
     *      this program (see #Program(ptr<Program>, ptr<Program>, ptr<Program>, ptr<Program>, ptr<Program>)).
     */
    void init(GLenum format, GLsizei length, unsigned char *binary, bool separable);

    /**
     * Initializes the given stage of this pipeline object.
     *
     * @param s a shader type (vertex, fragment, etc).
     * @param p the program to use for this stage in this pipelne object.
     */
    void init(Stage s, ptr<Program> p);

    /**
     * Initializes the uniforms of this program.
     */
    void initUniforms();

    /**
     * Swaps this program with the given one.
     */
    virtual void swap(ptr<Program> t);

private:
    /**
     * The id of this program.
     */
    GLuint programId;

    /**
     * The pipeline object id of this program, if applicable.
     */
    GLuint pipelineId;

    /**
     * The programs used in this pipeline object.
     */
    std::vector<GLuint> programIds;

    /**
     * The programs whose shaders are used in this pipeline object.
     */
    std::vector< ptr<Program> > pipelinePrograms;

    /**
     * The pipeline stages for which #pipelinePrograms are used.
     */
    std::vector<int> pipelineStages;

    /**
     * The uniforms of this program. This includes all uniforms,
     * whether outside or inside a uniform block, including uniform samplers
     * and uniform subroutines.
     */
    std::map<std::string, ptr<Uniform> > uniforms;

    /**
     * The uniforms of this program that were present in old versions,
     * but are no longer present in the current one.
     */
    std::map<std::string, ptr<Uniform> > oldUniforms;

    /**
     * The uniform samplers of this program.
     */
    std::vector< ptr<UniformSampler> > uniformSamplers;

    /**
     * The values of the uniform subroutines of this program.
     */
    GLuint** uniformSubroutines;

    /**
     * Bitfield indicating the program stages in which at least one
     * uniform subroutines has changed.
     */
    int dirtyStages;

    /**
     * The uniform blocks of this program.
     */
    std::map<std::string, ptr<UniformBlock> > uniformBlocks;

    /**
     * The program currently in use.
     */
    static Program *CURRENT;

    /**
     * Checks that each active program sampler is bound to a texture.
     *
     * @return true if all samplers are bound.
     */
    bool checkSamplers();

    /**
     * Sets this program as the current program.
     */
    void set();

    /**
     * Binds the textures and uniform blocks of this program to available units.
     */
    void bindTexturesAndUniformBlocks();

    /**
     * Updates the value of the uniforms of this program. This method unmaps the
     * buffers of the uniform blocks, updates the uniform subroutines, and
     * optionally updates the value of the "regular" uniforms whose value has
     * changed since the last time this program was used (this only happens with
     * the ORK_NO_GLPROGRAMUNIFORM preprocessor option).
     *
     * @param stages a bitfield indicating the stages whose uniform subroutines
     *      must be updated.
     */
    void updateDirtyUniforms(int stages);

    /**
     * Adds or removes this program as a user of the textures bound to
     * the uniform samplers of this program.
     */
    void updateTextureUsers(bool add);

    /**
     * Adds to or removes from #uniforms the uniforms that are inside
     * unifom blocks.
     */
    void updateUniformBlocks(bool add);

    /**
     * Sets the owner program of the uniforms and uniform blocks of this
     * program to the given value.
     *
     * @param owner the new owner for the uniforms of this program.
     */
    void updateUniforms(Program *owner);

    /**
     * Returns true if this program is the current one, or is part of the
     * current pipeline object.
     */
    bool isCurrent() const;

    friend class Uniform;

    friend class UniformSampler;

    friend class UniformSubroutine;

    friend class Texture;

    friend class MeshBuffers;

    friend class TransformFeedback;

    friend class FrameBuffer;
};

inline ptr<Uniform1f> Program::getUniform1f(const std::string &name)
{
    return getUniform(name).cast<Uniform1f>();
}

inline ptr<Uniform1d> Program::getUniform1d(const std::string &name)
{
    return getUniform(name).cast<Uniform1d>();
}

inline ptr<Uniform1i> Program::getUniform1i(const std::string &name)
{
    return getUniform(name).cast<Uniform1i>();
}

inline ptr<Uniform1ui> Program::getUniform1ui(const std::string &name)
{
    return getUniform(name).cast<Uniform1ui>();
}

inline ptr<Uniform1b> Program::getUniform1b(const std::string &name)
{
    return getUniform(name).cast<Uniform1b>();
}

inline ptr<Uniform2f> Program::getUniform2f(const std::string &name)
{
    return getUniform(name).cast<Uniform2f>();
}

inline ptr<Uniform2d> Program::getUniform2d(const std::string &name)
{
    return getUniform(name).cast<Uniform2d>();
}

inline ptr<Uniform2i> Program::getUniform2i(const std::string &name)
{
    return getUniform(name).cast<Uniform2i>();
}

inline ptr<Uniform2ui> Program::getUniform2ui(const std::string &name)
{
    return getUniform(name).cast<Uniform2ui>();
}

inline ptr<Uniform2b> Program::getUniform2b(const std::string &name)
{
    return getUniform(name).cast<Uniform2b>();
}

inline ptr<Uniform3f> Program::getUniform3f(const std::string &name)
{
    return getUniform(name).cast<Uniform3f>();
}

inline ptr<Uniform3d> Program::getUniform3d(const std::string &name)
{
    return getUniform(name).cast<Uniform3d>();
}

inline ptr<Uniform3i> Program::getUniform3i(const std::string &name)
{
    return getUniform(name).cast<Uniform3i>();
}

inline ptr<Uniform3ui> Program::getUniform3ui(const std::string &name)
{
    return getUniform(name).cast<Uniform3ui>();
}

inline ptr<Uniform3b> Program::getUniform3b(const std::string &name)
{
    return getUniform(name).cast<Uniform3b>();
}

inline ptr<Uniform4f> Program::getUniform4f(const std::string &name)
{
    return getUniform(name).cast<Uniform4f>();
}

inline ptr<Uniform4d> Program::getUniform4d(const std::string &name)
{
    return getUniform(name).cast<Uniform4d>();
}

inline ptr<Uniform4i> Program::getUniform4i(const std::string &name)
{
    return getUniform(name).cast<Uniform4i>();
}

inline ptr<Uniform4ui> Program::getUniform4ui(const std::string &name)
{
    return getUniform(name).cast<Uniform4ui>();
}

inline ptr<Uniform4b> Program::getUniform4b(const std::string &name)
{
    return getUniform(name).cast<Uniform4b>();
}

inline ptr<UniformMatrix2f> Program::getUniformMatrix2f(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix2f>();
}

inline ptr<UniformMatrix2d> Program::getUniformMatrix2d(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix2d>();
}

inline ptr<UniformMatrix3f> Program::getUniformMatrix3f(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix3f>();
}

inline ptr<UniformMatrix3d> Program::getUniformMatrix3d(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix3d>();
}

inline ptr<UniformMatrix4f> Program::getUniformMatrix4f(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix4f>();
}

inline ptr<UniformMatrix4d> Program::getUniformMatrix4d(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix4d>();
}

inline ptr<UniformMatrix2x3f> Program::getUniformMatrix2x3f(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix2x3f>();
}

inline ptr<UniformMatrix2x3d> Program::getUniformMatrix2x3d(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix2x3d>();
}

inline ptr<UniformMatrix2x4f> Program::getUniformMatrix2x4f(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix2x4f>();
}

inline ptr<UniformMatrix2x4d> Program::getUniformMatrix2x4d(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix2x4d>();
}

inline ptr<UniformMatrix3x2f> Program::getUniformMatrix3x2f(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix3x2f>();
}

inline ptr<UniformMatrix3x2d> Program::getUniformMatrix3x2d(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix3x2d>();
}

inline ptr<UniformMatrix3x4f> Program::getUniformMatrix3x4f(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix3x4f>();
}

inline ptr<UniformMatrix3x4d> Program::getUniformMatrix3x4d(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix3x4d>();
}

inline ptr<UniformMatrix4x2f> Program::getUniformMatrix4x2f(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix4x2f>();
}

inline ptr<UniformMatrix4x2d> Program::getUniformMatrix4x2d(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix4x2d>();
}

inline ptr<UniformMatrix4x3f> Program::getUniformMatrix4x3f(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix4x3f>();
}

inline ptr<UniformMatrix4x3d> Program::getUniformMatrix4x3d(const std::string &name)
{
    return getUniform(name).cast<UniformMatrix4x3d>();
}

inline ptr<UniformSampler> Program::getUniformSampler(const std::string &name)
{
    return getUniform(name).cast<UniformSampler>();
}

inline ptr<UniformSubroutine> Program::getUniformSubroutine(Stage stage, const std::string &name)
{
    switch (stage) {
    case VERTEX:
        return getUniform("VERTEX " + name).cast<UniformSubroutine>();
    case TESSELATION_CONTROL:
        return getUniform("TESS_CONTROL " + name).cast<UniformSubroutine>();
    case TESSELATION_EVALUATION:
        return getUniform("TESS_EVAL " + name).cast<UniformSubroutine>();
    case GEOMETRY:
        return getUniform("GEOMETRY " + name).cast<UniformSubroutine>();
    case FRAGMENT:
        return getUniform("FRAGMENT " + name).cast<UniformSubroutine>();
    }
    throw std::exception();
}

}

#endif
