#include "ECS/Component/Component.h"
#include "imgui/imgui.h"
#include <ECS/Component/MaterialComponent.h>
namespace ECS::Components {
MaterialComponent::MaterialComponent(Actor& parentactor):
Component(parentactor)
{

}

void MaterialComponent::Tick(float delta_t){
    
}

DrawCmd MaterialComponent::GetInspectorDrawCmd(){
    return std::bind(&MaterialComponent::DrawInspector,this);
}



void MaterialComponent::DrawInspector(){
    if (ImGui::CollapsingHeader("MaterialComponent")){
        ImGui::Text( "Material");
    }
    
}


}