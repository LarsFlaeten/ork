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

#ifndef _ORK_MAT2_H_
#define _ORK_MAT2_H_

#include <string.h>
#include "pmath.h"
#include "ork/math/vec2.h"
#include "ork/math/box2.h"

namespace ork
{

/**
 * A 2x2 matrix.
 * @ingroup math
 */
template <typename type> class mat2
{
protected:
    /**
     * The matrix coeffients
     */
    union {
        /**
         * The matrix coefficients as a 2D array.
         */
        type m[2][2];

        /**
         * The matrix coefficients as a 1D array.
         */
        type _m[4];
    };

public:
    /**
     * Creates a new, uninitialized matrix.
     */
    mat2();

    /**
     * Creates a new matrix with the given components. The first index is the
     * row index, the second one is the column index.
     */
    mat2(type m00, type m01,
         type m10, type m11);

    /**
     * Creates a new matrix with the given components. The first index is the
     * row index, the second one is the column index.
     */
    explicit mat2(const type array[2][2]);

    /**
     * Creates a new matrix as a copy of the given matrix.
     */
    mat2(const mat2& mat);

    /**
     * Returns the coefficients of this matrix.
     */
    const type* coefficients() const;

    /**
     * Returns the row of this matrix whose index is given.
     */
    type* operator[](int iRow) const;

    /**
     * Returns the column of this matrix whose index is given.
     */
    vec2<type> getColumn(int iCol) const;

    /**
     * Sets the column of this matrix whose index is given.
     */
    void setColumn(int iCol, const vec2<type>& v);

    /**
     * Assigns the given matrix to this matrix.
     */
    mat2& operator=(const mat2& mat);

    /**
     * Returns true is this matrix is equal to the given matrix.
     */
    bool operator==(const mat2& mat) const;

    /**
     * Returns true is this matrix is different from the given matrix.
     */
    bool operator!=(const mat2& mat) const;

    /**
     * Returns the sum of this matrix and of the given matrix.
     */
    mat2 operator+(const mat2& mat) const;

    /**
     * Returns the difference of this matrix and of the given matrix.
     */
    mat2 operator-(const mat2& mat) const;

    /**
     * Returns the product of this matrix and of the given matrix.
     */
    mat2 operator*(const mat2& mat) const;

    /**
     * Returns the product of this matrix and of the given vector.
     */
    vec2<type> operator*(const vec2<type> &v) const;

    /**
     * Returns the product of this matrix and of the given scalar.
     */
    mat2 operator*(const type scalar) const;

    /**
     * Returns the opposite of this matrix.
     */
    mat2 operator-() const;

    /**
     * Returns the transpose of this matrix.
     */
    mat2 transpose() const;

    /**
     * Returns true is this matrix has an inverse. In this case the inverse
     * matrix is returned in the given matrix parameter.
     */
    bool inverse(mat2& mInv, type tolerance = 1e-06) const;

    /**
     * Returns the inverse of this matrix.
     */
    mat2 inverse(type tolerance = 1e-06) const;

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
    mat2<t> cast() const
    {
        return mat2<t>((t) m[0][0], (t) m[0][1],
                       (t) m[1][0], (t) m[1][1]);
    }

    /**
     * The null matrix.
     */
    static const mat2 ZERO;

    /**
     * The identity matrix.
     */
    static const mat2 IDENTITY;
};

/**
 * A 2x2 matrix with float components.
 * @ingroup math
 */
typedef mat2<float> mat2f;

/**
 * A 2x2 matrix with double components.
 * @ingroup math
 */
typedef mat2<double> mat2d;

template <typename type>
inline mat2<type>::mat2()
{
}

template <typename type>
inline mat2<type>::mat2(type m00, type m01,
                        type m10, type m11)
{
    m[0][0] = m00;
    m[0][1] = m01;
    m[1][0] = m10;
    m[1][1] = m11;
}

template <typename type>
inline mat2<type>::mat2(const type array[2][2])
{
    memcpy(m, array, 4*sizeof(type));
}

template <typename type>
inline mat2<type>::mat2(const mat2& mat)
{
    memcpy(m, mat.m, 4*sizeof(type));
}

template <typename type>
inline const type* mat2<type>::coefficients() const
{
    return _m;
}

template <typename type>
inline type* mat2<type>::operator[](int iRow) const
{
    return (type*)m[iRow];
}

template <typename type>
inline vec2<type> mat2<type>::getColumn(int iCol) const
{
    return vec2<type>(m[0][iCol], m[1][iCol]);
}

template <typename type>
inline void mat2<type>::setColumn(int iCol, const vec2<type>& v)
{
    m[0][iCol] = v.x;
    m[1][iCol] = v.y;
}

template <typename type>
inline mat2<type>& mat2<type>::operator=(const mat2<type>& mat)
{
    memcpy(m, mat.m, 4*sizeof(type));
    return *this;
}

template <typename type>
bool mat2<type>::operator==(const mat2<type>& mat) const
{
    for (int iRow = 0; iRow < 2; iRow++) {
        for (int iCol = 0; iCol < 2; iCol++) {
            if (m[iRow][iCol] != mat.m[iRow][iCol]) {
                return false;
            }
        }
    }
    return true;
}

template <typename type>
inline bool mat2<type>::operator!=(const mat2<type>& mat) const
{
    return !operator==(mat);
}

template <typename type>
mat2<type> mat2<type>::operator+(const mat2<type>& mat) const
{
    mat2<type> kSum;
    for (int iRow = 0; iRow < 2; iRow++) {
        for (int iCol = 0; iCol < 2; iCol++) {
            kSum.m[iRow][iCol] = m[iRow][iCol] + mat.m[iRow][iCol];
        }
    }
    return kSum;
}

template <typename type>
mat2<type> mat2<type>::operator-(const mat2<type>& mat) const
{
    mat2<type> kDiff;
    for (int iRow = 0; iRow < 2; iRow++) {
        for (int iCol = 0; iCol < 2; iCol++) {
            kDiff.m[iRow][iCol] = m[iRow][iCol] - mat.m[iRow][iCol];
        }
    }
    return kDiff;
}

template <typename type>
mat2<type> mat2<type>::operator*(const mat2<type>& mat) const
{
    mat2<type> kProd;
    for (int iRow = 0; iRow < 2; iRow++) {
        for (int iCol = 0; iCol < 2; iCol++) {
            kProd.m[iRow][iCol] = m[iRow][0] * mat.m[0][iCol] + m[iRow][1] * mat.m[1][iCol];
        }
    }
    return kProd;
}

template <typename type>
vec2<type> mat2<type>::operator*(const vec2<type>& v) const
{
    vec2<type> kProd;
    for (int iRow = 0; iRow < 2; iRow++) {
        kProd[iRow] =
            m[iRow][0] * v[0] +
            m[iRow][1] * v[1];
    }
    return kProd;
}

template <typename type>
mat2<type> mat2<type>::operator*(type fScalar) const
{
    mat2<type> kProd;
    for (int iRow = 0; iRow < 2; iRow++) {
        for (int iCol = 0; iCol < 2; iCol++) {
            kProd[iRow][iCol] = fScalar * m[iRow][iCol];
        }
    }
    return kProd;
}

template <typename type>
mat2<type> mat2<type>::operator-() const
{
    mat2<type> kNeg;
    for (int iRow = 0; iRow < 2; iRow++) {
        for (int iCol = 0; iCol < 2; iCol++) {
            kNeg[iRow][iCol] = -m[iRow][iCol];
        }
    }
    return kNeg;
}

template <typename type>
mat2<type> mat2<type>::transpose() const
{
    mat2<type> kTranspose;
    for (int iRow = 0; iRow < 2; iRow++) {
        for (int iCol = 0; iCol < 2; iCol++) {
            kTranspose[iRow][iCol] = m[iCol][iRow];
        }
    }
    return kTranspose;
}

template <typename type>
bool mat2<type>::inverse(mat2<type>& mInv, type tolerance) const
{
    type det = determinant();

    if (abs(det) <= tolerance) {
        return false;
    }

    type invDet = 1 / det;

    mInv[0][0] = m[1][1] * invDet;
    mInv[0][1] = -m[0][1] * invDet;
    mInv[1][0] = -m[1][0] * invDet;
    mInv[1][1] = m[0][0] * invDet;
    return true;
}

template <typename type>
mat2<type> mat2<type>::inverse(type tolerance) const
{
    mat2<type> kInverse;
    inverse(kInverse, tolerance);
    return kInverse;
}

template <typename type>
type mat2<type>::determinant() const
{
    return m[0][0] * m[1][1] - m[1][0] * m[0][1];
}

template <typename type>
type mat2<type>::trace() const
{
   return m[0][0] + m[1][1];
}

template <typename type>
const mat2<type> mat2<type>::ZERO(0, 0,
                                  0, 0);

template <typename type>
const mat2<type> mat2<type>::IDENTITY(1, 0,
                                      0, 1);

}

#endif
