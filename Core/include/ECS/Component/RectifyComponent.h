#pragma once
#include "SRender/Resources/STexture.h"
#include <ECS/Component/Component.h>
#include <memory>
namespace ECS::Components {

class RectifyComponent:public Component{

public:
    RectifyComponent(Actor& parentactor);
    virtual void Tick(float delta_t) override;

    virtual void DrawInspector() override;

    virtual void Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

    virtual void Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

    std::vector<std::vector<std::pair<double,double>>>lines;
    std::unique_ptr<SRender::Resources::STexture> uimgp;
    std::string img_pth_="";
    double lossval_=-1.0;
};


}