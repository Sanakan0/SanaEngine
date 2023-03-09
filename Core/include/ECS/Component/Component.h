#pragma once
#include <functional>
#include <string>

namespace ECS{
class Actor;
}

namespace ECS::Components {
using DrawCmd = std::function<void()>;
class Component{
public:
    Component(Actor& parentactor);
    virtual ~Component();

    virtual void Tick(float delta_t);

    //Draw IMGUI for interact
    virtual DrawCmd GetInspectorDrawCmd()=0;
    virtual void DrawInspector(){};
    Actor& parentactor_;
    const std::string type_name_;
};

}