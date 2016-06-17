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

#ifndef _ORK_BOX3_H_
#define _ORK_BOX3_H_

#include "pmath.h"
#include <algorithm>

#include "ork/math/vec3.h"

// Lars edit
using std::min;
using std::max;

namespace ork
{

/**
 * A 3D bounding box.
 * @ingroup math
 */
template <typename type> struct box3
{
    /**
     * Minimum x coordinate.
     */
    type xmin;

    /**
     * Maximum x coordinate.
     */
    type xmax;

    /**
     * Minimum y coordinate.
     */
    type ymin;

    /**
     * Maximum y coordinate.
     */
    type ymax;

    /**
     * Minimum z coordinate.
     */
    type zmin;

    /**
     * Maximum z coordinate.
     */
    type zmax;

    /**
     * Creates a new, empty bounding box.
     */
    box3(): xmin(INFINITY), xmax(-INFINITY), ymin(INFINITY),
            ymax(-INFINITY), zmin(INFINITY), zmax(-INFINITY)
    {
    }

    /**
     * Creates a new bounding box with the given coordinates.
     */
    box3(type xmin, type xmax, type ymin, type ymax, type zmin, type zmax):
        xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax), zmin(zmin), zmax(zmax)
    {
    }

    /**
     * Creates a new bounding box enclosing the two given points.
     */
    box3(const vec3<type> &p, const vec3<type> &q):
        xmin(min(p.x, q.x)), xmax(max(p.x, q.x)),
        ymin(min(p.y, q.y)), ymax(max(p.y, q.y)),
        zmin(min(p.z, q.z)), zmax(max(p.z, q.z))
    {
    }

    /**
     * Returns the center of this bounding box.
     */
    vec3<type> center() const
    {
        return vec3<type>((xmin + xmax) / 2, (ymin + ymax) / 2, (zmin + zmax) / 2);
    }

    /**
     * Returns the bounding box containing this box and the given point.
     *
     * @param p an arbitrary point.
     */
    box3<type> enlarge(const vec3<type> &p) const
    {
        return box3<type>(min(xmin, p.x), max(xmax, p.x), min(ymin, p.y), max(ymax, p.y), min(zmin, p.z), max(zmax, p.z));
    }

    /**
     * Returns the bounding box containing this box and the given box.
     *
     * @param r an arbitrary bounding box.
     */
    box3<type> enlarge(const box3<type> &r) const
    {
        return box3<type>(min(xmin, r.xmin), max(xmax, r.xmax), min(ymin, r.ymin), max(ymax, r.ymax), min(zmin, r.zmin), max(zmax, r.zmax));
    }

    /**
     * Returns true if this bounding box contains the given point.
     *
     * @param p an arbitrary point.
     */
    bool contains(const vec3<type> &p) const
    {
        return p.x >= xmin && p.x <= xmax && p.y >= ymin && p.y <= ymax && p.z >=zmin && p.z <= zmax;
    }

    /**
     * Casts this bounding box to another base type.
     */
    template <class t>
    box3<t> cast() {
        return box3<t>(xmin, xmax, ymin, ymax, zmin, zmax);
    }
};

/**
 * A 3D bounding box with int coordinates.
 * @ingroup math
 */
typedef box3<int> box3i;

/**
 * A 3D bounding box with float coordinates.
 * @ingroup math
 */
typedef box3<float> box3f;

/**
 * A 3D bounding box with double coordinates.
 * @ingroup math
 */
typedef box3<double> box3d;

}

#endif
