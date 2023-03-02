#pragma once
#include "SRender/Resources/SModel.h"
#include <ECS/Component/Component.h>
namespace ECS::Components {

class MeshComponent:public Component{
public:
    MeshComponent(Actor& parentactor);
    virtual void Tick(float delta_t) override;


private:
    SRender::Resources::SModel* model_;
    

};  

}