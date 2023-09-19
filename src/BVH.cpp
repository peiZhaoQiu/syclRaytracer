#include <algorithm>
#include "BVH.hpp"
#include <queue>

BVHAccel::BVHAccel(std::vector<Object*> p, int maxPrimsInNode, SplitMethod splitMethod): maxPrimsInNode(maxPrimsInNode), splitMethod(splitMethod), primitives(std::move(p))
{
    if (primitives.empty()) return;
    root = build(primitives);
}


BVHAccel::~BVHAccel()
{
    if (root != nullptr)
    {
        reclusiveDelete(root);
    }
    
}


void BVHAccel::reclusiveDelete(BVHNode* node)
{
    if(node->left != nullptr)  reclusiveDelete(node->left);
    if(node->right != nullptr) reclusiveDelete(node->right);

    if (node != nullptr){
        if (node->object != nullptr){
            delete node->object;
        }
        delete node;
    }

    
}


BVHNode* BVHAccel::build(std::vector<Object*> objects)
{

    BVHNode* node = new BVHNode();
    //std::shared_ptr<BVHNode> node = std::make_shared<BVHNode>();
    if (objects.size() == 0){
        return node;
    } 
    else if (objects.size() == 1)
    {
        node->object = objects[0];
        node->bounds = objects[0]->getBounds();
        node->area = objects[0]->getArea(); 
        node->left = nullptr;
        node->right = nullptr;
        return node; 
    }
    else if (objects.size() == 2)
    {
        node-> left = build(std::vector<Object*>{objects[0]});
        node-> right = build(std::vector<Object*>{objects[1]});
        node->bounds = Union(node->left->bounds, node->right->bounds);
        node->area = node->left->area + node->right->area;
        node->object = nullptr;
        return node;
    }
    else
    {
        Bounds3 centroidBounds;
        for (auto object : objects)
        {
            centroidBounds = Union(centroidBounds, object->getBounds().Centroid());
        }
        int dim = centroidBounds.maxExtent();
        if(dim == 0)
        {
            std::sort(objects.begin(), objects.end(), [](Object* a, Object* b) {
                return a->getBounds().Centroid().x < b->getBounds().Centroid().x;
            });
        }
        else if(dim == 1)
        {
            std::sort(objects.begin(), objects.end(), [](Object* a, Object* b) {
                return a->getBounds().Centroid().y < b->getBounds().Centroid().y;
            });
        }
        else if(dim == 2)
        {
            std::sort(objects.begin(), objects.end(), [](Object* a, Object* b) {
                return a->getBounds().Centroid().z < b->getBounds().Centroid().z;
            });
        }
        int mid = objects.size() / 2;
        node->left = (build(std::vector<Object*>(objects.begin(), objects.begin() + mid)));
        node->right = (build(std::vector<Object*>(objects.begin() + mid, objects.end()))); 

        node->bounds = Union(node->left->bounds, node->right->bounds);
        node->area = node->left->area + node->right->area;

    }    

    return node;   
}

Intersection BVHAccel::Intersect(const Ray& ray) const
{
    Intersection inter;
    if (!root) return inter;
    inter = getIntersection(root, ray);
    return inter;
}

Intersection BVHAccel::getIntersection(const BVHNode* node, const Ray& ray) const
{
    Intersection inter;

    Vec3f indiv(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z);
    std::array<int, 3> dirIsNeg;
    dirIsNeg[0] = ray.direction.x >0; 
    dirIsNeg[1] = ray.direction.y >0; 
    dirIsNeg[2] = ray.direction.z >0; 

    if(!node->bounds.IntersectP(ray, indiv, dirIsNeg)){return inter;}

    std::queue<BVHNode*> q;
    q.push(const_cast<BVHNode*>(node));
    while(!q.empty())
    {
        BVHNode* cur = q.front();
        q.pop();
        if(cur->object)
        {
            Intersection tmp = cur->object->getIntersection(ray);
            if(inter._distance > tmp._distance)
            {
                inter = tmp;
            }
        }
        if (cur->left)
        {
            q.push(cur->left);
        }

        if (cur->right)
        {
            q.push(cur->right);
        }
        
    }
    return inter;
}


