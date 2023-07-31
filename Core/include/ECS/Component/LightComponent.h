#pragma once
#include <ECS/Component/Component.h>
#include <memory>
namespace ECS::Components {

class LightComponent:public Component{

public:
    LightComponent(Actor& parentactor);
    virtual void Tick(float delta_t) override;

    virtual void DrawInspector() override;

    virtual void Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

    virtual void Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;


    
};


}