#pragma once
#include <cstddef>
#include <unordered_map>
#include <mutex>
namespace ResourceManager {
template <typename T>
class ResourceRepo{ 
public:
    T* operator[](const std::string& name){
        if (auto tmp = resources_.find(name);tmp!=resources_.end()){
            return tmp->second;
        }
        return nullptr;
    };
    void Append(const std::string& name,T* value){
        const std::lock_guard<std::mutex>lock(resource_mutex);
        if ((*this)[name]==nullptr){
            resources_[name]=value;
        }else{
            delete value;
        }
    };
    void Remove(const std::string& name){
        const std::lock_guard<std::mutex>lock(resource_mutex);
        resources_.erase(name);
    };
    void ClearAllResources(){
        for (auto&[key,value]:resources_){
            delete value;
        }
        resources_.clear();
    };
    std::unordered_map<std::string, T*> resources_;
private:
    std::mutex resource_mutex;
    
};


}