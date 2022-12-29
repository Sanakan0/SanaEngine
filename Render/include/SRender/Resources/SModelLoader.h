#pragma once
#include "SRender/Resources/SAnimation.h"
#include "SRender/Resources/SModel.h"
#include "SResourceManager/TextureManager.h"
namespace SRender::Resources{
class SModelLoader{
public:
    static void LoadSimpleModel(std::string path,SModel& model);
    static void LoadModelWithAnima(std::string path,SModel& model,std::vector<SAnimation>& animas);
    inline static ResourceManager::TextureManager* texture_manager_=nullptr;
};

}