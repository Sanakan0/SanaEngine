#pragma once
#include <SRender/Resources/SModel.h>
#include "SResourceManager/ResourceRepo.h"
#include "SRender/Resources/Settings.h"
namespace ResourceManager {

class ModelManager{
public:
    /*
    SRender::Resources::SModel* CreateResources(const std::string& pth,bool is_cached=false);
    if is_cached=true, Create resources to memory but wont upload to gpu memory (for multithread loading)
    */
    SRender::Resources::SModel* CreateResources(const std::string& pth,bool is_cached=false,const SRender::Resources::ModelLoadSetting& setting=SRender::Resources::ModelLoadSetting());
    //Delay resource loading
    SRender::Resources::SModel* CreateEmptyResources(const std::string& pth);

    void LoadResourcesMultiThread();

    bool LoadModel();
    void KillResource(const std::string& pth);
    void ReloadResource(const std::string& pth);
    void ClearAll();
    void UploadAll();
private:
    ResourceRepo<SRender::Resources::SModel> repo_;
};

}