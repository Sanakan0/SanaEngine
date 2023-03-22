#include "SEditor/Core/AssetLoader.h"
#include "SCore/Global/ServiceLocator.h"
#include "SResourceManager/ModelManager.h"
#include "SResourceManager/TextureManager.h"
#include "SceneSys/SceneManager.h"
#include <filesystem>
namespace SEditor::Core{

AssetLoader::AssetLoader():
scenemanager_(SANASERVICE(SceneSys::SceneManager)),
modelmanager_(SANASERVICE(ResourceManager::ModelManager)),
texturemanager_(SANASERVICE(ResourceManager::TextureManager))
{

}

void AssetLoader::LoadTiles(const std::string& filename){
     namespace fs = std::filesystem;
    //fs::path tile_pth(R"(D:\beihang reconstruction data\dxobj)");
    //fs::path tile_pth(R"(E:\user\cnt0\beihang reconstruction data\dxobj)");
    
    fs::path tile_pth(filename);
    int cnt=3;
    int cur=0;
    int st=1;
    int ed=st+cnt-1;
    //simplerenderpass.render_resources_.resize(cnt);
    std::vector<std::thread>threadtest;
    if (std::filesystem::is_directory(tile_pth)){
        for (auto& entry:std::filesystem::directory_iterator(tile_pth)){
            cur++;
            if (cur<st) continue;
            if (entry.is_directory()){
                auto& tmppth = entry.path();
                auto objpth = tmppth/(tmppth.filename().string()+".obj");
                if (fs::exists(objpth)){
                    //spdlog::info(objpth.generic_string());
                    // renderpass.render_resources_.push_back(
                    //     modelmanager.CreateResources(objpth.generic_string()));
                    ///task1(objpth.generic_string(),cur-st);
                    threadtest.emplace_back(&SEditor::Core::AssetLoader::LoadAModelTask,this,objpth.generic_string(),cur-st);
                }
            }
            if (cur==ed) break;
        }
        for (auto& t:threadtest){
            t.join();
        }
        modelmanager_.UploadAll();
        texturemanager_.UploadAll();
    }
}


void AssetLoader::LoadAModelTask(std::string pth,int idx){
    auto tmpmodel = modelmanager_.CreateResources(pth,true);
    //renderpass.render_resources_[idx] = tmpmodel;
    auto& tmpa=scenemanager_.GetScene()->CreateActor();
    
    tmpa.AddComponent<ECS::Components::MeshComponent>(tmpmodel);
    if (tmpmodel->GetMaterial()){
        //add matcomp
    }
    tmpa.AddComponent<ECS::Components::TransformComponent>();
}

}