#include "ECS/Component/MeshComponent.h"
#include "SMath/Quaternion.h"
#include "Serialize/Serializer.h"
#include "glm/fwd.hpp"
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
    euler_xyz_deg_ = sm::Quat2Eul(trans_.GetOrienW());
    auto meshcomp = static_cast<MeshComponent*>(
        parentactor_.GetComponent("MeshComponent"));
    if (!meshcomp) return;
    trans_.SetPosW(meshcomp->GetModel()->GetBoundingSphere()->pos);
    trans_.SetPosL(-trans_.GetPosW());
}

void TransformComponent::Tick(float delta_t){
    
}



void TransformComponent::DrawInspector(){
    if(ImGui::CollapsingHeader("Transform")){
        auto tmp_deg_=sm::Quat2Eul(trans_.GetOrienW());
        auto tmppos = trans_.GetPosW();
        ImGui::DragFloat("trans x", &tmppos.x);
        ImGui::DragFloat("trans y", &tmppos.y);
        ImGui::DragFloat("trans z", &tmppos.z);
        
        trans_.SetPosW(tmppos);

        auto euler_xyz_deg = trans_.GetEulerW();
        ImGui::DragFloat("roll (x-axis)", &euler_xyz_deg.x);
        ImGui::DragFloat("pitch (y-axis)", &euler_xyz_deg.y);
        ImGui::DragFloat("yaw (z-axis)", &euler_xyz_deg.z);
        trans_.SetEulerW(euler_xyz_deg);
        
        ImGui::InputFloat3("Deserialized eular", &tmp_deg_.x,"%.4f",ImGuiInputTextFlags_ReadOnly);
        if (ImGui::Button("reset trans")){
            trans_.SetPosW(-trans_.GetPosL());
        }
        
    
    }
    
}


void TransformComponent::Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {
    SCore::Serializer::SerializeVec3(p_doc, p_node, "local_Position", trans_.GetPosL());
    SCore::Serializer::SerializeVec3(p_doc, p_node, "local_Scale", trans_.GetScaleL());
    SCore::Serializer::SerializeQuat(p_doc, p_node, "local_Orien_Quat", trans_.GetOrienL());

    SCore::Serializer::SerializeVec3(p_doc, p_node, "world_Position", trans_.GetPosW());
    SCore::Serializer::SerializeVec3(p_doc, p_node, "world_Scale", trans_.GetScaleW());
    SCore::Serializer::SerializeQuat(p_doc, p_node, "world_Orien_Quat", trans_.GetOrienW());
 

}

void TransformComponent::Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {
    
    glm::vec3 localpos,worldpos,localscale,worldscale;
    glm::quat localorien,worldorien;
    
    SCore::Serializer::DeserializeVec3(p_doc, p_node, "local_Position", localpos);
    SCore::Serializer::DeserializeVec3(p_doc, p_node, "local_Scale", localscale);
    SCore::Serializer::DeserializeQuat(p_doc, p_node, "local_Orien_Quat", localorien);

    SCore::Serializer::DeserializeVec3(p_doc, p_node, "world_Position", worldpos);
    SCore::Serializer::DeserializeVec3(p_doc, p_node, "world_Scale", worldscale);
    SCore::Serializer::DeserializeQuat(p_doc, p_node, "world_Orien_Quat", worldorien);

    trans_.SetOrienL(localorien);
    trans_.SetOrienW(worldorien);
    trans_.SetPosL(localpos);
    trans_.SetPosW(worldpos);
    trans_.SetScaleL(localscale);
    trans_.SetScaleW(worldscale);

    euler_xyz_deg_ = sm::Quat2Eul(worldorien);


}



}