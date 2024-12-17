#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "hittable_list.h"
#include "triangle.h"

bool DoTheImportThing( const std::string& pFile, std::shared_ptr<hittable_list>& output_scene) {
    // Create an instance of the Importer class
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    const aiScene* pScene = importer.ReadFile( pFile,
    aiProcess_CalcTangentSpace       |
    aiProcess_Triangulate            |
    aiProcess_JoinIdenticalVertices  |
    aiProcess_SortByPType);

    // If the import failed, report it
    if (nullptr == pScene) {
        std::cout <<  importer.GetErrorString() << "\n";
        return false;
    }

    // Now we can access the file's contents.
    const auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
    output_scene.reset(new hittable_list{});
    for (unsigned int i = 0 ; i < pScene->mNumMeshes ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) 
        {
            const aiFace& Face = paiMesh->mFaces[i];
            //assert(Face.mNumIndices == 3);
            //Indices.push_back(Face.mIndices[0]);
            //Indices.push_back(Face.mIndices[1]);
            //Indices.push_back(Face.mIndices[2]);
            const aiVector3D* pPos1 = &(paiMesh->mVertices[Face.mIndices[0]]);
            const aiVector3D* pPos2 = &(paiMesh->mVertices[Face.mIndices[1]]);
            const aiVector3D* pPos3 = &(paiMesh->mVertices[Face.mIndices[2]]);

            const point3 p1{pPos1->x, pPos1->y, pPos1->z};
            const point3 p2{pPos2->x, pPos2->y, pPos2->z};
            const point3 p3{pPos3->x, pPos3->y, pPos3->z};

            output_scene->add(std::make_shared<triangle>(p1, p2, p3, ground_material));
        }
    }

  // We're done. Everything will be cleaned up by the importer destructor
  return true;
}