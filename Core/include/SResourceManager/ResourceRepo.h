#pragma once
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <mutex>
namespace ResourceManager {
template <typename T>
class ResourceRepo{ 
public:
    ~ResourceRepo(){}
    T* operator[](const std::string& name){
        const std::lock_guard<std::mutex>lock(resource_mutex);
        if (auto tmp = resources_.find(name);tmp!=resources_.end()){
            return tmp->second.get();
        }
        return nullptr;
    };
    void Append(const std::string& name,std::unique_ptr<T>& value){
        const std::lock_guard<std::mutex>lock(resource_mutex);
        if (auto tmp = resources_.find(name);tmp==resources_.end()){
            resources_[name]=std::move(value);
        }else{

        }
    };
    void Remove(const std::string& name){
        const std::lock_guard<std::mutex>lock(resource_mutex);
        resources_.erase(name);
    };
    void ClearAllResources(){
        const std::lock_guard<std::mutex>lock(resource_mutex);
        resources_.clear();
    };
    std::unordered_map<std::string, std::unique_ptr<T>> resources_;
private:
    std::mutex resource_mutex;
    
};


}