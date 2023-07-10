#pragma once
#include "SRender/Resources/SModel.h"
#include <ECS/Component/Component.h>
namespace ECS::Components {

class MeshComponent:public Component{
public:
    MeshComponent(Actor& parentactor);
    virtual ~MeshComponent() override{
        
    } 
    void SetModel(SRender::Resources::SModel* model);
    virtual void Tick(float delta_t) override;

    virtual void DrawInspector() override;

    SRender::Resources::SModel* GetModel(){return model_;}

    virtual void Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

    virtual void Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;



private:
    SRender::Resources::SModel* model_;
    

};  

}