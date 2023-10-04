#include "ECS/Actor.h"
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
       
        static auto ActorSelectable = [&](ECS::Actor& actor){
            return ImGui::Selectable((actor.GetName() + "_" + std::to_string(actor.GetID())).c_str(),actor.GetID()==selectedActorId);
        };

        if (ImGui::TreeNode("model")){
            for  (auto &meshcomp:scenemanager_.GetScene()->GetBasicRenderComponent().meshcomps){
                auto& actor = meshcomp->parentactor_;
                if (ActorSelectable(actor)){
                    scenemanager_.SetSelectedActor(actor.GetID());
                }
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("camera")){
            for  (auto &meshcomp:scenemanager_.GetScene()->GetBasicRenderComponent().camcomps){
                auto& actor = meshcomp->parentactor_;
                if (ActorSelectable(actor)){
                    scenemanager_.SetSelectedActor(actor.GetID());
                }
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("img rectifier")){
            for  (auto& [_,actor]:scenemanager_.GetScene()->GetActors()){
                if (actor->GetComponent("RectifyComponent")){
                    //ImGui::PushID(("img rectifier"+std::to_string(actor->GetID())).c_str());
                    if (ActorSelectable(*actor)){
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
                    if (ActorSelectable(*actor)){
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