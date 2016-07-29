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

#ifndef _ORK_MODULE_H_
#define _ORK_MODULE_H_

#include <map>
#include <set>
#include <string>

#include "ork/core/Iterator.h"
#include "ork/render/AttributeBuffer.h"
#include "ork/render/Texture.h"
#include "ork/render/Value.h"

namespace ork
{

class Program;

/**
 * A module made of a vertex, a tesselation, a geometry, and a fragment shader
 * parts. All parts are optional. These parts must be defined either each in its
 * own GLSL compilation unit, or all grouped in a single compilation unit but
 * separated with the following preprocessor directives:
@verbatim
 ... common code ...
#ifdef _VERTEX_
 ... vertex shader code ...
#endif
#ifdef _TESS_CONTROL_
 ... tessellation control shader code ...
#endif
#ifdef _TESS_EVAL_
 ... tessellation evaluation shader code ...
#endif
#ifdef _GEOMETRY_
 ... geometry shader code ...
#endif
#ifdef _FRAGMENT_
 ... fragment shader code ...
#endif
@endverbatim
 * A module can specify some initial values for its uniform variables, and
 * can also specify which output varying variable must be recorded in transform
 * feedback mode.
 *
 * @ingroup render
 */
class ORK_API Module : public Object
{
public:
    /**
     * Creates a new module whose parts are all grouped in a single compilation
     * unit, but separated with preprocessor directives (see #Module).
     *
     * @param version the GLSL version used for the source code.
     * @param source the source code of the module.
     */
    Module(int version, const char* source);

    /**
     * Creates a new module whose parts are defined in separate compilation units.
     *
     * @param version the GLSL version used for the source code.
     * @param vertex the vertex shader source code (maybe NULL).
     * @param fragment the fragment shader source code (maybe NULL).
     */
    Module(int version, const char* vertex, const char* fragment);

    /**
     * Creates a new module whose parts are defined in separate compilation units.
     *
     * @param version the GLSL version used for the source code.
     * @param vertex the vertex shader source code (maybe NULL).
     * @param geometry the geometry shader source code (maybe NULL).
     * @param fragment the fragment shader source code (maybe NULL).
     */
    Module(int version, const char* vertex, const char* geometry, const char* fragment);

    /**
     * Creates a new module whose parts are defined in separate compilation units.
     *
     * @param version the GLSL version used for the source code.
     * @param vertex the vertex shader source code (maybe NULL).
     * @param tessControl the tessellation control shader source code (maybe NULL).
     * @param tessEvaluation the tessellation evaluation shader source code (maybe NULL).
     * @param geometry the geometry shader source code (maybe NULL).
     * @param fragment the fragment shader source code (maybe NULL).
     */
    Module(int version, const char* vertex, const char *tessControl,
        const char* tessEvaluation, const char* geometry, const char* fragment);

    /**
     * Deletes this module.
     */
    virtual ~Module();

    /**
     * Returns the id of the vertex shader part of this module.
     *
     * @return the id of the vertex shader part of this module, or -1
     *       if this module does not have a vertex shader.
     */
    int getVertexShaderId() const;

    /**
     * Returns the id of the tessellation control shader part of this shader.
     */
    int getTessControlShaderId() const;

    /**
     * Returns the id of the tessellation evaluation shader part of this shader.
     */
    int getTessEvalShaderId() const;

    /**
     * Returns the id of the geometry shader part of this shader.
     */
    int getGeometryShaderId() const;

    /**
     * Returns the id of the fragment shader part of this shader.
     */
    int getFragmentShaderId() const;

    /**
     * Returns the programs that use this Module.
     */
    const std::set<Program*> &getUsers() const;

    /**
     * Sets the format to use when a Program using this module is
     * used in transform feedback.
     *
     * @param interleaved true to interleave the recorded output varying
     *      variables in a single buffer, or false to record each output
     *      variable in a separate buffer.
     */
    void setFeedbackMode(bool interleaved);

    /**
     * Adds an output varying variable that must be recorded in transform
     * feedback mode. The order of these variables is important: they are
     * recorded in the same order as they as declared with this method.
     *
     * @param name the name of an output varying variable to record.
     */
    void addFeedbackVarying(const std::string &name);

    /**
     * Adds an initial value for the given uniform variable.
     *
     * @param value an initial value for an uniform of this module.
     */
    void addInitialValue(ptr<Value> value);

    /**
     * Utility function, allows to add a define statement in
     * the start of an allready expressed shader source.
     * The original shader source is not modified, new shader
     * source with define appended is given in newShader
     * @param origShader the original shader source
     * @param defineClause the define statement to be included. The "#define"
     * keyword shall not be included
     * @param newShader reference to string where new shader source will
     * be stored
     */
    static void addDefine(const char* origShader, const char* defineClause,
        std::string& newShader);


protected:
    /**
     * Creates an uninitialized module.
     */
    Module();

    /**
     * Initializes this module, with parts that are all grouped in a single
     * compilation unit, but separated with preprocessor directives (see #Module).
     *
     * @param version the GLSL version used for the source code.
     * @param source the source code of the module.
     */
    void init(int version, const char* source);

    /**
     * Initializes this module.
     *
     * @param version the GLSL version used for the source code.
     * @param vertexHeader an optional header for the the vertex shader source
     *      code (maybe NULL).
     * @param vertex the vertex shader source code (maybe NULL).
     * @param tessControlHeader an optional header for the the tessellation
     *      control shader source code (maybe NULL).
     * @param tessControl the tessellation control shader source code (maybe NULL).
     * @param tessEvaluationHeader an optional header for the the tessellation
     *      evaluation shader source code (maybe NULL).
     * @param tessEvaluation the tessellation evaluation shader source code (maybe NULL).
     * @param geometryHeader an optional header for the the geometry shader
     *      source code (maybe NULL).
     * @param geometry the geometry shader source code (maybe NULL).
     * @param fragmentHeader an optional header for the the fragment shader
     *      source code (maybe NULL).
     * @param fragment the fragment shader source code (maybe NULL).
     */
    void init(int version,
        const char* vertexHeader, const char* vertex,
        const char* tessControlHeader, const char *tessControl,
        const char* tessEvaluationHeader, const char* tessEvaluation,
        const char* geometryHeader, const char* geometry,
        const char* fragmentHeader, const char* fragment);

    /**
     * Swaps this module with the given one.
     */
    virtual void swap(ptr<Module> s);

private:
    /**
     * The Program instances that use this Module.
     */
    std::set<Program*> users;

    /**
     * The id of the vertex shader part of this shader.
     */
    int vertexShaderId;

    /**
     * The id of the tessellation control shader part of this shader.
     */
    int tessControlShaderId;

    /**
     * The id of the tessellation evaluation shader part of this shader.
     */
    int tessEvalShaderId;

    /**
     * The id of the geometry shader part of this shader.
     */
    int geometryShaderId;

    /**
     * The id of the fragment shader part of this shader.
     */
    int fragmentShaderId;

    /**
     * The transform feedback mode to use with this module.
     * 0 means 'any mode', 1 means 'interleaved attribs', 2 means 'separate attribs'.
     */
    int feedbackMode;

    /**
     * The output varying variables of this module that must be recorded in
     * transform feedback mode.
     */
    std::vector<std::string> feedbackVaryings;

    /**
     * The initial values for the uniforms of the shaders of this module.
     */
    std::map<std::string, ptr<Value> > initialValues;

    /**
     * Checks if a shader part has been correctly compiled.
     *
     * @param shaderId the id the shader part to check.
     */
    bool check(int shaderId);

    /**
     * Logs the shader compiler output.
     *
     * @param shaderId the id of the vertex, geometry or fragment part.
     * @param nlines number of lines in the compiler output text.
     * @param lines the compiler output text.
     * @param error true if the compiler found some errors.
     */
    void printLog(int shaderId, int nlines, const char** lines, bool error);

    friend class Program;
};

}

#endif
