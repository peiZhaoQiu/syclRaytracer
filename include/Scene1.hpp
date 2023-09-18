#pragma once
#include "Scene.hpp"
#include <string>
#include <unordered_map>
#include "OBJ_Loader.hpp"

Scene::Scene()
{
    _materialList = std::vector<Material*>();
    _objectsList = std::vector<Object*>();

}


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->_bvh = new BVHAccel(_objectsList, 1, BVHAccel::SplitMethod::NAIVE);
}

void Scene::addMeshObj(std::string objFilePath, std::string objFile)
{
    OBJ_result result = OBJ_Loader::addObject(objFilePath, objFile);
    std::vector<Material*> materialPtrList;

    for (size_t i = 0;i<result.MaterialsInfoList.size();i++)
    {

        //Material(Vec3f emission, Vec3f specular, Vec3f diffuse);
        //Material* materialPtr = new Material(result.Materials[i]._emission,result.Materials[i]._specular,result.Materials[i]._diffuse);

        //materialPtrList.emplace_back(materialPtr);
        MaterialInfo matInfo = result.MaterialsInfoList[i];
        Material* materialPtr = MaterialFactory::createMaterial(matInfo._emission,matInfo._specular,matInfo._diffuse);
        materialPtrList.emplace_back(materialPtr);
        _materialList.emplace_back(materialPtr); 
    }

    for(size_t i = 0; i< result.Triangles.size();i++)
    {

        Triangle *triPtr = new Triangle(result.Triangles[i]._v1, result.Triangles[i]._v2, result.Triangles[i]._v3);
        Object *new_obj = new Object(triPtr,materialPtrList[result.materialIDs[i]]);
        _objectsList.emplace_back(new_obj);
    }  

}