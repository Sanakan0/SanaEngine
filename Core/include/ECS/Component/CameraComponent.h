#pragma once
#include <ECS/Component/Component.h>
namespace ECS::Components {

class CameraComponent:public Component{
public:
    virtual void Tick(float delta_t) override;


private:
    //SEditor::Core::Camera
};

}