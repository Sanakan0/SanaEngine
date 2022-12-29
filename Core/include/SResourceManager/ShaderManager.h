#pragma once
#include "SRender/Resources/GLShader.h"
#include <SRender/Resources/STexture.h>
#include <string>
#include "SResourceManager/ResourceRepo.h"
namespace ResourceManager {

class ShaderManager{
public:
    SRender::Resources::GLShader* CreateResources(const std::string& pth);
    void KillResource(const std::string& pth);
    void ReloadResource(const std::string& pth);
    void ClearAll();
private:
    ResourceRepo<SRender::Resources::GLShader> repo_;
};

}