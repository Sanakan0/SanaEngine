#pragma once
#include<SRender/Buffers/GLFrameBuffer.h>
#include<SEditor/Panels/SView.h>
#include<SEditor/Core/RuntimeContext.h>
namespace SEditor::Panels{

class SceneView:public SView{
public:
    SceneView(Core::RuntimeContext& rtcontext);
    void LogicTick(float deltat);
    void RenderTick(float deltat);
private:
    Core::RuntimeContext& rtcontext_;
};

}