#pragma once
#include <ECS/Component/Component.h>
namespace ECS::Components {

class CMesh:public Component{
public:
    virtual void Tick(float delta_t) override;

};

}