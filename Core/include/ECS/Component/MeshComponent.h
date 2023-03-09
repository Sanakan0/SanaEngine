#pragma once
#include "SRender/Resources/SModel.h"
#include <ECS/Component/Component.h>
namespace ECS::Components {

class MeshComponent:public Component{
public:
    MeshComponent(Actor& parentactor,SRender::Resources::SModel* model);
    virtual void Tick(float delta_t) override;
    SRender::Resources::SModel* GetModel(){return model_;}
    virtual DrawCmd GetInspectorDrawCmd() override;
    virtual void DrawInspector() override;
private:
    SRender::Resources::SModel* model_;
    

};  

}