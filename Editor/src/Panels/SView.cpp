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


SView::SView():camctrl_(*this,SANASERVICE(SWnd::Context),cam_){
    scrollable_=false;

    
}



void SView::FillUBO(){
    auto& editor_ubo = SANASERVICE(SRender::Buffers::GLUniformBuffer);
    editor_ubo.BufferSubData(cam_.GetViewMat(),0);
    editor_ubo.BufferSubData(cam_.GetProjectionMat(),sizeof(glm::mat4)*1);
    editor_ubo.BufferSubData(cam_.GetProjectionMat()*cam_.GetViewMat(),sizeof(glm::mat4)*2); //viewprj
    editor_ubo.BufferSubData(camctrl_.GetPos(),sizeof(glm::mat4)*3);
}

void SView::UpdateViewCam(float deltat){
    camctrl_.HandleInputs(deltat);
    auto[w,h] = canvas_size_;
    cam_.CacheViewMat(camctrl_.GetPos(),camctrl_.GetOrien());
    cam_.CacheProjectionMat(w, h);
    fbo_.Resize(w,h);
    FillUBO();
}

static void ShowExampleAppSimpleOverlay(bool* p_open,ImVec2 pos,ImVec2 sz,std::string name)
{
    static int corner = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_ChildWindow|ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1)
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = pos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = sz;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    //ImGui::SetNextWindowBgAlpha(0.35f);
    
    window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
    //ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);
    ImGui::Begin(name.c_str(), p_open, window_flags);
    ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
        ImGui::Separator();
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
            if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            if (p_open && ImGui::MenuItem("Close")) *p_open = false;
            ImGui::EndPopup();
        }
        if (ImGui::Button("fuk")){

        }
    ImGui::End();
    //ImGui::EndChild();
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