#pragma once
#include <string>

namespace ECS{
class Actor;
}

namespace ECS::Components {

class Component{
public:
    Component(Actor& parentactor);
    virtual ~Component();

    virtual void Tick(float delta_t);

    Actor& parentactor_;
};

}