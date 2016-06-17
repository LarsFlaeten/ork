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

#ifndef _ORK_VALUE_H_
#define _ORK_VALUE_H_

#include <string>

#include "ork/math/vec2.h"
#include "ork/math/vec3.h"
#include "ork/math/vec4.h"
#include "ork/math/mat3.h"
#include "ork/math/mat4.h"
#include "ork/render/Texture.h"
#include "ork/render/GPUBuffer.h"

namespace ork
{

/**
 * An abstract value.
 * @ingroup render
 */
class ORK_API Value : public Object
{
public:
    /**
     * Deletes this value.
     */
    virtual ~Value();

    /**
     * Returns the type of this value.
     */
    virtual UniformType getType() const = 0;

    /**
     * Returns the name of this value.
     */
    std::string getName() const;

protected:
    /**
     * The name of this value.
     */
    std::string name;

    /**
     * Creates an uninitialized value.
     */
    Value(const char *type, const std::string &name);
};

// ----------------------------------------------------------------------------

/**
 * A Value holding a single number.
 * @ingroup render
 */
template<UniformType U, class T, const char *V>
class ORK_API Value1 : public Value
{
public:
    /**
     * Creates an uninitialized Value1.
     */
    Value1(const std::string &name) : Value(V, name)
    {
    }

    /**
     * Creates a Value1.
     */
    Value1(const std::string &name, T value) : Value(V, name), value(value)
    {
    }

    /**
     * Deletes this Value1.
     */
    virtual ~Value1()
    {
    }

    virtual UniformType getType() const
    {
        return U;
    }

    /**
     * Returns the current value of this Value1.
     */
    T get() const
    {
        return value;
    }

    /**
     * Sets the value of this Value1.
     *
     * @param value the new value for this Value1.
     */
    void set(T value)
    {
        this->value = value;
    }

private:
    /**
     * The current value of this Value1.
     */
    T value;
};

extern const char value1f[];

extern const char value1d[];

extern const char value1i[];

extern const char value1ui[];

extern const char value1b[];

/**
 * A Value holding a single float value.
 * @ingroup render
 */
typedef Value1<VEC1F, GLfloat, value1f> Value1f;

/**
 * A Value holding a single double value.
 * @ingroup render
 */
typedef Value1<VEC1D, GLdouble, value1d> Value1d;

/**
 * A Value holding a single signed integer value.
 * @ingroup render
 */
typedef Value1<VEC1I, GLint, value1i> Value1i;

/**
 * A Value holding a single unsigned integer value.
 * @ingroup render
 */
typedef Value1<VEC1UI, GLuint, value1ui> Value1ui;

/**
 * A Value holding a single boolean value.
 * @ingroup render
 */
typedef Value1<VEC1B, bool, value1b> Value1b;

// ----------------------------------------------------------------------------

/**
 * A Value holding a two vector value.
 * @ingroup render
 */
template<UniformType U, class T,  const char *V>
class ORK_API Value2 : public Value
{
public:
    /**
     * Creates an uninitialized Value2.
     */
    Value2(const std::string &name) : Value(V, name)
    {
    }

    /**
     * Creates a Value2.
     */
    Value2(const std::string &name, const vec2<T> &value) : Value(V, name), value(value)
    {
    }

    /**
     * Deletes this Value2.
     */
    virtual ~Value2()
    {
    }

    virtual UniformType getType() const
    {
        return U;
    }

    /**
     * Returns the current value of this Value2.
     */
    vec2<T> get() const
    {
        return value;
    }

    /**
     * Sets the value of this Value2.
     *
     * @param value the new value for this Value2.
     */
    void set(const vec2<T> &value)
    {
        this->value = value;
    }

private:
    /**
     * The current value of this Value2.
     */
    vec2<T> value;
};

extern const char value2f[];

extern const char value2d[];

extern const char value2i[];

extern const char value2ui[];

extern const char value2b[];

/**
 * A Value holding a single precision two vector.
 * @ingroup render
 */
typedef Value2<VEC2F, GLfloat, value2f> Value2f;

/**
 * A Value holding a double precision two vector.
 * @ingroup render
 */
typedef Value2<VEC2B, GLdouble, value2d> Value2d;

/**
 * A Value holding a signed integer two vector.
 * @ingroup render
 */
typedef Value2<VEC2I, GLint, value2i> Value2i;

/**
 * A Value holding an unsigned integer two vector.
 * @ingroup render
 */
typedef Value2<VEC2UI, GLuint, value2ui> Value2ui;

/**
 * A Value holding a boolean two vector.
 * @ingroup render
 */
typedef Value2<VEC2B, bool, value2b> Value2b;

// ----------------------------------------------------------------------------

/**
 * A Value holding a three vector value.
 * @ingroup render
 */
template<UniformType U, class T,  const char *V>
class ORK_API Value3 : public Value
{
public:
    /**
     * Creates an uninitialized Value3.
     */
    Value3(const std::string &name) : Value(V, name)
    {
    }

    /**
     * Creates a Value3.
     */
    Value3(const std::string &name, const vec3<T> &value) : Value(V, name), value(value)
    {
    }

    /**
     * Deletes this Value3.
     */
    virtual ~Value3()
    {
    }

    virtual UniformType getType() const
    {
        return U;
    }

    /**
     * Returns the current value of this Value3.
     */
    vec3<T> get() const
    {
        return value;
    }

    /**
     * Sets the value of this Value3.
     *
     * @param value the new value for this Value3.
     */
    void set(const vec3<T> &value)
    {
        this->value = value;
    }

private:
    /**
     * The current value of this Value3.
     */
    vec3<T> value;
};

extern const char value3f[];

extern const char value3d[];

extern const char value3i[];

extern const char value3ui[];

extern const char value3b[];

/**
 * A Value holding a single precision three vector.
 * @ingroup render
 */
typedef Value3<VEC3F, GLfloat, value3f> Value3f;

/**
 * A Value holding a double precision three vector.
 * @ingroup render
 */
typedef Value3<VEC3B, GLdouble, value3d> Value3d;

/**
 * A Value holding a signed integer three vector.
 * @ingroup render
 */
typedef Value3<VEC3I, GLint, value3i> Value3i;

/**
 * A Value holding an unsigned integer three vector.
 * @ingroup render
 */
typedef Value3<VEC3UI, GLuint, value3ui> Value3ui;

/**
 * A Value holding a boolean three vector.
 * @ingroup render
 */
typedef Value3<VEC3B, bool, value3b> Value3b;

// ----------------------------------------------------------------------------

/**
 * A Value holding a four vector value.
 * @ingroup render
 */
template<UniformType U, class T,  const char *V>
class ORK_API Value4 : public Value
{
public:
    /**
     * Creates an uninitialized Value4.
     */
    Value4(const std::string &name) : Value(V, name)
    {
    }

    /**
     * Creates a Value4.
     */
    Value4(const std::string &name, const vec4<T> &value) : Value(V, name), value(value)
    {
    }

    /**
     * Deletes this Value4.
     */
    virtual ~Value4()
    {
    }

    virtual UniformType getType() const
    {
        return U;
    }

    /**
     * Returns the current value of this Value4.
     */
    vec4<T> get() const
    {
        return value;
    }

    /**
     * Sets the value of this Value4.
     *
     * @param value the new value for this Value4.
     */
    void set(const vec4<T> &value)
    {
        this->value = value;
    }

private:
    /**
     * The current value of this Value4.
     */
    vec4<T> value;
};

extern const char value4f[];

extern const char value4d[];

extern const char value4i[];

extern const char value4ui[];

extern const char value4b[];

/**
 * A Value holding a single precision four vector.
 * @ingroup render
 */
typedef Value4<VEC4F, GLfloat, value4f> Value4f;

/**
 * A Value holding a double precision four vector.
 * @ingroup render
 */
typedef Value4<VEC4B, GLdouble, value4d> Value4d;

/**
 * A Value holding a signed integer four vector.
 * @ingroup render
 */
typedef Value4<VEC4I, GLint, value4i> Value4i;

/**
 * A Value holding an unsigned integer four vector.
 * @ingroup render
 */
typedef Value4<VEC4UI, GLuint, value4ui> Value4ui;

/**
 * A Value holding a boolean four vector.
 * @ingroup render
 */
typedef Value4<VEC4B, bool, value4b> Value4b;

// ----------------------------------------------------------------------------

/**
 * A Value holding a matrix value.
 * @ingroup render
 */
template<UniformType U, class T, int C, int R, const char *V>
class ORK_API ValueMatrix : public Value
{
public:
    /**
     * Creates an uninitialized ValueMatrix.
     */
    ValueMatrix(const std::string &name) : Value(V, name)
    {
    }

    /**
     * Creates a ValueMatrix.
     */
    ValueMatrix(const std::string &name, const T* value) : Value(V, name)
    {
        set(value);
    }

    /**
     * Deletes this ValueMatrix.
     */
    virtual ~ValueMatrix()
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
        for (int i = 0; i < R * C; ++i) {
            this->value[i] = value[i];
        }
    }

protected:
    /**
     * The current value of this ValueMatrix.
     * The matrix coefficients are stored in row major order.
     */
    T value[C * R];
};

/**
 * A Value holding a 3x3 matrix value.
 * @ingroup render
 */
template<UniformType U, class T, const char *V>
class ValueMatrix3 : public ValueMatrix<U, T, 3, 3, V>
{
public:
    /**
     * Creates an uninitialized ValueMatrix3.
     */
    ValueMatrix3(const std::string &name) : ValueMatrix<U, T, 3, 3, V>(name)
    {
    }

    /**
     * Creates a ValueMatrix3.
     */
    ValueMatrix3(const std::string &name, const mat3<T> &value) : ValueMatrix<U, T, 3, 3, V>(name)
    {
        setMatrix(value);
    }

    /**
     * Returns the current value of this uniform.
     */
    mat3<T> getMatrix() const
    {
        return mat3<T>(ValueMatrix<U, T, 3, 3, V>::get());
    }

    /**
     * Sets the value of this uniform.
     *
     * @param value the new value for this uniform.
     */
    void setMatrix(const mat3<T> &value)
    {
        ValueMatrix<U, T, 3, 3, V>::set(value.coefficients());
    }
};

/**
 * A Value holding a 4x4 matrix value.
 * @ingroup render
 */
template<UniformType U, class T, const char *V>
class ValueMatrix4 : public ValueMatrix<U, T, 4, 4, V>
{
public:
    /**
     * Creates an uninitialized ValueMatrix4.
     */
    ValueMatrix4(const std::string &name) : ValueMatrix<U, T, 4, 4, V>(name)
    {
    }

    /**
     * Creates a ValueMatrix4.
     */
    ValueMatrix4(const std::string &name, const mat4<T> &value) : ValueMatrix<U, T, 4, 4, V>(name)
    {
        setMatrix(value);
    }

    /**
     * Returns the current value of this uniform.
     */
    mat4<T> getMatrix() const
    {
        return mat4<T>(ValueMatrix<U, T, 4, 4, V>::get());
    }

    /**
     * Sets the value of this uniform.
     *
     * @param value the new value for this uniform.
     */
    void setMatrix(const mat4<T> &value)
    {
        ValueMatrix<U, T, 4, 4, V>::set(value.coefficients());
    }
};

extern const char valueMatrix2f[];

extern const char valueMatrix3f[];

extern const char valueMatrix4f[];

extern const char valueMatrix2x3f[];

extern const char valueMatrix2x4f[];

extern const char valueMatrix3x2f[];

extern const char valueMatrix3x4f[];

extern const char valueMatrix4x2f[];

extern const char valueMatrix4x3f[];

extern const char valueMatrix2d[];

extern const char valueMatrix3d[];

extern const char valueMatrix4d[];

extern const char valueMatrix2x3d[];

extern const char valueMatrix2x4d[];

extern const char valueMatrix3x2d[];

extern const char valueMatrix3x4d[];

extern const char valueMatrix4x2d[];

extern const char valueMatrix4x3d[];

/**
 * A Value holding a single precision 2x2 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT2F, GLfloat, 2, 2, valueMatrix2f> ValueMatrix2f;

/**
 * A Value holding a single precision 3x3 matrix.
 * @ingroup render
 */
typedef ValueMatrix3<MAT3F, GLfloat, valueMatrix3f> ValueMatrix3f;

/**
 * A Value holding a single precision 4x4 matrix.
 * @ingroup render
 */
typedef ValueMatrix4<MAT4F, GLfloat, valueMatrix4f> ValueMatrix4f;

/**
 * A Value holding a single precision 2x3 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT2x3F, GLfloat, 2, 3, valueMatrix2x3f> ValueMatrix2x3f;

/**
 * A Value holding a single precision 2x4 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT2x4F, GLfloat, 2, 4, valueMatrix2x4f> ValueMatrix2x4f;

/**
 * A Value holding a single precision 3x2 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT3x2F, GLfloat, 3, 2, valueMatrix3x2f> ValueMatrix3x2f;

/**
 * A Value holding a single precision 3x4 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT3x4F, GLfloat, 3, 4, valueMatrix3x4f> ValueMatrix3x4f;

/**
 * A Value holding a single precision 4x2 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT4x2F, GLfloat, 4, 2, valueMatrix4x2f> ValueMatrix4x2f;

/**
 * A Value holding a single precision 4x3 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT4x3F, GLfloat, 4, 3, valueMatrix4x3f> ValueMatrix4x3f;

/**
 * A Value holding a double precision 2x2 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT2D, GLdouble, 2, 2, valueMatrix2d> ValueMatrix2d;

/**
 * A Value holding a double precision 3x3 matrix.
 * @ingroup render
 */
typedef ValueMatrix3<MAT3D, GLdouble, valueMatrix3d> ValueMatrix3d;

/**
 * A Value holding a double precision 4x4 matrix.
 * @ingroup render
 */
typedef ValueMatrix4<MAT4D, GLdouble, valueMatrix4d> ValueMatrix4d;

/**
 * A Value holding a double precision 2x3 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT2x3D, GLdouble, 2, 3, valueMatrix2x3d> ValueMatrix2x3d;

/**
 * A Value holding a double precision 2x4 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT2x4D, GLdouble, 2, 4, valueMatrix2x4d> ValueMatrix2x4d;

/**
 * A Value holding a double precision 3x2 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT3x2D, GLdouble, 3, 2, valueMatrix3x2d> ValueMatrix3x2d;

/**
 * A Value holding a double precision 3x4 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT3x4D, GLdouble, 3, 4, valueMatrix3x4d> ValueMatrix3x4d;

/**
 * A Value holding a double precision 4x2 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT4x2D, GLdouble, 4, 2, valueMatrix4x2d> ValueMatrix4x2d;

/**
 * A Value holding a double precision 4x3 matrix.
 * @ingroup render
 */
typedef ValueMatrix<MAT4x3D, GLdouble, 4, 3, valueMatrix4x3d> ValueMatrix4x3d;

// ----------------------------------------------------------------------------

/**
 * A Value holding a Texture value.
 * @ingroup render
 */
class ORK_API ValueSampler : public Value
{
public:
    /**
     * Creates an uninitialized ValueSampler.
     */
    ValueSampler(UniformType type, const std::string &name);

    /**
     * Creates a ValueSampler.
     */
    ValueSampler(UniformType type, const std::string &name, ptr<Texture> value);

    /**
     * Deletes this ValueSampler.
     */
    virtual ~ValueSampler();

    virtual UniformType getType() const;

    /**
     * Returns the current value of this ValueSampler.
     */
    ptr<Texture> get() const;

    /**
     * Sets the value of this uniform.
     *
     * @param value the new value for this ValueSampler.
     */
    void set(const ptr<Texture> value);

private:
    /**
     * The type of this ValueSampler.
     */
    UniformType type;

    /**
     * The current value of this ValueSampler.
     */
    ptr<Texture> value;
};

// ----------------------------------------------------------------------------

/**
 * A Value holding a subroutine value.
 * @ingroup render
 */
class ORK_API ValueSubroutine : public Value
{
public:
    /**
     * Creates an uninitialized ValueSubroutine.
     */
    ValueSubroutine(Stage stage, const std::string &name);

    /**
     * Creates a ValueSubroutine.
     */
    ValueSubroutine(Stage stage, const std::string &name, const std::string &value);

    /**
     * Deletes this ValueSubroutine.
     */
    virtual ~ValueSubroutine();

    virtual UniformType getType() const;

    /**
     * Returns the shader type for this subroutine (vertex, fragment, etc).
     */
    Stage getStage() const;

    /**
     * Returns the current value of this ValueSubroutine.
     */
    std::string get() const;

    /**
     * Sets the value of this uniform.
     *
     * @param value the new value for this ValueSubroutine.
     */
    void set(const std::string &value);

private:
    /**
     * The type of this ValueSubroutine.
     */
    UniformType type;

    /**
     * The shader type for this subroutine (vertex, fragment, etc).
     */
    Stage stage;

    /**
     * The current value of this ValueSubroutine.
     */
    std::string value;
};

}

#endif
