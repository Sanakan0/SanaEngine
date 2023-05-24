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
        for (int i=0;i<model_->GetMeshes().size();++i){
            ImGui::Text("mesh_%d vertexsize: %d",i,model_->GetMeshes()[i]->VertexSize());
            ImGui::Text("mesh_%d idxsize: %d",i,model_->GetMeshes()[i]->IdxSize());
        }
        if (ImGui::CollapsingHeader("Material")){
            auto mat= model_->GetMaterial();
            ImGui::Text(mat->shaderfile_.c_str());
            for (auto& texs:mat->texturelist_){
                if (texs.DiffuseTex==nullptr) continue;
                if (ImGui::CollapsingHeader(texs.DiffuseTex->path.c_str(),ImGuiTreeNodeFlags_DefaultOpen)){
                    switch (texs.DiffuseTex->internalformat) {
                        case SRender::Resources::TexInternalFormat::Default:
                            ImGui::Text("internal: Default");
                            break;
                        case SRender::Resources::TexInternalFormat::DXT5:
                            ImGui::Text("internal: DXT5");
                            break;
                    }
                    
                    auto& io = ImGui::GetIO();
                    float my_tex_w = (float)texs.DiffuseTex->width/10;
                    float my_tex_h = (float)texs.DiffuseTex->height/10;
                    ImGui::BeginChild(texs.DiffuseTex->path.c_str(),ImVec2(0,my_tex_h),true,ImGuiWindowFlags_HorizontalScrollbar);
                    // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
                    // ImGui::Image(reinterpret_cast<void*>((uint64_t)texs.DiffuseTex->id) , ImVec2((float)texs.DiffuseTex->width/10,(float)texs.DiffuseTex->height/10),ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
                    // ImGui::PopStyleVar();
                    

                    {
                        
                        ImVec2 pos = ImGui::GetCursorScreenPos();
                        ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
                        ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
                        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
                        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
                        ImGui::Image(reinterpret_cast<void*>((uint64_t)texs.DiffuseTex->id) , ImVec2(my_tex_w,my_tex_h),ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
                        if (ImGui::IsItemHovered())
                        {
                            ImGui::BeginTooltip();
                            float region_sz = 32.0f;
                            float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
                            float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
                            float zoom = 10.0f;
                            if (region_x < 0.0f) { region_x = 0.0f; }
                            else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
                            if (region_y < 0.0f) { region_y = 0.0f; }
                            else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
                            ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                            ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
                            ImVec2 uv0 = ImVec2((region_x) / my_tex_w, 1.0-(region_y) / my_tex_h);
                            ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, 1.0-(region_y + region_sz) / my_tex_h);
                            ImGui::Image(reinterpret_cast<void*>((uint64_t)texs.DiffuseTex->id), ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
                            ImGui::EndTooltip();
                        }
                    }

                    ImGui::EndChild();
                }
            }
            
        }
    }
    
    
}


}