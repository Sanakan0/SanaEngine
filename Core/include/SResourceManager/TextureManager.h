#pragma once
#include <SRender/Resources/STexture.h>
#include "SResourceManager/ResourceRepo.h"
namespace ResourceManager {

class TextureManager{
public:
    public:
    SRender::Resources::STexture* CreateResources(const std::string& pth,bool is_cached=false,const SRender::Resources::TextureInfo& texinfo=SRender::Resources::TextureInfo());
    SRender::Resources::STexture* CreateResources(const std::string& name,uint32_t width,uint32_t height,void* data,bool is_cached=false);
    void KillResource(const std::string& pth);
    void ReloadResource(const std::string& pth);
    void ClearAll();
    void UploadAll();
private:
    ResourceRepo<SRender::Resources::STexture> repo_;
};

}