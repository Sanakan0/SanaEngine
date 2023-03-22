#pragma once


#include "SResourceManager/ModelManager.h"
#include "SResourceManager/TextureManager.h"
#include "SceneSys/SceneManager.h"
namespace SEditor::Core{

class AssetLoader{
public:
    AssetLoader();
    void LoadTiles(const std::string& filename);


private:
    void LoadAModelTask(std::string pth,int idx);

    SceneSys::SceneManager& scenemanager_;
    ResourceManager::ModelManager& modelmanager_;
    ResourceManager::TextureManager& texturemanager_;
};


}