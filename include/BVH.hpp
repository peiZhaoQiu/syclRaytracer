#pragma once

#include <vector>
#include <memory>
#include <ctime>
#include <ostream>
#include "Object.hpp"
#include "Bounds3.hpp"
#include "Ray.hpp"
#include "Vec.hpp"


struct BVHNode
{

    Bounds3 bounds = Bounds3();
    BVHNode* left = nullptr;
    BVHNode* right = nullptr;
    Object* object = nullptr;
    int splitAxis=0, firstPrimOffset=0, nPrimitives=0;
    float area;

    BVHNode() {
        bounds = Bounds3();
        left = nullptr;
        right = nullptr;
        object = nullptr;
    }

    BVHNode(Bounds3 b, Object* o): bounds(b), object(o) {}
    BVHNode(Bounds3 b, BVHNode* l, BVHNode* r): bounds(b), left(l), right(r) {}

    std::string toString() const{
        std::string curString;
        curString = "(";
        if (object != nullptr){
            curString += " Object ";
            std::string pMinstring = std::to_string(bounds.pMin.x) + " , " + std::to_string(bounds.pMin.y) + " , " + std::to_string(bounds.pMin.z);
            std::string pMaxstring = std::to_string(bounds.pMax.x) +  ", " + std::to_string(bounds.pMax.y) + " , " + std::to_string(bounds.pMax.z);
            curString = curString + pMinstring + " , " + pMaxstring;
        }
        else{
            curString += " Node";
            if(left != nullptr){
                curString += " Left: " + left->toString();
            }
            if(right != nullptr){
                curString += " Right: " + right->toString();
            }
        }
        curString += ")";
        return curString;
    }
    friend std::ostream & operator << (std::ostream &os, const BVHNode &v)
    {
        return os << v.toString(); 
    }
};


class BVHAccel
{
    public:
        enum class SplitMethod { NAIVE, SAH };
        BVHAccel(std::vector<Object*> p, int maxPrimsInNode, SplitMethod splitMethod = SplitMethod::NAIVE);
        Bounds3 WorldBounds() const;
        ~BVHAccel();

        Intersection Intersect(const Ray& ray) const;
        Intersection getIntersection(const BVHNode* node, const Ray& ray) const;
        bool IntersectP(const Ray& ray) const;
        void reclusiveDelete(BVHNode* node);
        BVHNode* root;

        BVHNode* build(std::vector<Object*> objects);


        const int maxPrimsInNode;
        const SplitMethod splitMethod;
        std::vector<Object*> primitives;

        void getSample(BVHNode* node, float p, Intersection &pos, float &pdf);
        void sample(Intersection &pos, float &pdf);
};