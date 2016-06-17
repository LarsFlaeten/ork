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

#ifndef _ORK_VEC3_H_
#define _ORK_VEC3_H_

#include <cassert>

#include "pmath.h"
#include "half.h"

#include "ork/math/vec2.h"

namespace ork
{

/**
 * A 3D vector.
 * @ingroup math
 */
template <typename type> class vec3
{
public:
    /**
     * x coordinate.
     */
    type x;

    /**
     * x coordinate.
     */
    type y;

    /**
     * z coordinate.
     */
    type z;

    /**
     * Creates a new, uninitialized vector.
     */
    vec3();

    /**
     * Creates a new vector with the given coordinates.
     */
    vec3(type xi, type yi, type zi);

    /**
     * Creates a new vector with the given coordinates.
     */
    vec3(const type v[3]);

    /**
     * Creates a new vector as a copy of the given vector.
     */
    vec3(const vec3& v);

    /**
     * Returns the coordinate of this vector whose index is given.
     */
    type operator[](const int i) const;

    /**
     * Returns the coordinate of this vector whose index is given.
     */
    type& operator[](const int i);

    /**
     * Assigns the given vector to this vector.
     */
    void operator=(const vec3& v);

    /**
     * Returns true if this vector is equal to the given vector.
     */
    bool operator==(const vec3& v) const;

    /**
     * Returns true if this vector is different from the given vector.
     */
    bool operator!=(const vec3& v) const;

    /**
     * Returns the sum of this vector and of the given vector.
     */
    vec3 operator+(const vec3& v) const;

    /**
     * Returns the difference of this vector and of the given vector.
     */
    vec3 operator-(const vec3& v) const;

    /**
     * Returns the product of this vector and of the given vector. The
     * product is done component by component.
     */
    vec3 operator*(const vec3& v) const;

    /**
     * Returns the product of this vector and of the given scalar.
     */
    vec3 operator*(const type scalar) const;

    /**
     * Returns the division of this vector and of the given vector. The
     * division is done component by component.
     */
    vec3 operator/(const vec3& v) const;

    /**
     * Returns the division of this vector and of the given scalar.
     */
    vec3 operator/(const type scalar) const;

    /**
     * Returns the opposite of this vector.
     */
    vec3 operator-() const;

    /**
     * Adds the given vector to this vector.
     */
    vec3& operator+=(const vec3& v);

    /**
     * Substracts the given vector from this vector.
     */
    vec3& operator-=(const vec3& v);

    /**
     * Multiplies this vector by the given scalar.
     */
    vec3& operator*=(const type scalar);

    /**
     * Divides this vector by the given scalar.
     */
    vec3& operator/=(const type scalar);

    /**
     * Compare first element, then second if equal, then third if equal.
     */
    bool operator<(const vec3& v) const;

    /**
     * Returns the length of this vector.
     */
    type length() const;

    /**
     * Returns the squared length of this vector.
     */
    type squaredLength() const;

    /**
     * Returns the dot product of this vector and of the given vector.
     */
    type dotproduct(const vec3& v) const;

    /**
     * Returns this vector normalized to unit length.
     */
    vec3 normalize() const;

    /**
     * Returns this vector normalized to the given length.
     */
    vec3 normalize(type l) const;

    /**
     * Returns this vector normalized to unit length. Returns also its initial length.
     */
    vec3 normalize(type *previousLength) const;

    /**
     * Returns this vector normalized to the given length. Returns also its initial length.
     */
    vec3 normalize(type l, type *previousLength);

    /**
     * Returns he cross product of this vector and of the given vector.
     */
    vec3 crossProduct(const vec3& v) const;

    /**
     * Returns the 2D vector defined by (x,y).
     */
    vec2<type> xy() const;

    /**
     * Casts this vector to another base type.
     */
    template <class t>
    vec3<t> cast() const {
        return vec3<t>((t) x, (t) y, (t) z);
    }

    /**
     * The null vector (0,0,0).
     */
    static const vec3 ZERO;

    /**
     * The unit x vector (1,0,0).
     */
    static const vec3 UNIT_X;

    /**
     * The unit y vector (0,1,0).
     */
    static const vec3 UNIT_Y;

    /**
     * The unit z vector (0,0,1).
     */
    static const vec3 UNIT_Z;
};

/**
 * A 3D vector with half float coordinates.
 * @ingroup math
 */
typedef vec3<half> vec3h;

/**
 * A 3D vector with float coordinates.
 * @ingroup math
 */
typedef vec3<float> vec3f;

/**
 * A 3D vector with double coordinates.
 * @ingroup math
 */
typedef vec3<double> vec3d;

/**
 * A 3D vector with int coordinates.
 * @ingroup math
 */
typedef vec3<int> vec3i;

template <typename type>
inline vec3<type>::vec3()
{
}

template <typename type>
inline vec3<type>::vec3(type xi, type yi, type zi) : x(xi), y(yi), z(zi)
{
}

template <typename type>
inline vec3<type>::vec3(const type v[3]) : x(v[0]), y(v[1]), z(v[2])
{
}

template <typename type>
inline vec3<type>::vec3(const vec3& v) : x(v.x), y(v.y), z(v.z)
{
}

template <typename type>
inline type vec3<type>::operator[](const int i) const
{
    //assert(i<3);
    return *(&x + i);
}

template <typename type>
inline type& vec3<type>::operator[](const int i)
{
    //assert(i<3);
    return *(&x + i);
}

template <typename type>
inline void vec3<type>::operator=(const vec3<type>& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

template <typename type>
inline bool vec3<type>::operator==(const vec3<type>& v) const
{
    return (x == v.x && y == v.y && z == v.z);
}

template <typename type>
inline bool vec3<type>::operator!=(const vec3<type>& v) const
{
    return (x != v.x || y != v.y || z != v.z);
}

template <typename type>
inline vec3<type> vec3<type>::operator+(const vec3<type>& v) const
{
    return vec3(x + v.x, y + v.y, z + v.z);
}

template <typename type>
inline vec3<type> vec3<type>::operator-(const vec3<type>& v) const
{
    return vec3(x - v.x, y - v.y, z - v.z);
}

template <typename type>
inline vec3<type> vec3<type>::operator*(const vec3<type>& v) const
{
    return vec3(x * v.x, y * v.y, z * v.z);
}

template <typename type>
inline vec3<type> vec3<type>::operator*(const type scalar) const
{
    return vec3(x * scalar, y * scalar, z * scalar);
}

template <typename type>
inline vec3<type> vec3<type>::operator/(const vec3<type>& v) const
{
    return vec3(x / v.x, y / v.y, z / v.z);
}

template <typename type>
inline vec3<type> vec3<type>::operator/(const type scalar) const
{
    assert(scalar != 0);
    type inv = 1 / scalar;
    return vec3(x * inv, y * inv, z * inv);
}

template <typename type>
inline vec3<type> vec3<type>::operator-() const
{
    return vec3(-x, -y, -z);
}

template <typename type>
inline vec3<type>& vec3<type>::operator+=(const vec3<type>& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

template <typename type>
inline vec3<type>& vec3<type>::operator-=(const vec3<type>& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

template <typename type>
inline vec3<type>& vec3<type>::operator*=(const type scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

template <typename type>
inline vec3<type>& vec3<type>::operator/=(const type scalar)
{
    assert(scalar != 0);
    type inv = 1 / scalar;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

template <typename type>
inline bool vec3<type>::operator<(const vec3<type>& v) const
{
    if (x < v.x) {
        return true;
    }
    if (x > v.x) {
        return false;
    }
    if (y < v.y) {
        return true;
    }
    if (y > v.y) {
        return false;
    }
    return z < v.z;
}

template <typename type>
inline type vec3<type>::length() const
{
    return sqrt(x*x + y*y + z*z);
}

template <typename type>
inline type vec3<type>::squaredLength() const
{
    return (x*x + y*y + z*z);
}

template <typename type>
inline type vec3<type>::dotproduct(const vec3<type>& v) const
{
    return (x*v.x + y*v.y + z*v.z);
}

template <typename type>
inline vec3<type> vec3<type>::normalize() const
{
    type length = sqrt(x * x + y * y + z * z);
    type invLength = (type)1.0 / length;
    return vec3(x * invLength, y * invLength, z * invLength);
}

template <typename type>
inline vec3<type> vec3<type>::normalize(type l) const
{
    type length = sqrt(x * x + y * y + z * z);
    type invLength = l / length;
    return vec3(x * invLength, y * invLength, z * invLength);
}

template <typename type>
inline vec3<type> vec3<type>::normalize(type *previousLength) const
{
    *previousLength = sqrt(x * x + y * y + z * z);
    type invLength = 1.0 / *previousLength;
    return vec3(x * invLength, y * invLength, z * invLength);
}

template <typename type>
inline vec3<type> vec3<type>::normalize(type l, type *previousLength)
{
    *previousLength = sqrt(x * x + y * y + z * z);
    type invLength = l / *previousLength;
    return vec3(x * invLength, y * invLength, z * invLength);
}

template <typename type>
inline vec3<type> vec3<type>::crossProduct(const vec3<type>& v) const
{
    return vec3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

template <typename type>
inline vec2<type> vec3<type>::xy() const
{
    return vec2<type>(x, y);
}

template <typename type>
const vec3<type> vec3<type>::ZERO(0, 0, 0);

template <typename type>
const vec3<type> vec3<type>::UNIT_X(1, 0, 0);

template <typename type>
const vec3<type> vec3<type>::UNIT_Y(0, 1, 0);

template <typename type>
const vec3<type> vec3<type>::UNIT_Z(0, 0, 1);

}

/**
 * Returns the product of this vector and of the given scalar.
 */
template <typename scalarType, typename vecType>
inline ork::vec3<vecType> operator*(const scalarType scalar, const ork::vec3<vecType> &v)
{
  return v * static_cast<vecType>(scalar);
}

#endif
