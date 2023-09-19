#pragma once

#include "tiny_obj_loader.h"
#include <string>
#include <vector>
#include "Material.hpp"
#include "Vec.hpp"
#include <iostream>
#include "Triangle.hpp"


struct OBJ_result{
    std::vector<Triangle> Triangles;
    std::vector<MaterialInfo> MaterialsInfoList;
//    std::vector<int> geomIDs;
    std::vector<int> materialIDs;
};





namespace OBJ_Loader
{

    OBJ_result addObject(std::string objFilePath, std::string objFile)
    {

        OBJ_result result;

        std::string inputfile = objFilePath + objFile;
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = objFilePath; // Path to material files

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(inputfile, reader_config)) 
        {
            if (!reader.Error().empty()) 
            {
                std::cerr << "TinyObjReader: " << reader.Error();
            }
            return result;
        }

        if (!reader.Warning().empty()) 
        {
            std::cout << "TinyObjReader: " << reader.Warning();
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();
        std::cout << "Loading " << inputfile << std::endl;
        std::cout << shapes.size() << "  "<< materials.size() <<std::endl;
        for(size_t i = 0; i< materials.size();i++)
        {
            //std::cout << materials[0].diffuse << " " << std::endl;
            Vec3f diffuseVec(materials[i].diffuse[0],materials[i].diffuse[1],materials[i].diffuse[2]);
            Vec3f specularVec(materials[i].specular[0],materials[i].specular[1],materials[i].specular[2]);
            Vec3f emissionVec(materials[i].emission[0],materials[i].emission[1],materials[i].emission[2]);
            MaterialInfo mat = MaterialInfo(emissionVec,specularVec,diffuseVec);
            result.MaterialsInfoList.push_back(mat);

            std::cout << materials[i].emission[0] << " " << materials[i].emission[1] << " " << materials[i].emission[2] << std::endl;
            std::cout << materials[i].diffuse[0] << " " << materials[i].diffuse[1] << " " << materials[i].diffuse[2] << std::endl;
            std::cout << std::endl;
        }

        for (size_t i = 0; i<shapes.size(); i++) 
        {
            tinyobj::shape_t shape = shapes[i];
            std::cout << shape.name << std::endl;
            std::vector<Vec3f> vertices;
            for (size_t i = 0; i < shape.mesh.indices.size(); i += 3) 
            {
                
                for (int j = 0; j < 3; ++j) 
                {
                    unsigned int index = shape.mesh.indices[i + j].vertex_index;
                    float vx = attrib.vertices[3 * index];
                    float vy = attrib.vertices[3 * index + 1];
                    float vz = attrib.vertices[3 * index + 2];
                    vertices.push_back(Vec3f(vx, vy, vz));
                }
            }

            for (size_t i = 0; i < shape.mesh.indices.size(); i += 3)
            {
                unsigned int index1 = shape.mesh.indices[i].vertex_index;
                unsigned int index2 = shape.mesh.indices[i + 1].vertex_index;
                unsigned int index3 = shape.mesh.indices[i + 2].vertex_index;

                Triangle tri(vertices[index1], vertices[index2], vertices[index3]);
                result.Triangles.push_back(tri);
            }

            //Triangle tri(vertices[0],vertices[1],vertices[2]);
            //result.Triangles.push_back(tri);
        
            for (auto &id : shape.mesh.material_ids)
            {

                std::cout << id << "  ";
                result.materialIDs.push_back(id);
            }
        }
        std::cout << "Loaded " << inputfile << "have " << result.Triangles.size() << " Triangles."<<std::endl;
        return result;
    }

};