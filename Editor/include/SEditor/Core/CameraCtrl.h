#pragma once
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
    CameraCtrl(SGUI::Panels::WndPanel& view,SWnd::Context& wndcontext,SRender::LowRenderer::Camera& cam);
    
    void HandleInputs(float delta_time);
    void HandleZoom();
    void HandleOrbitCamCtl(float delta_time);
    void HandleFpsCamCtl(float delta_time);
    void SetPos();
    void SetTowards();
    const glm::vec3& GetPos() const{return pos_;}
    const glm::quat& GetOrien() const{return orien_;} 

    void translate(glm::vec3 trans);
	
	void Orbit(float hori_deg,float verti_deg );
	void CalcLookAt(); //call to initialize
	void FpsRotate(float hori_deg,float verti_deg);
	void zoom(float step);
	void reset();
	void resetworldup();
	float CalcDisPerPix(int w,int h);
    
    SWnd::Context& wndcontext_;
    SGUI::Panels::WndPanel& view_;
    SWnd::Input::InputManager inputmanager_;
    SRender::LowRenderer::Camera& cam_;
    
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

	
	glm::vec3 pos_;
	glm::quat orien_;
	glm::vec3 euler_xyz_deg_;
};

}