#include "ECS/Component/Component.h"
#include "ECS/Component/MeshComponent.h"
#include "imgui/imgui.h"
#include <ECS/Component/MaterialComponent.h>
namespace ECS::Components {
MaterialComponent::MaterialComponent(Actor& parentactor,SRender::Resources::SMaterial* material):
Component(parentactor)
{
    material_=material;

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