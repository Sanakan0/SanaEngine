#include "ECS/Component/Component.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <ECS/Component/MeshComponent.h>
#include <stdint.h>
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
        if (ImGui::CollapsingHeader("Material")){
            auto mat= model_->GetMaterial();
            ImGui::Text(mat->shaderfile_.c_str());
            for (auto& texs:mat->texturelist_){
                if (texs.DiffuseTex==nullptr) continue;
                if (ImGui::CollapsingHeader(texs.DiffuseTex->path.c_str())){
                    
                    ImGui::BeginChild("textureimg",ImVec2(0,0),true,ImGuiWindowFlags_HorizontalScrollbar);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
                    ImGui::Image(reinterpret_cast<void*>((uint64_t)texs.DiffuseTex->id) , ImVec2((float)texs.DiffuseTex->width/10,(float)texs.DiffuseTex->height/10),ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
                    ImGui::PopStyleVar();
                    ImGui::EndChild();
                }
            }
            
        }
    }
    
    
}


}