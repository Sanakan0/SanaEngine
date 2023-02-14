#include "spdlog/spdlog.h"
#include <SRender/Resources/SModelLoader.h>
#include <SRender/Resources/AssimpParser.h>
namespace SRender::Resources{
void SModelLoader::LoadSimpleModel(std::string path, SModel &model){
    AssimpParser parser;
    model.path_=path;
    parser.LoadModel(model.modelmat_,path,model.meshes_,model.materials_,texture_manager_);
    model.modelmat_=model.modelmat_;
    spdlog::info("[ASSIMP] Simple Model loaded : "+path);
}
void SModelLoader::LoadModelWithAnima(std::string path, SModel &model, std::vector<SAnimation> &animas){
    AssimpParser parser;
    model.path_=path;
    parser.LoadModel(model.modelmat_,path,model.meshes_,model.GetJoints(),animas);
    model.modelmat_=model.modelmat_;
    for (auto& i : animas) i.modelp_ = &model;
    spdlog::info("[ASSIMP] Animated Model loaded : "+path);
}
void SModelLoader::Initialize(ResourceManager::TextureManager* texture_manager){
    texture_manager_=texture_manager;
}

}