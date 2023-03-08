#include "SEditor/Core/CameraCtrl.h"
#include <SMath/Quaternion.h>
#include <algorithm>
namespace SEditor::Core{

CameraCtrl::CameraCtrl(SGUI::Panels::WndPanel& view,SWnd::Context& wndcontext,SRender::LowRenderer::Camera& cam)
:wndcontext_(wndcontext),view_(view),cam_(cam){
    pos_ = glm::vec3(5, 5, 5);
	camcenter = glm::vec3(0, 0, 0);
    CalcLookAt();
}

void CameraCtrl::HandleInputs(float delta_time){
    if(view_.hovered_||inputmanager_.first_mouse_pressed_){
        inputmanager_.UpdateMouse();
        HandleZoom();
    }else{

    }
    //std::cout << inputmanager_.curpos_.first << " " << inputmanager_.curpos_.second <<"     < \r";
    if (is_fps_cam_mod_){
        HandleFpsCamCtl(delta_time);
    }else {
        HandleOrbitCamCtl(delta_time);
    }

    
}

void CameraCtrl::HandleOrbitCamCtl(float delta_time){
    if (inputmanager_.mid_btn_){
        auto[dx,dy] = inputmanager_.GetCursorDelta();
        if (inputmanager_.GetGlfwKeyState(GLFW_KEY_LEFT_SHIFT)){
            auto[w,h] = view_.canvas_size_;
            float apix = CalcDisPerPix(w,h);
            translate(glm::vec3(-dx*apix,dy*apix,0));
        }else{
            Orbit(-dx*around_speed_,-dy*around_speed_);
        }
    }
}

void CameraCtrl::HandleFpsCamCtl(float delta_time){
    if (inputmanager_.mid_btn_){
        auto step = move_speed_*delta_time;
        auto[dx,dy] = inputmanager_.GetCursorDelta();
        if (inputmanager_.GetGlfwKeyState(GLFW_KEY_W)){
            translate(sm::OglCamPrimForward*step);
        }
        if (inputmanager_.GetGlfwKeyState(GLFW_KEY_A)){
            translate(-sm::OglCamPrimRight*step);
        }
        if (inputmanager_.GetGlfwKeyState(GLFW_KEY_S)){
            translate(-sm::OglCamPrimForward*step);
        }
        if (inputmanager_.GetGlfwKeyState(GLFW_KEY_D)){
            translate(sm::OglCamPrimRight*step);
        }
        
        FpsRotate(-dx*around_speed_,-dy*around_speed_);
    }
}

void CameraCtrl::HandleZoom(){
    ImGuiIO& io = ImGui::GetIO();
    if (io.MouseWheel){
        zoom(io.MouseWheel/abs(io.MouseWheel)* -zoom_speed_);
    }
    
}

void CameraCtrl::reset() {
	pos_ = glm::vec3(5, 5, 5);
	camcenter = glm::vec3(0, 0, 0);
	worldup = glm::vec3(0, 0, 1);
}

void CameraCtrl::resetworldup() {
	// glm::vec3 tmp = glm::cross(camright, camcenter - pos_)*worldup;
	// float flag = tmp[0] + tmp[1] + tmp[2];
	// if ( flag<0) {
	// 	worldup[1] *= -1;
	// }
}

void CameraCtrl::translate(glm::vec3 trans) {
	trans=orien_*trans;
	pos_ = trans+pos_;
	camcenter = trans + camcenter;
}

void CameraCtrl::zoom(float ratio) {
	glm::vec3 dir = pos_ - camcenter;
	pos_ = camcenter + dir*(ratio + 1);
}

void CameraCtrl::Orbit(float hori_deg,float verti_deg) {
	euler_xyz_deg_.z+=hori_deg;
	euler_xyz_deg_.x+=verti_deg;
	orien_ = sm::Eul2Quat(euler_xyz_deg_);
    pos_ =camcenter - (orien_*sm::OglCamPrimForward)*glm::length(camcenter-pos_);
}

void CameraCtrl::CalcLookAt(){
    orien_ = sm::LookAt(worldup,camcenter-pos_);
    euler_xyz_deg_ = sm::Quat2Eul(orien_);
}

float CameraCtrl::CalcDisPerPix(int w,int h){
	float dis = glm::distance(camcenter, pos_);
	return dis*tan(cam_.fov_ / 2 * SM_PI / 180)*2/h;
}

void CameraCtrl::FpsRotate(float hori_deg,float verti_deg){
    // float sinh = sin(glm::radians(hori_deg)*0.5f);
    // float sinv = sin(glm::radians(verti_deg)*0.5f);
    // glm::vec3 curup = orien_*sm::OglCamPrimUp;
    // orien_*=glm::quat(cos(glm::radians(hori_deg)*0.5f),sinh*curup);
    // glm::vec3 curright = orien_*sm::OglCamPrimRight;
    // orien_*=glm::quat(cos(glm::radians(verti_deg)*0.5f),sinv*curright);
	// euler_xyz_deg_ = sm::Quat2Eul(orien_);
	// camcenter = pos_ + glm::length(camcenter-pos_)*(orien_*sm::OglCamPrimForward);
	
	euler_xyz_deg_.z+=hori_deg;
	euler_xyz_deg_.x=std::clamp(euler_xyz_deg_.x+verti_deg,0.0f,180.0f);
	orien_ = sm::Eul2Quat(euler_xyz_deg_);
	camcenter = pos_ + glm::length(camcenter-pos_)*(orien_*sm::OglCamPrimForward);
    
}



}