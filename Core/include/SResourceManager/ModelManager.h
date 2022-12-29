#pragma once
#include <SRender/Resources/SModel.h>
#include "SResourceManager/ResourceRepo.h"
namespace ResourceManager {

class ModelManager{
public:
    public:
    SRender::Resources::SModel* CreateResources(const std::string& pth);
    void KillResource(const std::string& pth);
    void ReloadResource(const std::string& pth);
    void ClearAll();
private:
    ResourceRepo<SRender::Resources::SModel> repo_;
};

}