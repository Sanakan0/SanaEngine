#include "SResourceManager/ModelManager.h"
#include "SRender/Resources/SModel.h"
#include "SRender/Resources/SModelLoader.h"
#include "SRender/Resources/STexture.h"
#include "SRender/Resources/STextureLoader.h"
#include "SResourceManager/Util.h"
#include <spdlog/spdlog.h>
#include <stdint.h>
namespace ResourceManager {

SRender::Resources::SModel* ModelManager::CreateResources(const std::string& pth,bool is_cached){
    SRender::Resources::SModel* tmp = new SRender::Resources::SModel();
    SRender::Resources::SModelLoader::LoadSimpleModel(Util::GetFullPath(pth), *tmp,is_cached);
    repo_.Append(pth, tmp);
    return tmp;
}


void ModelManager::KillResource(const std::string &pth){
    if (auto tmp = repo_[pth];tmp!=nullptr){
        delete tmp;
        repo_.Remove(pth);
    }
}

void ModelManager::ClearAll(){
    repo_.ClearAllResources();
}

void ModelManager::UploadAll(){
    for (auto& i:repo_.resources_){
        i.second->UploadMeshBuffer();
    }
}

void ModelManager::ReloadResource(const std::string &pth){
    if (auto tmp = repo_[pth];tmp!=nullptr){
        //SRender::Resources::STextureLoader::RecompileShader(*tmpshaderp, pth);
    }
    
}

}