#pragma once
#include "SMath/STransform.h"
#include <ECS/Component/Component.h>
namespace ECS::Components {

class TransformComponent:public Component{
public:
    TransformComponent();
    virtual void Tick(float delta_t) override;
private:
    sm::STransform trans_;
};

}