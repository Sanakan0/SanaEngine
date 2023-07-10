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
using ActorID=int64_t;
class Actor:public SCore::ISerializable{
public:
    Actor(ActorID id,const std::string& name);
    ~Actor();

    void Tick(float delta_t);

    template<class T,typename ... ArgsType>
    inline T& AddComponent(ArgsType&&... args){
        static_assert(std::is_base_of<Components::Component, T>::value, "T should derive from Component");
        //std::make_pair(typeid(T).name(), std::make_shared<T>(*this,std::forward<ArgsType>(args)...)) 
        std::string tmpname=typeid(T).name();
        auto n = tmpname.rfind("::");
        auto componentType = tmpname.substr(n+2);
        if (auto found = GetComponent(componentType);found!=nullptr){
            T& instance = *dynamic_cast<T*>(found);
            return instance;
        }
        components_.push_back(
            std::make_pair(componentType, std::make_shared<T>(*this,std::forward<ArgsType>(args)...)) 
        );
        if (components_[components_.size()-1].first=="TransformComponent"){
            transform_=static_cast<Components::TransformComponent*>(
                components_[components_.size()-1].second.get());
        }
        AddComponentEvent.Invoke(*components_[components_.size()-1].second);
        T& instance = *dynamic_cast<T*>(  components_.back().second.get());
        return instance;
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

    virtual void Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

    virtual void Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

    
private:
    Actor(const Actor& p_actor) = delete;
    
    ActorID id_;
    std::string name_;
    
    std::vector<std::pair<std::string,std::shared_ptr<Components::Component>>> components_;
    Components::TransformComponent* transform_=nullptr;
    

};

}
