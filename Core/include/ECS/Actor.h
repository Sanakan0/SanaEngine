#pragma once
#include "ECS/Component/Component.h"
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


    
private:
    Actor(const Actor& p_actor) = delete;
    
    int64_t id_;
    std::string name_;
    
    std::vector<std::shared_ptr<Components::Component>> components_;


};

}
#include "ECS/Actor.inl"