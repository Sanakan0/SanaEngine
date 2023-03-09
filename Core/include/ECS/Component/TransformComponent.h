#pragma once
#include "SMath/Transform.h"
#include <ECS/Component/Component.h>
namespace ECS::Components {

class TransformComponent:public Component{
public:
    TransformComponent(Actor& parentactor);
    virtual void Tick(float delta_t) override;
    virtual DrawCmd GetInspectorDrawCmd() override;
    void DrawInspector() override;
    const glm::mat4& GetMat(){return trans_.GetMat();}
    
private:
    
    sm::Transform trans_;
};

}