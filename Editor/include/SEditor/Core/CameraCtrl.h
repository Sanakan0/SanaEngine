#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SGUI/Panels/WndPanel.h>
#include <SEditor/Core/Cameraa.h>
#include <SWnd/context/context.h>
#include <SWnd/Input/InputManager.h>
namespace SEditor::Core{

class CameraCtrl{
public:
    CameraCtrl(SGUI::Panels::WndPanel& view,SWnd::Context& wndcontext,Core::Cameraa& cam);
    
    void HandleInputs(float delta_time);
    void HandleZoom();
    void SetPos();
    void SetTowards();
    SGUI::Panels::WndPanel& view_;
    SWnd::Context& wndcontext_;
    SWnd::Input::InputManager inputmanager_;
    Cameraa& cam_;
    
private:
    int mid_btn_=0;
    int left_btn_=0;
    int right_btn_=0;
    float around_speed_=0.4f;
    float zoom_speed_=0.05f;
};

}