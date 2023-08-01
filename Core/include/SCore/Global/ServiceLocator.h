#pragma once
#include <any>
#include <unordered_map> 
#include <iostream>
#include "Debug/Assertion.h"
#define SANASERVICE(type) SCore::Global::ServiceLocator::Get<type>()
namespace SCore::Global{

class ServiceLocator{
public:
    template<typename T>
    static inline void Provide(T& service){
        services_[typeid(T).hash_code()]=std::any(&service);
    }

    template<typename T>
    static inline T& Get(){
        auto tmpitr = services_.find(typeid(T).hash_code());
        SANA_ASSERT(tmpitr!=services_.end()&&"SANASERVICE NOT FOUND");
        return *std::any_cast<T*>(tmpitr->second);
    }

private:
    static std::unordered_map<std::size_t,std::any> services_;
};

}