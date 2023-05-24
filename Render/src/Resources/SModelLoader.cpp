#include "spdlog/spdlog.h"
#include <SRender/Resources/SModelLoader.h>
#include <SRender/Resources/AssimpParser.h>
#include <memory>
namespace SRender::Resources{
bool SModelLoader::LoadSimpleModel(std::string path, SModel &model,bool is_cached,const ModelLoadSetting& loadsetting){
    AssimpParser parser;
    model.path_=path;
    std::vector<TextureStack> tmpmaterials;
    if(parser.LoadModel(model.modelmat_,path,model.meshes_,tmpmaterials,texture_manager_,is_cached,loadsetting)){
       
        model.material_=std::make_unique<SMaterial>(tmpmaterials);
      
        model.CalcBoundingSphere();
        spdlog::info("[ASSIMP] Simple Model loaded : "+path);
        return true;
    }
    spdlog::error("[ASSIMP] Simple Model load failed : "+path);
    return false;
}
bool SModelLoader::LoadModelWithAnima(std::string path, SModel &model, std::vector<SAnimation> &animas){
    AssimpParser parser;
    model.path_=path;
    if (parser.LoadModel(model.modelmat_,path,model.meshes_,model.GetJoints(),animas)){
        model.CalcBoundingSphere();
        for (auto& i : animas) i.modelp_ = &model;
        spdlog::info("[ASSIMP] Animated Model loaded : "+path);
        return true;
    }
    spdlog::error("[ASSIMP] Animated Model load failed : "+path);
    return false;
    
}
void SModelLoader::Initialize(ResourceManager::TextureManager* texture_manager){
    texture_manager_=texture_manager;
}



}