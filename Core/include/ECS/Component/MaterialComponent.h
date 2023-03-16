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
    virtual DrawCmd GetInspectorDrawCmd() override;
    virtual void DrawInspector() override;

    SRender::Resources::SMaterial* GetMaterial(){return material_;}
private:
    SRender::Resources::SMaterial* material_;

};  

}