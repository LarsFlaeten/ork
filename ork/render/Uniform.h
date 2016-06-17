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

#ifndef _ORK_UNIFORM_H_
#define _ORK_UNIFORM_H_

#include <map>
#include <string>

#include "ork/core/Factory.h"
#include "ork/math/vec2.h"
#include "ork/math/vec3.h"
#include "ork/math/vec4.h"
#include "ork/math/mat3.h"
#include "ork/math/mat4.h"
#include "ork/render/GPUBuffer.h"
#include "ork/render/Texture.h"
#include "ork/render/Value.h"

using namespace std;

namespace ork
{

class Program;

class UniformBlock;

#ifdef ORK_NO_GLPROGRAMUNIFORM
#define SETVALUE setValueIfCurrent
#else
#define SETVALUE setValue
#endif

/**
 * An abstract uniform variable of a Program. The uniform value can be set at
 * any time, and affects only the program that contains this uniform (unless
 * this uniform belongs to a UniformBlock whose Buffer is shared between different
 * programs, in which case setting the value of this uniform also changes its
 * value for the other programs).
 *
 * @ingroup render
 */
class ORK_API Uniform : public Object
{
public:
    /**
     * Deletes this uniform.
     */
    virtual ~Uniform();

    /**
     * Returns the type of this uniform.
     */
    virtual UniformType getType() const = 0;

    /**
     * Returns the name of this uniform.
     */
    string getName() const;

    /**
     * Sets the value of this uniform.
     *
     * @param v the new value for this uniform. Must be of the same
     *      type as this Uniform.
     */
    virtual void setValue(ptr<Value> v) = 0;

protected:
    /**
     * The Program to which this uniform belongs.
     */
    Program *program;

    /**
     * The UniformBlock to which this uniform belongs. Maybe NULL.
     */
    UniformBlock *block;

    /**
     * The name of this uniform.
     */
    string name;

    /**
     * The location of this uniform. For an uniform inside a block,
     * this location is an offset inside the uniform block buffer.
     */
    GLint location;

#ifdef ORK_NO_GLPROGRAMUNIFORM
    /**
     * True if the value of this uniform in its program is not up to date.
     */
    bool dirty;
#endif

    /**
     * Creates a new uniform.
     *
     * @param type the type of this uniform.
     * @param program the Program to which this uniform belongs.
     * @param block UniformBlock to which this uniform belongs. Maybe NULL.
     * @param name the name of the uniform in the GLSL shader code.
     * @param location the location of this uniform. For an uniform inside a
     *      block, this location is an offset inside the uniform block buffer.
     */
    Uniform(const char *type, Program *program, UniformBlock *block, const string &name, GLint location);

#ifdef ORK_NO_GLPROGRAMUNIFORM
    /**
     * Sets this uniform in its program if this it is the current one.
     */
    void setValueIfCurrent();
#endif

    /**
     * Sets this uniform in its program.
     */
    virtual void setValue() = 0;

    /**
     * Maps the GPUBuffer of the uniform block of this uniform into memory.
     */
    volatile void *mapBuffer(GLint offset) const;

    friend class Module;

    friend class ModuleResource;

    friend class Program;
};

// ----------------------------------------------------------------------------

/**
 * A uniform holding a single number.
 * @ingroup render
 */
template<UniformType U, class T, class R, const char *V, const char *W>
class ORK_API Uniform1 : public Uniform
{
public:
    /**
     * Deletes this uniform.
     */
    virtual ~Uniform1()
    {
    }

    virtual UniformType getType() const
    {
        return U;
    }

    /**
     * Returns the current value of this uniform.
     */
    T get() const
    {
        if (block == NULL || program  == NULL) {
            return value;
        } else {
            R* buf = (R*) mapBuffer(location);
            return *buf;
        }
    }

    /**
     * Sets the value of this uniform.
     *
     * @param value the new value for this uniform.
     */
    void set(T value)
    {
        if (block == NULL || program  == NULL) {
            this->value = value;
            if (program != NULL) {
                SETVALUE();
            }
        } else {
            R* buf = (R*) mapBuffer(location);
            *buf = value;
        }
    }

    virtual void setValue(ptr<Value> v)
    {
        set(v.cast< Value1<U, T, W> >()->get());
    }

protected:
    /**
     * Creates a new uniform.
     *
     * @param program the Program to which this uniform belongs.
     * @param block UniformBlock to which this uniform belongs. Maybe NULL.
     * @param name the name of the uniform in the GLSL shader code.
     * @param location the location of this uniform. For an uniform inside a
     *      block, this location is an offset inside the uniform block buffer.
     */
    Uniform1(Program *program, UniformBlock *block, const string &name, GLint location) :
        Uniform(V, program, block, name, location)
    {
    }

    virtual void setValue();

private:
    /**
     * The current value of this uniform.
     */
    T value;

    friend class Module;

    friend class ModuleResource;

    friend class Program;
};

extern const char uniform1f[];

extern const char uniform1d[];

extern const char uniform1i[];

extern const char uniform1ui[];

extern const char uniform1b[];

/**
 * A uniform holding a single float value.
 * @ingroup render
 */
typedef Uniform1<VEC1F, GLfloat, GLfloat, uniform1f, value1f> Uniform1f;

/**
 * A uniform holding a single double value.
 * @ingroup render
 */
typedef Uniform1<VEC1D, GLdouble, GLdouble, uniform1d, value1d> Uniform1d;

/**
 * A uniform holding a single signed integer value.
 * @ingroup render
 */
typedef Uniform1<VEC1I, GLint, GLint, uniform1i, value1i> Uniform1i;

/**
 * A uniform holding a single unsigned integer value.
 * @ingroup render
 */
typedef Uniform1<VEC1UI, GLuint, GLuint, uniform1ui, value1ui> Uniform1ui;

/**
 * A uniform holding a single boolean value.
 * @ingroup render
 */
typedef Uniform1<VEC1B, bool, GLuint, uniform1b, value1b> Uniform1b;

// ----------------------------------------------------------------------------

/**
 * A uniform holding a two vector value.
 * @ingroup render
 */
template<UniformType U, class T, class R, const char *V, const char *W>
class ORK_API Uniform2 : public Uniform
{
public:
    /**
     * Deletes this uniform.
     */
    virtual ~Uniform2()
    {
    }

    virtual UniformType getType() const
    {
        return U;
    }

    /**
     * Returns the current value of this uniform.
     */
    vec2<T> get() const
    {
        if (block == NULL || program  == NULL) {
            return value;
        } else {
            R* buf = (R*) mapBuffer(location);
            return vec2<T>(buf[0], buf[1]);
        }
    }

    /**
     * Sets the value of this uniform.
     *
     * @param value the new value for this uniform.
     */
    void set(const vec2<T> &value)
    {
        if (block == NULL || program  == NULL) {
            this->value = value;
            if (program != NULL) {
                SETVALUE();
            }
        } else {
            R* buf = (R*) mapBuffer(location);
            buf[0] = value.x;
            buf[1] = value.y;
        }
    }

    virtual void setValue(ptr<Value> v)
    {
        set(v.cast< Value2<U, T, W> >()->get());
    }

protected:
    /**
     * Creates a new uniform.
     *
     * @param program the Program to which this uniform belongs.
     * @param block UniformBlock to which this uniform belongs. Maybe NULL.
     * @param name the name of the uniform in the GLSL shader code.
     * @param location the location of this uniform. For an uniform inside a
     *      block, this location is an offset inside the uniform block buffer.
     */
    Uniform2(Program *program, UniformBlock *block, const string &name, GLint location) :
        Uniform(V, program, block, name, location)
    {
    }

    virtual void setValue();

private:
    /**
     * The current value of this uniform.
     */
    vec2<T> value;

    friend class Module;

    friend class ModuleResource;

    friend class Program;
};

extern const char uniform2f[];

extern const char uniform2d[];

extern const char uniform2i[];

extern const char uniform2ui[];

extern const char uniform2b[];

/**
 * A uniform holding a single precision two vector.
 * @ingroup render
 */
typedef Uniform2<VEC2F, GLfloat, GLfloat, uniform2f, value2f> Uniform2f;

/**
 * A uniform holding a double precision two vector.
 * @ingroup render
 */
typedef Uniform2<VEC2B, GLdouble, GLdouble, uniform2d, value2d> Uniform2d;

/**
 * A uniform holding a signed integer two vector.
 * @ingroup render
 */
typedef Uniform2<VEC2I, GLint, GLint, uniform2i, value2i> Uniform2i;

/**
 * A uniform holding an unsigned integer two vector.
 * @ingroup render
 */
typedef Uniform2<VEC2UI, GLuint, GLuint, uniform2ui, value2ui> Uniform2ui;

/**
 * A uniform holding a boolean two vector.
 * @ingroup render
 */
typedef Uniform2<VEC2B, bool, GLuint, uniform2b, value2b> Uniform2b;

// ----------------------------------------------------------------------------

/**
 * An Uniform holding a three vector value.
 * @ingroup render
 */
template<UniformType U, class T, class R, const char *V, const char *W>
class ORK_API Uniform3 : public Uniform
{
public:
    /**
     * Deletes this uniform.
     */
    virtual ~Uniform3()
    {
    }

    virtual UniformType getType() const
    {
        return U;
    }

    /**
     * Returns the current value of this uniform.
     */
    vec3<T> get() const
    {
        if (block == NULL || program  == NULL) {
            return value;
        } else {
            R* buf = (R*) mapBuffer(location);
            return vec3<T>(buf[0], buf[1], buf[2]);
        }
    }

    /**
     * Sets the value of this uniform.
     *
     * @param value the new value for this uniform.
     */
    void set(const vec3<T> &value)
    {
        if (block == NULL || program  == NULL) {
            this->value = value;
            if (program != NULL) {
                SETVALUE();
            }
        } else {
            R* buf = (R*) mapBuffer(location);
            buf[0] = value.x;
            buf[1] = value.y;
            buf[2] = value.z;
        }
    }

    virtual void setValue(ptr<Value> v)
    {
        set(v.cast< Value3<U, T, W> >()->get());
    }

protected:
    /**
     * Creates a new uniform.
     *
     * @param program the Program to which this uniform belongs.
     * @param block UniformBlock to which this uniform belongs. Maybe NULL.
     * @param name the name of the uniform in the GLSL shader code.
     * @param location the location of this uniform. For an uniform inside a
     *      block, this location is an offset inside the uniform block buffer.
     */
    Uniform3(Program *program, UniformBlock *block, const string &name, GLint location) :
        Uniform(V, program, block, name, location)
    {
    }

    virtual void setValue();

private:
    /**
     * The current value of this uniform.
     */
    vec3<T> value;

    friend class Module;

    friend class ModuleResource;

    friend class Program;
};

extern const char uniform3f[];

extern const char uniform3d[];

extern const char uniform3i[];

extern const char uniform3ui[];

extern const char uniform3b[];

/**
 * A uniform holding a single precision three vector.
 * @ingroup render
 */
typedef Uniform3<VEC3F, GLfloat, GLfloat, uniform3f, value3f> Uniform3f;

/**
 * A uniform holding a double precision three vector.
 * @ingroup render
 */
typedef Uniform3<VEC3D, GLdouble, GLdouble, uniform3d, value3d> Uniform3d;

/**
 * A uniform holding a signed integer three vector.
 * @ingroup render
 */
typedef Uniform3<VEC3I, GLint, GLint, uniform3i, value3i> Uniform3i;

/**
 * A uniform holding an unsigned integer three vector.
 * @ingroup render
 */
typedef Uniform3<VEC3UI, GLuint, GLuint, uniform3ui, value3ui> Uniform3ui;

/**
 * A uniform holding a boolean three vector.
 * @ingroup render
 */
typedef Uniform3<VEC3B, bool, GLuint, uniform3b, value3b> Uniform3b;

// ----------------------------------------------------------------------------

/**
 * A uniform holding a four vector value.
 * @ingroup render
 */
template<UniformType U, class T, class R, const char *V, const char *W>
class ORK_API Uniform4 : public Uniform
{
public:
    /**
     * Deletes this uniform.
     */
    virtual ~Uniform4()
    {
    }

    virtual UniformType getType() const
    {
        return U;
    }

    /**
     * Returns the current value of this uniform.
     */
    vec4<T> get() const
    {
        if (block == NULL || program  == NULL) {
            return value;
        } else {
            R* buf = (R*) mapBuffer(location);
            return vec4<T>(buf[0], buf[1], buf[2], buf[3]);
        }
    }

    /**
     * Sets the value of this uniform.
     *
     * @param value the new value for this uniform.
     */
    void set(const vec4<T> &value)
    {
        if (block == NULL || program  == NULL) {
            this->value = value;
            if (program != NULL) {
                SETVALUE();
            }
        } else {
            R* buf = (R*) mapBuffer(location);
            buf[0] = value.x;
            buf[1] = value.y;
            buf[2] = value.z;
            buf[3] = value.w;
        }
    }

    virtual void setValue(ptr<Value> v)
    {
        set(v.cast< Value4<U, T, W> >()->get());
    }

protected:
    /**
     * Creates a new uniform.
     *
     * @param program the Program to which this uniform belongs.
     * @param block UniformBlock to which this uniform belongs. Maybe NULL.
     * @param name the name of the uniform in the GLSL shader code.
     * @param location the location of this uniform. For an uniform inside a
     *      block, this location is an offset inside the uniform block buffer.
     */
    Uniform4(Program *program, UniformBlock *block, const string &name, GLint location) :
        Uniform(V, program, block, name, location)
    {
    }

    virtual void setValue();

private:
    /**
     * The current value of this uniform.
     */
    vec4<T> value;

    friend class Module;

    friend class ModuleResource;

    friend class Program;
};

extern const char uniform4f[];

extern const char uniform4d[];

extern const char uniform4i[];

extern const char uniform4ui[];

extern const char uniform4b[];

/**
 * A uniform holding a single precision four vector.
 * @ingroup render
 */
typedef Uniform4<VEC4F, GLfloat, GLfloat, uniform4f, value4f> Uniform4f;

/**
 * A uniform holding a double precision four vector.
 * @ingroup render
 */
typedef Uniform4<VEC4D, GLdouble, GLdouble, uniform4d, value4d> Uniform4d;

/**
 * A uniform holding a signed integer four vector.
 * @ingroup render
 */
typedef Uniform4<VEC4I, GLint, GLint, uniform4i, value4i> Uniform4i;

/**
 * A uniform holding an unsigned integer four vector.
 * @ingroup render
 */
typedef Uniform4<VEC4UI, GLuint, GLuint, uniform4ui, value4ui> Uniform4ui;

/**
 * A uniform holding a boolean four vector.
 * @ingroup render
 */
typedef Uniform4<VEC4B, bool, GLuint, uniform4b, value4b> Uniform4b;

// ----------------------------------------------------------------------------

/**
 * A uniform holding a matrix value.
 * @ingroup render
 */
template<UniformType U, class T, int C, int R, const char *V, const char *W>
class ORK_API UniformMatrix : public Uniform
{
public:
    /**
     * Deletes this uniform.
     */
    virtual ~UniformMatrix()
    {
    }

    virtual UniformType getType() const
    {
        return U;
    }

    /**
     * Returns the current value of this uniform.
     *
     * @return the matrix coefficients in row major order (i.e. first row,
     *       second row, etc).
     */
    const T* get() const
    {
        if (block != NULL && program != NULL) {
            unsigned char *buf = (unsigned char*) mapBuffer(location);
            if (isRowMajor) {
                for (int r = 0; r < R; ++r) {
                    for (int c = 0; c < C; ++c) {
                        ((T*) value)[r * C + c] = ((T*) (buf + r * stride))[c];
                    }
                }
            } else {
                for (int r = 0; r < R; ++r) {
                    for (int c = 0; c < C; ++c) {
                        ((T*) value)[r * C + c] = ((T*) (buf + c * stride))[r];
                    }
                }
            }
        }
        return value;
    }

    /**
     * Sets the value of this uniform.
     *
     * @param value the matrix coefficients in row major order (i.e. first row,
     *      second row, etc).
     */
    void set(const T *value)
    {
        if (block == NULL || program  == NULL) {
            for (int i = 0; i < R * C; ++i) {
                this->value[i] = value[i];
            }
            if (program != NULL) {
                SETVALUE();
            }
        } else {
            unsigned char *buf = (unsigned char*) mapBuffer(location);
            if (isRowMajor) {
                for (int r = 0; r < R; ++r) {
                    for (int c = 0; c < C; ++c) {
                        ((T*) (buf + r * stride))[c] = value[r * C + c];
                    }
                }
            } else {
                for (int r = 0; r < R; ++r) {
                    for (int c = 0; c < C; ++c) {
                        ((T*) (buf + c * stride))[r] = value[r * C + c];
                    }
                }
            }
        }
    }

    virtual void setValue(ptr<Value> v)
    {
        set(v.cast< ValueMatrix<U, T, C, R, W> >()->get());
    }

protected:
    /**
     * The current value of this uniform.
     * The matrix coefficients are stored in row major order.
     */
    T value[C * R];

    /**
     * Creates a new uniform.
     *
     * @param program the Program to which this uniform belongs.
     * @param block UniformBlock to which this uniform belongs. Maybe NULL.
     * @param name the name of the uniform in the GLSL shader code.
     * @param location the location of this uniform. For an uniform inside a
     *      block, this location is an offset inside the uniform block buffer.
     * @param stride the stride between two consecutive rows or columns when
     *      this uniform is stored in an uniform block.
     * @param isRowMajor true if this uniform is stored in row major order
     *      in an uniform block.
     */
    UniformMatrix(Program *program, UniformBlock *block, const string &name, GLint location, int stride, int isRowMajor) :
        Uniform(V, program, block, name, location), stride(stride), isRowMajor(isRowMajor)
    {
    }

    virtual void setValue();

private:
    /**
    * The stride between two consecutive rows or columns when this uniform is
    * stored in an uniform block.
    */
    int stride;

    /**
     * True if this uniform is stored in row major order in an uniform block.
     */
    int isRowMajor;

    friend class Module;

    friend class ModuleResource;

    friend class Program;
};

/**
 * A uniform holding a 3x3 matrix value.
 * @ingroup render
 */
template<UniformType U, class T, const char *V, const char *W>
class UniformMatrix3 : public UniformMatrix<U, T, 3, 3, V, W>
{
public:
    /**
     * Returns the current value of this uniform.
     */
    mat3<T> getMatrix() const
    {
        return mat3<T>(UniformMatrix<U, T, 3, 3, V, W>::get());
    }

    /**
     * Sets the value of this uniform.
     *
     * @param value the new value for this uniform.
     */
    void setMatrix(const mat3<T> &value)
    {
        UniformMatrix<U, T, 3, 3, V, W>::set(value.coefficients());
    }

protected:
    /**
     * Creates a new uniform.
     *
     * @param program the Program to which this uniform belongs.
     * @param block UniformBlock to which this uniform belongs. Maybe NULL.
     * @param name the name of the uniform in the GLSL shader code.
     * @param location the location of this uniform. For an uniform inside a
     *      block, this location is an offset inside the uniform block buffer.
     * @param stride the stride between two consecutive rows or columns when
     *      this uniform is stored in an uniform block.
     * @param isRowMajor true if this uniform is stored in row major order
     *      in an uniform block.
     */
    UniformMatrix3(Program *program, UniformBlock *block, const string &name, GLint location, int stride, int isRowMajor) :
        UniformMatrix<U, T, 3, 3, V, W>(program, block, name, location, stride, isRowMajor)
    {
    }

    friend class Module;

    friend class ModuleResource;

    friend class Program;
};

/**
 * A uniform holding a 4x4 matrix value.
 * @ingroup render
 */
template<UniformType U, class T, const char *V, const char *W>
class UniformMatrix4 : public UniformMatrix<U, T, 4, 4, V, W>
{
public:
    /**
     * Returns the current value of this uniform.
     */
    mat4<T> getMatrix() const
    {
        return mat4<T>(UniformMatrix<U, T, 4, 4, V, W>::get());
    }

    /**
     * Sets the value of this uniform.
     *
     * @param value the new value for this uniform.
     */
    void setMatrix(const mat4<T> &value)
    {
        UniformMatrix<U, T, 4, 4, V, W>::set(value.coefficients());
    }

protected:
    /**
     * Creates a new uniform.
     *
     * @param program the Program to which this uniform belongs.
     * @param block UniformBlock to which this uniform belongs. Maybe NULL.
     * @param name the name of the uniform in the GLSL shader code.
     * @param location the location of this uniform. For an uniform inside a
     *      block, this location is an offset inside the uniform block buffer.
     * @param stride the stride between two consecutive rows or columns when
     *      this uniform is stored in an uniform block.
     * @param isRowMajor true if this uniform is stored in row major order
     *      in an uniform block.
     */
    UniformMatrix4(Program *program, UniformBlock *block, const string &name, GLint location, int stride, int isRowMajor) :
        UniformMatrix<U, T, 4, 4, V, W>(program, block, name, location, stride, isRowMajor)
    {
    }

    friend class Module;

    friend class ModuleResource;

    friend class Program;
};

extern const char uniformMatrix2f[];

extern const char uniformMatrix3f[];

extern const char uniformMatrix4f[];

extern const char uniformMatrix2x3f[];

extern const char uniformMatrix2x4f[];

extern const char uniformMatrix3x2f[];

extern const char uniformMatrix3x4f[];

extern const char uniformMatrix4x2f[];

extern const char uniformMatrix4x3f[];

extern const char uniformMatrix2d[];

extern const char uniformMatrix3d[];

extern const char uniformMatrix4d[];

extern const char uniformMatrix2x3d[];

extern const char uniformMatrix2x4d[];

extern const char uniformMatrix3x2d[];

extern const char uniformMatrix3x4d[];

extern const char uniformMatrix4x2d[];

extern const char uniformMatrix4x3d[];

/**
 * A uniform holding a single precision 2x2 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT2F, GLfloat, 2, 2, uniformMatrix2f, valueMatrix2f> UniformMatrix2f;

/**
 * A uniform holding a single precision 3x3 matrix.
 * @ingroup render
 */
typedef UniformMatrix3<MAT3F, GLfloat, uniformMatrix3f, valueMatrix3f> UniformMatrix3f;

/**
 * A uniform holding a single precision 4x4 matrix.
 * @ingroup render
 */
typedef UniformMatrix4<MAT4F, GLfloat, uniformMatrix4f, valueMatrix4f> UniformMatrix4f;

/**
 * A uniform holding a single precision 2x3 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT2x3F, GLfloat, 2, 3, uniformMatrix2x3f, valueMatrix2x3f> UniformMatrix2x3f;

/**
 * A uniform holding a single precision 2x4 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT2x4F, GLfloat, 2, 4, uniformMatrix2x4f, valueMatrix2x4f> UniformMatrix2x4f;

/**
 * A uniform holding a single precision 3x2 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT3x2F, GLfloat, 3, 2, uniformMatrix3x2f, valueMatrix3x2f> UniformMatrix3x2f;

/**
 * A uniform holding a single precision 3x4 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT3x4F, GLfloat, 3, 4, uniformMatrix3x4f, valueMatrix3x4f> UniformMatrix3x4f;

/**
 * A uniform holding a single precision 4x2 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT4x2F, GLfloat, 4, 2, uniformMatrix4x2f, valueMatrix4x2f> UniformMatrix4x2f;

/**
 * A uniform holding a single precision 4x3 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT4x3F, GLfloat, 4, 3, uniformMatrix4x3f, valueMatrix4x3f> UniformMatrix4x3f;

/**
 * A uniform holding a double precision 2x2 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT2D, GLdouble, 2, 2, uniformMatrix2d, valueMatrix2d> UniformMatrix2d;

/**
 * A uniform holding a double precision 3x3 matrix.
 * @ingroup render
 */
typedef UniformMatrix3<MAT3D, GLdouble, uniformMatrix3d, valueMatrix3d> UniformMatrix3d;

/**
 * A uniform holding a double precision 4x4 matrix.
 * @ingroup render
 */
typedef UniformMatrix4<MAT4D, GLdouble, uniformMatrix4d, valueMatrix4d> UniformMatrix4d;

/**
 * A uniform holding a double precision 2x3 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT2x3D, GLdouble, 2, 3, uniformMatrix2x3d, valueMatrix2x3d> UniformMatrix2x3d;

/**
 * A uniform holding a double precision 2x4 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT2x4D, GLdouble, 2, 4, uniformMatrix2x4d, valueMatrix2x4d> UniformMatrix2x4d;

/**
 * A uniform holding a double precision 3x2 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT3x2D, GLdouble, 3, 2, uniformMatrix3x2d, valueMatrix3x2d> UniformMatrix3x2d;

/**
 * A uniform holding a double precision 3x4 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT3x4D, GLdouble, 3, 4, uniformMatrix3x4d, valueMatrix3x4d> UniformMatrix3x4d;

/**
 * A uniform holding a double precision 4x2 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT4x2D, GLdouble, 4, 2, uniformMatrix4x2d, valueMatrix4x2d> UniformMatrix4x2d;

/**
 * A uniform holding a double precision 4x3 matrix.
 * @ingroup render
 */
typedef UniformMatrix<MAT4x3D, GLdouble, 4, 3, uniformMatrix4x3d, valueMatrix4x3d> UniformMatrix4x3d;

// ----------------------------------------------------------------------------

/**
 * A uniform holding a Texture value. In addition to a texture, a UniformSampler
 * can also have a Sampler object to modify the default texture sampling
 * parameters of the bound texture (like the minification of magnification
 * filters).
 *
 * @ingroup render
 */
class ORK_API UniformSampler : public Uniform
{
public:
    /**
     * Deletes this uniform.
     */
    virtual ~UniformSampler();

    virtual UniformType getType() const;

    /**
     * Returns the sampler used to sample the texture bound to this uniform.
     *
     * @return the sampler used to sample the texture bound to this uniform.
     *       May be NULL.
     */
    ptr<Sampler> getSampler() const;

    /**
     * Sets the sampler used to sample the texture bound to this uniform.
     *
     * @param sampler a sampler object. May be NULL.
     */
    void setSampler(const ptr<Sampler> sampler);

    /**
     * Returns the current value of this uniform.
     */
    ptr<Texture> get() const;

    /**
     * Sets the value of this uniform.
     *
     * @param value the new value for this uniform.
     */
    void set(ptr<Texture> value);

    virtual void setValue(ptr<Value> v);

protected:
    /**
     * Creates a new uniform.
     *
     * @param type the type of this uniform.
     * @param program the Program to which this uniform belongs.
     * @param block UniformBlock to which this uniform belongs. Maybe NULL.
     * @param name the name of the uniform in the GLSL shader code.
     * @param location the location of this uniform. For an uniform inside a
     *      block, this location is an offset inside the uniform block buffer.
     */
    UniformSampler(UniformType type, Program *program, UniformBlock *block, const string &name, GLint location);

    virtual void setValue();

private:
    /**
     * The type of this uniform.
     */
    UniformType type;

    /**
     * The current sampler used to sample the texture bound to this uniform. May be NULL.
     */
    ptr<Sampler> sampler;

    /**
     * The current value of this uniform.
     */
    ptr<Texture> value;

    /**
     * The current texture unit value of this uniform.
     */
    int unit;

    friend class Module;

    friend class ModuleResource;

    friend class Program;
};

// ----------------------------------------------------------------------------

/**
 * A named block of uniforms. The values of the uniforms in a uniform block are
 * stored in a GPUBuffer. Different Programs having identical uniform blocks have
 * different UniformBlock objects, but these objects can share the same GPUBuffer
 * to store their values. Hence, changing values inside this GPUBuffer changes the
 * uniform values in all the programs that use this GPUBuffer.
 * You don't have to manipulate the GPUBuffer content yourself to change the
 * uniforms inside a uniform block: this is automatically managed by the Uniform
 * and UniformBlock classes. In particular, these classes know the offset of each
 * uniform in the buffer, and map and unmap this buffer in client memory when
 * necessary.
 * Initially the GPUBuffer associated with a UniformBlock is NULL. You must set it
 * with #setBuffer() before using the uniforms of this block.
 *
 * @ingroup render
 */
class ORK_API UniformBlock : public Object
{
public:
    /**
     * Deletes this uniform block.
     */
    virtual ~UniformBlock();

    /**
     * Returns the name of this uniform block.
     */
    string getName() const;

    /**
     * Returns the GPUBuffer that stores the values of the uniforms of this block.
     */
    ptr<GPUBuffer> getBuffer() const;

    /**
     * Returns the uniform of this block whose name is given.
     *
     * @param name a uniform name.
     * @return the uniform of this block whose name is given, or NULL if there
     *       is no such uniform.
     */
    ptr<Uniform> getUniform(const string &name) const;

    /**
     * Returns the uniform1f of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform1f> getUniform1f(const string &name);

    /**
     * Returns the uniform1d of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform1d> getUniform1d(const string &name);

    /**
     * Returns the uniform1i of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform1i> getUniform1i(const string &name);

    /**
     * Returns the uniform1ui of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform1ui> getUniform1ui(const string &name);

    /**
     * Returns the uniform1b of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform1b> getUniform1b(const string &name);

    /**
     * Returns the uniform2f of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform2f> getUniform2f(const string &name);

    /**
     * Returns the uniform2d of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform2d> getUniform2d(const string &name);

    /**
     * Returns the uniform2i of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform2i> getUniform2i(const string &name);

    /**
     * Returns the uniform2ui of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform2ui> getUniform2ui(const string &name);

    /**
     * Returns the uniform2b of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform2b> getUniform2b(const string &name);

    /**
     * Returns the uniform3f of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform3f> getUniform3f(const string &name);

    /**
     * Returns the uniform3d of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform3d> getUniform3d(const string &name);

    /**
     * Returns the uniform3i of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform3i> getUniform3i(const string &name);

    /**
     * Returns the uniform3ui of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform3ui> getUniform3ui(const string &name);

    /**
     * Returns the uniform3b of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform3b> getUniform3b(const string &name);

    /**
     * Returns the uniform4f of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform4f> getUniform4f(const string &name);

    /**
     * Returns the uniform4d of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform4d> getUniform4d(const string &name);

    /**
     * Returns the uniform4i of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform4i> getUniform4i(const string &name);

    /**
     * Returns the uniform4ui of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform4ui> getUniform4ui(const string &name);

    /**
     * Returns the uniform4b of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<Uniform4b> getUniform4b(const string &name);

    /**
     * Returns the uniformMatrix2f of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix2f> getUniformMatrix2f(const string &name);

    /**
     * Returns the uniformMatrix2d of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix2d> getUniformMatrix2d(const string &name);

    /**
     * Returns the uniformMatrix3f of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix3f> getUniformMatrix3f(const string &name);

    /**
     * Returns the uniformMatrix3d of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix3d> getUniformMatrix3d(const string &name);

    /**
     * Returns the uniformMatrix4f of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix4f> getUniformMatrix4f(const string &name);

    /**
     * Returns the uniformMatrix4d of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix4d> getUniformMatrix4d(const string &name);

    /**
     * Returns the uniformMatrix2x3f of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix2x3f> getUniformMatrix2x3f(const string &name);

    /**
     * Returns the uniformMatrix2x3d of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix2x3d> getUniformMatrix2x3d(const string &name);

    /**
     * Returns the uniformMatrix2x4f of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix2x4f> getUniformMatrix2x4f(const string &name);

    /**
     * Returns the uniformMatrix2x4d of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix2x4d> getUniformMatrix2x4d(const string &name);

    /**
     * Returns the uniformMatrix3x2f of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix3x2f> getUniformMatrix3x2f(const string &name);

    /**
     * Returns the uniformMatrix3x2d of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix3x2d> getUniformMatrix3x2d(const string &name);

    /**
     * Returns the uniformMatrix3x4f of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix3x4f> getUniformMatrix3x4f(const string &name);

    /**
     * Returns the uniformMatrix3x4d of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix3x4d> getUniformMatrix3x4d(const string &name);

    /**
     * Returns the uniformMatrix4x2f of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix4x2f> getUniformMatrix4x2f(const string &name);

    /**
     * Returns the uniformMatrix4x2d of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix4x2d> getUniformMatrix4x2d(const string &name);

    /**
     * Returns the uniformMatrix4x3f of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix4x3f> getUniformMatrix4x3f(const string &name);

    /**
     * Returns the uniformMatrix4x2d of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformMatrix4x3d> getUniformMatrix4x3d(const string &name);

    /**
     * Returns the uniform sampler of this block whose name is given.
     *
     * @param name a GLSL uniform name.
     * @return the uniform of this block whose name is given,
     *       or NULL if there is no such uniform.
     */
    inline ptr<UniformSampler> getUniformSampler(const string &name);

    /**
     * Sets the GPUBuffer to store the values of the uniforms of this block.
     *
     * @param buffer a GPUBuffer. This buffer can be shared between different
     *      UniformBlock instances corresponding to the same uniform block
     *      declaration.
     */
    void setBuffer(ptr<GPUBuffer> buffer);

protected:
    /**
     * The Program to which this uniform block belongs.
     */
    Program *program;

    /**
     * The name of this uniform block.
     */
    string name;

    /**
     * The index of this uniform block in its program.
     */
    GLuint index;

    /**
     * The total size of this uniform block's uniforms.
     */
    GLuint size;

    /**
     * The GPUBuffer that stores the values of the uniforms of this block.
     */
    ptr<GPUBuffer> buffer;

    /**
     * The uniforms of this block.
     */
    map<string, ptr<Uniform> > uniforms;

    /**
     * The buffers associated to each uniform blocks.
     * When creating a new uniform block, the user should check if a buffer was
     * already created for that UB name. Otherwise, he may create a new one.
     */
    static static_ptr<Factory<string, ptr<GPUBuffer> > >buffers;

    /**
     * Callback method to create a new buffer. For use with #buffers.
     */
    static ptr<GPUBuffer> newBuffer(string name);

    /**
     * Creates a new uniform block.
     *
     * @param program the program to which this uniform block belongs.
     * @param name the name of this uniform block in the GLSL code.
     * @param index the index of this uniform block in its program.
     * @param size the minimum buffer size to store the uniforms of this block.
     */
    UniformBlock(Program *program, const string &name, GLuint index, GLuint size);

    /**
     * Returns true if the GPUBuffer associated with this block is currently
     * mapped in client memory.
     */
    bool isMapped() const;

    /**
     * Maps the GPUBuffer associated with this block in client memory. This
     * method also returns the address in client memory of the value at the
     * given offset in the buffer.
     *
     * @param offset an offset in bytes from the start of the buffer.
     * @return a pointer to the value at 'offset' in the mapped buffer.
     */
    volatile void *mapBuffer(GLint offset);

    /**
     * Unmaps the GPUBuffer associated with this block in client memory.
     */
    void unmapBuffer();

    friend class Uniform;

    friend class Module;

    friend class ModuleResource;

    friend class Program;
};

inline ptr<Uniform1f> UniformBlock::getUniform1f(const string &name)
{
    return getUniform(name).cast<Uniform1f>();
}

inline ptr<Uniform1d> UniformBlock::getUniform1d(const string &name)
{
    return getUniform(name).cast<Uniform1d>();
}

inline ptr<Uniform1i> UniformBlock::getUniform1i(const string &name)
{
    return getUniform(name).cast<Uniform1i>();
}

inline ptr<Uniform1ui> UniformBlock::getUniform1ui(const string &name)
{
    return getUniform(name).cast<Uniform1ui>();
}

inline ptr<Uniform1b> UniformBlock::getUniform1b(const string &name)
{
    return getUniform(name).cast<Uniform1b>();
}

inline ptr<Uniform2f> UniformBlock::getUniform2f(const string &name)
{
    return getUniform(name).cast<Uniform2f>();
}

inline ptr<Uniform2d> UniformBlock::getUniform2d(const string &name)
{
    return getUniform(name).cast<Uniform2d>();
}

inline ptr<Uniform2i> UniformBlock::getUniform2i(const string &name)
{
    return getUniform(name).cast<Uniform2i>();
}

inline ptr<Uniform2ui> UniformBlock::getUniform2ui(const string &name)
{
    return getUniform(name).cast<Uniform2ui>();
}

inline ptr<Uniform2b> UniformBlock::getUniform2b(const string &name)
{
    return getUniform(name).cast<Uniform2b>();
}

inline ptr<Uniform3f> UniformBlock::getUniform3f(const string &name)
{
    return getUniform(name).cast<Uniform3f>();
}

inline ptr<Uniform3d> UniformBlock::getUniform3d(const string &name)
{
    return getUniform(name).cast<Uniform3d>();
}

inline ptr<Uniform3i> UniformBlock::getUniform3i(const string &name)
{
    return getUniform(name).cast<Uniform3i>();
}

inline ptr<Uniform3ui> UniformBlock::getUniform3ui(const string &name)
{
    return getUniform(name).cast<Uniform3ui>();
}

inline ptr<Uniform3b> UniformBlock::getUniform3b(const string &name)
{
    return getUniform(name).cast<Uniform3b>();
}

inline ptr<Uniform4f> UniformBlock::getUniform4f(const string &name)
{
    return getUniform(name).cast<Uniform4f>();
}

inline ptr<Uniform4d> UniformBlock::getUniform4d(const string &name)
{
    return getUniform(name).cast<Uniform4d>();
}

inline ptr<Uniform4i> UniformBlock::getUniform4i(const string &name)
{
    return getUniform(name).cast<Uniform4i>();
}

inline ptr<Uniform4ui> UniformBlock::getUniform4ui(const string &name)
{
    return getUniform(name).cast<Uniform4ui>();
}

inline ptr<Uniform4b> UniformBlock::getUniform4b(const string &name)
{
    return getUniform(name).cast<Uniform4b>();
}

inline ptr<UniformMatrix2f> UniformBlock::getUniformMatrix2f(const string &name)
{
    return getUniform(name).cast<UniformMatrix2f>();
}

inline ptr<UniformMatrix2d> UniformBlock::getUniformMatrix2d(const string &name)
{
    return getUniform(name).cast<UniformMatrix2d>();
}

inline ptr<UniformMatrix3f> UniformBlock::getUniformMatrix3f(const string &name)
{
    return getUniform(name).cast<UniformMatrix3f>();
}

inline ptr<UniformMatrix3d> UniformBlock::getUniformMatrix3d(const string &name)
{
    return getUniform(name).cast<UniformMatrix3d>();
}

inline ptr<UniformMatrix4f> UniformBlock::getUniformMatrix4f(const string &name)
{
    return getUniform(name).cast<UniformMatrix4f>();
}

inline ptr<UniformMatrix4d> UniformBlock::getUniformMatrix4d(const string &name)
{
    return getUniform(name).cast<UniformMatrix4d>();
}

inline ptr<UniformMatrix2x3f> UniformBlock::getUniformMatrix2x3f(const string &name)
{
    return getUniform(name).cast<UniformMatrix2x3f>();
}

inline ptr<UniformMatrix2x3d> UniformBlock::getUniformMatrix2x3d(const string &name)
{
    return getUniform(name).cast<UniformMatrix2x3d>();
}

inline ptr<UniformMatrix2x4f> UniformBlock::getUniformMatrix2x4f(const string &name)
{
    return getUniform(name).cast<UniformMatrix2x4f>();
}

inline ptr<UniformMatrix2x4d> UniformBlock::getUniformMatrix2x4d(const string &name)
{
    return getUniform(name).cast<UniformMatrix2x4d>();
}

inline ptr<UniformMatrix3x2f> UniformBlock::getUniformMatrix3x2f(const string &name)
{
    return getUniform(name).cast<UniformMatrix3x2f>();
}

inline ptr<UniformMatrix3x2d> UniformBlock::getUniformMatrix3x2d(const string &name)
{
    return getUniform(name).cast<UniformMatrix3x2d>();
}

inline ptr<UniformMatrix3x4f> UniformBlock::getUniformMatrix3x4f(const string &name)
{
    return getUniform(name).cast<UniformMatrix3x4f>();
}

inline ptr<UniformMatrix3x4d> UniformBlock::getUniformMatrix3x4d(const string &name)
{
    return getUniform(name).cast<UniformMatrix3x4d>();
}

inline ptr<UniformMatrix4x2f> UniformBlock::getUniformMatrix4x2f(const string &name)
{
    return getUniform(name).cast<UniformMatrix4x2f>();
}

inline ptr<UniformMatrix4x2d> UniformBlock::getUniformMatrix4x2d(const string &name)
{
    return getUniform(name).cast<UniformMatrix4x2d>();
}

inline ptr<UniformMatrix4x3f> UniformBlock::getUniformMatrix4x3f(const string &name)
{
    return getUniform(name).cast<UniformMatrix4x3f>();
}

inline ptr<UniformMatrix4x3d> UniformBlock::getUniformMatrix4x3d(const string &name)
{
    return getUniform(name).cast<UniformMatrix4x3d>();
}

inline ptr<UniformSampler> UniformBlock::getUniformSampler(const string &name)
{
    return getUniform(name).cast<UniformSampler>();
}

#undef SETVALUE

}

#endif
