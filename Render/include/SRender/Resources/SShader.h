//DEPRICATED
#pragma once
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/STexture.h"
#include <map>
#include <stdint.h>
#include <string>
#include <any>
namespace SRender::Resources{

// class SMaterial{
// public:
//     template<typename T>
//     void Set(const std::string& name,const T& value){
//         uniform_data_[name] = std::any(value);
//     }
// private:
//     GLShader* shaderp_;
//     std::map<std::string,std::any> uniform_data_;
    
// };


struct BlinnPhongMaterial{
    STexture* diffuse_tex;
    uint32_t dif_pos;
    STexture* specular_tex;
    uint32_t spec_pos;
};

class SShader{
public:
    SShader(GLShader* shaderp);
    ~SShader() = default;
    template<typename T>
    void Set(const std::string& name,const T& value){
        uniform_data_[name] = std::any(value);
    }
    void Bind();
    void Unbind();

private:
    GLShader* shaderp_;
    std::map<std::string,std::any> uniform_data_;
    
};




}