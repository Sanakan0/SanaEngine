#pragma once
#include "SMath/Transform.h"
#include <SRender/Buffers/GLFrameBuffer.h>
#include <string>
#include <SGUI/Panels/WndPanel.h>
#include <imgui/imgui.h>
#include <SEditor/Core/CameraCtrl.h>
#include <SRender/Buffers/GLUniformBuffer.h>

namespace SEditor::Panels{

class SView:public SGUI::Panels::WndPanel{
public:
    SView();
    void FillUBO();
    void UpdateViewCam(float deltat);
    void DrawContent() override;
    SRender::LowRenderer::Camera cam_;
    sm::Transform cam_extrinsic_;
    Core::CameraCtrl camctrl_;
    SRender::Buffers::GLFrameBuffer fbo_; 
    bool has_cursor_=false;
    bool cursor_selecting=false;
    bool enable_normal=false;
    bool enable_line = false;
};

}