#include "SEditor/Core/AssetLoader.h"
#include "SCore/Global/ServiceLocator.h"
#include "SResourceManager/ModelManager.h"
#include "SResourceManager/TextureManager.h"
#include "SceneSys/SceneManager.h"
#include "iconv/iconv.h"
#include "spdlog/spdlog.h"
#include <filesystem>
#include <string>
namespace SEditor::Core{

AssetLoader::AssetLoader():
scenemanager_(SANASERVICE(SceneSys::SceneManager)),
modelmanager_(SANASERVICE(ResourceManager::ModelManager)),
texturemanager_(SANASERVICE(ResourceManager::TextureManager))
{

}

void AssetLoader::LoadTiles(const std::string& filename,int st,int cnt,const SRender::Resources::ModelLoadSetting& loadsetting){
     namespace fs = std::filesystem;
    loadsetting_=loadsetting;
    //fs::path tile_pth(R"(D:\beihang reconstruction data\dxobj)");
    //fs::path tile_pth(R"(E:\user\cnt0\beihang reconstruction data\dxobj)");
    size_t srclen = filename.size();
    size_t dstlen = 200;
    wchar_t dst[200];
    iconv_t conv = iconv_open("wchar_t","utf-8");
    std::string tmpstr(filename);
    char* inptr = tmpstr.data();
    char* outptr = reinterpret_cast<char*>(dst);
    auto tmpres = iconv(conv,&inptr,&srclen,&outptr,&dstlen);
    
     
    auto tmpwstr = std::wstring(dst);
    auto tile_pth = std::filesystem::u8path(filename);

    int cur=0;

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
                    threadtest.emplace_back(&SEditor::Core::AssetLoader::LoadAModelTask,this,objpth.generic_u8string(),cur-st);
                }
            }
            if (cur==ed) break;
        }
        for (auto& t:threadtest){
            t.join();
        }
        spdlog::info("[ASSET Loader] assimp parser finished");
        modelmanager_.UploadAll();
        spdlog::info("[ASSET Loader] mesh upload finished");
        texturemanager_.UploadAll();
        spdlog::info("[ASSET Loader] texture upload finished");
    }
}


void AssetLoader::LoadAModelTask(std::string pth,int idx){
    auto tmpmodel = modelmanager_.CreateResources(pth,true,loadsetting_);
    //renderpass.render_resources_[idx] = tmpmodel;
    if (tmpmodel == nullptr) return;
    auto& tmpa=scenemanager_.GetScene()->CreateActor("Tile_");
    
    tmpa.AddComponent<ECS::Components::MeshComponent>(tmpmodel);
    if (tmpmodel->GetMaterial()){
        //add matcomp
    }
    tmpa.AddComponent<ECS::Components::TransformComponent>();
}

}