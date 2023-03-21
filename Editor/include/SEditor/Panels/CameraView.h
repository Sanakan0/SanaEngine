#pragma once
#include "ECS/Actor.h"
#include "SceneSys/Scene.h"
#include<SRender/Buffers/GLFrameBuffer.h>
#include<SEditor/Panels/SView.h>
#include<SEditor/Core/RuntimeContext.h>
namespace SEditor::Panels{

class CameraView:public SView{
public:
    CameraView(Core::RuntimeContext& rtcontext);
    void LogicTick(float deltat);
    void RenderTick(float deltat);
private:
    Core::RuntimeContext& rtcontext_;
    ECS::Actor* active_camera_actor_;
};

}