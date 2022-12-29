#pragma once
#include <cstddef>
#include <unordered_map>
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
        if ((*this)[name]==nullptr){
            resources_[name]=value;
        }else{
            delete value;
        }
    };
    void Remove(const std::string& name){
        resources_.erase(name);
    };
    void ClearAllResources(){
        for (auto&[key,value]:resources_){
            delete value;
        }
        resources_.clear();
    };
private:
 
    std::unordered_map<std::string, T*> resources_;
};


}