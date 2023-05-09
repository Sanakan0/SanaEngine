#include "SRender/Resources/GLShader.h"
#include <iostream>
namespace SRender::Resources{

GLShader::GLShader(const std::string path,uint32_t programid):id_(programid),path_(path){
    //GetUniforms();
}

GLShader::~GLShader(){
    glDeleteProgram(id_);
}

void GLShader::SetUniInt(const std::string& name, int val){
    Bind();
    glUniform1i(GetUniformLocation(name),val);
}

void GLShader::SetUniFloat(const std::string& name, float val){
    Bind();
    glUniform1f(GetUniformLocation(name),val);
}

void GLShader::SetUniFloatV(const std::string &name, float* arr,int len){
    Bind();
    glUniform1fv(GetUniformLocation(name),len,arr);
}

void GLShader::SetUniVec2(const std::string& name, glm::vec2 val){
    Bind();
    glUniform2f(GetUniformLocation(name),val.x,val.y);
}

void GLShader::SetUniVec3(const std::string& name, glm::vec3 val){
    Bind();
    glUniform3f(GetUniformLocation(name),val.x,val.y,val.z);
}

void GLShader::SetUniVec4(const std::string& name, glm::vec4 val){
    Bind();
    glUniform4f(GetUniformLocation(name),val.x,val.y,val.z,val.w);
}

void GLShader::SetUniMat4(const std::string& name, glm::mat4 val){
    Bind();
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

void GLShader::GetUniforms(){
    Bind();
    GLint active_uni_cnt=0;
    glGetProgramiv(id_,GL_ACTIVE_UNIFORMS,&active_uni_cnt);
    std::vector<GLchar> nameofuni(256);
    std::cout <<"shader >"<< id_<<std::endl;
    for (int i=0;i<active_uni_cnt;++i){
        GLsizei lengthofname;
        GLint size;
        GLenum type;
        glGetActiveUniform(id_,i,static_cast<GLsizei>(nameofuni.size()),&lengthofname,&size,&type,nameofuni.data());
        std::string name(nameofuni.data(),lengthofname);
        std::cout << name << std::endl;
    }
}

}