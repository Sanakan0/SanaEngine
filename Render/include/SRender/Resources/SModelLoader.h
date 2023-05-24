#pragma once
#include "SRender/Resources/SAnimation.h"
#include "SRender/Resources/SModel.h"
#include "SResourceManager/TextureManager.h"
namespace SRender::Resources{
class SModelLoader{
public:
    static bool LoadSimpleModel(std::string path,SModel& model,bool is_cached=false,const ModelLoadSetting& loadsetting=ModelLoadSetting());
    static bool LoadModelWithAnima(std::string path,SModel& model,std::vector<SAnimation>& animas);
    static void Initialize(ResourceManager::TextureManager* texture_manager);
    
    inline static ResourceManager::TextureManager* texture_manager_=nullptr;
private:
    static void AddEmptyMaterial(SModel& model);
};

}