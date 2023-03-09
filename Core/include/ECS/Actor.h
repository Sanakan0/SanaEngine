#pragma once
#include "ECS/Component/Component.h"
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

class Actor{
public:
    Actor(int64_t id,const std::string& name);
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
        AddComponentEvent.Invoke(*components_[components_.size()-1].second);
    }


    const std::string& GetName()const{return name_;}
    void SetName(const std::string& name){ name_ = name;}
    int64_t GetID() const {return id_;}
    void SetID(int64_t id) {id_=id;}

    template<class T>
    T* GetComponent(const std::string& component_type_name ){
        for (auto& i:components_){
            if (component_type_name==i.first){
                return static_cast<T*>(i.second.get());
            }
        }       
        return nullptr;
    }
    
    SCore::Event<Components::Component&> AddComponentEvent;
private:
    Actor(const Actor& p_actor) = delete;
    
    int64_t id_;
    std::string name_;
    
    std::vector<std::pair<std::string,std::shared_ptr<Components::Component>>> components_;
    

};

}
