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

#ifndef _ORK_VEC2_H_
#define _ORK_VEC2_H_

#include "pmath.h"
#include "half.h"
#include <cassert>

namespace ork
{

/**
 * @defgroup math math
 * @ingroup ork
 * Provides classes for vectors, matrices, segments, bounding boxes, etc.
 */

/**
 * A 2D vector.
 * @ingroup math
 */
template <typename type> class vec2
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
     * Creates a new, uninitialized vector.
     */
    vec2();

    /**
     * Creates a new vector with the given coordinates.
     */
    vec2(type xi, type yi);

    /**
     * Creates a new vector with the given coordinates.
     */
    vec2(const type v[2]);

    /**
     * Creates a new vector as a copy of the given vector.
     */
    vec2(const vec2& v);

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
    void operator=(const vec2& v);

    /**
     * Returns true if this vector is equal to the given vector.
     */
    bool operator==(const vec2& v) const;

    /**
     * Returns true if this vector is different from the given vector.
     */
    bool operator!=(const vec2& v) const;

    /**
     * Returns the sum of this vector and of the given vector.
     */
    vec2 operator+(const vec2& v) const;

    /**
     * Returns the difference of this vector and of the given vector.
     */
    vec2 operator-(const vec2& v) const;

    /**
     * Returns the product of this vector and of the given vector. The
     * product is done component by component.
     */
    vec2 operator*(const vec2& v) const;

    /**
     * Returns the product of this vector and of the given scalar.
     */
    vec2 operator*(const type scalar) const;

    /**
     * Returns the division of this vector and of the given vector. The
     * division is done component by component.
     */
    vec2 operator/(const vec2& v) const;

    /**
     * Returns the division of this vector and of the given scalar.
     */
    vec2 operator/(const type scalar) const;

    /**
     * Returns the opposite of this vector.
     */
    vec2 operator-() const;

    /**
     * Adds the given vector to this vector.
     */
    vec2& operator+=(const vec2& v);

    /**
     * Substracts the given vector from this vector.
     */
    vec2& operator-=(const vec2& v);

    /**
     * Multiplies this vector by the given scalar.
     */
    vec2& operator*=(const type scalar);

    /**
     * Divides this vector by the given scalar.
     */
    vec2& operator/=(const type scalar);

    /**
     * Compare first element, then second if equal.
     */
    bool operator<(const vec2& v) const;

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
    type dotproduct(const vec2& v) const;

    /**
     * Alias for dotproduct, here for compatibility
     */
    type dot(const vec2& v) const;

    /**
     * Returns this vector normalized to unit length.
     */
    vec2 normalize() const;

    /**
     * Returns this vector normalized to the given length.
     */
    vec2 normalize(type l) const;

    /**
     * Returns this vector normalized to unit length. Returns also its initial length.
     */
    vec2 normalize(type *previousLength) const;

    /**
     * Returns this vector normalized to the given length. Returns also its initial length.
     */
    vec2 normalize(type l, type *previousLength);

    /**
     * Casts this vector to another base type.
     */
    template <class t>
    vec2<t> cast() {
        return vec2<t>((t) x, (t) y);
    }

    /**
     * The null vector (0,0).
     */
    static const vec2 ZERO;

    /**
     * The unit x vector (1,0).
     */
    static const vec2 UNIT_X;

    /**
     * The unit y vector (0,1).
     */
    static const vec2 UNIT_Y;
};

/**
 * A 2D vector with half float coordinates.
 * @ingroup math
 */
typedef vec2<half> vec2h;

/**
 * A 2D vector with float coordinates.
 * @ingroup math
 */
typedef vec2<float> vec2f;

/**
 * A 2D vector with double coordinates.
 * @ingroup math
 */
typedef vec2<double> vec2d;

/**
 * A 2D vector with int coordinates.
 * @ingroup math
 */
typedef vec2<int> vec2i;

template <typename type>
inline vec2<type>::vec2()
{
}

template <typename type>
inline vec2<type>::vec2(type xi, type yi) : x(xi), y(yi)
{
}

template <typename type>
inline vec2<type>::vec2(const type v[2]) : x(v[0]), y(v[1])
{
}

template <typename type>
inline vec2<type>::vec2(const vec2& v) : x(v.x), y(v.y)
{
}

template <typename type>
inline type vec2<type>::operator[](const int i) const
{
    //assert(i<2);
    return *(&x + i);
}

template <typename type>
inline type& vec2<type>::operator[](const int i)
{
    //assert(i<2);
    return *(&x + i);
}

template <typename type>
inline void vec2<type>::operator=(const vec2<type>& v)
{
    x = v.x;
    y = v.y;
}

template <typename type>
inline bool vec2<type>::operator==(const vec2<type>& v) const
{
    return (x == v.x && y == v.y);
}

template <typename type>
inline bool vec2<type>::operator!=(const vec2<type>& v) const
{
    return (x != v.x || y != v.y);
}

template <typename type>
inline vec2<type> vec2<type>::operator+(const vec2<type>& v) const
{
    return vec2(x + v.x, y + v.y);
}

template <typename type>
inline vec2<type> vec2<type>::operator-(const vec2<type>& v) const
{
    return vec2(x - v.x, y - v.y);
}

template <typename type>
inline vec2<type> vec2<type>::operator*(const vec2<type>& v) const
{
    return vec2(x * v.x, y * v.y);
}

template <typename type>
inline vec2<type> vec2<type>::operator*(const type scalar) const
{
    return vec2(x * scalar, y * scalar);
}

template <typename type>
inline vec2<type> vec2<type>::operator/(const vec2<type>& v) const
{
    return vec2(x / v.x, y / v.y);
}

template <typename type>
inline vec2<type> vec2<type>::operator/(const type scalar) const
{
    assert(scalar != 0);
    type inv = 1 / scalar;
    return vec2(x * inv, y * inv);
}

template <typename type>
inline vec2<type> vec2<type>::operator-() const
{
    return vec2(-x, -y);
}

template <typename type>
inline vec2<type>& vec2<type>::operator+=(const vec2<type>& v)
{
    x += v.x;
    y += v.y;
    return *this;
}

template <typename type>
inline vec2<type>& vec2<type>::operator-=(const vec2<type>& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

template <typename type>
inline vec2<type>& vec2<type>::operator*=(const type scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

template <typename type>
inline vec2<type>& vec2<type>::operator/=(const type scalar)
{
    assert(scalar != 0);
    type inv = 1 / scalar;
    x *= inv;
    y *= inv;
    return *this;
}

template <typename type>
inline bool vec2<type>::operator<(const vec2<type>& v) const
{
    if (x < v.x) {
        return true;
    }
    if (x > v.x) {
        return false;
    }
    return y < v.y;
}

template <typename type>
inline type vec2<type>::length() const
{
    return (type) sqrt(float(x * x + y * y));
}

template <typename type>
inline type vec2<type>::squaredLength() const
{
    return x * x + y * y;
}

template <typename type>
inline type vec2<type>::dotproduct(const vec2<type>& v) const
{
    return (x*v.x + y*v.y);
}

template <typename type>
inline type vec2<type>::dot(const vec2<type>& v) const
{
    return (x*v.x + y*v.y);
}

template <typename type>
inline vec2<type> vec2<type>::normalize() const
{
    type length = sqrt(x * x + y * y);
    type invLength = 1.0 / length;
    return vec2(x * invLength, y * invLength);
}

template <typename type>
inline vec2<type> vec2<type>::normalize(type l) const
{
    type length = sqrt(x * x + y * y);
    type invLength = l / length;
    return vec2(x * invLength, y * invLength);
}

template <typename type>
inline vec2<type> vec2<type>::normalize(type *previousLength) const
{
    *previousLength = sqrt(x * x + y * y);
    type invLength = 1.0 / *previousLength;
    return vec2(x * invLength, y * invLength);
}

template <typename type>
inline vec2<type> vec2<type>::normalize(type l, type *previousLength)
{
    *previousLength = sqrt(x * x + y * y);
    type invLength = l / *previousLength;
    return vec2(x * invLength, y * invLength);
}

template <typename type>
const vec2<type> vec2<type>::ZERO(0, 0);

template <typename type>
const vec2<type> vec2<type>::UNIT_X(1, 0);

template <typename type>
const vec2<type> vec2<type>::UNIT_Y(0, 1);

}

#undef PREDICATE_BOOL_FUNCTION

#endif
