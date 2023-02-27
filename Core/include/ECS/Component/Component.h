#pragma once
#include <string>
namespace ECS::Components {

class Component{
public:
    Component();
    virtual ~Component();

    virtual void Tick(float delta_t);

};

}