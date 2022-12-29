#pragma once
#include <SRender/Resources/STexture.h>
#include "SResourceManager/ResourceRepo.h"
namespace ResourceManager {

class TextureManager{
public:
    public:
    SRender::Resources::STexture* CreateResources(const std::string& pth);
    SRender::Resources::STexture* CreateResources(const std::string& name,uint32_t width,uint32_t height,void* data);
    void KillResource(const std::string& pth);
    void ReloadResource(const std::string& pth);
    void ClearAll();
private:
    ResourceRepo<SRender::Resources::STexture> repo_;
};

}