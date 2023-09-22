#pragma once

#include "Vec.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Bounds3.hpp"



enum class GeometryType
{
    TRIANGLE 
};

class Geometry
{
public:
    Geometry(GeometryType type) :_type(type) {}
    virtual ~Geometry(){}
    //bool intersect(const Ray& ray);
    Intersection getIntersection(const Ray& ray);
    virtual float getArea()= 0;
    virtual void Sample(Intersection &pos,float &pdf)=0;
    virtual Bounds3 getBounds() = 0;
    GeometryType _type;
    //virtual bool hasEmit() = 0; 
};

#include "Triangle.hpp"



Intersection Geometry::getIntersection(const Ray& ray)
{
    switch (_type)
    {
    case GeometryType::TRIANGLE:
        return static_cast<Triangle*>(this)->getIntersection_virtual(ray);
    default:
        return Intersection();
    }
}




