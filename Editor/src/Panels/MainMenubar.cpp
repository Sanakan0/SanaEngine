#include "ECS/Component/CameraComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SEditor/Core/AssetLoader.h"
#include "SEditor/Util/NfdDialog.h"
#include "SceneSys/SceneManager.h"
#include "imgui/imgui.h"
#include <SEditor/Panels/MainMenubar.h>
namespace SEditor::Panels{

MainMenubar::MainMenubar():
assetloader_(SANASERVICE(Core::AssetLoader)),
scenemanager_(SANASERVICE(SceneSys::SceneManager))
{

}

void MainMenubar::DrawImpl(float deltat){
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("SanaEngine")){
            if (ImGui::MenuItem("Load tile")){
                auto res = Util::NfdDialog::OpenFolderDlg();
                if (res!=""){
                    assetloader_.LoadTiles(res);
                }
            }
            if (ImGui::BeginMenu("Add Actor")){
                if (ImGui::MenuItem("Add Camera")){
                    auto& tmpa=scenemanager_.GetScene()->CreateActor("Camera");
                    tmpa.AddComponent<ECS::Components::CameraComponent>();
                    tmpa.AddComponent<ECS::Components::TransformComponent>();
                    tmpa.GetTransformComponent()->trans_.world_pos_=scenemanager_.cursor_pos_;
                    scenemanager_.SetActiveCamera(tmpa.GetID());
                    scenemanager_.SetSelectedActor(tmpa.GetID());
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