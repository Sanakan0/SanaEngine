#include "ECS/Component/CameraComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SGUI/Core/UImanager.h"
#include "SceneSys/SceneManager.h"
#include "imgui/imgui.h"
#include <SEditor/Panels/Inspector.h>
#include <cstring>
#include <functional>
#include <sstream>
#include <string>
namespace SEditor::Panels{

const std::string Inspector::componentlist[]{
    "TransformComponent",
    "MeshComponent",
    "CameraComponent",
    "RectifyComponent",
    "LightComponent",
    "DatasetGenComponent"
};

Inspector::Inspector():
scenemanager_(SANASERVICE(SceneSys::SceneManager)),
sceneview_(SANASERVICE(SGUI::Core::UImanager).GetPanel<SEditor::Panels::SceneView>("Scene View"))
{
    name_="实体编辑";
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
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    ImGui::Spacing();
    
    
    
    if (selected_actor_!=scenemanager_.GetSelectedActor()){
        selected_actor_=scenemanager_.GetSelectedActor();
        component_drawlist_.Clear();
        if (selected_actor_!=nullptr){
            for (auto& compname:componentlist){
                auto tmp = selected_actor_->GetComponent(compname);
                if (tmp) component_drawlist_+=tmp->GetInspectorDrawCmd();
            }
        }
    }
    
    if (selected_actor_){
        ImGui::Text(selected_actor_->GetName().c_str());
        if (selected_actor_->GetComponent("CameraComponent")) {
            if (ImGui::Button("Set Active Cam")) scenemanager_.SetActiveCamera(selected_actor_->GetID());
        }
        if (ImGui::Button("Move to Actor")){
            auto meshcomp = (ECS::Components::MeshComponent*)selected_actor_->GetComponent("MeshComponent");
            sceneview_.camctrl_.Move2Target(selected_actor_->GetTransformComponent()->trans_.GetPosW(), meshcomp?meshcomp->GetModel()->GetBoundingSphere()->radius:5); 
        }

        std::stringstream ss;
        auto tmp = selected_actor_->GetName();
        static char cbuf[50];
        strcpy_s(cbuf,tmp.c_str());
        ImGui::InputText("name", cbuf,IM_ARRAYSIZE(cbuf));
        selected_actor_->SetName(cbuf);
        ss <<"id_"<<selected_actor_->GetID();
        ImGui::Text(ss.str().c_str());
        component_drawlist_.Invoke();    
    }
    ImGui::PopItemWidth();
    
}





}