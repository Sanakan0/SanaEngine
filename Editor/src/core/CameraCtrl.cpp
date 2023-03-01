#include "SEditor/Core/CameraCtrl.h"
#include <SMath/Quaternion.h>
namespace SEditor::Core{

CameraCtrl::CameraCtrl(SGUI::Panels::WndPanel& view,SWnd::Context& wndcontext,SRender::LowRenderer::Camera& cam)
:wndcontext_(wndcontext),view_(view),cam_(cam){

}

void CameraCtrl::HandleInputs(float delta_time){
    if(view_.hovered_||inputmanager_.first_mouse_pressed_){
        inputmanager_.UpdateMouse();
        HandleZoom();
    }else{

    }
    //std::cout << inputmanager_.curpos_.first << " " << inputmanager_.curpos_.second <<"     < \r";
    //HandleOrbitCamCtl(delta_time);
    HandleFpsCamCtl(delta_time);

    
}

void CameraCtrl::HandleOrbitCamCtl(float delta_time){
    if (inputmanager_.mid_btn_){
        auto[dx,dy] = inputmanager_.GetCursorDelta();
        if (inputmanager_.GetGlfwKeyState(GLFW_KEY_LEFT_SHIFT)){
            auto[w,h] = view_.canvas_size_;
            float apix = cam_.CalcDisPerPix(w,h);
            cam_.translate(glm::vec3(-dx*apix,dy*apix,0));
        }else{
            cam_.Orbit(-dx*around_speed_,-dy*around_speed_);
        }
    }
}

void CameraCtrl::HandleFpsCamCtl(float delta_time){
    if (inputmanager_.mid_btn_){
        auto step = move_speed_*delta_time;
        auto[dx,dy] = inputmanager_.GetCursorDelta();
        if (inputmanager_.GetGlfwKeyState(GLFW_KEY_W)){
            cam_.translate(sm::OglCamPrimForward*step);
        }
        if (inputmanager_.GetGlfwKeyState(GLFW_KEY_A)){
            cam_.translate(-sm::OglCamPrimRight*step);
        }
        if (inputmanager_.GetGlfwKeyState(GLFW_KEY_S)){
            cam_.translate(-sm::OglCamPrimForward*step);
        }
        if (inputmanager_.GetGlfwKeyState(GLFW_KEY_D)){
            cam_.translate(sm::OglCamPrimRight*step);
        }
        
        cam_.FpsRotate(-dx*around_speed_,-dy*around_speed_);
    }
}

void CameraCtrl::HandleZoom(){
    ImGuiIO& io = ImGui::GetIO();
    if (io.MouseWheel){
        cam_.zoom(io.MouseWheel/abs(io.MouseWheel)* -zoom_speed_);
    }
    
}



}