#pragma once
#include <cstddef>
#include <unordered_map>
#include <mutex>
namespace ResourceManager {
template <typename T>
class ResourceRepo{ 
public:
    ~ResourceRepo(){ClearAllResources();}
    T* operator[](const std::string& name){
        const std::lock_guard<std::mutex>lock(resource_mutex);
        if (auto tmp = resources_.find(name);tmp!=resources_.end()){
            return tmp->second;
        }
        return nullptr;
    };
    void Append(const std::string& name,T* value){
        const std::lock_guard<std::mutex>lock(resource_mutex);
        if (auto tmp = resources_.find(name);tmp==resources_.end()){
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
        const std::lock_guard<std::mutex>lock(resource_mutex);
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