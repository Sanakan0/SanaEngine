#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SGUI/Panels/WndPanel.h>
#include <SEditor/Core/Camera.h>
#include <SWnd/context/context.h>
#include <SWnd/Input/InputManager.h>
namespace SEditor::Core{

class CameraCtrl{
public:
    CameraCtrl(SGUI::Panels::WndPanel& view,SWnd::Context& wndcontext,Core::Camera& cam);
    
    void HandleInputs(float delta_time);
    void HandleZoom();
    void HandleOrbitCamCtl(float delta_time);
    void HandleFpsCamCtl(float delta_time);
    void SetPos();
    void SetTowards();
    SGUI::Panels::WndPanel& view_;
    SWnd::Context& wndcontext_;
    SWnd::Input::InputManager inputmanager_;
    Camera& cam_;
    
private:
    int mid_btn_=0;
    int left_btn_=0;
    int right_btn_=0;
    float around_speed_=0.4f;
    float zoom_speed_=0.05f;
    float move_speed_=1.0f;//  meter per second
};

}