#include "imgui/imgui.h"
#include <ECS/Component/TransformComponent.h>
#include <functional>
namespace ECS::Components {
TransformComponent::TransformComponent(Actor& parentactor):Component(parentactor){

}

void TransformComponent::Tick(float delta_t){
    
}

DrawCmd TransformComponent::GetInspectorDrawCmd(){
    return std::bind(&TransformComponent::DrawInspector,this);
}

void TransformComponent::DrawInspector(){
    if(ImGui::CollapsingHeader("Transform")){
        ImGui::DragFloat("trans x", &trans_.world_pos_.x);
        ImGui::DragFloat("trans y", &trans_.world_pos_.y);
        ImGui::DragFloat("trans z", &trans_.world_pos_.z);
    }
    
}

}