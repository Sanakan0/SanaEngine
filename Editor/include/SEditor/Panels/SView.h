#pragma once
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
    Core::CameraCtrl camctrl_;
    SRender::LowRenderer::Camera cam_;
    SRender::Buffers::GLFrameBuffer fbo_; 
};

}