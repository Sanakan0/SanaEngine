#include "SGUI/Panels/WndPanel.h"
#include "imgui/imgui.h"
#include <iostream>
namespace SGUI::Panels{


WndPanel::WndPanel():fbo_(){}





std::pair<int,int> WndPanel::GetImguiSize(){
    ImVec2 sz = ImGui::GetWindowSize();
    return{sz.x,sz.y};
}
std::pair<int,int> WndPanel::GetImguiPos(){
    ImVec2 pos = ImGui::GetWindowPos();
    return{pos.x,pos.y};
}
std::pair<int,int> WndPanel::GetCanvasPos(){
    ImVec2 pos = ImGui::GetCursorScreenPos(); 
    return{pos.x,pos.y};
}
std::pair<int,int> WndPanel::GetCanvasSize(){
    int barheight=canvas_pos_.second-pos_.second;
    return{size_.first,size_.second-barheight};
}

void WndPanel::UpdateWndStates(){
    pos_=GetImguiPos();
    canvas_pos_=GetCanvasPos();
    size_=GetImguiSize();
    canvas_size_=GetCanvasSize();
}

static void ShowExampleAppSimpleOverlay(bool* p_open,ImVec2 pos,ImVec2 sz)
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
    ImGui::Begin("Example: Simple overlay", p_open, window_flags);
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


void WndPanel::DrawImpl(){
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    if (opened_){
        int windowFlags = ImGuiWindowFlags_None;

		if (!resizable_)				windowFlags |= ImGuiWindowFlags_NoResize;
		if (!movable_)					windowFlags |= ImGuiWindowFlags_NoMove;
		if (!dockable_)					windowFlags |= ImGuiWindowFlags_NoDocking;
        windowFlags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        ImVec2 minszcons = {0,0};
        ImVec2 maxszcons = {10000,10000};
        ImGui::SetNextWindowSizeConstraints(minszcons,maxszcons);
        if (ImGui::Begin(name_.c_str(),&opened_,windowFlags)){
            hovered_ = ImGui::IsWindowHovered();
            focused_ = ImGui::IsWindowFocused();
            
            UpdateWndStates();
            
            // ImVec2 tmp = ImGui::GetCursorScreenPos(); 
            // ImVec2 tmp2 = ImGui::GetWindowPos();
           // std::cout << tmp2.x << " " <<tmp2.y<<" : ";
            //std::cout << tmp.x << " - " << tmp.y << "         \r";
            //ShowExampleAppSimpleOverlay(&opened_,ImVec2(pos_.first,pos_.second),ImVec2(size_.first,size_.second));
            
            ImGui::Image((void*)fbo_.tex_buf_id_, ImVec2(canvas_size_.first,canvas_size_.second), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
            
            
            
        }
        ImGui::End();
    }
    ImGui::PopStyleVar();
}

}