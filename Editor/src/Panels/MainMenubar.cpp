#include "ECS/Component/CameraComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SEditor/Core/AssetLoader.h"
#include "SceneSys/SceneManager.h"
#include "imgui/imgui.h"
#include "nfd.h"
#include "spdlog/spdlog.h"
#include <SEditor/Panels/MainMenubar.h>
namespace SEditor::Panels{

MainMenubar::MainMenubar():
assetloader_(SANASERVICE(Core::AssetLoader)),
scenemanager_(SANASERVICE(SceneSys::SceneManager))
{

}

void MainMenubar::DrawImpl(){
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("SanaEngine")){
            if (ImGui::MenuItem("Load tile")){
                nfdchar_t* outPath = NULL;
                nfdresult_t result = NFD_PickFolder(NULL, &outPath);
                if ( result == NFD_OKAY ) {
                    auto filepth = std::string(outPath);
                    free(outPath);
                    spdlog::info("[NFD] Success! "+filepth);
                    assetloader_.LoadTiles(filepth);
                    
                }
                else if ( result == NFD_CANCEL ) {
                    spdlog::info("[NFD] User pressed cancel.");
                }
                else {
                    spdlog::error("[NFD] "+std::string(NFD_GetError()));
                }
            }
            if (ImGui::BeginMenu("Add Actor")){
                if (ImGui::MenuItem("Add Camera")){
                    auto& tmpa=scenemanager_.GetScene()->CreateActor();
                    tmpa.SetName("Camera");
                    tmpa.AddComponent<ECS::Components::CameraComponent>();
                    tmpa.AddComponent<ECS::Components::TransformComponent>();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")){
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

}



}