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
    if (objects.size() == 1)
    {
        node->object = std::make_shared<Object>(*objects[0]);
        node->bounds = objects[0]->getBounds();
        node->area = objects[0]->getArea(); 
        node->left = nullptr;
        node->right = nullptr;
        return node; 
    }
    else if (objects.size() == 2)
    {
        node-> left = std::make_shared<BVHNode>(build(std::vector<Object*>{objects[0]}));
        node-> right = std::make_shared<BVHNode>(build(std::vector<Object*>{objects[1]}));
        node->bounds = Union(node->left->bounds, node->right->bounds);
        node->area = node->left->area + node->right->area;
        node->object = nullptr;
        return node;
    }
    
        

}


