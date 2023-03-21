#pragma once
#include "SRender/LowRenderer/Camera.h"
#include <ECS/Component/Component.h>
namespace ECS::Components {

class CameraComponent:public Component{
public:
    CameraComponent(Actor& parentactor);
    virtual void Tick(float delta_t) override;
    virtual DrawCmd GetInspectorDrawCmd() override;
    virtual void DrawInspector() override;
    
    
    SRender::LowRenderer::Camera cam_;

private:
    
};

}