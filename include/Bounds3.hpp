#pragma once


#include "Ray.hpp"
#include "Vec.hpp"
#include <limits>
#include <array>
#include <cmath>

class Bounds3
{
    public:
        Vec3f pMin,pMax;
        Bounds3()
        {
            double minNum = std::numeric_limits<double>::lowest();
            double maxNum = std::numeric_limits<double>::max();
            pMax = Vec3f(minNum,minNum,minNum);
            pMin = Vec3f(maxNum,maxNum,maxNum);
        }

        Bounds3(const Vec3f p): pMin(p), pMax(p) {}
        Bounds3(const Vec3f p1, const Vec3f p2)
        {
            pMin = Vec3f( fmin(p1.x, p2.x), fmin(p1.y, p2.y), fmin(p1.z, p2.z) );
            pMax = Vec3f( fmax(p1.x, p2.x), fmax(p1.y, p2.y), fmax(p1.z, p2.z) );
        }

        Vec3f Diagonal () const {return pMax - pMin;}
        int maxExtent () const 
        {

            Vec3f d = Diagonal();
            if (d.x > d.y && d.x > d.z){return 0;}
            else if (d.y >d.z){return 1;}
            else {return 2;}
        }

        double SurfaceArea() const
        {
            Vec3f d = Diagonal();
            return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
        }

        Vec3f Centroid() {return 0.5 * pMin + 0.5 * pMax;}
        Bounds3 Intersect(const Bounds3& b)
        {
            return Bounds3(
                Vec3f( fmax(pMin.x, b.pMin.x), fmax(pMin.y, b.pMin.y),fmax(pMin.z, b.pMin.z) ),
                Vec3f( fmin(pMax.x, b.pMax.x), fmin(pMax.y, b.pMax.y),fmin(pMax.z, b.pMax.z) )
            );
        }

        bool Overlaps(const Bounds3& b1, const Bounds3& b2)
        {
            bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
            bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
            bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
            return (x && y && z);
        }

        bool Inside(const Vec3f& p, const Bounds3& b)
        {
            return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y &&
                    p.y <= b.pMax.y && p.z >= b.pMin.z && p.z <= b.pMax.z);
        }


};


