#include "SEditor/Core/CameraCtrl.h"

namespace SEditor::Core{

CameraCtrl::CameraCtrl(SGUI::Panels::WndPanel& view,SWnd::Context& wndcontext,Core::Cameraa& cam)
:wndcontext_(wndcontext),view_(view),cam_(cam){

}

void CameraCtrl::HandleInputs(float delta_time){
    if(view_.hovered_||inputmanager_.first_mouse_pressed_){
        inputmanager_.UpdateMouse();
        HandleZoom();
    }else{

    }
    //std::cout << inputmanager_.curpos_.first << " " << inputmanager_.curpos_.second <<"     < \r";
    if (inputmanager_.mid_btn_){
        auto[dx,dy] = inputmanager_.GetCursorDelta();
        if (inputmanager_.GetGlfwKeyState(GLFW_KEY_LEFT_SHIFT)){
            auto[w,h] = view_.canvas_size_;
            float apix = cam_.CalcDisPerPix(w,h);
            cam_.translate(glm::vec3(-dx*apix,dy*apix,0));
        }else{
            cam_.around(-dx*around_speed_,-dy*around_speed_);
        }
    }

    
}

void CameraCtrl::HandleZoom(){
    ImGuiIO& io = ImGui::GetIO();
    if (io.MouseWheel){
        cam_.zoom(io.MouseWheel/abs(io.MouseWheel)* -zoom_speed_);
    }
    
}



}