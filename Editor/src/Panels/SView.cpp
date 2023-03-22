#include "SEditor/Panels/SView.h"
#include "SCore/Global/ServiceLocator.h"
#include "SEditor/Core/RuntimeContext.h"
#include "SGUI/Core/UImanager.h"
#include "SResourceManager/Util.h"
#include "imgui/imgui.h"
#include "SGUI/Widgets/CustomWidgets.h"
#include <SGUI/IconsFontAwesome6.h>
#include <iostream>
namespace SEditor::Panels{


SView::SView():camctrl_(*this,SANASERVICE(SWnd::Context),cam_,cam_extrinsic_){
    scrollable_=false;
    window_padding_={0,0};
    
}



void SView::FillUBO(){
    auto& editor_ubo = SANASERVICE(SRender::Buffers::GLUniformBuffer);
    editor_ubo.BufferSubData(camctrl_.cam_->GetViewMat(),0);
    editor_ubo.BufferSubData(camctrl_.cam_->GetProjectionMat(),sizeof(glm::mat4)*1);
    editor_ubo.BufferSubData(camctrl_.cam_->GetProjectionMat()*camctrl_.cam_->GetViewMat(),sizeof(glm::mat4)*2); //viewprj
    editor_ubo.BufferSubData(camctrl_.GetPos(),sizeof(glm::mat4)*3);
}

void SView::UpdateViewCam(float deltat){
    camctrl_.HandleInputs(deltat);
    camctrl_.TickCamMove(deltat);
    auto[w,h] = canvas_size_;
    camctrl_.cam_->CacheViewMat(camctrl_.GetPos(),camctrl_.GetOrien());
    camctrl_.cam_->CacheProjectionMat(w, h);
    fbo_.Resize(w,h);
    FillUBO();
}





void SView::DrawContent(){
    
    
    // int windowFlags = ImGuiWindowFlags_None;
     static bool open=true;
    // if (ImGui::BeginChild("test",{100,200},windowFlags)){
    //     ImGui::Button("test");
    //     ImGui::EndChild();
    // }
    //showtest(&open, ImVec2( pos_.first,pos_.second), ImVec2(size_.first,size_.second));
    
    //ShowExampleAppSimpleOverlay(&open, ImVec2( pos_.first,pos_.second), ImVec2(size_.first,size_.second),name_+"childwind");
    ImGui::Image((void*)fbo_.tex_buf_id_, ImVec2(canvas_size_.first,canvas_size_.second), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
    
    ImGui::SetCursorPos({10,40});
    auto& tmpuimanager = SANASERVICE(SGUI::Core::UImanager);

  
    {
        ImGui::BeginGroup();

        SGUI::ToggleButton("cam_mode_sw",ICON_FA_CAMERA,ICON_FA_CAMERA_ROTATE,&camctrl_.is_fps_cam_mod_);
        //ImGui::ImageButton(ImTextureID user_texture_id, const ImVec2 &size)
        ImGui::EndGroup();
    }
    // if (ImGui::CollapsingHeader("tesst")){
    //     ImGui::Button("test");
    // }
    ImGui::SetCursorPos({size_.first-100.0f,100});
    SGUI::Campass(camctrl_.GetOrien());
    
}



}