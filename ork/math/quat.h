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

#ifndef _QUAT_H_
#define _QUAT_H_

#include "pmath.h"

#include "ork/math/vec3.h"
#include "ork/math/mat3.h"
#include "ork/math/mat4.h"

namespace ork
{

/**
 * A templated quaternion class.
 * Important : assumes T is typeing point type.
 * @ingroup math
 */
template <typename type>
class quat {
public:
    /**
     * x coordinate.
     */
    type x;

    /**
     * y coordinate.
     */
    type y;

    /**
     * z coordinate.
     */
    type z;

    /**
     * w coordinate.
     */
    type w;

    /**
     * Creates a new, uninitialized quaternion.
     */
    quat();

    /**
     * Creates a new quaternion with the given coordinates (useless in general outside this header).
     */
    quat(type x, type y, type z, type w);

    /**
     * Creates a new quaternion with the given coordinates.
     */
    quat(const type v[4]);

    /**
     * Creates a new quaternion as a copy of the given quaternion.
     */
    quat(const quat& v);

    /**
     * Create a normalized quaternion from an axis and an angle (MOST USEFUL)
     * @param axis an axis (do not need to be normalized)
     * @param angle angle in radian
     */
    quat(const vec3<type>& axis, type angle);

    /**
     * Create a normalized quaternion which transform a vector into another.
     * No need to pass normalized vectors.
     */
    quat(const vec3<type>& to, const vec3<type>& from);

    /**
     * Create a normalized quaternion from a 3x3 rotation matrix.
     */
    explicit quat(const mat3<type> &m);

    /**
     * Multiply by another quaternion (replace a 3x3 rotation matrix multiplication).
     */
    quat operator*(const quat &a) const;

    /**
     * Multiply directly by a vector. Returns a rotated vector.
     */
    vec3<type> operator*(const vec3<type>& a) const;

    /**
     * Multiply by another quaternion (replace a 3x3 rotation matrix multiplication).
     */
    quat& operator*=(const quat &a );

    /**
     * Returns the inverse of this quaternion.
     */
    quat inverse() const;

    /**
     * If length is 1, this is a normalized quaternion (the interesting ones).
     */
    type length() const;

    /**
     * Returns this quaternion normalized to unit length.
     */
    quat normalize() const;

    /**
     * Returns the correponding 3x3 rotation matrix.
     */
    mat3<type> toMat3() const;

    /**
     * Returns the correponding 4x4 rotation matrix.
     */
    mat4<type> toMat4() const;

    /**
     * Returns the axis.
     */
    vec3<type> getAxis() const;

    /**
     * Returns the angle.
     */
    type getAngle() const;

    static const quat ONE;
};

/**
 * Quaternion using float.
 * @ingroup math
 */
typedef quat<float> quatf;

/**
 * Quaternion using double.
 * @ingroup math
 */
typedef quat<double> quatd;

// no, there is not quat<int>. Get over it.

// constructors

template <typename type>
inline quat<type>::quat( )
{
}

template <typename type>
inline quat<type>::quat(type xi, type yi, type zi, type wi) : x(xi), y(yi), z(zi), w(wi)
{
}

template <typename type>
inline quat<type>::quat(const type v[4]) : x(v[0]), y(v[1]), z(v[2]), w(v[3])
{
}

template <typename type>
inline quat<type>::quat(const quat& v) : x(v.x), y(v.y), z(v.z), w(v.w)
{
}

template <typename type>
inline quat<type>::quat(const vec3<type>& axis, type angle)
{
    vec3<type> axisN = axis.normalize();
    type a = angle * 0.5f;
    type sina = sin(a);
    type cosa = cos(a);
    x = axisN.x * sina;
    y = axisN.y * sina;
    z = axisN.z * sina;
    w = cosa;
}

template <typename type>
quat<type>::quat(const vec3<type>& to, const vec3<type>& from)
{
    vec3<type> f = from.normalize();
    vec3<type> t = to.normalize();

    type dotProdPlus1 = 1.0 + f.dotproduct(t);

    if (dotProdPlus1 < (type)1e-7) {
        w = 0;
        if (fabs(f.x) < 0.6) {
            double norm = sqrt(1 - f.x * f.x);
            x = 0;
            y = f.z / norm;
            z = -f.y / norm;
        } else if (fabs(f.y) < 0.6) {
            double norm = sqrt(1 - f.y * f.y);
            x = -f.z / norm;
            y = 0;
            z = f.x / norm;
        } else {
            double norm = sqrt(1 - f.z * f.z);
            x = f.y / norm;
            y = -f.x / norm;
            z = 0;
        }
    } else {
        type s = (type)sqrt(0.5 * dotProdPlus1);
        vec3<type> tmp = (f.crossProduct(t)) / (2.0 * s);
        x = tmp.x;
        y = tmp.y;
        z = tmp.z;
        w = s;
    }
}

template <typename type>
quat<type>::quat(const mat3<type> &m)
{
    type tr = m[0][0] + m[1][1] + m[2][2] + 1;

    if (tr > 0) {
        type s = 0.5 / sqrt(tr);
        w = 0.25 / s;
        x = (m[2][1] - m[1][2] ) * s;
        y = (m[0][2] - m[2][0] ) * s;
        z = (m[1][0] - m[0][1] ) * s;
    } else {
        if (m[1][1] > m[0][0] && m[2][2] <= m[1][1] ) {
            type s = sqrt((m[1][1] - (m[2][2] +m[0][0] )) + 1.0);
            y = s * 0.5;
            if (s != 0.0) {
                s = 0.5 / s;
            }
            z = (m[1][2] +m[2][1] ) * s;
            x = (m[0][1] +m[1][0] ) * s;
            w = (m[0][2] -m[2][0] ) * s;
        } else if ((m[1][1] <= m[0][0]  && m[2][2] > m[0][0] ) || (m[2][2]  > m[1][1] )) {
            type s = sqrt((m[2][2] - (m[0][0] + m[1][1] )) + 1);
            z = s * 0.5;
            if (s != 0.0) {
                s = 0.5 / s;
            }
            x = (m[2][0] + m[0][2] ) * s;
            y = (m[1][2] + m[2][1] ) * s;
            z = (m[1][0] - m[0][1] ) * s;
        } else {
            type s = sqrt((m[0][0] - (m[1][1] + m[2][2] )) + 1);
            x = s * 0.5;
            if (s != 0.0) {
                s = 0.5 / s;
            }
            y = (m[0][1] + m[1][0] ) * s;
            z = (m[2][0] + m[0][2] ) * s;
            w = (m[2][1] - m[1][2] ) * s;
        }
    }
}

// operations

template <typename type>
quat<type> quat<type>::operator*( const quat &a ) const
{
    return quat(a.w * x  + a.x * w  + a.y * z  - a.z * y,
                a.w * y  - a.x * z  + a.y * w  + a.z * x,
                a.w * z  + a.x * y  - a.y * x  + a.z * w,
                a.w * w  - a.x * x  - a.y * y  - a.z * z);
}

template <typename type>
quat<type>& quat<type>::operator*=( const quat &a )
{
    *this = *this * a;
    return *this;
}

template <typename type>
vec3<type> quat<type>::operator*(const vec3<type>& a) const
{
    // TODO implement faster, direct multiplication
    return toMat3() * a;
}

template <typename type>
quat<type> quat<type>::inverse() const
{
    return quat( -x, -y, -z, w );
}

template <typename type>
type quat<type>::length() const
{
    type len = x * x + y * y + z * z + w * w;
    return static_cast<type>(sqrt(len));
}

template <typename type>
quat<type> quat<type>::normalize() const
{
    type invLength = 1 / length();
    return quat(x * invLength, y * invLength, z * invLength, w * invLength);
}

template <typename type>
mat3<type> quat<type>::toMat3() const
{
    type xx = x * x,
         xy = x * y,
         xz = x * z,
         xw = x * w,
         yy = y * y,
         yz = y * z,
         yw = y * w,
         zz = z * z,
         zw = z * w;
         return mat3<type>
         (
             1 - 2 * (yy + zz)    , 2 * (xy - zw)        , 2 * (xz + yw)    ,
             2 * (xy + zw)        , 1 - 2 * (xx + zz)    , 2 * (yz - xw)    ,
             2 * (xz - yw)        , 2 * (yz + xw)     , 1 - 2 * (xx + yy)
         );
 }

template <typename type>
mat4<type> quat<type>::toMat4() const
{
    return mat4<type>(toMat3());
}

template <typename type>
vec3<type> quat<type>::getAxis() const
{
    return vec3<type>(x,y,z).normalize();
}

template <typename type>
type quat<type>::getAngle() const
{
    return 2 * acos(w / length());
}

template <typename type>
const quat<type> quat<type>::ONE(0, 0, 0, 1);

template <typename type>
quat<type> slerp(const quat<type>& from, const quat<type>& to, type t)
{
    if (t <= 0) {
        return quat<type>(from);
    } else if (t >= 1) {
        return quat<type>(to);
    } else {
        type cosom = from.x * to.x + from.y * to.y + from.z * to.z + from.w * to.w;
        type absCosom = fabs(cosom);

        type scale0;
        type scale1;

        if ((1 - absCosom) > 1e-6f) {
            type omega = safe_acos( absCosom );
            type sinom = 1 / sin( omega );
            scale0 = sin( ( 1 - t ) * omega ) * sinom;
            scale1 = sin( t * omega ) * sinom;
        } else {
            scale0 = 1 - t;
            scale1 = t;
        }
        quat<type> res = quat<type>(scale0 * from.x + scale1 * to.x,
                                    scale0 * from.y + scale1 * to.y,
                                    scale0 * from.z + scale1 * to.z,
                                    scale0 * from.w + scale1 * to.w);
        return res.normalize();
    }
}

}

#endif // _QUAT_H_
