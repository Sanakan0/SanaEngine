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
    if (ImGui::Button("idtest")) std::cout <<"fk1\n";
    if (ImGui::Button("idtest##12")) std::cout <<"fk2\n";
    if (ImGui::TreeNode("root")){
        if (ImGui::Button("idtest")) std::cout <<"fk3\n";
        for (auto &actor:scenemanager_.GetScene()->GetActors()){
            
            if (ImGui::Selectable(std::to_string(actor.second->GetID()).c_str())){
                scenemanager_.SetSelectedActor(actor.second->GetID());
            }
            RightClickMenu();
        
        }
    }
    
}





}