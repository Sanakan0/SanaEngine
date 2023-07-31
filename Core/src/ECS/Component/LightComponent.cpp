#include "ECS/Component/LightComponent.h"
#include "Serialize/Serializer.h"
#include "glm/fwd.hpp"
#include "imgui/imgui.h"

namespace ECS::Components {

LightComponent::LightComponent(Actor& parentactor):
Component(parentactor){

}

void LightComponent::Tick(float delta_t){

}

void LightComponent::DrawInspector() {
    if(ImGui::CollapsingHeader("light")){
        
    }
}
void LightComponent::Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {

}

void LightComponent::Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {

}

}