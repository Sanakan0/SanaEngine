#include "ECS/Component/RectifyComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SceneSys/SceneManager.h"
#include "imgui/imgui.h"
#include <SEditor/Panels/Hierarchy.h>
#include <string>
namespace SEditor::Panels{

Hierarchy::Hierarchy():
scenemanager_(SANASERVICE(SceneSys::SceneManager))
{
    name_="Hierarchy";
}

void Hierarchy::RightClickMenu(){
    if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
    {
        ImGui::Text("This a popup!");
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

void Hierarchy::DrawContent(){
    ECS::ActorID selectedActorId=scenemanager_.GetSelectedActorID();
    if (ImGui::TreeNode("root")){
        if (ImGui::TreeNode("model")){
            for  (auto &meshcomp:scenemanager_.GetScene()->GetBasicRenderComponent().meshcomps){
                auto& actor = meshcomp->parentactor_;
                if (ImGui::Selectable((actor.GetName() + std::to_string(actor.GetID())).c_str(),actor.GetID()==selectedActorId)){
                    scenemanager_.SetSelectedActor(actor.GetID());
                }
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("camera")){
            for  (auto &meshcomp:scenemanager_.GetScene()->GetBasicRenderComponent().camcomps){
                auto& actor = meshcomp->parentactor_;
                if (ImGui::Selectable((actor.GetName() + std::to_string(actor.GetID())).c_str(),actor.GetID()==selectedActorId)){
                    scenemanager_.SetSelectedActor(actor.GetID());
                }
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("img rectifier")){
            for  (auto& [_,actor]:scenemanager_.GetScene()->GetActors()){
                if (actor->GetComponent("RectifyComponent")){
                    //ImGui::PushID(("img rectifier"+std::to_string(actor->GetID())).c_str());
                    if (ImGui::Selectable((actor->GetName() + std::to_string(actor->GetID())).c_str(),actor->GetID()==selectedActorId)){
                        scenemanager_.SetSelectedActor(actor->GetID());
                    }
                    //ImGui::PopID();
                
                }
                
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("light")){
            for  (auto& [_,actor]:scenemanager_.GetScene()->GetActors()){
                if (actor->GetComponent("LightComponent")){
                    if (ImGui::Selectable((actor->GetName() + std::to_string(actor->GetID())).c_str(),actor->GetID()==selectedActorId)){
                        scenemanager_.SetSelectedActor(actor->GetID());
                    }
                }
                
            }
            ImGui::TreePop();
        }



        ImGui::TreePop();
    }
    
}





}