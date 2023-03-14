#pragma once
#include "ECS/Component/Component.h"
#include "ECS/Component/TransformComponent.h"
#include "Eventing/Event.h"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <memory>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>
namespace ECS {
using ActorID=uint64_t;
class Actor{
public:
    Actor(ActorID id,const std::string& name);
    ~Actor();

    void Tick(float delta_t);

    template<class T,typename ... ArgsType>
    void AddComponent(ArgsType&&... args){
        static_assert(std::is_base_of<Components::Component, T>::value, "T should derive from Component");
        //std::make_pair(typeid(T).name(), std::make_shared<T>(*this,std::forward<ArgsType>(args)...)) 
        std::string tmpname=typeid(T).name();
        auto n = tmpname.rfind("::");
        components_.push_back(
            std::make_pair(tmpname.substr(n+2), std::make_shared<T>(*this,std::forward<ArgsType>(args)...)) 
        );
        if (components_[components_.size()-1].first=="TransformComponent"){
            transform_=static_cast<Components::TransformComponent*>(
                components_[components_.size()-1].second.get());
        }
        AddComponentEvent.Invoke(*components_[components_.size()-1].second);
    }


    const std::string& GetName()const{return name_;}
    void SetName(const std::string& name){ name_ = name;}
    ActorID GetID() const {return id_;}
    void SetID(ActorID id) {id_=id;}

    
    Components::Component* GetComponent(const std::string& component_type_name ){
        for (auto& i:components_){
            if (component_type_name==i.first){
                return (i.second.get());
            }
        }       
        return nullptr;
    }
    //Draw IMGUI for interact
    Components::TransformComponent* GetTransformComponent(){return transform_;}

    SCore::Event<Components::Component&> AddComponentEvent;

    
private:
    Actor(const Actor& p_actor) = delete;
    
    ActorID id_;
    std::string name_;
    
    std::vector<std::pair<std::string,std::shared_ptr<Components::Component>>> components_;
    Components::TransformComponent* transform_=nullptr;
    

};

}
