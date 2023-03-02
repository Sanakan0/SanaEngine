#pragma once
#include "ECS/Actor.h"
#include <memory>
#include <utility>

namespace ECS{

template<typename T, typename ...ArgsType>
void Actor::AddComponent(ArgsType&&... args){
    static_assert(std::is_base_of<Components::Component, T>::value, "T should derive from Component");
    components_.push_back(std::make_shared<T>(*this,std::forward<ArgsType>(args)...));
    AddComponentEvent.Invoke(*components_[components_.size()-1]);
}


}