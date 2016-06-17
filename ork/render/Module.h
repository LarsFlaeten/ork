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

#ifndef _ORK_MODULE_H_
#define _ORK_MODULE_H_

#include <map>
#include <set>
#include <string>

#include "ork/core/Iterator.h"
#include "ork/render/AttributeBuffer.h"
#include "ork/render/Texture.h"
#include "ork/render/Value.h"

using namespace std;

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
    const set<Program*> &getUsers() const;

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
    void addFeedbackVarying(const string &name);

    /**
     * Adds an initial value for the given uniform variable.
     *
     * @param value an initial value for an uniform of this module.
     */
    void addInitialValue(ptr<Value> value);

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
    set<Program*> users;

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
    vector<string> feedbackVaryings;

    /**
     * The initial values for the uniforms of the shaders of this module.
     */
    map<string, ptr<Value> > initialValues;

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
