#include "SResourceManager/TextureManager.h"
#include "SRender/Resources/STexture.h"
#include "SRender/Resources/STextureLoader.h"
#include "SResourceManager/Util.h"
#include "spdlog/spdlog.h"
#include <stdint.h>
namespace ResourceManager {

SRender::Resources::STexture* TextureManager::CreateResources(const std::string& pth,bool is_cached){
    if (auto i = repo_[pth]; i!=nullptr){
        spdlog::info("[TextureManager] Resource already created : "+pth);
        return i;
    } 
    auto tmp = is_cached?SRender::Resources::STextureLoader::LoadFromFile_cached(Util::GetFullPath(pth)):
        SRender::Resources::STextureLoader::LoadFromFile(Util::GetFullPath(pth));
    if (tmp!=nullptr) {
        repo_.Append(pth, tmp);
        return tmp;
    }
    spdlog::error("[TextureManager] Resource create failed : "+pth);
    return nullptr;
}
SRender::Resources::STexture* TextureManager::CreateResources(const std::string& name,uint32_t width,uint32_t height,void* data,bool is_cached){
    auto tmp = SRender::Resources::STextureLoader::LoadFromMemory(data,width,height,name);
    repo_.Append(name, tmp);
    return tmp;
}

void TextureManager::KillResource(const std::string &pth){
    if (auto tmp = repo_[pth];tmp!=nullptr){
        delete tmp;
        repo_.Remove(pth);
    }
}

void TextureManager::ClearAll(){
    repo_.ClearAllResources();
}
void TextureManager::UploadAll(){
    for (auto& i:repo_.resources_){
        i.second->UploadTexture();
    }
}

void TextureManager::ReloadResource(const std::string &pth){
    if (auto tmp = repo_[pth];tmp!=nullptr){
        //SRender::Resources::STextureLoader::RecompileShader(*tmpshaderp, pth);
    }
    
}

}