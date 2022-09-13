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
    ~SView();
    void FillUBO();
    virtual void LogicTick(float deltat);
    virtual void RenderImpl() = 0;
    void RenderTick();
    Core::CameraCtrl camctrl_;
    Core::Cameraa cam_;
    
};

}