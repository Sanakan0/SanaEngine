#pragma once
#include "Serialize/ISerializable.h"
#include <functional>
#include <string>
namespace ECS{
class Actor;
}

namespace ECS::Components {
using DrawCmd = std::function<void()>;
class Component: public SCore::ISerializable{
public:
    Component(Actor& parentactor);
    virtual ~Component();

    virtual void Tick(float delta_t);

    //Draw IMGUI for interact
    DrawCmd GetInspectorDrawCmd(){
        return std::bind(&Component::DrawInspector,this);
    }
    virtual void DrawInspector(){};
    Actor& parentactor_;
    const std::string type_name_;
};

}