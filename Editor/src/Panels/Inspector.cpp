#include "SCore/Global/ServiceLocator.h"
#include "SceneSys/SceneManager.h"
#include "imgui/imgui.h"
#include <SEditor/Panels/Inspector.h>
#include <functional>
#include <sstream>
#include <string>
namespace SEditor::Panels{

const std::string Inspector::componentlist[]{
    "TransformComponent",
    "MeshComponent"
};

Inspector::Inspector():
scenemanager_(SANASERVICE(SceneSys::SceneManager))
{
    name_="Inspector";
}

void Inspector::RightClickMenu(){
    if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
    {
        ImGui::Text("This a popup!");
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

void Inspector::DrawContent(){
    if (selected_actor_!=scenemanager_.GetSelectedActor()){
        selected_actor_=scenemanager_.GetSelectedActor();
        component_drawlist_.Clear();
        if (selected_actor_!=nullptr){
            for (auto& compname:componentlist){
                auto tmp = selected_actor_->GetComponent(compname);
                component_drawlist_+=tmp->GetInspectorDrawCmd();
            }
        }
    }
    
    if (selected_actor_){
        std::stringstream ss;
        ss <<selected_actor_->GetName()<<"_"<<selected_actor_->GetID();
        ImGui::Text(ss.str().c_str());
        component_drawlist_.Invoke();    
    }
    
    
}





}