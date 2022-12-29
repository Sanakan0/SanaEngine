#include "SRender/Resources/SShader.h"
#include "SRender/Resources/GLShader.h"

namespace SRender::Resources{

SShader::SShader(GLShader* shaderp):shaderp_(shaderp){}

void SShader::Bind(){
    if (shaderp_!=nullptr){
        shaderp_->Bind();
        for (auto&[k,v]:uniform_data_){
            
        }
    }
}


}