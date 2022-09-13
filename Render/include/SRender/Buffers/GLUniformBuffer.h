#pragma once
#include <glad/glad.h>
#include <SRender/Resources/GLShader.h>
namespace SRender::Buffers{

class GLUniformBuffer{
public:
    GLUniformBuffer(size_t sz,int bindingpoint=0,GLenum usage = GL_STATIC_DRAW);
    ~GLUniformBuffer();
    void Bind();
    void Unbind();
    void BindShaderUniBlock(Resources::GLShader& shader,const std::string& blockname,int bindingpoint);

    template <typename T>
    void BufferSubData(const T& data,size_t offset){
        Bind();
        glBufferSubData(GL_UNIFORM_BUFFER,offset,sizeof(T),&data);
        Unbind();
    }

    unsigned int GetID() const;
    unsigned int GetBlockLoc(const std::string name,Resources::GLShader& shader);
    unsigned int ubo_id_;
};

}