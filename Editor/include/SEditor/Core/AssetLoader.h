#pragma once


#include "SResourceManager/ModelManager.h"
#include "SResourceManager/TextureManager.h"
#include "SceneSys/SceneManager.h"
namespace SEditor::Core{

class AssetLoader{
public:
    AssetLoader();
    void LoadTiles(const std::string& filename,int st=0,int cnt=60,const SRender::Resources::ModelLoadSetting& loadsetting_=SRender::Resources::ModelLoadSetting());


private:
    void LoadAModelTask(std::string pth,int idx);

    SceneSys::SceneManager& scenemanager_;
    ResourceManager::ModelManager& modelmanager_;
    ResourceManager::TextureManager& texturemanager_;
    SRender::Resources::ModelLoadSetting loadsetting_;
};


}