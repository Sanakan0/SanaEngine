#include "SResourceManager/TextureManager.h"
#include "SRender/Resources/STexture.h"
#include "SRender/Resources/STextureLoader.h"
#include "SResourceManager/Util.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <stdint.h>
#include <future>
namespace ResourceManager {

SRender::Resources::STexture* TextureManager::CreateResources(const std::string& pth,bool is_cached,const SRender::Resources::TextureInfo& texinfo){
    if (auto i = repo_[pth]; i!=nullptr){
        spdlog::info("[TextureManager] Resource already created : "+pth);
        return i;
    } 
    auto tmprawp = is_cached?SRender::Resources::STextureLoader::LoadFromFile_cached(pth,texinfo):
        SRender::Resources::STextureLoader::LoadFromFile(pth,texinfo);
    auto tmp = std::unique_ptr<SRender::Resources::STexture>(tmprawp);
    if (tmp!=nullptr) {
        repo_.Append(pth, tmp);
        return repo_[pth];
    }
    spdlog::error("[TextureManager] Resource create failed : "+pth);
    return nullptr;
}
SRender::Resources::STexture* TextureManager::CreateResources(const std::string& name,uint32_t width,uint32_t height,void* data,bool is_cached){
    auto tmprawp = SRender::Resources::STextureLoader::LoadFromMemory(data,width,height,name);
    auto tmp = std::unique_ptr<SRender::Resources::STexture>(tmprawp);
    repo_.Append(name, tmp);
    return repo_[name];
}

void TextureManager::KillResource(const std::string &pth){
    repo_.Remove(pth);
}

void TextureManager::ClearAll(){
    repo_.ClearAllResources();
}
void TextureManager::UploadAll(){
    std::vector<std::future<bool>> futs; 
    futs.resize(repo_.resources_.size());
    int cnt=0;
    for (auto& i:repo_.resources_){
        futs[cnt++]=std::async([&i](){return i.second->LoadFromDisk();});
        //i.second->LoadFromDisk();
    }
    for (auto& i:futs) i.get();
    spdlog::info("[TextureManager] texture load from disk finished");
    for (auto& i:repo_.resources_){
        i.second->UploadTexture();
    }
    spdlog::info("[TextureManager] texture upload to gpu finished");
    for (auto& i:repo_.resources_){
        i.second->FreeRawData();
    }
}

void TextureManager::ReloadResource(const std::string &pth){
    if (auto tmp = repo_[pth];tmp!=nullptr){
        //SRender::Resources::STextureLoader::RecompileShader(*tmpshaderp, pth);
    }
    
}

}