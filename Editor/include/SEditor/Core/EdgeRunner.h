#pragma once
#include<SEditor/Core/RuntimeContext.h>
#include<SRender/Buffers/GLFrameBuffer.h>
#include<SEditor/Core/Camera.h>

namespace SEngine{

class EdgeRunner{
public:
    EdgeRunner(SRender::Buffers::GLFrameBuffer& frm_buf,SEditor::Core::Camera& view_cam):frm_buf_(frm_buf),view_cam_(view_cam){};
    ~EdgeRunner(){};
    void LogicTick(float deltaT);
    void RenderTick(float deltaT);
private:
    SRender::Buffers::GLFrameBuffer& frm_buf_;
    SEditor::Core::Camera& view_cam_;

};

}