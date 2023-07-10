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




void MaterialComponent::DrawInspector(){
    if (ImGui::CollapsingHeader("MaterialComponent")){
        ImGui::Text( "Material");
    }
    
}

void MaterialComponent::Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {
    
}

void MaterialComponent::Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {
    
}



}