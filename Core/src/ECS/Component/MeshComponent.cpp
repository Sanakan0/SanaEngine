#include "ECS/Component/Component.h"
#include "imgui/imgui.h"
#include <ECS/Component/MeshComponent.h>
namespace ECS::Components {
MeshComponent::MeshComponent(Actor& parentactor,SRender::Resources::SModel* model):
Component(parentactor),
model_(model)
{

}

void MeshComponent::Tick(float delta_t){
    
}

DrawCmd MeshComponent::GetInspectorDrawCmd(){
    return std::bind(&MeshComponent::DrawInspector,this);
}



void MeshComponent::DrawInspector(){
    if (ImGui::CollapsingHeader("MeshComponent")){
        ImGui::Text( model_->path_.c_str());
    }
    
}


}