#pragma once
#include <ECS/Component/Component.h>
namespace ECS::Components {

class MeshComponent:public Component{
public:
    virtual void Tick(float delta_t) override;

};

}