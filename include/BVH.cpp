#include <algorithm>
#include "BVH.hpp"

BVHAccel::BVHAccel(std::vector<Object*> p, int maxPrimsInNode, SplitMethod splitMethod): maxPrimsInNode(maxPrimsInNode), splitMethod(splitMethod), primitives(std::move(p))
{
    if (primitives.empty()) return;
    root = build(primitives);
}


BVHNode* BVHAccel::build(std::vector<Object*> objects)
{
    BVHNode* node = new BVHNode();
    Bounds3 bounds;
    for (auto& object : objects)
    {
        //bounds = bounds.Intersect(object->WorldBound());
        bounds = Union(bounds, object->getBound());
    }
    if (objects.size() == 1)

}


