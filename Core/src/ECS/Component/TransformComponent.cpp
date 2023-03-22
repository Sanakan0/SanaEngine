#include "ECS/Component/MeshComponent.h"
#include "SMath/Quaternion.h"
#include "glm/geometric.hpp"
#include "imgui/imgui.h"
#include <ECS/Component/TransformComponent.h>
#include <functional>
#include "ECS/Actor.h"
namespace ECS::Components {
TransformComponent::TransformComponent(Actor& parentactor):Component(parentactor){
    euler_xyz_deg_ = sm::Quat2Eul(trans_.world_orien_);
    auto meshcomp = static_cast<MeshComponent*>(
        parentactor_.GetComponent("MeshComponent"));
    if (!meshcomp) return;
    trans_.world_pos_ = meshcomp->GetModel()->GetBoundingSphere()->pos;
    trans_.local_pos_ = -trans_.world_pos_;
}

void TransformComponent::Tick(float delta_t){
    
}

DrawCmd TransformComponent::GetInspectorDrawCmd(){
    return std::bind(&TransformComponent::DrawInspector,this);
}

void TransformComponent::DrawInspector(){
    if(ImGui::CollapsingHeader("Transform")){
        euler_xyz_deg_=sm::Quat2Eul(trans_.world_orien_);
        ImGui::DragFloat("trans x", &trans_.world_pos_.x);
        ImGui::DragFloat("trans y", &trans_.world_pos_.y);
        ImGui::DragFloat("trans z", &trans_.world_pos_.z);
        
        ImGui::DragFloat("roll (x-axis)", &euler_xyz_deg_.x);
        ImGui::DragFloat("pitch (y-axis)", &euler_xyz_deg_.y);
        ImGui::DragFloat("yaw (z-axis)", &euler_xyz_deg_.z);
        trans_.world_orien_=sm::Eul2Quat(euler_xyz_deg_);

        if (ImGui::Button("reset trans")){
            trans_.world_pos_=-trans_.local_pos_;
        }
        
    
    }
    
}

}