#pragma once
#include "SRender/Resources/SMesh.h"
#include<SRender/Buffers/GLFrameBuffer.h>
#include<SEditor/Panels/SView.h>
#include<SEditor/Core/RuntimeContext.h>
namespace SEditor::Panels{
class TestView:public SView{
public:
    TestView(Core::RuntimeContext& rtcontext);
    ~TestView();
    void LogicTick(float deltat);
    void RenderTick(float deltat);
private:   
    Core::RuntimeContext& rtcontext_;
    std::unique_ptr<SRender::Resources::GLShader>  Dshaderp;
    std::unique_ptr<SRender::Resources::SMesh> trimeshp;
};


}