#pragma once
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <ECS/Component/Component.h>
#include <memory>
namespace ECS::Components {

class DatasetGenComponent:public Component{

public:
    DatasetGenComponent(Actor& parentactor);
    virtual void Tick(float delta_t) override;

    virtual void DrawInspector() override;

    virtual void OnDrawGizmo(float delta_t) override;

    virtual void Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

    virtual void Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

private:
    glm::vec3 extends_{10,10,10};
    float step_=5;
    float fovy_=1;
    float aspect_=1;
    
};


}