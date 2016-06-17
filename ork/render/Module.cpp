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

#include "ork/render/Module.h"

#include <sstream>
#include <GL/glew.h>

#include "ork/math/mat2.h"
#include "ork/resource/ResourceTemplate.h"
#include "ork/render/FrameBuffer.h"

namespace ork
{

Module::Module() : Object("Module")
{
}

Module::Module(int version, const char *source) : Object("Module")
{
    init(version, source);
}

Module::Module(int version, const char* vertex, const char* fragment) : Object("Module")
{
    init(version, NULL, vertex, NULL, NULL, NULL, NULL, NULL, NULL, NULL, fragment);
}

Module::Module(int version, const char* vertex, const char* geometry, const char* fragment) : Object("Module")
{
    init(version, NULL, vertex, NULL, NULL, NULL, NULL, NULL, geometry, NULL, fragment);
}

Module::Module(int version, const char* vertex, const char *tessControl, const char* tessEvaluation,
    const char* geometry, const char* fragment) : Object("Module")
{
    init(version, NULL, vertex, NULL, tessControl, NULL, tessEvaluation, NULL, geometry, NULL, fragment);
}

void Module::init(int version, const char *source)
{
    init(version,
        "#define _VERTEX_\n", strstr(source, "_VERTEX_") != NULL ? source : NULL,
        "#define _TESS_CONTROL_\n", strstr(source, "_TESS_CONTROL_") != NULL ? source : NULL,
        "#define _TESS_EVAL_\n", strstr(source, "_TESS_EVAL_") != NULL ? source : NULL,
        "#define _GEOMETRY_\n", strstr(source, "_GEOMETRY_") != NULL ? source : NULL,
        "#define _FRAGMENT_\n", strstr(source, "_FRAGMENT_") != NULL ? source : NULL);
}

void Module::init(int version,
    const char* vertexHeader, const char* vertex,
    const char* tessControlHeader, const char *tessControl,
    const char* tessEvaluationHeader, const char* tessEvaluation,
    const char* geometryHeader, const char* geometry,
    const char* fragmentHeader, const char* fragment)
{
    int lineCount;
    const char* lines[3];

    ostringstream oss;
    oss << "#version " << version << "\n";
	string versionLine = oss.str();

    lines[0] = versionLine.c_str();

    bool error;

    GLint glVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion);

    //compiles and checks the vertex shader part
    if (vertex != NULL) {
        lineCount = vertexHeader != NULL ? 3 : 2;
        lines[1] = lineCount == 3 ? vertexHeader : vertex;
        lines[2] = vertex;
        vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderId, lineCount, lines, NULL);
        glCompileShader(vertexShaderId);
        error = !check(vertexShaderId);
        printLog(vertexShaderId, lineCount, lines, error);
        if (error) {
            // deletes already allocated objects
            glDeleteShader(vertexShaderId);
            vertexShaderId = -1;
            assert(FrameBuffer::getError() == 0);
            throw exception();
        }
    } else {
        vertexShaderId = -1;
    }

    //compiles and checks the tessellation control shader part
    if (tessControl != NULL && glVersion >= 4) {
        lineCount = tessControlHeader != NULL ? 3 : 2;
        lines[1] = lineCount == 3 ? tessControlHeader : tessControl;
        lines[2] = tessControl;
        tessControlShaderId = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tessControlShaderId, lineCount, lines, NULL);
        glCompileShader(tessControlShaderId);
        error = !check(tessControlShaderId);
        printLog(tessControlShaderId, lineCount, lines, error);
        if (error) {
            // deletes already allocated objects
            if (vertexShaderId != -1) {
                glDeleteShader(vertexShaderId);
                vertexShaderId = -1;
            }
            glDeleteShader(tessControlShaderId);
            tessControlShaderId = -1;
            assert(FrameBuffer::getError() == 0);
            throw exception();
        }
    } else {
        tessControlShaderId = -1;
    }

    //compiles and checks the tessellation evaluation shader part
    if (tessEvaluation != NULL && glVersion >= 4) {
        lineCount = tessEvaluationHeader != NULL ? 3 : 2;
        lines[1] = lineCount == 3 ? tessEvaluationHeader : tessEvaluation;
        lines[2] = tessEvaluation;
        tessEvalShaderId = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(tessEvalShaderId, lineCount, lines, NULL);
        glCompileShader(tessEvalShaderId);
        error = !check(tessEvalShaderId);
        printLog(tessEvalShaderId, lineCount, lines, error);
        if (error) {
            // deletes already allocated objects
            if (vertexShaderId != -1) {
                glDeleteShader(vertexShaderId);
                vertexShaderId = -1;
            }
            if (tessControlShaderId != -1) {
                glDeleteShader(tessControlShaderId);
                geometryShaderId = -1;
            }
            glDeleteShader(tessEvalShaderId);
            tessEvalShaderId = -1;
            assert(FrameBuffer::getError() == 0);
            throw exception();
        }
    } else {
        tessEvalShaderId = -1;
    }

    //compiles and checks the geometry shader part
    if (geometry != NULL) {
        lineCount = geometryHeader != NULL ? 3 : 2;
        lines[1] = lineCount == 3 ? geometryHeader : geometry;
        lines[2] = geometry;
        geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShaderId, lineCount, lines, NULL);
        glCompileShader(geometryShaderId);
        error = !check(geometryShaderId);
        printLog(geometryShaderId, lineCount, lines, error);
        if (error) {
            // deletes already allocated objects
            if (vertexShaderId != -1) {
                glDeleteShader(vertexShaderId);
                vertexShaderId = -1;
            }
            if (tessControlShaderId != -1) {
                glDeleteShader(tessControlShaderId);
                geometryShaderId = -1;
            }
            if (tessEvalShaderId != -1) {
                glDeleteShader(tessEvalShaderId);
                tessEvalShaderId = -1;
            }
            glDeleteShader(geometryShaderId);
            geometryShaderId = -1;
            assert(FrameBuffer::getError() == 0);
            throw exception();
        }
    } else {
        geometryShaderId = -1;
    }

    //and finally compiles and checks the fragment shader part
    if (fragment != NULL) {
        lineCount = fragmentHeader != NULL ? 3 : 2;
        lines[1] = lineCount == 3 ? fragmentHeader : fragment;
        lines[2] = fragment;
        fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderId, lineCount, lines, NULL);
        glCompileShader(fragmentShaderId);
        error = !check(fragmentShaderId);
        printLog(fragmentShaderId, lineCount, lines, error);
        if (error) {
            // deletes already allocated objects
            if (vertexShaderId != -1) {
                glDeleteShader(vertexShaderId);
                vertexShaderId = -1;
            }
            if (tessControlShaderId != -1) {
                glDeleteShader(tessControlShaderId);
                geometryShaderId = -1;
            }
            if (tessEvalShaderId != -1) {
                glDeleteShader(tessEvalShaderId);
                tessEvalShaderId = -1;
            }
            if (geometryShaderId != -1) {
                glDeleteShader(geometryShaderId);
                geometryShaderId = -1;
            }
            glDeleteShader(fragmentShaderId);
            fragmentShaderId = -1;
            assert(FrameBuffer::getError() == 0);
            throw exception();
        }
    } else {
        fragmentShaderId = -1;
    }

    if (glGetError() != 0) {
        assert(false);
        throw exception();
    }

    feedbackMode = 0;
}

Module::~Module()
{
    if (vertexShaderId != -1) {
        glDeleteShader(vertexShaderId);
    }
    if (tessControlShaderId != -1) {
        glDeleteShader(tessControlShaderId);
    }
    if (tessEvalShaderId != -1) {
        glDeleteShader(tessEvalShaderId);
    }
    if (geometryShaderId != -1) {
        glDeleteShader(geometryShaderId);
    }
    if (fragmentShaderId != -1) {
        glDeleteShader(fragmentShaderId);
    }
    assert(FrameBuffer::getError() == 0);

    users.clear();
    feedbackVaryings.clear();
    initialValues.clear();
}

int Module::getVertexShaderId() const
{
    return vertexShaderId;
}

int Module::getTessControlShaderId() const
{
    return tessControlShaderId;
}

int Module::getTessEvalShaderId() const
{
    return tessEvalShaderId;
}

int Module::getGeometryShaderId() const
{
    return geometryShaderId;
}

int Module::getFragmentShaderId() const
{
    return fragmentShaderId;
}

const set<Program*> &Module::getUsers() const
{
    return users;
}

void Module::setFeedbackMode(bool interleaved)
{
    feedbackMode = interleaved ? 1 : 2;
}

void Module::addFeedbackVarying(const string &name)
{
    feedbackVaryings.push_back(name);
}

void Module::addInitialValue(ptr<Value> value)
{
    initialValues.insert(make_pair(value->getName(), value));
}

void Module::swap(ptr<Module> s)
{
    std::swap(vertexShaderId, s->vertexShaderId);
    std::swap(tessControlShaderId, s->tessControlShaderId);
    std::swap(tessEvalShaderId, s->tessEvalShaderId);
    std::swap(geometryShaderId, s->geometryShaderId);
    std::swap(fragmentShaderId, s->fragmentShaderId);
    std::swap(initialValues, s->initialValues);
}

bool Module::check(int shaderId)
{
    GLint compiled;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
    return compiled != 0;
}

void Module::printLog(int shaderId, int nlines, const char** lines, bool error)
{
    GLint logLength;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
    ptr<Logger> logger = error ? Logger::ERROR_LOGGER : Logger::WARNING_LOGGER;
    if (logger != NULL && logLength > 1) {
        ostringstream msg;
        int l = 1;
        msg << "\033" << endl;
        msg << l << ": ";
        for (int i = 0; i < nlines; ++i) {
            int j = 0;
            while (lines[i][j] != 0) {
                if (lines[i][j] != '\r') {
                    msg << lines[i][j];
                }
                if (lines[i][j] == '\n') {
                    ++l;
                    msg << l << ": ";
                }
                ++j;
            }
        }
        GLsizei length;
        char *log = new char[logLength];
        glGetShaderInfoLog(shaderId, GLsizei(logLength), &length, log);
        msg << endl << string(log);
        msg << "\033";
        logger->log("COMPILER", msg.str());
        delete[] log;
    }
}

/// @cond RESOURCES

class ModuleResource : public ResourceTemplate<20, Module>
{
public:
    ModuleResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<20, Module>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        try {
            checkParameters(desc, e, "name,version, source,vertex,tessControl,tessEvaluation,geometry,fragment,options,");

            int version;
            getIntParameter(desc, e, "version", &version);

            const TiXmlNode *n = e->FirstChild();
            while (n != NULL) {
                const TiXmlElement *f = n->ToElement();
                if (f != NULL) {
                    string n = f->Attribute("name");

                    string type = f->Value();
                    ptr<Value> v;
                    if (strncmp(type.c_str(), "uniform1", 8) == 0) {
                        checkParameters(desc, f, "name,x,");
                        float x = 0.f;
                        getFloatParameter(desc, f, "x", &x);
                        switch (type[8]) {
                            case 'f':
                                v = new Value1f(n, GLfloat(x));
                                break;
                            case 'd':
                                v = new Value1d(n, GLdouble(x));
                                break;
                            case 'i':
                                v = new Value1i(n, GLint(x));
                                break;
                            case 'b':
                                v = new Value1b(n, x);
                                break;
                            case 'v':
                                if (type[9] == 'i') {
                                    v = new Value1ui(n, GLuint(x));
                                    break;
                                }
                            default:
                                if (Logger::ERROR_LOGGER != NULL) {
                                    log(Logger::ERROR_LOGGER, desc, e, "Invalid type specifier '" + type + "'");
                                }
                                throw exception();

                        }
                    } else if (strncmp(type.c_str(), "uniform2", 8) == 0) {
                        checkParameters(desc, f, "name,x,y,");
                        float x = 0.f;
                        float y = 0.f;
                        getFloatParameter(desc, f, "x", &x);
                        getFloatParameter(desc, f, "y", &y);
                        switch (type[8]) {
                            case 'f':
                                v = new Value2f(n, vec2<GLfloat>(x, y));
                                break;
                            case 'd':
                                v = new Value2d(n, vec2<GLdouble>(x, y));
                                break;
                            case 'i':
                                v = new Value2i(n, vec2<GLint>(GLint(x), GLint(y)));
                                break;
                            case 'b':
                                v = new Value2b(n, vec2<bool>(x, y));
                                break;
                            case 'v':
                                if (type[9] == 'i') {
                                    v = new Value2ui(n, vec2<GLuint>(GLuint(x), GLuint(y)));
                                    break;
                                }
                            default:
                                if (Logger::ERROR_LOGGER != NULL) {
                                    log(Logger::ERROR_LOGGER, desc, e, "Invalid type specifier '" + type + "'");
                                }
                                throw exception();

                        }
                    } else if (strncmp(type.c_str(), "uniform3", 8) == 0) {
                        checkParameters(desc, f, "name,x,y,z,");
                        float x = 0.f;
                        float y = 0.f;
                        float z = 0.f;
                        getFloatParameter(desc, f, "x", &x);
                        getFloatParameter(desc, f, "y", &y);
                        getFloatParameter(desc, f, "z", &z);
                        switch (type[8]) {
                            case 'f':
                                v = new Value3f(n, vec3<GLfloat>(x, y, z));
                                break;
                            case 'd':
                                v = new Value3d(n, vec3<GLdouble>(x, y, z));
                                break;
                            case 'i':
                                v = new Value3i(n, vec3<GLint>(GLint(x), GLint(y), GLint(z)));
                                break;
                            case 'b':
                                v = new Value3b(n, vec3<bool>(x, y, z));
                                break;
                            case 'v':
                                if (type[9] == 'i') {
                                    v = new Value3ui(n, vec3<GLuint>(GLuint(x), GLuint(y), GLuint(z)));
                                    break;
                                }
                            default:
                                if (Logger::ERROR_LOGGER != NULL) {
                                    log(Logger::ERROR_LOGGER, desc, e, "Invalid type specifier '" + type + "'");
                                }
                                throw exception();

                        }
                    } else if (strncmp(type.c_str(), "uniform4", 8) == 0) {
                        checkParameters(desc, f, "name,x,y,z,w,");
                        float x = 0.f;
                        float y = 0.f;
                        float z = 0.f;
                        float w = 0.f;
                        getFloatParameter(desc, f, "x", &x);
                        getFloatParameter(desc, f, "y", &y);
                        getFloatParameter(desc, f, "z", &z);
                        getFloatParameter(desc, f, "w", &w);
                        switch (type[8]) {
                            case 'f':
                                v = new Value4f(n, vec4<GLfloat>(x, y, z, w));
                                break;
                            case 'd':
                                v = new Value4d(n, vec4<GLdouble>(x, y, z, w));
                                break;
                            case 'i':
                                v = new Value4i(n, vec4<GLint>(GLint(x), GLint(y), GLint(z), GLint(w)));
                                break;
                            case 'b':
                                v = new Value4b(n, vec4<bool>(x, y, z, w));
                                break;
                            case 'v':
                                if (type[9] == 'i') {
                                    v = new Value4ui(n, vec4<GLuint>(GLuint(x), GLuint(y), GLuint(z), GLuint(w)));
                                    break;
                                }
                            default:
                                if (Logger::ERROR_LOGGER != NULL) {
                                    log(Logger::ERROR_LOGGER, desc, e, "Invalid type specifier '" + type + "'");
                                }
                                throw exception();

                        }
                    } else if (strncmp(type.c_str(), "uniformMatrix", 13) == 0) {
                        checkParameters(desc, f, "name,value,");
                        string value = f->Attribute("value");
                        if (strcmp(value.c_str(), "identity") == 0) {
                            if (strncmp(type.c_str(), "uniformMatrix2", 14) == 0) {
                                switch(type[14]) {
                                    case 'd': {
                                        GLdouble m[] = {1, 0, 0, 1};
                                        v = new ValueMatrix2d(n, m);
                                        break;
                                    }
                                    case 'f': {
                                        GLfloat m[] = {1.f, 0.f, 0.f, 1.f};
                                        v = new ValueMatrix2f(n, m);
                                        break;
                                    }
                                    default:
                                        if (Logger::ERROR_LOGGER != NULL) {
                                            log(Logger::ERROR_LOGGER, desc, e, "Unsupported matrix type '" + type + "'");
                                        }
                                        throw exception();
                                }
                            } else if (strncmp(type.c_str(), "uniformMatrix3", 14) == 0) {
                                switch(type[14]) {
                                    case 'd':
                                        v = new ValueMatrix3d(n, mat3<GLdouble>::IDENTITY);
                                        break;
                                    case 'f':
                                        v = new ValueMatrix3f(n, mat3<GLfloat>::IDENTITY);
                                        break;
                                    default:
                                        if (Logger::ERROR_LOGGER != NULL) {
                                            log(Logger::ERROR_LOGGER, desc, e, "Unsupported matrix type '" + type + "'");
                                        }
                                        throw exception();
                                }
                            } else if (strncmp(type.c_str(), "uniformMatrix4", 14) == 0) {
                                switch(type[14]) {
                                    case 'd':
                                        v = new ValueMatrix4d(n, mat4<GLdouble>::IDENTITY);
                                        break;
                                    case 'f':
                                        v = new ValueMatrix4f(n, mat4<GLfloat>::IDENTITY);
                                        break;
                                    default:
                                        if (Logger::ERROR_LOGGER != NULL) {
                                            log(Logger::ERROR_LOGGER, desc, e, "Unsupported matrix type '" + type + "'");
                                        }
                                        throw exception();
                                }
                            } else {
                                if (Logger::ERROR_LOGGER != NULL) {
                                    log(Logger::ERROR_LOGGER, desc, e, "Unsupported matrix type or invalid value'" + type + "'");
                                }
                                throw exception();
                            }
                        } else {
                            GLfloat *valuesF = NULL;
                            GLdouble *valuesD = NULL; // to avoid copying the buffer if type is double instead of float;
                            int nValues = -1;
                            bool isFloat = true;
                            if (strncmp(type.c_str(), "uniformMatrix2x3", 16) == 0) {
                                nValues = 6;
                                isFloat = type[16] != 'd';
                            } else if (strncmp(type.c_str(), "uniformMatrix2x4", 16) == 0) {
                                nValues = 8;
                                isFloat = type[16] != 'd';
                            } else if (strncmp(type.c_str(), "uniformMatrix3x2", 16) == 0) {
                                nValues = 6;
                                isFloat = type[16] != 'd';
                            } else if (strncmp(type.c_str(), "uniformMatrix3x4", 16) == 0) {
                                nValues = 12;
                                isFloat = type[16] != 'd';
                            } else if (strncmp(type.c_str(), "uniformMatrix4x2", 16) == 0) {
                                nValues = 8;
                                isFloat = type[16] != 'd';
                            } else if (strncmp(type.c_str(), "uniformMatrix4x3", 16) == 0) {
                                nValues = 12;
                                isFloat = type[16] != 'd';
                            } else if (strncmp(type.c_str(), "uniformMatrix2", 14) == 0) {
                                nValues = 4;
                                isFloat = type[14] != 'd';
                            } else if (strncmp(type.c_str(), "uniformMatrix3", 14) == 0) {
                                nValues = 9;
                                isFloat = type[14] != 'd';
                            } else if (strncmp(type.c_str(), "uniformMatrix4", 14) == 0) {
                                nValues = 16;
                                isFloat = type[14] != 'd';
                            } else {
                                if (Logger::ERROR_LOGGER != NULL) {
                                    log(Logger::ERROR_LOGGER, desc, e, "Unsupported matrix type '" + type + "'");
                                }
                                throw exception();
                            }

                            if (isFloat) {
                                valuesF = new GLfloat[nValues];
                            } else {
                                valuesD = new GLdouble[nValues];
                            }
                            if (strcmp(value.c_str(), "zero") == 0) {
                                for (int i = 0; i < nValues; ++i) {
                                    if (isFloat) {
                                        valuesF[i] = 0.f;
                                    } else {
                                        valuesD[i] = 0;
                                    }
                                }
                            } else {
                                int cpt = 0;
                                value = value + ',';
                                string::size_type start = 0;
                                string::size_type index;
                                while ((index = value.find(',', start)) != string::npos && cpt < nValues) {
                                    string val = value.substr(start, index - start);
                                    if (isFloat) {
                                        valuesF[cpt] = atof(val.c_str());
                                    } else {
                                        valuesD[cpt] = double(atof(val.c_str()));
                                    }
                                    ++cpt;
                                    start = index + 1;
                                }
                                if (cpt != nValues) {
                                    if (Logger::ERROR_LOGGER != NULL) {
                                        log(Logger::ERROR_LOGGER, desc, e, "Invalid matrix format '" + value + "'");
                                    }
                                    throw exception();
                                }
                            }

                            if (strncmp(type.c_str(), "uniformMatrix2x3", 16) == 0) {
                                if (isFloat) {
                                    v = new ValueMatrix2x3f(n, valuesF);
                                } else {
                                    v = new ValueMatrix2x3d(n, valuesD);
                                }
                            } else if (strncmp(type.c_str(), "uniformMatrix2x4", 16) == 0) {
                                if (isFloat) {
                                    v = new ValueMatrix2x4f(n, valuesF);
                                } else {
                                    v = new ValueMatrix2x4d(n, valuesD);
                                }
                            } else if (strncmp(type.c_str(), "uniformMatrix3x2", 16) == 0) {
                                if (isFloat) {
                                    v = new ValueMatrix3x2f(n, valuesF);
                                } else {
                                    v = new ValueMatrix3x2d(n, valuesD);
                                }
                            } else if (strncmp(type.c_str(), "uniformMatrix3x4", 16) == 0) {
                                if (isFloat) {
                                    v = new ValueMatrix3x4f(n, valuesF);
                                } else {
                                    v = new ValueMatrix3x4d(n, valuesD);
                                }
                            } else if (strncmp(type.c_str(), "uniformMatrix4x2", 16) == 0) {
                                if (isFloat) {
                                    v = new ValueMatrix4x2f(n, valuesF);
                                } else {
                                    v = new ValueMatrix4x2d(n, valuesD);
                                }
                            } else if (strncmp(type.c_str(), "uniformMatrix4x3", 16) == 0) {
                                if (isFloat) {
                                    v = new ValueMatrix4x3f(n, valuesF);
                                } else {
                                    v = new ValueMatrix4x3d(n, valuesD);
                                }
                            } else if (strncmp(type.c_str(), "uniformMatrix2", 14) == 0) {
                                if (isFloat) {
                                    v = new ValueMatrix2f(n, valuesF);
                                } else {
                                    v = new ValueMatrix3d(n, valuesD);
                                }
                            } else if (strncmp(type.c_str(), "uniformMatrix3", 14) == 0) {
                                if (isFloat) {
                                    v = new ValueMatrix3f(n, valuesF);
                                } else {
                                    v = new ValueMatrix3d(n, valuesD);
                                }
                            } else if (strncmp(type.c_str(), "uniformMatrix4", 14) == 0) {
                                if (isFloat) {
                                    v = new ValueMatrix4f(n, valuesF);
                                } else {
                                    v = new ValueMatrix4d(n, valuesD);
                                }
                            }
                        }
                    } else if (strncmp(type.c_str(), "uniformSampler", 14) == 0) {
                        checkParameters(desc, f, "name,texture,");
                        ptr<Texture> t = manager->loadResource(f->Attribute("texture")).cast<Texture>();
                        if (t == NULL) {
                            if (Logger::ERROR_LOGGER != NULL) {
                                log(Logger::ERROR_LOGGER, desc, e, "Cannot find '" + string(f->Attribute("texture")) + "' texture");
                            }
                            throw exception();
                        }
                        v = new ValueSampler(SAMPLER_2D, n, t);
                    } else {
                        if (Logger::ERROR_LOGGER != NULL) {
                            log(Logger::ERROR_LOGGER, desc, e, "Unsupported type specifier '" + type + "'");
                        }
                        throw exception();
                    }
                    addInitialValue(v);
                }
                n = n->NextSibling();
            }

            string header;
            if (e->Attribute("options") != NULL) {
                string source;
                string options = string(e->Attribute("options")) + ",";
                string::size_type start = 0;
                string::size_type index;
                while ((index = options.find(',', start)) != string::npos) {
                    string option = options.substr(start, index - start);
                    header = header + "#define " + option + "\n";
                    start = index + 1;
                }
            }

            if (strlen((const char*) desc->getData()) < desc->getSize()) {
                const char* p1 = ((const char*) desc->getData());
                const char* p2 = p1 + strlen(p1) + 1;
                const char* p3 = p2 + strlen(p2) + 1;
                const char* p4 = p3 + strlen(p3) + 1;
                const char* p5 = p4 + strlen(p4) + 1;
                const char* head = header.c_str();
                const char* vertex = strlen(p1) == 0 ? NULL : p1;
                const char* tessControl = strlen(p2) == 0 ? NULL : p2;
                const char* tessEval = strlen(p3) == 0 ? NULL : p3;
                const char* geometry = strlen(p4) == 0 ? NULL : p4;
                const char* fragment = strlen(p5) == 0 ? NULL : p5;
                init(version, head, vertex, head, tessControl, head, tessEval, head, geometry, head, fragment);
            } else {
                if (e->Attribute("options") != NULL) {
                    header += (const char*) desc->getData();
                    init(version, header.c_str());
                } else {
                    init(version, (const char*) desc->getData());
                }
            }

            if (e->Attribute("feedback") != NULL) {
                bool interleaved;
                if (strcmp(e->Attribute("feedback"), "interleaved") == 0) {
                    interleaved = true;
                } else {
                    interleaved = false;
                }
                setFeedbackMode(interleaved);

                string varyings = string(e->Attribute("varyings")) + ",";
                string::size_type start = 0;
                string::size_type index;
                while ((index = varyings.find(',', start)) != string::npos) {
                    string varying = varyings.substr(start, index - start);
                    addFeedbackVarying(varying);
                    start = index + 1;
                }

            }

            desc->clearData();
            if (Logger::INFO_LOGGER != NULL) {
                Logger::INFO_LOGGER->log("COMPILER", "Compiled module '" + name + "'");
            }
        } catch (...) {
            desc->clearData();
            throw exception();
        }
    }
};

extern const char module[] = "module";

static ResourceFactory::Type<module, ModuleResource> ModuleType;

/// @endcond

}
