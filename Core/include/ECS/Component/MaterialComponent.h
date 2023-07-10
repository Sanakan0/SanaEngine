#pragma once
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SMaterial.h"
#include "SRender/Resources/SModel.h"
#include <ECS/Component/Component.h>
namespace ECS::Components {

class MaterialComponent:public Component{
public:
    MaterialComponent(Actor& parentactor,SRender::Resources::SMaterial* material);
    virtual void Tick(float delta_t) override;

    virtual void DrawInspector() override;

    SRender::Resources::SMaterial* GetMaterial(){return material_;}

    virtual void Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

    virtual void Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;


private:
    SRender::Resources::SMaterial* material_;

};  

}