#include "SRender/Resources/SMesh.h"
#include <SRender/Resources/AssimpParser.h>
#include <assimp/mesh.h>
#include <iostream>
#include <stdint.h>
#include <utility>
#include <vector>
namespace SRender::Resources{

bool AssimpParser::LoadModel(std::string path, std::vector<SMesh *>& meshes){
    Assimp::Importer imp;
	const aiScene *scene = imp.ReadFile(path, aiProcess_Triangulate );
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << imp.GetErrorString() << std::endl;
		return false;
	}
	//std::string dir = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene, meshes);
    return true;
}

void AssimpParser::ProcessNode(const aiNode* node, const aiScene *scene,std::vector<SMesh*>& meshes){
    for (uint32_t i=0;i<node->mNumMeshes;++i){
        ProcessMesh(scene->mMeshes[node->mMeshes[i]],scene,meshes);
    }
    for (uint32_t i=0;i<node->mNumChildren;++i){
        ProcessNode(node->mChildren[i], scene,meshes);
    }
}

void AssimpParser::ProcessMesh(const aiMesh* mesh,const aiScene* scene,std::vector<SMesh*>& meshes ){
    std::vector<Vertex> vs;
    std::vector<uint32_t> idx;
    //load vertices
    for (uint32_t i=0;i<mesh->mNumVertices;i++){
        std::pair<float, float> tex_coord=(mesh->mTextureCoords[0])?std::make_pair(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y):std::make_pair(0.0f,0.0f);
        vs.push_back({
            {mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z},
            {tex_coord.first,tex_coord.second},
            {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z}
        });
    }
    for (uint32_t i=0;i<mesh->mNumFaces;i++){
        const auto& face = mesh->mFaces[i];
        for (uint32_t j=0;j<face.mNumIndices;j++)
            idx.push_back(face.mIndices[j]);
    }
    meshes.push_back(new SMesh(vs,idx));
}


}