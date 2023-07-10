#pragma once
#include "SRender/LowRenderer/Camera.h"
#include <ECS/Component/Component.h>
namespace ECS::Components {

class CameraComponent:public Component{
public:
    CameraComponent(Actor& parentactor);
    virtual ~CameraComponent() override{
        
    } 
    virtual void Tick(float delta_t) override;

    virtual void DrawInspector() override;
    
    
    SRender::LowRenderer::Camera cam_;

    virtual void Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

    virtual void Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;


private:
    
};

}