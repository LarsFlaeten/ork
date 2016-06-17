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

#ifndef _ORK_MAT3_H_
#define _ORK_MAT3_H_

#include <string.h>
#include "pmath.h"
#include "ork/math/vec3.h"
#include "ork/math/box3.h"

namespace ork
{

/**
 * A 3x3 matrix.
 * @ingroup math
 */
template <typename type> class mat3
{
protected:
    /**
     * The matrix coeffients
     */
    union {
        /**
         * The matrix coefficients as a 2D array.
         */
        type m[3][3];

        /**
         * The matrix coefficients as a 1D array.
         */
        type _m[9];
    };

public:
    /**
     * Creates a new, uninitialized matrix.
     */
    mat3();

    /**
     * Creates a new matrix with the given components. The first index is the
     * row index, the second one is the column index.
     */
    mat3(type m00, type m01, type m02,
         type m10, type m11, type m12,
         type m20, type m21, type m22);

    /**
     * Creates a new matrix with the given components in row major order.
     */
    mat3(const type *array);

    /**
     * Creates a new matrix with the given components. The first index is the
     * row index, the second one is the column index.
     */
    explicit mat3(const type array[3][3]);

    /**
     * Creates a new matrix as a copy of the given matrix.
     */
    mat3(const mat3& mat);

    /**
     * Returns the coefficients of this matrix.
     */
    const type* coefficients() const;

    /**
     * Returns the row of this matrix whose index is given.
     */
    const type* operator[](int iRow) const;

    /**
     * Returns the row of this matrix whose index is given (read-write).
     */
    type* operator[](int iRow);

    /**
     * Returns the column of this matrix whose index is given.
     */
    vec3<type> getColumn(int iCol) const;

    /**
     * Sets the column of this matrix whose index is given.
     */
    void setColumn(int iCol, const vec3<type>& v);

    /**
     * Sets the columns of this matrix from the 3 given column vectors.
     */
    void fromAxes(const vec3<type>& xAxis, const vec3<type>& yAxis, const vec3<type>& zAxis);

    /**
     * Assigns the given matrix to this matrix.
     */
    mat3& operator=(const mat3& mat);

    /**
     * Returns true is this matrix is equal to the given matrix.
     */
    bool operator==(const mat3& mat) const;

    /**
     * Returns true is this matrix is different from the given matrix.
     */
    bool operator!=(const mat3& mat) const;

    /**
     * Returns the sum of this matrix and of the given matrix.
     */
    mat3 operator+(const mat3& mat) const;

    /**
     * Returns the difference of this matrix and of the given matrix.
     */
    mat3 operator-(const mat3& mat) const;

    /**
     * Returns the product of this matrix and of the given matrix.
     */
    mat3 operator*(const mat3& mat) const;

    /**
     * Returns the product of this matrix and of the given vector.
     */
    vec3<type> operator*(const vec3<type> &v) const;

    /**
     * Returns the product of this matrix and of the given vector. The given
     * vector z coordinate is set to 1, and the vec3 result is converted
     * to a vec2 by dividing its xy components by its z component.
     */
    vec2<type> operator*(const vec2<type>& v) const;

    /**
     * Returns the bounding box of the given bounding box, transformed
     * by the linear transformation represented by this matrix.
     */
    box3<type> operator*(const box3<type> &b) const;

    /**
     * Returns the product of this matrix and of the given scalar.
     */
    mat3 operator*(const type scalar) const;

    /**
     * Returns the opposite of this matrix.
     */
    mat3 operator-() const;

    /**
     * Returns the transpose of this matrix.
     */
    mat3 transpose() const;

    /**
     * Returns true is this matrix has an inverse. In this case the inverse
     * matrix is returned in the given matrix parameter.
     */
    bool inverse(mat3& mInv, type tolerance = 1e-06) const;

    /**
     * Returns the inverse of this matrix.
     */
    mat3 inverse(type tolerance = 1e-06) const;

    /**
     * Returns the determinant of this matrix.
     */
    type determinant() const;

    /**
     * Returns the trace of this matrix.
     */
    type trace() const;

    /**
     * Casts this matrix to another base type.
     */
    template <class t>
    mat3<t> cast() const
    {
        return mat3<t>((t) m[0][0], (t) m[0][1], (t) m[0][2],
            (t) m[1][0], (t)m[1][1], (t) m[1][2],
            (t) m[2][0], (t)m[2][1], (t) m[2][2]
        );
    }

    /**
     * The null matrix.
     */
    static const mat3 ZERO;

    /**
     * The identity matrix.
     */
    static const mat3 IDENTITY;
};

/**
 * A 3x3 matrix with float components.
 * @ingroup math
 */
typedef mat3<float> mat3f;

/**
 * A 3x3 matrix with double components.
 * @ingroup math
 */
typedef mat3<double> mat3d;

template <typename type>
inline mat3<type>::mat3()
{
}

template <typename type>
inline mat3<type>::mat3(type m00, type m01, type m02,
    type m10, type m11, type m12,
    type m20, type m21, type m22)
{
    m[0][0] = m00;
    m[0][1] = m01;
    m[0][2] = m02;
    m[1][0] = m10;
    m[1][1] = m11;
    m[1][2] = m12;
    m[2][0] = m20;
    m[2][1] = m21;
    m[2][2] = m22;
}

template <typename type>
inline mat3<type>::mat3(const type *array)
{
    memcpy(m, array, 9*sizeof(type));
}

template <typename type>
inline mat3<type>::mat3(const type array[3][3])
{
    memcpy(m, array, 9*sizeof(type));
}

template <typename type>
inline mat3<type>::mat3(const mat3& mat)
{
    memcpy(m, mat.m, 9*sizeof(type));
}

template <typename type>
inline const type* mat3<type>::coefficients() const
{
    return _m;
}

template <typename type>
inline const type* mat3<type>::operator[](int iRow) const
{
    return m[iRow];
}

template <typename type>
inline type* mat3<type>::operator[](int iRow)
{
    return m[iRow];
}

template <typename type>
inline vec3<type> mat3<type>::getColumn(int iCol) const
{
    //assert(0 <= iCol && iCol < 3);
    return vec3<type>(m[0][iCol], m[1][iCol], m[2][iCol]);
}

template <typename type>
inline void mat3<type>::setColumn(int iCol, const vec3<type>& v)
{
    //assert(0 <= iCol && iCol < 3);
    m[0][iCol] = v.x;
    m[1][iCol] = v.y;
    m[2][iCol] = v.z;
}

template <typename type>
inline void mat3<type>::fromAxes(const vec3<type>& xAxis, const vec3<type>& yAxis, const vec3<type>& zAxis)
{
    setColumn(0, xAxis);
    setColumn(1, yAxis);
    setColumn(2, zAxis);
}

template <typename type>
inline mat3<type>& mat3<type>::operator=(const mat3<type>& mat)
{
    memcpy(m, mat.m, 9*sizeof(type));
    return *this;
}

template <typename type>
bool mat3<type>::operator==(const mat3<type>& mat) const
{
    for (int iRow = 0; iRow < 3; iRow++) {
        for (int iCol = 0; iCol < 3; iCol++) {
            if (m[iRow][iCol] != mat.m[iRow][iCol]) {
                return false;
            }
        }
    }
    return true;
}

template <typename type>
inline bool mat3<type>::operator!=(const mat3<type>& mat) const
{
    return !operator==(mat);
}

template <typename type>
mat3<type> mat3<type>::operator+(const mat3<type>& mat) const
{
    mat3<type> kSum;
    for (int iRow = 0; iRow < 3; iRow++) {
        for (int iCol = 0; iCol < 3; iCol++) {
            kSum.m[iRow][iCol] = m[iRow][iCol] + mat.m[iRow][iCol];
        }
    }
    return kSum;
}

template <typename type>
mat3<type> mat3<type>::operator-(const mat3<type>& mat) const
{
    mat3<type> kDiff;
    for (int iRow = 0; iRow < 3; iRow++) {
        for (int iCol = 0; iCol < 3; iCol++) {
            kDiff.m[iRow][iCol] = m[iRow][iCol] - mat.m[iRow][iCol];
        }
    }
    return kDiff;
}

template <typename type>
mat3<type> mat3<type>::operator*(const mat3<type>& mat) const
{
    mat3<type> kProd;
    for (int iRow = 0; iRow < 3; iRow++) {
        for (int iCol = 0; iCol < 3; iCol++) {
            kProd.m[iRow][iCol] =
                m[iRow][0] * mat.m[0][iCol] +
                m[iRow][1] * mat.m[1][iCol] +
                m[iRow][2] * mat.m[2][iCol];
        }
    }
    return kProd;
}

template <typename type>
vec3<type> mat3<type>::operator*(const vec3<type>& v) const
{
    vec3<type> kProd;
    for (int iRow = 0; iRow < 3; iRow++) {
        kProd[iRow] =
            m[iRow][0] * v[0] +
            m[iRow][1] * v[1] +
            m[iRow][2] * v[2];
    }
    return kProd;
}

template <typename type>
vec2<type> mat3<type>::operator*(const vec2<type>& v) const
{
    vec3<type> prod = operator*(vec3<type>(v.x, v.y, 1));
    return (prod.xy() / prod.z);
}

template <typename type>
box3<type> mat3<type>::operator*(const box3<type>& v) const
{
    box3<type> b;
    b = b.enlarge(operator*(vec3<type>(v.xmin, v.ymin, v.zmin)));
    b = b.enlarge(operator*(vec3<type>(v.xmax, v.ymin, v.zmin)));
    b = b.enlarge(operator*(vec3<type>(v.xmin, v.ymax, v.zmin)));
    b = b.enlarge(operator*(vec3<type>(v.xmax, v.ymax, v.zmin)));
    b = b.enlarge(operator*(vec3<type>(v.xmin, v.ymin, v.zmax)));
    b = b.enlarge(operator*(vec3<type>(v.xmax, v.ymin, v.zmax)));
    b = b.enlarge(operator*(vec3<type>(v.xmin, v.ymax, v.zmax)));
    b = b.enlarge(operator*(vec3<type>(v.xmax, v.ymax, v.zmax)));
    return b;
}

template <typename type>
mat3<type> mat3<type>::operator*(type fScalar) const
{
    mat3<type> kProd;
    for (int iRow = 0; iRow < 3; iRow++) {
        for (int iCol = 0; iCol < 3; iCol++) {
            kProd[iRow][iCol] = fScalar * m[iRow][iCol];
        }
    }
    return kProd;
}

template <typename type>
mat3<type> mat3<type>::operator-() const
{
    mat3<type> kNeg;
    for (int iRow = 0; iRow < 3; iRow++) {
        for (int iCol = 0; iCol < 3; iCol++) {
            kNeg[iRow][iCol] = -m[iRow][iCol];
        }
    }
    return kNeg;
}

template <typename type>
mat3<type> mat3<type>::transpose() const
{
    mat3<type> kTranspose;
    for (int iRow = 0; iRow < 3; iRow++) {
        for (int iCol = 0; iCol < 3; iCol++) {
            kTranspose[iRow][iCol] = m[iCol][iRow];
        }
    }
    return kTranspose;
}

template <typename type>
bool mat3<type>::inverse(mat3<type>& mInv, type tolerance) const
{
    // Invert a 3x3 using cofactors.  This is about 8 times faster than
    // the Numerical Recipes code which uses Gaussian elimination.
    mInv[0][0] = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    mInv[0][1] = m[0][2] * m[2][1] - m[0][1] * m[2][2];
    mInv[0][2] = m[0][1] * m[1][2] - m[0][2] * m[1][1];
    mInv[1][0] = m[1][2] * m[2][0] - m[1][0] * m[2][2];
    mInv[1][1] = m[0][0] * m[2][2] - m[0][2] * m[2][0];
    mInv[1][2] = m[0][2] * m[1][0] - m[0][0] * m[1][2];
    mInv[2][0] = m[1][0] * m[2][1] - m[1][1] * m[2][0];
    mInv[2][1] = m[0][1] * m[2][0] - m[0][0] * m[2][1];
    mInv[2][2] = m[0][0] * m[1][1] - m[0][1] * m[1][0];
    type fDet =
        m[0][0] * mInv[0][0] +
        m[0][1] * mInv[1][0] +
        m[0][2] * mInv[2][0];
    if (abs(fDet) <= tolerance) {
        return false;
    }
    type fInvDet = 1.0 / fDet;
    for (int iRow = 0; iRow < 3; iRow++) {
        for (int iCol = 0; iCol < 3; iCol++)
            mInv[iRow][iCol] *= fInvDet;
    }
    return true;
}

template <typename type>
mat3<type> mat3<type>::inverse(type tolerance) const
{
    mat3<type> kInverse;
    inverse(kInverse, tolerance);
    return kInverse;
}

template <typename type>
type mat3<type>::determinant() const
{
    type fCofactor00 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    type fCofactor10 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
    type fCofactor20 = m[1][0] * m[2][1] - m[1][1] * m[2][0];
    type fDet =
        m[0][0] * fCofactor00 +
        m[0][1] * fCofactor10 +
        m[0][2] * fCofactor20;
    return fDet;
}

template <typename type>
type mat3<type>::trace() const
{
   return m[0][0] + m[1][1] + m[2][2];
}

template <typename type>
const mat3<type> mat3<type>::ZERO(0, 0, 0,
                                  0, 0, 0,
                                  0, 0, 0);

template <typename type>
const mat3<type> mat3<type>::IDENTITY(1, 0, 0,
                                      0, 1, 0,
                                      0, 0, 1);

}

/**
 * Returns the product of this matrix and of the given scalar.
 */
template <typename scalarType, typename matType>
inline ork::mat3<matType> operator*(const scalarType scalar, const ork::mat3<matType> &m)
{
  return m * static_cast<matType>(scalar);
}

#endif
