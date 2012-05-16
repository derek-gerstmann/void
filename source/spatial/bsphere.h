// ============================================================================================== //
//
// License:     The Lesser GNU Public License (LGPL) v3.0.
//
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA).
//              As well as the shoulders of many giants...
//
// This file is part of the Void framework.
//
// The Void framework is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The Void framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Void framework.  If not, see <http://www.gnu.org/licenses/>.
//
// ============================================================================================== //

#ifndef VD_SPATIAL_VPTREE_INCLUDED
#define VD_SPATIAL_VPTREE_INCLUDED

// ============================================================================================== //

#include "spatial/spatial.h"

#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <queue>
#include <limits>

// ============================================================================================== //

VD_SPATIAL_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Bounding
{

// ============================================================================================== //

template<class PT, typename Real>
struct Sphere
{
    Point Center;
    Real Radius;

    inline Sphere()
    {
        Radius = 0.0f;
    }

    inline Sphere(Stream* stream)
    {
        Center = Point(stream);
        Radius = stream->ReadReal();
    }

    /// Create a bounding sphere from a given center point and Radius
    inline Sphere(const Point& center, Real Radius)
        : Center(center), Radius(Radius)
    {
    }

    inline Sphere(const BSphere& boundingSphere)
        : Center(boundingSphere.center), Radius(boundingSphere.Radius)
    {
    }

    /// Return whether this bounding sphere has a Radius of zero or less.
    inline bool IsEmpty() const
    {
        return Radius <= 0.0f;
    }

    /// Expand the bounding sphere Radius to contain another point.
    inline void ExpandBy(const Point p)
    {
        Radius = std::max(Radius, (p - center).length());
    }

    /// Check whether the specified point is inside or on the sphere
    inline bool IsWithinBounds(const Point p) const
    {
        return (p - center).length() <= Radius;
    }

    /// Equality test
    inline bool operator==(const BSphere& boundingSphere) const
    {
        return center == boundingSphere.center && Radius == boundingSphere.Radius;
    }

    /// Inequality test
    inline bool operator!=(const BSphere& boundingSphere) const
    {
        return center != boundingSphere.center || Radius != boundingSphere.Radius;
    }

    inline bool Intersect(const Ray& ray, Real& nearHit, Real& farHit) const
    {
        Vector originToCenter = center - ray.o;
        Real distToRayClosest = dot(originToCenter, ray.d);
        Real tmp1 = originToCenter.lengthSquared() - Radius * Radius;

        if(tmp1 <= 0.0f)
        {
            nearHit = farHit = Math::Sqrt(distToRayClosest * distToRayClosest - tmp1) + distToRayClosest;
            return true;
        }

        /* Points in different direction */
        if(distToRayClosest < 0.0f)
            return false;

        Real sqrOriginToCenterLength = originToCenter.lengthSquared();
        Real sqrHalfChordDist = Radius * Radius - sqrOriginToCenterLength
                                 + distToRayClosest * distToRayClosest;

        if(sqrHalfChordDist < 0)  // Miss
            return false;

        // Hit
        Real hitDistance = std::sqrt(sqrHalfChordDist);
        nearHit = distToRayClosest - hitDistance;
        farHit = distToRayClosest + hitDistance;

        if(nearHit == 0)
            nearHit = farHit;

        return true;
    }

    /// Serialize this bounding sphere to a binary data stream
    inline void Serialize(Stream* stream) const
    {
        Center.serialize(stream);
        stream->WriteReal(Radius);
    }

    /// Return a string representation of the bounding sphere
    inline std::string ToString() const
    {
        std::ostringstream oss;
        oss << "Spatial::Bounding::Sphere";
        oss << "[ ";
        oss << "Center = " << Center.ToString(); oss << ", "
        oss << "Radius = " << Radius;
        oss << " ]";
        return oss.str();
    }
};

// ============================================================================================== //

} // END NAMESPACE: Bounds

// ============================================================================================== //

VD_SPATIAL_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_SPATIAL_BSPHERE_INCLUDED
