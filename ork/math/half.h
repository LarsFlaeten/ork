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
