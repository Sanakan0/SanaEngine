#include <ECS/Component/Component.h>

namespace ECS::Components {

Component::Component(Actor& parentactor):parentactor_(parentactor){}

Component::~Component(){}

void Component::Tick(float delta_t){}

}