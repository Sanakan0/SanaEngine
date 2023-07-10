#include "ECS/Component/MeshComponent.h"
#include "SMath/Quaternion.h"
#include "Serialize/Serializer.h"
#include "glm/geometric.hpp"
#include "imgui/imgui.h"
#include <ECS/Component/TransformComponent.h>
#include <functional>
#include "ECS/Actor.h"
namespace ECS::Components {
TransformComponent::TransformComponent(Actor& parentactor):Component(parentactor){
    GetTransFromBounds();
}


void TransformComponent::GetTransFromBounds(){
    euler_xyz_deg_ = sm::Quat2Eul(trans_.world_orien_);
    auto meshcomp = static_cast<MeshComponent*>(
        parentactor_.GetComponent("MeshComponent"));
    if (!meshcomp) return;
    trans_.world_pos_ = meshcomp->GetModel()->GetBoundingSphere()->pos;
    trans_.local_pos_ = -trans_.world_pos_;
}

void TransformComponent::Tick(float delta_t){
    
}



void TransformComponent::DrawInspector(){
    if(ImGui::CollapsingHeader("Transform")){
        auto tmp_deg_=sm::Quat2Eul(trans_.world_orien_);
        ImGui::DragFloat("trans x", &trans_.world_pos_.x);
        ImGui::DragFloat("trans y", &trans_.world_pos_.y);
        ImGui::DragFloat("trans z", &trans_.world_pos_.z);
        
        ImGui::DragFloat("roll (x-axis)", &euler_xyz_deg_.x);
        ImGui::DragFloat("pitch (y-axis)", &euler_xyz_deg_.y);
        ImGui::DragFloat("yaw (z-axis)", &euler_xyz_deg_.z);
        trans_.world_orien_=sm::Eul2Quat(euler_xyz_deg_);
        
        ImGui::InputFloat3("Deserialized eular", &tmp_deg_.x,"%.4f",ImGuiInputTextFlags_ReadOnly);
        if (ImGui::Button("reset trans")){
            trans_.world_pos_=-trans_.local_pos_;
        }
        
    
    }
    
}


void TransformComponent::Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {
    SCore::Serializer::SerializeVec3(p_doc, p_node, "local_Position", trans_.local_pos_);
    SCore::Serializer::SerializeVec3(p_doc, p_node, "local_Scale", trans_.local_scale_);
    SCore::Serializer::SerializeQuat(p_doc, p_node, "local_Orien_Quat", trans_.local_orien_);

    SCore::Serializer::SerializeVec3(p_doc, p_node, "world_Position", trans_.world_pos_);
    SCore::Serializer::SerializeVec3(p_doc, p_node, "world_Scale", trans_.world_scale_);
    SCore::Serializer::SerializeQuat(p_doc, p_node, "world_Orien_Quat", trans_.world_orien_);
 

}

void TransformComponent::Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {
    

    
    SCore::Serializer::DeserializeVec3(p_doc, p_node, "local_Position", trans_.local_pos_);
    SCore::Serializer::DeserializeVec3(p_doc, p_node, "local_Scale", trans_.local_scale_);
    SCore::Serializer::DeserializeQuat(p_doc, p_node, "local_Orien_Quat", trans_.local_orien_);

    SCore::Serializer::DeserializeVec3(p_doc, p_node, "world_Position", trans_.world_pos_);
    SCore::Serializer::DeserializeVec3(p_doc, p_node, "world_Scale", trans_.world_scale_);
    SCore::Serializer::DeserializeQuat(p_doc, p_node, "world_Orien_Quat", trans_.world_orien_);

    euler_xyz_deg_ = sm::Quat2Eul(trans_.world_orien_);


}



}