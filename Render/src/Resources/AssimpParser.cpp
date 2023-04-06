#include "SRender/Resources/SBaseRenderResources.h"
#include "SRender/Resources/STexture.h"
#include "assimp/material.h"
#include "assimp/scene.h"
#include "assimp/texture.h"
#include "assimp/types.h"
#include "assimp/vector3.h"
#include "assimp/DefaultIOSystem.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include "spdlog/spdlog.h"
#include <SRender/Resources/AssimpParser.h>
#include <assimp/IOSystem.hpp>
#include <assimp/anim.h>
#include <assimp/matrix4x4.h>
#include <assimp/mesh.h>
#include <cstddef>
#include <iostream>
#include <stdint.h>
#include <unordered_map>
#include <utility>
#include <vcruntime.h>
#include <vector>
namespace SRender::Resources{

AssimpParser::AssimpParser(){}
AssimpParser::~AssimpParser(){}

//simple loader
bool AssimpParser::LoadModel(glm::mat4& model_mat,std::string path, std::vector<SMesh*>& meshes,std::vector<TextureStack>& materials,ResourceManager::TextureManager* tex_manager,bool is_cached,uint32_t assimp_flag){
    loadwithskeleton=0;
    is_cached_=is_cached;
    Assimp::Importer imp;
    
    //imp.SetIOHandler(new Assimp::DefaultIOSystem());

	const aiScene *scene = imp.ReadFile(path, assimp_flag );
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
        spdlog::error("[ASSIMP ERROR]"+std::string(imp.GetErrorString()));
		return false;
	}
    

    aiMatrix4x4 identity;
    ProcessNode(identity, scene->mRootNode, scene);
    model_mat=glm::transpose( glm::make_mat4x4(&ai_model_mat.a1));
    meshes.reserve(vertex_data_.size());
    for (uint32_t i =0;i<vertex_data_.size();i++){
        //meshes.push_back(SMesh(vertex_data_[i],idx_data_[i]));
        meshes.push_back(new SMesh(vertex_data_[i],idx_data_[i],material_idx_[i],is_cached));
    }

    ProcessMaterial(scene,materials,tex_manager,path);

    Clear();
    return true;
}
//load with skeleton
bool AssimpParser::LoadModel(glm::mat4& model_mat,std::string path, std::vector<SMesh*>& meshes,std::vector<SJoint>& joints,std::vector<SAnimation>& sanimas,bool is_cached,uint32_t assimp_flag){
    loadwithskeleton=1;
    is_cached_=is_cached;
    Assimp::Importer imp;
	const aiScene *scene = imp.ReadFile(path, aiProcess_Triangulate);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{   
        spdlog::error("[ASSIMP ERROR]"+std::string(imp.GetErrorString()));
		return false;
	}

    //ProcessMaterial(scene,nullptr);

	//std::string dir = path.substr(0, path.find_last_of('/'));
    //print bonename
    // for (uint32_t i=0;i<scene->mMeshes[0]->mNumBones;i++) std::cout <<scene->mMeshes[0]->mBones[i]->mName.C_Str()<<" ";
    // std::cout <<"\n";

    BuildName2NodeMap(scene->mRootNode);
    aiMatrix4x4 identity;
    ProcessNode(identity,scene->mRootNode, scene);
    model_mat=glm::transpose( glm::make_mat4x4(&ai_model_mat.a1));
    ProcessSkeleton(scene->mRootNode, scene, joints);
    ProcessAnimation(scene, sanimas, joints);
    
    // add joint data to mesh
    for (int i=0;i<vertex_data_w_.size();++i){
        std::vector<std::vector<std::pair<uint32_t,float>>> tmp_weight(vertex_data_w_[i].size());
        aiMesh* aim=scene->mMeshes[aimeshofmesh_[i]];
        for (uint32_t j=0;j<aim->mNumBones;++j){
            aiBone* bone = aim->mBones[j];
            joints[name2jointidx_[std::string(bone->mName.C_Str())]].inverse_bind_mat=glm::transpose(glm::make_mat4x4(&bone->mOffsetMatrix.a1));
            uint32_t joint_idx = name2jointidx_[std::string(bone->mName.C_Str())];
            for (uint32_t k=0;k<bone->mNumWeights;++k){
                if(bone->mWeights[k].mWeight==0) continue;
                tmp_weight[bone->mWeights[k].mVertexId].push_back({joint_idx,bone->mWeights[k].mWeight});
                // vertex_data_w_[i][bone->mWeights[k].mVertexId].joint_ids[jointcnt]=joint_idx;
                // vertex_data_w_[i][bone->mWeights[k].mVertexId].weights[jointcnt]=bone->mWeights[k].mWeight;
                
                //meshes[i]->weights_[bone->mWeights[k].mVertexId].push_back({joint_idx,bone->mWeights[k].mWeight});
            }
        }

        for (int j=0;j<tmp_weight.size();++j){
            for (int k=0;k<tmp_weight[j].size()&&k<4;++k){
                vertex_data_w_[i][j].joint_ids[k]=tmp_weight[j][k].first;
                vertex_data_w_[i][j].weights[k]=tmp_weight[j][k].second;
            }
        }

    }
    meshes.reserve(vertex_data_w_.size());
    for (int i =0;i<vertex_data_w_.size();i++){
        meshes.push_back(new SMesh(vertex_data_w_[i],idx_data_[i],material_idx_[i],is_cached));
        //meshes.push_back(SMesh(vertex_data_w_[i],idx_data_[i]));
    }
    Clear();
    return true;
}

void AssimpParser::Clear(){
    aimeshofmesh_.clear();
    name2jointidx_.clear();
    name2ainode_.clear();
    vertex_data_.clear();
    vertex_data_w_.clear();
    idx_data_.clear();
    material_idx_.clear();
    diffuse_tex_.clear();
}

STexture* AssimpParser::LoadTexture(const std::filesystem::path& full_pth,const aiScene* scene,ResourceManager::TextureManager* tex_manager){

    if(auto tex = scene->GetEmbeddedTexture(full_pth.filename().generic_string().c_str())){
        //TODO load embeded
        return nullptr;
    }else{
        return tex_manager->CreateResources(full_pth.generic_string(),is_cached_);
    }
}

void AssimpParser::ProcessMaterial(const aiScene* scene,std::vector<TextureStack>& materials,ResourceManager::TextureManager* tex_manager,std::string pth){
    if (tex_manager==nullptr) {
        spdlog::warn("[ASSIMP WARNING] Texture Manager Missing, No Texture Loaded. Set SRender::Resources::SModelLoader::texture_manager_ plz");
        return;
    };
    namespace fs = std::filesystem;
    fs::path model_path(pth);
    fs::path pa_path = model_path.parent_path();
    materials.clear();
    materials.resize(scene->mNumMaterials);
    for (uint32_t i=0;i<scene->mNumMaterials;++i){
        aiMaterial* material = scene->mMaterials[i];
        aiString name;
        material->Get(AI_MATKEY_NAME,name);
        int shadingm;
        material->Get(AI_MATKEY_SHADING_MODEL,shadingm);
        
        // load texture (only diffuse tex supported)
        aiString aidiff_tex_file;
        material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE,0),aidiff_tex_file);
        std::string diff_tex_file(aidiff_tex_file.C_Str());
        materials[i].DiffuseTex=(diff_tex_file!="")?
            LoadTexture(pa_path/diff_tex_file, scene, tex_manager):nullptr;

        aiString aispec_tex_file;
        material->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR,0),aispec_tex_file);
        std::string spec_tex_file(aispec_tex_file.C_Str());
        materials[i].SpecularTex=(spec_tex_file!="")?
            LoadTexture(pa_path/spec_tex_file, scene, tex_manager):nullptr;

        aiString ainorm_tex_file;
        material->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR,0),ainorm_tex_file);
        std::string norm_tex_file(ainorm_tex_file.C_Str());
        materials[i].NormalsTex=(norm_tex_file!="")?
            LoadTexture(pa_path/norm_tex_file, scene, tex_manager):nullptr;
        


        // for (uint32_t j=0;j<material->mNumProperties;++j){
        //     auto prop=material->mProperties[j];
        // }
        // int x=material->GetTextureCount(aiTextureType_DIFFUSE);


    }
    // for (int i=0;i<material_idx_.size();++i){
    //     int mat_idx=material_idx_[i];
    //     materials.push_back(tex_stacks_[mat_idx]);
    // }
}

//static int depth=0; print node tree
void AssimpParser::ProcessNode(const aiMatrix4x4& transform_mat,const aiNode* node, const aiScene *scene){
    //std::cout<<std::string(depth*2,'-')<<node->mName.C_Str()<<std::endl;
    //depth++;

    aiMatrix4x4 node_transmat = transform_mat*node->mTransformation;
    if (node->mNumMeshes>0) ai_model_mat = node_transmat;
    for (uint32_t i=0;i<node->mNumMeshes;++i){
        ProcessMesh(node_transmat,scene->mMeshes[node->mMeshes[i]],scene);
        aimeshofmesh_.push_back(node->mMeshes[i]);
    }
    for (uint32_t i=0;i<node->mNumChildren;++i){
        ProcessNode(node_transmat,node->mChildren[i], scene);
    }
    //depth--;
}

// void AssimpParser::ProcessBone(const aiMesh* mesh,const aiScene* scene,std::vector<SBone>& bones ){
//     for (uint32_t i=0;i<mesh->mNumBones;++i){
//         const aiBone* bone = mesh->mBones[i];
//         str2aibonep[std::string(bone->mName.C_Str())]=bone;
//     }
// }
void AssimpParser::ProcessSkeleton(const aiNode* node, const aiScene *scene,std::vector<SJoint>& joints){
    if (name2jointidx_.find(std::string(node->mName.C_Str()))!=name2jointidx_.end()){
        joints.push_back({
            glm::vec3(1),
            glm::quat(),
            glm::vec3(1),
            glm::mat4(1),
            -1
        });
        name2jointidx_[std::string(node->mName.C_Str())] =static_cast<uint32_t>( joints.size()-1);
        if (joints.size()>1){
            aiNode* pa = node->mParent;
            joints[joints.size()-1].parent = name2jointidx_[std::string(pa->mName.C_Str())];
        }
    }
    for (uint32_t i=0;i<node->mNumChildren;++i){
        ProcessSkeleton(node->mChildren[i], scene,joints);
    }
}

void AssimpParser::MarkSkeleton(const aiNode* node){
    while(node!=nullptr&&
    name2jointidx_.find(std::string(node->mName.C_Str()))==name2jointidx_.end()){
        bool ischildmeshes=0;
        for (uint32_t i=0;i<node->mNumChildren;i++){
            if (node->mChildren[i]->mNumMeshes>0){
                ischildmeshes=1;
                break;
            }
        }
        bool isselfmeshes=node->mNumMeshes>0;
        if (ischildmeshes||isselfmeshes){
            return;
        }
        name2jointidx_[std::string(node->mName.C_Str())]=0;
        node=node->mParent;
    }
}

void AssimpParser::ProcessMesh(const aiMatrix4x4& transform_mat,const aiMesh* mesh,const aiScene* scene){
    //load vertex data
    if (loadwithskeleton){
        vertex_data_w_.push_back(std::vector<VertexWithWeight>());
        std::vector<VertexWithWeight>& vs = vertex_data_w_[vertex_data_w_.size()-1];
        vs.reserve(mesh->mNumVertices);
        //load vertices
        for (uint32_t i=0;i<mesh->mNumVertices;++i){
            aiVector3D tex_coord=(mesh->mTextureCoords[0]!=nullptr)?mesh->mTextureCoords[0][i]:aiVector3D(0,0,0);
            aiVector3D normals=(mesh->mNormals!=nullptr)?mesh->mNormals[i]:aiVector3D(0,0,0);
            vs.push_back({
                {mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z},
                {tex_coord.x,tex_coord.y},
                {normals.x, normals.y, normals.z}
            });
        }
    }else{
        vertex_data_.push_back(std::vector<Vertex>());
        std::vector<Vertex>& vs = vertex_data_[vertex_data_.size()-1];
        vs.reserve(mesh->mNumVertices);
        //load vertices
        for (uint32_t i=0;i<mesh->mNumVertices;++i){
            aiVector3D tex_coord=(mesh->mTextureCoords[0]!=nullptr)?mesh->mTextureCoords[0][i]:aiVector3D(0,0,0);
            aiVector3D normals=(mesh->mNormals!=nullptr)?mesh->mNormals[i]:aiVector3D(0,0,0);
            vs.push_back({
                {mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z},
                {tex_coord.x,tex_coord.y},
                {normals.x, normals.y, normals.z}
            });
        }
    }
    //load index data
    idx_data_.push_back(std::vector<uint32_t>());
    std::vector<uint32_t>& idx = idx_data_[idx_data_.size()-1] ;
    idx.reserve(mesh->mNumFaces);
    for (uint32_t i=0;i<mesh->mNumFaces;++i){
        const auto& face = mesh->mFaces[i];
        for (uint32_t j=0;j<face.mNumIndices;++j)
            idx.push_back(face.mIndices[j]);
    }
    //mark bones
    for (uint32_t i=0;i<mesh->mNumBones;++i){
        const aiBone* bone = mesh->mBones[i];
        MarkSkeleton(name2ainode_[std::string(bone->mName.C_Str())]);
    }
    //keep material id
    material_idx_.push_back(mesh->mMaterialIndex);
}
void AssimpParser::BuildName2NodeMap(const aiNode* node){
    name2ainode_[std::string(node->mName.C_Str())]=node;
    for (uint32_t i =0;i<node->mNumChildren;++i)
        BuildName2NodeMap(node->mChildren[i]);
}
void AssimpParser::ProcessAnimation(const aiScene *scene,std::vector<SAnimation>& sanimas,std::vector<SJoint>& joints){
    sanimas.reserve(scene->mNumAnimations);
    for (uint32_t i=0;i<scene->mNumAnimations;++i){
        aiAnimation* anima = scene->mAnimations[i];
        
        //aiVectorKey
        
        std::vector<joint_anima_t> joint_animation;
        joint_animation.resize(joints.size());
        for (uint32_t j=0;j<anima->mNumChannels;j++){
            aiNodeAnim* aninode = anima->mChannels[j];
            uint32_t joint_idx = name2jointidx_[std::string(aninode->mNodeName.C_Str())];
            auto& tmpjointani = joint_animation[joint_idx];
            size_t sz;
            for (uint32_t k=0;k<aninode->mNumPositionKeys;++k){
                auto& tmp = aninode->mPositionKeys[k];
                sz=tmpjointani.trans_seq.size();
                tmpjointani.trans_seq.push_back(
                    {tmp.mTime/anima->mTicksPerSecond,
                    {tmp.mValue.x,tmp.mValue.y,tmp.mValue.z}});
            }
            for (uint32_t k=0;k<aninode->mNumScalingKeys;++k){
                auto& tmp = aninode->mScalingKeys[k];
                sz=tmpjointani.scale_seq.size();
                tmpjointani.scale_seq.push_back(
                    {tmp.mTime/anima->mTicksPerSecond,
                    {tmp.mValue.x,tmp.mValue.y,tmp.mValue.z}});
            }
            for (uint32_t k=0;k<aninode->mNumRotationKeys;++k){
                auto& tmp = aninode->mRotationKeys[k];
                sz=tmpjointani.orien_seq.size();
                tmpjointani.orien_seq.push_back(
                    {tmp.mTime/anima->mTicksPerSecond,
                    {tmp.mValue.w,tmp.mValue.x,tmp.mValue.y,tmp.mValue.z}});
            }
        }
        sanimas.emplace_back(anima->mName.C_Str(),anima->mDuration/anima->mTicksPerSecond,joint_animation);
    }
}

}