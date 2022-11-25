#pragma once
#include "SRender/Resources/GLShader.h"
#include <map>
#include <string>
#include <any>
namespace SRender::Resources{

class SMaterial{
public:
    template<typename T>
    void Set(const std::string& name,const T& value){
        uniform_data_[name] = std::any(value);
    }
private:
    GLShader* shaderp_;
    std::map<std::string,std::any> uniform_data_;
    
};




}