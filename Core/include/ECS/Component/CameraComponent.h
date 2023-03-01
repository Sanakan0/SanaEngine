#pragma once
#include "SRender/LowRenderer/Camera.h"
#include <ECS/Component/Component.h>
namespace ECS::Components {

class CameraComponent:public Component{
public:
    virtual void Tick(float delta_t) override;


private:
    SRender::LowRenderer::Camera cam_;
};

}