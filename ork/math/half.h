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

#ifndef _HALF_FLOAT_H_
#define _HALF_FLOAT_H_

namespace ork
{

// You can use these conversions directly, but it's far better to use the half type

/**
 * Converts a float to its half-float representation.
 * Expect inaccuracies and infinities to occur.
 * @ingroup math
 */
ORK_API unsigned short floatToHalf(float x);

/**
 * Converts a half-float representation back to a float.
 * @ingroup math
 */
ORK_API float halfToFloat(unsigned short h);

/**
 *
 * A 16-bit floating point number. Contains 1 sign bit, 5 biased exponent bit, and
 * 10 mantissa bits.
 *
 * @ingroup math
 */
struct half
{
public:
    /**
     * Constructs an uninitialized half.
     */
    half()
    {
    }

    /**
     * Constructs a half from a 32-bits floating point value.
     * Allows to write: half x = 10.f;
     */
    half(float x);

    /**
     * Converts to a 32-bits floating point value.
     */
    inline float toFloat() const;

    /**
     * Overloaded float cast (allows to write: float x = half).
     */
    inline operator float() const;

    // Half-float arithmetic.

    /**
     * Equality.
     */
    inline half operator==(const half v) const;

    /**
     * Difference.
     */
    inline bool operator!=(const half v) const;

    /**
     * Addition.
     */
    inline half operator+(const half v) const;

    /**
     * Subtraction.
     */
    inline half operator-(const half v) const;

    /**
     * Multiplication.
     */
    inline half operator*(const half v) const;

    /**
     * Divide.
     */
    inline half operator/(const half v) const;

    /**
     * Test for NaN.
     */
    inline bool isNaN() const;

    /**
     * Test for not (NaN or infinity).
     */
    inline bool isFinite() const;

    /**
     * Test for infinity.
     */
    inline bool isInf() const;

    /**
     * Test for non-denormals.
     */
    inline bool isNormal() const;

private:
    /**
     * The value of this half, encoded in an unsigned short.
     */
    unsigned short value;

    /**
     * Gets the 5-bits 15-biased exponent.
     */
    inline int exponent() const;

    /**
     * Gets the 10-bits mantissa.
     */
    inline int mantissa() const;
};

inline half::half(float x) : value(floatToHalf(x))
{
}

inline float half::toFloat() const
{
    return halfToFloat(value);
}

inline half::operator float() const
{
    return toFloat();
}

inline half half::operator==(const half v) const
{
    return value == v.value;
}

bool half::operator!=(const half v) const
{
    return value != v.value;
}

inline half half::operator+(const half v) const
{
    return half(toFloat() + v.toFloat());
}

inline half half::operator-(const half v) const
{
    return half(toFloat() - v.toFloat());
}

inline half half::operator*(const half v) const
{
    return half(toFloat() * v.toFloat());
}

inline half half::operator/(const half v) const
{
    return half(toFloat() / v.toFloat());
}

inline bool half::isNaN() const
{
    return (exponent() == 31) && (mantissa() != 0);
}

inline bool half::isFinite() const
{
    return (exponent() != 31);
}

inline bool half::isInf() const
{
    return (exponent() == 31) && (mantissa() == 0);
}

inline bool half::isNormal() const
{
    return isFinite() && ( (exponent() != 0) || (mantissa() == 0) );
}

inline int half::exponent() const
{
    return (value & 0x7C00) >> 10;
}

inline int half::mantissa() const
{
    return value & 0x03ff;
}

// faster isNaN without conversion
// to be used by half vector templates
inline bool isNaN(half x)
{
    return x.isNaN();
}

}

#endif
