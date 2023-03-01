#pragma once
#include "SMath/Transform.h"
#include <ECS/Component/Component.h>
namespace ECS::Components {

class TransformComponent:public Component{
public:
    TransformComponent();
    virtual void Tick(float delta_t) override;
private:
    sm::Transform trans_;
};

}