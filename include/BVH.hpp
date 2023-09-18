#pragma once

#include <vector>
#include <memory>
#include <ctime>
#include "Object.hpp"
#include "Bounds3.hpp"
#include "Ray.hpp"
#include "Vec.hpp"


struct BVHNode
{

    Bounds3 bounds;
    BVHNode* left;
    BVHNode* right;
    Object* object;
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