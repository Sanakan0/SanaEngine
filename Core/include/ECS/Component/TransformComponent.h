#pragma once
#include "SMath/Transform.h"
#include <ECS/Component/Component.h>
namespace ECS::Components {

class TransformComponent:public Component{
public:
    TransformComponent(Actor& parentactor);
    virtual ~TransformComponent() override{
        
    } 
    virtual void Tick(float delta_t) override;

    void GetTransFromBounds();
    
    void DrawInspector() override;
    const glm::mat4& GetMat(){return trans_.GetMat();}
    
    virtual void Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

    virtual void Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;



    glm::vec3 euler_xyz_deg_;
    sm::Transform trans_;
};

}