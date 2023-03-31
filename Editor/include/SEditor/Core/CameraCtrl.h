#pragma once
#include "ECS/Actor.h"
#include "SMath/Transform.h"
#include "SRender/Buffers/GLUniformBuffer.h"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SGUI/Panels/WndPanel.h>
#include <SRender/LowRenderer/Camera.h>
#include <SWnd/context/context.h>
#include <SWnd/Input/InputManager.h>
namespace SEditor::Core{


class ACamera{
public:
    ACamera(SRender::LowRenderer::Camera& cam,sm::Transform& extrinsic);
    
    //strictly use camera para to fillubo (include aspect ratio)
    void CalcAndFillUBO(SRender::Buffers::GLUniformBuffer& ubo);
    void SetExtrinsic(const glm::quat& orien,const glm::vec3& pos);
    void FillUBO(SRender::Buffers::GLUniformBuffer& ubo);
    void InitFromActor(ECS::Actor& actor);
    void SetCamInExParam(SRender::LowRenderer::Camera& intrinsic,sm::Transform& extrinsic);
    void UpdateExtraParam();
    const glm::vec3& GetPos() const{return extrinsic_->world_pos_;}
    const glm::quat& GetOrien() const{return extrinsic_->world_orien_;} 
    const glm::vec3& GetCamcenter(){return camcenter;}
    
    void translate(glm::vec3 trans);
	void Orbit(float hori_deg,float verti_deg );
	void CalcLookAt(); //call to initialize
	void FpsRotate(float hori_deg,float verti_deg);
	void zoom(float step);
    SRender::LowRenderer::Camera* cam_;
    sm::Transform* extrinsic_;
   
protected:

//CAM extrinsic 
    float dist2center;
    glm::vec3 camcenter;
	glm::vec3 worldup{0, 0, 1};
	glm::vec3 euler_xyz_deg_;



};

class CameraCtrl:public ACamera{
public:
    CameraCtrl(SGUI::Panels::WndPanel& view,SWnd::Context& wndcontext,SRender::LowRenderer::Camera& cam,sm::Transform& extrinsic);
    void HandleInputs(float delta_time);
    void HandleZoom();
    void HandleOrbitCamCtl(float delta_time);
    void HandleFpsCamCtl(float delta_time);

    void Move2Target(glm::vec3 targetpos,float dis);

    float CalcDisPerPix(int w,int h);
    void TickCamMove(float deltat);

    void reset();
	void resetworldup();


    SWnd::Context& wndcontext_;
    SGUI::Panels::WndPanel& view_;
    SWnd::Input::InputManager inputmanager_;

    bool is_fps_cam_mod_=1;
   
private:
    int mid_btn_=0;
    int left_btn_=0;
    int right_btn_=0;
    float around_speed_=0.2f;
    float zoom_speed_=0.05f;
    float move_speed_=100.0f;//  meter per second



//move to data
    glm::vec3 mv2pos_;
    glm::quat mv2orien_;
    float anima_time_inv_=2; //second
    float anima_process_normalized_=0; //[0,1]
    bool anima_activate_=0;
};

}