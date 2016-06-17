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

#ifndef _ORK_BOX2_H_
#define _ORK_BOX2_H_

#include <algorithm>
#include <assert.h>

#include "pmath.h"
#include "ork/math/vec2.h"

using namespace std;

namespace ork
{

/**
 * A 2D bounding box.
 * @ingroup math
 */
template<typename type>
struct box2
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
     * Creates a new, empty bounding box.
     */
    box2() : xmin(INFINITY), xmax(-INFINITY),
             ymin(INFINITY), ymax(-INFINITY)
    {
    }

    /**
     * Creates a new bounding box with the given coordinates.
     */
    box2(type xmin, type xmax, type ymin, type ymax) :
        xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax)
    {
    }

    /**
     * Creates a new bounding box enclosing the two given points.
     */
    box2(const vec2<type> &p, const vec2<type> &q) :
        xmin(min(p.x, q.x)), xmax(max(p.x, q.x)), ymin(min(p.y, q.y)), ymax(max(p.y, q.y))
    {
    }

    /**
     * Returns the center of this bounding box.
     */
    vec2<type> center() const
    {
        return vec2<type>((xmin + xmax) / 2, (ymin + ymax) / 2);
    }

    /**
     * Returns the width of this bounding box.
     */
    type width() const
    {
        return xmax - xmin;
    }

    /**
     * Returns the height of this bounding box.
     */
    type height() const
    {
        return ymax - ymin;
    }

    /**
     * Returns the area of this bounding box.
     */
    type area() const
    {
        return (xmax - xmin) * (ymax - ymin);
    }

    /**
     * Returns the bounding box containing this box and the given border.
     *
     * @param w the border to be added to this box.
     */
    box2 enlarge(type w) const
    {
        return box2(xmin - w, xmax + w, ymin - w, ymax + w);
    }

    /**
     * Returns the bounding box containing this box and the given point.
     *
     * @param p an arbitrary point.
     */
    box2 enlarge(const vec2<type> &p) const
    {
        return box2(min(xmin, p.x), max(xmax, p.x), min(ymin, p.y), max(ymax, p.y));
    }

    /**
     * Returns the bounding box containing this box and the given box.
     *
     * @param r an arbitrary bounding box.
     */
    box2 enlarge(const box2 &r) const
    {
        return box2(min(xmin, r.xmin), max(xmax, r.xmax), min(ymin, r.ymin), max(ymax, r.ymax));
    }

    /**
     * Returns true if this bounding box contains the given point.
     *
     * @param p an arbitrary point.
     */
    bool contains(const vec2<type> &p) const
    {
        return p.x >= xmin && p.x <= xmax && p.y >= ymin && p.y <= ymax;
    }

    /**
     * Returns true if this bounding box contains the given bounding box.
     *
     * @param bb a box2
     */
    bool contains(const box2<type>& bb) const
    {
        return bb.xmin >= xmin && bb.xmax <= xmax && bb.ymin >= ymin && bb.ymax <= ymax;
    }

    /**
     * Alias for clipRectangle.
     */
    bool intersects(const box2 &a) const
    {
        return a.xmax >= xmin && a.xmin <= xmax && a.ymax >= ymin && a.ymin <= ymax;
    }

    /**
     * Returns the nearest point to a contained in the box.
     */
    vec2<type> nearestInnerPoint(vec2<type> a) const
    {
        vec2<type> nearest = a;
        if (a.x < xmin) {
            nearest.x = xmin;
        } else if (a.x > xmax) {
            nearest.x = xmax;
        }

        if (a.y < ymin) {
            nearest.y = ymin;
        } else if (a.y > ymax) {
            nearest.y = ymax;
        }
        return nearest;
    }

    /**
     * Returns the minimum distance between the box and a point.
     *
     * @param a the point
     */
    float distanceTo(vec2<type> a) const
    {
        return nearestInnerPoint(a).distanceTo(a);
    }

    /**
     * Returns the squared minimum distance between the box and a point.
     *
     * @param a the point
     */
    float squaredDistanceTo(vec2<type> a) const
    {
        return (nearestInnerPoint(a)).squaredDistanceTo(a);
    }

    /**
     * Returns true if this box is equal to the given box.
     */
    bool operator==(const box2& b) const
    {
        return xmin == b.xmin && xmax == b.xmax && ymin == b.ymin && ymax == b.max;
    }

    /**
     * Returns true if this box is different from the given box.
     */
    bool operator!=(const box2& b) const
    {
        return xmin != b.xmin || xmax != b.xmax || ymin != b.ymin || ymax != b.ymax;
    }
};

/**
 * A 2D bounding box with int coordinates.
 * @ingroup math
 */
typedef box2<int> box2i;

/**
 * A 2D bounding box with float coordinates.
 * @ingroup math
 */
typedef box2<float> box2f;

/**
 * A 2D bounding box with double coordinates.
 * @ingroup math
 */
typedef box2<double> box2d;

}

#endif
