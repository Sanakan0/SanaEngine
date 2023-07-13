#include "SResourceManager/ModelManager.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Resources/SModel.h"
#include "SRender/Resources/SModelLoader.h"
#include "SRender/Resources/STexture.h"
#include "SRender/Resources/STextureLoader.h"
#include "SResourceManager/Util.h"
#include "SceneSys/SceneManager.h"
#include <cstddef>
#include <memory>
#include <spdlog/spdlog.h>
#include <stdint.h>
namespace ResourceManager {

SRender::Resources::SModel* ModelManager::CreateResources(const std::string& pth,bool is_cached,const SRender::Resources::ModelLoadSetting& setting){
    if (auto m = repo_[pth];m!=nullptr) {
        spdlog::info("[ModelManager] Resource already created : "+pth);
        return m;
    }
    
    auto tmp=std::make_unique<SRender::Resources::SModel>();
    if (SRender::Resources::SModelLoader::LoadSimpleModel(Util::GetFullPath(pth), *tmp,is_cached,setting)){
        
        repo_.Append(pth, tmp);
        return repo_[pth];
    }
    spdlog::error("[ModelManager] Resource create failed : "+pth);
    return nullptr;
}

SRender::Resources::SModel* ModelManager::CreateEmptyResources(const std::string& pth){
    if (auto m = repo_[pth];m!=nullptr) {
        spdlog::info("[ModelManager] Resource already created : "+pth);
        return m;
    }
    auto tmp=std::make_unique<SRender::Resources::SModel>();
    tmp->path_=pth;
    repo_.Append(pth, tmp);
    return repo_[pth];
}

void ModelManager::LoadResourcesMultiThread(){
    std::vector<std::thread> assimpParseThreads;
    auto& scenemanager = SANASERVICE(SceneSys::SceneManager);
    for (auto& [_,model]:repo_.resources_){
        if (model->GetMeshes().empty())
            assimpParseThreads.emplace_back(&SRender::Resources::SModelLoader::LoadSimpleModel,
                model->path_, std::ref(*model),true,scenemanager.GetSceneLoadSetting());
    }
    for (auto& t:assimpParseThreads){
        t.join();
    }
    spdlog::info("[ASSET Loader] assimp parser finished");
    UploadAll();
    spdlog::info("[ASSET Loader] mesh upload finished");
}

void ModelManager::KillResource(const std::string &pth){
    repo_.Remove(pth);
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