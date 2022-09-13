#include "SRender/Resources/GLShader.h"

namespace SRender::Resources{

GLShader::GLShader(const std::string path,uint32_t programid):path_(path),id_(programid){
}

GLShader::~GLShader(){
    glDeleteProgram(id_);
}

void GLShader::SetUniInt(const std::string& name, int val){
    glUniform1i(GetUniformLocation(name),val);
}

void GLShader::SetUniFloat(const std::string& name, float val){
    glUniform1f(GetUniformLocation(name),val);
}

void GLShader::SetUniVec2(const std::string& name, glm::vec2 val){
    glUniform2f(GetUniformLocation(name),val.x,val.y);
}

void GLShader::SetUniVec3(const std::string& name, glm::vec3 val){
    glUniform3f(GetUniformLocation(name),val.x,val.y,val.z);
}

void GLShader::SetUniVec4(const std::string& name, glm::vec4 val){
    glUniform4f(GetUniformLocation(name),val.x,val.y,val.z,val.w);
}

void GLShader::SetUniMat4(const std::string& name, glm::mat4 val){
    glUniformMatrix4fv(GetUniformLocation(name),1,GL_FALSE,&val[0].x);
}

int GLShader::GetUniformLocation(const std::string& name){
    // TODO:: A Uniform cache
    return glGetUniformLocation(id_,name.c_str());
}

void GLShader::Bind(){
    glUseProgram(id_);
}

void GLShader::Unbind(){
    glUseProgram(0);
}

unsigned int GLShader::GetID() const{
    return id_;
}


}