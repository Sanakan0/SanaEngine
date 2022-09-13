#include "SGUI/Panels/WndPanel.h"
#include <iostream>
namespace SGUI::Panels{


WndPanel::WndPanel():fbo_(){}

WndPanel::~WndPanel(){}



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
            
            ImVec2 tmp = ImGui::GetCursorScreenPos(); 
            ImVec2 tmp2 = ImGui::GetWindowPos();
           // std::cout << tmp2.x << " " <<tmp2.y<<" : ";
            //std::cout << tmp.x << " - " << tmp.y << "         \r";
            
            ImGui::Image((void*)fbo_.tex_buf_id_, ImVec2(canvas_size_.first,canvas_size_.second), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
              
        }
        ImGui::End();
    }
    ImGui::PopStyleVar();
}

}