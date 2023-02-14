#include "SResourceManager/TextureManager.h"
#include "SRender/Resources/STexture.h"
#include "SRender/Resources/STextureLoader.h"
#include "SResourceManager/Util.h"
#include <stdint.h>
namespace ResourceManager {

SRender::Resources::STexture* TextureManager::CreateResources(const std::string& pth){
    auto tmp = SRender::Resources::STextureLoader::LoadFromFile(Util::GetFullPath(pth));
    repo_.Append(pth, tmp);
    return tmp;
}
SRender::Resources::STexture* TextureManager::CreateResources(const std::string& name,uint32_t width,uint32_t height,void* data){
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

void TextureManager::ReloadResource(const std::string &pth){
    if (auto tmp = repo_[pth];tmp!=nullptr){
        //SRender::Resources::STextureLoader::RecompileShader(*tmpshaderp, pth);
    }
    
}

}