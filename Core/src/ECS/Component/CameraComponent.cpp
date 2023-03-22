#include "ECS/Component/CameraComponent.h"
#include "ECS/Component/Component.h"
#include "imgui/imgui.h"
namespace ECS::Components {

CameraComponent::CameraComponent(Actor& parentactor):
Component(parentactor){

}

void CameraComponent::Tick(float delta_t){

}
DrawCmd CameraComponent::GetInspectorDrawCmd() {
    return std::bind(&CameraComponent::DrawInspector,this);
}

void CameraComponent::DrawInspector() {
    if(ImGui::CollapsingHeader("Camera")){
        
    }
}





}