#pragma once
#include "SceneSys/Scene.h"
#include<SRender/Buffers/GLFrameBuffer.h>
#include<SEditor/Panels/SView.h>
#include<SEditor/Core/RuntimeContext.h>
namespace SEditor::Panels{

class TestView:public SView{
public:
    TestView(Core::RuntimeContext& rtcontext);
    void LogicTick(float deltat);
    void RenderTick(float deltat);
private:
    Core::RuntimeContext& rtcontext_;
    
};

}