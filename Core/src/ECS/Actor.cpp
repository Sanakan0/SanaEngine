#include <ECS/Actor.h>

namespace ECS {

Actor::Actor(int64_t id,const std::string& name):id_(id),name_(name){
    
}

Actor::~Actor(){}

void Actor::Tick(float delta_t){
    for (auto& component:components_){
        component.second->Tick(delta_t);
    }
}

}