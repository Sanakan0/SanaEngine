#pragma once
#include "ECS/Component/Component.h"
#include "Eventing/Event.h"
#include <cstddef>
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>
namespace ECS {

class Actor{
public:
    Actor(int64_t id,const std::string& name);
    ~Actor();

    void Tick(float delta_t);

    template<class T,typename ... ArgsType>
    void AddComponent(ArgsType&&... args);

    const std::string& GetName()const{return name_;}
    void SetName(const std::string& name){ name_ = name;}
    int64_t GetID() const {return id_;}
    void SetID(int64_t id) {id_=id;}

    template<class T>
    T* GetComponent(){
        T* tmp;
        for (auto& i:components_){
            tmp = dynamic_cast<T*>(i.get());
            if(tmp){
                return tmp;
            }
        }       
        return nullptr;
    }
    
private:
    Actor(const Actor& p_actor) = delete;
    
    int64_t id_;
    std::string name_;
    
    std::vector<std::shared_ptr<Components::Component>> components_;
    SCore::Event<Components::Component&> AddComponentEvent;

};

}
#include "ECS/Actor.inl"