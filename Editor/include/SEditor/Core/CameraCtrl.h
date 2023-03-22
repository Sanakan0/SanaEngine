#pragma once
#include "SMath/Transform.h"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SGUI/Panels/WndPanel.h>
#include <SRender/LowRenderer/Camera.h>
#include <SWnd/context/context.h>
#include <SWnd/Input/InputManager.h>
namespace SEditor::Core{

class CameraCtrl{
public:
    CameraCtrl(SGUI::Panels::WndPanel& view,SWnd::Context& wndcontext,SRender::LowRenderer::Camera& cam,sm::Transform& extrinsic);
    
    void HandleInputs(float delta_time);
    void HandleZoom();
    void HandleOrbitCamCtl(float delta_time);
    void HandleFpsCamCtl(float delta_time);
    void SetCamInExParam(SRender::LowRenderer::Camera& intrinsic,sm::Transform& extrinsic);
    const glm::vec3& GetPos() const{return extrinsic_->world_pos_;}
    const glm::quat& GetOrien() const{return extrinsic_->world_orien_;} 

    void translate(glm::vec3 trans);
	
	void Orbit(float hori_deg,float verti_deg );
	void CalcLookAt(); //call to initialize
    void Move2Target(glm::vec3 targetpos,float dis);
	void FpsRotate(float hori_deg,float verti_deg);
	void zoom(float step);
	void reset();
	void resetworldup();
	float CalcDisPerPix(int w,int h);
    void TickCamMove(float deltat);
    SWnd::Context& wndcontext_;
    SGUI::Panels::WndPanel& view_;
    SWnd::Input::InputManager inputmanager_;
    SRender::LowRenderer::Camera* cam_;
    sm::Transform* extrinsic_;
    bool is_fps_cam_mod_=1;
private:
    int mid_btn_=0;
    int left_btn_=0;
    int right_btn_=0;
    float around_speed_=0.4f;
    float zoom_speed_=0.05f;
    float move_speed_=1.0f;//  meter per second

//CAM extrinsic 
    glm::vec3 camcenter;
	glm::vec3 worldup{0, 0, 1};
	glm::vec3 euler_xyz_deg_;

//move to data
    glm::vec3 mv2pos_;
    glm::quat mv2orien_;
    float anima_time_inv_=2; //second
    float anima_process_normalized_=0; //[0,1]
    bool anima_activate_=0;
};

}