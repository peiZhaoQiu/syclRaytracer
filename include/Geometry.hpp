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
    Intersection getIntersection(const Ray& ray) ;
    float getArea() ;
    void Sample(Intersection &pos,float &pdf);
    Bounds3 getBounds();
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

void Geometry::Sample(Intersection &pos,float &pdf)
{
    switch (_type)
    {
    case GeometryType::TRIANGLE:
        static_cast<Triangle*>(this)->Sample_virtual(pos,pdf);
        break;
    default:
        break;
    }
}

float Geometry::getArea()
{
    switch (_type)
    {
    case GeometryType::TRIANGLE:
        return static_cast<Triangle*>(this)->getArea_virtual();
    default:
        return 0;
    }
}

Bounds3 Geometry::getBounds() 
{
    switch (_type)
    {
    case GeometryType::TRIANGLE:
        return static_cast<Triangle*>(this)->getBounds_virtual();
    default:
        return Bounds3();
    }
}






